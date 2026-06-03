#!/usr/bin/env python3
import pathlib
import re
import matplotlib.pyplot as plt


def load_all_arrays_from_txt(path: pathlib.Path) -> list[list[int]]:
    """
    Parse a GDB output file containing multiple arrays like:
      $1 = {123, 456, 0 <repeats 12 times>, ...}
      $2 = {789, ...}
    Returns a list of arrays (one per match).
    """
    text = path.read_text()
    token_re = re.compile(r'(\d+)\s*<repeats\s+(\d+)\s+times>|(\d+)')
    arrays = []

    for match in re.finditer(r'\$\d+\s*=\s*\{(.+?)\}', text, re.DOTALL):
        raw = match.group(1)
        values = []
        for m in token_re.finditer(raw):
            if m.group(1) is not None:
                values.extend([int(m.group(1))] * int(m.group(2)))
            else:
                values.append(int(m.group(3)))
        if values:
            arrays.append(values)

    return arrays


def plot_all_and_mean(arrays: list[list[int]]):
    # Trim all arrays to the shortest length so mean is well-defined
    min_len = min(len(a) for a in arrays)
    trimmed = [a[:min_len] for a in arrays]

    mean = [sum(trimmed[i][j] for i in range(len(trimmed))) / len(trimmed)
            for j in range(min_len)]

    fig, ax = plt.subplots(figsize=(14, 5))

    for i, arr in enumerate(trimmed):
        ax.plot(arr, linewidth=0.4, alpha=0.5, label=f"Run {i+1}")

    ax.plot(mean, linewidth=1.5, color="red", label="Mean", zorder=10)

    ax.set_xlabel("Index")
    ax.set_ylabel("Value (cycles)")
    ax.set_title(f"All runs + mean ({len(arrays)} iterations)")
    ax.legend(loc="upper right", fontsize=7)
    fig.tight_layout()
    plt.show()


if __name__ == "__main__":
    path = pathlib.Path("result_analysis/data_output/spy_results2.txt")
    arrays = load_all_arrays_from_txt(path)
    print(f"Found {len(arrays)} arrays, lengths: {[len(a) for a in arrays]}")
    plot_all_and_mean(arrays)
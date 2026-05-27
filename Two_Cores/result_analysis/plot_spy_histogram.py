#!/usr/bin/env python3
import pathlib
import struct
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider

def load_numbers_from_bin(path: pathlib.Path) -> list[int]:
    data = path.read_bytes()
    if len(data) % 4 != 0:
        raise ValueError(f"{path} size is not divisible by 4 bytes")
    return [value for (value,) in struct.iter_unpack("<I", data)]


def color_for_value(value: int, treshold: int) -> str:
    if value < treshold:
        return "green"
    else:
        return "red"

def color_for_value(value: int, treshold: int) -> str:
    if value < treshold:
        return "green"
    else:
        return "red"

def plot_histogram(times, bins=50, title="Cache Timing Histogram"):
    """
    times: list of access times (e.g., in CPU cycles or ns)
    bins: number of buckets for the histogram
    """

    plt.figure()
   
    plt.scatter(range(len(times)), times)
    
    plt.xlabel("Access Time (cycles or ns)")
    plt.ylabel("Frequency")
    plt.title(title)
    
    plt.show()

if __name__ == "__main__":
   
    values = load_numbers_from_bin(pathlib.Path("result_analysis/data_output/spy_results.bin"))
    print(f"Loaded {len(values)} values")
    # values = [x for x in values if x < 2500 and x > 20]
    plot_histogram(values, bins= 50)
    # histogram_with_slider(values, bins= 50)
#!/usr/bin/env bash
# run_experiments.sh — runs the flush-reload experiment N times
# Usage: ./run_experiments.sh <n>

set -euo pipefail

EXPERIMENT=ex_6_flush_reload
OUTPUT=result_analysis/data_output/spy_results2.txt
LOGS_DIR=result_analysis/logs
OCD_READY_SLEEP=10   # seconds to wait for OpenOCD to be ready

if [[ $# -ne 1 || ! $1 =~ ^[0-9]+$ ]]; then
    echo "Usage: $0 <n>"
    exit 1
fi

N=$1
mkdir -p "$LOGS_DIR"
mkdir -p "$(dirname "$OUTPUT")"

# Clear output file at the start
> "$OUTPUT"

for ((i = 1; i <= N; i++)); do
    echo "=== Iteration $i / $N ==="

    # --- clean & build ---
    echo "  [1/4] Cleaning..."
    make EXPERIMENT=$EXPERIMENT clean > "$LOGS_DIR/clean_$i.log" 2>&1

    echo "  [2/4] Building..."
    make EXPERIMENT=$EXPERIMENT       > "$LOGS_DIR/build_$i.log" 2>&1

    echo "  [3/4] Flashing..."
    make EXPERIMENT=$EXPERIMENT flash > "$LOGS_DIR/flash_$i.log" 2>&1

    # --- start OpenOCD in the background (the "terminal 1" process) ---
    echo "  [4/4] Starting OpenOCD..."
    make EXPERIMENT=$EXPERIMENT ocd   > "$LOGS_DIR/ocd_$i.log" 2>&1 &
    OCD_PID=$!

    echo "        Waiting ${OCD_READY_SLEEP}s for OpenOCD to be ready..."
    sleep "$OCD_READY_SLEEP"

    # --- dump (the "terminal 2" step) ---
    echo "        Running dump_foo..."
    make dump_foo >> "$OUTPUT" 2>> "$LOGS_DIR/dump_$i.log"

    # --- kill OpenOCD ---
    echo "        Killing OpenOCD (PID $OCD_PID)..."
    kill "$OCD_PID" 2>/dev/null && wait "$OCD_PID" 2>/dev/null || true

    echo "  Done."
done

echo ""
echo "All $N iterations complete. Results appended to $OUTPUT"
#!/bin/bash
# Benchmark script for MapReduce OS Project
# Measures execution time and memory for 1, 2, 4, and 8 workers

RESULTS_DIR="./results"
mkdir -p $RESULTS_DIR
OUTFILE="$RESULTS_DIR/benchmark_results.txt"

echo "Benchmark Results - $(date)" > $OUTFILE
echo "" >> $OUTFILE

# Part 1: Sorting performance
echo "Part 1: Parallel Sorting Performance" >> $OUTFILE
for workers in 1 2 4 8
do
    echo "Workers: $workers" >> $OUTFILE
    echo "Threading Sort" >> $OUTFILE
    /usr/bin/time -v ./part1_sorting/threading_sort $workers 131072 2>> $OUTFILE
    echo "Multiprocessing Sort" >> $OUTFILE
    /usr/bin/time -v ./part1_sorting/multiprocessing_sort $workers 131072 2>> $OUTFILE
    echo "" >> $OUTFILE
done

# Part 2: Synchronization performance
echo "Part 2: Max Value Aggregation Performance" >> $OUTFILE
for workers in 1 2 4 8
do
    echo "Workers: $workers" >> $OUTFILE
    echo "Threading Max" >> $OUTFILE
    /usr/bin/time -v ./part2_max_value/threading_max $workers 131072 2>> $OUTFILE
    echo "Multiprocessing Max" >> $OUTFILE
    /usr/bin/time -v ./part2_max_value/multiprocessing_max $workers 131072 2>> $OUTFILE
    echo "" >> $OUTFILE
done

echo "Benchmarking complete. Results saved to $OUTFILE"

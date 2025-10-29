#!/bin/bash
# MapReduce OS Project - Measure performance for required configurations

RESULTS_DIR="./results"
mkdir -p $RESULTS_DIR
OUTFILE="$RESULTS_DIR/performance_results.txt"

echo "MapReduce OS Project Performance Report - $(date)" > $OUTFILE
echo "" >> $OUTFILE

# Compile all programs
echo "Compiling all programs..."
gcc -Wall -O2 -pthread part1_sorting/threading_sort.c -o part1_sorting/threading_sort
gcc -Wall -O2 part1_sorting/multiprocessing_sort.c -o part1_sorting/multiprocessing_sort
gcc -Wall -O2 -pthread part2_max_value/threading_max.c -o part2_max_value/threading_max
gcc -Wall -O2 part2_max_value/multiprocessing_max.c -o part2_max_value/multiprocessing_max -lrt
echo "Compilation complete."
echo "" >> $OUTFILE

# Part 1: Parallel Sorting (Map Phase)
echo "Part 1: Parallel Sorting (Map Phase Performance)" >> $OUTFILE
for size in 32 131072
do
    echo "Array Size: $size" >> $OUTFILE
    for workers in 1 2 4 8
    do
        echo "Workers: $workers" >> $OUTFILE
        echo "Threading Sort" >> $OUTFILE
        /usr/bin/time -v ./part1_sorting/threading_sort $workers $size 2>> $OUTFILE
        echo "Multiprocessing Sort" >> $OUTFILE
        /usr/bin/time -v ./part1_sorting/multiprocessing_sort $workers $size 2>> $OUTFILE
        echo "" >> $OUTFILE
    done
done

# Part 2: Max Value Aggregation (Synchronization Impact)
echo "Part 2: Max Value Aggregation (Synchronization Performance)" >> $OUTFILE
for workers in 1 2 4 8
do
    echo "Workers: $workers" >> $OUTFILE
    echo "Threading Max" >> $OUTFILE
    /usr/bin/time -v ./part2_max_value/threading_max $workers 131072 2>> $OUTFILE
    echo "Multiprocessing Max" >> $OUTFILE
    /usr/bin/time -v ./part2_max_value/multiprocessing_max $workers 131072 2>> $OUTFILE
    echo "" >> $OUTFILE
done

echo "Performance measurements saved to $OUTFILE"

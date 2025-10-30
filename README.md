# Cmpsc-472-Proj1

Clone or download the repo onto your local machine (git clone https://github.com/SamShtram/Cmpsc-472-Proj1.git)

Open a terminal in Ubuntu and navigate to the project folder (for example: cd Cmpsc-472-Proj1).

Make the run script executable by typing chmod +x run_all.sh.

Run the full project by typing ./run_all.sh.

The script will automatically compile all C source files, execute both parts (Parallel Sorting and Max-Value Aggregation), and test with 1, 2, 4, and 8 workers.

Execution time and memory usage will be measured and saved in the file results/performance_results.txt.

To view the results, open that file or type cat results/performance_results.txt in the terminal.

To run a specific program manually, navigate to the appropriate folder and use a command such as ./part1_sorting/threading_sort 4 131072 or ./part2_max_value/multiprocessing_max 8 131072.

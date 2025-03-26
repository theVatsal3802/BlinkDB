#!/bin/bash

# Ensure the results directory exists
RESULTS_DIR="./results"
mkdir -p "$RESULTS_DIR"

# Define command and connection combinations
commands=(10000 100000 1000000)
connections=(10 100 1000)

# Run benchmarks
for num_cmds in "${commands[@]}"; do
    for num_conns in "${connections[@]}"; do
        output_file="$RESULTS_DIR/results_${num_conns}_${num_cmds}.txt"
        echo "Running benchmark: Parallel Connections = $num_conns, Number of Commands = $num_cmds"
        
        # Execute redis-benchmark with only SET and GET commands
        redis-benchmark -n "$num_cmds" -c "$num_conns" -t set,get -p 5000 > "$output_file"

        echo "Results saved to $output_file"
    done
done

echo "Benchmarking completed. Results are stored in $RESULTS_DIR."

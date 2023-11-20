# Check if a command is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <command>"
  exit 1
fi

# Run the command and measure the time
start_time=$(date +%s.%N)
"$@"
end_time=$(date +%s.%N)

# Calculate the elapsed time
elapsed_time=$(echo "$end_time - $start_time" | bc)

# Display the result
echo "Time taken: $elapsed_time seconds"

#!/usr/bin/python3

import sys
import pandas as pd
import matplotlib.pyplot as plt

sys.dont_write_bytecode = True

# Assumes that the snapshot file is in the following format:
#
# [0] = 0 0
# [1] = 7 7
# [2] = 23 23
# [3] = 42 42
# ...
def do_analysis(input_file):

    # Read the lines from the file
    with open(input_file, 'r') as file:
        lines = file.readlines()

    # Initialize lists to store left and right channel values
    left_channel = []
    right_channel = []

    # Extract values for each tuple
    for line in lines:
        parts = line.split('=')
        if len(parts) == 2:
            # Split the tuple into left and right channel values
            left, right = parts[1].strip().split()
            left_channel.append(int(left))
            right_channel.append(int(right))

    # Create DataFrame
    df = pd.DataFrame({'Left Channel': left_channel, 'Right Channel': right_channel})

    # Print the DataFrame
    print(df)

    # Plot the DataFrame
    df.plot(subplots=True)
    plt.show()

if __name__ == '__main__':
    # Check if the input file path is provided
    if len(sys.argv) != 2:
        print("Usage: python script.py <input_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    do_analysis(input_file)

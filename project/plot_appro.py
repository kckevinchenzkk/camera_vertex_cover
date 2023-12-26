import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Data from the user
# CNF-SAT-VC data
cnf_data = {
    'V5': [1, 2, 1, 1, 1, 2, 2, 3, 2, 3],
    'V10': [2, 2, 2, 3, 2, 2, 3, 4, 3, 2],
    'V15': [5, 4, 4, 5, 5, 4, 4, 5, 3, 4],
    'V20': [7, 6, 7, 7, 7, 7, 7, 7, 7, 7]
}

# APPROX-VC-1 data
vc1_data = {
    'V5': [1, 2, 1, 1, 1, 2, 2, 3, 2, 3],
    'V10': [2, 2, 2, 3, 2, 2, 3, 4, 3, 2],
    'V15': [5, 4, 4, 5, 5, 4, 4, 5, 3, 4],
    'V20': [7, 6, 7, 7, 7, 7, 7, 8, 7, 7]
}

# APPROX-VC-2 data
vc2_data = {
    'V5': [2, 2, 2, 2, 2, 4, 4, 4, 4, 4],
    'V10': [4, 4, 4, 6, 4, 4, 6, 8, 6, 4],
    'V15': [10, 8, 8, 10, 10, 8, 8, 10, 6, 8],
    'V20': [14, 12, 14, 14, 14, 14, 14, 16, 14, 14]
}

# Function to calculate the approximation ratio
def calculate_approximation_ratios(approx_data, cnf_data):
    approximation_ratios = []
    for key in cnf_data:
        ratios = np.array(approx_data[key]) / np.array(cnf_data[key])
        approximation_ratios.append(np.mean(ratios))
    return approximation_ratios

# Function to calculate the standard deviation of the approximation ratio
def calculate_std(approx_data, cnf_data):
    std_devs = []
    for key in cnf_data:
        ratios = np.array(approx_data[key]) / np.array(cnf_data[key])
        std_devs.append(np.std(ratios))
    return std_devs

# Calculate approximation ratios
approx_ratios_vc1 = calculate_approximation_ratios(vc1_data, cnf_data)
approx_ratios_vc2 = calculate_approximation_ratios(vc2_data, cnf_data)

# Calculate standard deviations
std_dev_vc1 = calculate_std(vc1_data, cnf_data)
std_dev_vc2 = calculate_std(vc2_data, cnf_data)

# X-axis values
vertices = [5, 10, 15, 20]

# Plotting the graph
plt.figure(figsize=(10, 6))
plt.errorbar(vertices, approx_ratios_vc1, yerr=std_dev_vc1, label='APPROX-VC-1/CNF-SAT-VC', fmt='-o')
plt.errorbar(vertices, approx_ratios_vc2, yerr=std_dev_vc2, label='APPROX-VC-2/CNF-SAT-VC', fmt='-o')

# Labeling the axes and title
plt.xlabel('Number of vertices')
plt.ylabel('Approximation Ratio')
plt.title('Approximation Ratio Analysis')
plt.legend()
plt.grid(True)
plt.show()

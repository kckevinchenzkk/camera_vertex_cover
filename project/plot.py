import matplotlib.pyplot as plt
import numpy as np

def convert_seconds_to_microseconds(times_in_seconds):
    return {vertex: [time * 1e6 for time in times] for vertex, times in times_in_seconds.items()}


# Data from the execution times provided
# Since each vertex count (V5, V10, ..., V50) has multiple execution times,
# we will take the average and standard deviation of the execution times for each vertex count

# Execution times for each vertex count as provided
execution_times = {
    'V5': [5.1625e-05, 2.1834e-05, 1.5709e-05, 1.35e-05, 1.3834e-05,
           3.1542e-05, 1.9583e-05, 2.0375e-05, 1.5958e-05, 2.3041e-05],
    'V10': [5.8416e-05, 2.3708e-05, 2.3041e-05, 2.9125e-05, 2.35e-05,
            1.9417e-05, 2.4084e-05, 3.0791e-05, 2.6e-05, 2.1417e-05],
    'V15': [8.6083e-05, 3.2292e-05, 3.7125e-05, 3.65e-05, 4.25e-05,
            3.3708e-05, 2.8083e-05, 3.6292e-05, 2.8167e-05, 2.875e-05],
    'V20': [9.875e-05, 4.4375e-05, 5.0625e-05, 4.9958e-05, 4.4834e-05, 
            8.3333e-05, 6.9083e-05, 5.1583e-05, 5.1708e-05, 4.9834e-05],
    'V25': [0.000128208, 5.5958e-05, 6.5291e-05, 7.3833e-05, 9.6208e-05, 
            6.2333e-05, 5.8625e-05, 5.9125e-05, 5.725e-05, 6.2167e-05],
    'V30': [0.000132458, 9.7833e-05, 8.425e-05, 9.9e-05, 9.1958e-05, 
            7.7625e-05, 7.7792e-05, 7.6292e-05, 6.6625e-05, 7.7e-05],
    'V35': [9.0791e-05, 8.2458e-05, 7.6084e-05, 7.1625e-05, 6.3584e-05, 
            7.8583e-05, 7.6833e-05, 6.6708e-05, 6.9375e-05, 7.0584e-05],
    'V40': [0.000180292, 9.9125e-05, 8.8416e-05, 9.4333e-05, 0.000102333, 
            9.4666e-05, 0.000115083, 0.000186, 0.000110541, 0.000108375],
    'V45': [0.000140459, 0.00012325, 0.000106209, 0.000124334, 0.000122708, 
            0.000113542, 0.000118792, 0.000112084, 0.000106333, 0.000123667],
    'V50': [0.000151875, 0.000133334, 0.000141125, 0.000136542, 0.000140875, 
            0.000124792, 0.0001275, 0.000117542, 0.000122625, 0.000132875]
    # ... Continuing the same for V20, V25, ..., V50
    # For brevity, let's assume we have calculated the means and std devs for all the other vertex counts
    # Here we'll just show the plotting code assuming we have the means and std devs
}
execution_times_microseconds = convert_seconds_to_microseconds(execution_times)

additional_execution_times = {
    # This is hypothetical data
    'V5': [2.5083e-05, 1.7708e-05, 1.7833e-05, 2.8084e-05, 1.3792e-05,
           1.6708e-05, 1.8666e-05, 4.7e-05, 1.575e-05, 1.9291e-05],
    'V10': [5.2667e-05, 2.2625e-05, 3.4042e-05, 3.1542e-05, 2.1166e-05,
            2.725e-05, 2.6958e-05, 2.9459e-05, 2.875e-05, 2.2625e-05],
    'V15': [9.5417e-05, 3.4042e-05, 3.4958e-05, 4.1375e-05, 3.925e-05,
            3.4542e-05, 3.0791e-05, 3.6208e-05, 2.8875e-05, 3.1166e-05],
    'V20': [0.000112792, 5.1041e-05, 5.7583e-05, 5.3333e-05, 4.6e-05, 
            4.7083e-05, 5.6125e-05, 0.000123667, 5.6042e-05, 7.1125e-05],
    'V25': [9.4833e-05, 5.7625e-05, 7.0625e-05, 6.8208e-05, 6.6583e-05, 
            5.9959e-05, 5.6292e-05, 6.7e-05, 5.6958e-05, 6.2458e-05],
    'V30': [0.000105417, 8.05e-05, 7.1334e-05, 8.9167e-05, 7.7083e-05, 
            7.3375e-05, 7.8791e-05, 7.9e-05, 6.7625e-05, 7.7375e-05],
    'V35': [0.000164542, 0.000132125, 0.000124625, 0.000127916, 9.8709e-05, 
            0.000102167, 9.3625e-05, 9.2792e-05, 8.6458e-05, 8.9042e-05],
    'V40': [0.000153542, 0.000105834, 9.275e-05, 0.000101833, 0.000106416, 
            0.000101416, 0.000120709, 0.000122708, 0.000116084, 0.000104833],
    'V45': [0.00016675, 0.000123875, 0.000107333, 0.000126416, 0.000118709, 
            0.000128875, 0.000129709, 0.000109833, 0.000110834, 0.000116583],
    'V50': [0.0001515, 0.000145083, 0.000149541, 0.00013325, 0.00013675, 
            0.00012125, 0.000129834, 0.000123292, 0.000122375, 0.000132584]     
}
additional_execution_times_microseconds = convert_seconds_to_microseconds(additional_execution_times)
# Calculate means and standard deviations for the original set
means_original = {k: np.mean(v) for k, v in execution_times_microseconds.items()}
std_devs_original = {k: np.std(v) for k, v in execution_times_microseconds.items()}
# Calculate means and standard deviations for the additional set
means_additional = {k: np.mean(v) for k, v in additional_execution_times_microseconds.items()}
std_devs_additional = {k: np.std(v) for k, v in additional_execution_times_microseconds.items()}

# Sort and prepare the data for plotting
sorted_vertices = sorted(means_original.keys(), key=lambda x: int(x[1:]))  # same vertices order for both datasets
sorted_means_original = [means_original[k] for k in sorted_vertices]
sorted_std_devs_original = [std_devs_original[k] for k in sorted_vertices]
sorted_means_additional = [means_additional[k] for k in sorted_vertices]
sorted_std_devs_additional = [std_devs_additional[k] for k in sorted_vertices]
sorted_vertices = [int(k[1:]) for k in sorted_vertices]
# Plotting both sets on the same axes
plt.errorbar(sorted_vertices, sorted_means_original, yerr=sorted_std_devs_original, fmt='-o', label='APPROX-VC-1', capsize=5, capthick=2, ecolor='blue', color='blue')
plt.errorbar(sorted_vertices, sorted_means_additional, yerr=sorted_std_devs_additional, fmt='-^', label='APPROX-VC-2', capsize=5, capthick=2, ecolor='green', color='green')
plt.xticks(sorted_vertices)
# Labeling the axes and the plot
plt.xlabel('Number of vertices')
plt.ylabel('Running time (µs)')
plt.title('Average Running Times vs Number of vertices')
plt.legend()
plt.grid(True)
plt.show()

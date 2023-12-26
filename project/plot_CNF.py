import matplotlib.pyplot as plt
import numpy as np

def convert_seconds_to_microseconds(times_in_seconds):
    return {vertex: [time * 1e0 for time in times] for vertex, times in times_in_seconds.items()}


# Data from the execution times provided
# Since each vertex count (V5, V10, ..., V50) has multiple execution times,
# we will take the average and standard deviation of the execution times for each vertex count

# Execution times for each vertex count as provided
execution_times = {
     'V5': [2.93417e-04, 2.53625e-04, 1.02084e-04, 1.86459e-04, 1.49083e-04, 2.56125e-04, 2.23958e-04, 4.77000e-04, 2.20458e-04, 4.76875e-04],
    'V10': [7.36084e-04, 5.76750e-04, 4.73292e-04, 1.28875e-03, 4.95583e-04, 4.77416e-04, 9.58333e-04, 3.29329e-03, 8.62042e-04, 3.53583e-04],
    'V15': [2.65296e-02, 2.85525e-03, 2.99529e-03, 1.53846e-02, 1.39283e-02, 2.29458e-03, 2.08033e-03, 1.03132e-02, 1.58292e-03, 1.88275e-03],
    'V20': [5.56534e-01, 6.92595e-02, 7.12470e-01, 4.78250e-01, 6.26581e-01, 6.59255e-01, 8.20183e-01, 0.80841, 7.31477e-01, 7.15831e-01]
    # ... Continuing the same for V20, V25, ..., V50
    # For brevity, let's assume we have calculated the means and std devs for all the other vertex counts
    # Here we'll just show the plotting code assuming we have the means and std devs
}
execution_times_microseconds = convert_seconds_to_microseconds(execution_times)

# Calculating means and standard deviations
means = {k: np.mean(v) for k, v in execution_times_microseconds.items()}
std_devs = {k: np.std(v) for k, v in execution_times_microseconds.items()}

# Sorting keys and values for plotting
sorted_keys = sorted(means, key=lambda x: int(x[1:]))
sorted_means = [means[k] for k in sorted_keys]
sorted_std_devs = [std_devs[k] for k in sorted_keys]
sorted_vertices = [int(k[1:]) for k in sorted_keys]

# Plotting with lines and error bars
plt.errorbar(sorted_vertices, sorted_means, yerr=sorted_std_devs, fmt='-o', label='CNF-SAT-VC', capsize=5, capthick=2, ecolor='black', linestyle='-', marker='o', color='b')

plt.xlabel('Number of vertices')
plt.ylabel('Running time (s)')
plt.title('Average Running Time vs Number of Vertices')
#plt.yscale('log') # Apply log scale to the y-axis
plt.legend()
plt.grid(True)
plt.show()

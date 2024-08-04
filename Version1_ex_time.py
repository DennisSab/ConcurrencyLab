import numpy as np
import matplotlib.pyplot as plt

# Data from the runs
execution_times = [
    100.183, 100.858, 102.988, 100.332, 100.918, 99.608, 99.915, 
    99.584, 95.812, 101.899, 101.865, 99.497, 98.044, 101.498, 
    100.615, 100.726, 100.689, 101.604, 94.241, 91.365, 93.237, 
    94.028, 90.784, 92.547, 93.004, 92.191, 94.989, 99.428, 
    99.773, 100.105, 101.645, 101.556, 98.960, 98.416, 86.285, 
    98.503, 94.737, 96.904, 94.079, 97.753,  96.586, 96.110, 97.658
]

# Calculate the median
median_time = np.median(execution_times)

# Plot the data
plt.figure(figsize=(10, 5))
plt.plot(execution_times, marker='o')
plt.axhline(y=median_time, color='r', linestyle='--', label=f'Median: {median_time:.3f} ms')
plt.title("Execution Times of Program Runs")
plt.xlabel("Run Number")
plt.ylabel("Execution Time (milliseconds)")
plt.legend()
plt.grid(True)
plt.show()

print(f"Median Execution Time: {median_time:.3f} ms")

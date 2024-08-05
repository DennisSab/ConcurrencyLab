import numpy as np
import matplotlib.pyplot as plt

# Data from the runs
execution_times = [
    103.925, 100.672, 101.550, 100.943, 94.393, 101.004, 101.094, 260.567, 172.963, 101.902,
    101.744, 101.679, 96.991, 99.794, 99.192, 137.996, 106.918, 101.930, 101.740, 101.815,
    100.508, 100.579, 104.849, 105.624, 101.879, 103.742, 103.171, 105.896, 116.076, 240.582,
    99.988, 100.388, 101.645, 100.637, 95.237, 97.554, 97.611, 100.902, 95.864, 100.513,
    101.598, 101.471, 100.561, 99.929, 93.819, 91.812, 101.353
]

# Calculate the median
median_time = np.median(execution_times)

# Plot the data
plt.figure(figsize=(10, 5))
plt.plot(execution_times, marker='o')
plt.axhline(y=median_time, color='r', linestyle='--', label=f'Median: {median_time:.3f} ms')
plt.title("Version2 execution time")
plt.xlabel("Run Number")
plt.ylabel("Execution Time (milliseconds)")
plt.legend()
plt.grid(True)
plt.show()

print(f"Median Execution Time: {median_time:.3f} ms")

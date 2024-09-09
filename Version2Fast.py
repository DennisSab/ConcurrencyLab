import matplotlib.pyplot as plt

# Data for the diagram
cores = [4, 8, 16, 32, 64]
times = [64.101, 56.759, 45.317, 38.016, 25.592]

# Create the plot
plt.figure(figsize=(8, 6))
plt.plot(cores, times, marker='o', linestyle='-', color='b')

# Add titles and labels
plt.title('Version2Fast-Execution Times vs Number of Cores')
plt.xlabel('Number of Cores')
plt.ylabel('Execution Time (ms)')

# Add grid
plt.grid(True)

# Show plot
plt.show()

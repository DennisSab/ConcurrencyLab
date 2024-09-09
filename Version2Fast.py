import matplotlib.pyplot as plt

# Data for the diagram
cores = [4, 8, 16, 32, 64]
times = [93.687, 74.031, 52.788, 45.526, 20.225]

# Create the plot
plt.figure(figsize=(8, 6))
plt.plot(cores, times, marker='o', linestyle='-', color='b')

# Add titles and labels
plt.title('Version2InsertOnly-Execution Times vs Number of Cores')
plt.xlabel('Number of Cores')
plt.ylabel('Execution Time (ms)')

# Add grid
plt.grid(True)

# Show plot
plt.show()

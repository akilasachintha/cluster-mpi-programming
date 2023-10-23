import matplotlib.pyplot as plt

# Read data from the file
data = []
with open("merge_sort_times_sequential.txt", "r") as file:
    for line in file:
        size, execution_time = line.strip().split()
        data.append((int(size), float(execution_time)))

# Separate the data into X and Y
sizes, execution_times = zip(*data)

# Create a plot
plt.figure(figsize=(10, 6))
plt.plot(sizes, execution_times, marker='o', linestyle='-')
plt.title("Merge Sort Execution Time vs. Input Size")
plt.xlabel("Input Size")
plt.ylabel("Execution Time (seconds)")
plt.grid(True)

plt.savefig("merge_sort_graph_sequential.png")
plt.show()  # Uncomment if you want to display the graph in the window

print("Graph saved as merge_sort_graph_sequential.png")

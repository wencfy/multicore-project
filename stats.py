import matplotlib.pyplot as plt

# Data setup
thread_counts = ['Serial', '1', '2', '4', '8', '16']
data_sizes = ['10000', '100000', '500000', '1000000']

# Time data for each configuration
deletion_time_serial = [0.029, 0.331, 2.166, 4.235]
deletion_time_1 = [0.030, 0.376, 2.313, 5.380]
deletion_time_2 = [0.032, 0.403, 2.269, 4.662]
deletion_time_4 = [0.027, 0.266, 1.492, 3.267]
deletion_time_8 = [0.035, 0.245, 1.073, 2.237]
deletion_time_16 = [0.054, 0.505, 2.301, 4.437]

deletion_times = [deletion_time_serial, deletion_time_1, deletion_time_2, deletion_time_4, deletion_time_8, deletion_time_16]

# Create a figure and a set of subplots
fig, ax = plt.subplots(figsize=(10, 6))

# Plotting each thread count's data
for index, times in enumerate(deletion_times):
    ax.plot(data_sizes, times, marker='o', label=f'{thread_counts[index]} Threads')

# Set labels and title for the plot
ax.set_xlabel('Data Size')
ax.set_ylabel('Deletion Time (seconds)')
ax.set_title('Deletion Times by Thread Count and Data Size')
ax.legend(title="Thread Counts")

# Display the plot
plt.show()

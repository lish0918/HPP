import matplotlib.pyplot as plt

# Time taken for different number of threads
time_taken_25 = [0.08932, 0.038593, 0.030187, 0.027294, 0.02665, 0.024847, 0.023526, 0.023519, 0.023847, 0.018599, 0.02282, 0.024126]
time_taken_36 = [3.428561, 1.099001, 0.752702, 1.073943, 0.42942, 0.339292, 0.367634, 0.341547, 0.341606, 0.657021, 0.749158, 0.536951]

# Number of threads
num_threads = range(1, 13)

# Calculate speedup and ideal speedup
speedup_25 = [time_taken_25[0]/t for t in time_taken_25]
speedup_36 = [time_taken_36[0]/t for t in time_taken_36]

# Plotting the speedup and ideal speedup
plt.plot(num_threads, speedup_25, label='25x25', color='orange', marker='o')
plt.plot(num_threads, speedup_36, label='36x36', color='green', marker='s')
plt.plot(num_threads, num_threads, label='Ideal Speedup', color='grey', linestyle='--')

# Set labels and title
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.title('Parallelism speeds up with thread growth')

# Add legend
plt.legend(loc='upper left')

#加网格背景
plt.grid(True)
#加背景颜色
plt.gca().set_facecolor('whitesmoke')

# Show the plot
plt.show()

# Save the plot as an image
plt.savefig('/home/lish6557/HPP/Sudoku_Solver_V2/speedup_plot.png')
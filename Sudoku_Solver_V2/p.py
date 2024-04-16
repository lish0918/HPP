import matplotlib.pyplot as plt

# 线程数和解决时间数据
threads = list(range(1, 13))  # 1到12个线程
solve_times = {
    "25_hard": [0.012325, 0.010689, 0.009167, 0.009141, 0.006875, 0.009063, 0.006212, 0.00738, 0.006507, 0.006298, 0.00657, 0.005331],
    "36_easy": [0.001428, 0.001795, 0.001814, 0.001875, 0.001909, 0.001909, 0.002225, 0.002338, 0.002561, 0.002953, 0.002728, 0.00289],
    "36_medium": [0.007553, 0.006848, 0.009698, 0.008473, 0.007281, 0.010206, 0.009942, 0.007004, 0.005086, 0.006484, 0.007092, 0.006484],
    "36_hard": [0.531446, 0.482359, 0.273293, 0.300941, 0.210356, 0.176512, 0.150545, 0.148351, 0.148351, 0.142402, 0.129188, 0.159435],
    #"36_hard_2": [34.981684, 33.766038, 34.135213, 16.992831, 17.983605, 14.97043, 15.327629, 16.226477, 21.678924, 22.249124, 13.280458, 22.227531],
    "49_easy": [0.27656, 0.17028, 0.115484, 0.1013, 0.091149, 0.085356, 0.088035, 0.0969, 0.091952, 0.103302, 0.100129, 0.101626],
    "49_medium": [0.009649, 0.00981, 0.010772, 0.007872, 0.011232, 0.011197, 0.011141, 0.011649, 0.011583, 0.011627, 0.012143, 0.011446],
    "49_hard": [45.303574, 23.302782, 19.463242, 19.428681, 19.447505, 16.930618, 16.900764, 15.260484, 15.275854, 11.011232, 11.037146, 8.220084],
    "64_easy": [0.011907, 0.012191, 0.011375, 0.012952, 0.012054, 0.009618, 0.011977, 0.012869, 0.014593, 0.014445, 0.014424, 0.013235],
    "64_medium": [0.94143, 0.905984, 0.894187, 0.531837, 0.53613, 0.531478, 0.382163, 0.415271, 0.401119, 0.379081, 0.391809, 0.385763]
}

# 串行解决时间（线程数为1时的解决时间）
serial_times = {key: solve_times[key][0] for key in solve_times}

# 计算速度提升
speedups = {key: [serial_times[key] / solve_times[key][i] for i in range(len(threads))] for key in solve_times}

# 绘制速度提升图
plt.figure(figsize=(12, 8))
for key in solve_times:
    plt.plot(threads, speedups[key], marker='o', label=key)

plt.plot(threads, threads, label='Ideal Speedup', color='grey', linestyle='--')

plt.xlabel('Number of Threads', fontsize=14)
plt.ylabel('Speedup', fontsize=14)
plt.title('Speedup of Sudoku Solver with Different Puzzle Sizes and Difficulties', fontsize=16)
plt.legend(loc='upper left', fontsize=12)
plt.grid(True)
plt.gca().set_facecolor('whitesmoke')
plt.xticks(threads)
plt.show()
plt.savefig('/home/lish6557/HPP/Sudoku_Solver_V2/speedup.png')
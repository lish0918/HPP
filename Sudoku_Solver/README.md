# 数独问题

了解数独游戏的逻辑性，我读了这篇[博客](https://blog.csdn.net/only_me_canthis/article/details/123800468)和这篇[论文](https://max.book118.com/html/2016/0507/42155092.shtm)。

## 数独题目生成思路

出题有两种思维模式，一种是挖空出题，一种是空盘出题。

1. **挖空出题**：先找到一个终盘（也就是答案盘），然后挖出其中的提示数，然后自己尝试去解题，发现多解之后，再多添加一些候选数即可。

2. **空盘出题**：先在一个空盘面之内，找到一些位置，用来放提示数，然后随便写数字，采用“边写边解”的方式，来进行出题。矛盾了就“回溯”，即倒退。

可以参考[多次求解比对法](https://www2.zhihu.com/question/381719541)和[挖空出题](https://blog.csdn.net/any_ways/article/details/128063742?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-0-128063742-blog-106110737.235^v43^control&spm=1001.2101.3001.4242.1&utm_relevant_index=3)。

## 实现代码

- **Sudoku.c**：数独题目生成器，利用终端命令 `./Sudoku N` 执行，其中 N 为数独个数。
  
  运行方法：
  ```shell
  gcc -o Sudoku Sudoku.c 
  ./Sudoku 1000000
  ```

- **Sudoku_Solver.c**：数独求解器。

- **Sudoku_Test.c**：答案测试器。

## 代码参考

本项目中的数独求解器参考了这篇[C++数读生成](https://blog.csdn.net/any_ways/article/details/128063742?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-0-128063742-blog-106110737.235^v43^control&spm=1001.2101.3001.4242.1&utm_relevant_index=3)代码。

# 并行数度求解器

这个项目旨在利用并行处理来解决数独问题，以提高解决问题的效率。以下是一些关键要点：

- 并行任务：每个猜测（guess）都被视为一个独立的任务，可以并行处理。这意味着可以将每个猜测分配给一个单独的线程或任务来处理，从而加速解决问题的过程。
- 避免同一板上的冲突：在解决数独问题时，同一数独棋盘上的两个任务不能同时进行。这是因为每个任务都会对同一数独棋盘进行更改，如果同时进行可能会导致数据竞争和不确定的结果。
- 任务取消：当找到解决方案时，不应再发出更多的任务，并且应取消先前发出的任务。这是为了避免浪费计算资源并提高效率。
- 内部任务创建可以在任务内部创建更多的任务，以确保线程池中始终有足够的工作量。但是，应该避免创建过小的任务，因为这可能会增加内存和任务管理的开销。

## 代码参考
可以参考这个利用OpenMP解决[N皇后问题](https://z-rui.github.io/post/2015/11/openmp/)的代码，它同样基于回溯算法实现。
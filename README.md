# simple_threads
## 分支
### main
无需 c++11 标准的线程池，以学习为目的，简单封装 pthread，持续改进中
### cpp11
基于 c++11 实现的简单线程池
## 问题
~~当前问题：shutdown() shtdownNow() 存在一定概率的 segmentation fault 导致 core dump，怀疑是线程取消引出的相关问题，dubug 中。~~

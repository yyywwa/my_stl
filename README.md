#veclist 使用vector<T*>类型的index管理空间

shared_ptr使用一个表来进行计数管理，表类型为veclist,每个元素都包含指针和计数器

shared_ptr可接收函数型和仿函数型删除器

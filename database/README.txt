README

Attention：可以使用该文件夹下的timer.db在本地测试数据库！该数据库只保存在本地，无设备间共享功能。

安装方法：将database.h、database.cpp分别加入到headers和sources中，在需要使用的代码文件中#include “database.h”即可。使用前，需要手动修改Database::connect()函数中的数据库路径为本地.db文件的绝对路径（在我的Mac系统上不能使用相对路径，win的情况尚不清楚）。

常见操作：
1. 连接数据库：Database db; db.connect(); 在读取数据之前，必须连接数据库
2. 断开数据库：db.disconnect(); 在程序结束之前，必须断开数据库
3. 批量插入数据：每条数据需要被包装为Work类对象（已定义在database.h中），并组成一个QVector，使用Database::insert_worklist(QVector &)函数即可
4. 查询数据前必须要做的：Selector selector(db); 在进行数据查询之前，必须保证数据库已经连接，并实例化Selector类（即执行本条所述操作）
5. 查询过去某天的全部label及各自的总时长：对于当前时刻之前的第n天（例如，昨天是第一天），使用selector.getdailystat(int n)，该函数主要是为界面5中绘制折线图设计
6. 查询过去某天的某个label对应的全部事项信息：对于当前时刻之前的第n天，使用selector.getdaylabel(int n, QString label)，该函数主要是为界面4中光标移动到饼图时显示该label下的详细信息设计
7. 查询过去某天的日期：对于当前时刻之前的第n天，使用selector.getdate()
8. 查询历史上使用过的n条名称：使用selector.getnames(n); 返回顺序为时间从近到远且去重，该函数与9中的函数主要是为界面1中创建事项/标签的下拉菜单联想设计
9. 查询历史上使用过的n条标签：使用selector.getlabels(n); 返回顺序为时间从近到远且去重

一些需要注意的地方：
1. 上述操作3的时间格式必须严格为“YYYY-MM-DD HH:MM:SS”，例如：1900-01-01 08:09:01（注意前导零）
2. 不要轻易执行除常见操作中列出的操作之外的指令，可能出现不可恢复的错误

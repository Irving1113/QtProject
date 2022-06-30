# YourDay 时间分配记录软件

> 作者：鲁浩然 王乾旭 郭一凡

## 概述

YourDay是一个能够量化记录每日时间分配的PC端时间管理软件，为用户提供了非常方便的时间流记录功能。

## 功能

YourDay的功能包括但不限于：

- 快捷地记录一日内各个事项的进行时间
- 为每个事项个性化地添加标签与备注，对每日事项进行分类
- 查看过去一段时间内每日时间的分配情况
- 查看过去一段时间内各类事项的时间变化趋势
- 使用本地SQLite数据库统一管理和统计数据

## 安装及运行

YourDay使用C++/Qt编写，仅依赖于Qt5。可以使用下述方法便捷地安装YourDay：

1. `git clone https://github.com/Irving1113/QtProject.git`
2. 使用Qt Creator打开QtProject/first_window.pro
3. 更改位于QtProject/database.cpp:15的本地数据库路径
4. 编译并运行

## 注意事项

编译YourDay时**必须使用Qt5**。使用其他Qt版本可能导致严重错误。


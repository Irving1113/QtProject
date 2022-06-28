#include "database.h"

Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");
}

void Database::connect() {
    bool is_open = db.isOpen();
    if (is_open){
        printf("The database has already opened.\n");
        return;
    }
    else{
        db.setDatabaseName("/Users/guoyifan/Desktop/timer_gyf.db");     //修改为本地.db文件的绝对路径
        bool success = db.open();
        if (!success){
            printf("Failed to open database.\n");
            return;
        }
    }
}

void Database::disconnect() {
    bool is_open = db.isOpen();
    if (!is_open){
        printf("The database has not opened before.\n");
        return;
    }
    else {
        db.close();
        is_open = db.isOpen();
        if (is_open){
            printf("Failed to close the database.\n");
            return;
        }
    }
}

void Database::insert_data(QString cmd){        //不要调用！
    bool is_open = db.isOpen();
    if (!is_open){
        printf("The database has not opened before.\n");
        return;
    }
    else {
        QSqlQuery query(db);
        bool success = query.exec(cmd);
        if (!success){
            printf("Failed to execute command.\n");
            return;
        }
        return;
    }
}

void Database::insert_worklist(QVector<Work> &worklist) {       //用QVector包装需要传入数据库的数据，数据为Work类对象
    QString cmd = "insert into events(name, begintime, endtime, lasttime, label, remarks) values";
    bool first = true;
    for (auto w: worklist) {
        if (!first)
            cmd += ", ";
        first = false;
        QString str = "(" + w.name + ", " + w.begin_time + ", " + w.end_time + ", " + QString::number(w.last_time, 10) + ", " + w.label + ", " + w.remarks + ")";
        cmd += str;
    }
    insert_data(cmd);
}

QVector<QVector<QString> > Database::get_data(QString cmd, int n){      //不要调用！
    bool is_open = db.isOpen();
    if (!is_open){
        printf("The database has not opened before.\n");
        return QVector<QVector<QString> >();
    }
    else {
        QVector<QVector<QString> > data;
        QSqlQuery query(db);
        bool success = query.exec(cmd);
        if (!success){
            printf("Failed to get data from database.\n");
            return QVector<QVector<QString> >();
        }
        else {
            while (query.next()){
                QVector<QString> result;
                for (int i = 0; i < n; i++)
                    result.append(query.value(i).toString());
                data.append(result);
            }
            return data;
        }
    }
}

Selector::Selector(Database & _db): db(_db) {}

QVector<QPair<QString, int> > Selector::getdailystat(int n) {       //参数n表示从当前开始n天前，返回第n天前的各个label及其持续时间
    QString date = getdate(n);
    QString date2 = getdate(n - 1);
    QString cond1 = "begintime between datetime('" + date + " 00:00:00') and datetime('" + date2 + " 00:00:00')";
    QString cmd = "select label, sum(lasttime) from events where " + cond1 + " group by(label);";
    auto retval = db.get_data(cmd, 2);
    QVector<QPair<QString, int> > tmp;
    for (auto line: retval)
        tmp.append(qMakePair(line[0], line[1].toInt()));
    return tmp;
}

QVector<Work> Selector::getdaylabel(int n, QString label) {     //参数n表示从当前开始n天前，参数label表示所要查看的label，返回该label下各个事项的Work对象
    QString date = getdate(n);
    QString date2 = getdate(n - 1);
    QString cond1 = "begintime between datetime('" + date + " 00:00:00') and datetime('" + date2 + " 00:00:00')";
    QString cond2 = "label = '" + label + "'";
    QString cmd = "select name, begintime, endtime, lasttime from events where " + cond1 + " and " + cond2 + ";";
    printf("%s\n", cmd.toStdString().c_str());
    auto retval = db.get_data(cmd, 4);
    QVector<Work> tmp;
    for (auto line: retval) {       //返回的Work对象中仅包含事项名称、开始时刻、结束时刻和持续时间
        Work work;
        work.name = line[0];
        work.begin_time = line[1];
        work.end_time = line[2];
        work.last_time = line[3].toInt();
        tmp.append(work);
    }
    return tmp;
}

QString Selector::getdate(int n) {      //参数n表示从当前开始n天前，返回n天前的日期
    QString cmd = "select date('now', '" + QString::number(-n, 10) + " days');";
    auto retval = db.get_data((QString)cmd, 1);
    return QString(retval[0][0]);
}

QVector<QDateTime> Selector::getalldate(int n) {
    QVector<QDateTime> v;
    for (int i = n; i > 0; i--) {
        QString ymd = Selector::getdate(i);
        QDateTime format_date = QDateTime::fromString(ymd, "yyyy-MM-dd");
        v.append(format_date);
    }
    return v;
}

QMap<QString, QVector<int> > Selector::datacollection(int n) {      //获取近n天中各个label的使用情况：<label, <date, last_time>>
    QMap<QString, QVector<int> > retval;
    for (int j = n; j > 0; j--) {
        QVector<QPair<QString, int> > days_before = Selector::getdailystat(j);
        for (auto line: days_before) {
            if (retval.contains(line.first))
                retval[line.first][n - j] = line.second;
            else {
                retval.insert(line.first, QVector<int>(n, 0));
                retval[line.first][n - j] = line.second;
            }

        }
    }
    return retval;
}

QVector<QString> Selector::getlabels_days(int n){       //获取近n天中全部的label
    QString date = getdate(n);
    QString date2 = getdate(0);
    QString cond1 = "begintime between datetime('" + date + " 00:00:00') and datetime('" + date2 + " 00:00:00')";
    QString cmd = "select distinct label from events where " + cond1 + ";";
    auto retval = db.get_data(cmd, 1);
    QVector<QString> tmp;
    for (auto line: retval)
        tmp.append(line[0]);
    return tmp;
}

QVector<QString> Selector::getnames(int n) {        //参数n表示希望查询最近插入数据库的n个事项，返回这n个事项的名称
    QString cmd = "select distinct name from events order by begintime desc limit " + QString::number(n, 10) + ";";
    auto retval = db.get_data(cmd, 1);
    QVector<QString> tmp;
    for (auto line: retval)
        tmp.append(line[0]);
    return tmp;
}

QVector<QString> Selector::getlabels(int n) {       //参数n表示希望查询最近插入数据库的n个label，返回这n个项目的label
    QString cmd = "select distinct label from events order by begintime desc limit " + QString::number(n, 10) + ";";
    auto retval = db.get_data(cmd, 1);
    QVector<QString> tmp;
    for (auto line: retval)
        tmp.append(line[0]);
    return tmp;
}

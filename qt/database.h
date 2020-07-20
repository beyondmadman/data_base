#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>


class DataBase
{
public:
    DataBase(const QString strConnName);
    ~DataBase();

    //打开数据库
    bool openDB(const QString strSvrName,const QString strUserID,
                const QString strUserPwd ,const QString strDBname);
    //关闭数据库
    void closeDB();

    //是否打开
    bool isOpen() const;

    //是否结束
    bool IsRecordsetEnd() const;
    //移到开头
    bool moveToFirst() const;
    //往下移动
    bool moveToNext() const;
    //获取数目
    long getRecordCount() const;

    //执行语句
    bool executeSentence(const QString strExcutePara);
    //存储过程
    bool executeProcess(const QString strProcess, QString str1, QString& str2);

private:
    QString                             m_strConnName;
    QSqlDatabase *                      m_pDB;
    QSqlQuery *                         m_query;
};

#endif // DATABASE_H

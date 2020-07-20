#include "database.h"

#include <QSqlError>
#include <QSqlDriver>
#include <QDebug>

DataBase::DataBase(const QString strConnName)
{
    m_strConnName = strConnName;
    m_pDB = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", m_strConnName));
}

DataBase::~DataBase()
{
    closeDB();

    delete m_pDB;
    delete m_query;

    QSqlDatabase::removeDatabase(m_strConnName);
}

bool DataBase::openDB(const QString strSvrName, const QString strUserID, const QString strUserPwd, const QString strDBname)
{
    m_pDB->setHostName(strSvrName);
    m_pDB->setUserName(strUserID);
    m_pDB->setPassword(strUserPwd);
    m_pDB->setDatabaseName(strDBname);

    if(!m_pDB->open())
    {
        qDebug() << m_pDB->lastError().text();
        return false;
    }

    m_query = new QSqlQuery(*m_pDB);
    return true;
}

void DataBase::closeDB()
{
    m_pDB->close();
}

bool DataBase::isOpen() const
{
    return m_pDB->isOpen();
}

bool DataBase::IsRecordsetEnd() const
{
    if(m_query == NULL)
        return true;
    return (QSql::AfterLastRow == m_query->at());
}

bool DataBase::moveToFirst() const
{
    if (!isOpen() || m_query == NULL)
    {
        return false;
    }

    if(m_query->isActive())
    {
        return m_query->first();
    }
    else
    {
        return false;
    }
}

bool DataBase::moveToNext() const
{
    if (!isOpen() || m_query == NULL)
    {
        return false;
    }

    if(m_query->isActive())
    {
        return m_query->next();
    }
    else
    {
        return false;
    }
}

long DataBase::getRecordCount() const
{
    if (!isOpen()|| m_query == NULL)
    {
        return -1;
    }

    if(m_pDB->driver()->hasFeature(QSqlDriver::QuerySize))
    {
        return m_query->size();
    }
    else
    {
        int i = m_query->at();
        m_query->last();
        int iRows = m_query->at() + 1;
        m_query->seek(i);
        return iRows;
    }
}

bool DataBase::executeSentence(const QString strExcutePara)
{
    if(!m_query->exec(strExcutePara))
    {
        qDebug() << m_query->lastError().text();
        return false;
    }
    return true;
}

bool DataBase::executeProcess(const QString strProcess, QString str1, QString &str2)
{
    QString strSql = "exec " + strProcess + " ?, ? output";
    m_query->prepare(strSql);
    m_query->bindValue(0, str1);
    m_query->bindValue(1, 0, QSql::Out);
    bool b = m_query->exec();

    str2 = b ? m_query->boundValue(1).toString() : "";

    return b;
}



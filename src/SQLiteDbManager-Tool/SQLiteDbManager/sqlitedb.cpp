/**
 *  @Copyright 裴鹏帅
 *  @Brief	 操作SQLite数据库的接口.
 *  @Author	 裴鹏帅
 *  @Date    2015-01-09
 *
 *  使用方法：
 *     
 *
 *	 修改记录 : 
 *	 日 期            版本            修改人                 修改内容
 *	 2015/8/15	      1.0			 <裴鹏帅>				<封装了操作SQLite数据库的接口> 
 */

#include "StdAfx.h"
#include "sqlitedb.h"


/************************************************************************/
/* 构造对象                                                              */
/************************************************************************/
CSQLiteDatabase::CSQLiteDatabase(void): 
	m_db(0),
	m_bEof(false),
	m_nFieldCount(0),
	m_stmt(0),
	m_bConnect(false)
{
}  

/************************************************************************/
/*                                                                      */
/************************************************************************/
CSQLiteDatabase::CSQLiteDatabase(const CSQLiteDatabase &sqlLite)
{
	m_db=sqlLite.m_db;
	m_stmt=sqlLite.m_stmt;
	m_bEof=sqlLite.m_bEof;
	m_nFieldCount = sqlLite.m_nFieldCount;
	m_bConnect=sqlLite.m_bConnect;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CSQLiteDatabase &CSQLiteDatabase::operator=(const CSQLiteDatabase &sqlLite)
{
	m_db=sqlLite.m_db;
	m_stmt=sqlLite.m_stmt;
	m_bEof=sqlLite.m_bEof;
	m_nFieldCount = sqlLite.m_nFieldCount;
	m_bConnect=sqlLite.m_bConnect;
	return *this;
}


/************************************************************************/
/* 析构对象                                                             */
/************************************************************************/
CSQLiteDatabase::~CSQLiteDatabase(void)  
{  	
	if (m_stmt)
	{
		sqlite3_finalize(m_stmt);
	}
	Close();  
}  

/************************************************************************/
/* 打开数据库路径名
   *param lpDbFlie 数据库路径名
   @return 如果成功，返回true；如果失败，返回false
*/
/************************************************************************/
bool CSQLiteDatabase::Open(tstring tsDbFlie)  
{
	if(tsDbFlie.length() > 0 && !m_bConnect)  
	{
#ifdef  UNICODE   
		m_bConnect = (sqlite3_open16(tsDbFlie.c_str(), &m_db) != SQLITE_OK) ? false : true;
#else  
		m_bConnect = (sqlite3_open(tsDbFlie.c_str(), &m_db) != SQLITE_OK) ? false : true;
#endif  
	}

	return m_bConnect;  
}  

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CSQLiteDatabase::IsOpened()
{
	return m_bConnect;
}

/************************************************************************/
/* 关闭打开的数据库                                                      */
/************************************************************************/
void CSQLiteDatabase::Close()  
{  
	if (m_db && m_bConnect)
	{  
		sqlite3_close(m_db);  
		m_db = 0;  
	}  
	m_bConnect=false;
}  

/************************************************************************/
/*  执行增删改命令                                                       */
/************************************************************************/
bool CSQLiteDatabase::ExecuteCommand(tstring tsSql)  
{  
	bool result = false;
	if(tsSql.length() > 0 && m_bConnect)  
	{  
		sqlite3_stmt * stmt = 0;
#ifdef  UNICODE
		result = (sqlite3_prepare16_v2(m_db, tsSql.c_str(), -1, &stmt, NULL) != SQLITE_OK) ? false : true;
#else  
		result = (sqlite3_prepare_v2(m_db, tsSql.c_str(), -1, &stmt, NULL) != SQLITE_OK) ? false : true;
#endif
		result = (sqlite3_step(stmt) != SQLITE_DONE) ? false : true;
		//result = (sqlite3_finalize(stmt) != SQLITE_OK) ? false : true;
	}
	return result;
}  

 
/************************************************************************/
/* 执行查询                                                             */
/************************************************************************/
bool CSQLiteDatabase::ExcuteQuery(tstring tsSql)  
{
	bool result = false;
	int nresult = 0;
	if (tsSql.length() && m_bConnect)
	{
		sqlite3_stmt* stmt = 0;
		if (stmt)
		{
			result = (sqlite3_finalize(stmt) != SQLITE_OK) ? false : true;
		}
		
#ifdef  UNICODE   
		result = (sqlite3_prepare16_v2(m_db, tsSql.c_str(), -1, &stmt, NULL) != SQLITE_OK) ? false : true;
#else  
		result = (sqlite3_prepare_v2(m_db, tsSql.c_str(), -1, &stmt, NULL) != SQLITE_OK) ? false : true;
#endif  
		m_stmt = stmt;
		nresult = sqlite3_step(m_stmt);
		if (nresult == SQLITE_DONE)
		{
			m_bEof = false;
		}
		else if (nresult == SQLITE_ROW)
		{
			m_bEof = true;
		}
		else
		{
			//no op
		}
		m_nFieldCount = GetFieldNum();
		result = true;
	}
	return result;	
} 

/************************************************************************/
/*是否有更多记录                                                             */
/************************************************************************/
bool CSQLiteDatabase::MoreRecord()
{
    return m_bEof;
}

/************************************************************************/
/*记录集移动到下一条记录                                                      */
/************************************************************************/
void CSQLiteDatabase::NextRecord()
{
	m_bEof = (sqlite3_step(m_stmt) != SQLITE_ROW) ? false : true;
}

/************************************************************************/
/*  获取浮点型字段值                                                                    */
/************************************************************************/

bool CSQLiteDatabase::GetFieldValue(int nFieldIdx, double& dVal)
{
	bool result = false;
	if (nFieldIdx < m_nFieldCount)
	{
		dVal = sqlite3_column_double(m_stmt, nFieldIdx);
		result = true;
	}
	return result;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CSQLiteDatabase::GetFieldValue(int nFieldIdx, tstring & tsVal)
{
	bool result = false;
	if (nFieldIdx < m_nFieldCount)
	{
		_TCHAR * p = 0;
#ifdef  UNICODE   
		p = (_TCHAR *)sqlite3_column_text16(m_stmt, nFieldIdx);
#else  
		p = (_TCHAR *)sqlite3_column_text(m_stmt, nFieldIdx);
#endif
		tsVal = (p != 0) ? p : _T("");
		result = true;
	}
	return result;
}

/************************************************************************/
/*  获取浮点型字段值                                                    */
/************************************************************************/
bool CSQLiteDatabase::GetFieldValue(int nFieldIdx, int & nVal)
{
	bool result = false;
	if (nFieldIdx < m_nFieldCount)
	{
		nVal = sqlite3_column_int(m_stmt, nFieldIdx);
		result = true;
	}
	return result;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
sqlite3_stmt *CSQLiteDatabase::GetRecordSet()
{
	return m_stmt;
}

/************************************************************************/
/*	获取字段个数														*/
/************************************************************************/
int CSQLiteDatabase::GetFieldNum()
{
	int result = 0;
	if (m_stmt)
	{
		result = sqlite3_column_count(m_stmt);
	}
	return result;
}

/************************************************************************/
/*	获取字段的类型														*/
/************************************************************************/
int CSQLiteDatabase::GetFieldType(int nFieldIdx)
{
	int result = 0;
	if (m_stmt && nFieldIdx < m_nFieldCount)
	{
		result = sqlite3_column_type(m_stmt, nFieldIdx);
	}
	return result;
}

/************************************************************************/
/*   根据列号获取字段名                                                  */
/************************************************************************/
bool CSQLiteDatabase::GetFieldName(int nFieldIdx, tstring & tsFieldName)
{
	bool result = false;
	if (nFieldIdx < m_nFieldCount)
	{
		_TCHAR *p = 0;
#ifdef  UNICODE   
		p = (_TCHAR *)sqlite3_column_name16(m_stmt, nFieldIdx);
#else  
		p = (_TCHAR *)sqlite3_column_name(m_stmt, nFieldIdx);
#endif
		tsFieldName = (p != 0) ? p : _T("");
		result = true;
	}
	return result;
}



/************************************************************************/
/* 开始事务                                                                     */
/************************************************************************/
bool CSQLiteDatabase::BeginTransaction()  
{  
	bool result = false;
	char * errmsg = 0;

	result = (sqlite3_exec(m_db, "BEGIN TRANSACTION;", NULL, NULL, &errmsg) != SQLITE_OK) ? false : true;

	return result;  
}  

/************************************************************************/
/*  提交事务                                                                    */
/************************************************************************/
bool CSQLiteDatabase::CommitTransaction()  
{
	bool result = false;
	char * errmsg = 0;

	result = (sqlite3_exec(m_db, "COMMIT TRANSACTION;;", NULL, NULL, &errmsg) != SQLITE_OK) ? false : true;

	return result;
}  

/************************************************************************/
/*  回滚事务                                                                    */
/************************************************************************/
bool CSQLiteDatabase::RollbackTransaction()  
{
	bool result = false;
	char * errmsg = 0;

	result = (sqlite3_exec(m_db, "ROLLBACK  TRANSACTION;", NULL, NULL, &errmsg) != SQLITE_OK) ? false : true;

	return result;
}  

/************************************************************************/
/*      获取上一条错误信息                                               */
/************************************************************************/
tstring CSQLiteDatabase::GetLastError()  
{
	_TCHAR * p = 0;
#ifdef UNICODE   
	p = (_TCHAR *)sqlite3_errmsg16(m_db);
#else  
	p = (_TCHAR *)sqlite3_errmsg(m_db);  
#endif
	return (p != 0) ? p : _T("");
}  







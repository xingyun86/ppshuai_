/**
 *  @Copyright ����˧
 *  @Brief	 ����SQLite���ݿ�Ľӿ�.
 *  @Author	 ����˧
 *  @Date    2015-01-09
 *
 *  ʹ�÷�����
 *     
 *
 *	 �޸ļ�¼ : 
 *	 �� ��            �汾            �޸���                 �޸�����
 *	 2015/8/15	      1.0			 <����˧>				<��װ�˲���SQLite���ݿ�Ľӿ�> 
 */

#include "StdAfx.h"
#include "sqlitedb.h"


/************************************************************************/
/* �������                                                              */
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
/* ��������                                                             */
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
/* �����ݿ�·����
   *param lpDbFlie ���ݿ�·����
   @return ����ɹ�������true�����ʧ�ܣ�����false
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
/* �رմ򿪵����ݿ�                                                      */
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
/*  ִ����ɾ������                                                       */
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
/* ִ�в�ѯ                                                             */
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
/*�Ƿ��и����¼                                                             */
/************************************************************************/
bool CSQLiteDatabase::MoreRecord()
{
    return m_bEof;
}

/************************************************************************/
/*��¼���ƶ�����һ����¼                                                      */
/************************************************************************/
void CSQLiteDatabase::NextRecord()
{
	m_bEof = (sqlite3_step(m_stmt) != SQLITE_ROW) ? false : true;
}

/************************************************************************/
/*  ��ȡ�������ֶ�ֵ                                                                    */
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
/*  ��ȡ�������ֶ�ֵ                                                    */
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
/*	��ȡ�ֶθ���														*/
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
/*	��ȡ�ֶε�����														*/
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
/*   �����кŻ�ȡ�ֶ���                                                  */
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
/* ��ʼ����                                                                     */
/************************************************************************/
bool CSQLiteDatabase::BeginTransaction()  
{  
	bool result = false;
	char * errmsg = 0;

	result = (sqlite3_exec(m_db, "BEGIN TRANSACTION;", NULL, NULL, &errmsg) != SQLITE_OK) ? false : true;

	return result;  
}  

/************************************************************************/
/*  �ύ����                                                                    */
/************************************************************************/
bool CSQLiteDatabase::CommitTransaction()  
{
	bool result = false;
	char * errmsg = 0;

	result = (sqlite3_exec(m_db, "COMMIT TRANSACTION;;", NULL, NULL, &errmsg) != SQLITE_OK) ? false : true;

	return result;
}  

/************************************************************************/
/*  �ع�����                                                                    */
/************************************************************************/
bool CSQLiteDatabase::RollbackTransaction()  
{
	bool result = false;
	char * errmsg = 0;

	result = (sqlite3_exec(m_db, "ROLLBACK  TRANSACTION;", NULL, NULL, &errmsg) != SQLITE_OK) ? false : true;

	return result;
}  

/************************************************************************/
/*      ��ȡ��һ��������Ϣ                                               */
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







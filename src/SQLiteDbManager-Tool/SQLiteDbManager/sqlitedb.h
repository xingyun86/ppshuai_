/////////////////////////////////////////////////////////////////////////
//  @Copyright ����˧
//  @Brief	 ����SQLite���ݿ�Ľӿ�.
//  @Author	 ����˧
//  @Date    2015 - 01 - 09
/////////////////////////////////////////////////////////////////////////


#pragma once  
#include <sqlite3.h>
#pragma comment(lib,"winsqlite.lib")
//typedef int (_cdecl *QueryCallback) (void *para, int n_column, char **column_value, char **column_name); 
class CSQLiteDatabase  
{  
public:
	CSQLiteDatabase(void); 
   	CSQLiteDatabase(const CSQLiteDatabase &sqlLite);
	CSQLiteDatabase& operator=(const CSQLiteDatabase &sqlLite);
	~CSQLiteDatabase(void);
public:  
	/** 
     * �����ݿ�
     * @param   tsDbFlie ���ݿ�·����
     * @return  �򿪳ɹ�������TRUE����ʧ�ܣ�����FALSE       
     */
	bool Open(tstring tsDbFlie);
	   
   /**
    *	�ر����ݿ�  
    */
	void Close();  

	/**
     * ���ݿ��Ƿ��Ѵ�
     * @return  �Ѵ򿪣�����true;�ޣ�����false
     */
	bool IsOpened();

   /** 
    *	ִ����ɾ�ĵ�����
	*	@param lpSql ��ɾ�ĵ�����
	*	@return ִ�гɹ�������true�����󷵻�false
	*/
	bool ExecuteCommand(tstring tsSql);

   /** 
    *	ִ�пɴ���������ɾ�ĵ�����
	*	@param pCmd ��ɾ�ĵ������ַ���
	*	@return ִ�гɹ�������TRUE�����󷵻�FALSE
	*/
	//bool ExecuteDML(SQLiteCommand* pCmd);  


    /** 
     *	ִ�в�ѯ����
     *  @param lpSql   ��ѯ�����ַ���
     *  @return ����   SQLiteDataReader�������ڲ������ݼ�   
     */                                                              
//	SQLiteDataReader ExcuteQuery(LPCTSTR lpSql);  
	bool ExcuteQuery(tstring tsSql);

	/**
	 *  ��ȡ��¼��
	 *  @return ����SQLite��¼��
	 */
	sqlite3_stmt *GetRecordSet();
	
    /** 
     *	ִ�в�ѯ����ص���ʽ��
     *  @param lpSql ��ѯ�����ַ���
     *  @param pCallBack �Զ���Ļص�����
     *  @return ִ�гɹ�����TRUE��ʧ���򷵻�FALSE   
     */   
	//BOOL ExcuteQuery(LPCTSTR lpSql,QueryCallback pCallBack);  

	/**
	 *  �����ֶθ���
	 *  @return �����ֶθ���
     */
	 int GetFieldNum();

	 /**
	 *��ȡ�ֶ�����
	 *@return �����ֶ�����
	 */
	 int GetFieldType(int nFieldIdx);

	 /**
	  *  ��ȡ�ֶ���
	  *  @param nFieldIdx �к�
	  *  @param tsVal �ַ���������ֵ
	  *  @return ��ȡ�ɹ�������true����ȡʧ�ܣ�����false
	  */
	 bool GetFieldName(int nFieldIdx, tstring &tsVal);

	 /*
	  *  �����ֶ���
	  *  @param nCol �к�
	  *  @return �ֶ��� 
	  */
	 tstring GetFieldName(int nFieldIdx);

  	 /**
      *	 �Ƿ��и����¼     
      *	 @return �ǣ�����TRUE;�񣬷���FALSE
      */
    bool MoreRecord();

    /**
     *	�ƶ�����һ����¼
     */
	void NextRecord();

    /**
     *  ��ȡ�����ֶ�ֵ
     *  @param nFieldIdx �к�
     *  @param nVal ��������ֵ
     *  @return ��ȡ�ɹ�������true����ȡʧ�ܣ�����false
     */
	bool GetFieldValue(int nFieldIdx, int& nVal);
	/**
	*  ��ȡ64�����ֶ�ֵ
	*  @param nFieldIdx �к�
	*  @param nVal ��������ֵ
	*  @return ��ȡ�ɹ�������true����ȡʧ�ܣ�����false
	*/
	bool GetFieldValue(int nFieldIdx, sqlite3_int64 & nVal);
    /**
     *  ��ȡ�ַ����ֶ�ֵ
     *  @param nFieldIdx �к�
     *  @param tsVal �ַ���������ֵ
     *  @return ��ȡ�ɹ�������true����ȡʧ�ܣ�����false
     */
	bool GetFieldValue(int nFieldIdx, tstring &tsVal);

    /**
     *  ��ȡ�������ֶ�ֵ
     *  @param nFieldIdx �к�
     *  @param dVal ����������ֵ
     *  @return ��ȡ�ɹ�������true����ȡʧ�ܣ�����false
     */
	bool GetFieldValue(int nFieldIdx, double& dVal);

	/**
	 *  ��ȡ��¼���е�n�е��ַ���ֵ
	 *  @param nFieldIdx �к�
	 *  @����tstring��ʽ��ֵ
	 */
	tstring GetStringValue(int nFieldIdx);

    /**
     * ��ȡ���͸�ʽ��ֵ
     * @param nFieldIdx�к�
     * @return �����ֶ�ֵ
     */
	int GetIntValue(int nFieldIdx);

    /**
     *  ��ȡ˫�����ʽ��ֵ
     *  @param nFieldIdx�к�
     *  @return ˫����ֵ
     */
	double GetDoubleValue(int nFieldIdx);

    /**
     *	��ʼ����
     *	@return �ɹ�����true��ʧ�ܷ���false
     */
	bool BeginTransaction();  

    /**
     *	�ύ����
     *	@return �ɹ�����true��ʧ�ܷ���false
     */ 
	bool CommitTransaction();  


	/**
     *	�ع�����
     *	@return �ɹ�����true��ʧ�ܷ���false
     */ 
	bool RollbackTransaction();  

	/**
     *	��ȡ���һ��������Ϣ
     *	@return ���ش�����Ϣ
     */
	tstring GetLastError();
public:
private:  
	sqlite3 *m_db;  // 
    sqlite3_stmt *m_stmt;
    bool m_bEof; //�Ƿ�Ǽ�¼�������һ��
	int m_nFieldCount;  //�ֶθ���
	bool m_bConnect;   //�Ƿ��
};  
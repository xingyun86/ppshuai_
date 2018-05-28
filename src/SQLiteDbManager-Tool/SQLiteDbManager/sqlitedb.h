/////////////////////////////////////////////////////////////////////////
//  @Copyright 裴鹏帅
//  @Brief	 操作SQLite数据库的接口.
//  @Author	 裴鹏帅
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
     * 打开数据库
     * @param   tsDbFlie 数据库路径名
     * @return  打开成功，返回TRUE，打开失败，返回FALSE       
     */
	bool Open(tstring tsDbFlie);
	   
   /**
    *	关闭数据库  
    */
	void Close();  

	/**
     * 数据库是否已打开
     * @return  已打开，返回true;无，返回false
     */
	bool IsOpened();

   /** 
    *	执行增删改等命令
	*	@param lpSql 增删改等命令
	*	@return 执行成功，返回true，错误返回false
	*/
	bool ExecuteCommand(tstring tsSql);

   /** 
    *	执行可带参数的增删改等命令
	*	@param pCmd 增删改等命令字符串
	*	@return 执行成功，返回TRUE，错误返回FALSE
	*/
	//bool ExecuteDML(SQLiteCommand* pCmd);  


    /** 
     *	执行查询命令
     *  @param lpSql   查询命令字符串
     *  @return 返回   SQLiteDataReader对象，用于操作数据集   
     */                                                              
//	SQLiteDataReader ExcuteQuery(LPCTSTR lpSql);  
	bool ExcuteQuery(tstring tsSql);

	/**
	 *  获取记录集
	 *  @return 返回SQLite记录集
	 */
	sqlite3_stmt *GetRecordSet();
	
    /** 
     *	执行查询命令（回调方式）
     *  @param lpSql 查询命令字符串
     *  @param pCallBack 自定义的回调函数
     *  @return 执行成功返回TRUE，失败则返回FALSE   
     */   
	//BOOL ExcuteQuery(LPCTSTR lpSql,QueryCallback pCallBack);  

	/**
	 *  返回字段个数
	 *  @return 返回字段个数
     */
	 int GetFieldNum();

	 /**
	 *获取字段类型
	 *@return 返回字段类型
	 */
	 int GetFieldType(int nFieldIdx);

	 /**
	  *  获取字段名
	  *  @param nFieldIdx 列号
	  *  @param tsVal 字符串形引用值
	  *  @return 获取成功，返回true；获取失败，返回false
	  */
	 bool GetFieldName(int nFieldIdx, tstring &tsVal);

	 /*
	  *  返回字段名
	  *  @param nCol 列号
	  *  @return 字段名 
	  */
	 tstring GetFieldName(int nFieldIdx);

  	 /**
      *	 是否还有更多记录     
      *	 @return 是，返回TRUE;否，返回FALSE
      */
    bool MoreRecord();

    /**
     *	移动到下一条记录
     */
	void NextRecord();

    /**
     *  获取整型字段值
     *  @param nFieldIdx 列号
     *  @param nVal 整形引用值
     *  @return 获取成功，返回true；获取失败，返回false
     */
	bool GetFieldValue(int nFieldIdx, int& nVal);
	/**
	*  获取64整型字段值
	*  @param nFieldIdx 列号
	*  @param nVal 整形引用值
	*  @return 获取成功，返回true；获取失败，返回false
	*/
	bool GetFieldValue(int nFieldIdx, sqlite3_int64 & nVal);
    /**
     *  获取字符串字段值
     *  @param nFieldIdx 列号
     *  @param tsVal 字符串形引用值
     *  @return 获取成功，返回true；获取失败，返回false
     */
	bool GetFieldValue(int nFieldIdx, tstring &tsVal);

    /**
     *  获取浮点形字段值
     *  @param nFieldIdx 列号
     *  @param dVal 浮点形引用值
     *  @return 获取成功，返回true；获取失败，返回false
     */
	bool GetFieldValue(int nFieldIdx, double& dVal);

	/**
	 *  获取记录集中第n列的字符串值
	 *  @param nFieldIdx 列号
	 *  @返回tstring格式列值
	 */
	tstring GetStringValue(int nFieldIdx);

    /**
     * 获取整型格式的值
     * @param nFieldIdx列号
     * @return 整形字段值
     */
	int GetIntValue(int nFieldIdx);

    /**
     *  获取双浮点格式的值
     *  @param nFieldIdx列号
     *  @return 双浮点值
     */
	double GetDoubleValue(int nFieldIdx);

    /**
     *	开始事务
     *	@return 成功返回true，失败返回false
     */
	bool BeginTransaction();  

    /**
     *	提交事务
     *	@return 成功返回true，失败返回false
     */ 
	bool CommitTransaction();  


	/**
     *	回滚事务
     *	@return 成功返回true，失败返回false
     */ 
	bool RollbackTransaction();  

	/**
     *	获取最近一条错误信息
     *	@return 返回错误信息
     */
	tstring GetLastError();
public:
private:  
	sqlite3 *m_db;  // 
    sqlite3_stmt *m_stmt;
    bool m_bEof; //是否非记录集的最后一条
	int m_nFieldCount;  //字段个数
	bool m_bConnect;   //是否打开
};  
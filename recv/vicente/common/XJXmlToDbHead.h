#ifndef _XJXmlToDbHead_H_
#define _XJXmlToDbHead_H_
/** @brief			 XJXmlToDb.dll�ӿڳ�ʼ���õ����ݿ��������־����*/
#include "define.h"
typedef struct  
{
	/** @brief			 ���ݿ�����   1 -- sqlserver , 2 -- oracle  3 -- mysql*/
	int  dbtype;

	/** @brief           ���ݿ�ip*/
	string ip;		

	/** @brief           ���ݿ���*/
	string db;	
	
	/** @brief			 �û���  */
	string user;		
	
	/** @brief           ����    */
	string password;     
	
	/** @brief           ��־����*/
	int loglevel;		
	
	/** @brief           ��־·��*/
	char m_szLogPath[FILE_NAME_MAX_LEN];
	
} db_parameter;
/** @brief		     �ص���������error��Ϣ�����*/
const int ERRORMES    = -1;
const int WARNINGMES  = 0;
const int SUCCESSMES  = 1;
/** @brief		     ��������*/
const int UTF8CODE    = 0;
const int GB2312CODE  = 1;
const int UNICODECODE = 2;
const int GBKCODE     = 3;
const int ASCIICODE   = 4;
/** @brief			 �ص�������������   ����n Ϊ�����ֶ�Ĭ��Ϊ0*/
typedef int (* FUNCallError)(const char * error ,XJHANDLE instance=NULL,int n = 0);
/** @brief			 ���ڽ��涨��ʽ��xml���ݵ������ݿ�*/
typedef int (*XmlToDb)(const char * xml_file, XJHANDLE instance = NULL, FUNCallError f_CallPrint = NULL,int encoding = 0);
/** @brief			 ���ڳ�ʼ���ӿ���   ����f_CallPrint Ĭ��Ϊ��  ����Ҫ���ص�������ִ�����������*/
typedef int (*InitXmlToDb)(const db_parameter * in_dbparam);
#endif
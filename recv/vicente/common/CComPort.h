#ifndef _CCOMPORT_H_
#define _CCOMPORT_H_

/*************************************************************
 类		名: CComPort
 父		类: 无
 功能概要 : 串口操作类，完成串口的打开、关闭、读写数据等操作，
		  : 同时定义了和串口操作相关的数据结构等。
 使用范例 : 
 注		意:
			编译该文件时，需指明具体的系统平台。
*************************************************************/

#ifdef OS_WINDOWS

	#include <Windows.h>
	#include <stdio.h>

	typedef HANDLE		HRSPORT;
#endif/*OS_WINDOWS*/

#ifdef OS_LINUX

	#include <stdio.h>	
	#include <unistd.h>
	#include <sys/stat.h>   
	#include <fcntl.h>
	#include <termios.h>  

	typedef int			HRSPORT;
#endif/*OS_LINUX*/

const unsigned int	COMM_READBUFF_LEN=2048;//2k
const unsigned int	COMM_WRITEBUFF_LEN=1024;//1k

typedef enum 
{
	rsbaudate_110 = 0,
	rsbaudate_300,	
	rsbaudate_600,	
	rsbaudate_1200,	
	rsbaudate_2400,	
	rsbaudate_4800,	
	rsbaudate_9600,	
	rsbaudate_19200,
	rsbaudate_38400,	
	rsbaudate_57600,	
	rsbaudate_115200,
}rsbaudate;

typedef enum
{
	rsparity_none = 0,
	rsparity_even,
	rsparity_odd,
	rsparity_space,
}rsparity;

typedef enum
{
	rsdatabit_5 = 0,
	rsdatabit_6,
	rsdatabit_7,
	rsdatabit_8,
}rsdatabit;

typedef enum
{
	rsstopbit_1 = 0,
	rsstopbit_1_5,
	rsstopbit_2,
}rsstopbit;

typedef struct 
{
	int fBinary;
	rsbaudate baudrate;
	rsdatabit databit;
	rsstopbit stopbit;
	rsparity parity;
}rsport_attr;

typedef enum
{
	DGRAM_ALL = 0,
	DGRAM_HEAD,
	DGRAM_BODY
}COMM_READ_TYPE;

/**	
* @brief	串口读缓冲区结构体。
*/
typedef struct 
{
	unsigned int	m_len;
	unsigned char	m_buff[COMM_READBUFF_LEN];/*unsigned char buff*/
}read_buff;

/**	
* @brief	串口写缓冲区结构体。
*/
typedef struct 
{
	unsigned int	m_len;
	unsigned char	m_buff[COMM_WRITEBUFF_LEN];/*unsigned char buff*/
}write_buff;

class CComPort
{
private:

	/**	
	* @brief	串口号。
	*/
	unsigned short	m_port_no;

	/**	
	* @brief	串口属性。
	*/
	rsport_attr		m_port_attr;

	/**	
	* @brief	串口句柄。
	*/
	HRSPORT m_comm_port;

	/**	
	* @brief	串口读超时，单位：毫秒。
	*/
	unsigned int m_nReadTimeout;

	/**	
	* @brief	串口写超时，单位：毫秒。
	*/
	unsigned int m_nWriteTimeout;

	/**	
	* @brief	串口读缓冲区。
	*/
	read_buff	m_readbuff;

	/**	
	* @brief	串口写缓冲区。
	*/
	write_buff	m_writebuff;

private:
	
	/*************************************************************
	 函 数 名 : setup_attr()
	 功能概要 : 设置串口属性，必须open成功后再调用该函数。
	 返 回 值 : 0为成功；其它为失败
	 参    数 : 
				无
	*************************************************************/
	int setup_attr();

protected:

public:

	/*************************************************************
	 函 数 名 : CComPort()
	 功能概要 : 构造函数
	 返 回 值 : 无
	 参    数 : 
				无
	*************************************************************/
	CComPort();

	/*************************************************************
	 函 数 名 : ~CComPort()
	 功能概要 : 析构函数
	 返 回 值 : 无
	 参    数 : 
				无
	*************************************************************/
	~CComPort();

	/*************************************************************
	 函 数 名 : do_config()
	 功能概要 : 对串口的参数进行配置
	 返 回 值 : 0为成功；其它为失败
	 参    数 : 
				port_no		串口号0、1、2
				baudate		波特率
				data_bit	数据位
				stop_bit	停止位
				parity		校验类型'e'、'o'、或'n'
				read_timeout	读超时间隔，默认500，单位：毫秒
				write_timeout	写超时间隔，默认500，单位：毫秒
	*************************************************************/
	int do_config(	unsigned short	port_no, 
					unsigned int	baudate, 
					unsigned short	data_bit, 
					unsigned short	stop_bit, 
					char			parity,
					unsigned int	read_timeout=500,
					unsigned int	write_timeout=500);

	/*************************************************************
	 函 数 名 : query_config()
	 功能概要 : 获取串口的参数配置
	 返 回 值 : 0为成功；其它为失败
	 参    数 : 
				port_no		串口号0、1、2
				baudate		波特率
				data_bit	数据位
				stop_bit	停止位
				parity		校验类型'e'、'o'、或'n'
				read_timeout	读超时间隔，单位：毫秒
				write_timeout	写超时间隔，单位：毫秒
	*************************************************************/
	int query_config(	unsigned short	&port_no, 
						unsigned int	&baudate, 
						unsigned short	&data_bit, 
						unsigned short	&stop_bit, 
						char			&parity,
						unsigned int	&read_timeout,
						unsigned int	&write_timeout);

	/*************************************************************
	 函 数 名 : open()
	 功能概要 : 打开串口
	 返 回 值 : 0为成功；其它为失败
	 参    数 : 
				无
	*************************************************************/
	int open();

	/*************************************************************
	 函 数 名 : close()
	 功能概要 : 关闭串口
	 返 回 值 : 0为成功；其它为失败
	 参    数 : 
				无
	*************************************************************/
	int close();

	/*************************************************************
	 函 数 名 : read_bytes()
	 功能概要 : 从串口读取数据到对象外的缓冲区
	 返 回 值 : 0为读超时；>0为读到的字节数；<0为失败
	 参    数 : 
				pBuffer			输出缓冲区指针
				nBytesToRead	待读出的字节数
	*************************************************************/
	int read_bytes(char* pBuffer, unsigned int nBytesToRead);

	/*************************************************************
	 函 数 名 : write_bytes()
	 功能概要 : 把对象外缓冲区的数据写入串口
	 返 回 值 : 0为串口被关闭；>0为写入的字节数；<0为失败
	 参    数 : 
				pBuffer			待写入数据缓冲区的指针
				nBytesToWrite	待写入的字节数
	*************************************************************/
	int write_bytes(char* pBuffer, unsigned int nBytesToWrite);

	/*************************************************************
	 函 数 名 : read_to_buff()
	 功能概要 : 从串口读取数据到对象缓冲区
	 返 回 值 : 0为读超时；>0为读到的字节数；<0为失败
	 参    数 : 
				bytes_to_read		待读出的字节数
				flag				读标志：
											DGRAM_ALL	一次读完
											DGRAM_HEAD	读报头
											DGRAM_BODY	读报体
											报头和报体必须连续完成
	*************************************************************/
	int read_to_buff(unsigned int bytes_to_read=COMM_READBUFF_LEN, 
					 unsigned int  flag=DGRAM_ALL);

	/*************************************************************
	 函 数 名 : write_from_buff()
	 功能概要 : 把对象缓冲区的数据写入串口
	 返 回 值 : 0为串口被关闭；>0为写入的字节数；<0为失败
	 参    数 : 
				无
	*************************************************************/
	int write_from_buff();

	/*************************************************************
	 函 数 名 : purge_comm()
	 功能概要 : 清空串口的读写缓冲区
	 返 回 值 : 0为成功；其它为失败
	 参    数 : 
				read	非0表示清空读缓冲区；0表示不清空
				write	非0表示清空写缓冲区；0表示不清空
	*************************************************************/
	int purge_comm(int read, int write); 

	/*************************************************************
	 函 数 名 : get_read_buff()
	 功能概要 : 取对象读缓冲区的指针
	 返 回 值 : 非NULL为成功；NULL为失败
	 参    数 : 
				无
	*************************************************************/
	read_buff	*get_read_buff();
	
	/*************************************************************
	 函 数 名 : get_write_buff()
	 功能概要 : 取对象写缓冲区的指针
	 返 回 值 : 非NULL为成功；NULL为失败
	 参    数 : 
				无
	*************************************************************/
	write_buff	*get_write_buff();

	/*************************************************************
	 函 数 名 : get_com_handle()
	 功能概要 : 得到串口操作句柄
	 返 回 值 : 非NULL为成功；NULL为失败
	 参    数 : 
			    无
	*************************************************************/
	HRSPORT	get_com_handle();
};
#endif/*_CCOMPORT_H_*/

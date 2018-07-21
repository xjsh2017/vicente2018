#ifndef _CCOMPORT_H_
#define _CCOMPORT_H_

/*************************************************************
 ��		��: CComPort
 ��		��: ��
 ���ܸ�Ҫ : ���ڲ����࣬��ɴ��ڵĴ򿪡��رա���д���ݵȲ�����
		  : ͬʱ�����˺ʹ��ڲ�����ص����ݽṹ�ȡ�
 ʹ�÷��� : 
 ע		��:
			������ļ�ʱ����ָ�������ϵͳƽ̨��
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
* @brief	���ڶ��������ṹ�塣
*/
typedef struct 
{
	unsigned int	m_len;
	unsigned char	m_buff[COMM_READBUFF_LEN];/*unsigned char buff*/
}read_buff;

/**	
* @brief	����д�������ṹ�塣
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
	* @brief	���ںš�
	*/
	unsigned short	m_port_no;

	/**	
	* @brief	�������ԡ�
	*/
	rsport_attr		m_port_attr;

	/**	
	* @brief	���ھ����
	*/
	HRSPORT m_comm_port;

	/**	
	* @brief	���ڶ���ʱ����λ�����롣
	*/
	unsigned int m_nReadTimeout;

	/**	
	* @brief	����д��ʱ����λ�����롣
	*/
	unsigned int m_nWriteTimeout;

	/**	
	* @brief	���ڶ���������
	*/
	read_buff	m_readbuff;

	/**	
	* @brief	����д��������
	*/
	write_buff	m_writebuff;

private:
	
	/*************************************************************
	 �� �� �� : setup_attr()
	 ���ܸ�Ҫ : ���ô������ԣ�����open�ɹ����ٵ��øú�����
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊʧ��
	 ��    �� : 
				��
	*************************************************************/
	int setup_attr();

protected:

public:

	/*************************************************************
	 �� �� �� : CComPort()
	 ���ܸ�Ҫ : ���캯��
	 �� �� ֵ : ��
	 ��    �� : 
				��
	*************************************************************/
	CComPort();

	/*************************************************************
	 �� �� �� : ~CComPort()
	 ���ܸ�Ҫ : ��������
	 �� �� ֵ : ��
	 ��    �� : 
				��
	*************************************************************/
	~CComPort();

	/*************************************************************
	 �� �� �� : do_config()
	 ���ܸ�Ҫ : �Դ��ڵĲ�����������
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊʧ��
	 ��    �� : 
				port_no		���ں�0��1��2
				baudate		������
				data_bit	����λ
				stop_bit	ֹͣλ
				parity		У������'e'��'o'����'n'
				read_timeout	����ʱ�����Ĭ��500����λ������
				write_timeout	д��ʱ�����Ĭ��500����λ������
	*************************************************************/
	int do_config(	unsigned short	port_no, 
					unsigned int	baudate, 
					unsigned short	data_bit, 
					unsigned short	stop_bit, 
					char			parity,
					unsigned int	read_timeout=500,
					unsigned int	write_timeout=500);

	/*************************************************************
	 �� �� �� : query_config()
	 ���ܸ�Ҫ : ��ȡ���ڵĲ�������
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊʧ��
	 ��    �� : 
				port_no		���ں�0��1��2
				baudate		������
				data_bit	����λ
				stop_bit	ֹͣλ
				parity		У������'e'��'o'����'n'
				read_timeout	����ʱ�������λ������
				write_timeout	д��ʱ�������λ������
	*************************************************************/
	int query_config(	unsigned short	&port_no, 
						unsigned int	&baudate, 
						unsigned short	&data_bit, 
						unsigned short	&stop_bit, 
						char			&parity,
						unsigned int	&read_timeout,
						unsigned int	&write_timeout);

	/*************************************************************
	 �� �� �� : open()
	 ���ܸ�Ҫ : �򿪴���
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊʧ��
	 ��    �� : 
				��
	*************************************************************/
	int open();

	/*************************************************************
	 �� �� �� : close()
	 ���ܸ�Ҫ : �رմ���
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊʧ��
	 ��    �� : 
				��
	*************************************************************/
	int close();

	/*************************************************************
	 �� �� �� : read_bytes()
	 ���ܸ�Ҫ : �Ӵ��ڶ�ȡ���ݵ�������Ļ�����
	 �� �� ֵ : 0Ϊ����ʱ��>0Ϊ�������ֽ�����<0Ϊʧ��
	 ��    �� : 
				pBuffer			���������ָ��
				nBytesToRead	���������ֽ���
	*************************************************************/
	int read_bytes(char* pBuffer, unsigned int nBytesToRead);

	/*************************************************************
	 �� �� �� : write_bytes()
	 ���ܸ�Ҫ : �Ѷ����⻺����������д�봮��
	 �� �� ֵ : 0Ϊ���ڱ��رգ�>0Ϊд����ֽ�����<0Ϊʧ��
	 ��    �� : 
				pBuffer			��д�����ݻ�������ָ��
				nBytesToWrite	��д����ֽ���
	*************************************************************/
	int write_bytes(char* pBuffer, unsigned int nBytesToWrite);

	/*************************************************************
	 �� �� �� : read_to_buff()
	 ���ܸ�Ҫ : �Ӵ��ڶ�ȡ���ݵ����󻺳���
	 �� �� ֵ : 0Ϊ����ʱ��>0Ϊ�������ֽ�����<0Ϊʧ��
	 ��    �� : 
				bytes_to_read		���������ֽ���
				flag				����־��
											DGRAM_ALL	һ�ζ���
											DGRAM_HEAD	����ͷ
											DGRAM_BODY	������
											��ͷ�ͱ�������������
	*************************************************************/
	int read_to_buff(unsigned int bytes_to_read=COMM_READBUFF_LEN, 
					 unsigned int  flag=DGRAM_ALL);

	/*************************************************************
	 �� �� �� : write_from_buff()
	 ���ܸ�Ҫ : �Ѷ��󻺳���������д�봮��
	 �� �� ֵ : 0Ϊ���ڱ��رգ�>0Ϊд����ֽ�����<0Ϊʧ��
	 ��    �� : 
				��
	*************************************************************/
	int write_from_buff();

	/*************************************************************
	 �� �� �� : purge_comm()
	 ���ܸ�Ҫ : ��մ��ڵĶ�д������
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊʧ��
	 ��    �� : 
				read	��0��ʾ��ն���������0��ʾ�����
				write	��0��ʾ���д��������0��ʾ�����
	*************************************************************/
	int purge_comm(int read, int write); 

	/*************************************************************
	 �� �� �� : get_read_buff()
	 ���ܸ�Ҫ : ȡ�������������ָ��
	 �� �� ֵ : ��NULLΪ�ɹ���NULLΪʧ��
	 ��    �� : 
				��
	*************************************************************/
	read_buff	*get_read_buff();
	
	/*************************************************************
	 �� �� �� : get_write_buff()
	 ���ܸ�Ҫ : ȡ����д��������ָ��
	 �� �� ֵ : ��NULLΪ�ɹ���NULLΪʧ��
	 ��    �� : 
				��
	*************************************************************/
	write_buff	*get_write_buff();

	/*************************************************************
	 �� �� �� : get_com_handle()
	 ���ܸ�Ҫ : �õ����ڲ������
	 �� �� ֵ : ��NULLΪ�ɹ���NULLΪʧ��
	 ��    �� : 
			    ��
	*************************************************************/
	HRSPORT	get_com_handle();
};
#endif/*_CCOMPORT_H_*/

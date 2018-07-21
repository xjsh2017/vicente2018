#include "CComPort.h"

/**
* @name		xj_rsport_open
* @brief	打开串口。
* @param	rsport_nr	串口号，从1开始。
* @return	非NULL为成功；NULL为失败。
* @note 
*/
HRSPORT xj_rsport_open(int rsport_nr);

/**
* @name		xj_rsport_close
* @brief	关闭串口。
* @param	rsport	串口句柄。
* @return	无
* @note 
*/
void xj_rsport_close(HRSPORT rsport);

/**
* @name		xj_rsport_setup
* @brief	设置串口的属性。
* @param	rsport	串口句柄。
* @param	attr	属性结构指针。
* @return	0为成功；非0为失败。
* @note		成功打开串口后，才能执行此操作。
*/
int xj_rsport_setup(HRSPORT rsport, const rsport_attr* attr);

/**
* @name		xj_rsport_set_timeout
* @brief	设置串口读写超时。
* @param	rsport			串口句柄。
* @param	read_timeout	读超时，单位：毫秒。
* @param	write_timeout	写超时，单位：毫秒。
* @return	0为成功；非0为失败。
* @note		成功打开串口后，才能执行此操作。
*/
int xj_rsport_set_timeout(HRSPORT rsport, 
						  unsigned int read_timeout/*in msecs*/, 
						  unsigned int write_timeout/*in msecs*/);

/**
* @name		xj_rsport_write
* @brief	串口写数据。
* @param	rsport		串口句柄。
* @param	data		待写数据缓冲区的指针。
* @param	data_len	待写数据的长度。
* @return	>=0为写入的字节长度；-1为错误。
* @note 
*/
int xj_rsport_write(HRSPORT rsport, const char* data, int data_len);

/**
* @name		xj_rsport_read
* @brief	串口读数据。
* @param	rsport			串口句柄。
* @param	buffer			读数据缓冲区的指针。
* @param	bytes_to_read	读数据缓冲区的长度。
* @return	>=0为读出的字节长度；-1为错误。
* @note 
*/
int xj_rsport_read(HRSPORT rsport, char* buffer, int bytes_to_read);

/**
* @name		xj_rsport_purge
* @brief	串口缓冲区读或写清空。
* @param	rsport			串口句柄。
* @param	f_purge_read	读清空标志：非0为清空；0为不清空。
* @param	f_purge_write	写清空标志：非0为清空；0为不清空。
* @return	0为成功；非0为失败。
* @note 
*/
int xj_rsport_purge(HRSPORT rsport, int f_purge_read, int f_purge_write);

CComPort::CComPort()
{
	m_port_no = 1;
	m_port_attr.fBinary = 1;
	m_port_attr.baudrate = rsbaudate_9600;
	m_port_attr.parity = rsparity_none;
	m_port_attr.stopbit = rsstopbit_1;
	m_port_attr.databit = rsdatabit_8;
	m_comm_port = 0;

	m_nReadTimeout = 500;
	m_nWriteTimeout = 500;
}

CComPort::~CComPort()
{
}

HRSPORT CComPort::get_com_handle()
{
	return m_comm_port;
}

int CComPort::do_config(	unsigned short	port_no, 
							unsigned int	baudate, 
							unsigned short	data_bit, 
							unsigned short	stop_bit, 
							char			parity,
							unsigned int	read_timeout,
							unsigned int	write_timeout)
{
	m_port_no = port_no;

	//	baudrate=?; 
	//可选: 110,300,600,1200,2400,4800,9600,19200,38400,57600,115200
	switch(baudate)
	{
	case 110:		
		m_port_attr.baudrate = rsbaudate_110;
		break;
	case 300:
		m_port_attr.baudrate = rsbaudate_300;
		break;
	case 600:
		m_port_attr.baudrate = rsbaudate_600;
		break;
	case 1200:
		m_port_attr.baudrate = rsbaudate_1200;
		break;
	case 2400:
		m_port_attr.baudrate = rsbaudate_2400;
		break;
	case 4800:
		m_port_attr.baudrate = rsbaudate_4800;
		break;
	case 9600:
		m_port_attr.baudrate = rsbaudate_9600; 
		break;
	case 19200:
		m_port_attr.baudrate = rsbaudate_19200;
		break;
	case 38400:
		m_port_attr.baudrate = rsbaudate_38400;
		break;
	case 57600:
		m_port_attr.baudrate = rsbaudate_57600;
		break;
	case 115200:
		m_port_attr.baudrate = rsbaudate_115200;
		break;
	default:
		return -1; 
	}

	//parity=?;可选: even, odd, no, space
	switch(parity)
	{
	case 'e':
	case 'E':
		m_port_attr.parity = rsparity_even;
		break;
	case 'o':
	case 'O':
		m_port_attr.parity = rsparity_odd;
		break;
	case 'n':
	case 'N':
		m_port_attr.parity = rsparity_none;
		break;
	case 's':
	case 'S':
		m_port_attr.parity = rsparity_space;
		break;
	default :
		return -1;
		break;
	}
	
	//datebits=?; 可选: 5,6,7,8 默认为8
	switch(data_bit)
	{
	case 5:
		m_port_attr.databit = rsdatabit_5;
		break;
	case 6:
		m_port_attr.databit = rsdatabit_6;
		break;
	case 7:
		m_port_attr.databit = rsdatabit_7;
		break;
	case 8:
		m_port_attr.databit = rsdatabit_8;
		break;
	default:
		return -1;
	}

	//StopBits=?; 可选: 1, 1.5, 2 默认为1
	switch(stop_bit)
	{
	case 1:
		m_port_attr.stopbit = rsstopbit_1;
		break;
	case 2:
		m_port_attr.stopbit = rsstopbit_2;
		break;
	case 3:
		m_port_attr.stopbit = rsstopbit_1_5;
		break;
	default :
		return -1;
		break;
	}

	//
	m_nReadTimeout = read_timeout;
	m_nWriteTimeout = write_timeout;

	return 0;
}

int CComPort::query_config(		unsigned short	&port_no, 
								unsigned int	&baudate, 
								unsigned short	&data_bit, 
								unsigned short	&stop_bit, 
								char			&parity,
								unsigned int	&read_timeout,
								unsigned int	&write_timeout)
{
	port_no = m_port_no;

	switch(m_port_attr.baudrate)
	{
	case rsbaudate_110:		
		baudate = 110;
		break;
	case rsbaudate_300:
		baudate = 300;
		break;
	case rsbaudate_600:
		baudate = 600;
		break;
	case rsbaudate_1200:
		baudate = 1200;
		break;
	case rsbaudate_2400:
		baudate = 2400;
		break;
	case rsbaudate_4800:
		baudate = 4800;
		break;
	case rsbaudate_9600:
		baudate = 9600; 
		break;
	case rsbaudate_19200:
		baudate = 19200;
		break;
	case rsbaudate_38400:
		baudate = 38400;
		break;
	case rsbaudate_57600:
		baudate = 57600;
		break;
	case rsbaudate_115200:
		baudate = 115200;
		break;
	default:
		return -1;
		break;
	}

	switch(m_port_attr.parity)
	{
	case rsparity_even:
		parity = 'e';
		break;
	case rsparity_odd:
		parity = 'o';
		break;
	case rsparity_none:
		parity = 'n';
		break;	
	case rsparity_space:
		parity = 's';
		break;	
	default:
		return -1;
		break;
	}

	switch(m_port_attr.databit)
	{
	case rsdatabit_5:
		data_bit = 5;
		break;
	case rsdatabit_6:
		data_bit = 6;
		break;
	case rsdatabit_7:
		data_bit = 7;
		break;
	case rsdatabit_8:
		data_bit = 8;
		break;
	default:
		return -1;
		break;
	}

	switch(m_port_attr.stopbit) 
	{
	case rsstopbit_1:
		stop_bit = 1;
		break;
	case rsstopbit_2:
		stop_bit = 2;
		break;
	case rsstopbit_1_5:
		stop_bit = 3;
		break;	
	default:
		return -1;
		break;
	}

	//
	read_timeout = m_nReadTimeout;
	write_timeout = m_nWriteTimeout;
	
	return 0;
}

int CComPort::open()
{
	m_comm_port = xj_rsport_open(m_port_no);
		
	if (m_comm_port==0)
	{
		return -1;
	}

	int returnvalue = setup_attr();

	if (returnvalue!=0)
	{
		xj_rsport_close(m_comm_port);
		return returnvalue;
	}
	
	return 0;
}

int CComPort::setup_attr()
{
	int rev1 = xj_rsport_setup(m_comm_port, &m_port_attr);

	if (rev1==-1)
	{
		return -1;
	}

	int rev2 = xj_rsport_set_timeout(m_comm_port, m_nReadTimeout, m_nWriteTimeout);
	if(rev2==-2)
	{
		return -1;
	}

	return 0;
}

int CComPort::close()
{
	xj_rsport_close(m_comm_port);
	m_comm_port = NULL;
	return 0;
}

int CComPort::read_bytes(char* pBuffer, unsigned int nBytesToRead)
{
	if(nBytesToRead<=0)
	{
		return -1;
	}
	
	char *ptr;
	ptr = pBuffer;

	int RecvNum = 0;
	int bytes_left; 

	bytes_left=nBytesToRead; 
	while(bytes_left>0) 
	{ 
		RecvNum = xj_rsport_read(m_comm_port, ptr, bytes_left);
		if(RecvNum<0) 
		{ 
			return -1;
		} 
		else if(RecvNum==0) 
		{
			break; 
		}

		bytes_left-=RecvNum; 
		ptr+=RecvNum; 
	} 

	return (nBytesToRead-bytes_left);
}

int CComPort::write_bytes(char* pBuffer, unsigned int nBytesToWrite)
{
	if(nBytesToWrite<=0)
	{
		return -1;
	}
	
	char *ptr;
	ptr = pBuffer;

	int WritNum=0;
	int bytes_left; 

	bytes_left=nBytesToWrite; 
	while(bytes_left>0) 
	{ 
		WritNum = xj_rsport_write(m_comm_port, ptr, bytes_left);

		if(WritNum<0) 
		{ 
			return -1;
		} 
		else if(WritNum==0) 
		{
			break; 
		}

		bytes_left-=WritNum; 
		ptr+=WritNum; 
	} 
	
	return (nBytesToWrite-bytes_left);
}

int CComPort::read_to_buff(unsigned int bytes_to_read, unsigned int flag)
{
	int		nTotalLen=0;
	char	*pbuff=0;

	if (bytes_to_read > COMM_READBUFF_LEN)
	{
		return -1;
	}

	// 初始化读指针
	switch(flag) 
	{
	case DGRAM_ALL://单帧报文
	case DGRAM_HEAD://多帧报头
		{
			m_readbuff.m_len = 0;
			pbuff = (char *)m_readbuff.m_buff;
		}
		break;
	case DGRAM_BODY://多帧报体
		{
			if (bytes_to_read+m_readbuff.m_len > COMM_READBUFF_LEN)
			{
				return -1;
			}
			else
			{
				pbuff=(char *)&m_readbuff.m_buff[m_readbuff.m_len];
			}
		}
		break;
	default://单帧报文
		{
			m_readbuff.m_len = 0;
			pbuff = (char *)m_readbuff.m_buff;
		}
		break;
	}

	nTotalLen=read_bytes(pbuff, bytes_to_read);

	if (nTotalLen<0)
		m_readbuff.m_len=0;
	else
		m_readbuff.m_len=nTotalLen;
/*
	int i=0;

	fprintf(stdout, "read len: %d!\n", m_readbuff.m_len);
	for (i=0; i<m_readbuff.m_len; i++)
	{
		fprintf(stdout, "%.2X ", m_readbuff.m_buff[i]);
	}

	fprintf(stdout, "\n");
*/
	return nTotalLen;
}

int CComPort::write_from_buff()
{
	int nTotalLen=0;

	if (m_writebuff.m_len > COMM_WRITEBUFF_LEN)
	{
		return -1;
	}

	nTotalLen=write_bytes((char *)m_writebuff.m_buff, m_writebuff.m_len);

	if (nTotalLen<0)
		m_writebuff.m_len=0;
	else
		m_writebuff.m_len=nTotalLen;
/*
	int i=0;

	fprintf(stdout, "write len: %d!\n", m_writebuff.m_len);
	for (i=0; i<m_writebuff.m_len; i++)
	{
		fprintf(stdout, "%.2X ", m_writebuff.m_buff[i]);
	}

	fprintf(stdout, "\n");
*/
	return nTotalLen;
}

int CComPort::purge_comm(int read, int write)
{
	if (xj_rsport_purge(m_comm_port, read, write) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

read_buff *CComPort::get_read_buff()
{
	return &(m_readbuff);	
}

write_buff *CComPort::get_write_buff()
{
	return &(m_writebuff);
}

//API定义如下

#ifdef OS_WINDOWS

//0 error
HRSPORT xj_rsport_open(int rsport_nr)
{
	char port_name[128];
	sprintf(port_name, "COM%d", rsport_nr);
	HRSPORT hPort = CreateFile(port_name, 
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);
	
	if(hPort == INVALID_HANDLE_VALUE)
	{
		hPort = 0;
	}

	return hPort;
}

void xj_rsport_close(HRSPORT rsport)
{
	CloseHandle(rsport);
}

const int baudratemap[] =
{
	CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800, CBR_9600, 
	CBR_19200, CBR_38400, CBR_57600, CBR_115200,
};

const byte paritymap[] = 
{
	NOPARITY,EVENPARITY,ODDPARITY,SPACEPARITY,
};

const byte databit[] = 
{
	5,6,7,8,
};

const byte stopbit[] = 
{
	ONESTOPBIT,ONE5STOPBITS,TWOSTOPBITS,
};

//0 success; -1 error
int xj_rsport_setup(HRSPORT rsport, const rsport_attr* attr)
{
	DCB dcb;
	GetCommState(rsport, &dcb);

	dcb.fBinary = attr->fBinary;
	dcb.BaudRate= baudratemap[attr->baudrate];
	dcb.fParity = (attr->parity != rsparity_none);
	dcb.Parity = paritymap[attr->parity];
	dcb.ByteSize = databit[attr->databit];
	dcb.StopBits= stopbit[attr->stopbit];
	
	if(!SetCommState(rsport, &dcb))
		return -1;

	return 0;
}

//0 success; -1 error
int xj_rsport_set_timeout(HRSPORT rsport, 
						  unsigned int read_timeout/*in msecs*/, 
						  unsigned int write_timeout/*in msecs*/)
{
	COMMTIMEOUTS cto;

	cto.ReadIntervalTimeout = 0;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = read_timeout;
	cto.WriteTotalTimeoutConstant = write_timeout;
	cto.WriteTotalTimeoutMultiplier = 0;

	if(!SetCommTimeouts(rsport, &cto))
		return -1;

	return 0;
}

//-1 error
int xj_rsport_write(HRSPORT rsport, const char* data, int data_len)
{
	DWORD dwret;

	if(!WriteFile(rsport, data, (DWORD)data_len, &dwret, 0))
		return -1;

	return (int)dwret;

	return 0;
}

//-1 error
int xj_rsport_read(HRSPORT rsport, char* buffer, int bytes_to_read)
{
	DWORD dwret;
	
	if(!ReadFile(rsport, buffer, bytes_to_read, &dwret, 0))
		return -1;

	return (int)dwret;
}

//0 success; -1 error
int xj_rsport_purge(HRSPORT rsport, int f_purge_read, int f_purge_write)
{
	int opt = 0;
	
	if(f_purge_read)
		opt |= PURGE_RXCLEAR;
	if(f_purge_write)
		opt |= PURGE_TXCLEAR;

	if(opt)
	{
		if(!PurgeComm(rsport, opt))
			return -1;
	}

	return 0;
}

#endif/*OS_WINDOWS*/

#ifdef OS_LINUX

//0 error
HRSPORT xj_rsport_open(int rsport_nr)
{
	char port_name[128];
	HRSPORT rsport = 0;

	port_name[0] = 0;
	sprintf(port_name, "/dev/ttyS%d", rsport_nr-1);
	
	if (port_name[0] != 0)
	{
		rsport = open(port_name, O_RDWR | O_NOCTTY);
	}
	
	if (rsport < 0)
		rsport = 0;
	
	return rsport;
}

void xj_rsport_close(HRSPORT rsport)
{
	close(rsport);
}

const speed_t baudratemap[] =
{
	B110, B300, B600, B1200, B2400, B4800, B9600, 
	B19200, B38400, B57600, B115200,
};

const int databit[] = 
{
	CS5,CS6,CS7,CS8,
};

//0 success; -1 error
int xj_rsport_setup(HRSPORT rsport, const rsport_attr* attr)
{
	struct termios options;

	if (tcgetattr(rsport,&options) < 0 ||
		cfsetispeed(&options, (speed_t)baudratemap[attr->baudrate]) < 0 ||
		cfsetospeed(&options, (speed_t)baudratemap[attr->baudrate]) < 0)
	{
		return -1;
	}

	options.c_cflag &= ~CSIZE;
	options.c_cflag |= databit[attr->databit];

	options.c_cflag &= ~CSTOPB;
	/* 设置停止位*/   
	switch (attr->stopbit)
	{
	case rsstopbit_1:
		options.c_cflag &= ~CSTOPB;
		break;
	case rsstopbit_2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		return -1;
		break;
	}
	
	switch (attr->parity)
	{
	case rsparity_none:
		options.c_cflag &= ~PARENB;  
		options.c_iflag &= ~(INPCK | ICRNL);    
		break;
	case rsparity_odd:
		options.c_cflag |= PARODD ;
		options.c_cflag |= PARENB;  
		options.c_iflag |= INPCK;   
		break;
	case rsparity_even:
		options.c_cflag |= PARENB;     
		options.c_cflag &= ~PARODD;   
		options.c_iflag |= INPCK;       
		break;
	case rsparity_space: /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~(INPCK | ICRNL);
		break;
	default:
		return -1;
		break;
	}
	
	/* set the input options */
	options.c_iflag &=~(IXON | IXOFF | IXANY);
	options.c_iflag &=~(INLCR | IGNCR | ICRNL);
	
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	
    /* set the output options */	
	options.c_oflag &= ~OPOST;

	/* set local mode and enable the receiver */
	options.c_cflag |= (CLOCAL | CREAD);

	//strange! tcgetattr failed on linux with errno set to 5(I/O error)
	tcsetattr(rsport, TCSANOW, &options);

	return 0;
}

//0 success; -1 error
int xj_rsport_set_timeout(HRSPORT rsport, 
						  unsigned int read_timeout/*in msecs*/, 
						  unsigned int write_timeout/*in msecs*/)
{
	struct termios options;

	tcgetattr(rsport,&options);

    options.c_cc[VTIME] = read_timeout / 100; 
    options.c_cc[VMIN] = 0;

	tcsetattr(rsport, TCSANOW, &options);
	
	return 0;
}

//-1 error
int xj_rsport_write(HRSPORT rsport, const char* data, int data_len)
{
	return write(rsport, data, data_len);
}

//-1 error
int xj_rsport_read(HRSPORT rsport, char* buffer, int bytes_to_read)
{
	return read(rsport, buffer, bytes_to_read);
}

//0 success; -1 error
int xj_rsport_purge(HRSPORT rsport, int f_purge_read, int f_purge_write)
{
	int opt = 0;
	
	if(f_purge_read)
		opt = TCIFLUSH;
	else
	{
		if(f_purge_write)
			opt = TCIOFLUSH;
		else
			opt = TCOFLUSH;
	}

	if(opt)
	{
		return tcflush(rsport, opt);
	}

	return 0;
}

#endif/*OS_LINUX*/

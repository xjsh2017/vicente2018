
/************************************************************************
							顶级公用模块修订履历


************************************************************************/

/*
 *	2009-4-8 修改SttpDefine.h ，具体如下： 
		1.STTP客户端类型修改原子站为服务端子站，增加客户端子站类型和电厂预警客户端类型
		2.信息类型定义增加软压板校核相关的三张表的定义
	Ver 2.0.4	修改人：程清
 *	2009-4-8 修改LogFile.cpp ，具体如下： 
		1.修改formatadd函数中浮点处理时的长度统计
	Ver 2.0.4	修改人：程清
 *	2009-4-8 修改globalfun.cpp ，具体如下： 
		1.修改GetIniKey中flag = 1的分支return前未关闭文件的BUG
	Ver 2.0.4	修改人：程清
 *	2009-4-8 修改xjlib.cpp ，具体如下： 
		1.修改xj_get_liberror的windows实现
	Ver 2.0.4	修改人：程清
 *	2009-4-8 修改xjlib.cpp ，具体如下： 
		1.修改xj_get_liberror的windows实现
	Ver 2.0.4	修改人：程清
 *	2009-4-15 修改messagelog.cpp ，具体如下： 
		1.修改formatadd函数中浮点处理时的长度统计
	Ver 2.0.6	修改人：程清
 *	2009-4-16 修改net.cpp ，具体如下： 
		1.修改SetOption函数，sun系统下不调用setsockopt系统调用
		2.修改收发超时时的默认值为60s
	Ver 2.0.6	修改人：程清
 *	2009-4-17 修改XJXmlToDbHead.h ，具体如下： 
		1.加入回调函数参数error信息的类别
		2.加入编码类型
		3.加入XmlToDb接口encoding参数
	Ver 2.0.6	修改人：蔡振兴
 *	2009-4-17 修改XJFile.cpp ，具体如下：
		1.open函数增加输出错误码
	Ver 2.0.6	修改人：程清
 *	2009-4-17 修改Net.cpp ，具体如下：
		1.open函数增加输出错误码
	Ver 2.0.7	修改人：程清
 *	2009-4-28 修改SttpDefine.h ，具体如下：
		1.信息类型定义增加断面数据相关三个常量定义、预警相关两项常量定义
	Ver 2.0.8	修改人：程清
 *	2009-4-29 修改golbalfun.cpp ，具体如下：
		1.更新GetIniKey的实现 支持注释和文件末尾不换行
	Ver 2.0.8	修改人：程清
 *	2009-5-6 新加ShareMemory.h ，具体如下：
		1.访问共享内存的API封装
	Ver 2.0.8	修改人：程清
 *	2009-5-8 新加SectionDataWrapper.h和SectionDataWrapper.cpp ，具体如下：
		1.断面数据共享内存库的外覆类
	Ver 2.0.8	修改人：程清
 *	2009-5-13 修改SttpDefine.h ，具体如下：
		1.STTP客户端类型定义增加断面数据客户端的定义
	Ver 2.0.8	修改人：程清
 *	2009-5-21 修改SttpMsgAnalyze.cpp ，具体如下：
		1.修改Sttp_20158_To_Data()中拷贝断面数据内容的问题
	Ver 2.0.8	修改人：程清
 *	2009-5-21 修改SttpMsgAnalyze.cpp ，具体如下：
		1.修改Data_To_Sttp_20158()，不设置结束帧标志
	Ver 2.0.8	修改人：程清
 *	2009-6-1 修改LogFile.cpp ，具体如下：
		1.修改两个Flush函数，判读调用clear的返回值，失败直接返回
		2.修改Clear函数，将原来的删除文件，删除file对象，改为用file对象已创建方式重新打开文件
	Ver 2.0.10	修改人：程清
 *	2009-6-1 修改LogFile.h ，具体如下：
		1.修改日志文件默认容量为0x800000
	Ver 2.0.10	修改人：程清	
 *	2009-6-17 修改STTPStream.cpp ，具体如下：
		1.增加一些跟踪日志
	Ver 2.0.11	修改人：程清
 *	2009-6-18 修改net.cpp ，具体如下：
		1.修改默认超时为60s
	Ver 2.0.11	修改人：程清
 *	2009-6-23 修改messagelog.h和messagelog.cpp ，具体如下：
		1.加入互斥锁，保证重新打开新文件的操作是原子操作，避免多线程调用时出现同步问题
	Ver 2.0.12	修改人：程清
 *	2009-6-25 修改LogFile.cpp和messagelog.cpp ，具体如下：
		1.两个类的FormatAdd()方法增加对“%x”的兼容处理
	Ver 2.0.12	修改人：程清
 *	2009-7-1 修改LogFile.cpp ，具体如下：
		1.修改两个Flush函数，将文件大小超过限制自动清除的操作置于共享锁的保护下
		2.修改clear函数，将更新文件当前大小的操作置于共享锁的保护下
	Ver 2.0.13	修改人：程清
 *	2009-7-8 修改LogFile.h和MessageLog.h ，具体如下：
		1.将各publish接口函数声明为virtual函数，以便多态的创建和访问日志对象
	Ver 2.0.14	修改人：程清
 *	2009-7-14 修改MemSet.h和MemSet.cpp ，具体如下：
		1.增加锁机制,当往MemSet中更新或者插入数据时将锁住,防止读取时调用
	Ver 2.0.15	修改人：李鹏
 *	2009-7-17 修改SttpDefine.h ，具体如下：
	    1.增加三张新表的支持EX_STTP_INFO_DEVICEPARAM_CFG,EX_STTP_INFO_DFRCFG_CFG,EX_STTP_INFO_PDEVICE_CFG
		2.增加一个新的客户端类型双端测距模块(STTPNET_MODEL_XJFAULTLOCATION)
	Ver 2.0.16	修改人：李鹏
 *	2009-7-18 修改SttpMsgAnalyze.cpp ，具体如下：
		1、修正Sttp_20004_To_Data原在取pt_id时取的8字符宽度。
		2、修正Sttp_20016_To_Data的取值时定义的变量名重复。
	Ver 2.0.16	修改人：任文平	
 *	2009-7-18 修改SttpDefine.h ，具体如下：
		1、增加连接中断回调函数原型声明
	Ver 2.0.16	修改人：程清	
 *	2009-8-30 修改MessageLog.cpp和MessageLog.h ，具体如下：
		1、修改SharedOperateStart()只负责更新成员变量中的路径、文件名，不设置LogFile的参数，
		   增加日期文件夹改变时执行扫描清理工作。
		2、修改Open()，先根据成员变量设置LogFile的路径，再打开文件。
		3、增加扫描日志文件天数及按天数保存、清理的功能。
	Ver 2.0.19	修改人：程清
 *	2009-9-1 修改LogFile.cpp ，具体如下：
		1、修改Open()和Close()将判断文件指针的有效性移到互斥锁lock操作的后面。
		2、Flush()增加在打印前判断文件指针有效性。
	Ver 2.0.19	修改人：程清	
 *	2009-9-1 修改XJNet_Stream.cpp和XJNet_Stream.h ，具体如下：
		1、修改Open()和Close()将判断文件指针的有效性移到互斥锁lock操作的后面。
	Ver 2.0.19	修改人：程清	
 *	2009-9-1 修改STTPStream.cpp和STTPStream.h ，具体如下：
		1、增加实现从父类继承的Open()和Close()接口，负责打开关闭日志文件。
		2、重构了PrintSttpMsg修正以前长度统计不准的问题。
		3、重构了报文、错误日志的输出。
	Ver 2.0.19	修改人：程清
 *	2009-9-7 修改XJNet_Acceptor.cpp ，具体如下：
		1、修改Accept函数，Accept成功后，调用Stream对象的open接口。
	Ver 2.0.19	修改人：程清
 *	2009-9-7 修改XJNet_Connector.cpp ，具体如下：
		1、修改SharedConnectStart函数，先打开Stream在根据超时参数决定是否设为非阻塞方式，设置失败时关闭Stream。
		2、修改ShardConnectFinish函数，如果连接失败时，关闭Stream对象。
	Ver 2.0.19	修改人：程清
 *	2009-10-29 修改SttpMsgAnalyze.cpp ，具体如下：
		1、修正SttpMsgAnalyze.cpp文件在Sttp_20086_To_Data函数在取状态值时，未正确取到状态值问题。
	Ver 2.0.21	修改人：任文平
 *  2009-11-4 修改SttpMsgMaker.h和SttpMsgMaker.cpp ，具体如下：
		1、修改Make20144Msg和Make20146Msg函数
	Ver 2.0.22	修改人：程清	
 *  2009-11-10 修改SttpStream.cpp ，具体如下：
		1、修改RecvSTTPMessage函数对于接受小于预期的一律返回0
	Ver 2.0.22	修改人：程清		
 *  2009-11-23 修改define.h ，具体如下：
		1、增加结构BLOB_MUTISQL_DATA和BLOB_DATA，用于传输长度较大的SQL语句(总长度小于8K)
	Ver 2.0.22	修改人：李鹏
 *  2009-11-24 修改BiServerConnector.h和BiServerConnector.cpp ，具体如下：
		1、增加对于A机、B机的在线标志并提供接口查询在线机
 *  2009-11-29 修改MessageLog.cpp ，具体如下：
		1、完成Linux下自动扫描路径及自动清理文件夹只保留指定天数的实现	
	Ver 2.0.22	修改人：程清			
 *  2009-11-30 修改SttpMsgMaker.h和SttpMsgMaker.cpp ，具体如下：
		1、增加Make00215Msg函数
	Ver 2.0.22	修改人：程清		
 *  2009-12-1 修改globalfun.cpp ，具体如下：
		1、修改AddField函数，不将值域进行大写转换
		2、完成PingDestination函数的win版和linux、solaris版的实现
	Ver 2.0.22	修改人：程清		
 *  2009-12-3 修改define.h ，具体如下：
		1、增加PingDestination函数定义
	Ver 2.0.22	修改人：程清	
 *  2009-12-7 SttpMsgAnalyze.cpp ，具体如下：
		1、增加通用文件下装要求（00213）报文转换
		2、增加通用文件下裝通知 (00214)报文转换
		3、增加通用文件下装完毕通知 (00215)报文转换
		4、增加站配置变化通知 (20138)报文转换
	Ver 2.0.22	修改人：rwp	
 *  2009-12-14 SttpMsgAnalyze.cpp ，具体如下：
		1、20146报文两个方向增加通断原因
	Ver 2.0.23	修改人：rwp		
 *  2009-12-14 SttpMsgMaker.cpp ，具体如下：
		1、20146报文制作增加通断原因
	Ver 2.0.23	修改人：程清		
 *  2009-12-29 XJNet_Stream.cpp/h ，具体如下：
		1、增加SendToPeer和RecvFromPeer处理部分接收或发送时继续操作直到完成指定长度或网络出错
		2、Read和Write函数改为调用RecvFromPeer或SendToPeer
	Ver 2.0.24	修改人：程清				   
 *  2009-12-30 Net.cpp ，具体如下：
		1、修改两个read函数，将原来solaris环境下的select操作移到循环内，每次接受之前都调一次select
		2、修改两个write函数，将原来solaris环境下的select操作移到循环内，每次发送之前都调一次select
		   并修改出错时的返回，有部分发送出去时返回实际的发送大小		
	Ver 2.0.24	修改人：程清		
 *  2010-01-05 LogFile.cpp ，具体如下：
		1、修改对Open函数的调用，将CLogFile类中对Open的调用明确指定调CLogFile::Open
	Ver 2.0.24	修改人：程清		
 *  2010-02-08 SttpMsgAnalyze.cpp ，具体如下：
		1、修改Data_To_Sttp_00204函数解决未按外部传的结束标志设置报文的结束标志问题。
		2、版本升级到Ver 2.1.1

		影响范围： 通用文件列表数据转换到多帧报文。
		
	Ver 2.1.1	修改人：任文平		
 *  2010-03-8 Net.cpp ，具体如下：
 		1、修改接收、发送函数，将原先的OS_SUNUNIX标签改为__SOSELECT_TIMEOUT让不支持设置SOCKET
 		收发超时的平台统一定义使用该选项
	Ver 2.1.2	修改人：程清		 		
 *  2010-03-12 xjlib.cpp ，具体如下：
 		1、修改xjtime_to_timet函数，在mktime前调用tzset初始化进程的时区字符串，避免UNIX平台下时间转换
 		受时区的影响
 		Ver 2.1.2 修改人：程清	
 *	2010-03-18 MemSet.cpp，具体如下：
 		1、修改SetValue函数对于无效值的判断，SetValue函数在进来后会先删除原来的值，如果原值为空时，可能导致异常
 		Ver 2.1.2 修改人：李鹏 		
 *  2010-04-21 SttpMsgAnalyze.cpp ，具体如下：
       1、修改Data_To_Sttp_20152函数在STTP转出FULLDATA时在加入vector文件列表时，后续加入元素未清零引起加入到vector的内容存在前面的内容问题。
       2、版本升级到Ver 2.1.3
       影响范围：自动归档场景保持要求（20152）报文STTP转fulldata,
       Ver 2.1.3   修改人：任文平  
 *  2010-04-21 修改MessageLog.cpp/h ，具体如下：
		1、类Linux平台下自动删除日志不成功，原因为对于lstat的调用错误未将路径一并传入，修正之	
	Ver 2.1.3	修改人：程清	
 *  2010-04-22 修改SttpDefine.h ，具体如下：
	1、增加tb_pt_commu_detail,tb_station_status_config和tb_station_status_change三张表的宏定义	
	Ver 2.1.4	修改人：李鹏				
 *  2010-05-05 修改globalfun.cpp ，具体如下：
		1、修改GetFileStatus函数，linux下取文件名时过滤文件名前的路径
	Ver 2.1.4	修改人：程清			
 *  2010-05-21 SttpMsgAnalyze.cpp ，具体如下：
		1、20144报文定义变更，修改代码与基本设计书同步。
		2、增加20160、20161、20162报文双向转换
		3、20146报文变更，修改代码与基本设计书同步。

		影响范围： 20144、20160、20161、20162、20146报文转换。
		
	Ver 2.1.5	修改人：任文平	
 *  2010-05-25 增加XJTime.cpp/h ，具体如下：
		1、增加CXJTime类，支持系统中各种报文格式的互转
	Ver 2.1.5	修改人：程清
 *  2010-05-25 修改Net.cpp ，具体如下：
		1、修改Close函数，去掉服务端模式下	ioctl的调用避免退出时的阻塞
	Ver 2.1.5	修改人：程清	
 *  2010-06-04 修改globalfun.cpp和define.h ，具体如下：
		1、增加格式化网关字符串及取网关的公用函数
	Ver 2.1.5	修改人：程清		
 *  2010-06-04 修改SttpMsgMaker.h和SttpMsgMaker.cpp ，具体如下：
		1、修改Make20144Msg和Make20146Msg函数,加入报文中新加的时间位
	Ver 2.1.5	修改人：程清		
 *  2010-06-10 SttpMsgAnalyze.cpp ，具体如下：		
		1、修改Data_To_Sttp_20116，将其中一些原先硬拷贝规定长度的操作改为strncpy
		考虑长度不足时按实际拷贝
	Ver 2.1.5	修改人：任文平	 
 *  2010-08-20 修改xjlib.h/.cpp ，具体如下：
		1、将原先的HHANDLE 替换为 XJHANDLE
		2、修改xj_modify_time中对于localtime的参数进行类型转换以兼容VS2010的处理
	Ver 2.2.1	修改人：程清		
 *  2010-08-20 修改XJSttpNetAPI.h ，具体如下：
		1、将原先的HHANDLE 替换为 XJHANDLE
	Ver 2.2.1	修改人：程清			
 *  2010-08-20 修改define.h ，具体如下：
		1、将原先自定义的类型HHANDLE 重命名为 XJHANDLE
	Ver 2.2.1	修改人：程清			
 *  2010-08-20 修改flatform.h ，具体如下：
		1、将原先的HHANDLE 替换为 XJHANDLE
	Ver 2.2.1	修改人：程清			
 *  2010-09-02 修改SectionDataWrapper.h ，具体如下：
		1、将原先的HHANDLE 替换为 XJHANDLE
	Ver 2.2.1	修改人：程清			
 *  2010-09-06 增加XJMsgMonitorAPI.h
	Ver 2.2.1	修改人：程清			
 *  2010-09-14 修改SttpMsgMaker.h和SttpMsgMaker.cpp ，具体如下：
		1、新加20126的报文处理函数
	Ver 2.2.1	修改人：程清			
 *  2010-09-21 增加XJMsgMonitorWrapper.h/.cpp
	Ver 2.2.1	修改人：程清			
 *  2010-09-29 修改Net.cpp ，具体如下：
		1、修改SetOptions增加设置Broadcast属性
	Ver 2.2.1	修改人：程清			
 *  2010-09-29 修改define.h ，具体如下：
		1、为SetOptions的参数增加BROADCAST的宏定义
	Ver 2.2.1	修改人：程清				
 *  2010-09-29 SttpMsgAnalyze.cpp ，具体如下：		
		CSttpMsgAnalyze类修正在string resize长度后复制内容时，判断一下长度的合法性，以免在VC2010时越界异常抛异常，涉及函数SetDBDataUnit
		Sttp_00211_To_Data、Sttp_00800_To_Data、Sttp_00901_To_Data、Sttp_10005_To_Data、Sttp_20036_To_Data、Sttp_20037_To_Data、
		Sttp_20039_To_Data、Sttp_20040_To_Data、Sttp_20041_To_Data、Sttp_20044_To_Data、Sttp_20115_To_Data、Sttp_20155_To_Data、Sttp_20158_To_Data
		
		影响范围: 程序稳定性
	Ver 2.2.1	修改人：任文平	 
 *  2010-10-19 修改XJTime.cpp ，具体如下：
		1、修改CXJTime类中对于月份处理的BUG,内部的结构中月份从0开始在使用时需要进行转换
	Ver 2.2.1	修改人：程清				
 *  2010-10-27 修改XJSttpNetAPI.h ，具体如下：
		1、增加装置注册表结构用于XJSttpNet的接口，支持在连接时向服务器注册关注的装置列表
	Ver 2.2.1	修改人：程清		
 *  2010-10-28 修改xjlib.cpp ，具体如下：
		1、修改xj_modify_time中对于localtime的参数进行类型转换以兼容VS2010的处理
	Ver 2.2.1	修改人：程清			
 *  2010-10-28 修改net.cpp ，具体如下：
		1、修改read_udp中socklen的数据类型
	Ver 2.2.1	修改人：程清			
 *  2010-10-28 修改define.h ，具体如下：
		1、修改OS_HPUNIX标签及对于hp-unix平台的兼容支持
	Ver 2.2.1	修改人：程清				
 *  2010-11-05 修改golbalfun.cpp ，具体如下：
		1、修改ping函数支持hp-unix平台
	Ver 2.2.1	修改人：程清			
 *  2010-11-05 修改ShareMemory.h ，具体如下：
		1、修改linux下共享内存文件路径，由原先的根目录改为当前目录解决权限问题
		2、修改Delete函数中linux平台下一处指针引用错误
	Ver 2.2.1	修改人：程清			
 *  2010-11-08 修改define.h ，具体如下：
		1、对于hp-unix平台增加选择_POSIX_C_SOURCE的定义
	Ver 2.2.1	修改人：程清		
 *  2010-11-10 SttpMsgAnalyze.cpp ，具体如下：		
		CSttpMsgAnalyze类修正Data_To_Sttp_20016函数转出报文时如果FULLDATA结果里包含有长值定值值(值长超过8)时，
		分帧转出的STTP报文长度会超过STTP报文体最大大小8192Byte.
		影响范围: 20016FULLDATA转换为STTP报文
	Ver 2.2.1	修改人：任文平
 *  2010-11-10 修改SttpDefine.h ，具体如下：
		1、增加子站测试仪客户端编号的宏定义	
	Ver 2.2.1	修改人：程清	
 *  2010-11-11 修改修改XJMsgMonitorWrapper.h/.cpp、XJMsgMonitorAPI.h，具体如下：
		1、增加103报文监视的专用接口
	Ver 2.2.1	修改人：程清			
 *  2010-11-15 SttpMsgAnalyze.cpp ，具体如下：		
		CSttpMsgAnalyze类增加20167报文两个方向上的处理
	Ver 2.2.1	修改人：任文平				
 *  2010-12-02 SttpMsgAnalyze.cpp/h ，具体如下：		
		CSttpMsgAnalyze类增加录播联网报文的支持
	Ver 2.2.1	修改人：任文平				
 *  2010-12-09 修改SttpDefine.h ，具体如下：
		1、增加录播统一平台客户端编号的宏定义	
		2、增加一次设备ems值配置表的宏定义		
		3、增加录波联网的数据表编号的宏定义	
		4、增加录播联网子站类型编号的宏定义				
	Ver 2.2.1	修改人：程清		
 *  2010-12-09 修改ShareMemory.h ，具体如下：
		1、hp-unix下创建ipc的路径必须有权限所以放在当前目录下，其他类linux的平台放在根目录下
	Ver 2.2.1	修改人：程清				
 *  2010-12-13 修改XJTime.cpp ，具体如下：
		1、构造函数中将时间结构体中的tm_isdst初始化为0（0表示完全不处理夏令时）
		2、拷贝构造函数对tm_isdst字段进行拷贝
	Ver 2.2.2	修改人：程清				
 *  2010-12-13 修改SttpMsgMaker.cpp ，具体如下：
		1、修改Make20144Msg中厂站ID的处理改为strncpy以避免字符串访问越界
	Ver 2.2.2	修改人：程清				
 *  2010-12-13 修改Logfile.cpp ，具体如下：
		1、修改两个Flush操作,将原先的多次写入改为合在一起一次写入
	Ver 2.2.3	修改人：程清			
 *  2011-1-5 修改SttpMsgAnalyze.cpp/h ，具体如下：
		1、新加对20168报文的支持
	Ver 2.3.1	修改人：程清			
 *  2011-1-7 修改SttpMsgAnalyze.cpp/h ，具体如下：
		1、新加对30180、30181、30182报文的支持
	Ver 2.3.1	修改人：程清			
 *  2011-3-8 修改XJTime.cpp ，具体如下：
		1、修改对于103中7字节时间转换时年份处理的BUG
	Ver 2.3.1	修改人：程清			
 *  2011-3-22 修改Mesage.cpp ，具体如下：
		1、将扫描比较文件夹时原先取访问时间比较改为取创建时间比较
	Ver 2.3.1	修改人：程清			
 *  2011-3-25 修改globalfun.cpp和define.h ，具体如下：
		1、增加取文件名中的扩展名的公用函数
	Ver 2.3.1	修改人：程清				
 *  2011-3-28 修改SttpMsgMaker.cpp ，具体如下：
		1、修改Make20156Msg函数中对原因字符串的拷贝，避免内存溢出
	Ver 2.3.1	修改人：程清		
	
 *  2011-5-18 修改具体如下：
		1、SttpDefine.h增加二次设备定值区号配置表号定义

		2、SttpMsgAnalyze.h&cpp增加20169、20170报文的双向转换，原来的20171~20179报文（录波联网早期定义报文）删除。
		20171～20174按基本设计书新增报文定义处理，原来代码实现删除
		
		3、版本升级到Ver 2.3.2

		影响范围：南网远控项目新增功能
	Ver 2.3.2	修改人：任文平	
 *  2011-6-17 修改具体如下 
    (1) define.h :增加solaris平台下对_SOPOLL_TIMEOUT的宏定义支持;
    (2) Net.cpp :增加有_SOPOLL_TIMEOUT宏定义时的socket接手、发送、连接超时处理,该方式下采用系统调用poll(),主要解决在不同平台下原先使用select()函数有文件描述符不能超过1024限制的问题.
		影响范围：solaris下涉及socket通信的所有模块需重新编译.	
	Ver 2.3.3	修改人：姜健琳	
	
 *  2011-7-19 修改具体如下：
		1、SttpMsgAnalyze.cpp 修改10003报文转换为STTP_FULL_DATA时原有函数处理错误（10003报文无记录数的位置）。修改Data_To_Sttp_00210、Sttp_To_Data_00210报文中报文体RII用nFlag来存放不再与报文头RII一致
		2、SttpMsgMaker.cpp 修改Make20156Msg和Make20144Msg中长度计算错误
		3、版本升级到Ver 2.3.4

		影响范围：10003报文转换sttp_full_data结构,210报文双向转换,SttpMsgMaker组织20156与20144报文
		
	Ver 2.3.4	修改人：任文平		
		*  2011-10-10 修改具体如下
		1、解决录波联网30132报文中新增(录波文件的接收时间)字段在Fulldata中位置与其他字段重复问题。
		2、解决BUG(3353)因组织的20026报文内容不正确，错误位置取出文件数目不正确，循环时超过报文体大小时溢出崩溃。在20026报文转结构时增加安全性判断。
	  影响范围：30132、20026报文转换
	Ver 2.3.5	修改人：任文平	

 *  2011-10-13 修改具体如下
		1、增加一个调用初始化动态库需包含的头文件XJInitializingLibHeader.h。
		影响范围：子站测试仪界面程序
	Ver 2.3.6	修改人：李鹏
	
 *  2011-10-18 修改具体如下
		1、增加一个调用中文字符编码转换动态库需类文件GBK2UTF8.h&&GBK2UTF8.cpp
		影响范围：新增功能
	Ver 2.3.7	修改人：任文平	


 *  2012-09-25 修改具体如下
		1、增加一个修改录波CFG文件首行内容使之与保信系统的设备进行关联的文件WaveCfgBindDevice.h/cpp
		
		2、修改define.h中关于AIX下SOCK_LEN的定义，修改为采用AIX系统提供的定义兼容32位和64位,去除宏STTPMSG_QUEUE，
		结构BUFFER与61850底层库存在命名冲突重定义为XJBUFFER
		
		3、CXJSttpAnalyze类重写为调用动态库来实现相应的转换逻辑。原有类封装为一个DLL，原来调用此类的代码不作变更，但
		发布程序时应该在调用目录增加libXJSttpAnalyzer.so
		
		4、修改Net.cpp文件函数ConnectServer在重连时把IP字符清零避免，ConnectServer在多次调用但IP长度不同时复制IP地址不正确问题。
		原BUFFER结构重命名为XJBUFFER后相关代码的变更。CNet类不再打开日志文件。避免同一进程都写CNet.txt文件句柄关闭文件时最后才释放。
		
		5、修改BiServerConnector.h/cpp函数Connect因BiNetConnecter的Connect函数要返回错误原因，本函数返回错误原因参数，在此函数
		内不再写日志避免一个连接器就有一个日志文件太多。
		
		6、修改BiNetConnecter.h/cpp的Connect函数增加返回错误原因参数。
		
		7、增加XJSrvOnlineManagerAPI.h接口文件。以支持多机在线切换功能。
		
		8、CXJDBFacade类增加Connect接口
		
		9、增加CD5kMsgBusFacade.h/cpp实现对D5000消息总线包装，并提供相应的接口。
		
		10、SttpDefine.h增加宏定义（录波联网子站、服务端分站、分站之子站，录波联网服务端子站、61850子站）调度员模块ID，增加表号119~135
		
		11、CSttpMsgMaker函数Make20156Msg复制装置ID时由memcpy修心为strncpy.
		
		12、增加XJServerAccpeterAPI.h用于61850主站
		
		13、增加XJHeartbeatHandlerAPI.h及HeartbeatWrapper.h/cpp用于实现UDP心跳报文的收发判断。
		
		14、增加XJLoadAquirerAPI.h用于负载分数采集。
			
		15、XJSttpNetAPI.h文件增加#include define.h
		
		16、增加XJSysparamAcquirelibHeader.h用于采集系统参数
		
		17、修正MessageLog.cpp中扫描日志目录列表（ScanLogPath）时在WIN平台的提取的为文件访问时间，实际应为文件生成时间，
		在使用此函数获取的列表时会引起误删除。
		
	Ver 2.3.8	修改人：任文平
	
 *  2012-09-25 修改具体如下
		1、SttpDefine.h增加模块ID的宏，用于辅助决策与稳定控制两个系统
	Ver 2.3.9	修改人：任文平	

 *  2012-09-29 修改具体如下
		1、SttpDefine.h增加表ID的宏，表ID新增136-140
	Ver 2.3.10	修改人：李鹏

 *  2012-10-24 修改具体如下
		1、SttpDefine.h增加录波联网几张缺失定义的表和视图ID的宏定义，ID新增范围：1012-1017
							修改人：李鹏	

 *  2012-10-26 修改具体如下
		1、SttpDefine.h增加智能预警系统模块ID的宏定义
	Ver 2.3.11	修改人：任文平		
	
 *  2012-11-12 修改具体如下
		1、增加XJSttpProxyAPI.h解决XJSttpProxyAPI模块编译不过问题。
	Ver 2.3.12	修改人：任文平

 *  2012-11-13 修改具体如下
		1、增加检修单位类型配置表(141)和检修单位配置表(142)表的ID宏定义。
	Ver 2.3.13	修改人：李鹏

 *  2012-11-27 修改具体如下
		1、增加开关量校核基准值表(143)、开关量校核结果日志表(144)、开关量基准修改日志表（145）的表ID宏定义。
	
 *  2012-12-4 修改具体如下
		1、修改globalfun.cpp函数PingDestination，现场发现在某些工程人员以root权限运行过后，再用ipofas权限运行通讯程序，
		如果调PingDestination函数由于没权限写ping.txt，生成文件将失败返回NULL值，在读取文件时对文件指针进行判断避免程
		序崩溃。
		2、define.h中增加新的数据库类型4（达梦数据库）5（金仓数据库）

	Ver 2.3.14	修改人：任文平	
	
 *  2013-01-22 修改具体如下
		1、为刘岩增加模拟量校核的几张表格(146~149)
		2、增加客户端类型为111的“消息总线”客户端类型。以便界面配置时选择类型。
		3、SttpDefine.h增加主保护状态表（150）的表ID宏定义（李鹏）。
		4、修改XJTime.cpp转换为CP56time2a时间时，2000年的转出的时间年字段为100超过其定义的表示范围(0~99)		
	Ver 2.3.15	修改人：任文平		
	
 *	2013-07-11修改文件define.h文件如下
		//增加宏 #define REUSEADDR	 7	   //端口共用
		
 *	2013-07-11修改文件globalfun.cpp文件如下
		//修改PingDestination函数在WIN平台下ping命令返回结果为内容中“time”字符串可能为中文“时间”兼容处理。以免
		返回ping失败
		
 *	2013-07-11修改文件Net.cpp文件如下
		//修改SetOptions函数增加设置Socket端口重用属性。
		
 *	2013-07-11修改文件LogFile.cpp文件如下
		//修改FormatAdd函数计算完毕字符串的最大长度后,再增加255个预留长度，以防计算错误。
		
	Ver 2.3.16	修改人：任文平		
	
 *	2013-07-29修改文件SttpDefine.h文件如下
	1、增加通讯状态监视模块ID定义宏const int STTPNET_MODLE_STATUSMONITER       = 112;
 *	2013-07-29修改文件XJDBFacade.h/cpp文件如下	
	1、增加接口SetLogPath函数定义
	
	Ver 2.3.17	修改人：任文平
	
 *	2013-07-31修改文件MemSet.h、MemSet.cpp如下
	1、启用自定义的双向链表（EntityContainer）,改用stl标准的vector用户存放行数据指针，提高随机访问效率
	2、增加一个移动到指定行数据的接口函数MoveToUserRow(int nRownum)
	
	Ver 2.3.18	修改人：李鹏
	
 *	2013-08-07修改文件SttpMsgParser.cpp文件如下
	1、修改设置MsgRII时最大RII为255的限制，扩展到999,取报文帧总数及当前帧序号时可能因为没有结束符引起转出的数目不正确问题。
 *	2013-08-07修改文件globalfun.cpp文件如下	
	1、修正在生成ping权限不足时，打开文件为NULL，关闭此文件时会引起段错误问题
	 *	2013-08-07修改文件MemSet.cpp文件如下	
	1、	修正公用memset类中移动到最后一行后再下移一行未回到第一行导致movefirst函数功能失效的问题

	Ver 2.6.1	修改人：任文平

 *	2013-09-02修改文件BiNetServerConnector.h&cpp文件如下	
	1、增加connect函数接口参数，以便外部使用者可以控制双服务器的连接顺序。
	2、对于20128报文增加一个来源标志，用于控制总召唤结果的处理。

	Ver 2.6.2	修改人：任文平	

  *	2014-03-19修改文件SttpDefine.h文件如下
  1、增加时钟偏差告警表的的常量定义（const UINT EX_STTP_INFO_TB_DEVCLOCK_OFFSET=151）。
  2、增加两种测距子站类型的常量定义（ 测距系统2010子站const int STTPNET_MODLE_SUBSTATION_WFL2010  = 113
	  测距系统2012子站const int STTPNET_MODLE_SUBSTATION_WFL2012 =  114）。
	修改人：任文平
  *	2014-03-21修改文件define.h文件如下
  1、增加测距装置的字符常量定义（define STRING_DISTANCE "测距装置"）。
	修改人：刘裕桦
  *	2014-03-26修改文件SttpDefine.h文件如下
  1、增加召唤结果映射表的的常量定义（const UINT EX_STTP_INFO_TB_OC_DUTY_CONFIG=152）。
	修改人：李鹏
  *	2014-05-11修改文件define.h文件如下
  1、define.h中增加两种装置类型的定义（故障测距装置虚拟类型TYPE_FD_DEV和故障测距站虚拟类型TYPE_FD_STATION），明确二次设备定义（增加中文描述）。
	修改人：郭华文
  *	2014-05-28修改文件MemSet.cpp文件如下
  1、修正MemSet.cpp中FreeData函数中将数据库清理后vector未清空导致复用时释放出问题的缺陷。
	修改人：李鹏
	
	Ver 2.6.3	修改人：李鹏

  *	2014-07-21修改文件SttpDefine.h文件如下
  1、增加系统编码表的的常量定义（const UINT EX_STTP_INFO_TB_SYSTEM_CODE_CONFIG=153）。
  修改人：李鹏
  *	2014-10-09修改文件define.h文件如下
  1、结构体Field,Condition,MutiSQL_DATA,REALDATA_CONDITION,BLOB_FIELD,BLOB_CONDITION,BLOB_MUTISQL_DATA增加默认构造函数，初始化值防止误用。
  修改人：李鹏
  *	2014-10-16修改文件SttpDefine.h文件如下
  1、增加基准定值模板表的常量定义（const UINT EX_STTP_INFO_ST_STANDARD_SETTING = 154）；
  2、增加定值校核结果概要信息表的常量定义（const UINT EX_STTP_INFO_ST_CHECKSETTING_SUM = 155）；
  3、增加定值单列表表的常量定义（const UINT EX_STTP_INFO_ST_SETTING_ORDER_LIST = 156）；
  4、增加用户与厂站关联关系表的常量定义（const UINT EX_STTP_INFO_TB_USER_RELATED = 157）。
  修改人：李鹏
  *	2014-10-27 修改文件define.h文件如下
  1、加了自动巡检标志的宏定义（#define XJ_STTP_CMD_AUTOCALL_FLAG 2）。
  修改人：黄小倩
  *	2014-11-05 修改文件MemSet.h和MemSet.cpp文件如下
  1、新增两个函数UINT GetCurRowNum()（获取当前遍历的行号）和SQL_ROW GetCurRow()（获取当前数据行的指针）。
  修改人：李鹏
	
	Ver 2.6.4	修改人：李鹏

	*	2015-01-26修改文件MessageLog.cpp文件如下
	1、fix &string[0] with string.data()。
	修改人：郭华文
  *	2015-02-01修改文件XJSttpNetAPI.h文件如下
  1、STTP_FULL_DATA结构体下各个子结构体增加构造函数用户给各子项赋初值。
  修改人：李鹏
  *	2015-02-09修改文件XJDeque.h文件如下
  1、增加两个函数get_front_pop用于获取头节点后删除头节点元素，get_back_pop用于获取尾节点后删除尾节点元素，在多线程环境下如果取头/尾节点与删除头/尾
     节点分开操作容易导致操作异常。
  修改人：李鹏
  
  Ver 2.6.5	修改人：李鹏

  *	2015-05-19修改文件xjlib.cpp，xjlib.h文件如下
  1、xj_thread_create函数中增加对NULL返回值的判断，在资源问题线程启动失败时会返回NULL的结果；
  2、xj_thread_create函数增加一个函数参数，用以传递stack_size的大小，默认为0，某些场景程序线程过多会导致分配失败，减少stacksize的值可行；
  3、xjlib.h增加一个XJTHREAD_DEFAULT_STACK_SIZE的常量定义，默认的栈大小为0；
  4、以上修改针对windows下程序生效，linux/unix下由ulimit相关参数控制。
	修改人：李鹏
  *	2015-06-10修改文件ShareMemory.h文件如下
  1、create、open、Delete函数增加部分关键行为的printf输出；
  2、create、open、Delete函数取消在映射共享内存后将句柄赋值为-1的逻辑，映射失败赋值才有意义。
	修改人：李鹏
  
  Ver 2.6.6	修改人：李鹏
  
   * 2015-07-10修改文件SttpDefine.h文件如下
   1、增加一个数据转发服务模块（STTPNET_MODEL_XJDATAROUTER 20）。
   修改人：李鹏
   * 2015-08-07增加文件XJFtpClientManagerAPI.h文件
   1、增加Ftp操作库XJFtpClientManager的导出函数定义和数据结构申明头文件，供其它模块使用。
   修改人：李鹏
   * 2015-08-13增加文件XJSttpXmlTransformAPI.h文件
   1、增加STTP和XML文件互转库XJSttpXmlTransform的导出函数定义和数据结构申明头文件，供其它模块使用。
   修改人：李鹏
   * 2015-08-31修改文件SttpDefine.h文件如下
   1、增加一个录波分析服务模块（STTPNET_MODEL_WAVE_ANALYST 21）。
	
  Ver 2.6.7	修改人：李鹏

  * 2015-10-13增加文件ComtradeDefine.h
  1、增加一个接口文件夹Interface；
  2、增加Comtrade解析头文件ComtradeDefine.h
  修改人：刘裕桦
  * 2015-10-19修改文件ComtradeDefine.h
  1、增加相关依赖的系统头文件引用，扩充数据结构。
  修改人：刘裕桦
  * 2015-12-17修改文件SttpDefine.h文件
  1、增加通讯服务器类型的相关定义（
     const UINT COMMON_COMMUSERVER = 1;const UINT C61850_COMMUSERVER = 2;const UINT WAVE_COMMUSERVER = 3;）。
  修改人：黄小倩
  * 2016-01-25修改文件SttpDefine.h文件如下
  1、增加一个web同步发送端模块定义（const int STTPNET_MODLE_WEB_PROVIDER_SENDER = 115）。
  修改人：李鹏
  
  Ver 2.6.8	修改人：李鹏

  * 2016-07-19修改文件define.h
  1、更改MFC_MODELING预编译项的名称为MFC_ADO_USED_DEFINE，改项定义为解决使用ado的程序冲突的问题；
  修改人：李鹏
  * 2016-07-20修改文件globalfun.cpp
  1、GetSysTime函数在linux下的取本地时间的函数采用线程安全的localtime_r，并修改相应的实现；
  2、AddField、AddCondition、Zero_SQL_DATA增加预编译命令MFC_ADO_USED_DEFINE判断，防止与使用ado的程序冲突。
  修改人：李鹏
  * 2016-07-21修改文件MemSet.cpp文件
  1、StringToLower函数中，当p_val为NULL时，取消对string的赋值，否则会产生异常。
  修改人：李鹏
  
  Ver 2.6.9	修改人：李鹏

  * 2016-08-31修改文件SttpDefine.h
  1、增加日报表统计概要信息表的定义（const UINT EX_STTP_INFO_TB_STAT_DAILY_BRIEF = 158）；
  2、增加日报表子站统计信息表的定义（const UINT EX_STTP_INFO_TB_STAT_DAILY_DETAIL = 159）；
  3、增加月报表统计概要信息表的定义（const UINT EX_STTP_INFO_TB_STAT_MONTHLY_BRIEF = 160）；
  4、增加月报表子站统计信息表的定义（const UINT EX_STTP_INFO_TB_STAT_MONTHLY_DETAIL = 161）；
  修改人：李鹏
  
  Ver 2.6.10 修改人：李鹏
*/
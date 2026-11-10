/////////////////////////////////////////////////////////////////////////
///@author 中泰证券股份有限公司
///@file xapi4_data_type.h
///@brief 定义api使用的兼容数据基本类型
/////////////////////////////////////////////////////////////////////////
#ifndef _XAPI4_DATA_TYPE_H_
#define _XAPI4_DATA_TYPE_H_

namespace XTPX {
	namespace API {

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_LOG_LEVEL是日志输出级别类型
		/////////////////////////////////////////////////////////////////////////
		typedef enum XTP_LOG_LEVEL {
			XTP_LOG_LEVEL_FATAL, ///<严重错误级别
			XTP_LOG_LEVEL_ERROR, ///<错误级别
			XTP_LOG_LEVEL_WARNING, ///<警告级别
			XTP_LOG_LEVEL_INFO,   ///<info级别
			XTP_LOG_LEVEL_DEBUG,  ///<debug级别
			XTP_LOG_LEVEL_TRACE   ///<trace级别
		}XTP_LOG_LEVEL;

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_PROTOCOL_TYPE是通讯传输协议方式
		/////////////////////////////////////////////////////////////////////////
		typedef enum XTP_PROTOCOL_TYPE
		{
			XTP_PROTOCOL_TCP = 1,	///<采用TCP方式传输
			XTP_PROTOCOL_UDP		///<采用UDP方式传输(仅行情接口支持)
		}XTP_PROTOCOL_TYPE;

	}
}



#endif //_XAPI4_DATA_TYPE_H_

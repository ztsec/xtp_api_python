/////////////////////////////////////////////////////////////////////////
///@author 中泰证券股份有限公司
///@file xtp_api4_struct_common.h
///@brief 定义业务公共数据结构
/////////////////////////////////////////////////////////////////////////

#ifndef _XTP_API4_STRUCT_COMMON_H_
#define _XTP_API4_STRUCT_COMMON_H_

#if defined(_MSC_VER) && _MSC_VER<1600

#ifndef _XTP_API_DATA_TYPE_DEFINE_INT_
#define _XTP_API_DATA_TYPE_DEFINE_INT_

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

#endif // !_XTP_API_DATA_TYPE_DEFINE_INT_

#else
#include <stdint.h>
#endif

#pragma pack(push)
#pragma pack(1)

namespace XTPX {

	namespace API {
		///错误信息的字符串长度
		constexpr int32_t XTP_ERR_MSG_LEN = 124;
		///响应信息
		typedef struct XTPRspInfoStruct
		{
			///错误代码
			int32_t	error_id;
			///错误信息
			char	error_msg[XTP_ERR_MSG_LEN];
		} XTPRI;

	}
}



#pragma pack(pop)

#endif // !_XTP_API4_STRUCT_COMMON_H_

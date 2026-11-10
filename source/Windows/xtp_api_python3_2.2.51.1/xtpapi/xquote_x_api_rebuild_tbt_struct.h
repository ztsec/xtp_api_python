/////////////////////////////////////////////////////////////////////////
///@author 中泰证券股份有限公司
///@file xquote_x_api_rebuild_tbt_struct.h
///@brief 定义行情类相关数据结构
/////////////////////////////////////////////////////////////////////////

#ifndef __XQUOTE_NG_API_REBUILD_TBT_STRUCT_H_
#define __XQUOTE_NG_API_REBUILD_TBT_STRUCT_H_
#include "xquote_x_api_data_type.h"

#pragma pack(push)
#pragma pack(1)

namespace XTPX {

	namespace API {
		//////////////////////////////////////////////////////////////////////////
		///实时行情回补查询相关结构体
		//////////////////////////////////////////////////////////////////////////

		///实时行情回补请求结构体
		typedef struct XTPQuoteRebuildReq
		{
			///请求id 请求端自行管理定义
			int32_t                     request_id;
			///请求数据类型 1-快照 2-逐笔 3-指定股票逐笔
			XTP_QUOTE_REBUILD_DATA_TYPE data_type;
			///请求市场 1-上海  2-深圳
			XTP_EXCHANGE_TYPE           exchange_id;
			///data_type=逐笔或者指定股票逐笔时，表示逐笔通道号
			int16_t                     channel_number;
			///预留
			char                        unuse[2];
			///合约代码 以'\0'结尾  沪深A股6位  期权8位，当data_type=为快照或者指定股票逐笔时使用
			char                        ticker[16];
			///data_type=逐笔或者指定股票逐笔时 表示序列号begin； =快照 表示时间begin(格式为YYYYMMDDHHMMSSsss)
			int64_t                     begin;
			///data_type=逐笔或者指定股票逐笔时 表示序列号end； =快照 表示时间end(格式为YYYYMMDDHHMMSSsss)   逐笔区间：[begin, end]前后闭区间  快照区间：[begin, end)  前闭后开区间      
			int64_t                     end;
		}XTPQuoteRebuildReq;

		///实时行情回补响应结构体
		typedef struct XTPQuoteRebuildResultRsp
		{
			///请求id 请求包带过来的id
			int32_t                     request_id;
			///市场类型 上海 深圳
			XTP_EXCHANGE_TYPE           exchange_id;
			///总共返回的数据条数
			uint32_t                    size;
			///结果类型码
			XTP_REBUILD_RET_TYPE        result_code;
			///逐笔数据 通道号                
			int16_t                     channel_number;
			///预留
			char                        unused[6];
			///逐笔 表示序列号begin; 快照 表示时间begin(格式为YYYYMMDDHHMMSSsss)            
			int64_t                     begin;
			///逐笔 表示序列号end; 快照 表示时间end(格式为YYYYMMDDHHMMSSsss)
			int64_t                     end;

			///结果信息文本
			char                        msg[64];
		}XTPQuoteRebuildResultRsp;

	}//API
}//XTPX
#pragma pack(pop)

#endif // __XQUOTE_API4_REBUILD_TBT_STRUCT_H_

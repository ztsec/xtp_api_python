/////////////////////////////////////////////////////////////////////////
///@author 中泰证券股份有限公司
///@file xtpx_quote_api.h
///@brief 定义行情订阅客户端接口
/////////////////////////////////////////////////////////////////////////

#ifndef _XTPX_QUOTE_API_H_
#define _XTPX_QUOTE_API_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stddef.h>
#include "xtpx_api_data_type.h"
#include "xtpx_api_struct_common.h"
#include "xquote_x_api_rebuild_tbt_struct.h"
#include "xquote_x_api_struct.h"

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_XTPX_MD_API_EXPORT
#define XTPX_MD_API_EXPORT __declspec(dllexport)
#else
#define XTPX_MD_API_EXPORT __declspec(dllimport)
#endif
#else
#define XTPX_MD_API_EXPORT 
#endif

/*!
* \class XTPX::API::QuoteSpi
*
* \brief 行情回调类
*
* \author 中泰证券股份有限公司
* \date 三月 2025
*/
namespace XTPX {
	namespace API {
		class QuoteSpi
		{
		public:

			///当客户端与行情后台通信连接断开时，该方法被调用。
			///@param reason 错误原因，请与错误代码表对应
			///@remark api不会自动重连，当断线发生时，请用户自行选择后续操作。可以在此函数中调用Login重新登录。注意用户重新登录后，需要重新订阅行情
			virtual void OnDisconnected(int reason) { (void)reason; };

			///错误应答
			///@param error_info 当服务器响应发生错误时的具体的错误代码和错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 此函数只有在服务器发生错误时才会调用，一般无需用户处理
			virtual void OnError(XTPRI *error_info) { (void)error_info; };

			///订阅行情应答，包括股票、指数和期权
			///@param ticker 详细的合约订阅情况
			///@param error_info 订阅合约发生错误时的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@remark 每条订阅的合约均对应一条订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) { (void)ticker; (void)error_info; (void)is_last; };

			///退订行情应答，包括股票、指数和期权
			///@param ticker 详细的合约取消订阅情况
			///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次取消订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@remark 每条取消订阅的合约均对应一条取消订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnUnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last) { (void)ticker; (void)error_info; (void)is_last; };

			///深度行情通知，包含买一卖一队列
			///@param market_data 行情数据
			///@param bid1_qty 买一队列数据
			///@param bid1_count 买一队列的有效委托笔数，即bid1_qty数组的长度，最大为50
			///@param max_bid1_count 买一队列总委托笔数
			///@param ask1_qty 卖一队列数据
			///@param ask1_count 卖一队列的有效委托笔数，即ask1_qty数组的长度，最大为50
			///@param max_ask1_count 卖一队列总委托笔数
			///@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count, int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count) { (void)market_data; (void)bid1_qty; (void)bid1_count; (void)max_bid1_count; (void)ask1_qty; (void)ask1_count; (void)max_ask1_count; };

			/// ETF的IOPV通知
			/// @param iopv ETF的参考单位基金净值数据，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnETFIOPVData(IOPV *iopv) { (void)iopv; };

			///订阅行情订单簿应答，包括股票、债券
			///@param ticker 详细的合约订阅情况
			///@param error_info 订阅合约发生错误时的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@remark 每条订阅的合约均对应一条订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnSubOrderBook(XTPST *ticker, XTPRI *error_info, bool is_last) { (void)ticker; (void)error_info; (void)is_last; };

			///退订行情订单簿应答，包括股票、债券
			///@param ticker 详细的合约取消订阅情况
			///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次取消订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@remark 每条取消订阅的合约均对应一条取消订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnUnSubOrderBook(XTPST *ticker, XTPRI *error_info, bool is_last) { (void)ticker; (void)error_info; (void)is_last; };

			///行情订单簿通知，包括股票、债券
			///@param order_book 行情订单簿数据，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnOrderBook(XTPOB *order_book) { (void)order_book; };

			///订阅逐笔行情应答，包括股票、债券
			///@param ticker 详细的合约订阅情况
			///@param error_info 订阅合约发生错误时的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@remark 每条订阅的合约均对应一条订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last) { (void)ticker; (void)error_info; (void)is_last; };

			///退订逐笔行情应答，包括股票、债券
			///@param ticker 详细的合约取消订阅情况
			///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次取消订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			///@remark 每条取消订阅的合约均对应一条取消订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnUnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last) { (void)ticker; (void)error_info; (void)is_last; };

			///逐笔行情通知，包括股票、债券
			///@param tbt_data 逐笔行情数据，包括逐笔委托和逐笔成交，此为共用结构体，需要根据type来区分是逐笔委托还是逐笔成交，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
			virtual void OnTickByTick(XTPTBT *tbt_data) { (void)tbt_data; };

			///订阅全市场的股票行情应答
			///@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnSubscribeAllMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) { (void)exchange_id; (void)error_info; };

			///退订全市场的股票行情应答
			///@param exchange_id 表示当前退订的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnUnSubscribeAllMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) { (void)exchange_id; (void)error_info; };

			///订阅全市场的股票行情订单簿应答
			///@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnSubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) { (void)exchange_id; (void)error_info; };

			///退订全市场的股票行情订单簿应答
			///@param exchange_id 表示当前退订的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnUnSubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) { (void)exchange_id; (void)error_info; };

			///订阅全市场的股票逐笔行情应答
			///@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) { (void)exchange_id; (void)error_info; };

			///退订全市场的股票逐笔行情应答
			///@param exchange_id 表示当前退订的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnUnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) { (void)exchange_id; (void)error_info; };


			///查询合约部分静态信息的应答
			///@param ticker_info 合约部分静态信息
			///@param error_info 查询合约部分静态信息时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次查询合约部分静态信息的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			virtual void OnQueryAllTickers(XTPQSI* ticker_info, XTPRI *error_info, bool is_last) { (void)ticker_info; (void)error_info; (void)is_last; };

			///查询合约的最新价格信息应答
			///@param ticker_info 合约的最新价格信息
			///@param error_info 查询合约的最新价格信息时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次查询的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			virtual void OnQueryTickersPriceInfo(XTPTPI* ticker_info, XTPRI *error_info, bool is_last) { (void)ticker_info; (void)error_info; (void)is_last; };

			///订阅全市场的期权行情应答
			///@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnSubscribeAllOptionMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) { (void)exchange_id; (void)error_info; };

			///退订全市场的期权行情应答
			///@param exchange_id 表示当前退订的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@param error_info 取消订阅合约时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnUnSubscribeAllOptionMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info) { (void)exchange_id; (void)error_info; };

			///查询沪深2市合约完整静态信息的应答
			///@param ticker_info 合约完整静态信息
			///@param error_info 查询合约完整静态信息时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次查询合约完整静态信息的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			virtual void OnQueryAllTickersFullInfo(XTPQFI* ticker_info, XTPRI *error_info, bool is_last) { (void)ticker_info; (void)error_info; (void)is_last; };

			///查询新三板合约完整静态信息的应答
			///@param ticker_info 合约完整静态信息
			///@param error_info 查询合约完整静态信息时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次查询合约完整静态信息的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			virtual void OnQueryAllNQTickersFullInfo(XTPNQFI* ticker_info, XTPRI *error_info, bool is_last) { (void)ticker_info; (void)error_info; (void)is_last; };

			///新三板合约完整静态信息盘中推送通知
			///@param ticker_info 合约完整静态信息
			///@remark 订阅新三板行情的时候会触发推送通知
			virtual void OnXTPQuoteNQFullInfo(XTPNQFI* ticker_info) { (void)ticker_info; };

			///查询合约的最新快照信息应答
			///@param market_data 合约的最新快照信息
			///@param error_info 查询合约的最新快照信息时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次查询的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			virtual void OnQueryTickersLatestMarketData(XTPMD* market_data, XTPRI *error_info, bool is_last) { (void)market_data; (void)error_info; (void)is_last; };

			///订阅指数通行情应答
			///@param error_info 订阅指数通时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnSubscribeAllIndexPress(XTPRI *error_info) { (void)error_info; };

			///取消订阅指数通行情应答
			///@param error_info 取消订阅指数通时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnUnSubscribeAllIndexPress(XTPRI *error_info) { (void)error_info; };

			///指数通行情通知
			///@param idx 指数通的行情数据
			///@remark 订阅指数通行情的时候触发推送通知
			virtual void OnIndexPress(XTPIndexPress *idx) { (void)idx; };

			///订阅港股通行情应答
			///@param error_info 订阅港股通通时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnSubscribeAllHKCMarketData(XTPRI *error_info) { (void)error_info; };

			///取消订阅港股通行情应答
			///@param error_info 取消订阅港股通时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@remark 需要快速返回
			virtual void OnUnSubscribeAllHKCMarketData(XTPRI *error_info) { (void)error_info; };

			///港股通实时额度通知
			///@param hkc_data 港股通实时额度数据
			///@remark 订阅港股通行情的时候会触发推送通知
			virtual void OnHKRLData(XTPHKCRL *hkc_data) { (void)hkc_data; };

			///港股通行情通知
			///@param hkc_md 港股通的行情数据
			///@remark 订阅港股通行情的时候会触发推送通知
			virtual void OnHKCMarketData(XTPHKCMD *hkc_md) { (void)hkc_md; };

			///请求回补指定频道的逐笔行情的总体结果应答
			///@param rebuild_result 当回补结束时被调用，如果回补结果失败，则msg参数表示失败原因
			///@remark 需要快速返回，仅在回补数据发送结束后调用，如果请求数据太多，一次性无法回补完，那么rebuild_result.result_code = XTP_REBUILD_RET_PARTLY，此时需要根据回补结果继续发起回补数据请求
			virtual void OnRequestRebuildQuote(XTPQuoteRebuildResultRsp* rebuild_result) { (void)rebuild_result; };

			///回补的逐笔行情数据
			///@param tbt_data 回补的逐笔行情数据
			///@remark 需要快速返回，此函数调用与OnTickByTick不在一个线程内，会在OnRequestRebuildQuote()之前回调
			virtual void OnRebuildTickByTick(XTPTBT *tbt_data) { (void)tbt_data; };

			///回补的快照行情数据
			///@param md_data 回补的快照行情数据
			///@remark 需要快速返回，此函数调用与OnDepthMarketData不在一个线程内，会在OnRequestRebuildQuote()之前回调
			virtual void OnRebuildMarketData(XTPMD *md_data) { (void)md_data; };

			///查询港股通完整静态信息的应答
			///@param hkcsi 港股通静态信息
			///@param error_info 查询港股通完整静态信息时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次查询港股通完整静态信息的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			virtual void OnQueryAllHKCInfo(XTPHKCSI* hkcsi, XTPRI *error_info, bool is_last) { (void)hkcsi; (void)error_info; (void)is_last; };

			///查询指数通完整静态信息的应答
			///@param ipsi 指数通完整静态信息
			///@param error_info 查询指数通完整静态信息时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
			///@param is_last 是否此次查询指数通完整静态信息的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
			virtual void OnQueryAllIndexPressInfo(XTPIPSI* ipsi, XTPRI *error_info, bool is_last) { (void)ipsi; (void)error_info; (void)is_last; };

		};
	}
}

#ifndef WINDOWS
#if __GNUC__ >= 4
#pragma GCC visibility push(default)
#endif
#endif

/*!
* \class XTPX::API::QuoteApi
*
* \brief 行情订阅接口类
*
* \author 中泰证券股份有限公司
* \date 三月 2025
*/
namespace XTPX {
	namespace API {
		class XTPX_MD_API_EXPORT QuoteApi
		{
		public:
			///创建QuoteApi
			///@param client_id （必须输入）用于区分同一用户的不同客户端，由用户自定义
			///@param save_file_path （必须输入）存贮订阅信息文件的目录，请设定一个有可写权限的真实存在的路径，如果路径不存在的话，可能会因为写冲突而造成断线
			///@param log_level 日志输出级别
			///@param udpseq_output udpseq异步日志是否输出标识，默认为true，如果不想输出异步日志，请设置此参数为false
			///@return 创建出的UserApi
			///@remark 如果一个账户需要在多个客户端登录，请使用不同的client_id，系统允许一个账户同时登录多个客户端，但是对于同一账户，相同的client_id只能保持一个session连接，后面的登录在前一个session存续期间，无法连接
			static QuoteApi *CreateQuoteApi(uint8_t client_id, const char *save_file_path, XTP_LOG_LEVEL log_level=XTP_LOG_LEVEL_DEBUG, bool udpseq_output = true);

			///删除接口对象本身
			///@remark 不再使用本接口对象时,调用该函数删除接口对象
			virtual void Release() = 0;

			///获取API的发行版本号
			///@return 返回api发行版本号
			virtual const char* GetApiVersion() = 0;

			///获取API的系统错误
			///@return 返回的错误信息，可以在Login、Logout、订阅、取消订阅失败时调用，获取失败的原因
			///@remark 可以在调用api接口失败时调用，例如login失败时
			virtual XTPRI *GetApiLastError() = 0;

			///注册回调接口
			///@param spi 派生自回调接口类的实例，请在登录之前设定
			virtual void RegisterSpi(QuoteSpi *spi) = 0;

			///设置心跳检测时间间隔，单位为秒
			///@param interval 心跳检测时间间隔，单位为秒
			///@remark 此函数必须在Login之前调用
			virtual void SetHeartBeatInterval(uint32_t interval) = 0;


			///设置行情接收的配置文件
			///@return 设置配置文件是否成功，true-成功，false-失败，需要检查配置文件是否正确
			///@param filename 包含绝对路径的配置文件名
			///@remark 此函数必须在Login之前调用，如果不调用，将无法获取行情
			virtual bool SetConfigFile(const char* filename) = 0;

			///使用UDP接收行情时，设置接收和解析行情线程绑定的cpu集合
			///@return 配置是否成功，true-成功，false-失败
			///@param cpu_no_array 设置绑定的cpu集合数组
			///@param count cpu集合数组长度
			///@remark 此函数可不调用。若不调用，系统将自动采用配置文件中的默认CPU配置；若需调用则必须严格遵循调用时序——仅允许在执行Login操作前且完成SetConfigFile设置后调用，否则配置将无法生效。在绑核分配环节，api会按数组从前往后的核序号依次分配给配置文件中md、ob、tbt这些CPU设置项（enable为OFF的不会分配）。
			virtual bool SetUDPThreadAffinityArray(int32_t cpu_no_array[], int32_t count) = 0;

			///订阅行情，包括股票、指数、期权、债券等。
			///@return 订阅接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格 
			///@param count 要订阅/退订行情的合约个数
			///@param exchange_id 交易所代码（TCP模式下不支持新三板行情订阅）
			///@remark 可以一次性订阅同一证券交易所的多个合约，在TCP连接方式时，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情
			virtual int SubscribeMarketData(char *ticker[], int count, XTP_EXCHANGE_TYPE exchange_id) = 0;

			///退订行情，包括股票、指数、期权、债券等。
			///@return 取消订阅接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
			///@param count 要订阅/退订行情的合约个数
			///@param exchange_id 交易所代码（TCP模式下不支持北交所行情订阅）
			///@remark 可以一次性取消订阅同一证券交易所的多个合约，需要与订阅行情接口配套使用
			virtual int UnSubscribeMarketData(char *ticker[], int count, XTP_EXCHANGE_TYPE exchange_id) = 0;

			///订阅行情订单簿，包括股票、债券等。（新三板暂不支持）
			///@return 订阅行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格 
			///@param count 要订阅/退订行情订单簿的合约个数
			///@param exchange_id 交易所代码
			///@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情(仅支持深交所)
			virtual int SubscribeOrderBook(char *ticker[], int count, XTP_EXCHANGE_TYPE exchange_id) = 0;

			///退订行情订单簿，包括股票、债券等。（新三板暂不支持）
			///@return 取消订阅行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
			///@param count 要订阅/退订行情订单簿的合约个数
			///@param exchange_id 交易所代码
			///@remark 可以一次性取消订阅同一证券交易所的多个合约，需要与订阅行情订单簿接口配套使用
			virtual int UnSubscribeOrderBook(char *ticker[], int count, XTP_EXCHANGE_TYPE exchange_id) = 0;

			///订阅逐笔行情，包括股票、债券等。（新三板暂不支持）
			///@return 订阅逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
			///@param count 要订阅/退订行情订单簿的合约个数
			///@param exchange_id 交易所代码
			///@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情
			virtual int SubscribeTickByTick(char *ticker[], int count, XTP_EXCHANGE_TYPE exchange_id) = 0;

			///退订逐笔行情，包括股票、债券等。（新三板暂不支持）
			///@return 取消订阅逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
			///@param count 要订阅/退订行情订单簿的合约个数
			///@param exchange_id 交易所代码
			///@remark 可以一次性取消订阅同一证券交易所的多个合约，需要与订阅逐笔行情接口配套使用
			virtual int UnSubscribeTickByTick(char *ticker[], int count, XTP_EXCHANGE_TYPE exchange_id) = 0;

			///订阅全市场的股票、债券、指数等行情
			///@return 订阅全市场行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板（TCP模式下不支持）
			///@remark 需要与全市场退订行情接口配套使用
			virtual int SubscribeAllMarketData(XTP_EXCHANGE_TYPE exchange_id = XTP_EXCHANGE_UNKNOWN) = 0;

			///退订全市场的股票、债券、指数等行情
			///@return 退订全市场行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param exchange_id 表示当前退订的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板（TCP模式下不支持）
			///@remark 需要与订阅全市场行情接口配套使用
			virtual int UnSubscribeAllMarketData(XTP_EXCHANGE_TYPE exchange_id = XTP_EXCHANGE_UNKNOWN) = 0;

			///订阅全市场的股票、债券等行情订单簿（新三板暂不支持）
			///@return 订阅全市场行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@remark 需要与全市场退订行情订单簿接口配套使用
			virtual int SubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchange_id = XTP_EXCHANGE_UNKNOWN) = 0;

			///退订全市场的股票、债券等行情订单簿（新三板暂不支持）
			///@return 退订全市场行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param exchange_id 表示当前退订的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@remark 需要与订阅全市场行情订单簿接口配套使用
			virtual int UnSubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchange_id = XTP_EXCHANGE_UNKNOWN) = 0;

			///订阅全市场的股票、债券等逐笔行情（新三板暂不支持）
			///@return 订阅全市场逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@remark 需要与全市场退订逐笔行情接口配套使用
			virtual int SubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchange_id = XTP_EXCHANGE_UNKNOWN) = 0;

			///退订全市场的股票、债券等逐笔行情（新三板暂不支持）
			///@return 退订全市场逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param exchange_id 表示当前退订的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@remark 需要与订阅全市场逐笔行情接口配套使用
			virtual int UnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchange_id = XTP_EXCHANGE_UNKNOWN) = 0;

			///用户登录请求
			///@return 登录是否成功，“0”表示登录成功，“-1”表示连接服务器出错，此时用户可以调用GetApiLastError()来获取错误代码，“-2”表示已存在连接，不允许重复登录，如果需要重连，请先logout，“-3”表示输入有错误
			///@param ip 服务器ip地址，类似“127.0.0.1”
			///@param port 服务器端口号
			///@param user 登陆用户名
			///@param password 登陆密码
			///@param sock_type “1”代表TCP，“2”代表UDP
			///@param local_ip 本地网卡地址，类似“127.0.0.1”
			///@remark 此函数为同步阻塞式，不需要异步等待登录成功，当函数返回即可进行后续操作，此api只能有一个连接
			virtual int Login(const char* ip, int port, const char* user, const char* password, XTP_PROTOCOL_TYPE sock_type, const char* local_ip = NULL) = 0;


			///登出请求
			///@return 登出是否成功，“0”表示登出成功，非“0”表示登出出错，此时用户可以调用GetApiLastError()来获取错误代码
			///@remark 此函数为同步阻塞式，不需要异步等待登出，当函数返回即可进行后续操作，不允许在回调线程调用。
			virtual int Logout() = 0;

			///获取沪深2市当前交易日合约部分静态信息
			///@return 发送查询请求是否成功，“0”表示发送查询请求成功，非“0”表示发送查询请求不成功
			///@param exchange_id 交易所代码，必须提供 1-上海 2-深圳，仅支持单市场查询
			virtual int QueryAllTickers(XTP_EXCHANGE_TYPE exchange_id) = 0;

			///获取合约的最新价格信息
			///@return 发送查询请求是否成功，“0”表示发送查询请求成功，非“0”表示发送查询请求不成功
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
			///@param count 要查询的合约个数
			///@param exchange_id 交易所代码
			virtual int QueryTickersPriceInfo(char *ticker[], int count, XTP_EXCHANGE_TYPE exchange_id) = 0;

			///获取所有合约的最新价格信息
			///@return 发送查询请求是否成功，“0”表示发送查询请求成功，非“0”表示发送查询请求不成功
			///@param exchange_id 表示当前全查询的市场，必须指定，仅支持单市场查询
			virtual int QueryAllTickersPriceInfo(XTP_EXCHANGE_TYPE exchange_id) = 0;
			
			///订阅全市场的期权行情
			///@return 订阅全市期权场行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@remark 需要与全市场退订期权行情接口配套使用
			virtual int SubscribeAllOptionMarketData(XTP_EXCHANGE_TYPE exchange_id = XTP_EXCHANGE_UNKNOWN) = 0;

			///退订全市场的期权行情
			///@return 退订全市场期权行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param exchange_id 表示当前退订的市场，如果为XTP_EXCHANGE_UNKNOWN，表示沪深全市场（不包括新三板），XTP_EXCHANGE_SH表示为上海全市场，XTP_EXCHANGE_SZ表示为深圳全市场，XTP_EXCHANGE_NQ表示新三板
			///@remark 需要与订阅全市场期权行情接口配套使用
			virtual int UnSubscribeAllOptionMarketData(XTP_EXCHANGE_TYPE exchange_id = XTP_EXCHANGE_UNKNOWN) = 0;

			///获取沪深2市所有合约的详细静态信息，包括指数等非可交易的，不包括新三板
			///@return 发送查询请求是否成功，“0”表示发送查询请求成功，非“0”表示发送查询请求不成功
			///@param exchange_id 交易所代码，必须提供 1-上海 2-深圳
			virtual int QueryAllTickersFullInfo(XTP_EXCHANGE_TYPE exchange_id) = 0;

			///获取新三板所有合约的详细静态信息，包括指数等非可交易的
			///@return 发送查询请求是否成功，“0”表示发送查询请求成功，非“0”表示发送查询请求不成功
			virtual int QueryAllNQTickersFullInfo() = 0;

			///获取合约的最新快照信息
			///@return 发送查询请求是否成功，“0”表示发送查询请求成功，非“0”表示发送查询请求不成功
			///@param ticker 合约ID数组，注意合约代码必须以'\0'结尾，不包含空格  
			///@param count 要查询的合约个数
			///@param exchange_id 交易所代码
			virtual int QueryTickersLatestMarketData(char *ticker[], int count, XTP_EXCHANGE_TYPE exchange_id) = 0;

			///订阅指数通行情（TCP模式下不支持）
			///@return 发送订阅请求是否成功，“0”表示发送订阅请求成功，非“0”表示发送订阅请求不成功
			///@remark 对应响应函数是OnSubscribeAllIndexPress()
			virtual int SubscribeAllIndexPress() = 0;

			///取消订阅指数通行情（TCP模式下不支持）
			///@return 发送订阅请求是否成功，“0”表示发送订阅请求成功，非“0”表示发送订阅请求不成功
			///@remark 对应响应函数是OnUnSubscribeAllIndexPress()
			virtual int UnSubscribeAllIndexPress() = 0;

			///订阅港股通的行情（TCP模式下不支持）
			///@return 发送订阅请求是否成功，“0”表示发送订阅请求成功，非“0”表示发送订阅请求不成功
			///@remark 对应响应函数是OnSubscribeAllHKCMarketData()
			virtual int SubscribeAllHKCMarketData() = 0;

			///取消订阅港股通的行情（TCP模式下不支持）
			///@return 发送订阅请求是否成功，“0”表示发送订阅请求成功，非“0”表示发送订阅请求不成功
			///@remark 对应响应函数是OnUnSubscribeAllHKCMarketData()
			virtual int UnSubscribeAllHKCMarketData() = 0;

			///请求回补指定行情，包括快照和逐笔
			///@return 请求回补指定频道的逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
			///@param rebuild_param 指定回补的参数信息，注意一次性回补最多1000个数据，超过1000需要分批次请求，一次只能指定一种类型的数据
			///@remark 仅在逐笔行情丢包时或者确实快照行情时使用，回补的行情数据将从OnRebuildTickByTick或者OnRebuildMarketData()接口回调提供，与订阅的行情数据不在同一个线程内
			virtual int RequestRebuildQuote(XTPQuoteRebuildReq* rebuild_param) = 0;

			///获取港股通的静态信息
			///@return 发送查询请求是否成功，“0”表示发送查询请求成功，非“0”表示发送查询请求不成功
			virtual int QueryAllHKCInfo() = 0;

			///获取指数通的静态信息
			///@return 发送查询请求是否成功，“0”表示发送查询请求成功，非“0”表示发送查询请求不成功
			virtual int QueryAllIndexPressInfo() = 0;


		protected:
			~QuoteApi() {};
		};
	}
}

#ifndef WINDOWS
#if __GNUC__ >= 4
#pragma GCC visibility pop
#endif
#endif


#endif

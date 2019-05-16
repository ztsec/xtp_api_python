#pragma once
#include "xtp_quote_api.h"
#include <fstream>
#include <time.h>
#include <sys/timeb.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32

#define NUM_OF_ROUND 1

using namespace XTP::API;

class MyQuoteSpi : public QuoteSpi
{
public:
	MyQuoteSpi();
	~MyQuoteSpi();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param reason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnDisconnected(int reason);

///错误应答
	virtual void OnError(XTPRI *error_info,bool is_last);


	virtual void OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnUnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count, int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count);
	virtual void OnSubOrderBook(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnUnSubOrderBook(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnUnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last);
	virtual void OnOrderBook(XTPOB *order_book);
	virtual void OnTickByTick(XTPTBT *tbt_data);
	virtual void OnQueryAllTickers(XTPQSI* ticker_info, XTPRI *error_info, bool is_last);
	virtual void OnQueryTickersPriceInfo(XTPTPI* ticker_info, XTPRI *error_info, bool is_last);
	virtual void OnSubscribeAllMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnSubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnSubscribeAllOptionMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllOptionMarketData(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnSubscribeAllOptionOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllOptionOrderBook(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnSubscribeAllOptionTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);
	virtual void OnUnSubscribeAllOptionTickByTick(XTP_EXCHANGE_TYPE exchange_id, XTPRI *error_info);


private:


	bool IsErrorRspInfo(XTPRI *pRspInfo);

};
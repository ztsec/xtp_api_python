#include "trade_spi.h"
#include <iostream>
#include <cstring>
#include <iomanip>
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#else
#include <windows.h>

#endif
#include <sstream>
#include <map>
#include "FileUtils.h"

using namespace std;

extern bool is_connected_;
extern std::string trade_server_ip;
extern int trade_server_port;
extern uint64_t session_id_;
extern std::map<uint64_t, uint64_t> map_session;
extern uint64_t* session_arrary;
extern FileUtils* fileUtils;
extern XTPOrderInsertInfo* orderList;

MyTraderSpi::MyTraderSpi()
{
	order_num = 0;
	trade_num = 0;
	insert_order_num = 0;
	cancel_order_num = 0;

#ifndef WIN32
	std::string orderName = "log/";
	std::string tradeName = "log/";
	std::string fundName = "log/";
	std::string positonName = "log/";
	std::string qryOrderName = "log/";
	std::string qryTradeName = "log/";
	std::string cancelOrderName = "log/";
	std::string positionCountName = "log/";
#else
	std::string orderName = "log\\";
	std::string tradeName = "log\\";
	std::string fundName = "log\\";
	std::string positonName = "log\\";
	std::string qryOrderName = "log\\";
	std::string qryTradeName = "log\\";
	std::string cancelOrderName = "log\\";
	std::string positionCountName = "log\\";
#endif // !WIN32




#ifndef WIN32
	struct timeval start;
	gettimeofday(&start, 0);
	stringstream ss;
	ss << start.tv_sec;

	std::string pre;
	ss >> pre;

	orderName.append(pre);
	tradeName.append(pre);
	fundName.append(pre);
	positonName.append(pre);
	qryOrderName.append(pre);
	qryTradeName.append(pre);
	cancelOrderName.append(pre);
	positionCountName.append(pre);
#else

	SYSTEMTIME wtm;
	GetLocalTime(&wtm);

	stringstream ss;
	ss << wtm.wDay << wtm.wHour << wtm.wMinute << wtm.wSecond;
	
	std::string pre;
	ss >> pre;

	orderName.append(pre);
	tradeName.append(pre);
	fundName.append(pre);
	positonName.append(pre);
	qryOrderName.append(pre);
	qryTradeName.append(pre);
	cancelOrderName.append(pre);
	positionCountName.append(pre);

#endif

	orderName.append("order.txt");
	tradeName.append("tradeReport.txt");
	fundName.append("fund.txt");
	positonName.append("stock_position.txt");
	qryOrderName.append("qry_order.txt");
	qryTradeName.append("qry_tradeReport.txt");
	cancelOrderName.append("cancel_order.txt");
	positionCountName.append("qry_positionCount.txt");

	fout_order.open(orderName.c_str());
	fout_trade.open(tradeName.c_str());
	fout_fund.open(fundName.c_str());
	fout_position.open(positonName.c_str());
	fout_qry_order.open(qryOrderName.c_str());
	fout_qry_trade.open(qryTradeName.c_str());
	fout_cancel_order.open(cancelOrderName.c_str());
	fout_position_count.open(positionCountName.c_str());

	outCount = 1;

	m_iquestID = 1;

	save_to_file_ = false;

}

MyTraderSpi::~MyTraderSpi()
{
	fout_order.close();
	fout_trade.close();
	fout_fund.close();
	fout_position.close();
	fout_qry_order.close();
	fout_qry_trade.close();
	fout_cancel_order.close();
	fout_position_count.close();
}

void MyTraderSpi::OnDisconnected(uint64_t session_id, int reason)
{
	cout << "-------------------- OnDisconnected -------------------------" << endl;
	uint64_t aid = 0;
	std::map<uint64_t, uint64_t>::iterator pos = map_session.find(session_id);
	if (pos != map_session.end())
	{
		aid = pos->second;
		map_session.erase(pos);
	}

	//断线后，重新连接
	is_connected_ = false;
	uint64_t temp_session_id_ = 0;
	do
	{
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1);
#endif // WIN32
		std::string account_name = fileUtils->stdStringForKey("account[%d].user", aid);
		std::string account_pw = fileUtils->stdStringForKey("account[%d].password", aid);
		temp_session_id_ = pUserApi->Login(trade_server_ip.c_str(), trade_server_port, account_name.c_str(), account_pw.c_str(), XTP_PROTOCOL_TCP);

	} while (temp_session_id_ == 0);

	//重新登录成功后更新映射关系
	map_session.insert(std::make_pair(temp_session_id_, aid));
	session_id_ = temp_session_id_;
	is_connected_ = true;
	session_arrary[aid] = temp_session_id_;
}

void MyTraderSpi::OnError(XTPRI *error_info)
{
	cout << "--->>> " << "OnRspError" << endl;
	IsErrorRspInfo(error_info);
}


void MyTraderSpi::OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id)
{
#if _IS_LOG_SHOW_
	cout << "------------------- OnOrderEvent-----------" << endl;
	cout << "Order_Client_ID:" << order_info->order_client_id << endl;
	cout << "Order_XTP_ID:" << order_info->order_xtp_id << endl;
	cout << "Order_status:" << order_info->order_status << endl;
	cout << "Order_submit_status:" << order_info->order_submit_status << endl;
	cout << "update time:" << order_info->update_time << endl;
	if (error_info)
	{
		cout << "Error:" << error_info->error_id << endl;
	}
#endif
	updateOrderNum();
	if (order_num%outCount == 0)
	{

		cout << "!!!!!!!!!!!!!!!!!!!!! OnOrderEvent total count:" << order_num << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	}

	if (save_to_file_)
	{
#ifdef WIN32
		SYSTEMTIME wtm;
		GetLocalTime(&wtm);
		fout_order << "[" << wtm.wHour << ":" << wtm.wMinute << ":" << wtm.wSecond << "." << wtm.wMilliseconds << "],";
#else
		struct timeval start;
		gettimeofday(&start, 0);
		struct tm *ptr;
		time_t lt;
		lt = time(NULL);
		ptr = localtime(&lt);
		fout_order << "[" << ptr->tm_hour << ":" << ptr->tm_min << ":" << ptr->tm_sec << "." << start.tv_usec << "],";
#endif // WIN32

		fout_order << "xtp_id:" << order_info->order_xtp_id << ",client_id:" << order_info->order_client_id << ",status:" << order_info->order_status << ",cancel_xtp_id:" << order_info->order_cancel_xtp_id << ",cancel_client_id:" << order_info->order_cancel_client_id;
		fout_order << ",order_submit_status:" << order_info->order_submit_status << ",ticker:" << order_info->ticker << ",market:" << order_info->market << ",price:" << order_info->price;
		fout_order << ",quantity:" << order_info->quantity << ",price_type:" << order_info->price_type << ",side:" << order_info->side << ",qty_traded:" << order_info->qty_traded << ",qty_left:" << order_info->qty_left;
		fout_order << ",insert_time:" << order_info->insert_time << ",update_time:" << order_info->update_time << ",cancel_time:" << order_info->cancel_time << ",trade_amount:" << order_info->trade_amount;
		fout_order << ",order_local_id:" << order_info->order_local_id << ",order_type:" << order_info->order_type << ",error_id:" << error_info->error_id << ",error_msg:" << error_info->error_msg << endl;
	}

	//此为乒乓测试
	//根据报单响应情况来处理，当不是最终状态时，发起撤单，如果是最终状态，就再下一单
	switch (order_info->order_status)
	{
	case XTP_ORDER_STATUS_NOTRADEQUEUEING:
	{
		pUserApi->CancelOrder(order_info->order_xtp_id, session_arrary[order_info->order_client_id]);
		break;
	}
	case XTP_ORDER_STATUS_ALLTRADED:
	case XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING:
	case XTP_ORDER_STATUS_CANCELED:
	case XTP_ORDER_STATUS_REJECTED:
	{
		int i = order_info->order_client_id;
		pUserApi->InsertOrder(&(orderList[i]), session_arrary[i]);
		break;
	}

	default:
		break;
	}


}

void MyTraderSpi::OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id)
{
#if _IS_LOG_SHOW_
	cout << "------------------- OnTradeEvent-----------" << endl;
	cout << "Order_Client_ID:" << trade_info->order_client_id << endl;
	cout << "Local_Order_ID:" << trade_info->local_order_id << endl;
	cout << "Order_Exch_ID:" << trade_info->order_exch_id << endl;
	cout << "Order_xtp_id:" << trade_info->order_xtp_id << endl;
	cout << "Side:" << trade_info->side << endl;
	cout << "Price:" << trade_info->price << endl;
	cout << "Quantity:" << trade_info->quantity << endl;
	cout << "Trade amount" << trade_info->trade_amount << endl;
	cout << "Trade Time:" << trade_info->trade_time << endl;
	cout << "Report Index:" << trade_info->report_index << endl;
#endif


	updateTradeNum();

	if (trade_num%outCount == 0)
	{
		cout << "!!!!!!!!!!!!!!!!!!!!! OnTradeEvent total count:" << trade_num << "!!!!!!!!!!!!!!!!!!!!!" << endl;
	}

	if (save_to_file_)
	{
#ifdef WIN32
		SYSTEMTIME wtm;
		GetLocalTime(&wtm);
		fout_trade << "[" << wtm.wHour << ":" << wtm.wMinute << ":" << wtm.wSecond << "." << wtm.wMilliseconds << "],";
#else
		struct timeval start;
		gettimeofday(&start, 0);
//		fout_trade << "time:" << start.tv_sec << "." << start.tv_usec << ",";
		struct tm *ptr;
		time_t lt;
		lt = time(NULL);
		ptr = localtime(&lt);
		fout_trade << "[" << ptr->tm_hour << ":" << ptr->tm_min << ":" << ptr->tm_sec << "." << start.tv_usec << "],";
#endif // WIN32
		fout_trade << "xtp_id:" << trade_info->order_xtp_id << ",client_id:" << trade_info->order_client_id;
		fout_trade << ",ticker:" << trade_info->ticker << ",market:" << trade_info->market << ",price:" << trade_info->price;
		fout_trade << ",quantity:" << trade_info->quantity << ",side:" << trade_info->side << ",trade_time:" << trade_info->trade_time << ",trade_amount:" << trade_info->trade_amount;
		fout_trade << ",exec_id:" << trade_info->exec_id << ",report_index:" << trade_info->report_index << ",order_exch_id:" << trade_info->order_exch_id;
		fout_trade << ",trade_type:" << trade_info->trade_type << ",branch_pbu:" << trade_info->branch_pbu << endl;
	}


}

void MyTraderSpi::OnCancelOrderError(XTPOrderCancelInfo * cancel_info, XTPRI * error_info, uint64_t session_id)
{
// 	cout << "-----------------OnCancelOrderError---------------------" << endl;
// 	cout << "cancel_order_xtp_id:" << cancel_info->order_cancel_xtp_id << "   order_xtp_id:" << cancel_info->order_xtp_id << "  errorcode:" << error_info->error_id << "  msg:" << error_info->error_msg << endl;
	if (save_to_file_)
	{
		fout_cancel_order << "cancel_order_xtp_id:" << cancel_info->order_cancel_xtp_id << ",order_xtp_id:" << cancel_info->order_xtp_id << ",error_id:" << error_info->error_id << ",msg:" << error_info->error_msg << endl;
	}
}

void MyTraderSpi::OnQueryOrder(XTPQueryOrderRsp * order_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id)
{
	if (error_info && error_info->error_id != 0)
	{
		//查询出错
		if (error_info->error_id == 11000350)
		{
			//账户没有下过订单
			cout << "------------------- No Order.-----------" << endl;
		}
		else
		{
			//真正的出错
		}
		return;
	}

// 	cout << "------------------- OnQueryOrder-----------" << endl;
// 	cout << "OnQueryOrder: " << order_info->ticker << ":" << order_info->order_xtp_id  << ":" << request_id << ":" << is_last << endl;

	if (save_to_file_)
	{
#ifdef WIN32
		SYSTEMTIME wtm;
		GetLocalTime(&wtm);
		fout_qry_order << "[" << wtm.wHour << ":" << wtm.wMinute << ":" << wtm.wSecond << "." << wtm.wMilliseconds << "],";
#else
		struct timeval start;
		gettimeofday(&start, 0);
		struct tm *ptr;
		time_t lt;
		lt = time(NULL);
		ptr = localtime(&lt);
		fout_qry_order << "[" << ptr->tm_hour << ":" << ptr->tm_min << ":" << ptr->tm_sec << "." << start.tv_usec << "],";
#endif // WIN32
		fout_qry_order << "xtp_id:" << order_info->order_xtp_id << ",client_id:" << order_info->order_client_id << ",status:" << order_info->order_status << ",cancel_xtp_id:" << order_info->order_cancel_xtp_id << ",cancel_client_id:" << order_info->order_cancel_client_id;
		fout_qry_order << ",order_submit_status:" << order_info->order_submit_status << ",ticker:" << order_info->ticker << ",market:" << order_info->market << ",price:" << order_info->price;
		fout_qry_order << ",quantity:" << order_info->quantity << ",price_type:" << order_info->price_type << ",side:" << order_info->side << ",qty_traded:" << order_info->qty_traded << ",qty_left:" << order_info->qty_left;
		fout_qry_order << ",insert_time:" << order_info->insert_time << ",update_time:" << order_info->update_time << ",cancel_time:" << order_info->cancel_time << ",trade_amount:" << order_info->trade_amount;
		fout_qry_order << ",order_local_id:" << order_info->order_local_id << ",order_type:" << order_info->order_type <<",error_id:" << error_info->error_id << ",error_msg:" << error_info->error_msg << endl;
	}
}

void MyTraderSpi::OnQueryTrade(XTPQueryTradeRsp * trade_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id)
{
	if (error_info && error_info->error_id != 0)
	{
		//查询出错
		if (error_info->error_id == 11000350)
		{
			//账户没有成交
			cout << "------------------- No Transaction Record.-----------" << endl;
		}
		else
		{
			//真正的出错
		}
		return;
	}

// 	cout << "------------------- OnQueryTrade-----------" << endl;
// 	cout << "OnQueryTrade: " << trade_info->ticker << ":" << trade_info->order_xtp_id << ":" << request_id << ":" << is_last << endl;

	if (save_to_file_)
	{
		fout_qry_trade << "xtp_id:" << trade_info->order_xtp_id << ",client_id:" << trade_info->order_client_id;
		fout_qry_trade << ",ticker:" << trade_info->ticker << ",market:" << trade_info->market << ",price:" << trade_info->price;
		fout_qry_trade << ",quantity:" << trade_info->quantity << ",side:" << trade_info->side << ",trade_time:" << trade_info->trade_time << ",trade_amount:" << trade_info->trade_amount;
		fout_qry_trade << ",exec_id:" << trade_info->exec_id << ",report_index:" << trade_info->report_index << ",order_exch_id:" << trade_info->order_exch_id;
		fout_qry_trade << ",trade_type:" << trade_info->trade_type << ",branch_pbu:" << trade_info->branch_pbu << endl;
	}
}

void MyTraderSpi::OnQueryPosition(XTPQueryStkPositionRsp * investor_position, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id)
{
	if (error_info && error_info->error_id !=0)
	{
		//查询出错
		if (error_info->error_id == 11000350)
		{
			//账户里没有持仓
			cout << "------------------- Position is empty.-----------" << endl;
		}
		else
		{
			//真正的出错
		}
		return;
	}


	if (save_to_file_)
	{
#ifdef WIN32
		SYSTEMTIME wtm;
		GetLocalTime(&wtm);
		fout_position << "time:" << wtm.wSecond << "." << wtm.wMilliseconds << ",";

#else
		struct timeval start;
		gettimeofday(&start, 0);

		struct tm *ptr;
		time_t lt;
		lt = time(NULL);
		ptr = localtime(&lt);
		fout_position << "[" << ptr->tm_hour << ":" << ptr->tm_min << ":" << ptr->tm_sec << "." << start.tv_usec << "],";

#endif // WIN32



		fout_position << "request_id:" << request_id << ",is_last:" << is_last << ",";
		fout_position << "ticker:" << investor_position->ticker << ",ticker_name:" << investor_position->ticker_name;
		fout_position << ",total_qty:" << investor_position->total_qty << ",sellable_qty:" << investor_position->sellable_qty << ",avg_price:" << investor_position->avg_price;
		fout_position << ",unrealized_pnl:" << investor_position->unrealized_pnl << endl;
	}
}

void MyTraderSpi::OnQueryAsset(XTPQueryAssetRsp * trading_account, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id)
{
// 	cout << "------------------- OnQueryTradingAccount-----------" << endl;
// 	cout << "OnQueryTradingAccount: " << trading_account->buying_power << ":" << trading_account->fund_buy_amount << ":" << request_id << ":" << is_last << endl;

	if (save_to_file_)
	{
#ifdef WIN32
		SYSTEMTIME wtm;
		GetLocalTime(&wtm);
		fout_fund << "time:" << wtm.wSecond << "." << wtm.wMilliseconds << ",";
#else
		struct timeval start;
		gettimeofday(&start, 0);
		struct tm *ptr;
		time_t lt;
		lt = time(NULL);
		ptr = localtime(&lt);
		fout_fund << "[" << ptr->tm_hour << ":" << ptr->tm_min << ":" << ptr->tm_sec << "." << start.tv_usec << "],";
#endif // WIN32
		fout_fund << "request_id:" << request_id << ",total_asset:" << setprecision(14) << trading_account->total_asset << ",security_asset:" << setprecision(16) << trading_account->security_asset;
		fout_fund << ",buying_power:" << setprecision(16) << trading_account->buying_power << ",fund_buy_amount:" << setprecision(16) << trading_account->fund_buy_amount;
		fout_fund << ",fund_buy_fee:" << setprecision(16) << trading_account->fund_buy_fee << ",fund_sell_amount:" << setprecision(16) << trading_account->fund_sell_amount << ",fund_sell_fee:" << setprecision(16) << trading_account->fund_sell_fee << endl;
	}
}

void MyTraderSpi::OnQueryStructuredFund(XTPStructuredFundInfo * fund_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id)
{
}

void MyTraderSpi::OnQueryFundTransfer(XTPFundTransferNotice * fund_transfer_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id)
{
}

void MyTraderSpi::OnFundTransfer(XTPFundTransferNotice * fund_transfer_info, XTPRI * error_info, uint64_t session_id)
{
}

void MyTraderSpi::OnQueryETF(XTPQueryETFBaseRsp * etf_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id)
{
}

void MyTraderSpi::OnQueryETFBasket(XTPQueryETFComponentRsp * etf_component_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id)
{
}

void MyTraderSpi::OnQueryIPOInfoList(XTPQueryIPOTickerRsp * ipo_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id)
{
}

void MyTraderSpi::OnQueryIPOQuotaInfo(XTPQueryIPOQuotaRsp * quota_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id)
{
}

void MyTraderSpi::OnQueryOptionAuctionInfo(XTPQueryOptionAuctionInfoRsp * option_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id)
{
}

void MyTraderSpi::updateOrderNum()
{
	order_num++;
}

int MyTraderSpi::getOrderNum()
{
	return order_num;
}

void MyTraderSpi::resetOrderNum()
{
	order_num = 0;
}

void MyTraderSpi::updateTradeNum()
{
	trade_num++;
}

int MyTraderSpi::getTradeNum()
{
	return trade_num;
}

void MyTraderSpi::resetTradeNum()
{
	trade_num = 0;
}

void MyTraderSpi::updateInsertOrderNum()
{
	insert_order_num++;
}

int MyTraderSpi::getInsertOrderNum()
{
	return insert_order_num;
}

void MyTraderSpi::resetInsertOrderNum()
{
	insert_order_num = 0;
}

void MyTraderSpi::updateCancelOrderNum()
{
	cancel_order_num++;
}

int MyTraderSpi::getCancelOrderNum()
{
	return cancel_order_num;
}

void MyTraderSpi::resetCancelOrderNum()
{
	cancel_order_num = 0;
}


bool MyTraderSpi::IsErrorRspInfo(XTPRI *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->error_id != 0));
	if (bResult)
		cout << "--->>> ErrorID=" << pRspInfo->error_id << ", ErrorMsg=" << pRspInfo->error_msg << endl;
	return bResult;
}

int MyTraderSpi::getCurrQuestID()
{
	return m_iquestID++;
}

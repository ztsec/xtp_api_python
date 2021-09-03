// vnctpmd.cpp : 定义 DLL 应用程序的导出函数。
//
//#include "stdafx.h"
#include "vnxtptrader.h"

///-------------------------------------------------------------------------------------
///从Python对象到C++类型转换用的函数
///-------------------------------------------------------------------------------------

// 嵌套结构体-int类型处理
void getNestedDictValue(dict d, string key1, string key2, int *value) {
	if (d.has_key(key1)) {        //检查字典中是否存在该键值
		dict o1 = (dict)d[key1];    //获取该键值
		if (o1.has_key(key2)) {
			object o2 = o1[key2];    //获取该键值
			extract<int> x(o2);    //创建提取器
			if (x.check()) {        //如果可以提取
				*value = x();    //对目标整数指针赋值
			}
		}
	}
}

// 嵌套结构体-字符串类型处理
void getNestedDictChar(dict d, string key1, string key2, char *value) {
	if (d.has_key(key1)) {        //检查字典中是否存在该键值
		dict o1 = (dict)d[key1];    //获取该键值
		if (o1.has_key(key2)) {
			object o2 = o1[key2];    //获取该键值
			extract<string> x(o2);    //创建提取器
			if (x.check()) {        //如果可以提取
				string s = x();
				const char *buffer = s.c_str();
#ifdef WIN32
				strcpy_s(value, strlen(buffer) + 1, buffer);
#else
				strncpy(value, buffer, strlen(buffer) + 1);
#endif
			}
		}
	}
}

// 嵌套结构体数组-字符串类型处理
void getNestedDictChar2(dict d, string key1, string key2, string key3, char *value, int index) {
	if (d.has_key(key1)) {        //检查字典中是否存在该键值
		dict o1 = (dict)d[key1];    //获取该键值
		if (o1.has_key(key2)) {
			dict o2 = (dict)(((boost::python::list)o1[key2])[index]);    //获取该键值
			if (o2.has_key(key3)) {
				object o3 = o2[key3];    //获取该键值
				extract<string> x(o3);    //创建提取器
				if (x.check()) {        //如果可以提取
					string s = x();
					const char *buffer = s.c_str();
#ifdef WIN32
					strcpy_s(value, strlen(buffer) + 1, buffer);
#else
					strncpy(value, buffer, strlen(buffer) + 1);
#endif
				}
			}
		}
	}
}

// 嵌套结构体数组-int类型处理
void getNestedDictValue2(dict d, string key1, string key2, string key3, int *value, int index) 
{
	if (d.has_key(key1)) {        //检查字典中是否存在该键值
		dict o1 = (dict)d[key1];    //获取该键值
		if (o1.has_key(key2)) {
			dict o2 = (dict)(((boost::python::list)o1[key2])[index]);    //获取该键值
			if (o2.has_key(key3)) {
				object o3 = o2[key3];    //获取该键值
				extract<int> x(o3);    //创建提取器
				if (x.check()) {        //如果可以提取
					*value = x();    //对目标整数指针赋值
				}
			}
		}
	}
}

void getInt(dict d, string key, int *value)
{
	if (d.has_key(key))		//检查字典中是否存在该键值
	{
		object o = d[key];	//获取该键值
		extract<int> x(o);	//创建提取器
		if (x.check())		//如果可以提取
		{
			*value = x();	//对目标整数指针赋值
		}
	}
};

void getUint64(dict d, string key, uint64_t *value)
{
	if (d.has_key(key))		//检查字典中是否存在该键值
	{
		object o = d[key];	//获取该键值
		extract<int> x(o);	//创建提取器
		if (x.check())		//如果可以提取
		{
			*value = x();	//对目标整数指针赋值
		}
	}
};

void getUint32(dict d, string key, uint32_t *value)
{
	if (d.has_key(key))		//检查字典中是否存在该键值
	{
		object o = d[key];	//获取该键值
		extract<int> x(o);	//创建提取器
		if (x.check())		//如果可以提取
		{
			*value = x();	//对目标整数指针赋值
		}
	}
};

void getInt64(dict d, string key, int64_t *value)
{
	if (d.has_key(key))		//检查字典中是否存在该键值
	{
		object o = d[key];	//获取该键值
		extract<int> x(o);	//创建提取器
		if (x.check())		//如果可以提取
		{
			*value = x();	//对目标整数指针赋值
		}
	}
};

void getDouble(dict d, string key, double *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<double> x(o);
		if (x.check())
		{
			*value = x();
		}
	}
};

void getStr(dict d, string key, char *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<string> x(o);
		if (x.check())
		{
			string s = x();
			const char *buffer = s.c_str();
			//对字符串指针赋值必须使用strcpy_s, vs2013使用strcpy编译通不过
			//+1应该是因为C++字符串的结尾符号？不是特别确定，不加这个1会出错
#ifdef _MSC_VER //WIN32
			strcpy_s(value, strlen(buffer) + 1, buffer);
#elif __GNUC__
			strncpy(value, buffer, strlen(buffer) + 1);
#endif
		}
	}
};

void getChar(dict d, string key, char *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<string> x(o);
		if (x.check())
		{
			string s = x();
			const char *buffer = s.c_str();
			*value = *buffer;
		}
	}
};

string addEndingChar(char *value){
	//printf("value:%s\n",value);
	string endStr = value;
	//printf("endStr:%s\n",endStr.c_str());
	return endStr;
}

///-------------------------------------------------------------------------------------
///C++的回调函数将数据保存到队列中
///-------------------------------------------------------------------------------------

void TraderApi::OnDisconnected(uint64_t session_id, int reason)
{
	Task* task = new Task();
	task->task_name = ONDISCONNECTED;
	task->addtional_int = session_id;		//手动修改
	task->task_id = reason;
	this->task_queue.push(task);
};

void TraderApi::OnError(XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONERROR;

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	this->task_queue.push(task);
};

void TraderApi::OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONORDEREVENT;

	if (order_info)
	{
		XTPOrderInfo *task_data = new XTPOrderInfo();
		*task_data = *order_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->addtional_int = session_id;

	this->task_queue.push(task);
};

void TraderApi::OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONTRADEEVENT;

	if (trade_info)
	{
		XTPTradeReport *task_data = new XTPTradeReport();
		*task_data = *trade_info;
		task->task_data = task_data;
	}

	task->addtional_int = session_id;

	this->task_queue.push(task);
};

void TraderApi::OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONCANCELORDERERROR;

	if (cancel_info)
	{
		XTPOrderCancelInfo *task_data = new XTPOrderCancelInfo();
		*task_data = *cancel_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->addtional_int = session_id;
		
	this->task_queue.push(task);
};

void TraderApi::OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYORDER;

	if (order_info)
	{
		XTPQueryOrderRsp *task_data = new XTPQueryOrderRsp();
		*task_data = *order_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnQueryOrderByPage(XTPQueryOrderRsp *order_info, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYORDERBYPAGE;

	if (order_info)
	{
		XTPQueryOrderRsp *task_data = new XTPQueryOrderRsp();
		*task_data = *order_info;
		task->task_data = task_data;
	}

	task->addtional_int_two = req_count;
	task->addtional_int_three = order_sequence;
	task->addtional_int_four = query_reference;
	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYTRADE;

	if (trade_info)
	{
		XTPQueryTradeRsp *task_data = new XTPQueryTradeRsp();
		*task_data = *trade_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnQueryTradeByPage(XTPQueryTradeRsp *trade_info, int64_t req_count, int64_t trade_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYTRADEBYPAGE;

	if (trade_info)
	{
		XTPQueryTradeRsp *task_data = new XTPQueryTradeRsp();
		*task_data = *trade_info;
		task->task_data = task_data;
	}

	task->addtional_int_two = req_count;
	task->addtional_int_three = trade_sequence;
	task->addtional_int_four = query_reference;
	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYPOSITION;

	if (position)
	{
		XTPQueryStkPositionRsp *task_data = new XTPQueryStkPositionRsp();
		*task_data = *position;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYASSET;

	if (asset)
	{
		XTPQueryAssetRsp *task_data = new XTPQueryAssetRsp();
		*task_data = *asset;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnQueryStructuredFund(XTPStructuredFundInfo *fund_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYSTRUCTUREDFUND;

	if (fund_info)
	{
		XTPStructuredFundInfo *task_data = new XTPStructuredFundInfo();
		*task_data = *fund_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnQueryFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYFUNDTRANSFER;

	if (fund_transfer_info)
	{
		XTPFundTransferNotice *task_data = new XTPFundTransferNotice();
		*task_data = *fund_transfer_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONFUNDTRANSFER;

	if (fund_transfer_info)
	{
		XTPFundTransferNotice *task_data = new XTPFundTransferNotice();
		*task_data = *fund_transfer_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnQueryETF(XTPQueryETFBaseRsp *etf_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYETF;

	if (etf_info)
	{
		XTPQueryETFBaseRsp *task_data = new XTPQueryETFBaseRsp();
		*task_data = *etf_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnQueryETFBasket(XTPQueryETFComponentRsp *etf_component_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYETFBASKET;

	if (etf_component_info)
	{
		XTPQueryETFComponentRsp *task_data = new XTPQueryETFComponentRsp();
		*task_data = *etf_component_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnQueryIPOInfoList(XTPQueryIPOTickerRsp *ipo_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYIPOINFOLIST;

	if (ipo_info)
	{
		XTPQueryIPOTickerRsp *task_data = new XTPQueryIPOTickerRsp();
		*task_data = *ipo_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};

void TraderApi::OnQueryIPOQuotaInfo(XTPQueryIPOQuotaRsp *quota_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYIPOQUOTAINFO;

	if (quota_info)
	{
		XTPQueryIPOQuotaRsp *task_data = new XTPQueryIPOQuotaRsp();
		*task_data = *quota_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
};


void TraderApi::OnQueryOptionAuctionInfo(XTPQueryOptionAuctionInfoRsp *option_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	Task* task = new Task();
	task->task_name = ONQUERYOPTIONAUCTIONINFO;

	if (option_info)
	{
		XTPQueryOptionAuctionInfoRsp *task_data = new XTPQueryOptionAuctionInfoRsp();
		*task_data = *option_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}

void TraderApi::OnCreditCashRepay(XTPCrdCashRepayRsp *cash_repay_info, XTPRI *error_info, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONCREDITCASHREPAY;
	if (cash_repay_info)
	{
		XTPCrdCashRepayRsp *task_data = new XTPCrdCashRepayRsp();
		*task_data = *cash_repay_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryCreditCashRepayInfo(XTPCrdCashRepayInfo * cash_repay_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYCREDITCASHREPAYINFO;
	if (cash_repay_info)
	{
		XTPCrdCashRepayInfo *task_data = new XTPCrdCashRepayInfo();
		*task_data = *cash_repay_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryCreditFundInfo(XTPCrdFundInfo * fund_info, XTPRI * error_info, int request_id, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYCREDITFUNDINFO;
	if (fund_info)
	{
		XTPCrdFundInfo *task_data = new XTPCrdFundInfo();
		*task_data = *fund_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryCreditDebtInfo(XTPCrdDebtInfo * debt_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYCREDITDEBTINFO;
	if (debt_info)
	{
		XTPCrdDebtInfo *task_data = new XTPCrdDebtInfo();
		*task_data = *debt_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryCreditTickerDebtInfo(XTPCrdDebtStockInfo * debt_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYCREDITTICKERDEBTINFO;
	if (debt_info)
	{
		XTPCrdDebtStockInfo *task_data = new XTPCrdDebtStockInfo();
		*task_data = *debt_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryCreditAssetDebtInfo(double remain_amount, XTPRI * error_info, int request_id, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYCREDITASSETDEBTINFO;
	task->remain_amount = remain_amount;

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryCreditTickerAssignInfo(XTPClientQueryCrdPositionStkInfo * assign_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYCREDITTICKERASSIGNINFO;
	if (assign_info)
	{
		XTPClientQueryCrdPositionStkInfo *task_data = new XTPClientQueryCrdPositionStkInfo();
		*task_data = *assign_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryCreditExcessStock(XTPClientQueryCrdSurplusStkRspInfo * stock_info, XTPRI * error_info, int request_id, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYCREDITEXCESSSTOCK;
	if (stock_info)
	{
		XTPClientQueryCrdSurplusStkRspInfo *task_data = new XTPClientQueryCrdSurplusStkRspInfo();
		*task_data = *stock_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}

void TraderApi::OnQueryMulCreditExcessStock(XTPClientQueryCrdSurplusStkRspInfo * stock_info, XTPRI * error_info, int request_id, uint64_t session_id, bool is_last) {

	Task* task = new Task();
	task->task_name = ONQUERYMULCREDITEXCESSSTOCK;
	if (stock_info)
	{
		XTPClientQueryCrdSurplusStkRspInfo *task_data = new XTPClientQueryCrdSurplusStkRspInfo();
		*task_data = *stock_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->addtional_int = session_id;
	task->task_last = is_last;
	this->task_queue.push(task);
}


void TraderApi::OnCreditExtendDebtDate(XTPCreditDebtExtendNotice *debt_extend_info, XTPRI * error_info, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONCREDITEXTENDDEBTDATE;
	if (debt_extend_info)
	{
		XTPCreditDebtExtendNotice *task_data = new XTPCreditDebtExtendNotice();
		*task_data = *debt_extend_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->addtional_int = session_id;
	this->task_queue.push(task);
}

void TraderApi::OnQueryCreditExtendDebtDateOrders(XTPCreditDebtExtendNotice *debt_extend_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYCREDITEXTENDDEBTDATEORDERS;
	if (debt_extend_info)
	{
		XTPCreditDebtExtendNotice *task_data = new XTPCreditDebtExtendNotice();
		*task_data = *debt_extend_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;

	this->task_queue.push(task);
}

void TraderApi::OnQueryCreditFundExtraInfo(XTPCrdFundExtraInfo *fund_info, XTPRI *error_info, int request_id, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYCREDITFUNDEXTRAINFO;
	if (fund_info)
	{
		XTPCrdFundExtraInfo *task_data = new XTPCrdFundExtraInfo();
		*task_data = *fund_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->addtional_int = session_id;

	this->task_queue.push(task);
}

void TraderApi::OnQueryCreditPositionExtraInfo(XTPCrdPositionExtraInfo *fund_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYCREDITPOSITIONEXTRAINFO;
	if (fund_info)
	{
		XTPCrdPositionExtraInfo *task_data = new XTPCrdPositionExtraInfo();
		*task_data = *fund_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;

	this->task_queue.push(task);
}

void TraderApi::OnCreditCashRepayDebtInterestFee(XTPCrdCashRepayDebtInterestFeeRsp *cash_repay_info, XTPRI *error_info, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONCREDITCASHREPAYDEBTINTERESTFEE;
	if (cash_repay_info)
	{
		XTPCrdCashRepayDebtInterestFeeRsp *task_data = new XTPCrdCashRepayDebtInterestFeeRsp();
		*task_data = *cash_repay_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->addtional_int = session_id;
	this->task_queue.push(task);
}



void TraderApi::OnCancelOptionCombinedOrderError(XTPOrderCancelInfo * cancel_info, XTPRI * error_info, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONCANCELOPTIONCOMBINEDORDERERROR;

	if (cancel_info) {
		XTPOrderCancelInfo *task_data = new XTPOrderCancelInfo();
		*task_data = *cancel_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->addtional_int = session_id;
	this->task_queue.push(task);
}

void TraderApi::OnOptionCombinedOrderEvent(XTPOptCombOrderInfo * order_info, XTPRI * error_info, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONOPTIONCOMBINEDORDEREVENT;

	if (order_info) {
		XTPOptCombOrderInfo *task_data = new XTPOptCombOrderInfo();
		*task_data = *order_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnOptionCombinedTradeEvent(XTPOptCombTradeReport * trade_info, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONOPTIONCOMBINEDTRADEEVENT;

	if (trade_info) {
		XTPOptCombTradeReport *task_data = new XTPOptCombTradeReport();
		*task_data = *trade_info;
		task->task_data = task_data;
	}


	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryOptionCombinedOrders(XTPQueryOptCombOrderRsp * order_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id) {
 
	Task* task = new Task();
	task->task_name = ONQUERYOPTIONCOMBINEDORDERS;

	if (order_info) {
		XTPQueryOptCombOrderRsp *task_data = new XTPQueryOptCombOrderRsp();
		*task_data = *order_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryOptionCombinedOrdersByPage(XTPQueryOptCombOrderRsp * order_info, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYOPTIONCOMBINEDORDERSBYPAGE;

	if (order_info) {
		XTPQueryOptCombOrderRsp *task_data = new XTPQueryOptCombOrderRsp();
		*task_data = *order_info;
		task->task_data = task_data;
	}

	task->req_count = req_count;
	task->order_sequence = order_sequence;
	task->query_reference = query_reference;

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}

void TraderApi::OnQueryOptionCombinedTrades(XTPQueryOptCombTradeRsp * trade_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYOPTIONCOMBINEDTRADES;

	if (trade_info) {
		XTPQueryOptCombTradeRsp *task_data = new XTPQueryOptCombTradeRsp();
		*task_data = *trade_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryOptionCombinedTradesByPage(XTPQueryOptCombTradeRsp * trade_info, int64_t req_count, int64_t trade_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYOPTIONCOMBINEDTRADESBYPAGE;


	if (trade_info) {
		XTPQueryOptCombTradeRsp *task_data = new XTPQueryOptCombTradeRsp();
		*task_data = *trade_info;
		task->task_data = task_data;
	}
	task->task_id = request_id;
	task->task_last = is_last;
	task->query_reference = query_reference;

	task->req_count = req_count;
	task->trade_sequence = trade_sequence;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryOptionCombinedPosition(XTPQueryOptCombPositionRsp * position_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id) {

	Task* task = new Task();
	task->task_name = ONQUERYOPTIONCOMBINEDPOSITION;

	if (position_info) {
		XTPQueryOptCombPositionRsp *task_data = new XTPQueryOptCombPositionRsp();
		*task_data = *position_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryOptionCombinedStrategyInfo(XTPQueryCombineStrategyInfoRsp * strategy_info, XTPRI * error_info, int request_id, bool is_last, uint64_t session_id) {
	Task* task = new Task();
	task->task_name = ONQUERYOPTIONCOMBINEDSTRATEGYINFO;

	if (strategy_info) {
		XTPQueryCombineStrategyInfoRsp *task_data = new XTPQueryCombineStrategyInfoRsp();
		*task_data = *strategy_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}

void TraderApi::OnQueryOptionCombinedExecPosition(XTPQueryOptCombExecPosRsp *position_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) {
	Task* task = new Task();
	task->task_name = ONQUERYOPTIONCOMBINEDEXECPOSITION;
	if (position_info) {
		XTPQueryOptCombExecPosRsp *task_data = new XTPQueryOptCombExecPosRsp();
		*task_data = *position_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->task_last = is_last;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


void TraderApi::OnQueryOtherServerFund(XTPFundQueryRsp *fund_info, XTPRI *error_info, int request_id, uint64_t session_id) {
	Task* task = new Task();
	task->task_name = ONQUERYOTHERSERVERFUND;

	if (fund_info) {
		XTPFundQueryRsp *task_data = new XTPFundQueryRsp();
		*task_data = *fund_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPRI *task_error = new XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_id = request_id;
	task->addtional_int = session_id;
	this->task_queue.push(task);
}


///-------------------------------------------------------------------------------------
///工作线程从队列中取出数据，转化为python对象后，进行推送
///-------------------------------------------------------------------------------------

void TraderApi::processTask()
{
	while (1)
	{
		Task* task = this->task_queue.wait_and_pop();

		switch (task->task_name)
		{
			case ONDISCONNECTED:
			{
				this->processDisconnected(task);
				break;
			}

			case ONERROR:
			{
				this->processError(task);
				break;
			}

			case ONORDEREVENT:
			{
				this->processOrderEvent(task);
				break;
			}

			case ONTRADEEVENT:
			{
				this->processTradeEvent(task);
				break;
			}

			case ONCANCELORDERERROR:
			{
				this->processCancelOrderError(task);
				break;
			}

			case ONQUERYORDER:
			{
				this->processQueryOrder(task);
				break;
			}
			case ONQUERYORDERBYPAGE:
			{
				this->processQueryOrderByPage(task);
				break;
			}
			case ONQUERYTRADE:
			{
				this->processQueryTrade(task);
				break;
			}
			case ONQUERYTRADEBYPAGE:
			{
				this->processQueryTradeByPage(task);
				break;
			}
			case ONQUERYPOSITION:
			{
				this->processQueryPosition(task);
				break;
			}

			case ONQUERYASSET:
			{
				this->processQueryAsset(task);
				break;
			}


			case ONQUERYSTRUCTUREDFUND:
			{
				this->processQueryStructuredFund(task);
				break;
			}

			case ONQUERYFUNDTRANSFER:
			{
				this->processQueryFundTransfer(task);
				break;
			}

			case ONFUNDTRANSFER:
			{
				this->processFundTransfer(task);
				break;
			}

			case ONQUERYETF:
			{
				this->processQueryETF(task);
				break;
			}

			case ONQUERYETFBASKET:
			{
				this->processQueryETFBasket(task);
				break;
			}

			case ONQUERYIPOINFOLIST:
			{
				this->processQueryIPOInfoList(task);
				break;
			}

			case ONQUERYIPOQUOTAINFO:
			{
				this->processQueryIPOQuotaInfo(task);
				break;
			}


			case ONQUERYOPTIONAUCTIONINFO:
			{
				this->processQueryOptionAuctionInfo(task);
				break;
			}

			case ONCREDITCASHREPAY: {
				this->processCreditCashRepay(task);
				break;
									}

			case ONQUERYCREDITCASHREPAYINFO: {
				this->processQueryCreditCashRepayInfo(task);
				break;
											 }

			case ONQUERYCREDITFUNDINFO: {
				this->processQueryCreditFundInfo(task);
				break;
										}

			case ONQUERYCREDITDEBTINFO: {
				this->processQueryCreditDebtInfo(task);
				break;
										}

			case ONQUERYCREDITTICKERDEBTINFO: {
				this->processQueryCreditTickerDebtInfo(task);
				break;
											  }

			case ONQUERYCREDITASSETDEBTINFO: {
				this->processQueryCreditAssetDebtInfo(task);
				break;
											 }

			case ONQUERYCREDITTICKERASSIGNINFO: {
				this->processQueryCreditTickerAssignInfo(task);
				break;
												}

			case ONQUERYCREDITEXCESSSTOCK: {
				this->processQueryCreditExcessStock(task);
				break;
										   }
			case ONQUERYMULCREDITEXCESSSTOCK: {
				this->processQueryMulCreditExcessStock(task);
				break;
										   }

			case ONCREDITEXTENDDEBTDATE: {
				this->processCreditExtendDebtDate(task);
				break;
											  }

			case ONQUERYCREDITEXTENDDEBTDATEORDERS: {
				this->processQueryCreditExtendDebtDateOrders(task);
				break;
											 }

			case ONQUERYCREDITFUNDEXTRAINFO: {
				this->processQueryCreditFundExtraInfo(task);
				break;
												}

			case ONQUERYCREDITPOSITIONEXTRAINFO: {
				this->processQueryCreditPositionExtraInfo(task);
				break;
										   }
			case ONCREDITCASHREPAYDEBTINTERESTFEE: {
				this->processCreditCashRepayDebtInterestFee(task);
				break;
									}

			case ONOPTIONCOMBINEDORDEREVENT: {
				this->processOptionCombinedOrderEvent(task);
				break;
											 }

			case ONOPTIONCOMBINEDTRADEEVENT: {
				this->processOptionCombinedTradeEvent(task);
				break;
											 }

			case ONQUERYOPTIONCOMBINEDORDERS: {
				this->processQueryOptionCombinedOrders(task);
				break;
											  }

			case ONQUERYOPTIONCOMBINEDORDERSBYPAGE: {
				this->processQueryOptionCombinedOrdersByPage(task);
				break;
													}

			case ONQUERYOPTIONCOMBINEDTRADES: {
				this->processQueryOptionCombinedTrades(task);
				break;
											  }

			case ONQUERYOPTIONCOMBINEDTRADESBYPAGE: {
				this->processQueryOptionCombinedTradesByPage(task);
				break;
													}

			case ONQUERYOPTIONCOMBINEDPOSITION: {
				this->processQueryOptionCombinedPosition(task);
				break;
			}

			case ONQUERYOPTIONCOMBINEDSTRATEGYINFO: {
				this->processQueryOptionCombinedStrategyInfo(task);
				break;
			}
			case ONCANCELOPTIONCOMBINEDORDERERROR: {
				this->processCancelOptionCombinedOrderError(task);
				break;
			}
			case ONQUERYOPTIONCOMBINEDEXECPOSITION: {
				this->processQueryOptionCombinedExecPosition(task);
				break;
			}
			case ONQUERYOTHERSERVERFUND: {
				this->processQueryOtherServerFund(task);
				break;
			}
		};
	}
};

void TraderApi::processDisconnected(Task *task)
{
	PyLock lock;
	this->onDisconnected(task->addtional_int, task->task_id);
	delete task;
};

void TraderApi::processError(Task *task)
{
	PyLock lock;
	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onError(error);
	delete task;
};

void TraderApi::processOrderEvent(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPOrderInfo *task_data = (XTPOrderInfo*)task->task_data;
		data["cancel_time"] = task_data->cancel_time;
		data["update_time"] = task_data->update_time;
		data["order_cancel_xtp_id"] = task_data->order_cancel_xtp_id;
		data["order_client_id"] = task_data->order_client_id;
		data["trade_amount"] = task_data->trade_amount;
		data["price_type"] = (int)task_data->price_type;
		data["order_type"] = task_data->order_type;
		data["price"] = task_data->price;
		data["qty_traded"] = task_data->qty_traded;
		data["qty_left"] = task_data->qty_left;
		data["order_local_id"] = task_data->order_local_id;
		data["side"] = (int)task_data->side;
		data["position_effect"] = (int)task_data->position_effect;
		data["reserved1"] = (int)task_data->reserved1;
		data["reserved2"] = (int)task_data->reserved2;
		data["order_submit_status"] = (int)task_data->order_submit_status;
		data["insert_time"] = task_data->insert_time;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["order_status"] = (int)task_data->order_status;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["order_cancel_client_id"] = task_data->order_cancel_client_id;
		data["market"] = (int)task_data->market;
		data["quantity"] = task_data->quantity;
		data["business_type"] = int(task_data->business_type);
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onOrderEvent(data, error, task->addtional_int);
	delete task;
};

void TraderApi::processTradeEvent(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPTradeReport *task_data = (XTPTradeReport*)task->task_data;
		data["branch_pbu"] = addEndingChar(task_data->branch_pbu);
		data["trade_amount"] = task_data->trade_amount;
		data["exec_id"] = addEndingChar(task_data->exec_id);
		data["trade_type"] = task_data->trade_type;
		data["order_client_id"] = task_data->order_client_id;
		data["order_exch_id"] = addEndingChar(task_data->order_exch_id);
		data["price"] = task_data->price;
		data["report_index"] = task_data->report_index;
		data["local_order_id"] = task_data->local_order_id;
		data["trade_time"] = task_data->trade_time;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["side"] = (int)task_data->side;
		data["position_effect"] = (int)task_data->position_effect;
		data["reserved1"] = (int)task_data->reserved1;
		data["reserved2"] = (int)task_data->reserved2;
		data["market"] = (int)task_data->market;
		data["quantity"] = task_data->quantity;
		data["business_type"] = int(task_data->business_type);
		delete task->task_data;
	}

	this->onTradeEvent(data, task->addtional_int);
	delete task;
};

void TraderApi::processCancelOrderError(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPOrderCancelInfo *task_data = (XTPOrderCancelInfo*)task->task_data;
		data["order_cancel_xtp_id"] = task_data->order_cancel_xtp_id;
		data["order_xtp_id"] = task_data->order_xtp_id;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onCancelOrderError(data, error, task->addtional_int);
	delete task;
};

void TraderApi::processQueryOrder(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPQueryOrderRsp *task_data = (XTPQueryOrderRsp*)task->task_data;
		data["cancel_time"] = task_data->cancel_time;
		data["update_time"] = task_data->update_time;
		data["order_cancel_xtp_id"] = task_data->order_cancel_xtp_id;
		data["order_client_id"] = task_data->order_client_id;
		data["trade_amount"] = task_data->trade_amount;
		data["price_type"] = (int)task_data->price_type;
		data["order_type"] = task_data->order_type;
		data["price"] = task_data->price;
		data["qty_traded"] = task_data->qty_traded;
		data["qty_left"] = task_data->qty_left;
		data["order_local_id"] = addEndingChar(task_data->order_local_id);
		data["side"] = (int)task_data->side;
		data["position_effect"] = (int)task_data->position_effect;
		data["reserved1"] = (int)task_data->reserved1;
		data["reserved2"] = (int)task_data->reserved2;
		data["order_submit_status"] = (int)task_data->order_submit_status;
		data["insert_time"] = task_data->insert_time;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["order_status"] = (int)task_data->order_status;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["order_cancel_client_id"] = task_data->order_cancel_client_id;
		data["market"] = (int)task_data->market;
		data["quantity"] = task_data->quantity;
		data["business_type"] = int(task_data->business_type);
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryOrder(data, error, task->task_id, task->task_last, task->addtional_int);
	delete task;
};

void TraderApi::processQueryOrderByPage(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPQueryOrderRsp *task_data = (XTPQueryOrderRsp*)task->task_data;
		data["cancel_time"] = task_data->cancel_time;
		data["update_time"] = task_data->update_time;
		data["order_cancel_xtp_id"] = task_data->order_cancel_xtp_id;
		data["order_client_id"] = task_data->order_client_id;
		data["trade_amount"] = task_data->trade_amount;
		data["price_type"] = (int)task_data->price_type;
		data["order_type"] = task_data->order_type;
		data["price"] = task_data->price;
		data["qty_traded"] = task_data->qty_traded;
		data["qty_left"] = task_data->qty_left;
		data["order_local_id"] = addEndingChar(task_data->order_local_id);
		data["side"] = (int)task_data->side;
		data["position_effect"] = (int)task_data->position_effect;
		data["reserved1"] = (int)task_data->reserved1;
		data["reserved2"] = (int)task_data->reserved2;
		data["order_submit_status"] = (int)task_data->order_submit_status;
		data["insert_time"] = task_data->insert_time;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["order_status"] = (int)task_data->order_status;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["order_cancel_client_id"] = task_data->order_cancel_client_id;
		data["market"] = (int)task_data->market;
		data["quantity"] = task_data->quantity;
		data["business_type"] = int(task_data->business_type);
		delete task->task_data;
	}



	this->onQueryOrderByPage(data, task->addtional_int_two, task->addtional_int_three, task->addtional_int_four, task->task_id, task->task_last, task->addtional_int);
	delete task;
};

void TraderApi::processQueryTrade(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPQueryTradeRsp *task_data = (XTPQueryTradeRsp*)task->task_data;
		data["branch_pbu"] = addEndingChar(task_data->branch_pbu);
		data["trade_amount"] = task_data->trade_amount;
		data["exec_id"] = addEndingChar(task_data->exec_id);
		data["trade_type"] = task_data->trade_type;
		data["order_client_id"] = task_data->order_client_id;
		data["order_exch_id"] = addEndingChar(task_data->order_exch_id);
		data["price"] = task_data->price;
		data["report_index"] = task_data->report_index;
		data["local_order_id"] = task_data->local_order_id;
		data["trade_time"] = task_data->trade_time;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["side"] = (int)task_data->side;
		data["position_effect"] = (int)task_data->position_effect;
		data["reserved1"] = (int)task_data->reserved1;
		data["reserved2"] = (int)task_data->reserved2;
		data["market"] = (int)task_data->market;
		data["quantity"] = task_data->quantity;
		data["business_type"] = int(task_data->business_type);
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryTrade(data, error, task->task_id, task->task_last, task->addtional_int);
	delete task;
};

void TraderApi::processQueryTradeByPage(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPQueryTradeRsp *task_data = (XTPQueryTradeRsp*)task->task_data;
		data["branch_pbu"] = addEndingChar(task_data->branch_pbu);
		data["trade_amount"] = task_data->trade_amount;
		data["exec_id"] = addEndingChar(task_data->exec_id);
		data["trade_type"] = task_data->trade_type;
		data["order_client_id"] = task_data->order_client_id;
		data["order_exch_id"] = addEndingChar(task_data->order_exch_id);
		data["price"] = task_data->price;
		data["report_index"] = task_data->report_index;
		data["local_order_id"] = task_data->local_order_id;
		data["trade_time"] = task_data->trade_time;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["side"] = (int)task_data->side;
		data["position_effect"] = (int)task_data->position_effect;
		data["reserved1"] = (int)task_data->reserved1;
		data["reserved2"] = (int)task_data->reserved2;
		data["market"] = (int)task_data->market;
		data["quantity"] = task_data->quantity;
		data["business_type"] = (int)(task_data->business_type);
		delete task->task_data;
	}

	this->onQueryTradeByPage(data, task->addtional_int_two, task->addtional_int_three, task->addtional_int_four, task->task_id, task->task_last, task->addtional_int);
	delete task;
};

void TraderApi::processQueryPosition(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPQueryStkPositionRsp *task_data = (XTPQueryStkPositionRsp*)task->task_data;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["ticker_name"] = addEndingChar(task_data->ticker_name);
		data["market"] = int(task_data->market);	//手动修改
		data["total_qty"] = task_data->total_qty;
		data["sellable_qty"] = task_data->sellable_qty;
		data["avg_price"] = task_data->avg_price;
		data["unrealized_pnl"] = task_data->unrealized_pnl;
		data["yesterday_position"] = task_data->yesterday_position;
		data["purchase_redeemable_qty"] = task_data->purchase_redeemable_qty;	

		data["position_direction"] = (int)task_data->position_direction;
		data["position_security_type"] = (int)task_data->position_security_type;
		data["executable_option"] = task_data->executable_option;
		data["lockable_position"] = task_data->lockable_position;
		data["executable_underlying"] = task_data->executable_underlying;
		data["locked_position"] = task_data->locked_position;
		data["usable_locked_position"] = task_data->usable_locked_position;

		data["profit_price"] = task_data->profit_price;
		data["buy_cost"] = task_data->buy_cost;
		data["profit_cost"] = task_data->profit_cost;

		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryPosition(data, error, task->task_id, task->task_last, task->addtional_int);

	delete task;
};

void TraderApi::processQueryAsset(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPQueryAssetRsp *task_data = (XTPQueryAssetRsp*)task->task_data;

		data["total_asset"] = task_data->total_asset;
		data["buying_power"] = task_data->buying_power;
		data["security_asset"] = task_data->security_asset;
		data["fund_buy_amount"] = task_data->fund_buy_amount;
		data["fund_buy_fee"] = task_data->fund_buy_fee;
		data["fund_sell_amount"] = task_data->fund_sell_amount;
		data["fund_sell_fee"] = task_data->fund_sell_fee;
		data["withholding_amount"] = task_data->withholding_amount;
		data["account_type"] = (int)task_data->account_type;

		data["frozen_margin"] = task_data->frozen_margin;
		data["frozen_exec_cash"] = task_data->frozen_exec_cash;
		data["frozen_exec_fee"] = task_data->frozen_exec_fee;
		data["pay_later"] = task_data->pay_later;
		data["preadva_pay"] = task_data->preadva_pay;
		data["orig_banlance"] = task_data->orig_banlance;
		data["banlance"] = task_data->banlance;
		data["deposit_withdraw"] = task_data->deposit_withdraw;
		data["trade_netting"] = task_data->trade_netting;
		data["captial_asset"] = task_data->captial_asset;
		data["force_freeze_amount"] = task_data->force_freeze_amount;
		data["preferred_amount"] = task_data->preferred_amount;

		data["repay_stock_aval_banlance"] = task_data->repay_stock_aval_banlance;
		data["fund_order_data_charges"] = task_data->fund_order_data_charges;
		data["fund_cancel_data_charges"] = task_data->fund_cancel_data_charges;
		data["exchange_cur_risk_degree"] = task_data->exchange_cur_risk_degree;
		data["company_cur_risk_degree"] = task_data->company_cur_risk_degree;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryAsset(data, error, task->task_id, task->task_last, task->addtional_int);
	delete task;
};

void TraderApi::processQueryStructuredFund(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPStructuredFundInfo *task_data = (XTPStructuredFundInfo*)task->task_data;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["sf_ticker"] = addEndingChar(task_data->sf_ticker);
		data["sf_ticker_name"] = addEndingChar(task_data->sf_ticker_name);
		data["ticker"] = addEndingChar(task_data->ticker);
		data["ticker_name"] = addEndingChar(task_data->ticker_name);
		data["split_merge_status"] = (int)task_data->split_merge_status;
		data["ratio"] = task_data->ratio;
		data["min_split_qty"] = task_data->min_split_qty;
		data["min_merge_qty"] = task_data->min_merge_qty;
		data["net_price"] = task_data->net_price;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryStructuredFund(data, error, task->task_id, task->task_last, task->addtional_int);
	delete task;
};

void TraderApi::processQueryFundTransfer(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPFundTransferNotice *task_data = (XTPFundTransferNotice*)task->task_data;
		data["serial_id"] = task_data->serial_id;
		data["transfer_type"] = (int)task_data->transfer_type;
		data["amount"] = task_data->amount;
		data["oper_status"] = (int)task_data->oper_status;
		data["transfer_time"] = task_data->transfer_time;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryFundTransfer(data, error, task->task_id, task->task_last, task->addtional_int);
	delete task;
};

void TraderApi::processFundTransfer(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPFundTransferNotice *task_data = (XTPFundTransferNotice*)task->task_data;
		data["serial_id"] = task_data->serial_id;
		data["transfer_type"] = (int)task_data->transfer_type;
		data["amount"] = task_data->amount;
		data["oper_status"] = (int)task_data->oper_status;
		data["transfer_time"] = task_data->transfer_time;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onFundTransfer(data, error, task->addtional_int);
	delete task;
};

void TraderApi::processQueryETF(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPQueryETFBaseRsp *task_data = (XTPQueryETFBaseRsp*)task->task_data;
		data["market"] = (int)task_data->market;
		data["etf"] = addEndingChar(task_data->etf);
		data["subscribe_redemption_ticker"] = addEndingChar(task_data->subscribe_redemption_ticker);
		data["unit"] = task_data->unit;
		data["subscribe_status"] = task_data->subscribe_status;
		data["redemption_status"] = task_data->redemption_status;
		data["max_cash_ratio"] = task_data->max_cash_ratio;
		data["estimate_amount"] = task_data->estimate_amount;
		data["cash_component"] = task_data->cash_component;
		data["net_value"] = task_data->net_value;
		data["total_amount"] = task_data->total_amount;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryETF(data, error, task->task_id, task->task_last, task->addtional_int);
	delete task;
};

void TraderApi::processQueryETFBasket(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPQueryETFComponentRsp *task_data = (XTPQueryETFComponentRsp*)task->task_data;
		data["market"] = (int)task_data->market;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["component_ticker"] = addEndingChar(task_data->component_ticker);
		data["component_name"] = addEndingChar(task_data->component_name);
		data["quantity"] = task_data->quantity;
		data["component_market"] = (int)task_data->component_market;
		data["replace_type"] = (int)task_data->replace_type;
		data["premium_ratio"] = task_data->premium_ratio;
		data["amount"] = task_data->amount;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryETFBasket(data, error, task->task_id, task->task_last, task->addtional_int);
	delete task;
};

void TraderApi::processQueryIPOInfoList(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPQueryIPOTickerRsp *task_data = (XTPQueryIPOTickerRsp*)task->task_data;
		data["market"] = (int)task_data->market;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["ticker_name"] = addEndingChar(task_data->ticker_name);
		data["price"] = task_data->price;
		data["unit"] = task_data->unit;
		data["qty_upper_limit"] = task_data->qty_upper_limit;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryIPOInfoList(data, error, task->task_id, task->task_last, task->addtional_int);
	delete task;
};

void TraderApi::processQueryIPOQuotaInfo(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPQueryIPOQuotaRsp *task_data = (XTPQueryIPOQuotaRsp*)task->task_data;
		data["market"] = (int)task_data->market;
		data["quantity"] = task_data->quantity;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryIPOQuotaInfo(data, error, task->task_id, task->task_last, task->addtional_int);
	delete task;
};



void TraderApi::processQueryOptionAuctionInfo(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPQueryOptionAuctionInfoRsp *task_data = (XTPQueryOptionAuctionInfoRsp*)task->task_data;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["security_id_source"] = (int)task_data->security_id_source;
		data["symbol"] = addEndingChar(task_data->symbol);
		data["contract_id"] = addEndingChar(task_data->contract_id);
		data["underlying_security_id"] = addEndingChar(task_data->underlying_security_id);
		data["underlying_security_id_source"] = (int)task_data->underlying_security_id_source;

		data["list_date"] = task_data->list_date;
		data["last_trade_date"] = task_data->last_trade_date;
		data["ticker_type"] = (int)task_data->ticker_type;
		data["day_trading"] = task_data->day_trading;

		data["call_or_put"] = (int)task_data->call_or_put;
		data["delivery_day"] = task_data->delivery_day;
		data["delivery_month"] = task_data->delivery_month;

		data["exercise_type"] = (int)task_data->exercise_type;
		data["exercise_begin_date"] = task_data->exercise_begin_date;
		data["exercise_end_date"] = task_data->exercise_end_date;
		data["exercise_price"] = task_data->exercise_price;

		data["qty_unit"] = task_data->qty_unit;
		data["contract_unit"] = task_data->contract_unit;
		data["contract_position"] = task_data->contract_position;

		data["prev_close_price"] = task_data->prev_close_price;
		data["prev_clearing_price"] = task_data->prev_clearing_price;

		data["lmt_buy_max_qty"] = task_data->lmt_buy_max_qty;
		data["lmt_buy_min_qty"] = task_data->lmt_buy_min_qty;
		data["lmt_sell_max_qty"] = task_data->lmt_sell_max_qty;
		data["lmt_sell_min_qty"] = task_data->lmt_sell_min_qty;
		data["mkt_buy_max_qty"] = task_data->mkt_buy_max_qty;
		data["mkt_buy_min_qty"] = task_data->mkt_buy_min_qty;
		data["mkt_sell_max_qty"] = task_data->mkt_sell_max_qty;
		data["mkt_sell_min_qty"] = task_data->mkt_sell_min_qty;

		data["price_tick"] = task_data->price_tick;
		data["upper_limit_price"] = task_data->upper_limit_price;
		data["lower_limit_price"] = task_data->lower_limit_price;
		data["sell_margin"] = task_data->sell_margin;
		data["margin_ratio_param1"] = task_data->margin_ratio_param1;
		data["margin_ratio_param2"] = task_data->margin_ratio_param2;

		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryOptionAuctionInfo(data, error, task->task_id, task->task_last, task->addtional_int);
	delete task;
};


void TraderApi::processCreditCashRepay(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPCrdCashRepayRsp *task_data = (XTPCrdCashRepayRsp*)task->task_data;
		data["xtp_id"] = task_data->xtp_id;
		data["request_amount"] = task_data->request_amount;
		data["cash_repay_amount"] = task_data->cash_repay_amount;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onCreditCashRepay(data, error, task->addtional_int);
}

void TraderApi::processQueryCreditCashRepayInfo(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPCrdCashRepayInfo *task_data = (XTPCrdCashRepayInfo*)task->task_data;
		data["xtp_id"] = task_data->xtp_id;
		data["status"] = (int)task_data->status;
		data["request_amount"] = task_data->request_amount;
		data["cash_repay_amount"] = task_data->cash_repay_amount;
		data["position_effect"] = (int)task_data->position_effect;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryCreditCashRepayInfo(data, error, task->task_id, task->task_last, task->addtional_int);
}

void TraderApi::processQueryCreditFundInfo(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPCrdFundInfo *task_data = (XTPCrdFundInfo*)task->task_data;
		data["maintenance_ratio"] = task_data->maintenance_ratio;
		data["all_asset"] = task_data->all_asset;
		data["all_debt"] = task_data->all_debt;
		data["line_of_credit"] = task_data->line_of_credit;
		data["guaranty"] = task_data->guaranty;
		data["reserved"] = task_data->reserved;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryCreditFundInfo(data, error, task->task_id, task->addtional_int);
}

void TraderApi::processQueryCreditDebtInfo(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPCrdDebtInfo *task_data = (XTPCrdDebtInfo*)task->task_data;
		data["debt_type"] = task_data->debt_type;
		data["debt_id"] = addEndingChar(task_data->debt_id);
		data["position_id"] = task_data->position_id;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["debt_status"] = task_data->debt_status;
		data["market"] = (int)task_data->market;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["order_date"] = task_data->order_date;
		data["end_date"] = task_data->end_date;
		data["orig_end_date"] = task_data->orig_end_date;
		data["is_extended"] = task_data->is_extended;
		data["remain_amt"] = task_data->remain_amt;
		data["remain_qty"] = task_data->remain_qty;
		data["remain_principal"] = task_data->remain_principal;
		data["due_right_qty"] = task_data->due_right_qty;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryCreditDebtInfo(data, error, task->task_id, task->task_last, task->addtional_int);
}

void TraderApi::processQueryCreditTickerDebtInfo(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPCrdDebtStockInfo *task_data = (XTPCrdDebtStockInfo*)task->task_data;
		data["market"] = (int)task_data->market;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["stock_repay_quantity"] = task_data->stock_repay_quantity;
		data["stock_total_quantity"] = task_data->stock_total_quantity;
		delete task->task_data;	
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryCreditTickerDebtInfo(data, error, task->task_id, task->task_last, task->addtional_int);
}

void TraderApi::processQueryCreditAssetDebtInfo(Task *task) {
	PyLock lock;
	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryCreditAssetDebtInfo(task->remain_amount, error, task->task_id, task->addtional_int);
}

void TraderApi::processQueryCreditTickerAssignInfo(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPClientQueryCrdPositionStkInfo *task_data = (XTPClientQueryCrdPositionStkInfo*)task->task_data;
		data["market"] = (int)task_data->market;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["limit_qty"] = task_data->limit_qty;
		data["yesterday_qty"] = task_data->yesterday_qty;
		data["left_qty"] = task_data->left_qty;
		data["frozen_qty"] = task_data->frozen_qty;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryCreditTickerAssignInfo(data, error, task->task_id, task->task_last, task->addtional_int);
}

void TraderApi::processQueryCreditExcessStock(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPClientQueryCrdSurplusStkRspInfo *task_data = (XTPClientQueryCrdSurplusStkRspInfo*)task->task_data;
		data["market"] = (int)task_data->market;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["transferable_quantity"] = task_data->transferable_quantity;
		data["transferred_quantity"] = task_data->transferred_quantity;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryCreditExcessStock(data, error, task->task_id, task->addtional_int);
}


void TraderApi::processQueryMulCreditExcessStock(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPClientQueryCrdSurplusStkRspInfo *task_data = (XTPClientQueryCrdSurplusStkRspInfo*)task->task_data;
		data["market"] = (int)task_data->market;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["transferable_quantity"] = task_data->transferable_quantity;
		data["transferred_quantity"] = task_data->transferred_quantity;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryMulCreditExcessStock(data, error, task->task_id, task->addtional_int, task->task_last);
}

void TraderApi::processCreditExtendDebtDate(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPCreditDebtExtendNotice *task_data = (XTPCreditDebtExtendNotice*)task->task_data;
		data["xtpid"] = task_data->xtpid;
		data["debt_id"] = addEndingChar(task_data->debt_id);
		data["oper_status"] = (int)task_data->oper_status;
		data["oper_time"] = task_data->oper_time;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onCreditExtendDebtDate(data, error, task->addtional_int);
}

void TraderApi::processQueryCreditExtendDebtDateOrders(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPCreditDebtExtendNotice *task_data = (XTPCreditDebtExtendNotice*)task->task_data;
		data["xtpid"] = task_data->xtpid;
		data["debt_id"] = addEndingChar(task_data->debt_id);
		data["oper_status"] = (int)task_data->oper_status;
		data["oper_time"] = task_data->oper_time;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryCreditExtendDebtDateOrders(data, error,task->task_id,task->task_last, task->addtional_int);
}

void TraderApi::processQueryCreditFundExtraInfo(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPCrdFundExtraInfo *task_data = (XTPCrdFundExtraInfo*)task->task_data;
		data["mf_rs_avl_used"] = task_data->mf_rs_avl_used;
		data["reserve"] = addEndingChar(task_data->reserve);
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryCreditFundExtraInfo(data, error,task->task_id, task->addtional_int);
}

void TraderApi::processQueryCreditPositionExtraInfo(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPCrdPositionExtraInfo *task_data = (XTPCrdPositionExtraInfo*)task->task_data;
		data["market"] = (int)task_data->market;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["mf_rs_avl_used"] = task_data->mf_rs_avl_used;
		data["reserve"] = addEndingChar(task_data->reserve);
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryCreditPositionExtraInfo(data, error,task->task_id,task->task_last, task->addtional_int);
}

void TraderApi::processCreditCashRepayDebtInterestFee(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPCrdCashRepayDebtInterestFeeRsp *task_data = (XTPCrdCashRepayDebtInterestFeeRsp*)task->task_data;
		data["xtp_id"] = task_data->xtp_id;
		data["request_amount"] = task_data->request_amount;
		data["cash_repay_amount"] = task_data->cash_repay_amount;
		data["debt_compact_id"] = addEndingChar(task_data->debt_compact_id);
		data["unknow"] = addEndingChar(task_data->unknow);
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onCreditCashRepayDebtInterestFee(data, error, task->addtional_int);
}



void TraderApi::processOptionCombinedOrderEvent(Task *task) {
	PyLock lock;
	dict data;

	if (task->task_data){
		XTPOptCombOrderInfo *task_data = (XTPOptCombOrderInfo*)task->task_data;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["order_client_id"] = task_data->order_client_id;
		data["order_cancel_client_id"] = task_data->order_cancel_client_id;
		data["order_cancel_xtp_id"] = task_data->order_cancel_xtp_id;
		data["market"] = (int)task_data->market;
		data["quantity"] = task_data->quantity;
		data["side"] = (int)task_data->side;
		data["business_type"] = (int)task_data->business_type;
		data["qty_traded"] = task_data->qty_traded;
		data["qty_left"] = task_data->qty_left;
		data["insert_time"] = task_data->insert_time;
		data["update_time"] = task_data->update_time;
		data["cancel_time"] = task_data->cancel_time;
		data["trade_amount"] = task_data->trade_amount;
		data["order_local_id"] = task_data->order_local_id;
		data["order_status"] = (int)task_data->order_status;
		data["order_submit_status"] = (int)task_data->order_submit_status;
		data["order_type"] = task_data->order_type;
		data["strategy_id"] = task_data->opt_comb_info.strategy_id;
		data["comb_num"] = task_data->opt_comb_info.comb_num;
		data["num_legs"] = task_data->opt_comb_info.num_legs;
		boost::python::list leg_detail_list;
		for (int i = 0;i < task_data->opt_comb_info.num_legs; i++) 
		{
			dict leg_detail_dict;
			leg_detail_dict["leg_security_id"] = task_data->opt_comb_info.leg_detail[i].leg_security_id;
			leg_detail_dict["leg_cntr_type"] = (int)task_data->opt_comb_info.leg_detail[i].leg_cntr_type;
			leg_detail_dict["leg_side"] = (int)task_data->opt_comb_info.leg_detail[i].leg_side;
			leg_detail_dict["leg_covered"] = (int)task_data->opt_comb_info.leg_detail[i].leg_covered;
			leg_detail_dict["leg_qty"] = task_data->opt_comb_info.leg_detail[i].leg_qty;
			leg_detail_list.append(leg_detail_dict);
		}
		data["leg_detail"] = leg_detail_list;
	}
	

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onOptionCombinedOrderEvent(data, error,task->addtional_int);
}

void TraderApi::processOptionCombinedTradeEvent(Task *task) {
	PyLock lock;

	dict data;
	if (task->task_data){
		XTPOptCombTradeReport *task_data = (XTPOptCombTradeReport*)task->task_data;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["order_client_id"] = task_data->order_client_id;
		data["market"] = (int)task_data->market;
		data["local_order_id"] = task_data->local_order_id;
		data["exec_id"] = task_data->exec_id;
		data["quantity"] = task_data->quantity;
		data["trade_time"] = task_data->trade_time;
		data["trade_amount"] = task_data->trade_amount;
		data["report_index"] = task_data->report_index;
		data["order_exch_id"] = task_data->order_exch_id;
		data["trade_type"] = task_data->trade_type;
		data["side"] = (int)task_data->side;
		data["business_type"] = (int)task_data->business_type;
		data["branch_pbu"] = task_data->branch_pbu;
		data["strategy_id"] = task_data->opt_comb_info.strategy_id;
		data["comb_num"] = task_data->opt_comb_info.comb_num;
		data["num_legs"] = task_data->opt_comb_info.num_legs;
		boost::python::list leg_detail_list;
		for (int i = 0;i < task_data->opt_comb_info.num_legs; i++) 
		{
			dict leg_detail_dict;
			leg_detail_dict["leg_security_id"] = task_data->opt_comb_info.leg_detail[i].leg_security_id;
			leg_detail_dict["leg_cntr_type"] = (int)task_data->opt_comb_info.leg_detail[i].leg_cntr_type;
			leg_detail_dict["leg_side"] = (int)task_data->opt_comb_info.leg_detail[i].leg_side;
			leg_detail_dict["leg_covered"] = (int)task_data->opt_comb_info.leg_detail[i].leg_covered;
			leg_detail_dict["leg_qty"] = task_data->opt_comb_info.leg_detail[i].leg_qty;
			leg_detail_list.append(leg_detail_dict);
		}
		data["leg_detail"] = leg_detail_list;
	}
	
	this->onOptionCombinedTradeEvent(data,task->addtional_int);
}

void TraderApi::processQueryOptionCombinedOrders(Task *task) {
	PyLock lock;

	dict data;
	if (task->task_data){
		XTPQueryOptCombOrderRsp *task_data = (XTPQueryOptCombOrderRsp*)task->task_data;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["order_client_id"] = task_data->order_client_id;
		data["order_cancel_client_id"] = task_data->order_cancel_client_id;
		data["order_cancel_xtp_id"] = task_data->order_cancel_xtp_id;
		data["market"] = (int)task_data->market;
		data["quantity"] = task_data->quantity;
		data["side"] = (int)task_data->side;
		data["business_type"] = (int)task_data->business_type;
		data["qty_traded"] = task_data->qty_traded;
		data["qty_left"] = task_data->qty_left;
		data["insert_time"] = task_data->insert_time;
		data["update_time"] = task_data->update_time;
		data["cancel_time"] = task_data->cancel_time;
		data["trade_amount"] = task_data->trade_amount;
		data["order_local_id"] = task_data->order_local_id;
		data["order_status"] = (int)task_data->order_status;
		data["order_submit_status"] = (int)task_data->order_submit_status;
		data["order_type"] = task_data->order_type;
		data["strategy_id"] = task_data->opt_comb_info.strategy_id;
		data["comb_num"] = task_data->opt_comb_info.comb_num;
		data["num_legs"] = task_data->opt_comb_info.num_legs;
		boost::python::list leg_detail_list;
		for (int i = 0;i < task_data->opt_comb_info.num_legs; i++) 
		{
			dict leg_detail_dict;
			leg_detail_dict["leg_security_id"] = task_data->opt_comb_info.leg_detail[i].leg_security_id;
			leg_detail_dict["leg_cntr_type"] =(int) task_data->opt_comb_info.leg_detail[i].leg_cntr_type;
			leg_detail_dict["leg_side"] = (int)task_data->opt_comb_info.leg_detail[i].leg_side;
			leg_detail_dict["leg_covered"] = (int)task_data->opt_comb_info.leg_detail[i].leg_covered;
			leg_detail_dict["leg_qty"] = task_data->opt_comb_info.leg_detail[i].leg_qty;
			leg_detail_list.append(leg_detail_dict);
		}
		data["leg_detail"] = leg_detail_list;
	}
	

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryOptionCombinedOrders(data, error, task->task_id, task->task_last,task->addtional_int);
}

void TraderApi::processQueryOptionCombinedOrdersByPage(Task *task) {
	PyLock lock;

	dict data;
	if (task->task_data){
		XTPQueryOptCombOrderRsp *task_data = (XTPQueryOptCombOrderRsp*)task->task_data;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["order_client_id"] = task_data->order_client_id;
		data["order_cancel_client_id"] = task_data->order_cancel_client_id;
		data["order_cancel_xtp_id"] = task_data->order_cancel_xtp_id;
		data["market"] = (int)task_data->market;
		data["quantity"] = task_data->quantity;
		data["side"] = (int)task_data->side;
		data["business_type"] = (int)task_data->business_type;
		data["qty_traded"] = task_data->qty_traded;
		data["qty_left"] = task_data->qty_left;
		data["insert_time"] = task_data->insert_time;
		data["update_time"] = task_data->update_time;
		data["cancel_time"] = task_data->cancel_time;
		data["trade_amount"] = task_data->trade_amount;
		data["order_local_id"] = task_data->order_local_id;
		data["order_status"] = (int)task_data->order_status;
		data["order_submit_status"] = (int)task_data->order_submit_status;
		data["order_type"] = task_data->order_type;
		data["strategy_id"] = task_data->opt_comb_info.strategy_id;
		data["comb_num"] = task_data->opt_comb_info.comb_num;
		data["num_legs"] = task_data->opt_comb_info.num_legs;
		boost::python::list leg_detail_list;
		for (int i = 0;i < task_data->opt_comb_info.num_legs; i++) 
		{
			dict leg_detail_dict;
			leg_detail_dict["leg_security_id"] = task_data->opt_comb_info.leg_detail[i].leg_security_id;
			leg_detail_dict["leg_cntr_type"] = (int)task_data->opt_comb_info.leg_detail[i].leg_cntr_type;
			leg_detail_dict["leg_side"] = (int)task_data->opt_comb_info.leg_detail[i].leg_side;
			leg_detail_dict["leg_covered"] = (int)task_data->opt_comb_info.leg_detail[i].leg_covered;
			leg_detail_dict["leg_qty"] = task_data->opt_comb_info.leg_detail[i].leg_qty;
			leg_detail_list.append(leg_detail_dict);
		}
		data["leg_detail"] = leg_detail_list;

	}

	this->onQueryOptionCombinedOrdersByPage(data, task->req_count, task->order_sequence, task->query_reference, task->task_id, task->task_last,task->addtional_int);
}

void TraderApi::processQueryOptionCombinedTrades(Task *task) {
	PyLock lock;

	dict data;
	if (task->task_data){
		XTPQueryOptCombTradeRsp *task_data = (XTPQueryOptCombTradeRsp*)task->task_data;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["order_client_id"] = task_data->order_client_id;
		data["market"] = (int)task_data->market;
		data["local_order_id"] = task_data->local_order_id;
		data["exec_id"] = task_data->exec_id;
		data["quantity"] = task_data->quantity;
		data["trade_time"] = task_data->trade_time;
		data["trade_amount"] = task_data->trade_amount;
		data["report_index"] = task_data->report_index;
		data["order_exch_id"] = task_data->order_exch_id;
		data["trade_type"] = task_data->trade_type;
		data["side"] = (int)task_data->side;
		data["business_type"] = (int)task_data->business_type;
		data["branch_pbu"] = task_data->branch_pbu;
		data["strategy_id"] = task_data->opt_comb_info.strategy_id;
		data["comb_num"] = task_data->opt_comb_info.comb_num;
		data["num_legs"] = task_data->opt_comb_info.num_legs;
		boost::python::list leg_detail_list;
		for (int i = 0;i < task_data->opt_comb_info.num_legs; i++) 
		{
			dict leg_detail_dict;
			leg_detail_dict["leg_security_id"] = task_data->opt_comb_info.leg_detail[i].leg_security_id;
			leg_detail_dict["leg_cntr_type"] = (int)task_data->opt_comb_info.leg_detail[i].leg_cntr_type;
			leg_detail_dict["leg_side"] = (int)task_data->opt_comb_info.leg_detail[i].leg_side;
			leg_detail_dict["leg_covered"] = (int)task_data->opt_comb_info.leg_detail[i].leg_covered;
			leg_detail_dict["leg_qty"] = task_data->opt_comb_info.leg_detail[i].leg_qty;
			leg_detail_list.append(leg_detail_dict);
		}
		data["leg_detail"] = leg_detail_list;
	}
	


	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryOptionCombinedTrades(data, error, task->task_id, task->task_last,task->addtional_int);
}

void TraderApi::processQueryOptionCombinedTradesByPage(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data){
		XTPQueryOptCombTradeRsp *task_data = (XTPQueryOptCombTradeRsp*)task->task_data;
		data["order_xtp_id"] = task_data->order_xtp_id;
		data["order_client_id"] = task_data->order_client_id;
		data["market"] = (int)task_data->market;
		data["local_order_id"] = task_data->local_order_id;
		data["exec_id"] = task_data->exec_id;
		data["quantity"] = task_data->quantity;
		data["trade_time"] = task_data->trade_time;
		data["trade_amount"] = task_data->trade_amount;
		data["report_index"] = task_data->report_index;
		data["order_exch_id"] = task_data->order_exch_id;
		data["trade_type"] = task_data->trade_type;
		data["side"] = (int)task_data->side;
		data["business_type"] = (int)task_data->business_type;
		data["branch_pbu"] = task_data->branch_pbu;
		data["strategy_id"] = task_data->opt_comb_info.strategy_id;
		data["comb_num"] = task_data->opt_comb_info.comb_num;
		data["num_legs"] = task_data->opt_comb_info.num_legs;
		boost::python::list leg_detail_list;
		for (int i = 0;i < task_data->opt_comb_info.num_legs; i++) 
		{
			dict leg_detail_dict;
			leg_detail_dict["leg_security_id"] = task_data->opt_comb_info.leg_detail[i].leg_security_id;
			leg_detail_dict["leg_cntr_type"] = (int)task_data->opt_comb_info.leg_detail[i].leg_cntr_type;
			leg_detail_dict["leg_side"] = (int)task_data->opt_comb_info.leg_detail[i].leg_side;
			leg_detail_dict["leg_covered"] = (int)task_data->opt_comb_info.leg_detail[i].leg_covered;
			leg_detail_dict["leg_qty"] = task_data->opt_comb_info.leg_detail[i].leg_qty;
			leg_detail_list.append(leg_detail_dict);
		}
		data["leg_detail"] = leg_detail_list;
	}
	

	this->onQueryOptionCombinedTradesByPage(data, task->req_count, task->trade_sequence, task->query_reference, task->task_id, task->task_last,task->addtional_int);
}

void TraderApi::processQueryOptionCombinedPosition(Task *task) {
	PyLock lock;

	dict data;
	if (task->task_data){
		XTPQueryOptCombPositionRsp *task_data = (XTPQueryOptCombPositionRsp*)task->task_data;
		data["strategy_id"] = task_data->strategy_id;
		data["strategy_name"] = addEndingChar(task_data->strategy_name);
		data["market"] = (int)task_data->market;
		data["total_qty"] = task_data->total_qty;
		data["available_qty"] = task_data->available_qty;
		data["yesterday_position"] = task_data->yesterday_position;
		data["comb_strategy_id"] = task_data->opt_comb_info.strategy_id;
		data["comb_num"] = task_data->opt_comb_info.comb_num;
		data["num_legs"] = task_data->opt_comb_info.num_legs;
		boost::python::list leg_detail_list;
		for (int i = 0;i < XTP_STRATEGE_LEG_NUM; i++)
		{
			dict leg_detail_dict;
			leg_detail_dict["leg_security_id"] = task_data->opt_comb_info.leg_detail[i].leg_security_id;
			leg_detail_dict["leg_cntr_type"] = (int)task_data->opt_comb_info.leg_detail[i].leg_cntr_type;
			leg_detail_dict["leg_side"] = (int)task_data->opt_comb_info.leg_detail[i].leg_side;
			leg_detail_dict["leg_covered"] = (int)task_data->opt_comb_info.leg_detail[i].leg_covered;
			leg_detail_dict["leg_qty"] = task_data->opt_comb_info.leg_detail[i].leg_qty;
			leg_detail_list.append(leg_detail_dict);
		}
		data["leg_detail"] = leg_detail_list;

	}


	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryOptionCombinedPosition(data, error, task->task_id, task->task_last,task->addtional_int);
}

void TraderApi::processQueryOptionCombinedStrategyInfo(Task *task) {
	PyLock lock;

	dict data;
	if (task->task_data){
		XTPQueryCombineStrategyInfoRsp *task_data = (XTPQueryCombineStrategyInfoRsp*)task->task_data;
		//printf("task_data->strategy_id:%s,strategy_name:%s\n",task_data->strategy_id,task_data->strategy_name);
		data["strategy_id"] = task_data->strategy_id;
		//char strategy_name[64] = {"\0"};
		//printf("sizeof(task_data->strategy_name):%d",sizeof(task_data->strategy_name));
		//strcpy(strategy_name,  task_data->strategy_name);
		//printf("task_data->strategy_name:%s",strategy_name);
		//string strategy_name = addEndingChar(task_data->strategy_name);
		data["strategy_name"] = addEndingChar(task_data->strategy_name);
		//printf("addEndingChar->strategy_name:%s",addEndingChar(task_data->strategy_name).c_str());
		data["market"] = (int)task_data->market;
		data["leg_num"] = task_data->leg_num;
		boost::python::list leg_strategy_list;
		for (int i = 0;i < XTP_STRATEGE_LEG_NUM; i++)
		{
			dict leg_strategy_dict;
			leg_strategy_dict["call_or_put"] = (int)task_data->leg_strategy[i].call_or_put;
			leg_strategy_dict["position_side"] = (int)task_data->leg_strategy[i].position_side;
			leg_strategy_dict["exercise_price_seq"] = task_data->leg_strategy[i].exercise_price_seq;
			leg_strategy_dict["expire_date_seq"] = task_data->leg_strategy[i].expire_date_seq;
			leg_strategy_dict["leg_qty"] = task_data->leg_strategy[i].leg_qty;
			leg_strategy_list.append(leg_strategy_dict);
		}
		data["leg_strategy"] = leg_strategy_list;
		data["expire_date_type"] = (int)task_data->expire_date_type;
		data["underlying_type"] = (int)task_data->underlying_type;
		data["auto_sep_type"] = (int)task_data->auto_sep_type;
	}
	

	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryOptionCombinedStrategyInfo(data, error, task->task_id, task->task_last,task->addtional_int);
}


void TraderApi::processCancelOptionCombinedOrderError(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data){
		XTPOptCombOrderCancelInfo *task_data = (XTPOptCombOrderCancelInfo*)task->task_data;
		data["order_cancel_xtp_id"] = task_data->order_cancel_xtp_id;
		data["order_xtp_id"] = task_data->order_xtp_id;
	}
	
	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onCancelOptionCombinedOrderError(data, error,task->addtional_int);
}


void TraderApi::processQueryOptionCombinedExecPosition(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data){
		XTPQueryOptCombExecPosRsp *task_data = (XTPQueryOptCombExecPosRsp*)task->task_data;
		data["market"] = (int)task_data->market;
		data["cntrt_code_1"] = addEndingChar(task_data->cntrt_code_1);
		data["cntrt_name_1"] = addEndingChar(task_data->cntrt_name_1);
		data["position_side_1"] = (int)task_data->position_side_1;
		data["call_or_put_1"] = (int)task_data->call_or_put_1;
		data["avl_qty_1"] = task_data->avl_qty_1;
		data["orig_own_qty_1"] = task_data->orig_own_qty_1;
		data["own_qty_1"] = task_data->own_qty_1;

		data["cntrt_code_2"] = addEndingChar(task_data->cntrt_code_2);
		data["cntrt_name_2"] = addEndingChar(task_data->cntrt_name_2);
		data["position_side_2"] = (int)task_data->position_side_2;
		data["call_or_put_2"] = (int)task_data->call_or_put_2;
		data["avl_qty_2"] = task_data->avl_qty_2;
		data["orig_own_qty_2"] = task_data->orig_own_qty_2;
		data["own_qty_2"] = task_data->own_qty_2;

		data["net_qty"] = task_data->net_qty;
		data["order_qty"] = task_data->order_qty;
		data["confirm_qty"] = task_data->confirm_qty;
		data["avl_qty"] = task_data->avl_qty;
		//data["reserved"] = task_data->reserved;
	}


	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryOptionCombinedExecPosition(data, error,task->task_id, task->task_last,task->addtional_int);
}

void TraderApi::processQueryOtherServerFund(Task *task) {
	PyLock lock;
	dict data;
	if (task->task_data){
		XTPFundQueryRsp *task_data = (XTPFundQueryRsp*)task->task_data;
		data["amount"] = task_data->amount;
		data["query_type"] = (int)task_data->query_type;
	}


	dict error;
	if (task->task_error)
	{
		XTPRI *task_error = (XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryOtherServerFund(data, error,task->task_id,task->addtional_int);
}

///-------------------------------------------------------------------------------------
///主动函数
///-------------------------------------------------------------------------------------

void TraderApi::createTraderApi(uint8_t clientid, string path, int log_level)
{
	this->api = XTP::API::TraderApi::CreateTraderApi(clientid, path.c_str(),(XTP_LOG_LEVEL)log_level);
	this->api->RegisterSpi(this);
};

void TraderApi::release()
{
	this->api->Release();
};

int TraderApi::exit()
{
	//该函数在原生API里没有，用于安全退出API用，原生的join似乎不太稳定
	this->api->RegisterSpi(NULL);
	this->api->Release();
	this->api = NULL;
	return 1;
};

string TraderApi::getTradingDay()
{
	string ret ="";
	const char* p = this->api->GetTradingDay();
	if (p == NULL)
		ret = "NULL";
	else
		ret = p;
	return ret;
};

dict TraderApi::getApiLastError()
{
	dict d;
	XTPRI *error = this->api->GetApiLastError();
	if(error == NULL)
		return d;

	d["error_id"] = error->error_id;
	d["error_msg"] = addEndingChar(error->error_msg);

	return d;
}

string TraderApi::getApiVersion()
{
	string ret ="";
	const char* p = this->api->GetApiVersion();
	if (p == NULL)
		ret = "NULL";
	else
		ret = p;
	return ret;
}

uint8_t TraderApi::getClientIDByXTPID(uint64_t orderid)
{
	return this->api->GetClientIDByXTPID(orderid);
}

string TraderApi::getAccountByXTPID(uint64_t orderid)
{
	string ret ="";
	const char* p = this->api->GetAccountByXTPID(orderid);
	if (p == NULL)
		ret = "NULL";
	else
		ret = p;
	
	return ret;
}

void TraderApi::subscribePublicTopic(int type)
{
	this->api->SubscribePublicTopic((XTP_TE_RESUME_TYPE)type);
}

void TraderApi::setSoftwareKey(string key)
{
	this->api->SetSoftwareKey(key.c_str());
}

void TraderApi::setSoftwareVersion(string version)
{
	this->api->SetSoftwareVersion(version.c_str());
}

void TraderApi::setHeartBeatInterval(uint32_t interval)
{
	this->api->SetHeartBeatInterval(interval);
};

uint64_t TraderApi::login(string ip, int port, string user, string password, int socktype,string local_ip)
{
	return this->api->Login(ip.c_str(), port, user.c_str(), password.c_str(), (XTP_PROTOCOL_TYPE)socktype,local_ip.c_str());
};

int TraderApi::logout(uint64_t sessionid)
{
	return this->api->Logout(sessionid);
};


uint64_t TraderApi::insertOrder(dict req, uint64_t sessionid)
{
	XTPOrderInsertInfo myreq = XTPOrderInsertInfo();
	memset(&myreq, 0, sizeof(myreq));
	getDouble(req, "stop_price", &myreq.stop_price);
	getDouble(req, "price", &myreq.price);
	getStr(req, "ticker", myreq.ticker);

	getUint32(req, "order_client_id", &myreq.order_client_id);
	getUint64(req, "order_xtp_id", &myreq.order_xtp_id);
	getInt64(req, "quantity", &myreq.quantity);

	int price_type;
	int side;
	int position_effect;
	int reserved1;
	int reserved2;
	int market;
	int business_type;
	getInt(req, "price_type", &price_type);
	getInt(req, "side", &side);
	getInt(req, "position_effect", &position_effect);
	getInt(req, "reserved1", &reserved1);
	getInt(req, "reserved2", &reserved2);
	getInt(req, "market", &market);
	getInt(req, "business_type", &business_type);
	myreq.price_type = (XTP_PRICE_TYPE)price_type;
	myreq.side = (XTP_SIDE_TYPE)side;
	myreq.position_effect = (XTP_POSITION_EFFECT_TYPE)position_effect;
	myreq.reserved1 = reserved1;
	myreq.reserved2 = reserved2;
	myreq.market = (XTP_MARKET_TYPE)market;
	myreq.business_type = (XTP_BUSINESS_TYPE)business_type;

	return this->api->InsertOrder(&myreq, sessionid);
};

uint64_t TraderApi::cancelOrder(uint64_t orderid, uint64_t sessionid)
{
	return this->api->CancelOrder(orderid, sessionid);
}

int TraderApi::queryOrderByXTPID(uint64_t orderid, uint64_t sessionid, int reqid)
{
	return this->api->QueryOrderByXTPID(orderid, sessionid, reqid);
};

int TraderApi::queryOrders(dict req, uint64_t sessionid, int reqid)
{
	XTPQueryOrderReq myreq = XTPQueryOrderReq();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ticker", myreq.ticker);
	getInt64(req, "end_time", &myreq.end_time);
	getInt64(req, "begin_time", &myreq.begin_time);
	return this->api->QueryOrders(&myreq, sessionid, reqid);
};

int TraderApi::queryUnfinishedOrders(uint64_t sessionid, int reqid)
{
	return this->api->QueryUnfinishedOrders(sessionid, reqid);
};

int TraderApi::queryTradesByXTPID(uint64_t orderid, uint64_t sessionid, int reqid)
{
	return this->api->QueryTradesByXTPID(orderid, sessionid, reqid);
};

int TraderApi::queryTrades(dict req, uint64_t sessionid, int reqid)
{
	XTPQueryTraderReq myreq = XTPQueryTraderReq();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ticker", myreq.ticker);
	getInt64(req, "end_time", &myreq.end_time);
	getInt64(req, "begin_time", &myreq.begin_time);
	return this->api->QueryTrades(&myreq, sessionid, reqid);
};

int TraderApi::queryPosition(string ticker, uint64_t sessionid, int reqid)
{
	return this->api->QueryPosition(ticker.c_str(), sessionid, reqid);
};

int TraderApi::queryAsset(uint64_t sessionid, int reqid)
{
	return this->api->QueryAsset(sessionid, reqid);
};

int TraderApi::queryStructuredFund(dict req, uint64_t sessionid, int reqid)
{
	XTPQueryStructuredFundInfoReq myreq = XTPQueryStructuredFundInfoReq();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "sf_ticker", myreq.sf_ticker);

	int exchange_id;
	getInt(req, "exchange_id", &exchange_id);
	myreq.exchange_id = (XTP_EXCHANGE_TYPE)exchange_id;

	return this->api->QueryStructuredFund(&myreq, sessionid, reqid);

};

uint64_t TraderApi::fundTransfer(dict req, uint64_t sessionid)
{
	XTPFundTransferReq myreq = XTPFundTransferReq();
	memset(&myreq, 0, sizeof(myreq));
	getUint64(req, "serial_id", &myreq.serial_id);
	getStr(req, "fund_account", myreq.fund_account);
	getStr(req, "password", myreq.password);
	getDouble(req, "amount", &myreq.amount);

	int transfer_type;
	getInt(req, "transfer_type", &transfer_type);
	myreq.transfer_type = (XTP_FUND_TRANSFER_TYPE)transfer_type;

	return this->api->FundTransfer(&myreq, sessionid);
};

int TraderApi::queryFundTransfer(dict req, uint64_t sessionid, int reqid)
{
	XTPQueryFundTransferLogReq myreq = XTPQueryFundTransferLogReq();
	memset(&myreq, 0, sizeof(myreq));
	getUint64(req, "serial_id", &myreq.serial_id);
	return this->api->QueryFundTransfer(&myreq, sessionid, reqid);
};

int TraderApi::queryETF(dict req, uint64_t sessionid, int reqid)
{
	XTPQueryETFBaseReq myreq = XTPQueryETFBaseReq();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ticker", myreq.ticker);

	int market;
	getInt(req, "market", &market);
	myreq.market = (XTP_MARKET_TYPE)market;

	return this->api->QueryETF(&myreq, sessionid, reqid);
};

int TraderApi::queryETFTickerBasket(dict req, uint64_t sessionid, int reqid)
{
	XTPQueryETFComponentReq myreq = XTPQueryETFComponentReq();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ticker", myreq.ticker);

	int market;
	getInt(req, "market", &market);
	myreq.market = (XTP_MARKET_TYPE)market;

	return this->api->QueryETFTickerBasket(&myreq, sessionid, reqid);
};

int TraderApi::queryIPOInfoList(uint64_t sessionid, int reqid)
{
	return this->api->QueryIPOInfoList(sessionid, reqid);
};

int TraderApi::queryIPOQuotaInfo(uint64_t sessionid, int reqid)
{
	return this->api->QueryIPOQuotaInfo(sessionid, reqid);
};

int TraderApi::queryOptionAuctionInfo(dict req,uint64_t sessionid, int reqid)
{
	XTPQueryOptionAuctionInfoReq myreq = XTPQueryOptionAuctionInfoReq();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ticker", myreq.ticker);

	int market;
	getInt(req, "market", &market);
	myreq.market = (XTP_MARKET_TYPE)market;
	return this->api->QueryOptionAuctionInfo(&myreq,sessionid, reqid);
};

uint64_t TraderApi::creditCashRepay(double remain_amount, uint64_t session_id = 0) {
	return this->api->CreditCashRepay(remain_amount, session_id);
}

int TraderApi::queryCreditCashRepayInfo(uint64_t session_id, int request_id) {

	return this->api->QueryCreditCashRepayInfo(session_id, request_id);
}

int TraderApi::queryCreditFundInfo(uint64_t session_id, int request_id) {
	return this->api->QueryCreditFundInfo(session_id, request_id);
}

int TraderApi::queryCreditDebtInfo(uint64_t session_id, int request_id) {
	return this->api->QueryCreditDebtInfo(session_id, request_id);
}

int TraderApi::queryCreditTickerDebtInfo(dict req, uint64_t session_id, int request_id) {
	XTPClientQueryCrdDebtStockReq myreq = XTPClientQueryCrdDebtStockReq();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ticker", myreq.ticker);

	int market;
	getInt(req, "market", &market);
	myreq.market = (XTP_MARKET_TYPE)market;
	return this->api->QueryCreditTickerDebtInfo(&myreq, session_id, request_id);
}

int TraderApi::queryCreditAssetDebtInfo(uint64_t session_id, int request_id) {
	return this->api->QueryCreditAssetDebtInfo(session_id, request_id);
}

int TraderApi::queryCreditTickerAssignInfo(dict req, uint64_t session_id, int request_id) {
	XTPClientQueryCrdPositionStockReq myreq = XTPClientQueryCrdPositionStockReq();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ticker", myreq.ticker);

	int market;
	getInt(req, "market", &market);
	myreq.market = (XTP_MARKET_TYPE)market;
	return this->api->QueryCreditTickerAssignInfo(&myreq, session_id, request_id);
}

int TraderApi::queryCreditExcessStock(dict req, uint64_t session_id, int request_id) {
	XTPClientQueryCrdSurplusStkReqInfo myreq = XTPClientQueryCrdSurplusStkReqInfo();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ticker", myreq.ticker);

	int market;
	getInt(req, "market", &market);
	myreq.market = (XTP_MARKET_TYPE)market;
	return this->api->QueryCreditExcessStock(&myreq, session_id, request_id);
}

int TraderApi::queryMulCreditExcessStock(dict req, uint64_t session_id, int request_id) {
	XTPClientQueryCrdSurplusStkReqInfo myreq = XTPClientQueryCrdSurplusStkReqInfo();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ticker", myreq.ticker);

	int market;
	getInt(req, "market", &market);
	myreq.market = (XTP_MARKET_TYPE)market;
	return this->api->QueryMulCreditExcessStock(&myreq, session_id, request_id);
}

uint64_t TraderApi::creditExtendDebtDate(dict req,uint64_t session_id) {
	XTPCreditDebtExtendReq myreq = XTPCreditDebtExtendReq();
	memset(&myreq, 0, sizeof(myreq));
	getUint64(req, "xtpid", &myreq.xtpid);
	getStr(req, "debt_id", myreq.debt_id);
	getUint32(req, "xtpid", &myreq.defer_days);
	getStr(req, "fund_account", myreq.fund_account);
	getStr(req, "password", myreq.password);

	return this->api->CreditExtendDebtDate(&myreq, session_id);
}

int TraderApi::queryCreditExtendDebtDateOrders(uint64_t xtp_id, uint64_t session_id, int request_id) {

	return this->api->QueryCreditExtendDebtDateOrders(xtp_id, session_id, request_id);
}

int TraderApi::queryCreditFundExtraInfo(uint64_t session_id, int request_id) {

	return this->api->QueryCreditFundExtraInfo(session_id, request_id);
}

int TraderApi::queryCreditPositionExtraInfo(dict req, uint64_t session_id, int request_id) {

	XTPClientQueryCrdPositionStockReq myreq = XTPClientQueryCrdPositionStockReq();
	memset(&myreq, 0, sizeof(myreq));

	int market;
	getInt(req, "market", &market);
	myreq.market = (XTP_MARKET_TYPE)market;
	getStr(req, "ticker", myreq.ticker);

	return this->api->QueryCreditPositionExtraInfo(&myreq, session_id, request_id);
}

int TraderApi::queryOrdersByPage(dict req, uint64_t sessionid, int reqid)
{
	XTPQueryOrderByPageReq myreq = XTPQueryOrderByPageReq();
	memset(&myreq, 0, sizeof(myreq));
	getInt64(req, "req_count", &myreq.req_count);
	getInt64(req, "reference", &myreq.reference);
	getInt64(req, "reserved", &myreq.reserved);
	return this->api->QueryOrdersByPage(&myreq, sessionid, reqid);
};

int TraderApi::queryTradesByPage(dict req, uint64_t sessionid, int reqid)
{
	XTPQueryTraderByPageReq myreq = XTPQueryTraderByPageReq();
	memset(&myreq, 0, sizeof(myreq));
	getInt64(req, "req_count", &myreq.req_count);
	getInt64(req, "reference", &myreq.reference);
	getInt64(req, "reserved", &myreq.reserved);
	return this->api->QueryTradesByPage(&myreq, sessionid, reqid);
};

bool TraderApi::isServerRestart(uint64_t session_id)
{
	return this->api->IsServerRestart(session_id);
};

uint64_t TraderApi::creditCashRepayDebtInterestFee(string debt_id, double amount, uint64_t session_id) {
	return this->api->CreditCashRepayDebtInterestFee(debt_id.c_str(),amount, session_id);
}

uint64_t TraderApi::creditSellStockRepayDebtInterestFee(dict req, string debt_id, uint64_t session_id) {
	XTPOrderInsertInfo myreq = XTPOrderInsertInfo();
	memset(&myreq, 0, sizeof(myreq));
	getDouble(req, "stop_price", &myreq.stop_price);
	getDouble(req, "price", &myreq.price);
	getStr(req, "ticker", myreq.ticker);

	getUint32(req, "order_client_id", &myreq.order_client_id);
	getUint64(req, "order_xtp_id", &myreq.order_xtp_id);
	getInt64(req, "quantity", &myreq.quantity);

	int price_type;
	int side;
	int position_effect;
	int reserved1;
	int reserved2;
	int market;
	int business_type;
	getInt(req, "price_type", &price_type);
	getInt(req, "side", &side);
	getInt(req, "position_effect", &position_effect);
	getInt(req, "reserved1", &reserved1);
	getInt(req, "reserved2", &reserved2);
	getInt(req, "market", &market);
	getInt(req, "business_type", &business_type);
	myreq.price_type = (XTP_PRICE_TYPE)price_type;
	myreq.side = (XTP_SIDE_TYPE)side;
	myreq.position_effect = (XTP_POSITION_EFFECT_TYPE)position_effect;
	myreq.reserved1 = reserved1;
	myreq.reserved2 = reserved2;
	myreq.market = (XTP_MARKET_TYPE)market;
	myreq.business_type = (XTP_BUSINESS_TYPE)business_type;

	return this->api->CreditSellStockRepayDebtInterestFee(&myreq,debt_id.c_str(),session_id);
}


uint64_t TraderApi::insertOptionCombinedOrder(dict req, uint64_t session_id) {

	XTPOptCombOrderInsertInfo query_param;
	memset(&query_param, 0, sizeof(query_param));

	getUint64(req, "order_xtp_id", &query_param.order_xtp_id);
	getUint32(req, "order_client_id", &query_param.order_client_id);

	int side;
	int market;
	int business_type;
	dict opt_comb_info; 

	getInt(req, "side", &side);
	getInt(req, "market", &market);
	getInt(req, "business_type", &business_type);
	query_param.side = (XTP_SIDE_TYPE)side;
	query_param.market = (XTP_MARKET_TYPE)market;
	query_param.business_type = (XTP_BUSINESS_TYPE)business_type;

	getNestedDictChar(req, "opt_comb_info", "strategy_id", query_param.opt_comb_info.strategy_id);
	getNestedDictChar(req, "opt_comb_info", "comb_num", query_param.opt_comb_info.comb_num);
	getNestedDictValue(req, "opt_comb_info", "num_legs", &query_param.opt_comb_info.num_legs);

	int leg_cntr_type;
	int leg_side;
	int leg_covered;
	for (int i = 0; i< query_param.opt_comb_info.num_legs; i++)
	{
		getNestedDictChar2(req, "opt_comb_info", "leg_detail", "leg_security_id", query_param.opt_comb_info.leg_detail[i].leg_security_id, i);
		getNestedDictValue2(req, "opt_comb_info", "leg_detail", "leg_cntr_type", &leg_cntr_type, i);
		getNestedDictValue2(req, "opt_comb_info", "leg_detail", "leg_side", &leg_side, i);
		getNestedDictValue2(req, "opt_comb_info", "leg_detail", "leg_covered", &leg_covered, i);
		getNestedDictValue2(req, "opt_comb_info", "leg_detail", "leg_qty", &query_param.opt_comb_info.leg_detail[i].leg_qty, i);
		query_param.opt_comb_info.leg_detail[i].leg_cntr_type = (XTP_OPT_CALL_OR_PUT_TYPE)leg_cntr_type;
		query_param.opt_comb_info.leg_detail[i].leg_side = (XTP_POSITION_DIRECTION_TYPE)leg_side;
		query_param.opt_comb_info.leg_detail[i].leg_covered = (XTP_OPT_COVERED_OR_UNCOVERED)leg_covered;

	}

	getInt64(req, "quantity", &query_param.quantity);
	return this->api->InsertOptionCombinedOrder(&query_param, session_id);
}

int TraderApi::queryOptionCombinedUnfinishedOrders(uint64_t session_id, int request_id) {

	return this->api->QueryOptionCombinedUnfinishedOrders(session_id, request_id);
}

int TraderApi::queryOptionCombinedOrderByXTPID(const uint64_t order_xtp_id, uint64_t session_id, int request_id) {

	return this->api->QueryOptionCombinedOrderByXTPID(order_xtp_id, session_id, request_id);
}

int TraderApi::queryOptionCombinedOrders(dict req, uint64_t session_id, int request_id) {

	XTPQueryOptCombOrderReq query_param;
	memset(&query_param, 0, sizeof(query_param));

	getChar(req, "comb_num", query_param.comb_num);
	getInt64(req, "begin_time", &query_param.begin_time);
	getInt64(req, "end_time", &query_param.end_time);
	return this->api->QueryOptionCombinedOrders(&query_param, session_id, request_id);
}

int TraderApi::queryOptionCombinedOrdersByPage(dict req, uint64_t session_id, int request_id) {

	XTPQueryOptCombOrderByPageReq query_param;
	memset(&query_param, 0, sizeof(query_param));

	getInt64(req, "req_count", &query_param.req_count);
	getInt64(req, "reference", &query_param.reference);
	getInt64(req, "reserved", &query_param.reserved);
	return this->api->QueryOptionCombinedOrdersByPage(&query_param, session_id, request_id);
}

int TraderApi::queryOptionCombinedTradesByXTPID(const uint64_t order_xtp_id, uint64_t session_id, int request_id) {

	return this->api->QueryOptionCombinedTradesByXTPID(order_xtp_id, session_id, request_id);
}

int TraderApi::queryOptionCombinedTrades(dict req, uint64_t session_id, int request_id) {

	XTPQueryOptCombTraderReq query_param;
	memset(&query_param, 0, sizeof(query_param));

	getChar(req, "comb_num", query_param.comb_num);
	getInt64(req, "begin_time", &query_param.begin_time);
	getInt64(req, "end_time", &query_param.end_time);
	return this->api->QueryOptionCombinedTrades(&query_param, session_id, request_id);
}

int TraderApi::queryOptionCombinedTradesByPage(dict req, uint64_t session_id, int request_id) {

	XTPQueryOptCombTraderByPageReq query_param;
	memset(&query_param, 0, sizeof(query_param));

	getInt64(req, "req_count", &query_param.req_count);
	getInt64(req, "reference", &query_param.reference);
	getInt64(req, "reserved", &query_param.reserved);
	return this->api->QueryOptionCombinedTradesByPage(&query_param, session_id, request_id);
}

int TraderApi::queryOptionCombinedPosition(dict req, uint64_t session_id, int request_id) {

	XTPQueryOptCombPositionReq query_param;
	memset(&query_param, 0, sizeof(query_param));

	int market;
	getChar(req, "combined_security_id", query_param.comb_num);
	getInt(req, "market", &market);
	query_param.market = (XTP_MARKET_TYPE)market;

	//getValue(req, "market", &query_param.market);
	return this->api->QueryOptionCombinedPosition(&query_param, session_id, request_id);
}

int TraderApi::queryOptionCombinedStrategyInfo(uint64_t session_id, int request_id) {
	//getValue(req, "market", &query_param.market);
	return this->api->QueryOptionCombinedStrategyInfo(session_id, request_id);
}

uint64_t TraderApi::cancelOptionCombinedOrder(const uint64_t order_xtp_id, uint64_t session_id = 0) {
	return this->api->CancelOptionCombinedOrder(order_xtp_id, session_id);
}

int TraderApi::queryOptionCombinedExecPosition(dict req,uint64_t session_id, int request_id) {
	XTPQueryOptCombExecPosReq query_param;
	memset(&query_param, 0, sizeof(query_param));

	int market;
	getInt(req, "market", &market);
	query_param.market = (XTP_MARKET_TYPE)market;
	getStr(req, "cntrt_code_1", query_param.cntrt_code_1);
	getStr(req, "cntrt_code_2", query_param.cntrt_code_2);
	return this->api->QueryOptionCombinedExecPosition(&query_param,session_id, request_id);
}

int TraderApi::queryOtherServerFund(dict req,uint64_t session_id, int request_id) {
	XTPFundQueryReq query_param;
	memset(&query_param, 0, sizeof(query_param));
	getChar(req, "fund_account", query_param.fund_account);
	getChar(req, "password", query_param.password);
	int query_type;
	getInt(req, "query_type", &query_type);
	query_param.query_type = (XTP_FUND_QUERY_TYPE)query_type;
	return this->api->QueryOtherServerFund(&query_param,session_id, request_id);
}

///-------------------------------------------------------------------------------------
///Boost.Python封装
///-------------------------------------------------------------------------------------

struct TraderApiWrap : TraderApi, wrapper < TraderApi >
{
	virtual void onDisconnected(uint64_t session, int reason)
	{
		try
		{
			this->get_override("onDisconnected")(session, reason);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onError(dict data)
	{
		try
		{
			this->get_override("onError")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onOrderEvent(dict data, dict error, uint64_t session)
	{
		try
		{
			this->get_override("onOrderEvent")(data, error, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onTradeEvent(dict data, uint64_t session)
	{
		try
		{
			this->get_override("onTradeEvent")(data, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onCancelOrderError(dict data, dict error, uint64_t session)
	{
		try
		{
			this->get_override("onCancelOrderError")(data, error, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onQueryOrder(dict data, dict error, int id, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryOrder")(data, error, id, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};
	virtual void onQueryOrderByPage(dict data, int64_t req_count, int64_t order_sequence, int64_t query_reference, int reqid, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryOrderByPage")(data,req_count,order_sequence, query_reference, reqid, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};
	virtual void onQueryTrade(dict data, dict error, int id, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryTrade")(data, error, id, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onQueryTradeByPage(dict data, int64_t req_count, int64_t trade_sequence, int64_t query_reference, int reqid, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryTradeByPage")(data, req_count,trade_sequence,query_reference, reqid, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onQueryPosition(dict data, dict error, int id, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryPosition")(data, error, id, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onQueryAsset(dict data, dict error, int id, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryAsset")(data, error, id, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onQueryStructuredFund(dict data, dict error, int id, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryStructuredFund")(data, error, id, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onQueryFundTransfer(dict data, dict error, int id, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryFundTransfer")(data, error, id, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onFundTransfer(dict data, dict error, uint64_t session)
	{
		try
		{
			this->get_override("onFundTransfer")(data, error, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onQueryETF(dict data, dict error, int id, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryETF")(data, error, id, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onQueryETFBasket(dict data, dict error, int id, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryETFBasket")(data, error, id, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onQueryIPOInfoList(dict data, dict error, int id, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryIPOInfoList")(data, error, id, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onQueryIPOQuotaInfo(dict data, dict error, int id, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryIPOQuotaInfo")(data, error, id, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};


	virtual void onQueryOptionAuctionInfo(dict data, dict error, int id, bool last, uint64_t session)
	{
		try
		{
			this->get_override("onQueryOptionAuctionInfo")(data, error, id, last, session);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onCreditCashRepay(dict data, dict error_info, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onCreditCashRepay")(data, error_info, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryCreditCashRepayInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryCreditCashRepayInfo")(data, error_info, request_id, is_last, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryCreditFundInfo(dict data, dict error_info, int request_id, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryCreditFundInfo")(data, error_info, request_id, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryCreditDebtInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryCreditDebtInfo")(data, error_info, request_id, is_last, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryCreditTickerDebtInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryCreditTickerDebtInfo")(data, error_info, request_id, is_last, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryCreditAssetDebtInfo(double remain_amount, dict error_info, int request_id, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryCreditAssetDebtInfo")(remain_amount, error_info, request_id, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryCreditTickerAssignInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryCreditTickerAssignInfo")(data, error_info, request_id, is_last, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryCreditExcessStock(dict data, dict error_info, int request_id, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryCreditExcessStock")(data, error_info, request_id, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryMulCreditExcessStock(dict data, dict error_info, int request_id, uint64_t session, bool is_last) {
		PyLock lock;

		try {
			this->get_override("onQueryMulCreditExcessStock")(data, error_info, request_id, session,is_last);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onCreditExtendDebtDate(dict data, dict error_info, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onCreditExtendDebtDate")(data, error_info, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryCreditExtendDebtDateOrders(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryCreditExtendDebtDateOrders")(data, error_info, request_id,is_last, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryCreditFundExtraInfo(dict data, dict error_info, int request_id, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryCreditFundExtraInfo")(data, error_info, request_id, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryCreditPositionExtraInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryCreditPositionExtraInfo")(data, error_info, request_id,is_last, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onCreditCashRepayDebtInterestFee(dict data, dict error_info, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onCreditCashRepayDebtInterestFee")(data, error_info, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onOptionCombinedOrderEvent(dict data, dict error_info, uint64_t session_id) {
		PyLock lock;

		try {
			this->get_override("onOptionCombinedOrderEvent")(data, error_info, session_id);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onOptionCombinedTradeEvent(dict data, uint64_t session_id) {
		PyLock lock;

		try {
			this->get_override("onOptionCombinedTradeEvent")(data,session_id);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryOptionCombinedOrders(dict data, dict error_info, int request_id, bool is_last, uint64_t session_id) {
		PyLock lock;

		try {
			this->get_override("onQueryOptionCombinedOrders")(data, error_info, request_id, is_last, session_id);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryOptionCombinedOrdersByPage(dict data, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {
		PyLock lock;

		try {
			this->get_override("onQueryOptionCombinedOrdersByPage")(data, req_count, order_sequence, query_reference, request_id, is_last, session_id);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryOptionCombinedTrades(dict data, dict error_info, int request_id, bool is_last, uint64_t session_id) {
		PyLock lock;

		try {
			this->get_override("onQueryOptionCombinedTrades")(data, error_info, request_id, is_last,session_id);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryOptionCombinedTradesByPage(dict data, int64_t req_count, int64_t trade_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {
		PyLock lock;

		try {
			this->get_override("onQueryOptionCombinedTradesByPage")(data, req_count, trade_sequence, query_reference, request_id, is_last,session_id);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryOptionCombinedPosition(dict data, dict error_info, int request_id, bool is_last, uint64_t session_id) {
		PyLock lock;

		try {
			this->get_override("onQueryOptionCombinedPosition")(data, error_info, request_id, is_last,session_id);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryOptionCombinedStrategyInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session_id) {
		PyLock lock;

		try {
			this->get_override("onQueryOptionCombinedStrategyInfo")(data, error_info, request_id, is_last,session_id);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onCancelOptionCombinedOrderError(dict data, dict error_info, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onCancelOptionCombinedOrderError")(data, error_info,session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryOptionCombinedExecPosition(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryOptionCombinedExecPosition")(data, error_info, request_id, is_last,session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryOtherServerFund(dict data, dict error_info, int request_id, uint64_t session) {
		PyLock lock;

		try {
			this->get_override("onQueryOtherServerFund")(data, error_info, request_id, session);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};
};


BOOST_PYTHON_MODULE(vnxtptrader)
{
	PyEval_InitThreads();	//导入时运行，保证先创建GIL

	class_<TraderApiWrap, boost::noncopyable>("TraderApi")
		.def("createTraderApi", &TraderApiWrap::createTraderApi)
		.def("release", &TraderApiWrap::release)
		.def("exit", &TraderApiWrap::exit)
		.def("getTradingDay", &TraderApiWrap::getTradingDay)
		.def("getApiLastError", &TraderApiWrap::getApiLastError)
		.def("getApiVersion", &TraderApiWrap::getApiVersion)
		.def("getClientIDByXTPID", &TraderApiWrap::getClientIDByXTPID)
		.def("getAccountByXTPID", &TraderApiWrap::getAccountByXTPID)
		.def("subscribePublicTopic", &TraderApiWrap::subscribePublicTopic)
		.def("setSoftwareKey", &TraderApiWrap::setSoftwareKey)
		.def("setSoftwareVersion", &TraderApiWrap::setSoftwareVersion)
		.def("setHeartBeatInterval", &TraderApiWrap::setHeartBeatInterval)
		.def("login", &TraderApiWrap::login)
		.def("logout", &TraderApiWrap::logout)
		.def("insertOrder", &TraderApiWrap::insertOrder)
		.def("cancelOrder", &TraderApiWrap::cancelOrder)
		.def("queryOrderByXTPID", &TraderApiWrap::queryOrderByXTPID)
		.def("queryOrders", &TraderApiWrap::queryOrders)
		.def("queryUnfinishedOrders", &TraderApiWrap::queryUnfinishedOrders)
		.def("queryTradesByXTPID", &TraderApiWrap::queryTradesByXTPID)
		.def("queryTrades", &TraderApiWrap::queryTrades)
		.def("queryPosition", &TraderApiWrap::queryPosition)
		.def("queryAsset", &TraderApiWrap::queryAsset)
		.def("queryStructuredFund", &TraderApiWrap::queryStructuredFund)
		.def("fundTransfer", &TraderApiWrap::fundTransfer)
		.def("queryFundTransfer", &TraderApiWrap::queryFundTransfer)
		.def("queryETF", &TraderApiWrap::queryETF)
		.def("queryETFTickerBasket", &TraderApiWrap::queryETFTickerBasket)
		.def("queryIPOInfoList", &TraderApiWrap::queryIPOInfoList)
		.def("queryIPOQuotaInfo", &TraderApiWrap::queryIPOQuotaInfo)
		.def("queryOptionAuctionInfo", &TraderApiWrap::queryOptionAuctionInfo)
		.def("creditCashRepay", &TraderApiWrap::creditCashRepay)
		.def("queryCreditCashRepayInfo", &TraderApiWrap::queryCreditCashRepayInfo)
		.def("queryCreditFundInfo", &TraderApiWrap::queryCreditFundInfo)
		.def("queryCreditDebtInfo", &TraderApiWrap::queryCreditDebtInfo)
		.def("queryCreditTickerDebtInfo", &TraderApiWrap::queryCreditTickerDebtInfo)
		.def("queryCreditAssetDebtInfo", &TraderApiWrap::queryCreditAssetDebtInfo)
		.def("queryCreditTickerAssignInfo", &TraderApiWrap::queryCreditTickerAssignInfo)
		.def("queryCreditExcessStock", &TraderApiWrap::queryCreditExcessStock)
		.def("queryMulCreditExcessStock", &TraderApiWrap::queryMulCreditExcessStock)

		.def("creditExtendDebtDate", &TraderApiWrap::creditExtendDebtDate)
		.def("queryCreditExtendDebtDateOrders", &TraderApiWrap::queryCreditExtendDebtDateOrders)
		.def("queryCreditFundExtraInfo", &TraderApiWrap::queryCreditFundExtraInfo)
		.def("queryCreditPositionExtraInfo", &TraderApiWrap::queryCreditPositionExtraInfo)

		.def("queryOrdersByPage", &TraderApiWrap::queryOrdersByPage)
		.def("queryTradesByPage", &TraderApiWrap::queryTradesByPage)
		.def("isServerRestart", &TraderApiWrap::isServerRestart)
		.def("creditCashRepayDebtInterestFee", &TraderApiWrap::creditCashRepayDebtInterestFee)
		.def("creditSellStockRepayDebtInterestFee", &TraderApiWrap::creditSellStockRepayDebtInterestFee)
		.def("insertOptionCombinedOrder", &TraderApiWrap::insertOptionCombinedOrder)
		.def("queryOptionCombinedUnfinishedOrders", &TraderApiWrap::queryOptionCombinedUnfinishedOrders)
		.def("queryOptionCombinedOrderByXTPID", &TraderApiWrap::queryOptionCombinedOrderByXTPID)
		.def("queryOptionCombinedOrders", &TraderApiWrap::queryOptionCombinedOrders)
		.def("queryOptionCombinedOrdersByPage", &TraderApiWrap::queryOptionCombinedOrdersByPage)
		.def("queryOptionCombinedTradesByXTPID", &TraderApiWrap::queryOptionCombinedTradesByXTPID)
		.def("queryOptionCombinedTrades", &TraderApiWrap::queryOptionCombinedTrades)
		.def("queryOptionCombinedTradesByPage", &TraderApiWrap::queryOptionCombinedTradesByPage)
		.def("queryOptionCombinedPosition", &TraderApiWrap::queryOptionCombinedPosition)
		.def("queryOptionCombinedStrategyInfo", &TraderApiWrap::queryOptionCombinedStrategyInfo)
		.def("cancelOptionCombinedOrder", &TraderApiWrap::cancelOptionCombinedOrder)
		.def("queryOptionCombinedExecPosition", &TraderApiWrap::queryOptionCombinedExecPosition)
		.def("queryOtherServerFund", &TraderApiWrap::queryOtherServerFund)

		.def("onDisconnected", pure_virtual(&TraderApiWrap::onDisconnected))
		.def("onError", pure_virtual(&TraderApiWrap::onError))
		.def("onOrderEvent", pure_virtual(&TraderApiWrap::onOrderEvent))
		.def("onTradeEvent", pure_virtual(&TraderApiWrap::onTradeEvent))
		.def("onCancelOrderError", pure_virtual(&TraderApiWrap::onCancelOrderError))
		.def("onQueryOrder", pure_virtual(&TraderApiWrap::onQueryOrder))
		.def("onQueryTrade", pure_virtual(&TraderApiWrap::onQueryTrade))
		.def("onQueryPosition", pure_virtual(&TraderApiWrap::onQueryPosition))
		.def("onQueryAsset", pure_virtual(&TraderApiWrap::onQueryAsset))
		.def("onQueryStructuredFund", pure_virtual(&TraderApiWrap::onQueryStructuredFund))
		.def("onQueryFundTransfer", pure_virtual(&TraderApiWrap::onQueryFundTransfer))
		.def("onFundTransfer", pure_virtual(&TraderApiWrap::onFundTransfer))
		.def("onQueryETF", pure_virtual(&TraderApiWrap::onQueryETF))
		.def("onQueryETFBasket", pure_virtual(&TraderApiWrap::onQueryETFBasket))
		.def("onQueryIPOInfoList", pure_virtual(&TraderApiWrap::onQueryIPOInfoList))
		.def("onQueryIPOQuotaInfo", pure_virtual(&TraderApiWrap::onQueryIPOQuotaInfo))
		.def("onQueryOptionAuctionInfo", pure_virtual(&TraderApiWrap::onQueryOptionAuctionInfo))
		.def("onCreditCashRepay", pure_virtual(&TraderApiWrap::onCreditCashRepay))
		.def("onQueryCreditCashRepayInfo", pure_virtual(&TraderApiWrap::onQueryCreditCashRepayInfo))
		.def("onQueryCreditFundInfo", pure_virtual(&TraderApiWrap::onQueryCreditFundInfo))
		.def("onQueryCreditDebtInfo", pure_virtual(&TraderApiWrap::onQueryCreditDebtInfo))
		.def("onQueryCreditTickerDebtInfo", pure_virtual(&TraderApiWrap::onQueryCreditTickerDebtInfo))
		.def("onQueryCreditAssetDebtInfo",pure_virtual(&TraderApiWrap::onQueryCreditAssetDebtInfo))
		.def("onQueryCreditTickerAssignInfo", pure_virtual(&TraderApiWrap::onQueryCreditTickerAssignInfo))
		.def("onQueryCreditExcessStock", pure_virtual(&TraderApiWrap::onQueryCreditExcessStock))
		.def("onQueryMulCreditExcessStock", pure_virtual(&TraderApiWrap::onQueryMulCreditExcessStock))

		.def("onCreditExtendDebtDate", pure_virtual(&TraderApiWrap::onCreditExtendDebtDate))
		.def("onQueryCreditExtendDebtDateOrders",pure_virtual(&TraderApiWrap::onQueryCreditExtendDebtDateOrders))
		.def("onQueryCreditFundExtraInfo", pure_virtual(&TraderApiWrap::onQueryCreditFundExtraInfo))
		.def("onQueryCreditPositionExtraInfo", pure_virtual(&TraderApiWrap::onQueryCreditPositionExtraInfo))
		.def("onQueryOrderByPage", pure_virtual(&TraderApiWrap::onQueryOrderByPage))
		.def("onQueryTradeByPage", pure_virtual(&TraderApiWrap::onQueryTradeByPage))
		.def("onCreditCashRepayDebtInterestFee", pure_virtual(&TraderApiWrap::onCreditCashRepayDebtInterestFee))

		.def("onOptionCombinedOrderEvent", pure_virtual(&TraderApiWrap::onOptionCombinedOrderEvent))
		.def("onOptionCombinedTradeEvent", pure_virtual(&TraderApiWrap::onOptionCombinedTradeEvent))
		.def("onQueryOptionCombinedOrders", pure_virtual(&TraderApiWrap::onQueryOptionCombinedOrders))
		.def("onQueryOptionCombinedOrdersByPage", pure_virtual(&TraderApiWrap::onQueryOptionCombinedOrdersByPage))
		.def("onQueryOptionCombinedTrades", pure_virtual(&TraderApiWrap::onQueryOptionCombinedTrades))
		.def("onQueryOptionCombinedTradesByPage", pure_virtual(&TraderApiWrap::onQueryOptionCombinedTradesByPage))
		.def("onQueryOptionCombinedPosition", pure_virtual(&TraderApiWrap::onQueryOptionCombinedPosition))
		.def("onQueryOptionCombinedStrategyInfo", pure_virtual(&TraderApiWrap::onQueryOptionCombinedStrategyInfo))
		.def("onCancelOptionCombinedOrderError", pure_virtual(&TraderApiWrap::onCancelOptionCombinedOrderError))
		.def("onQueryOptionCombinedExecPosition", pure_virtual(&TraderApiWrap::onQueryOptionCombinedExecPosition))
		.def("onQueryOtherServerFund", pure_virtual(&TraderApiWrap::onQueryOtherServerFund))
		;
};

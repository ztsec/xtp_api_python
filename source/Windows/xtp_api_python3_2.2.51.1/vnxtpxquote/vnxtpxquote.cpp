// vnctpmd.cpp : 定义 DLL 应用程序的导出函数。
//
//#include "stdafx.h"
#include "vnxtpxquote.h"

///-------------------------------------------------------------------------------------
///从Python对象到C++类型转换用的函数
///-------------------------------------------------------------------------------------

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

void getInt64(dict d, string key, int64_t *value)
{
	if (d.has_key(key))		//检查字典中是否存在该键值
	{
		object o = d[key];	//获取该键值
		extract<int64_t> x(o);	//创建提取器
		if (x.check())		//如果可以提取
		{
			*value = x();	//对目标整数指针赋值
		}
	}
};

void getInt16(dict d, string key, int16_t *value)
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
}

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
	string endStr = value;
	return endStr;
}

///-------------------------------------------------------------------------------------
///C++的回调函数将数据保存到队列中
///-------------------------------------------------------------------------------------

void QuoteApi::OnDisconnected(int reason)
{
    Task* task = new Task();
    task->task_name = ONDISCONNECTED;
    task->task_id = reason;
    this->task_queue.push(task);
};

void QuoteApi::OnError(XTPX::API::XTPRI *error_info)
{
    Task* task = new Task();
    task->task_name = ONERROR;

    if (error_info)
    {
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
        *task_error = *error_info;
        task->task_error = task_error;
    }

    this->task_queue.push(task);
};


void QuoteApi::OnSubMarketData(XTPX::API::XTPST *ticker, XTPX::API::XTPRI *error_info, bool is_last)
{
    Task* task = new Task();
    task->task_name = ONSUBMARKETDATA;

    if (ticker)
    {
		XTPX::API::XTPST *task_data = new XTPX::API::XTPST();
        *task_data = *ticker;
        task->task_data = task_data;
    }

    if (error_info)
    {
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
        *task_error = *error_info;
        task->task_error = task_error;
    }

    task->task_last = is_last;
    this->task_queue.push(task);
};

void QuoteApi::OnUnSubMarketData(XTPX::API::XTPST *ticker, XTPX::API::XTPRI *error_info, bool is_last)
{
    Task* task = new Task();
    task->task_name = ONUNSUBMARKETDATA;

    if (ticker)
    {
		XTPX::API::XTPST *task_data = new XTPX::API::XTPST();
        *task_data = *ticker;
        task->task_data = task_data;
    }

    if (error_info)
    {
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
        *task_error = *error_info;
        task->task_error = task_error;
    }

    task->task_last = is_last;
    this->task_queue.push(task);
};

void QuoteApi::OnDepthMarketData(XTPX::API::XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count, int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count)
{
    Task* task = new Task();
    task->task_name = ONDEPTHMARKETDATA;

    if (market_data)
    {
		XTPX::API::XTPMD *task_data = new XTPX::API::XTPMD();
        *task_data = *market_data;
        task->task_data = task_data;
    }

	if (bid1_qty && bid1_count>0)
	{
		int64_t *task_data_one = new int64_t[bid1_count];
		for (int i=0;i<bid1_count;i++)
		{
			task_data_one[i]=bid1_qty[i];
		}
		task->task_data_one = task_data_one;
	}
	task->task_one_counts = bid1_count;
	task->task_one_all_counts = max_bid1_count;
	if (ask1_qty && ask1_count>0)
	{
		int64_t *task_data_two = new int64_t[ask1_count];
		for (int i=0;i<ask1_count;i++)
		{
			task_data_two[i]=ask1_qty[i];
		}
		task->task_data_two = task_data_two;
	}
	task->task_two_counts = ask1_count;
	task->task_two_all_counts =max_ask1_count;

    this->task_queue.push(task);
};

void QuoteApi::OnETFIOPVData(XTPX::API::IOPV *iopv)
{
	Task* task = new Task();
	task->task_name = ONETFIOPVDATA;

	if (iopv)
	{
		XTPX::API::IOPV *task_data = new XTPX::API::IOPV();
		*task_data = *iopv;
		task->task_data = task_data;
	}
	
	this->task_queue.push(task);
};

void QuoteApi::OnSubOrderBook(XTPX::API::XTPST *ticker, XTPX::API::XTPRI *error_info, bool is_last)
{
    Task* task = new Task();
    task->task_name = ONSUBORDERBOOK;

    if (ticker)
    {
		XTPX::API::XTPST *task_data = new XTPX::API::XTPST();
        *task_data = *ticker;
        task->task_data = task_data;
    }

    if (error_info)
    {
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
        *task_error = *error_info;
        task->task_error = task_error;
    }

    task->task_last = is_last;
    this->task_queue.push(task);
};

void QuoteApi::OnUnSubOrderBook(XTPX::API::XTPST *ticker, XTPX::API::XTPRI *error_info, bool is_last)
{
    Task* task = new Task();
    task->task_name = ONUNSUBORDERBOOK;

    if (ticker)
    {
		XTPX::API::XTPST *task_data = new XTPX::API::XTPST();
        *task_data = *ticker;
        task->task_data = task_data;
    }

    if (error_info)
    {
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
        *task_error = *error_info;
        task->task_error = task_error;
    }

    task->task_last = is_last;
    this->task_queue.push(task);
};

void QuoteApi::OnOrderBook(XTPX::API::XTPOB *order_book)
{
    Task* task = new Task();
    task->task_name = ONORDERBOOK;

    if (order_book)
    {
		XTPX::API::XTPOB *task_data = new XTPX::API::XTPOB();
        *task_data = *order_book;
        task->task_data = task_data;
    }
    this->task_queue.push(task);
};

void QuoteApi::OnSubTickByTick(XTPX::API::XTPST *ticker, XTPX::API::XTPRI *error_info, bool is_last)
{
	Task* task = new Task();
	task->task_name = ONSUBTICKBYTICK;

	if (ticker)
	{
		XTPX::API::XTPST *task_data = new XTPX::API::XTPST();
		*task_data = *ticker;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_last = is_last;
	this->task_queue.push(task);
};

void QuoteApi::OnUnSubTickByTick(XTPX::API::XTPST *ticker, XTPX::API::XTPRI *error_info, bool is_last)
{
	Task* task = new Task();
	task->task_name = ONUNSUBTICKBYTICK;

	if (ticker)
	{
		XTPX::API::XTPST *task_data = new XTPX::API::XTPST();
		*task_data = *ticker;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_last = is_last;
	this->task_queue.push(task);
};

void QuoteApi::OnTickByTick(XTPX::API::XTPTBT *tbt_data)
{
	Task* task = new Task();
	task->task_name = ONTICKBYTICK;

	if (tbt_data)
	{
		XTPX::API::XTPTBT *task_data = new XTPX::API::XTPTBT();
		*task_data = *tbt_data;
		task->task_data = task_data;
	}

	this->task_queue.push(task);
};

void QuoteApi::OnSubscribeAllMarketData(XTPX::API::XTP_EXCHANGE_TYPE exchage_id, XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONSUBSCRIBEALLMARKETDATA;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	task->exchange_id = exchage_id;
	this->task_queue.push(task);
};

void QuoteApi::OnUnSubscribeAllMarketData(XTPX::API::XTP_EXCHANGE_TYPE exchage_id, XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONUNSUBSCRIBEALLMARKETDATA;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	task->exchange_id = exchage_id;
	this->task_queue.push(task);
};

void QuoteApi::OnSubscribeAllOrderBook(XTPX::API::XTP_EXCHANGE_TYPE exchage_id, XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONSUBSCRIBEALLORDERBOOK;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	task->exchange_id = exchage_id;
	this->task_queue.push(task);
};


void QuoteApi::OnUnSubscribeAllOrderBook(XTPX::API::XTP_EXCHANGE_TYPE exchage_id, XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONUNSUBSCRIBEALLORDERBOOK;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	task->exchange_id = exchage_id;
	this->task_queue.push(task);
};


void QuoteApi::OnSubscribeAllTickByTick(XTPX::API::XTP_EXCHANGE_TYPE exchage_id, XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONSUBSCRIBEALLTICKBYTICK;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	task->exchange_id = exchage_id;
	this->task_queue.push(task);
};

void QuoteApi::OnUnSubscribeAllTickByTick(XTPX::API::XTP_EXCHANGE_TYPE exchage_id, XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONUNSUBSCRIBEALLTICKBYTICK;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	task->exchange_id = exchage_id;
	this->task_queue.push(task);
};

void QuoteApi::OnQueryAllTickers(XTPX::API::XTPQSI* ticker_info, XTPX::API::XTPRI *error_info, bool is_last)
{
    Task* task = new Task();
    task->task_name = ONQUERYALLTICKERS;

    if (ticker_info)
    {
		XTPX::API::XTPQSI *task_data = new XTPX::API::XTPQSI();
        *task_data = *ticker_info;
        task->task_data = task_data;
    }

    if (error_info)
    {
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
        *task_error = *error_info;
        task->task_error = task_error;
    }

    task->task_last = is_last;
    this->task_queue.push(task);
};

void QuoteApi::OnQueryTickersPriceInfo(XTPX::API::XTPTPI* ticker_info, XTPX::API::XTPRI *error_info, bool is_last)
{
	Task* task = new Task();
	task->task_name = ONQUERYTICKERSPRICEINFO;

	if (ticker_info)
	{
		XTPX::API::XTPTPI *task_data = new XTPX::API::XTPTPI();
		*task_data = *ticker_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_last = is_last;
	this->task_queue.push(task);
};


void QuoteApi::OnQueryAllTickersFullInfo(XTPX::API::XTPQFI * ticker_info, XTPX::API::XTPRI * error_info, bool is_last) {

	Task* task = new Task();
	task->task_name = ONQUERYALLTICKERSFULLINFO;

	if (ticker_info)
	{
		XTPX::API::XTPQFI *task_data = new XTPX::API::XTPQFI();
		*task_data = *ticker_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_last = is_last;
	this->task_queue.push(task);
}

void QuoteApi::OnQueryAllNQTickersFullInfo(XTPX::API::XTPNQFI* ticker_info, XTPX::API::XTPRI *error_info, bool is_last)
{
	Task* task = new Task();
	task->task_name = ONQUERYALLNQTICKERSFULLINFO;

	if (ticker_info)
	{
		XTPX::API::XTPNQFI *task_data = new XTPX::API::XTPNQFI();
		*task_data = *ticker_info;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	task->task_last = is_last;
	this->task_queue.push(task);
}

void QuoteApi::OnXTPQuoteNQFullInfo(XTPX::API::XTPNQFI* ticker_info)
{
	Task* task = new Task();
	task->task_name = ONXTPQUOTENQFULLINFO;

	if (ticker_info)
	{
		XTPX::API::XTPNQFI *task_data = new XTPX::API::XTPNQFI();
		*task_data = *ticker_info;
		task->task_data = task_data;
	}
	this->task_queue.push(task);
}

void QuoteApi::OnQueryTickersLatestMarketData(XTPX::API::XTPMD* market_data, XTPX::API::XTPRI *error_info, bool is_last)
{
	Task* task = new Task();
	task->task_name = ONQUERYTICKERSLATESTMARKETDATA;

	if (market_data)
	{
		XTPX::API::XTPMD *task_data = new XTPX::API::XTPMD();
		*task_data = *market_data;
		task->task_data = task_data;
	}

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	task->task_last = is_last;
	this->task_queue.push(task);
}

void QuoteApi::OnSubscribeAllIndexPress(XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONSUBSCRIBEALLINDEXPRESS;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	this->task_queue.push(task);
}

void QuoteApi::OnUnSubscribeAllIndexPress(XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONUNSUBSCRIBEALLINDEXPRESS;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	this->task_queue.push(task);
}

void QuoteApi::OnIndexPress(XTPX::API::XTPIndexPress *idx)
{
	Task* task = new Task();
	task->task_name = ONINDEXPRESS;

	if (idx)
	{
		XTPX::API::XTPIndexPress *task_data = new XTPX::API::XTPIndexPress();
		*task_data = *idx;
		task->task_data = task_data;
	}
	this->task_queue.push(task);
}

void QuoteApi::OnSubscribeAllHKCMarketData(XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONSUBSCRIBEALLHKCMARKETDATA;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}

	this->task_queue.push(task);
}

void QuoteApi::OnUnSubscribeAllHKCMarketData(XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONUNSUBSCRIBEALLHKCMARKETDATA;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	this->task_queue.push(task);
}

void QuoteApi::OnHKRLData(XTPX::API::XTPHKCRL *hkc_data)
{
	Task* task = new Task();
	task->task_name = ONHKRLDATA;

	if (hkc_data)
	{
		XTPX::API::XTPHKCRL *task_data = new XTPX::API::XTPHKCRL();
		*task_data = *hkc_data;
		task->task_data = task_data;
	}
	this->task_queue.push(task);

}

void QuoteApi::OnHKCMarketData(XTPX::API::XTPHKCMD *hkc_md)
{
	Task* task = new Task();
	task->task_name = ONHKCMARKETDATA;

	if (hkc_md)
	{
		XTPX::API::XTPHKCMD *task_data = new XTPX::API::XTPHKCMD();
		*task_data = *hkc_md;
		task->task_data = task_data;
	}
	this->task_queue.push(task);
}


void QuoteApi::OnRequestRebuildQuote(XTPX::API::XTPQuoteRebuildResultRsp* rebuild_result)
{
	Task* task = new Task();
	task->task_name = ONREQUESTREBUILDQUOTE;
	if (rebuild_result)
	{
		XTPX::API::XTPQuoteRebuildResultRsp* task_data = new XTPX::API::XTPQuoteRebuildResultRsp();
		*task_data = *rebuild_result;
		task->task_data = task_data;
	}
	this->task_queue.push(task);
}

void QuoteApi::OnRebuildTickByTick(XTPX::API::XTPTBT *tbt_data)
{
	Task* task = new Task();
	task->task_name = ONREBUILDTICKBYTICK;

	if (tbt_data)
	{
		XTPX::API::XTPTBT *task_data = new XTPX::API::XTPTBT();
		*task_data = *tbt_data;
		task->task_data = task_data;
	}

	this->task_queue.push(task);
}

void QuoteApi::OnRebuildMarketData(XTPX::API::XTPMD *md_data)
{
	Task* task = new Task();
	task->task_name = ONREBUILDMARKETDATA;
	if (md_data)
	{
		XTPX::API::XTPMD *task_data = new XTPX::API::XTPMD();
		*task_data = *md_data;
		task->task_data = task_data;
	}
	this->task_queue.push(task);
}

void QuoteApi::OnQueryAllHKCInfo(XTPX::API::XTPHKCSI* hkcsi, XTPX::API::XTPRI *error_info, bool is_last)
{
	Task* task = new Task();
	task->task_name = ONQUERYALLHKCINFO;
	if (hkcsi)
	{
		XTPX::API::XTPHKCSI *task_data = new XTPX::API::XTPHKCSI();
		*task_data = *hkcsi;
		task->task_data = task_data;
	}
	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	task->task_last = is_last;
	this->task_queue.push(task);
}

void QuoteApi::OnQueryAllIndexPressInfo(XTPX::API::XTPIPSI* ipsi, XTPX::API::XTPRI *error_info, bool is_last)
{
	Task* task = new Task();
	task->task_name = ONQUERYALLINDEXPRESSINFO;
	if (ipsi)
	{
		XTPX::API::XTPIPSI *task_data = new XTPX::API::XTPIPSI();
		*task_data = *ipsi;
		task->task_data = task_data;
	}
	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	task->task_last = is_last;
	this->task_queue.push(task);
}


void QuoteApi::OnSubscribeAllOptionMarketData(XTPX::API::XTP_EXCHANGE_TYPE exchage_id, XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONSUBSCRIBEALLOPTIONMARKETDATA;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	task->exchange_id = exchage_id;
	this->task_queue.push(task);
};

void QuoteApi::OnUnSubscribeAllOptionMarketData(XTPX::API::XTP_EXCHANGE_TYPE exchage_id, XTPX::API::XTPRI *error_info)
{
	Task* task = new Task();
	task->task_name = ONUNSUBSCRIBEALLOPTIONMARKETDATA;

	if (error_info)
	{
		XTPX::API::XTPRI *task_error = new XTPX::API::XTPRI();
		*task_error = *error_info;
		task->task_error = task_error;
	}
	task->exchange_id = exchage_id;
	this->task_queue.push(task);
};

///-------------------------------------------------------------------------------------
///工作线程从队列中取出数据，转化为python对象后，进行推送
///-------------------------------------------------------------------------------------

void QuoteApi::processTask()
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

			case ONSUBMARKETDATA:
			{
			    this->processSubMarketData(task);
			    break;
			}

			case ONUNSUBMARKETDATA:
			{
			    this->processUnSubMarketData(task);
			    break;
			}

			case ONDEPTHMARKETDATA:
			{
			    this->processDepthMarketData(task);
			    break;
			}

			case ONSUBORDERBOOK:
			{
			    this->processSubOrderBook(task);
			    break;
			}

			case ONUNSUBORDERBOOK:
			{
			    this->processUnSubOrderBook(task);
			    break;
			}

			case ONORDERBOOK:
			{
			    this->processOrderBook(task);
			    break;
			}

			case ONSUBTICKBYTICK:
			{
				this->processSubTickByTick(task);
				break;
			}

			case ONUNSUBTICKBYTICK:
			{
				this->processUnSubTickByTick(task);
				break;
			}

			case ONTICKBYTICK:
			{
				this->processTickByTick(task);
				break;
			}

			case ONSUBSCRIBEALLMARKETDATA:
			{
				this->processSubscribeAllMarketData(task);
				break;
			}

			case ONUNSUBSCRIBEALLMARKETDATA:
			{
				this->processUnSubscribeAllMarketData(task);
				break;
			}

			case ONSUBSCRIBEALLORDERBOOK:
			{
				this->processSubscribeAllOrderBook(task);
				break;
			}

			case ONUNSUBSCRIBEALLORDERBOOK:
			{
				this->processUnSubscribeAllOrderBook(task);
				break;
			}

			case ONSUBSCRIBEALLTICKBYTICK:
			{
				this->processSubscribeAllTickByTick(task);
				break;
			}

			case ONUNSUBSCRIBEALLTICKBYTICK:
			{
				this->processUnSubscribeAllTickByTick(task);
				break;
			}

			case ONQUERYALLTICKERS:
			{
			    this->processQueryAllTickers(task);
			    break;
			}

			case ONQUERYTICKERSPRICEINFO:
			{
				this->processQueryTickersPriceInfo(task);
				break;
			}



			case ONSUBSCRIBEALLOPTIONMARKETDATA:
				{
					this->processSubscribeAllOptionMarketData(task);
					break;
				}
			case ONUNSUBSCRIBEALLOPTIONMARKETDATA:
				{
					this->processUnSubscribeAllOptionMarketData(task);
					break;
				}
			case ONQUERYALLTICKERSFULLINFO: {
				this->processQueryAllTickersFullInfo(task);
				break;
											}
			
			case ONREQUESTREBUILDQUOTE:
			{
				this->processRequestRebuildQuote(task);
				break;
			}
			case ONREBUILDTICKBYTICK:
			{
				this->processRebuildTickByTick(task);
				break;
			}
			case ONREBUILDMARKETDATA:
			{
				this->processRebuildMarketData(task);
				break;
			}
			case ONQUERYALLNQTICKERSFULLINFO: 
			{
				this->processQueryAllNQTickersFullInfo(task);
				break;
			}
			case ONXTPQUOTENQFULLINFO:
			{
				this->processXTPQuoteNQFullInfo(task);
				break;
			}
			case ONQUERYTICKERSLATESTMARKETDATA:
			{
				this->processQueryTickersLatestMarketData(task);
				break;
			}
			case ONSUBSCRIBEALLINDEXPRESS:
			{
				this->processSubscribeAllIndexPress(task);
				break;
			}
			case ONUNSUBSCRIBEALLINDEXPRESS:
			{
				this->processUnSubscribeAllIndexPress(task);
				break;
			}
			case ONINDEXPRESS:
			{
				this->processIndexPress(task);
				break;
			}
			case ONSUBSCRIBEALLHKCMARKETDATA:
			{
				this->processSubscribeAllHKCMarketData(task);
				break;
			}
			case ONUNSUBSCRIBEALLHKCMARKETDATA:
			{
				this->processUnSubscribeAllHKCMarketData(task);
				break;
			}
			case ONHKRLDATA:
			{
				this->processHKRLData(task);
				break;
			}
			case ONHKCMARKETDATA:
			{
				this->processHKCMarketData(task);
				break;
			}

			case ONETFIOPVDATA:
			{
				this->processETFIOPVData(task);
				break;
			}
			case ONQUERYALLHKCINFO:
			{
				this->processQueryAllHKCInfo(task);
				break;
			}
			case ONQUERYALLINDEXPRESSINFO:
			{
				this->processQueryAllIndexPressInfo(task);
				break;
			}
		};
	}
};



void QuoteApi::processDisconnected(Task *task)
{
    PyLock lock;
    this->onDisconnected(task->task_id);
    delete task;
};

void QuoteApi::processError(Task *task)
{
    PyLock lock;
    dict error;
    if (task->task_error)
    {
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*) task->task_error;
        error["error_id"] = task_error->error_id;
        error["error_msg"] = addEndingChar(task_error->error_msg);
        delete task->task_error;
    }

    this->onError(error);
    delete task;
};

void QuoteApi::processSubMarketData(Task *task)
{
    PyLock lock;
    dict data;
    if (task->task_data)
    {
		XTPX::API::XTPST *task_data = (XTPX::API::XTPST*) task->task_data;
        data["exchange_id"] = (int)task_data->exchange_id;
        data["ticker"] = addEndingChar(task_data->ticker);
        delete task->task_data;
    }

    dict error;
    if (task->task_error)
    {
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*) task->task_error;
        error["error_id"] = task_error->error_id;
        error["error_msg"] = addEndingChar(task_error->error_msg);
        delete task->task_error;
    }

    this->onSubMarketData(data, error, task->task_last);
    delete task;
};

void QuoteApi::processUnSubMarketData(Task *task)
{
    PyLock lock;
    dict data;
    if (task->task_data)
    {
		XTPX::API::XTPST *task_data = (XTPX::API::XTPST*) task->task_data;
        data["exchange_id"] = (int)task_data->exchange_id;
        data["ticker"] = addEndingChar(task_data->ticker);
        delete task->task_data;
    }

    dict error;
    if (task->task_error)
    {
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*) task->task_error;
        error["error_id"] = task_error->error_id;
        error["error_msg"] = addEndingChar(task_error->error_msg);
        delete task->task_error;
    }

    this->onUnSubMarketData(data, error, task->task_last);
    delete task;
};

void QuoteApi::processDepthMarketData(Task *task)
{
    PyLock lock;
    dict data;
	if (task->task_data)
	{
		XTPX::API::XTPMD *task_data = (XTPX::API::XTPMD*)task->task_data;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["last_price"] = task_data->last_price;
		data["pre_close_price"] = task_data->pre_close_price;
		data["open_price"] = task_data->open_price;
		data["high_price"] = task_data->high_price;
		data["low_price"] = task_data->low_price;
		data["close_price"] = task_data->close_price;

		data["pre_total_long_positon"] = task_data->pre_total_long_positon;
		data["total_long_positon"] = task_data->total_long_positon;
		data["pre_settl_price"] = task_data->pre_settl_price;
		data["settl_price"] = task_data->settl_price;

		data["upper_limit_price"] = task_data->upper_limit_price;
		data["lower_limit_price"] = task_data->lower_limit_price;
		data["pre_delta"] = task_data->pre_delta;
		data["curr_delta"] = task_data->curr_delta;

		data["data_time"] = task_data->data_time;

		data["qty"] = task_data->qty;
		data["turnover"] = task_data->turnover;
		data["avg_price"] = task_data->avg_price;

		data["trades_count"] = task_data->trades_count;
		char str_ticker_status[9] = { "\0" };
#ifdef _MSC_VER //WIN32
		strncpy(str_ticker_status, task_data->ticker_status, sizeof(task_data->ticker_status));
#elif __GNUC__
		strncpy(str_ticker_status, task_data->ticker_status, sizeof(task_data->ticker_status));
#endif
		data["ticker_status"] = addEndingChar(str_ticker_status);
		data["r1"] = task_data->r1;

		boost::python::list ask;
		boost::python::list bid;
		boost::python::list ask_qty;
		boost::python::list bid_qty;

		for (int i = 0; i < 10; i++)
		{
			ask.append(task_data->ask[i]);
			bid.append(task_data->bid[i]);
			ask_qty.append(task_data->ask_qty[i]);
			bid_qty.append(task_data->bid_qty[i]);
		}

		data["ask"] = ask;
		data["bid"] = bid;
		data["bid_qty"] = bid_qty;
		data["ask_qty"] = ask_qty;

		data["data_type_v2"] = (int)task_data->data_type_v2;
		
		if (task_data->data_type_v2 == XTPX::API::XTP_MARKETDATA_V2_ACTUAL)
		{
			dict stk;
			stk["total_bid_qty"] = task_data->stk.total_bid_qty;
			stk["total_ask_qty"] = task_data->stk.total_ask_qty;
			stk["ma_bid_price"] = task_data->stk.ma_bid_price;
			stk["ma_ask_price"] = task_data->stk.ma_ask_price;
			stk["ma_bond_bid_price"] = task_data->stk.ma_bond_bid_price;
			stk["ma_bond_ask_price"] = task_data->stk.ma_bond_ask_price;
			stk["yield_to_maturity"] = task_data->stk.yield_to_maturity;
			stk["iopv"] = task_data->stk.iopv;
			stk["etf_buy_count"] = task_data->stk.etf_buy_count;
			stk["etf_sell_count"] = task_data->stk.etf_sell_count;
			stk["etf_buy_qty"] = task_data->stk.etf_buy_qty;
			stk["etf_buy_money"] = task_data->stk.etf_buy_money;
			stk["etf_sell_qty"] = task_data->stk.etf_sell_qty;
			stk["etf_sell_money"] = task_data->stk.etf_sell_money;
			stk["total_warrant_exec_qty"] = task_data->stk.total_warrant_exec_qty;
			stk["warrant_lower_price"] = task_data->stk.warrant_lower_price;
			stk["warrant_upper_price"] = task_data->stk.warrant_upper_price;
			stk["cancel_buy_count"] = task_data->stk.cancel_buy_count;
			stk["cancel_sell_count"] = task_data->stk.cancel_sell_count;
			stk["cancel_buy_qty"] = task_data->stk.cancel_buy_qty;
			stk["cancel_sell_qty"] = task_data->stk.cancel_sell_qty;
			stk["cancel_buy_money"] = task_data->stk.cancel_buy_money;
			stk["cancel_sell_money"] = task_data->stk.cancel_sell_money;
			stk["total_buy_count"] = task_data->stk.total_buy_count;
			stk["total_sell_count"] = task_data->stk.total_sell_count;
			stk["duration_after_buy"] = task_data->stk.duration_after_buy;
			stk["duration_after_sell"] = task_data->stk.duration_after_sell;
			stk["num_bid_orders"] = task_data->stk.num_bid_orders;
			stk["num_ask_orders"] = task_data->stk.num_ask_orders;
			stk["pre_iopv"] = task_data->stk.pre_iopv;
			stk["r1"] = task_data->stk.r1;
			stk["r2"] = task_data->stk.r2;
			data["stk"] = stk;
		}
		else if (task_data->data_type_v2 == XTPX::API::XTP_MARKETDATA_V2_OPTION) {
			dict opt;
			opt["auction_price"] = task_data->opt.auction_price;
			opt["auction_qty"] = task_data->opt.auction_qty;
			opt["last_enquiry_time"] = task_data->opt.last_enquiry_time;
			data["opt"] = opt;
		}
		else if (task_data->data_type_v2 == XTPX::API::XTP_MARKETDATA_V2_BOND)
		{
			dict bond;
			bond["total_bid_qty"] = task_data->bond.total_bid_qty;
			bond["total_ask_qty"] = task_data->bond.total_ask_qty;
			bond["ma_bid_price"] = task_data->bond.ma_bid_price;
			bond["ma_ask_price"] = task_data->bond.ma_ask_price;
			bond["ma_bond_bid_price"] = task_data->bond.ma_bond_bid_price;
			bond["ma_bond_ask_price"] = task_data->bond.ma_bond_ask_price;
			bond["yield_to_maturity"] = task_data->bond.yield_to_maturity;
			bond["match_lastpx"] = task_data->bond.match_lastpx;
			bond["ma_bond_price"] = task_data->bond.ma_bond_price;
			bond["match_qty"] = task_data->bond.match_qty;
			bond["match_turnover"] = task_data->bond.match_turnover;
			bond["r4"] = task_data->bond.r4;
			bond["r5"] = task_data->bond.r5;
			bond["r6"] = task_data->bond.r6;
			bond["r7"] = task_data->bond.r7;
			bond["r8"] = task_data->bond.r8;
			bond["cancel_buy_count"] = task_data->bond.cancel_buy_count;
			bond["cancel_sell_count"] = task_data->bond.cancel_sell_count;
			bond["cancel_buy_qty"] = task_data->bond.cancel_buy_qty;
			bond["cancel_sell_qty"] = task_data->bond.cancel_sell_qty;
			bond["cancel_buy_money"] = task_data->bond.cancel_buy_money;
			bond["cancel_sell_money"] = task_data->bond.cancel_sell_money;
			bond["total_buy_count"] = task_data->bond.total_buy_count;
			bond["total_sell_count"] = task_data->bond.total_sell_count;
			bond["duration_after_buy"] = task_data->bond.duration_after_buy;
			bond["duration_after_sell"] = task_data->bond.duration_after_sell;
			bond["num_bid_orders"] = task_data->bond.num_bid_orders;
			bond["num_ask_orders"] = task_data->bond.num_ask_orders;
			bond["instrument_status"] = addEndingChar(task_data->bond.instrument_status);
			data["bond"] = bond;
		}

		//data["r4"] = task_data->r4;

		delete task->task_data;
	}
	
	boost::python::list bid1_qty_list;
	if (task->task_data_one && task->task_one_counts>0)
	{
		for (int i=0;i<task->task_one_counts;i++)
		{
			int64_t *bid1_qty = (int64_t *)task->task_data_one+i;
			bid1_qty_list.append(*bid1_qty);
		}
		delete[] task->task_data_one;
	}
	int  bid1_count= task->task_one_counts;
	int  max_bid1_count= task->task_one_all_counts;

	boost::python::list ask1_qty_list;
	if (task->task_data_two && task->task_two_counts>0)
	{
		for (int i=0;i<task->task_two_counts;i++)
		{
			int64_t *ask1_qty = (int64_t *)task->task_data_two+i;
			ask1_qty_list.append(*ask1_qty);
		}
		delete[] task->task_data_two;
	}
	int  ask1_count= task->task_two_counts;
	int  max_ask1_count= task->task_two_all_counts;
    this->onDepthMarketData(data,bid1_qty_list,bid1_count,max_bid1_count,ask1_qty_list,ask1_count,max_ask1_count);
    delete task;
	
};

void QuoteApi::processETFIOPVData(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPX::API::IOPV *task_data = (XTPX::API::IOPV*)task->task_data;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["data_time"] = task_data->data_time;
		data["iopv"] = task_data->iopv;
		data["unused"] = task_data->unused;
		delete task->task_data;
	}	

	this->onETFIOPVData(data);
	delete task;
};

void QuoteApi::processSubOrderBook(Task *task)
{
    PyLock lock;
    dict data;
    if (task->task_data)
    {
		XTPX::API::XTPST *task_data = (XTPX::API::XTPST*) task->task_data;
        data["exchange_id"] = (int)task_data->exchange_id;
        data["ticker"] = addEndingChar(task_data->ticker);
        delete task->task_data;
    }

    dict error;
    if (task->task_error)
    {
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*) task->task_error;
        error["error_id"] = task_error->error_id;
        error["error_msg"] = addEndingChar(task_error->error_msg);
        delete task->task_error;
    }

    this->onSubOrderBook(data, error, task->task_last);
    delete task;
};

void QuoteApi::processUnSubOrderBook(Task *task)
{
    PyLock lock;
    dict data;
    if (task->task_data)
    {
		XTPX::API::XTPST *task_data = (XTPX::API::XTPST*) task->task_data;
        data["exchange_id"] = (int)task_data->exchange_id;
        data["ticker"] = addEndingChar(task_data->ticker);
        delete task->task_data;
    }

    dict error;
    if (task->task_error)
    {
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*) task->task_error;
        error["error_id"] = task_error->error_id;
        error["error_msg"] = addEndingChar(task_error->error_msg);
        delete task->task_error;
    }

    this->onUnSubOrderBook(data, error, task->task_last);
    delete task;
};

void QuoteApi::processOrderBook(Task *task)
{
    PyLock lock;
    dict data;
    if (task->task_data)
    {
		XTPX::API::XTPOB *task_data = (XTPX::API::XTPOB*) task->task_data;
				data["exchange_id"] = (int)task_data->exchange_id;
				data["ticker"] = addEndingChar(task_data->ticker);
				data["unused"] = task_data->unused;
				data["data_time"] = task_data->data_time;

				data["last_price"] = task_data->last_price;
				data["qty"] = task_data->qty;
				data["turnover"] = task_data->turnover;
				data["trades_count"] = task_data->trades_count;

				boost::python::list ask;
				boost::python::list bid;
				boost::python::list ask_qty;
				boost::python::list bid_qty;

				for (int i = 0; i < 10; i++)
				{
					ask.append(task_data->ask[i]);
					bid.append(task_data->bid[i]);
					ask_qty.append(task_data->ask_qty[i]);
					bid_qty.append(task_data->bid_qty[i]);
				}

				data["ask"] = ask;
				data["bid"] = bid;
				data["bid_qty"] = bid_qty;
				data["ask_qty"] = ask_qty;
        delete task->task_data;
    }

    this->onOrderBook(data);
    delete task;
};

void QuoteApi::processSubTickByTick(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPX::API::XTPST *task_data = (XTPX::API::XTPST*)task->task_data;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onSubTickByTick(data, error, task->task_last);
	delete task;
};

void QuoteApi::processUnSubTickByTick(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPX::API::XTPST *task_data = (XTPX::API::XTPST*)task->task_data;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onUnSubTickByTick(data, error, task->task_last);
	delete task;
};

void QuoteApi::processTickByTick(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPX::API::XTPTBT *task_data = (XTPX::API::XTPTBT*)task->task_data;


		data["exchange_id"] = (int)task_data->exchange_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["seq"] = task_data->seq;
		data["data_time"] = task_data->data_time;
		data["type"] = (int)task_data->type;

		if (task_data->type == XTPX::API::XTP_TBT_ENTRUST)
		{
			dict entrust;
			entrust["channel_no"] = task_data->entrust.channel_no;
			entrust["seq"] = task_data->entrust.seq;
			entrust["price"] = task_data->entrust.price;
			entrust["qty"] = task_data->entrust.qty;
			entrust["side"] = task_data->entrust.side;
			entrust["ord_type"] = task_data->entrust.ord_type;
			entrust["order_no"] = task_data->entrust.order_no;
			entrust["traded_qty"] = task_data->entrust.traded_qty;
			entrust["unused"] = addEndingChar(task_data->entrust.unused);
			data["entrust"] = entrust;
		}
		else if(task_data->type == XTPX::API::XTP_TBT_TRADE)
		{
			dict trade;
			trade["channel_no"] = task_data->trade.channel_no;
			trade["seq"] = task_data->trade.seq;
			trade["price"] = task_data->trade.price;
			trade["qty"] = task_data->trade.qty;
			trade["money"] = task_data->trade.money;
			trade["bid_no"] = task_data->trade.bid_no;
			trade["ask_no"] = task_data->trade.ask_no;
			trade["trade_flag"] = task_data->trade.trade_flag;
			trade["unused"] = addEndingChar(task_data->trade.unused);
			data["trade"] = trade;
		}
		else
		{
			dict state;
			state["channel_no"] = task_data->state.channel_no;
			state["unused1"] = task_data->state.unused1;
			state["seq"] = task_data->state.seq;
			state["flag"] = addEndingChar(task_data->state.flag);
			data["state"] = state;
		}

		delete task->task_data;
	}

	this->onTickByTick(data);
	delete task;
};

void QuoteApi::processSubscribeAllMarketData(Task *task)
{
	PyLock lock;
	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}
	this->onSubscribeAllMarketData(task->exchange_id,error);
	delete task;
};

void QuoteApi::processUnSubscribeAllMarketData(Task *task)
{
	PyLock lock;
	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onUnSubscribeAllMarketData(task->exchange_id,error);
	delete task;
};

void QuoteApi::processSubscribeAllOrderBook(Task *task)
{
	PyLock lock;
	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onSubscribeAllOrderBook(task->exchange_id,error);
	delete task;
};

void QuoteApi::processUnSubscribeAllOrderBook(Task *task)
{
	PyLock lock;
	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onUnSubscribeAllOrderBook(task->exchange_id,error);
	delete task;
};

void QuoteApi::processSubscribeAllTickByTick(Task *task)
{
	PyLock lock;
	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onSubscribeAllTickByTick(task->exchange_id,error);
	delete task;
};

void QuoteApi::processUnSubscribeAllTickByTick(Task *task)
{
	PyLock lock;
	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onUnSubscribeAllTickByTick(task->exchange_id,error);
	delete task;
};

void QuoteApi::processQueryAllTickers(Task *task)
{
    PyLock lock;

		//手动修改
		dict data;
		if (task->task_data)
		{
			XTPX::API::XTPQSI *task_data = (XTPX::API::XTPQSI*)task->task_data;
			data["exchange_id"] = (int)task_data->exchange_id;
			data["ticker"] = addEndingChar(task_data->ticker);
			data["ticker_name"] = addEndingChar(task_data->ticker_name);
			data["ticker_type"] = (int)task_data->ticker_type;
			data["pre_close_price"] = task_data->pre_close_price;
			data["upper_limit_price"] = task_data->upper_limit_price;
			data["lower_limit_price"] = task_data->lower_limit_price;
			data["price_tick"] = task_data->price_tick;
			data["buy_qty_unit"] = task_data->buy_qty_unit;
			data["sell_qty_unit"] = task_data->sell_qty_unit;

			delete task->task_data;
		}

    dict error;
    if (task->task_error)
    {
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*) task->task_error;
        error["error_id"] = task_error->error_id;
        error["error_msg"] = addEndingChar(task_error->error_msg);
        delete task->task_error;
    }

    this->onQueryAllTickers(data, error, task->task_last);
    delete task;
};

void QuoteApi::processQueryTickersPriceInfo(Task *task)
{
	PyLock lock;

	//手动修改
	dict data;
	if (task->task_data)
	{
		XTPX::API::XTPTPI *task_data = (XTPX::API::XTPTPI*)task->task_data;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["last_price"] = task_data->last_price;
		data["unused"] = task_data->unused;

		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryTickersPriceInfo(data, error, task->task_last);
	delete task;
};





void QuoteApi::processSubscribeAllOptionMarketData(Task *task)
{
	PyLock lock;
	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onSubscribeAllOptionMarketData(task->exchange_id,error);
	delete task;
};

void QuoteApi::processUnSubscribeAllOptionMarketData(Task *task)
{
	PyLock lock;
	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onUnSubscribeAllOptionMarketData(task->exchange_id,error);
	delete task;
};


void QuoteApi::processQueryAllTickersFullInfo(Task* task) {
	PyLock lock;

	dict data;
	if (task->task_data)
	{
		XTPX::API::XTPQFI *task_data = (XTPX::API::XTPQFI*)task->task_data;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["ticker_name"] = addEndingChar(task_data->ticker_name);
		data["security_type"] = (int)task_data->security_type;
		data["ticker_qualification_class"] = (int)task_data->ticker_qualification_class;
		data["is_registration"] = task_data->is_registration;
		data["is_VIE"] = task_data->is_VIE;
		data["is_noprofit"] = task_data->is_noprofit;
		data["is_weighted_voting_rights"] = task_data->is_weighted_voting_rights;
		data["is_have_price_limit"] = task_data->is_have_price_limit;
		data["is_inventory"] = task_data->is_inventory;
		data["unused"] = addEndingChar(task_data->unused);
		data["upper_limit_price"] = task_data->upper_limit_price;
		data["lower_limit_price"] = task_data->lower_limit_price;
		data["pre_close_price"] = task_data->pre_close_price;
		data["price_tick"] = task_data->price_tick;
		data["bid_qty_upper_limit"] = task_data->bid_qty_upper_limit;
		data["bid_qty_lower_limit"] = task_data->bid_qty_lower_limit;
		data["bid_qty_unit"] = task_data->bid_qty_unit;
		data["ask_qty_upper_limit"] = task_data->ask_qty_upper_limit;
		data["ask_qty_lower_limit"] = task_data->ask_qty_lower_limit;
		data["ask_qty_unit"] = task_data->ask_qty_unit;
		data["market_bid_qty_upper_limit"] = task_data->market_bid_qty_upper_limit;
		data["market_bid_qty_lower_limit"] = task_data->market_bid_qty_lower_limit;
		data["market_bid_qty_unit"] = task_data->market_bid_qty_unit;
		data["market_ask_qty_upper_limit"] = task_data->market_ask_qty_upper_limit;
		data["market_ask_qty_lower_limit"] = task_data->market_ask_qty_lower_limit;
		data["market_ask_qty_unit"] = task_data->market_ask_qty_unit;
		data["security_status"] = (int)task_data->security_status;
		data["unknown1"] = task_data->unknown1;
		boost::python::list unknown;
		for (int i = 0; i < 3; i++)
		{
			unknown.append(task_data->unknown[i]);
		}
		data["unknown"] = unknown;
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryAllTickersFullInfo(data, error, task->task_last);

	delete task;
}

void QuoteApi::processQueryAllNQTickersFullInfo(Task* task) {
	PyLock lock;

	dict data;
	if (task->task_data)
	{
		XTPX::API::XTPNQFI *task_data = (XTPX::API::XTPNQFI*)task->task_data;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["unused"] = task_data->unused;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["ticker_name"] = addEndingChar(task_data->ticker_name);
		data["security_type"] = (int)task_data->security_type;
		data["ticker_qualification_class"] = (int)task_data->ticker_qualification_class;
		data["ticker_abbr_en"] = addEndingChar(task_data->ticker_abbr_en);
		data["base_ticker"] = addEndingChar(task_data->base_ticker);
		data["industry_type"] = addEndingChar(task_data->industry_type);
		data["currency_type"] = addEndingChar(task_data->currency_type);
		data["trade_unit"] = task_data->trade_unit;
		data["hang_out_date"] = task_data->hang_out_date;
		data["value_date"] = task_data->value_date;
		data["maturity_date"] = task_data->maturity_date;
		data["per_limit_vol"] = task_data->per_limit_vol;
		data["buy_vol_unit"] = task_data->buy_vol_unit;
		data["sell_vol_unit"] = task_data->sell_vol_unit;
		data["mini_declared_vol"] = task_data->mini_declared_vol;
		data["limit_price_attr"] = task_data->limit_price_attr;
		data["market_maker_quantity"] = task_data->market_maker_quantity;
		data["price_gear"] = task_data->price_gear;
		data["first_limit_trans"] = task_data->first_limit_trans;
		data["subsequent_limit_trans"] = task_data->subsequent_limit_trans;
		data["limit_upper_price"] = task_data->limit_upper_price;
		data["limit_lower_price"] = task_data->limit_lower_price;
		data["block_trade_upper"] = task_data->block_trade_upper;
		data["block_trade_lower"] = task_data->block_trade_lower;
		data["convert_into_ration"] = task_data->convert_into_ration;
		data["trade_status"] = (int)task_data->trade_status;
		data["security_level"] = (int)task_data->security_level;
		data["trade_type"] = (int)task_data->trade_type;
		data["suspend_flag"] = (int)task_data->suspend_flag;
		data["ex_dividend_flag"] = (int)task_data->ex_dividend_flag;
		data["layer_type"] = (int)task_data->layer_type;
		data["reserved1"] = task_data->reserved1;		
		data["trade_places"] = addEndingChar(task_data->trade_places);
		data["is_rzbd"] = task_data->is_rzbd;
		data["is_rqbd"] = task_data->is_rqbd;
		data["is_drrz"] = task_data->is_drrz;
		data["is_drrq"] = task_data->is_drrq;
		//data["reserved"] = task_data->reserved;
		
		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}

	this->onQueryAllNQTickersFullInfo(data, error, task->task_last);

	delete task;
}

void QuoteApi::processXTPQuoteNQFullInfo(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPX::API::XTPNQFI *task_data = (XTPX::API::XTPNQFI*)task->task_data;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["unused"] = task_data->unused;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["ticker_name"] = addEndingChar(task_data->ticker_name);
		data["security_type"] = (int)task_data->security_type;
		data["ticker_qualification_class"] = (int)task_data->ticker_qualification_class;
		data["ticker_abbr_en"] = addEndingChar(task_data->ticker_abbr_en);
		data["base_ticker"] = addEndingChar(task_data->base_ticker);
		data["industry_type"] = addEndingChar(task_data->industry_type);
		data["currency_type"] = addEndingChar(task_data->currency_type);
		data["trade_unit"] = task_data->trade_unit;
		data["hang_out_date"] = task_data->hang_out_date;
		data["value_date"] = task_data->value_date;
		data["maturity_date"] = task_data->maturity_date;
		data["per_limit_vol"] = task_data->per_limit_vol;
		data["buy_vol_unit"] = task_data->buy_vol_unit;
		data["sell_vol_unit"] = task_data->sell_vol_unit;
		data["mini_declared_vol"] = task_data->mini_declared_vol;
		data["limit_price_attr"] = task_data->limit_price_attr;
		data["market_maker_quantity"] = task_data->market_maker_quantity;
		data["price_gear"] = task_data->price_gear;
		data["first_limit_trans"] = task_data->first_limit_trans;
		data["subsequent_limit_trans"] = task_data->subsequent_limit_trans;
		data["limit_upper_price"] = task_data->limit_upper_price;
		data["limit_lower_price"] = task_data->limit_lower_price;
		data["block_trade_upper"] = task_data->block_trade_upper;
		data["block_trade_lower"] = task_data->block_trade_lower;
		data["convert_into_ration"] = task_data->convert_into_ration;
		data["trade_status"] = (int)task_data->trade_status;
		data["security_level"] = (int)task_data->security_level;
		data["trade_type"] = (int)task_data->trade_type;
		data["suspend_flag"] = (int)task_data->suspend_flag;
		data["ex_dividend_flag"] = (int)task_data->ex_dividend_flag;
		data["layer_type"] = (int)task_data->layer_type;
		data["reserved1"] = task_data->reserved1;
		data["trade_places"] = addEndingChar(task_data->trade_places);
		data["is_rzbd"] = task_data->is_rzbd;
		data["is_rqbd"] = task_data->is_rqbd;
		data["is_drrz"] = task_data->is_drrz;
		data["is_drrq"] = task_data->is_drrq;
		delete task->task_data;
	}
	this->onXTPQuoteNQFullInfo(data);

	delete task;
}

void QuoteApi::processQueryTickersLatestMarketData(Task *task)
{
	PyLock lock;
	dict data;

	if (task->task_data)
	{
		XTPX::API::XTPMD *task_data = (XTPX::API::XTPMD*)task->task_data;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["last_price"] = task_data->last_price;
		data["pre_close_price"] = task_data->pre_close_price;
		data["open_price"] = task_data->open_price;
		data["high_price"] = task_data->high_price;
		data["low_price"] = task_data->low_price;
		data["close_price"] = task_data->close_price;

		data["pre_total_long_positon"] = task_data->pre_total_long_positon;
		data["total_long_positon"] = task_data->total_long_positon;
		data["pre_settl_price"] = task_data->pre_settl_price;
		data["settl_price"] = task_data->settl_price;

		data["upper_limit_price"] = task_data->upper_limit_price;
		data["lower_limit_price"] = task_data->lower_limit_price;
		data["pre_delta"] = task_data->pre_delta;
		data["curr_delta"] = task_data->curr_delta;

		data["data_time"] = task_data->data_time;

		data["qty"] = task_data->qty;
		data["turnover"] = task_data->turnover;
		data["avg_price"] = task_data->avg_price;

		data["trades_count"] = task_data->trades_count;
		char str_ticker_status[9] = { "\0" };
#ifdef _MSC_VER //WIN32
		strncpy(str_ticker_status, task_data->ticker_status, sizeof(task_data->ticker_status));
#elif __GNUC__
		strncpy(str_ticker_status, task_data->ticker_status, sizeof(task_data->ticker_status));
#endif
		data["ticker_status"] = addEndingChar(str_ticker_status);
		data["r1"] = task_data->r1;

		boost::python::list ask;
		boost::python::list bid;
		boost::python::list ask_qty;
		boost::python::list bid_qty;

		for (int i = 0; i < 10; i++)
		{
			ask.append(task_data->ask[i]);
			bid.append(task_data->bid[i]);
			ask_qty.append(task_data->ask_qty[i]);
			bid_qty.append(task_data->bid_qty[i]);
		}

		data["ask"] = ask;
		data["bid"] = bid;
		data["bid_qty"] = bid_qty;
		data["ask_qty"] = ask_qty;

		data["data_type_v2"] = (int)task_data->data_type_v2;
		if (task_data->data_type_v2 == XTPX::API::XTP_MARKETDATA_V2_ACTUAL) 
		{
			dict stk;
			stk["total_bid_qty"] = task_data->stk.total_bid_qty;
			stk["total_ask_qty"] = task_data->stk.total_ask_qty;
			stk["ma_bid_price"] = task_data->stk.ma_bid_price;
			stk["ma_ask_price"] = task_data->stk.ma_ask_price;
			stk["ma_bond_bid_price"] = task_data->stk.ma_bond_bid_price;
			stk["ma_bond_ask_price"] = task_data->stk.ma_bond_ask_price;
			stk["yield_to_maturity"] = task_data->stk.yield_to_maturity;
			stk["iopv"] = task_data->stk.iopv;
			stk["etf_buy_count"] = task_data->stk.etf_buy_count;
			stk["etf_sell_count"] = task_data->stk.etf_sell_count;
			stk["etf_buy_qty"] = task_data->stk.etf_buy_qty;
			stk["etf_buy_money"] = task_data->stk.etf_buy_money;
			stk["etf_sell_qty"] = task_data->stk.etf_sell_qty;
			stk["etf_sell_money"] = task_data->stk.etf_sell_money;
			stk["total_warrant_exec_qty"] = task_data->stk.total_warrant_exec_qty;
			stk["warrant_lower_price"] = task_data->stk.warrant_lower_price;
			stk["warrant_upper_price"] = task_data->stk.warrant_upper_price;
			stk["cancel_buy_count"] = task_data->stk.cancel_buy_count;
			stk["cancel_sell_count"] = task_data->stk.cancel_sell_count;
			stk["cancel_buy_qty"] = task_data->stk.cancel_buy_qty;
			stk["cancel_sell_qty"] = task_data->stk.cancel_sell_qty;
			stk["cancel_buy_money"] = task_data->stk.cancel_buy_money;
			stk["cancel_sell_money"] = task_data->stk.cancel_sell_money;
			stk["total_buy_count"] = task_data->stk.total_buy_count;
			stk["total_sell_count"] = task_data->stk.total_sell_count;
			stk["duration_after_buy"] = task_data->stk.duration_after_buy;
			stk["duration_after_sell"] = task_data->stk.duration_after_sell;
			stk["num_bid_orders"] = task_data->stk.num_bid_orders;
			stk["num_ask_orders"] = task_data->stk.num_ask_orders;
			stk["pre_iopv"] = task_data->stk.pre_iopv;
			stk["r1"] = task_data->stk.r1;
			stk["r2"] = task_data->stk.r2;
			data["stk"] = stk;
		}
		else if (task_data->data_type_v2 == XTPX::API::XTP_MARKETDATA_V2_OPTION) {
			dict opt;
			opt["auction_price"] = task_data->opt.auction_price;
			opt["auction_qty"] = task_data->opt.auction_qty;
			opt["last_enquiry_time"] = task_data->opt.last_enquiry_time;
			data["opt"] = opt;
		}
		else if (task_data->data_type_v2 == XTPX::API::XTP_MARKETDATA_V2_BOND)
		{
			dict bond;
			bond["total_bid_qty"] = task_data->bond.total_bid_qty;
			bond["total_ask_qty"] = task_data->bond.total_ask_qty;
			bond["ma_bid_price"] = task_data->bond.ma_bid_price;
			bond["ma_ask_price"] = task_data->bond.ma_ask_price;
			bond["ma_bond_bid_price"] = task_data->bond.ma_bond_bid_price;
			bond["ma_bond_ask_price"] = task_data->bond.ma_bond_ask_price;
			bond["yield_to_maturity"] = task_data->bond.yield_to_maturity;
			bond["match_lastpx"] = task_data->bond.match_lastpx;
			bond["ma_bond_price"] = task_data->bond.ma_bond_price;
			bond["match_qty"] = task_data->bond.match_qty;
			bond["match_turnover"] = task_data->bond.match_turnover;
			bond["r4"] = task_data->bond.r4;
			bond["r5"] = task_data->bond.r5;
			bond["r6"] = task_data->bond.r6;
			bond["r7"] = task_data->bond.r7;
			bond["r8"] = task_data->bond.r8;
			bond["cancel_buy_count"] = task_data->bond.cancel_buy_count;
			bond["cancel_sell_count"] = task_data->bond.cancel_sell_count;
			bond["cancel_buy_qty"] = task_data->bond.cancel_buy_qty;
			bond["cancel_sell_qty"] = task_data->bond.cancel_sell_qty;
			bond["cancel_buy_money"] = task_data->bond.cancel_buy_money;
			bond["cancel_sell_money"] = task_data->bond.cancel_sell_money;
			bond["total_buy_count"] = task_data->bond.total_buy_count;
			bond["total_sell_count"] = task_data->bond.total_sell_count;
			bond["duration_after_buy"] = task_data->bond.duration_after_buy;
			bond["duration_after_sell"] = task_data->bond.duration_after_sell;
			bond["num_bid_orders"] = task_data->bond.num_bid_orders;
			bond["num_ask_orders"] = task_data->bond.num_ask_orders;
			bond["instrument_status"] = addEndingChar(task_data->bond.instrument_status);
			data["bond"] = bond;
		}

		//data["r4"] = task_data->r4;

		delete task->task_data;
	}

	dict error;
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}
	this->onQueryTickersLatestMarketData(data, error, task->task_last);
	delete task;
}

void QuoteApi::processSubscribeAllIndexPress(Task *task)
{
	PyLock lock;
	dict error;

	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}
	this->onSubscribeAllIndexPress(error);
	delete task;
}

void QuoteApi::processUnSubscribeAllIndexPress(Task *task)
{
	PyLock lock;
	dict error;

	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}
	this->onUnSubscribeAllIndexPress(error);
	delete task;
}

void QuoteApi::processIndexPress(Task *task)
{
	PyLock lock;
	dict data;

	if (task->task_data)
	{
		XTPX::API::XTPIndexPress *task_data = (XTPX::API::XTPIndexPress*)task->task_data;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["exchange_id"] = (int)task_data->exchange_id;
		data["data_source"] = task_data->data_source;
		data["unused"] = addEndingChar(task_data->unused);
		data["data_time"] = task_data->data_time;
		data["last_price"] = task_data->last_price;
		data["pre_close_price"] = task_data->pre_close_price;
		data["open_price"] = task_data->open_price;
		data["high_price"] = task_data->high_price;
		data["low_price"] = task_data->low_price;
		data["close_price"] = task_data->close_price;
		data["qty"] = task_data->qty;
		data["turnover"] = task_data->turnover;
		data["trades_count"] = task_data->trades_count;
		delete task->task_data;
	}
	this->onIndexPress(data);
	delete task;
}

void QuoteApi::processSubscribeAllHKCMarketData(Task *task)
{
	PyLock lock;
	dict error;

	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}
	this->onSubscribeAllHKCMarketData(error);
	delete task;
}

void QuoteApi::processUnSubscribeAllHKCMarketData(Task *task)
{
	PyLock lock;
	dict error;

	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*)task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}
	this->onUnSubscribeAllHKCMarketData(error);
	delete task;
}

void QuoteApi::processHKRLData(Task *task)
{
	PyLock lock;
	dict data;

	if (task->task_data)
	{
		XTPX::API::XTPHKCRL *task_data = (XTPX::API::XTPHKCRL*)task->task_data;
		data["data_time"] = task_data->data_time;
		data["market_id"] = addEndingChar(task_data->market_id);
		data["market_segment_id"] = addEndingChar(task_data->market_segment_id);
		data["threshold_amount"] = task_data->threshold_amount;
		data["pos_amt"] = task_data->pos_amt;
		data["amount_status"] = task_data->amount_status;
		data["unused"] = addEndingChar(task_data->unused);
		delete task->task_data;
	}
	this->onHKRLData(data);
	delete task;
}

void QuoteApi::processHKCMarketData(Task *task)
{
	PyLock lock;
	dict data;

	if (task->task_data)
	{
		XTPX::API::XTPHKCMD *task_data = (XTPX::API::XTPHKCMD*)task->task_data;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["exchange_id"] = (int)task_data->exchange_id;
		data["unused"] = task_data->unused;
		data["data_time"] = task_data->data_time;
		char str_ticker_status[9] = { "\0" };
#ifdef _MSC_VER //WIN32
		strncpy(str_ticker_status, task_data->ticker_status, sizeof(task_data->ticker_status));
#elif __GNUC__
		strncpy(str_ticker_status, task_data->ticker_status, sizeof(task_data->ticker_status));
#endif
		data["ticker_status"] = addEndingChar(str_ticker_status);
		data["trades_count"] = task_data->trades_count;
		data["qty"] = task_data->qty;
		data["turnover"] = task_data->turnover;
		data["upper_limit_price"] = task_data->upper_limit_price;
		data["lower_limit_price"] = task_data->lower_limit_price;
		data["last_price"] = task_data->last_price;
		data["pre_close_price"] = task_data->pre_close_price;
		data["ref_price"] = task_data->ref_price;
		data["high_price"] = task_data->high_price;
		data["low_price"] = task_data->low_price;
		data["nominal_price"] = task_data->nominal_price;
		data["bid_upper_price"] = task_data->bid_upper_price;
		data["bid_lower_price"] = task_data->bid_lower_price;
		data["ask_upper_price"] = task_data->ask_upper_price;
		data["ask_lower_price"] = task_data->ask_lower_price;
		data["complex_event_start_time"] = task_data->complex_event_start_time;
		data["complex_event_end_time"] = task_data->complex_event_end_time;
		
		boost::python::list ask;
		boost::python::list bid;
		boost::python::list ask_qty;
		boost::python::list bid_qty;

		for (int i = 0; i < 10; i++)
		{
			ask.append(task_data->ask[i]);
			bid.append(task_data->bid[i]);
			ask_qty.append(task_data->ask_qty[i]);
			bid_qty.append(task_data->bid_qty[i]);
		}

		data["ask"] = ask;
		data["bid"] = bid;
		data["bid_qty"] = bid_qty;
		data["ask_qty"] = ask_qty;

		delete task->task_data;
	}
	this->onHKCMarketData(data);
	delete task;
}

void QuoteApi::processRequestRebuildQuote(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPX::API::XTPQuoteRebuildResultRsp* task_data = (XTPX::API::XTPQuoteRebuildResultRsp*)task->task_data;
		data["request_id"] = task_data->request_id;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["size"] = task_data->size;
		data["channel_number"] = task_data->channel_number;
		data["unused"] = addEndingChar(task_data->unused);
		data["begin"] = task_data->begin;
		data["end"] = task_data->end;
		data["result_code"] = (int)task_data->result_code;
		data["msg"] = addEndingChar(task_data->msg);
		delete task->task_data;
	}
	this->onRequestRebuildQuote(data);
	delete task;
}

void QuoteApi::processRebuildTickByTick(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPX::API::XTPTBT *task_data = (XTPX::API::XTPTBT*)task->task_data;


		data["exchange_id"] = (int)task_data->exchange_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["seq"] = task_data->seq;
		data["data_time"] = task_data->data_time;
		data["type"] = (int)task_data->type;

		if (task_data->type == XTPX::API::XTP_TBT_ENTRUST)
		{
			dict entrust;
			entrust["channel_no"] = task_data->entrust.channel_no;
			entrust["seq"] = task_data->entrust.seq;
			entrust["price"] = task_data->entrust.price;
			entrust["qty"] = task_data->entrust.qty;
			entrust["side"] = task_data->entrust.side;
			entrust["ord_type"] = task_data->entrust.ord_type;
			entrust["order_no"] = task_data->entrust.order_no;
			entrust["traded_qty"] = task_data->entrust.traded_qty;
			entrust["unused"] = addEndingChar(task_data->entrust.unused);
			data["entrust"] = entrust;
		}
		else if (task_data->type == XTPX::API::XTP_TBT_TRADE)
		{
			dict trade;
			trade["channel_no"] = task_data->trade.channel_no;
			trade["seq"] = task_data->trade.seq;
			trade["price"] = task_data->trade.price;
			trade["qty"] = task_data->trade.qty;
			trade["money"] = task_data->trade.money;
			trade["bid_no"] = task_data->trade.bid_no;
			trade["ask_no"] = task_data->trade.ask_no;
			trade["trade_flag"] = task_data->trade.trade_flag;
			trade["unused"] = addEndingChar(task_data->trade.unused);
			data["trade"] = trade;
		}
		else
		{
			dict state;
			state["channel_no"] = task_data->state.channel_no;
			state["unused1"] = task_data->state.unused1;
			state["seq"] = task_data->state.seq;
			state["flag"] = addEndingChar(task_data->state.flag);
			data["state"] = state;
		}

		delete task->task_data;
	}

	this->onRebuildTickByTick(data);
	delete task;
}

void QuoteApi::processRebuildMarketData(Task *task)
{
	PyLock lock;
	dict data;
	if (task->task_data)
	{
		XTPX::API::XTPMD *task_data = (XTPX::API::XTPMD*)task->task_data;
		data["exchange_id"] = (int)task_data->exchange_id;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["last_price"] = task_data->last_price;
		data["pre_close_price"] = task_data->pre_close_price;
		data["open_price"] = task_data->open_price;
		data["high_price"] = task_data->high_price;
		data["low_price"] = task_data->low_price;
		data["close_price"] = task_data->close_price;

		data["pre_total_long_positon"] = task_data->pre_total_long_positon;
		data["total_long_positon"] = task_data->total_long_positon;
		data["pre_settl_price"] = task_data->pre_settl_price;
		data["settl_price"] = task_data->settl_price;

		data["upper_limit_price"] = task_data->upper_limit_price;
		data["lower_limit_price"] = task_data->lower_limit_price;
		data["pre_delta"] = task_data->pre_delta;
		data["curr_delta"] = task_data->curr_delta;

		data["data_time"] = task_data->data_time;

		data["qty"] = task_data->qty;
		data["turnover"] = task_data->turnover;
		data["avg_price"] = task_data->avg_price;

		data["trades_count"] = task_data->trades_count;
		char str_ticker_status[9] = { "\0" };
#ifdef _MSC_VER //WIN32
		strncpy(str_ticker_status, task_data->ticker_status, sizeof(task_data->ticker_status));
#elif __GNUC__
		strncpy(str_ticker_status, task_data->ticker_status, sizeof(task_data->ticker_status));
#endif
		data["ticker_status"] = addEndingChar(str_ticker_status);
		data["r1"] = task_data->r1;

		boost::python::list ask;
		boost::python::list bid;
		boost::python::list ask_qty;
		boost::python::list bid_qty;

		for (int i = 0; i < 10; i++)
		{
			ask.append(task_data->ask[i]);
			bid.append(task_data->bid[i]);
			ask_qty.append(task_data->ask_qty[i]);
			bid_qty.append(task_data->bid_qty[i]);
		}

		data["ask"] = ask;
		data["bid"] = bid;
		data["bid_qty"] = bid_qty;
		data["ask_qty"] = ask_qty;

		data["data_type_v2"] = (int)task_data->data_type_v2;
		if (task_data->data_type_v2 == XTPX::API::XTP_MARKETDATA_V2_ACTUAL)
		{
			dict stk;
			stk["total_bid_qty"] = task_data->stk.total_bid_qty;
			stk["total_ask_qty"] = task_data->stk.total_ask_qty;
			stk["ma_bid_price"] = task_data->stk.ma_bid_price;
			stk["ma_ask_price"] = task_data->stk.ma_ask_price;
			stk["ma_bond_bid_price"] = task_data->stk.ma_bond_bid_price;
			stk["ma_bond_ask_price"] = task_data->stk.ma_bond_ask_price;
			stk["yield_to_maturity"] = task_data->stk.yield_to_maturity;
			stk["iopv"] = task_data->stk.iopv;
			stk["etf_buy_count"] = task_data->stk.etf_buy_count;
			stk["etf_sell_count"] = task_data->stk.etf_sell_count;
			stk["etf_buy_qty"] = task_data->stk.etf_buy_qty;
			stk["etf_buy_money"] = task_data->stk.etf_buy_money;
			stk["etf_sell_qty"] = task_data->stk.etf_sell_qty;
			stk["etf_sell_money"] = task_data->stk.etf_sell_money;
			stk["total_warrant_exec_qty"] = task_data->stk.total_warrant_exec_qty;
			stk["warrant_lower_price"] = task_data->stk.warrant_lower_price;
			stk["warrant_upper_price"] = task_data->stk.warrant_upper_price;
			stk["cancel_buy_count"] = task_data->stk.cancel_buy_count;
			stk["cancel_sell_count"] = task_data->stk.cancel_sell_count;
			stk["cancel_buy_qty"] = task_data->stk.cancel_buy_qty;
			stk["cancel_sell_qty"] = task_data->stk.cancel_sell_qty;
			stk["cancel_buy_money"] = task_data->stk.cancel_buy_money;
			stk["cancel_sell_money"] = task_data->stk.cancel_sell_money;
			stk["total_buy_count"] = task_data->stk.total_buy_count;
			stk["total_sell_count"] = task_data->stk.total_sell_count;
			stk["duration_after_buy"] = task_data->stk.duration_after_buy;
			stk["duration_after_sell"] = task_data->stk.duration_after_sell;
			stk["num_bid_orders"] = task_data->stk.num_bid_orders;
			stk["num_ask_orders"] = task_data->stk.num_ask_orders;
			stk["pre_iopv"] = task_data->stk.pre_iopv;
			stk["r1"] = task_data->stk.r1;
			stk["r2"] = task_data->stk.r2;
			data["stk"] = stk;
		}
		else if (task_data->data_type_v2 == XTPX::API::XTP_MARKETDATA_V2_OPTION) {
			dict opt;
			opt["auction_price"] = task_data->opt.auction_price;
			opt["auction_qty"] = task_data->opt.auction_qty;
			opt["last_enquiry_time"] = task_data->opt.last_enquiry_time;
			data["opt"] = opt;
		}
		else if (task_data->data_type_v2 == XTPX::API::XTP_MARKETDATA_V2_BOND)
		{
			dict bond;
			bond["total_bid_qty"] = task_data->bond.total_bid_qty;
			bond["total_ask_qty"] = task_data->bond.total_ask_qty;
			bond["ma_bid_price"] = task_data->bond.ma_bid_price;
			bond["ma_ask_price"] = task_data->bond.ma_ask_price;
			bond["ma_bond_bid_price"] = task_data->bond.ma_bond_bid_price;
			bond["ma_bond_ask_price"] = task_data->bond.ma_bond_ask_price;
			bond["yield_to_maturity"] = task_data->bond.yield_to_maturity;
			bond["match_lastpx"] = task_data->bond.match_lastpx;
			bond["ma_bond_price"] = task_data->bond.ma_bond_price;
			bond["match_qty"] = task_data->bond.match_qty;
			bond["match_turnover"] = task_data->bond.match_turnover;
			bond["r4"] = task_data->bond.r4;
			bond["r5"] = task_data->bond.r5;
			bond["r6"] = task_data->bond.r6;
			bond["r7"] = task_data->bond.r7;
			bond["r8"] = task_data->bond.r8;
			bond["cancel_buy_count"] = task_data->bond.cancel_buy_count;
			bond["cancel_sell_count"] = task_data->bond.cancel_sell_count;
			bond["cancel_buy_qty"] = task_data->bond.cancel_buy_qty;
			bond["cancel_sell_qty"] = task_data->bond.cancel_sell_qty;
			bond["cancel_buy_money"] = task_data->bond.cancel_buy_money;
			bond["cancel_sell_money"] = task_data->bond.cancel_sell_money;
			bond["total_buy_count"] = task_data->bond.total_buy_count;
			bond["total_sell_count"] = task_data->bond.total_sell_count;
			bond["duration_after_buy"] = task_data->bond.duration_after_buy;
			bond["duration_after_sell"] = task_data->bond.duration_after_sell;
			bond["num_bid_orders"] = task_data->bond.num_bid_orders;
			bond["num_ask_orders"] = task_data->bond.num_ask_orders;
			bond["instrument_status"] = addEndingChar(task_data->bond.instrument_status);
			data["bond"] = bond;
		}

		//data["r4"] = task_data->r4;

		delete task->task_data;
	}
	
	this->onRebuildMarketData(data);
	delete task;
}

void QuoteApi::processQueryAllHKCInfo(Task *task)
{
	PyLock lock;
	dict data;
	dict error;
	if (task && task->task_data)
	{
		XTPX::API::XTPHKCSI *task_data = (XTPX::API::XTPHKCSI*)task->task_data;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["ticker_name"] = addEndingChar(task_data->ticker_name);
		data["exchange_id"] = (int)task_data->exchange_id;
		data["unknown"] = addEndingChar(task_data->unknown);
		delete task->task_data;
	}
	if (task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*) task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}
	this->onQueryAllHKCInfo(data, error, task->task_last);
	delete task;
}

void QuoteApi::processQueryAllIndexPressInfo(Task *task)
{
	PyLock lock;
	dict data;
	dict error;

	if (task && task->task_data)
	{
		XTPX::API::XTPIPSI *task_data = (XTPX::API::XTPIPSI*)task->task_data;
		data["ticker"] = addEndingChar(task_data->ticker);
		data["ticker_name"] = addEndingChar(task_data->ticker_name);
		data["market_code"] = (int)task_data->market_code;
		data["unknown"] = addEndingChar(task_data->unknown);
		delete task->task_data;
	}
	if (task && task->task_error)
	{
		XTPX::API::XTPRI *task_error = (XTPX::API::XTPRI*) task->task_error;
		error["error_id"] = task_error->error_id;
		error["error_msg"] = addEndingChar(task_error->error_msg);
		delete task->task_error;
	}
	this->onQueryAllIndexPressInfo(data, error, task->task_last);
	delete task;
}


///-------------------------------------------------------------------------------------
///主动函数
///-------------------------------------------------------------------------------------

void QuoteApi::createQuoteApi(int clientid, string path, int log_level, bool udpseq_output)
{
	this->api = XTPX::API::QuoteApi::CreateQuoteApi(clientid, path.c_str(),(XTPX::API::XTP_LOG_LEVEL)log_level, udpseq_output);
	this->api->RegisterSpi(this);
};

void QuoteApi::release()
{
	this->api->Release();
};

int QuoteApi::exit()
{
	//该函数在原生API里没有，用于安全退出API用，原生的join似乎不太稳定
	this->api->RegisterSpi(NULL);
	this->api->Release();
	this->api = NULL;
	return 1;
};

string QuoteApi::getApiVersion()
{
	string ret ="";
	const char* p = this->api->GetApiVersion();
	if (p == NULL)
		ret = "NULL";
	else
		ret = p;
	return ret;
};

dict QuoteApi::getApiLastError()
{
	XTPX::API::XTPRI *error = this->api->GetApiLastError();
	dict err;
	if(error == NULL)
		return err;

	err["error_id"] = error->error_id;
	err["error_msg"] = error->error_msg;

	return err;
};

void QuoteApi::setHeartBeatInterval(int interval)
{
	this->api->SetHeartBeatInterval(interval);
};

bool QuoteApi::setConfigFile(string filename)
{
	bool ret = this->api->SetConfigFile(filename.c_str());
	return ret;
}

bool QuoteApi::setUDPThreadAffinityArray(boost::python::list cpu_no_array, int count)
{
	int listLength = boost::python::len(cpu_no_array);
	if (listLength <= 0)
		return -1;
	int32_t* cpuNoList = new int32_t[listLength];
	for (int i = 0; i < listLength; i++) 
	{
		dict req = (dict)cpu_no_array[i];
		getInt(req, "cpu_no", &cpuNoList[i]);
	}
	bool ret = this->api->SetUDPThreadAffinityArray(cpuNoList, count);
	delete[] cpuNoList;
	cpuNoList = NULL;
	return ret;
}

int QuoteApi::subscribeMarketData(boost::python::list tickerList,int count, int exchange)
{
	int listLength = boost::python::len(tickerList);
	if(listLength <= 0)
		return -1;
	char **myreqList = new char *[listLength];
	for(int i=0;i<listLength;i++){
		myreqList[i]=new char[256];
		dict req = (dict)tickerList[i];
		getStr(req,"ticker",myreqList[i]);
		//printf("i:%d,myreqList[i]:%s\n",i,myreqList[i]);
	}    
	int i = this->api->SubscribeMarketData(myreqList, count, (XTPX::API::XTP_EXCHANGE_TYPE) exchange);
	for(int i=0;i<listLength;i++){
		delete myreqList[i];
	}    
	delete[] myreqList;
	myreqList = NULL;

	return i;
};

int QuoteApi::unSubscribeMarketData(boost::python::list tickerList,int count, int exchange)
{
	int listLength = boost::python::len(tickerList);
	if(listLength <= 0)
		return -1;
	char **myreqList = new char *[listLength];
	for(int i=0;i<listLength;i++){
		myreqList[i]=new char[256];
		dict req = (dict)tickerList[i];
		getStr(req,"ticker",myreqList[i]);
	}    
	int i = this->api->UnSubscribeMarketData(myreqList, count, (XTPX::API::XTP_EXCHANGE_TYPE) exchange);
	for(int i=0;i<listLength;i++){
		delete myreqList[i];
	}    
	delete[] myreqList;
	myreqList = NULL;

	return i;
};

int QuoteApi::subscribeOrderBook(boost::python::list tickerList,int count, int exchange)
{
	int listLength = boost::python::len(tickerList);
	if(listLength <= 0)
		return -1;
	char **myreqList = new char *[listLength];
	for(int i=0;i<listLength;i++){
		myreqList[i]=new char[256];
		dict req = (dict)tickerList[i];
		getStr(req,"ticker",myreqList[i]);
	}    
	int i = this->api->SubscribeOrderBook(myreqList, count, (XTPX::API::XTP_EXCHANGE_TYPE) exchange);
	for(int i=0;i<listLength;i++){
		delete myreqList[i];
	}    
	delete[] myreqList;
	myreqList = NULL;
	return i;
};

int QuoteApi::unSubscribeOrderBook(boost::python::list tickerList,int count, int exchange)
{
	int listLength = boost::python::len(tickerList);
	if(listLength <= 0)
		return -1;
	char **myreqList = new char *[listLength];
	for(int i=0;i<listLength;i++){
		myreqList[i]=new char[256];
		dict req = (dict)tickerList[i];
		getStr(req,"ticker",myreqList[i]);
	}    
	int i = this->api->UnSubscribeOrderBook(myreqList, count, (XTPX::API::XTP_EXCHANGE_TYPE) exchange);
	for(int i=0;i<listLength;i++){
		delete myreqList[i];
	}    
	delete[] myreqList;
	myreqList = NULL;

	return i;
};

int QuoteApi::subscribeTickByTick(boost::python::list tickerList,int count, int exchange)
{
	int listLength = boost::python::len(tickerList);
	if(listLength <= 0)
		return -1;
	char **myreqList = new char *[listLength];
	for(int i=0;i<listLength;i++){
		myreqList[i]=new char[256];
		dict req = (dict)tickerList[i];
		getStr(req,"ticker",myreqList[i]);
	}    
	int i = this->api->SubscribeTickByTick(myreqList, count, (XTPX::API::XTP_EXCHANGE_TYPE) exchange);
	for(int i=0;i<listLength;i++){
		delete myreqList[i];
	}    
	delete[] myreqList;
	myreqList = NULL;
	return i;
};

int QuoteApi::unSubscribeTickByTick(boost::python::list tickerList,int count, int exchange)
{
	int listLength = boost::python::len(tickerList);
	if(listLength <= 0)
		return -1;
	char **myreqList = new char *[listLength];
	for(int i=0;i<listLength;i++){
		myreqList[i]=new char[256];
		dict req = (dict)tickerList[i];
		getStr(req,"ticker",myreqList[i]);
	}    
	int i = this->api->UnSubscribeTickByTick(myreqList, count, (XTPX::API::XTP_EXCHANGE_TYPE) exchange);
	for(int i=0;i<listLength;i++){
		delete myreqList[i];
	}    
	delete[] myreqList;
	myreqList = NULL;

	return i;
};

int QuoteApi::subscribeAllMarketData(int exchange)
{
	return this->api->SubscribeAllMarketData((XTPX::API::XTP_EXCHANGE_TYPE)exchange);
};

int QuoteApi::unSubscribeAllMarketData(int exchange)
{
	return this->api->UnSubscribeAllMarketData((XTPX::API::XTP_EXCHANGE_TYPE)exchange);
};

int QuoteApi::subscribeAllOrderBook(int exchange)
{
	return this->api->SubscribeAllOrderBook((XTPX::API::XTP_EXCHANGE_TYPE)exchange);
};

int QuoteApi::unSubscribeAllOrderBook(int exchange)
{
	return this->api->UnSubscribeAllOrderBook((XTPX::API::XTP_EXCHANGE_TYPE)exchange);
};

int QuoteApi::subscribeAllTickByTick(int exchange)
{
	return this->api->SubscribeAllTickByTick((XTPX::API::XTP_EXCHANGE_TYPE)exchange);
};

int QuoteApi::unSubscribeAllTickByTick(int exchange)
{
	return this->api->UnSubscribeAllTickByTick((XTPX::API::XTP_EXCHANGE_TYPE)exchange);
};

int QuoteApi::login(string ip, int port, string user, string password, int socktype,string local_ip)
{
	int i = this->api->Login(ip.c_str(), port, user.c_str(), password.c_str(), (XTPX::API::XTP_PROTOCOL_TYPE)socktype,local_ip.c_str());
	return i;
};

int QuoteApi::logout()
{
	int i = this->api->Logout();
	
	return i;
};

int QuoteApi::queryAllTickers(int exchange)
{
	int i = this->api->QueryAllTickers((XTPX::API::XTP_EXCHANGE_TYPE)exchange);
	return i;
};

int QuoteApi::queryTickersPriceInfo(boost::python::list tickerList,int count, int exchange)
{
	int listLength = boost::python::len(tickerList);
	if(listLength <= 0)
		return -1;
	//printf("listLength:%d\n",listLength);
	char **myreqList = new char *[listLength];
	for(int i=0;i<listLength;i++){
		myreqList[i]=new char[256];
		dict req = (dict)tickerList[i];
		getStr(req,"ticker",myreqList[i]);
		//printf("i:%d,myreqList[i]:%s\n",i,myreqList[i]);
	}    
	int i = this->api->QueryTickersPriceInfo(myreqList, count, (XTPX::API::XTP_EXCHANGE_TYPE) exchange);
	//printf("return i:%d\n",i);
	for(int i=0;i<listLength;i++){
		delete myreqList[i];
	}    
	delete[] myreqList;
	myreqList = NULL;
	return i;
}

int QuoteApi::queryAllTickersPriceInfo(int exchange)
{
	int i = this->api->QueryAllTickersPriceInfo((XTPX::API::XTP_EXCHANGE_TYPE) exchange);
	return i;
}

int QuoteApi::queryAllTickersFullInfo(int exchange) {
	int i =this->api->QueryAllTickersFullInfo((XTPX::API::XTP_EXCHANGE_TYPE) exchange);
	return i;
}

int QuoteApi::queryAllNQTickersFullInfo()
 {
	return this->api->QueryAllNQTickersFullInfo();
}

int QuoteApi::subscribeAllOptionMarketData(int exchange)
{
	int i = this->api->SubscribeAllOptionMarketData((XTPX::API::XTP_EXCHANGE_TYPE)exchange);
	return i;
};

int QuoteApi::unSubscribeAllOptionMarketData(int exchange)
{
	int i = this->api->UnSubscribeAllOptionMarketData((XTPX::API::XTP_EXCHANGE_TYPE)exchange);
	return i;
};

int QuoteApi::queryTickersLatestMarketData(boost::python::list tickerList, int count, int exchange)
{
	int listLength = boost::python::len(tickerList);
	if (listLength <= 0)
		return -1;
	//printf("listLength:%d\n",listLength);
	char **myreqList = new char *[listLength];
	for (int i = 0; i<listLength; i++) {
		myreqList[i] = new char[256];
		dict req = (dict)tickerList[i];
		getStr(req, "ticker", myreqList[i]);
		//printf("i:%d,myreqList[i]:%s\n",i,myreqList[i]);
	}
	int i = this->api->QueryTickersLatestMarketData(myreqList, count, (XTPX::API::XTP_EXCHANGE_TYPE)exchange);
	for (int i = 0; i<listLength; i++) {
		delete myreqList[i];
	}
	delete[] myreqList;
	myreqList = NULL;
	return i;
}

int QuoteApi::subscribeAllIndexPress()
{
	int i = this->api->SubscribeAllIndexPress();
	return i;
}

int QuoteApi::unSubscribeAllIndexPress()
{
	int i = this->api->UnSubscribeAllIndexPress();
	return i;
}

int QuoteApi::subscribeAllHKCMarketData()
{
	int i = this->api->SubscribeAllHKCMarketData();
	return i;
}

int QuoteApi::unSubscribeAllHKCMarketData()
{
	int i = this->api->UnSubscribeAllHKCMarketData();
	return i;
}


int QuoteApi::requestRebuildQuote(dict req)
{
	XTPX::API::XTPQuoteRebuildReq myreq = XTPX::API::XTPQuoteRebuildReq();
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "ticker", myreq.ticker);
	getInt(req, "request_id", &myreq.request_id);
	int data_type;
	int exchange_id;
	int channel_number;
	getInt(req, "data_type", &data_type);
	getInt(req, "exchange_id", &exchange_id);
	getInt16(req, "channel_number", &myreq.channel_number);
	getInt64(req, "begin", &myreq.begin);
	getInt64(req, "end", &myreq.end);
	
	myreq.data_type = (XTPX::API::XTP_QUOTE_REBUILD_DATA_TYPE)data_type;
	myreq.exchange_id = (XTPX::API::XTP_EXCHANGE_TYPE)exchange_id;

	int ret = api->RequestRebuildQuote(&myreq);

	return ret;
}

int QuoteApi::queryAllHKCInfo()
{
	int ret = api->QueryAllHKCInfo();
	return ret;
}

int QuoteApi::queryAllIndexPressInfo()
{
	int ret = api->QueryAllIndexPressInfo();
	return ret;
}

///-------------------------------------------------------------------------------------
///Boost.Python封装
///-------------------------------------------------------------------------------------

struct QuoteApiWrap : QuoteApi, wrapper < QuoteApi >
{
	virtual void onDisconnected(int reason)
	{
	    try
	    {
	        this->get_override("onDisconnected")(reason);
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

	virtual void onSubMarketData(dict data, dict error, bool last)
	{
	    try
	    {
	        this->get_override("onSubMarketData")(data, error, last);
	    }
	    catch (error_already_set const &)
	    {
	        PyErr_Print();
	    }
	};

	virtual void onUnSubMarketData(dict data, dict error, bool last)
	{
	    try
	    {
	        this->get_override("onUnSubMarketData")(data, error, last);
	    }
	    catch (error_already_set const &)
	    {
	        PyErr_Print();
	    }
	};

	virtual void onDepthMarketData(dict data,boost::python::list bid1_qty_list,int bid1_count,int max_bid1_count,boost::python::list ask1_qty_list,int ask1_count,int max_ask1_count)
	{
	    try
	    {
	        this->get_override("onDepthMarketData")(data,bid1_qty_list,bid1_count,max_bid1_count,ask1_qty_list,ask1_count,max_ask1_count);
	    }
	    catch (error_already_set const &)
	    {
	        PyErr_Print();
	    }
	};

	virtual void onETFIOPVData(dict data)
	{
		try
		{
			this->get_override("onETFIOPVData")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onSubOrderBook(dict data, dict error, bool last)
	{
	    try
	    {
	        this->get_override("onSubOrderBook")(data, error, last);
	    }
	    catch (error_already_set const &)
	    {
	        PyErr_Print();
	    }
	};

	virtual void onUnSubOrderBook(dict data, dict error, bool last)
	{
	    try
	    {
	        this->get_override("onUnSubOrderBook")(data, error, last);
	    }
	    catch (error_already_set const &)
	    {
	        PyErr_Print();
	    }
	};

	virtual void onOrderBook(dict data)
	{
	    try
	    {
	        this->get_override("onOrderBook")(data);
	    }
	    catch (error_already_set const &)
	    {
	        PyErr_Print();
	    }
	};

	virtual void onSubTickByTick(dict data, dict error, bool last)
	{
		try
		{
			this->get_override("onSubTickByTick")(data, error, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onUnSubTickByTick(dict data, dict error, bool last)
	{
		try
		{
			this->get_override("onUnSubTickByTick")(data, error, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onTickByTick(dict data)
	{
		try
		{
			this->get_override("onTickByTick")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onSubscribeAllMarketData(int exchange_id,dict data)
	{
		try
		{
			this->get_override("onSubscribeAllMarketData")(exchange_id,data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onUnSubscribeAllMarketData(int exchange_id,dict data)
	{
		try
		{
			this->get_override("onUnSubscribeAllMarketData")(exchange_id,data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onSubscribeAllOrderBook(int exchange_id,dict data)
	{
		try
		{
			this->get_override("onSubscribeAllOrderBook")(exchange_id,data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onUnSubscribeAllOrderBook(int exchange_id,dict data)
	{
		try
		{
			this->get_override("onUnSubscribeAllOrderBook")(exchange_id,data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onSubscribeAllTickByTick(int exchange_id,dict data)
	{
		try
		{
			this->get_override("onSubscribeAllTickByTick")(exchange_id,data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onUnSubscribeAllTickByTick(int exchange_id,dict data)
	{
		try
		{
			this->get_override("onUnSubscribeAllTickByTick")(exchange_id,data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onQueryAllTickers(dict data, dict error, bool last)
	{
	    try
	    {
	        this->get_override("onQueryAllTickers")(data, error, last);
	    }
	    catch (error_already_set const &)
	    {
	        PyErr_Print();
	    }
	};

	virtual void onQueryTickersPriceInfo(dict data, dict error, bool last)
	{
		try
		{
			this->get_override("onQueryTickersPriceInfo")(data, error, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};





	virtual void onSubscribeAllOptionMarketData(int exchange_id,dict data)
	{
		try
		{
			this->get_override("onSubscribeAllOptionMarketData")(exchange_id,data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onUnSubscribeAllOptionMarketData(int exchange_id,dict data)
	{
		try
		{
			this->get_override("onUnSubscribeAllOptionMarketData")(exchange_id,data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};


	virtual void onQueryAllTickersFullInfo(dict data, dict error, bool last) {
		PyLock lock;

		try {
			this->get_override("onQueryAllTickersFullInfo")(data, error, last);
		} catch (error_already_set const &) {
			PyErr_Print();
		}
	};

	virtual void onQueryAllNQTickersFullInfo(dict data, dict error, bool last) {
		PyLock lock;

		try {
			this->get_override("onQueryAllNQTickersFullInfo")(data, error, last);
		}
		catch (error_already_set const &) {
			PyErr_Print();
		}
	}

	virtual void onXTPQuoteNQFullInfo(dict data) {
		PyLock lock;

		try {
			this->get_override("onXTPQuoteNQFullInfo")(data);
		}
		catch (error_already_set const &) {
			PyErr_Print();
		}
	}

	virtual void onQueryTickersLatestMarketData(dict data, dict error, bool last) {
		PyLock lock;

		try {
			this->get_override("onQueryTickersLatestMarketData")(data, error, last);
		}
		catch (error_already_set const &) {
			PyErr_Print();
		}
	}

	virtual void onSubscribeAllIndexPress(dict error) {
		PyLock lock;

		try {
			this->get_override("onSubscribeAllIndexPress")(error);
		}
		catch (error_already_set const &) {
			PyErr_Print();
		}
	}

	virtual void onUnSubscribeAllIndexPress(dict error) {
		PyLock lock;

		try {
			this->get_override("onUnSubscribeAllIndexPress")(error);
		}
		catch (error_already_set const &) {
			PyErr_Print();
		}
	}

	virtual void onIndexPress(dict data) {
		PyLock lock;

		try {
			this->get_override("onIndexPress")(data);
		}
		catch (error_already_set const &) {
			PyErr_Print();
		}
	}

	virtual void onSubscribeAllHKCMarketData(dict error) {
		PyLock lock;

		try {
			this->get_override("onSubscribeAllHKCMarketData")(error);
		}
		catch (error_already_set const &) {
			PyErr_Print();
		}
	}

	virtual void onUnSubscribeAllHKCMarketData(dict error) {
		PyLock lock;

		try {
			this->get_override("onUnSubscribeAllHKCMarketData")(error);
		}
		catch (error_already_set const &) {
			PyErr_Print();
		}
	}

	virtual void onHKRLData(dict data) {
		PyLock lock;

		try {
			this->get_override("onHKRLData")(data);
		}
		catch (error_already_set const &) {
			PyErr_Print();
		}
	}

	virtual void onHKCMarketData(dict data) {
		PyLock lock;

		try {
			this->get_override("onHKCMarketData")(data);
		}
		catch (error_already_set const &) {
			PyErr_Print();
		}
	}

	virtual void onRequestRebuildQuote(dict data)
	{
		try
		{
			this->get_override("onRequestRebuildQuote")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	}

	virtual void onRebuildTickByTick(dict data)
	{
		try
		{
			this->get_override("onRebuildTickByTick")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	}

	virtual void onRebuildMarketData(dict data)
	{
		try
		{
			this->get_override("onRebuildMarketData")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	}

	virtual void onQueryAllHKCInfo(dict data, dict error, bool is_last)
	{
		try
		{
			this->get_override("onQueryAllHKCInfo")(data, error, is_last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	}

	virtual void onQueryAllIndexPressInfo(dict data, dict error, bool is_last)
	{
		try
		{
			this->get_override("onQueryAllIndexPressInfo")(data, error, is_last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	}
};




BOOST_PYTHON_MODULE(vnxtpxquote)
{
	//PyEval_InitThreads();	//导入时运行，保证先创建GIL
	Py_Initialize();

	class_<QuoteApiWrap, boost::noncopyable>("QuoteApi")
		.def("createQuoteApi", &QuoteApiWrap::createQuoteApi)
		.def("release", &QuoteApiWrap::release)
		.def("exit", &QuoteApiWrap::exit)
		.def("getApiVersion", &QuoteApiWrap::getApiVersion)
		.def("getApiLastError", &QuoteApiWrap::getApiLastError)
		.def("setHeartBeatInterval", &QuoteApiWrap::setHeartBeatInterval)
		.def("setConfigFile", &QuoteApiWrap::setConfigFile)
		.def("setUDPThreadAffinityArray", &QuoteApiWrap::setUDPThreadAffinityArray)
		.def("subscribeMarketData", &QuoteApiWrap::subscribeMarketData)
		.def("unSubscribeMarketData", &QuoteApiWrap::unSubscribeMarketData)
		.def("subscribeOrderBook", &QuoteApiWrap::subscribeOrderBook)
		.def("unSubscribeOrderBook", &QuoteApiWrap::unSubscribeOrderBook)
		.def("subscribeTickByTick", &QuoteApiWrap::subscribeTickByTick)
		.def("unSubscribeTickByTick", &QuoteApiWrap::unSubscribeTickByTick)
		.def("subscribeAllMarketData", &QuoteApiWrap::subscribeAllMarketData)
		.def("unSubscribeAllMarketData", &QuoteApiWrap::unSubscribeAllMarketData)
		.def("subscribeAllOrderBook", &QuoteApiWrap::subscribeAllOrderBook)
		.def("unSubscribeAllOrderBook", &QuoteApiWrap::unSubscribeAllOrderBook)
		.def("subscribeAllTickByTick", &QuoteApiWrap::subscribeAllTickByTick)
		.def("unSubscribeAllTickByTick", &QuoteApiWrap::unSubscribeAllTickByTick)
		.def("login", &QuoteApiWrap::login)
		.def("logout", &QuoteApiWrap::logout)
		.def("queryAllTickers", &QuoteApiWrap::queryAllTickers)
		.def("queryTickersPriceInfo", &QuoteApiWrap::queryTickersPriceInfo)
		.def("queryAllTickersPriceInfo", &QuoteApiWrap::queryAllTickersPriceInfo)
		.def("queryAllTickersFullInfo", &QuoteApiWrap::queryAllTickersFullInfo)
		.def("queryAllNQTickersFullInfo", &QuoteApiWrap::queryAllNQTickersFullInfo)
		.def("subscribeAllOptionMarketData", &QuoteApiWrap::subscribeAllOptionMarketData)
		.def("unSubscribeAllOptionMarketData", &QuoteApiWrap::unSubscribeAllOptionMarketData)
		.def("queryTickersLatestMarketData", &QuoteApiWrap::queryTickersLatestMarketData)
		.def("subscribeAllIndexPress", &QuoteApiWrap::subscribeAllIndexPress)
		.def("unSubscribeAllIndexPress", &QuoteApiWrap::unSubscribeAllIndexPress)
		.def("subscribeAllHKCMarketData", &QuoteApiWrap::subscribeAllHKCMarketData)
		.def("unSubscribeAllHKCMarketData", &QuoteApiWrap::unSubscribeAllHKCMarketData)
		.def("requestRebuildQuote", &QuoteApiWrap::requestRebuildQuote)
		.def("queryAllHKCInfo", &QuoteApiWrap::queryAllHKCInfo)
		.def("queryAllIndexPressInfo", &QuoteApiWrap::queryAllIndexPressInfo)


		.def("onDisconnected", pure_virtual(&QuoteApiWrap::onDisconnected))
		.def("onError", pure_virtual(&QuoteApiWrap::onError))
		.def("onSubMarketData", pure_virtual(&QuoteApiWrap::onSubMarketData))
		.def("onUnSubMarketData", pure_virtual(&QuoteApiWrap::onUnSubMarketData))
		.def("onDepthMarketData", pure_virtual(&QuoteApiWrap::onDepthMarketData))
		.def("onETFIOPVData", pure_virtual(&QuoteApiWrap::onETFIOPVData))
		.def("onSubOrderBook", pure_virtual(&QuoteApiWrap::onSubOrderBook))
		.def("onUnSubOrderBook", pure_virtual(&QuoteApiWrap::onUnSubOrderBook))
		.def("onOrderBook", pure_virtual(&QuoteApiWrap::onOrderBook))
		.def("onSubTickByTick", pure_virtual(&QuoteApiWrap::onSubTickByTick))
		.def("onUnSubTickByTick", pure_virtual(&QuoteApiWrap::onUnSubTickByTick))
		.def("onTickByTick", pure_virtual(&QuoteApiWrap::onTickByTick))
		.def("onSubscribeAllMarketData", pure_virtual(&QuoteApiWrap::onSubscribeAllMarketData))
		.def("onUnSubscribeAllMarketData", pure_virtual(&QuoteApiWrap::onUnSubscribeAllMarketData))
		.def("onSubscribeAllOrderBook", pure_virtual(&QuoteApiWrap::onSubscribeAllOrderBook))
		.def("onUnSubscribeAllOrderBook", pure_virtual(&QuoteApiWrap::onUnSubscribeAllOrderBook))
		.def("onSubscribeAllTickByTick", pure_virtual(&QuoteApiWrap::onSubscribeAllTickByTick))
		.def("onUnSubscribeAllTickByTick", pure_virtual(&QuoteApiWrap::onUnSubscribeAllTickByTick))
		.def("onQueryAllTickers", pure_virtual(&QuoteApiWrap::onQueryAllTickers))
		.def("onQueryTickersPriceInfo", pure_virtual(&QuoteApiWrap::onQueryTickersPriceInfo))
		.def("onQueryAllTickersFullInfo", &QuoteApiWrap::onQueryAllTickersFullInfo)
		.def("onQueryAllNQTickersFullInfo", &QuoteApiWrap::onQueryAllNQTickersFullInfo)
		.def("onXTPQuoteNQFullInfo", &QuoteApiWrap::onXTPQuoteNQFullInfo)
		.def("onQueryTickersLatestMarketData", &QuoteApiWrap::onQueryTickersLatestMarketData)
		.def("onSubscribeAllIndexPress", &QuoteApiWrap::onSubscribeAllIndexPress)
		.def("onUnSubscribeAllIndexPress", &QuoteApiWrap::onUnSubscribeAllIndexPress)
		.def("onIndexPress", &QuoteApiWrap::onIndexPress)
		.def("onSubscribeAllHKCMarketData", &QuoteApiWrap::onSubscribeAllHKCMarketData)
		.def("onUnSubscribeAllHKCMarketData", &QuoteApiWrap::onUnSubscribeAllHKCMarketData)
		.def("onHKRLData", &QuoteApiWrap::onHKRLData)
		.def("onHKCMarketData", &QuoteApiWrap::onHKCMarketData)
		.def("onRequestRebuildQuote", &QuoteApiWrap::onRequestRebuildQuote)
		.def("onRebuildTickByTick", &QuoteApiWrap::onRebuildTickByTick)
		.def("onRebuildMarketData", &QuoteApiWrap::onRebuildMarketData)
		.def("onQueryAllHKCInfo", &QuoteApiWrap::onQueryAllHKCInfo)
		.def("onQueryAllIndexPressInfo", &QuoteApiWrap::onQueryAllIndexPressInfo)

		.def("onSubscribeAllOptionMarketData", pure_virtual(&QuoteApiWrap::onSubscribeAllOptionMarketData))
		.def("onUnSubscribeAllOptionMarketData", pure_virtual(&QuoteApiWrap::onUnSubscribeAllOptionMarketData))
		;
};

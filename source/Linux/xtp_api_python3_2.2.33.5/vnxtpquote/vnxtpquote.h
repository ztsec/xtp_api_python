//˵������

//API
#include "xtp_quote_api.h"

//ϵͳ
//#ifdef WIN32
//#include "stdafx.h"
//#endif
#include <string>
#include <queue>
#include <vector>

//Boost
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python/module.hpp>	//python��װ
#include <boost/python/def.hpp>		//python��װ
#include <boost/python/dict.hpp>	//python��װ
#include <boost/python/list.hpp>	//python��װ
#include <boost/python/object.hpp>	//python��װ
#include <boost/python.hpp>			//python��װ
#include <boost/thread.hpp>			//������е��̹߳���
#include <boost/bind.hpp>			//������е��̹߳���


//�����ռ�
using namespace std;
using namespace boost::python;
using namespace boost;


//����
#define ONDISCONNECTED 1
#define ONERROR 2
#define ONSUBMARKETDATA 3
#define ONUNSUBMARKETDATA 4
#define ONDEPTHMARKETDATA 5
#define ONSUBORDERBOOK 6
#define ONUNSUBORDERBOOK 7
#define ONORDERBOOK 8
#define ONSUBTICKBYTICK 9
#define ONUNSUBTICKBYTICK 10
#define ONTICKBYTICK 11
#define ONSUBSCRIBEALLMARKETDATA 12
#define ONUNSUBSCRIBEALLMARKETDATA 13
#define ONSUBSCRIBEALLORDERBOOK 14
#define ONUNSUBSCRIBEALLORDERBOOK 15
#define ONSUBSCRIBEALLTICKBYTICK 16
#define ONUNSUBSCRIBEALLTICKBYTICK 17
#define ONQUERYALLTICKERS 18
#define ONQUERYTICKERSPRICEINFO 19

#define ONSUBSCRIBEALLOPTIONMARKETDATA 20
#define ONUNSUBSCRIBEALLOPTIONMARKETDATA 21
#define ONSUBSCRIBEALLOPTIONORDERBOOK 22
#define ONUNSUBSCRIBEALLOPTIONORDERBOOK 23
#define ONSUBSCRIBEALLOPTIONTICKBYTICK 24
#define ONUNSUBSCRIBEALLOPTIONTICKBYTICK 25
#define ONQUERYALLTICKERSFULLINFO 26
#define ONREBUILDQUOTESERVERDISCONNECTED 27
#define ONREQUESTREBUILDQUOTE 28
#define ONREBUILDTICKBYTICK 29
#define ONREBUILDMARKETDATA 30
///-------------------------------------------------------------------------------------
///API�еĲ������
///-------------------------------------------------------------------------------------

//GILȫ�����򻯻�ȡ�ã�
//���ڰ���C++�̻߳��GIL�����Ӷ���ֹpython����
class PyLock
{
private:
	PyGILState_STATE gil_state;

public:
	//��ĳ�����������д����ö���ʱ�����GIL��
	PyLock()
	{
		gil_state = PyGILState_Ensure();
	}

	//��ĳ��������ɺ����ٸö���ʱ�����GIL��
	~PyLock()
	{
		PyGILState_Release(gil_state);
	}
};


//����ṹ��
struct Task
{
	int task_name;		//�ص��������ƶ�Ӧ�ĳ���
	void *task_data;	//���ݽṹ��
	void *task_error;	//����ṹ��
	int task_id;		//����id
	bool task_last;		//�Ƿ�Ϊ��󷵻�
	int exchange_id;    //�����г�
	void *task_data_one;	//���ݽṹ��
	int task_one_counts;
	int task_one_all_counts;
	void *task_data_two;	//���ݽṹ��
	int task_two_counts;
	int task_two_all_counts;

};


///�̰߳�ȫ�Ķ���
template<typename Data>

class ConcurrentQueue
{
private:
	queue<Data> the_queue;								//��׼�����
	mutable boost::mutex the_mutex;							//boost������
	condition_variable the_condition_variable;			//boost��������

public:

	//�����µ�����
	void push(Data const& data)
	{
		boost::mutex::scoped_lock lock(the_mutex);				//��ȡ������
		the_queue.push(data);							//������д�������
		lock.unlock();									//�ͷ���
		the_condition_variable.notify_one();			//֪ͨ���������ȴ����߳�
	}

	//�������Ƿ�Ϊ��
	bool empty() const
	{
		boost::mutex::scoped_lock lock(the_mutex);
		return the_queue.empty();
	}

	//ȡ��
	Data wait_and_pop()
	{
		boost::mutex::scoped_lock lock(the_mutex);

		while (the_queue.empty())						//������Ϊ��ʱ
		{
			the_condition_variable.wait(lock);			//�ȴ���������֪ͨ
		}

		Data popped_value = the_queue.front();			//��ȡ�����е����һ������
		the_queue.pop();								//ɾ��������
		return popped_value;							//���ظ�����
	}

};


//���ֵ��л�ȡĳ����ֵ��Ӧ������������ֵ������ṹ������ֵ��
void getInt(dict d, string key, int* value);

//���ֵ��л�ȡĳ����ֵ��Ӧ�ĸ�����������ֵ������ṹ������ֵ��
void getDouble(dict d, string key, double* value);

//���ֵ��л�ȡĳ����ֵ��Ӧ���ַ�������ֵ������ṹ������ֵ��
void getChar(dict d, string key, char* value);

//���ֵ��л�ȡĳ����ֵ��Ӧ���ַ���������ֵ������ṹ������ֵ��
void getStr(dict d, string key, char* value);

void getInt64(dict d, string key, int64_t *value);

void getInt16(dict d, string key, int16_t *value);


///-------------------------------------------------------------------------------------
///C++ SPI�Ļص���������ʵ��
///-------------------------------------------------------------------------------------

//API�ļ̳�ʵ��
class QuoteApi : public XTP::API::QuoteSpi
{
private:
	XTP::API::QuoteApi* api;			//API����
	thread *task_thread;				//�����߳�ָ�루��python���������ݣ�
	ConcurrentQueue<Task*> task_queue;	//�������

public:
	QuoteApi()
	{
		function0<void> f = boost::bind(&QuoteApi::processTask, this);
		thread t(f);
		this->task_thread = &t;
	};

	~QuoteApi()
	{
	};

	//-------------------------------------------------------------------------------------
	//API�ص�����
	//-------------------------------------------------------------------------------------

	///���ͻ����������̨ͨ�����ӶϿ�ʱ���÷��������á�
	///@param reason ����ԭ��������������Ӧ
	///@remark api�����Զ������������߷���ʱ�����û�����ѡ����������������ڴ˺����е���Login���µ�¼��ע���û����µ�¼����Ҫ���¶�������
	virtual void OnDisconnected(int reason);


	///����Ӧ��
	///@param error_info ����������Ӧ��������ʱ�ľ���Ĵ������ʹ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark �˺���ֻ���ڷ�������������ʱ�Ż���ã�һ�������û�����
	virtual void OnError(XTPRI *error_info);

	///��������Ӧ��
	///@param ticker ��ϸ�ĺ�Լ�������
	///@param error_info ���ĺ�Լ��������ʱ�Ĵ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param is_last �Ƿ�˴ζ��ĵ����һ��Ӧ�𣬵�Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ÿ�����ĵĺ�Լ����Ӧһ������Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last);

	///�˶�����Ӧ��
	///@param ticker ��ϸ�ĺ�Լȡ���������
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param is_last �Ƿ�˴�ȡ�����ĵ����һ��Ӧ�𣬵�Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ÿ��ȡ�����ĵĺ�Լ����Ӧһ��ȡ������Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnUnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last);

	///�������֪ͨ��������һ��һ����
	///@param market_data ��������
	///@param bid1_qty ��һ��������
	///@param bid1_count ��һ���е���Чί�б���
	///@param max_bid1_count ��һ������ί�б���
	///@param ask1_qty ��һ��������
	///@param ask1_count ��һ���е���Чί�б���
	///@param max_ask1_count ��һ������ί�б���
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count, int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count);

	///�������鶩����Ӧ��
	///@param ticker ��ϸ�ĺ�Լ�������
	///@param error_info ���ĺ�Լ��������ʱ�Ĵ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param is_last �Ƿ�˴ζ��ĵ����һ��Ӧ�𣬵�Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ÿ�����ĵĺ�Լ����Ӧһ������Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnSubOrderBook(XTPST *ticker, XTPRI *error_info, bool is_last);

	///�˶����鶩����Ӧ��
	///@param ticker ��ϸ�ĺ�Լȡ���������
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param is_last �Ƿ�˴�ȡ�����ĵ����һ��Ӧ�𣬵�Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ÿ��ȡ�����ĵĺ�Լ����Ӧһ��ȡ������Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnUnSubOrderBook(XTPST *ticker, XTPRI *error_info, bool is_last);

	///���鶩����֪ͨ
	///@param order_book ���鶩�������ݣ���Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnOrderBook(XTPOB *order_book);

	///�����������Ӧ��
	///@param ticker ��ϸ�ĺ�Լ�������
	///@param error_info ���ĺ�Լ��������ʱ�Ĵ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param is_last �Ƿ�˴ζ��ĵ����һ��Ӧ�𣬵�Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ÿ�����ĵĺ�Լ����Ӧһ������Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last);

	///�˶��������Ӧ��
	///@param ticker ��ϸ�ĺ�Լȡ���������
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param is_last �Ƿ�˴�ȡ�����ĵ����һ��Ӧ�𣬵�Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ÿ��ȡ�����ĵĺ�Լ����Ӧһ��ȡ������Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnUnSubTickByTick(XTPST *ticker, XTPRI *error_info, bool is_last);

	///�������֪ͨ
	///@param tbt_data ����������ݣ��������ί�к���ʳɽ�����Ϊ���ýṹ�壬��Ҫ����type�����������ί�л�����ʳɽ�����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnTickByTick(XTPTBT *tbt_data);

	///����ȫ�г��Ĺ�Ʊ����Ӧ��
	///@param exchage_id ��ʾ��ǰȫ���ĵ��г������ΪXTP_EXCHANGE_UNKNOWN����ʾ����ȫ�г���XTP_EXCHANGE_SH��ʾΪ�Ϻ�ȫ�г���XTP_EXCHANGE_SZ��ʾΪ����ȫ�г�
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnSubscribeAllMarketData(XTP_EXCHANGE_TYPE exchage_id, XTPRI *error_info);

	///�˶�ȫ�г�������Ӧ��
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnUnSubscribeAllMarketData(XTP_EXCHANGE_TYPE exchage_id,XTPRI *error_info);

	///����ȫ�г������鶩����Ӧ��
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnSubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchage_id,XTPRI *error_info);

	///�˶�ȫ�г������鶩����Ӧ��
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnUnSubscribeAllOrderBook(XTP_EXCHANGE_TYPE exchage_id,XTPRI *error_info);

	///����ȫ�г����������Ӧ��
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchage_id,XTPRI *error_info);

	///�˶�ȫ�г����������Ӧ��
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnUnSubscribeAllTickByTick(XTP_EXCHANGE_TYPE exchage_id,XTPRI *error_info);


	///��ѯ�ɽ��׺�Լ��Ӧ��
	///@param ticker_info �ɽ��׺�Լ��Ϣ
	///@param error_info ��ѯ�ɽ��׺�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param is_last �Ƿ�˴β�ѯ�ɽ��׺�Լ�����һ��Ӧ�𣬵�Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	virtual void OnQueryAllTickers(XTPQSI* ticker_info, XTPRI *error_info, bool is_last);

	///��ѯ��Լ�����¼۸���ϢӦ��
	///@param ticker_info ��Լ�����¼۸���Ϣ
	///@param error_info ��ѯ��Լ�����¼۸���Ϣʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param is_last �Ƿ�˴β�ѯ�����һ��Ӧ�𣬵�Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	virtual void OnQueryTickersPriceInfo(XTPTPI* ticker_info, XTPRI *error_info, bool is_last);


	///����ȫ�г�����Ȩ����Ӧ��
	///@param exchage_id ��ʾ��ǰȫ���ĵ��г������ΪXTP_EXCHANGE_UNKNOWN����ʾ����ȫ�г���XTP_EXCHANGE_SH��ʾΪ�Ϻ�ȫ�г���XTP_EXCHANGE_SZ��ʾΪ����ȫ�г�
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnSubscribeAllOptionMarketData(XTP_EXCHANGE_TYPE exchage_id, XTPRI *error_info);

	///�˶�ȫ�г�����Ȩ����Ӧ��
	///@param exchage_id ��ʾ��ǰȫ���ĵ��г������ΪXTP_EXCHANGE_UNKNOWN����ʾ����ȫ�г���XTP_EXCHANGE_SH��ʾΪ�Ϻ�ȫ�г���XTP_EXCHANGE_SZ��ʾΪ����ȫ�г�
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnUnSubscribeAllOptionMarketData(XTP_EXCHANGE_TYPE exchage_id, XTPRI *error_info);

	///����ȫ�г�����Ȩ���鶩����Ӧ��
	///@param exchage_id ��ʾ��ǰȫ���ĵ��г������ΪXTP_EXCHANGE_UNKNOWN����ʾ����ȫ�г���XTP_EXCHANGE_SH��ʾΪ�Ϻ�ȫ�г���XTP_EXCHANGE_SZ��ʾΪ����ȫ�г�
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnSubscribeAllOptionOrderBook(XTP_EXCHANGE_TYPE exchage_id, XTPRI *error_info);

	///�˶�ȫ�г�����Ȩ���鶩����Ӧ��
	///@param exchage_id ��ʾ��ǰȫ���ĵ��г������ΪXTP_EXCHANGE_UNKNOWN����ʾ����ȫ�г���XTP_EXCHANGE_SH��ʾΪ�Ϻ�ȫ�г���XTP_EXCHANGE_SZ��ʾΪ����ȫ�г�
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnUnSubscribeAllOptionOrderBook(XTP_EXCHANGE_TYPE exchage_id, XTPRI *error_info);

	///����ȫ�г�����Ȩ�������Ӧ��
	///@param exchage_id ��ʾ��ǰȫ���ĵ��г������ΪXTP_EXCHANGE_UNKNOWN����ʾ����ȫ�г���XTP_EXCHANGE_SH��ʾΪ�Ϻ�ȫ�г���XTP_EXCHANGE_SZ��ʾΪ����ȫ�г�
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnSubscribeAllOptionTickByTick(XTP_EXCHANGE_TYPE exchage_id, XTPRI *error_info);

	///�˶�ȫ�г�����Ȩ�������Ӧ��
	///@param exchage_id ��ʾ��ǰȫ���ĵ��г������ΪXTP_EXCHANGE_UNKNOWN����ʾ����ȫ�г���XTP_EXCHANGE_SH��ʾΪ�Ϻ�ȫ�г���XTP_EXCHANGE_SZ��ʾΪ����ȫ�г�
	///@param error_info ȡ�����ĺ�Լʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ��Ҫ���ٷ���
	virtual void OnUnSubscribeAllOptionTickByTick(XTP_EXCHANGE_TYPE exchage_id, XTPRI *error_info);

	///��ѯ��Լ������̬��Ϣ��Ӧ��
	///@param ticker_info ��Լ������̬��Ϣ
	///@param error_info ��ѯ��Լ������̬��Ϣʱ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param is_last �Ƿ�˴β�ѯ��Լ������̬��Ϣ�����һ��Ӧ�𣬵�Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	virtual void OnQueryAllTickersFullInfo(XTPQFI* ticker_info, XTPRI *error_info, bool is_last);

	///���ͻ�����ز����������ͨ�����ӶϿ�ʱ���÷��������á�
	///@param reason ����ԭ��������������Ӧ
	///@remark api�����Զ������������߷���ʱ�����û�����ѡ������������ز���������������Ϣ������ᶨʱ���ߣ���ע�������Ҫ�ز�����ʱ�ű������ӣ��޻ز�����ʱ�������½��
	virtual void OnRebuildQuoteServerDisconnected(int reason);

	///����ز�ָ��Ƶ������������������Ӧ��
	///@param rebuild_result ���ز�����ʱ�����ã�����ز����ʧ�ܣ���msg������ʾʧ��ԭ��
	///@remark ��Ҫ���ٷ��أ����ڻز����ݷ��ͽ�������ã������������̫�࣬һ�����޷��ز��꣬��ôrebuild_result.result_code = XTP_REBUILD_RET_PARTLY����ʱ��Ҫ���ݻز������������ز���������
	virtual void OnRequestRebuildQuote(XTPQuoteRebuildResultRsp* rebuild_result) ;

	///�ز��������������
	///@param tbt_data �ز��������������
	///@remark ��Ҫ���ٷ��أ��˺���������OnTickByTick����һ���߳��ڣ�����OnRequestRebuildQuote()֮ǰ�ص�
	virtual void OnRebuildTickByTick(XTPTBT *tbt_data);

	///�ز��Ŀ�����������
	///@param md_data �ز��������������
	///@remark ��Ҫ���ٷ��أ��˺���������OnDepthMarketData����һ���߳��ڣ�����OnRequestRebuildQuote()֮ǰ�ص�
	virtual void OnRebuildMarketData(XTPMD *md_data);

	//-------------------------------------------------------------------------------------
	//task������
	//-------------------------------------------------------------------------------------

	void processTask();

	void processDisconnected(Task *task);

	void processError(Task *task);

	void processSubMarketData(Task *task);

	void processUnSubMarketData(Task *task);

	void processDepthMarketData(Task *task);

	void processSubOrderBook(Task *task);

	void processUnSubOrderBook(Task *task);

	void processOrderBook(Task *task);

	void processSubTickByTick(Task *task);

	void processUnSubTickByTick(Task *task);

	void processTickByTick(Task *task);

	void processSubscribeAllMarketData(Task *task);

	void processUnSubscribeAllMarketData(Task *task);

	void processSubscribeAllOrderBook(Task *task);

	void processUnSubscribeAllOrderBook(Task *task);

	void processSubscribeAllTickByTick(Task *task);

	void processUnSubscribeAllTickByTick(Task *task);

	void processQueryAllTickers(Task *task);

	void processQueryTickersPriceInfo(Task *task);

	void processQueryAllTickersFullInfo(Task *task);

	void processRebuildQuoteServerDisconnected(Task *task);

	void processRequestRebuildQuote(Task *task);

	void processRebuildTickByTick(Task *task);

	void processRebuildMarketData(Task *task);


	void processSubscribeAllOptionMarketData(Task *task);

	void processUnSubscribeAllOptionMarketData(Task *task);

	void processSubscribeAllOptionOrderBook(Task *task);

	void processUnSubscribeAllOptionOrderBook(Task *task);

	void processSubscribeAllOptionTickByTick(Task *task);

	void processUnSubscribeAllOptionTickByTick(Task *task);
	//-------------------------------------------------------------------------------------
	//data���ص������������ֵ�
	//error���ص������Ĵ����ֵ�
	//id������id
	//last���Ƿ�Ϊ��󷵻�
	//i������
	//-------------------------------------------------------------------------------------

	virtual void onDisconnected(int reason) {};

	virtual void onError(dict data) {};

	virtual void onSubMarketData(dict data, dict error, bool last) {};

	virtual void onUnSubMarketData(dict data, dict error, bool last) {};

	virtual void onDepthMarketData(dict data,boost::python::list bid1_qty_list,int bid1_count,int max_bid1_count,boost::python::list ask1_qty_list,int ask1_count,int max_ask1_count) {};

	virtual void onSubOrderBook(dict data, dict error, bool last) {};

	virtual void onUnSubOrderBook(dict data, dict error, bool last) {};

	virtual void onOrderBook(dict data) {};

	virtual void onSubTickByTick(dict data, dict error, bool last) {};

	virtual void onUnSubTickByTick(dict data, dict error, bool last) {};

	virtual void onTickByTick(dict data) {};

	virtual void onSubscribeAllMarketData(int exchange_id,dict error) {};

	virtual void onUnSubscribeAllMarketData(int exchange_id,dict error) {};

	virtual void onSubscribeAllOrderBook(int exchange_id,dict error) {};

	virtual void onUnSubscribeAllOrderBook(int exchange_id,dict error) {};

	virtual void onSubscribeAllTickByTick(int exchange_id,dict error) {};

	virtual void onUnSubscribeAllTickByTick(int exchange_id,dict error) {};

	virtual void onQueryAllTickers(dict data, dict error, bool last) {};

	virtual void onQueryTickersPriceInfo(dict data, dict error, bool last) {};

	virtual void onQueryAllTickersFullInfo(dict data, dict error, bool last) {};

	virtual void onRebuildQuoteServerDisconnected(int reason) {};

	virtual void onRequestRebuildQuote(dict data) {};

	virtual void onRebuildTickByTick(dict data) {};

	virtual void onRebuildMarketData(dict data) {};


	virtual void onSubscribeAllOptionMarketData(int exchange_id,dict error) {};

	virtual void onUnSubscribeAllOptionMarketData(int exchange_id,dict error) {};

	virtual void onSubscribeAllOptionOrderBook(int exchange_id,dict error) {};

	virtual void onUnSubscribeAllOptionOrderBook(int exchange_id,dict error) {};

	virtual void onSubscribeAllOptionTickByTick(int exchange_id,dict error) {};

	virtual void onUnSubscribeAllOptionTickByTick(int exchange_id,dict error) {};
	//-------------------------------------------------------------------------------------
	//req:���������������ֵ�
	//-------------------------------------------------------------------------------------

	void createQuoteApi(int clientid, string path, int log_level);

	void release();

	int exit();

	string getTradingDay();

	string getApiVersion();

	dict getApiLastError();

	void setUDPBufferSize(int size);

	void setHeartBeatInterval(int interval);

	void setUDPRecvThreadAffinity(int32_t cpu_no);

	void setUDPRecvThreadAffinityArray(boost::python::list tickerList,int count);

	void setUDPParseThreadAffinity(int32_t cpu_no);

	void setUDPParseThreadAffinityArray(boost::python::list tickerList,int count);

	void setUDPSeqLogOutPutFlag(bool flag);

	int subscribeMarketData(boost::python::list tickerList,int count, int exchange);

	int unSubscribeMarketData(boost::python::list tickerList,int count, int exchange);

	int subscribeOrderBook(boost::python::list tickerList,int count, int exchange);

	int unSubscribeOrderBook(boost::python::list tickerList,int count, int exchange);

	int subscribeTickByTick(boost::python::list tickerList,int count, int exchange);

	int unSubscribeTickByTick(boost::python::list tickerList,int count, int exchange);

	int subscribeAllMarketData(int exchange = 3);

	int unSubscribeAllMarketData(int exchange = 3);

	int subscribeAllOrderBook(int exchange = 3);

	int unSubscribeAllOrderBook(int exchange = 3);

	int subscribeAllTickByTick(int exchange = 3);

	int unSubscribeAllTickByTick(int exchange = 3);

	int login(string ip, int port, string user, string password, int socktype,string local_ip);

	int logout();

	int queryAllTickers(int exchange);

	int queryTickersPriceInfo(boost::python::list tickerList, int count, int exchange);

	int queryAllTickersPriceInfo();

	int queryAllTickersFullInfo(int exchange);


    int subscribeAllOptionMarketData(int exchange = XTP_EXCHANGE_UNKNOWN);

	int unSubscribeAllOptionMarketData(int exchange = XTP_EXCHANGE_UNKNOWN);

	int subscribeAllOptionOrderBook(int exchange = XTP_EXCHANGE_UNKNOWN);

	int unSubscribeAllOptionOrderBook(int exchange = XTP_EXCHANGE_UNKNOWN);

	int subscribeAllOptionTickByTick(int exchange = XTP_EXCHANGE_UNKNOWN);

	int unSubscribeAllOptionTickByTick(int exchange = XTP_EXCHANGE_UNKNOWN);

	int loginToRebuildQuoteServer(string ip, int port, string user, string password, int sock_type, string local_ip);

	int logoutFromRebuildQuoteServer();

	int requestRebuildQuote(dict req);
};

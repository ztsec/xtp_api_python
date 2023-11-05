//˵������

//API
#include "xtp_trader_api.h"

//ϵͳ
//#ifdef WIN32
//#include "stdafx.h"
//#endif
#include <string>
#include <queue>

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
#define ONORDEREVENT 3
#define ONTRADEEVENT 4
#define ONCANCELORDERERROR 5
#define ONQUERYORDER 6
#define ONQUERYTRADE 7
#define ONQUERYPOSITION 8
#define ONQUERYASSET 9
#define ONQUERYSTRUCTUREDFUND 10
#define ONQUERYFUNDTRANSFER 11
#define ONFUNDTRANSFER 12
#define ONQUERYETF 13
#define ONQUERYETFBASKET 14
#define ONQUERYIPOINFOLIST 15
#define ONQUERYIPOQUOTAINFO 16

#define ONQUERYOPTIONAUCTIONINFO 17

#define ONCREDITCASHREPAY 18
#define ONQUERYCREDITCASHREPAYINFO 19
#define ONQUERYCREDITFUNDINFO 20
#define ONQUERYCREDITDEBTINFO 21
#define ONQUERYCREDITTICKERDEBTINFO 22
#define ONQUERYCREDITASSETDEBTINFO 23
#define ONQUERYCREDITTICKERASSIGNINFO 24
#define ONQUERYCREDITEXCESSSTOCK 25

#define ONCREDITEXTENDDEBTDATE 26
#define ONQUERYCREDITEXTENDDEBTDATEORDERS 27
#define ONQUERYCREDITFUNDEXTRAINFO 28
#define ONQUERYCREDITPOSITIONEXTRAINFO 29

#define ONQUERYORDERBYPAGE 30
#define ONQUERYTRADEBYPAGE 31
#define ONCREDITCASHREPAYDEBTINTERESTFEE 32
#define ONQUERYMULCREDITEXCESSSTOCK 33

#define ONOPTIONCOMBINEDORDEREVENT 34
#define ONOPTIONCOMBINEDTRADEEVENT 35
#define ONQUERYOPTIONCOMBINEDORDERS 36
#define ONQUERYOPTIONCOMBINEDORDERSBYPAGE 37
#define ONQUERYOPTIONCOMBINEDTRADES 38
#define ONQUERYOPTIONCOMBINEDTRADESBYPAGE 39
#define ONQUERYOPTIONCOMBINEDPOSITION 40
#define ONQUERYOPTIONCOMBINEDSTRATEGYINFO 41
#define ONCANCELOPTIONCOMBINEDORDERERROR 42

#define ONQUERYOPTIONCOMBINEDEXECPOSITION 43
#define ONQUERYOTHERSERVERFUND 44


#define ONQUERYSTRATEGY 45
#define ONSTRATEGYASTATEREPORT 46
#define ONALGOUSERESTABLISHCHANNEL 47
#define ONINSERTALGOORDER 48
#define ONCANCELALGOORDER 49
#define ONALGODISCONNECTED 50
#define ONALGOCONNECTED 51
#define ONQUERYORDEREX 52
#define ONQUERYORDERBYPAGEEX 53
#define ONQUERYOPTIONCOMBINEDORDERSEX 54
#define ONQUERYOPTIONCOMBINEDORDERSBYPAGEEX 55
#define ONSTRATEGYSYMBOLSTATEREPORT 56
#define ONQUERYACCOUNTTRADEMARKET 57
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
	uint64_t addtional_int;		//���������ֶ�
	double remain_amount;//����double�ֶ�
	int64_t addtional_int_two;		//���������ֶ�
	int64_t addtional_int_three;		//���������ֶ�
	int64_t addtional_int_four;		//���������ֶ�

	int64_t req_count;
	int64_t order_sequence;
	int64_t query_reference;
	int64_t trade_sequence;

	string strategy_param;
	string user;
	int reason;
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


void getNestedDictValue(dict d, string key1, string key2, int *value);

void getNestedDictChar(dict d, string key1, string key2, char *value);

void getNestedDictChar2(dict d, string key1, string key2, string key3, char *value, int index);

void getNestedDictValue2(dict d, string key1, string key2, string key3, int *value, int index);

//���ֵ��л�ȡĳ����ֵ��Ӧ������������ֵ������ṹ������ֵ��
void getInt(dict d, string key, int *value);

void getUint64(dict d, string key, uint64_t *value);

void getUint32(dict d, string key, uint32_t *value);

void getInt64(dict d, string key, int64_t *value);



//���ֵ��л�ȡĳ����ֵ��Ӧ�ĸ�����������ֵ������ṹ������ֵ��
void getDouble(dict d, string key, double* value);


//���ֵ��л�ȡĳ����ֵ��Ӧ���ַ�������ֵ������ṹ������ֵ��
void getChar(dict d, string key, char* value);


//���ֵ��л�ȡĳ����ֵ��Ӧ���ַ���������ֵ������ṹ������ֵ��
void getStr(dict d, string key, char* value);


///-------------------------------------------------------------------------------------
///C++ SPI�Ļص���������ʵ��
///-------------------------------------------------------------------------------------

//API�ļ̳�ʵ��
class TraderApi : public XTP::API::TraderSpi
{
private:
	XTP::API::TraderApi* api;			//API����
	thread *task_thread;				//�����߳�ָ�루��python���������ݣ�
	ConcurrentQueue<Task*> task_queue;	//�������

public:
	TraderApi()
	{
		function0<void> f = boost::bind(&TraderApi::processTask, this);
		thread t(f);
		this->task_thread = &t;
	};

	~TraderApi()
	{
	};

	//-------------------------------------------------------------------------------------
	//API�ص�����
	//-------------------------------------------------------------------------------------

	///���ͻ��˵�ĳ�������뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á�
	///@param reason ����ԭ��������������Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark �û���������logout���µĶ��ߣ����ᴥ���˺�����api�����Զ������������߷���ʱ�����û�����ѡ����������������ڴ˺����е���Login���µ�¼��������session_id����ʱ�û��յ������ݸ�����֮ǰ��������
	virtual void OnDisconnected(uint64_t session_id, int reason) ;

	///����Ӧ��
	///@param error_info ����������Ӧ��������ʱ�ľ���Ĵ������ʹ�����Ϣ,��error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark �˺���ֻ���ڷ�������������ʱ�Ż���ã�һ�������û�����
	virtual void OnError(XTPRI *error_info) ;

	///�����ѯ�û��ڱ��ڵ��Ͽɽ����г�����Ӧ
	///@param trade_location ��ѯ���Ľ����г���Ϣ����λ�������ӵ�λ��ʼ������0λ��ʾ���У������(trade_location&0x01) == 0x01������ɽ��׻��У���1λ��ʾ���У������(trade_location&0x02) == 0x02����ʾ�ɽ������У������0λ�͵�1λ����1����(trade_location&(0x01|0x02)) == 0x03���ͱ�ʾ�ɽ��׻���2���г�
	///@param error_info ��ѯ�ɽ����г���������ʱ�����صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark �˲�ѯֻ����һ�����
	virtual void OnQueryAccountTradeMarket(int trade_location, XTPRI *error_info, int request_id, uint64_t session_id);

	///����֪ͨ
	///@param order_info ������Ӧ������Ϣ���û�����ͨ��order_info.order_xtp_id����������ͨ��GetClientIDByXTPID() == client_id�������Լ��Ķ�����order_info.qty_left�ֶ��ڶ���Ϊδ�ɽ������ɡ�ȫ�ɡ��ϵ�״̬ʱ����ʾ�˶�����û�гɽ����������ڲ�����ȫ��״̬ʱ����ʾ�˶���������������order_info.order_cancel_xtp_idΪ������Ӧ�ĳ���ID����Ϊ0ʱ��ʾ�˵������ɹ�
	///@param error_info �������ܾ����߷�������ʱ�������ʹ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ÿ�ζ���״̬����ʱ�����ᱻ���ã���Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߣ��ڶ���δ�ɽ���ȫ���ɽ���ȫ�����������ֳ������Ѿܾ���Щ״̬ʱ������Ӧ�����ڲ��ֳɽ�����������ɶ����ĳɽ��ر�������ȷ�ϡ����е�¼�˴��û��Ŀͻ��˶����յ����û��Ķ�����Ӧ
	virtual void OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id) ;

	///�ɽ�֪ͨ
	///@param trade_info �ɽ��ر��ľ�����Ϣ���û�����ͨ��trade_info.order_xtp_id����������ͨ��GetClientIDByXTPID() == client_id�������Լ��Ķ����������Ͻ�����exec_id����Ψһ��ʶһ�ʳɽ���������2�ʳɽ��ر�ӵ����ͬ��exec_id���������Ϊ�˱ʽ����Գɽ��ˡ����������exec_id��Ψһ�ģ���ʱ�޴��жϻ��ơ�report_index+market�ֶο������Ψһ��ʶ��ʾ�ɽ��ر���
	///@remark �����гɽ�������ʱ�򣬻ᱻ���ã���Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ����е�¼�˴��û��Ŀͻ��˶����յ����û��ĳɽ��ر�����ض���Ϊ����״̬����Ҫ�û�ͨ���ɽ��ر��ĳɽ�������ȷ����OnOrderEvent()�������Ͳ���״̬��
	virtual void OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id) ;

	///����������Ӧ
	///@param cancel_info ����������Ϣ������������order_cancel_xtp_id�ʹ�������order_xtp_id
	///@param error_info �������ܾ����߷�������ʱ�������ʹ�����Ϣ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߣ���error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ����Ӧֻ���ڳ�����������ʱ���ص�
	virtual void OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id) ;

	///�����ѯ������Ӧ
	///@param order_info ��ѯ����һ������
	///@param error_info ��ѯ����ʱ��������ʱ�����صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ����֧�ַ�ʱ�β�ѯ��һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) ;

	///�����ѯ������Ӧ-�°汾�ӿ�
	///@param order_info ��ѯ����һ��������Ϣ
	///@param error_info ��ѯ����ʱ��������ʱ�����صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ����֧�ַ�ʱ�β�ѯ��һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryOrderEx(XTPOrderInfoEx *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///��ҳ�����ѯ������Ӧ
	///@param order_info ��ѯ����һ������
	///@param req_count ��ҳ������������
	///@param order_sequence ��ҳ����ĵ�ǰ�ر�����
	///@param query_reference ��ǰ������Ϣ����Ӧ�Ĳ�ѯ��������Ҫ��¼�������ڽ�����һ�η�ҳ��ѯ��ʱ����Ҫ�õ�
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��order_sequenceΪ0���������β�ѯû�в鵽�κμ�¼����is_lastΪtrueʱ�����order_sequence����req_count����ô��ʾ���б��������Խ�����һ�η�ҳ��ѯ��������ȣ���ʾ���б����Ѿ���ѯ��ϡ�һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ�
	virtual void OnQueryOrderByPage(XTPQueryOrderRsp *order_info, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id);

	///��ҳ�����ѯ������Ӧ-�°汾�ӿ�
	///@param order_info ��ѯ����һ������
	///@param req_count ��ҳ������������
	///@param order_sequence ��ҳ����ĵ�ǰ�ر�����
	///@param query_reference ��ǰ������Ϣ����Ӧ�Ĳ�ѯ��������Ҫ��¼�������ڽ�����һ�η�ҳ��ѯ��ʱ����Ҫ�õ�
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��order_sequenceΪ0���������β�ѯû�в鵽�κμ�¼����is_lastΪtrueʱ�����order_sequence����req_count����ô��ʾ���б��������Խ�����һ�η�ҳ��ѯ��������ȣ���ʾ���б����Ѿ���ѯ��ϡ�һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ�
	virtual void OnQueryOrderByPageEx(XTPOrderInfoEx *order_info, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id);

	///�����ѯ�ɽ���Ӧ
	///@param trade_info ��ѯ����һ���ɽ��ر�
	///@param error_info ��ѯ�ɽ��ر���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ����֧�ַ�ʱ�β�ѯ��һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) ;

	///��ҳ�����ѯ�ɽ���Ӧ
	///@param trade_info ��ѯ����һ���ɽ���Ϣ
	///@param req_count ��ҳ������������
	///@param trade_sequence ��ҳ����ĵ�ǰ�ر�����
	///@param query_reference ��ǰ������Ϣ����Ӧ�Ĳ�ѯ��������Ҫ��¼�������ڽ�����һ�η�ҳ��ѯ��ʱ����Ҫ�õ�
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��trade_sequenceΪ0���������β�ѯû�в鵽�κμ�¼����is_lastΪtrueʱ�����trade_sequence����req_count����ô��ʾ���лر������Խ�����һ�η�ҳ��ѯ��������ȣ���ʾ���лر��Ѿ���ѯ��ϡ�һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ�
	virtual void OnQueryTradeByPage(XTPQueryTradeRsp *trade_info, int64_t req_count, int64_t trade_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id);

	///�����ѯͶ���ֲ߳���Ӧ
	///@param position ��ѯ����һֻ��Ʊ�ĳֲ����
	///@param error_info ��ѯ�˻��ֲַ�������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark �����û����ܳ��ж����Ʊ��һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) ;

	///�����ѯ�ʽ��˻���Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param asset ��ѯ�����ʽ��˻����
	///@param error_info ��ѯ�ʽ��˻���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) ;


	///�����ѯ�ּ�������Ϣ��Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param fund_info ��ѯ���ķּ��������
	///@param error_info ��ѯ�ּ�����������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryStructuredFund(XTPStructuredFundInfo *fund_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///�����ѯ�ʽ𻮲�������Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param fund_transfer_info ��ѯ�����ʽ��˻����
	///@param error_info ��ѯ�ʽ��˻���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///�ʽ𻮲�֪ͨ
	///@param fund_transfer_info �ʽ𻮲�֪ͨ�ľ�����Ϣ���û�����ͨ��fund_transfer_info.serial_id����������ͨ��GetClientIDByXTPID() == client_id�������Լ��Ķ�����
	///@param error_info �ʽ𻮲��������ܾ����߷�������ʱ�������ʹ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@remark ���ʽ𻮲�������״̬�仯��ʱ�򣬻ᱻ���ã���Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ����е�¼�˴��û��Ŀͻ��˶����յ����û����ʽ𻮲�֪ͨ��
	virtual void OnFundTransfer(XTPFundTransferNotice *fund_transfer_info, XTPRI *error_info, uint64_t session_id);

	///�����ѯETF�嵥�ļ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param etf_info ��ѯ����ETF�嵥�ļ����
	///@param error_info ��ѯETF�嵥�ļ���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryETF(XTPQueryETFBaseRsp *etf_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///�����ѯETF��Ʊ������Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param etf_component_info ��ѯ����ETF��Լ����سɷֹ���Ϣ
	///@param error_info ��ѯETF��Ʊ����������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryETFBasket(XTPQueryETFComponentRsp *etf_component_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///�����ѯ�����¹��깺��Ϣ�б����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param ipo_info ��ѯ���Ľ����¹��깺��һֻ��Ʊ��Ϣ
	///@param error_info ��ѯ�����¹��깺��Ϣ�б�������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryIPOInfoList(XTPQueryIPOTickerRsp *ipo_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///�����ѯ�û��¹��깺�����Ϣ����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param quota_info ��ѯ�����û�ĳ���г��Ľ����¹��깺�����Ϣ
	///@param error_info ���ѯ�û��¹��깺�����Ϣ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryIPOQuotaInfo(XTPQueryIPOQuotaRsp *quota_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);




	///�����ѯ��Ȩ��Լ����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param option_info ��ѯ������Ȩ��Լ���
	///@param error_info ��ѯ��Ȩ��Լ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryOptionAuctionInfo(XTPQueryOptionAuctionInfoRsp *option_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///������ȯҵ�����ֽ�ֱ�ӻ������Ӧ
	///@param cash_repay_info �ֽ�ֱ�ӻ���֪ͨ�ľ�����Ϣ���û�����ͨ��cash_repay_info.xtp_id����������ͨ��GetClientIDByXTPID() == client_id�������Լ��Ķ�����
	///@param error_info �ֽ𻹿������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnCreditCashRepay(XTPCrdCashRepayRsp *cash_repay_info, XTPRI *error_info, uint64_t session_id);

	///������ȯҵ�����ֽ�Ϣ����Ӧ
	///@param cash_repay_info �ֽ�Ϣ֪ͨ�ľ�����Ϣ���û�����ͨ��cash_repay_info.xtp_id����������ͨ��GetClientIDByXTPID() == client_id�������Լ��Ķ�����
	///@param error_info �ֽ�Ϣ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnCreditCashRepayDebtInterestFee(XTPCrdCashRepayDebtInterestFeeRsp *cash_repay_info, XTPRI *error_info, uint64_t session_id);

	///�����ѯ������ȯҵ���е��ֽ�ֱ�ӻ��������Ӧ
	///@param cash_repay_info ��ѯ����ĳһ���ֽ�ֱ�ӻ���֪ͨ�ľ�����Ϣ
	///@param error_info ��ѯ�ֽ�ֱ�ӱ�����������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryCreditCashRepayInfo(XTPCrdCashRepayInfo *cash_repay_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///�����ѯ�����˻�������Ϣ����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param fund_info ��ѯ���������˻�������Ϣ���
	///@param error_info ��ѯ�����˻�������Ϣ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryCreditFundInfo(XTPCrdFundInfo *fund_info, XTPRI *error_info, int request_id, uint64_t session_id) ;

	///�����ѯ�����˻���ծ��Ϣ����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param debt_info ��ѯ���������˻���Լ��ծ���
	///@param error_info ��ѯ�����˻���ծ��Ϣ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryCreditDebtInfo(XTPCrdDebtInfo *debt_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) ;

	///�����ѯ�����˻�ָ��֤ȯ��ծδ����Ϣ��Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param debt_info ��ѯ���������˻�ָ��֤ȯ��ծδ����Ϣ���
	///@param error_info ��ѯ�����˻�ָ��֤ȯ��ծδ����Ϣ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryCreditTickerDebtInfo(XTPCrdDebtStockInfo *debt_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///�����ѯ�����˻������ʽ����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param remain_amount ��ѯ���������˻������ʽ�
	///@param error_info ��ѯ�����˻������ʽ�������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryCreditAssetDebtInfo(double remain_amount, XTPRI *error_info, int request_id, uint64_t session_id);

	///�����ѯ�����˻�����ȯͷ����Ϣ����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param assign_info ��ѯ���������˻�����ȯͷ����Ϣ
	///@param error_info ��ѯ�����˻�����ȯͷ����Ϣ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryCreditTickerAssignInfo(XTPClientQueryCrdPositionStkInfo *assign_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///������ȯҵ���������ѯָ����ȯ��Ϣ����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param stock_info ��ѯ������ȯ��Ϣ
	///@param error_info ��ѯ�����˻���ȯ��Ϣ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryCreditExcessStock(XTPClientQueryCrdSurplusStkRspInfo* stock_info, XTPRI *error_info, int request_id, uint64_t session_id);

	///������ȯҵ���������ѯ��ȯ��Ϣ����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param stock_info ��ѯ������ȯ��Ϣ
	///@param error_info ��ѯ�����˻���ȯ��Ϣ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryMulCreditExcessStock(XTPClientQueryCrdSurplusStkRspInfo* stock_info, XTPRI *error_info, int request_id, uint64_t session_id, bool is_last);

	///������ȯҵ���и�ծ��Լչ�ڵ�֪ͨ
	///@param debt_extend_info ��ծ��Լչ��֪ͨ�ľ�����Ϣ���û�����ͨ��debt_extend_info.xtpid����������ͨ��GetClientIDByXTPID() == client_id�������Լ��Ķ�����
	///@param error_info ��ծ��Լչ�ڶ������ܾ����߷�������ʱ�������ʹ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ����ծ��Լչ�ڶ�����״̬�仯��ʱ�򣬻ᱻ���ã���Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ����е�¼�˴��û��Ŀͻ��˶����յ����û��ĸ�ծ��Լչ��֪ͨ��
	virtual void OnCreditExtendDebtDate(XTPCreditDebtExtendNotice *debt_extend_info, XTPRI *error_info, uint64_t session_id);

	///��ѯ������ȯҵ���и�ծ��Լչ�ڶ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param debt_extend_info ��ѯ���ĸ�ծ��Լչ�����
	///@param error_info ��ѯ��ծ��Լչ�ڷ�������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д��󡣵�error_info.error_id=11000350ʱ������û�м�¼����Ϊ������0ֵʱ��������Լ�����ܵ�ʱ�Ĵ���ԭ��
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryCreditExtendDebtDateOrders(XTPCreditDebtExtendNotice *debt_extend_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///��ѯ������ȯҵ���������˻�������Ϣ����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param fund_info �����˻�������Ϣ
	///@param error_info ��ѯ�����˻�������Ϣ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryCreditFundExtraInfo(XTPCrdFundExtraInfo *fund_info, XTPRI *error_info, int request_id, uint64_t session_id);

	///��ѯ������ȯҵ���������˻�ָ��֤ȯ�ĸ�����Ϣ����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param fund_info �����˻�ָ��֤ȯ�ĸ�����Ϣ
	///@param error_info ��ѯ�����˻�������Ϣ��������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryCreditPositionExtraInfo(XTPCrdPositionExtraInfo *fund_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///��Ȩ��ϲ��Ա���֪ͨ
	///@param order_info ������Ӧ������Ϣ���û�����ͨ��order_info.order_xtp_id����������ͨ��GetClientIDByXTPID() == client_id�������Լ��Ķ�����order_info.qty_left�ֶ��ڶ���Ϊδ�ɽ������ɡ�ȫ�ɡ��ϵ�״̬ʱ����ʾ�˶�����û�гɽ����������ڲ�����ȫ��״̬ʱ����ʾ�˶���������������order_info.order_cancel_xtp_idΪ������Ӧ�ĳ���ID����Ϊ0ʱ��ʾ�˵������ɹ�
	///@param error_info �������ܾ����߷�������ʱ�������ʹ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ÿ�ζ���״̬����ʱ�����ᱻ���ã���Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߣ��ڶ���δ�ɽ���ȫ���ɽ���ȫ�����������ֳ������Ѿܾ���Щ״̬ʱ������Ӧ�����ڲ��ֳɽ�����������ɶ����ĳɽ��ر�������ȷ�ϡ����е�¼�˴��û��Ŀͻ��˶����յ����û��Ķ�����Ӧ
	virtual void OnOptionCombinedOrderEvent(XTPOptCombOrderInfo *order_info, XTPRI *error_info, uint64_t session_id);

	///��Ȩ��ϲ��Գɽ�֪ͨ
	///@param trade_info �ɽ��ر��ľ�����Ϣ���û�����ͨ��trade_info.order_xtp_id����������ͨ��GetClientIDByXTPID() == client_id�������Լ��Ķ����������Ͻ�����exec_id����Ψһ��ʶһ�ʳɽ���������2�ʳɽ��ر�ӵ����ͬ��exec_id���������Ϊ�˱ʽ����Գɽ��ˡ����������exec_id��Ψһ�ģ���ʱ�޴��жϻ��ơ�report_index+market�ֶο������Ψһ��ʶ��ʾ�ɽ��ر���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark �����гɽ�������ʱ�򣬻ᱻ���ã���Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ����е�¼�˴��û��Ŀͻ��˶����յ����û��ĳɽ��ر�����ض���Ϊ����״̬����Ҫ�û�ͨ���ɽ��ر��ĳɽ�������ȷ����OnOrderEvent()�������Ͳ���״̬��
	virtual void OnOptionCombinedTradeEvent(XTPOptCombTradeReport *trade_info, uint64_t session_id);

	///��Ȩ��ϲ��Գ���������Ӧ
	///@param cancel_info ����������Ϣ������������order_cancel_xtp_id�ʹ�������order_xtp_id
	///@param error_info �������ܾ����߷�������ʱ�������ʹ�����Ϣ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߣ���error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ����Ӧֻ���ڳ�����������ʱ���ص�
	virtual void OnCancelOptionCombinedOrderError(XTPOptCombOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id) ;

	///�����ѯ��Ȩ��ϲ��Ա�����Ӧ
	///@param order_info ��ѯ����һ������
	///@param error_info ��ѯ����ʱ��������ʱ�����صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ����֧�ַ�ʱ�β�ѯ��һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ��˶�Ӧ����������������ѯʹ�ã�������������ʱ����������û���·ӵ�£�����api����
	virtual void OnQueryOptionCombinedOrders(XTPQueryOptCombOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) ;

	///�����ѯ��Ȩ��ϲ��Ա�����Ӧ-�°汾�ӿ�
	///@param order_info ��ѯ����һ������
	///@param error_info ��ѯ����ʱ��������ʱ�����صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ����֧�ַ�ʱ�β�ѯ��һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ��˶�Ӧ����������������ѯʹ�ã�������������ʱ����������û���·ӵ�£�����api����
	virtual void OnQueryOptionCombinedOrdersEx(XTPOptCombOrderInfoEx *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///��ҳ�����ѯ��Ȩ��ϲ��Ա�����Ӧ
	///@param order_info ��ѯ����һ������
	///@param req_count ��ҳ������������
	///@param order_sequence ��ҳ����ĵ�ǰ�ر�����
	///@param query_reference ��ǰ������Ϣ����Ӧ�Ĳ�ѯ��������Ҫ��¼�������ڽ�����һ�η�ҳ��ѯ��ʱ����Ҫ�õ�
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��order_sequenceΪ0���������β�ѯû�в鵽�κμ�¼����is_lastΪtrueʱ�����order_sequence����req_count����ô��ʾ���б��������Խ�����һ�η�ҳ��ѯ��������ȣ���ʾ���б����Ѿ���ѯ��ϡ�һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ�
	virtual void OnQueryOptionCombinedOrdersByPage(XTPQueryOptCombOrderRsp *order_info, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) ;

	///��ҳ�����ѯ��Ȩ��ϲ��Ա�����Ӧ-�°汾�ӿ�
	///@param order_info ��ѯ����һ������
	///@param req_count ��ҳ������������
	///@param order_sequence ��ҳ����ĵ�ǰ�ر�����
	///@param query_reference ��ǰ������Ϣ����Ӧ�Ĳ�ѯ��������Ҫ��¼�������ڽ�����һ�η�ҳ��ѯ��ʱ����Ҫ�õ�
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��order_sequenceΪ0���������β�ѯû�в鵽�κμ�¼����is_lastΪtrueʱ�����order_sequence����req_count����ô��ʾ���б��������Խ�����һ�η�ҳ��ѯ��������ȣ���ʾ���б����Ѿ���ѯ��ϡ�һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ�
	virtual void OnQueryOptionCombinedOrdersByPageEx(XTPOptCombOrderInfoEx *order_info, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) ;

	///�����ѯ��Ȩ��ϲ��Գɽ���Ӧ
	///@param trade_info ��ѯ����һ���ɽ��ر�
	///@param error_info ��ѯ�ɽ��ر���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ����֧�ַ�ʱ�β�ѯ��һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ��˶�Ӧ����������������ѯʹ�ã�������������ʱ����������û���·ӵ�£�����api����
	virtual void OnQueryOptionCombinedTrades(XTPQueryOptCombTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id) ;

	///��ҳ�����ѯ��Ȩ��ϲ��Գɽ���Ӧ
	///@param trade_info ��ѯ����һ���ɽ���Ϣ
	///@param req_count ��ҳ������������
	///@param trade_sequence ��ҳ����ĵ�ǰ�ر�����
	///@param query_reference ��ǰ������Ϣ����Ӧ�Ĳ�ѯ��������Ҫ��¼�������ڽ�����һ�η�ҳ��ѯ��ʱ����Ҫ�õ�
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��trade_sequenceΪ0���������β�ѯû�в鵽�κμ�¼����is_lastΪtrueʱ�����trade_sequence����req_count����ô��ʾ���лر������Խ�����һ�η�ҳ��ѯ��������ȣ���ʾ���лر��Ѿ���ѯ��ϡ�һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ�
	virtual void OnQueryOptionCombinedTradesByPage(XTPQueryOptCombTradeRsp *trade_info, int64_t req_count, int64_t trade_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id);

	///�����ѯ��Ȩ��ϲ��Գֲ���Ӧ
	///@param position_info ��ѯ����һ���ֲ���Ϣ
	///@param error_info ��ѯ�ֲַ�������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ�
	virtual void OnQueryOptionCombinedPosition(XTPQueryOptCombPositionRsp *position_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///�����ѯ��Ȩ��ϲ�����Ϣ��Ӧ
	///@param strategy_info ��ѯ����һ����ϲ�����Ϣ
	///@param error_info ��ѯ�ɽ��ر���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ�
	virtual void OnQueryOptionCombinedStrategyInfo(XTPQueryCombineStrategyInfoRsp *strategy_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///��ѯ��Ȩ��Ȩ�ϲ�ͷ�����Ӧ
	///@param position_info ��ѯ����һ����Ȩ�ϲ�ͷ����Ϣ
	///@param error_info ��ѯ�ֲַ�������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark һ����ѯ������ܶ�Ӧ�����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ�����ߡ�
	virtual void OnQueryOptionCombinedExecPosition(XTPQueryOptCombExecPosRsp *position_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id);

	///�����ѯ�����ڵ�����ʽ����Ӧ����Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	///@param fund_info ��ѯ���������ڵ�����ʽ����
	///@param error_info ��ѯ�����ڵ�����ʽ�������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryOtherServerFund(XTPFundQueryRsp *fund_info, XTPRI *error_info, int request_id, uint64_t session_id);


	////////////////////////////////////////////////////////////////
	/*****algo algo algo algo algo algo algo algo algo algo  ******/
	////////////////////////////////////////////////////////////////

	///algoҵ���в�ѯ�����б����Ӧ
	///@param strategy_info ���Ծ�����Ϣ
	///@param strategy_param �˲����а����Ĳ��������error_info.error_idΪ0ʱ��������
	///@param error_info ��ѯ��ѯ�����б�������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param request_id ����Ϣ��Ӧ������Ӧ������ID
	///@param is_last ����Ϣ��Ӧ�����Ƿ�Ϊrequest_id������������Ӧ�����һ����Ӧ����Ϊ���һ����ʱ��Ϊtrue�����Ϊfalse����ʾ��������������Ϣ��Ӧ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnQueryStrategy(XTPStrategyInfoStruct* strategy_info, char* strategy_param, XTPRI *error_info, int32_t request_id, bool is_last, uint64_t session_id);

	///algoҵ���в�������ʱ����״̬֪ͨ
	///@param strategy_state �û��������������״̬֪ͨ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnStrategyStateReport(XTPStrategyStateReportStruct* strategy_state, uint64_t session_id);

	///algoҵ�����û������㷨ͨ������Ϣ��Ӧ
	///@param user �û���
	///@param error_info �����㷨ͨ����������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д��󣬼��㷨ͨ���ɹ�
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark �㷨ͨ�������ɹ��󣬲��ܶ��û��������ԵȲ�����һ���û�ֻ��ӵ��һ���㷨ͨ�������֮ǰ�Ѿ��������������ظ�����
	virtual void OnALGOUserEstablishChannel(char* user, XTPRI* error_info, uint64_t session_id);

	///algoҵ���б��Ͳ��Ե�����Ӧ
	///@param strategy_info �û����͵Ĳ��Ե��ľ�����Ϣ
	///@param error_info ���Ͳ��Ե���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnInsertAlgoOrder(XTPStrategyInfoStruct* strategy_info, XTPRI *error_info, uint64_t session_id);

	///algoҵ���г������Ե�����Ӧ
	///@param strategy_info �û������Ĳ��Ե��ľ�����Ϣ
	///@param error_info �������Ե���������ʱ���صĴ�����Ϣ����error_infoΪ�գ�����error_info.error_idΪ0ʱ������û�д���
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnCancelAlgoOrder(XTPStrategyInfoStruct* strategy_info, XTPRI *error_info, uint64_t session_id);

	///���ͻ�����AlgoBusͨ�����ӶϿ�ʱ���÷��������á�
	///@param reason ����ԭ��������������Ӧ
	///@remark �벻Ҫ�������̣߳������Ӱ��algo�ĵ�¼����Algo֮������ӣ����ߺ���Զ��������û���������������
	virtual void OnAlgoDisconnected(int reason) ;

	///���ͻ�����AlgoBus���ߺ���������ʱ���÷��������ã����ڶ��������ɹ���ᱻ���á�
	virtual void OnAlgoConnected();

	///algoҵ���в�������ʱ����ָ��֤ȯִ��״̬֪ͨ
	///@param strategy_symbol_state �û�����ָ��֤ȯ���������״̬֪ͨ
	///@param session_id �ʽ��˻���Ӧ��session_id����¼ʱ�õ�
	///@remark ��Ҫ���ٷ��أ���������������Ϣ������������ʱ���ᴥ������
	virtual void OnStrategySymbolStateReport(XTPStrategySymbolStateReport* strategy_symbol_state, uint64_t session_id) ;

	//-------------------------------------------------------------------------------------
	//task������
	//-------------------------------------------------------------------------------------

	void processTask();

	void processDisconnected(Task *task);

	void processError(Task *task);

	void processQueryAccountTradeMarket(Task *task);

	void processOrderEvent(Task *task);

	void processTradeEvent(Task *task);

	void processCancelOrderError(Task *task);

	void processQueryOrder(Task *task);

	void processQueryOrderEx(Task *task);

	void processQueryOrderByPage(Task *task);

	void processQueryOrderByPageEx(Task *task);

	void processQueryTrade(Task *task);

	void processQueryTradeByPage(Task *task);

	void processQueryPosition(Task *task);

	void processQueryAsset(Task *task);

	void processQueryStructuredFund(Task *task);

	void processQueryFundTransfer(Task *task);

	void processFundTransfer(Task *task);

	void processQueryETF(Task *task);

	void processQueryETFBasket(Task *task);

	void processQueryIPOInfoList(Task *task);

	void processQueryIPOQuotaInfo(Task *task);

	void processQueryOptionAuctionInfo(Task *task);

	void processCreditCashRepay(Task *task);

	void processQueryCreditCashRepayInfo(Task *task);

	void processQueryCreditFundInfo(Task *task);

	void processQueryCreditDebtInfo(Task *task);

	void processQueryCreditTickerDebtInfo(Task *task);

	void processQueryCreditAssetDebtInfo(Task *task);

	void processQueryCreditTickerAssignInfo(Task *task);

	void processQueryCreditExcessStock(Task *task);

	void processQueryMulCreditExcessStock(Task *task);

	void processCreditExtendDebtDate(Task *task);

	void processQueryCreditExtendDebtDateOrders(Task *task);

	void processQueryCreditFundExtraInfo(Task *task);

	void processQueryCreditPositionExtraInfo(Task *task);

	void processCreditCashRepayDebtInterestFee(Task *task);


	void processOptionCombinedOrderEvent(Task *task);

	void processOptionCombinedTradeEvent(Task *task);

	void processQueryOptionCombinedOrders(Task *task);

	void processQueryOptionCombinedOrdersEx(Task *task);

	void processQueryOptionCombinedOrdersByPage(Task *task);

	void processQueryOptionCombinedOrdersByPageEx(Task *task);

	void processQueryOptionCombinedTrades(Task *task);

	void processQueryOptionCombinedTradesByPage(Task *task);

	void processQueryOptionCombinedPosition(Task *task);

	void processQueryOptionCombinedStrategyInfo(Task *task);

	void processCancelOptionCombinedOrderError(Task *task);

	void processQueryOptionCombinedExecPosition(Task *task);

	void processQueryOtherServerFund(Task *task);

	//////////algo/////////
	void processQueryStrategy(Task *task);

	void processStrategyStateReport(Task *task);

	void processALGOUserEstablishChannel(Task *task);

	void processInsertAlgoOrder(Task *task);

	void processCancelAlgoOrder(Task *task);

	void processAlgoDisconnected(Task *task);

	void processAlgoConnected(Task *task);

	void processStrategySymbolStateReport(Task *task);
	//-------------------------------------------------------------------------------------
	//data���ص������������ֵ�
	//error���ص������Ĵ����ֵ�
	//reqid������id
	//last���Ƿ�Ϊ��󷵻�
	//-------------------------------------------------------------------------------------

	virtual void onDisconnected(uint64_t session, int reason) {};

	virtual void onError(dict data) {};

	virtual void onQueryAccountTradeMarket(int trade_location, dict error, int request_id, uint64_t session_id) {};

	virtual void onOrderEvent(dict data, dict error, uint64_t session) {};

	virtual void onTradeEvent(dict data, uint64_t session) {};

	virtual void onCancelOrderError(dict data, dict error, uint64_t session) {};

	virtual void onQueryOrder(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onQueryOrderEx(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onQueryTrade(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onQueryPosition(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onQueryAsset(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onQueryStructuredFund(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onQueryFundTransfer(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onFundTransfer(dict data, dict error, uint64_t session) {};

	virtual void onQueryETF(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onQueryETFBasket(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onQueryIPOInfoList(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onQueryIPOQuotaInfo(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onQueryOptionAuctionInfo(dict data, dict error, int reqid, bool last, uint64_t session) {};

	virtual void onCreditCashRepay(dict data, dict error_info, uint64_t session) {};

	virtual void onQueryCreditCashRepayInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {};

	virtual void onQueryCreditFundInfo(dict data, dict error_info, int request_id, uint64_t session) {};

	virtual void onQueryCreditDebtInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {};

	virtual void onQueryCreditTickerDebtInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {};

	virtual void onQueryCreditAssetDebtInfo(double remain_amount, dict error_info, int request_id, uint64_t session) {};

	virtual void onQueryCreditTickerAssignInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {};

	virtual void onQueryCreditExcessStock(dict data, dict error_info, int request_id, uint64_t session) {};

	virtual void onQueryMulCreditExcessStock(dict data, dict error_info, int request_id, uint64_t session, bool last) {};

	virtual void onCreditExtendDebtDate(dict data, dict error_info, uint64_t session) {};

	virtual void onQueryCreditExtendDebtDateOrders(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {};

	virtual void onQueryCreditFundExtraInfo(dict data, dict error_info, int request_id, uint64_t session) {};

	virtual void onQueryCreditPositionExtraInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {};

	virtual void onQueryOrderByPage(dict data, int64_t req_count, int64_t order_sequence, int64_t query_reference, int reqid, bool last, uint64_t session) {};

	virtual void onQueryOrderByPageEx(dict data, int64_t req_count, int64_t order_sequence, int64_t query_reference, int reqid, bool last, uint64_t session) {};

	virtual void onQueryTradeByPage(dict data, int64_t req_count, int64_t trade_sequence, int64_t query_reference, int reqid, bool last, uint64_t session) {};

	virtual void onCreditCashRepayDebtInterestFee(dict data, dict error_info, uint64_t session) {};




	virtual void onOptionCombinedOrderEvent(dict data, dict error_info, uint64_t session_id) {};

	virtual void onOptionCombinedTradeEvent(dict data, uint64_t session_id) {};

	virtual void onQueryOptionCombinedOrders(dict data, dict error_info, int request_id, bool is_last, uint64_t session_id) {};

	virtual void onQueryOptionCombinedOrdersEx(dict data, dict error_info, int request_id, bool is_last, uint64_t session_id) {};

	virtual void onQueryOptionCombinedOrdersByPage(dict data, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {};

	virtual void onQueryOptionCombinedOrdersByPageEx(dict data, int64_t req_count, int64_t order_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {};

	virtual void onQueryOptionCombinedTrades(dict data, dict error_info, int request_id, bool is_last, uint64_t session_id) {};

	virtual void onQueryOptionCombinedTradesByPage(dict data, int64_t req_count, int64_t trade_sequence, int64_t query_reference, int request_id, bool is_last, uint64_t session_id) {};

	virtual void onQueryOptionCombinedPosition(dict data, dict error_info, int request_id, bool is_last, uint64_t session_id) {};

	virtual void onQueryOptionCombinedStrategyInfo(dict data, dict error_info, int request_id, bool is_last, uint64_t session_id) {};

	virtual void onCancelOptionCombinedOrderError(dict data, dict error_info, uint64_t session) {};

	virtual void onQueryOptionCombinedExecPosition(dict data, dict error_info, int request_id, bool is_last, uint64_t session) {};

	virtual void onQueryOtherServerFund(dict data, dict error_info, int request_id, uint64_t session) {};


	//////////////algo////////

	virtual void onQueryStrategy(dict data, string strategy_param, dict error_info, int32_t request_id, bool is_last, uint64_t session_id){};

	virtual void onStrategyStateReport(dict data,  uint64_t session_id){};

	virtual void onALGOUserEstablishChannel(string user,dict error_info, uint64_t session_id){};

	virtual void onInsertAlgoOrder(dict data,dict error_info, uint64_t session_id){};

	virtual void onCancelAlgoOrder(dict data,dict error_info, uint64_t session_id){};

	virtual void onAlgoDisconnected(int reason){};

	virtual void onAlgoConnected(){};

	virtual void onStrategySymbolStateReport(dict data, uint64_t session_id) {};
	//-------------------------------------------------------------------------------------
	//req:���������������ֵ�
	//-------------------------------------------------------------------------------------

	void createTraderApi(uint8_t clientid, string path, int log_level);

	void release();

	int exit();

	string getTradingDay();

	dict getApiLastError();

	string getApiVersion();

	uint8_t getClientIDByXTPID(uint64_t orderid);

	string getAccountByXTPID(uint64_t orderid);

	void subscribePublicTopic(int tpye);

	void setSoftwareKey(string key);

	void setSoftwareVersion(string version);

	void setHeartBeatInterval(uint32_t interval);

	uint64_t login(string ip, int port, string user, string password, int socktype,string local_ip);

	int logout(uint64_t sessionid);

	int modifyUserTerminalInfo(dict info, uint64_t session_id);

	int queryAccountTradeMarket(uint64_t session_id, int request_id);

	uint64_t getANewOrderXTPID(uint64_t session_id);

	uint64_t insertOrder(dict req, uint64_t sessionid);

	uint64_t insertOrderExtra(dict req, uint64_t session_id);

	uint64_t cancelOrder(uint64_t orderid, uint64_t sessionid);

	int queryOrderByXTPID(uint64_t orderid, uint64_t sessionid, int reqid);

	int queryOrderByXTPIDEx(uint64_t orderid, uint64_t sessionid, int reqid);

	int queryOrders(dict req, uint64_t sessionid, int reqid);

	int queryOrdersEx(dict req, uint64_t sessionid, int reqid);

	int queryUnfinishedOrders(uint64_t sessionid, int reqid);

	int queryUnfinishedOrdersEx(uint64_t sessionid, int reqid);

	int queryTradesByXTPID(uint64_t orderid, uint64_t sessionid, int reqid);

	int queryTrades(dict req, uint64_t sessionid, int reqid);

	int queryPosition(string ticker, uint64_t sessionid, int reqid);

	int queryAsset(uint64_t sessionid, int reqid);

	int queryStructuredFund(dict req, uint64_t sessionid, int reqid);

	uint64_t fundTransfer(dict req, uint64_t sessionid);

	int queryFundTransfer(dict req, uint64_t sessionid, int reqid);

	int queryETF(dict req, uint64_t sessionid, int reqid);

	int queryETFTickerBasket(dict req, uint64_t sessionid, int reqid);

	int queryIPOInfoList(uint64_t sessionid, int reqid);

	int queryIPOQuotaInfo(uint64_t sessionid, int reqid);

	int queryOptionAuctionInfo(dict req,uint64_t session_id, int request_id);

	uint64_t creditCashRepay(double amount, uint64_t session_id);

	int queryCreditCashRepayInfo(uint64_t session_id, int request_id);

	int queryCreditFundInfo(uint64_t session_id, int request_id);

	int queryCreditDebtInfo(uint64_t session_id, int request_id);

	int queryCreditTickerDebtInfo(dict req, uint64_t session_id, int request_id);

	int queryCreditAssetDebtInfo(uint64_t session_id, int request_id);

	int queryCreditTickerAssignInfo(dict req, uint64_t session_id, int request_id);

	int queryCreditExcessStock(dict req, uint64_t session_id, int request_id);

	int queryMulCreditExcessStock(dict req, uint64_t session_id, int request_id);

	uint64_t creditExtendDebtDate(dict req, uint64_t session_id);

	int queryCreditExtendDebtDateOrders(uint64_t xtp_id,uint64_t session_id, int request_id);

	int queryCreditFundExtraInfo( uint64_t session_id, int request_id);

	int queryCreditPositionExtraInfo(dict req, uint64_t session_id, int request_id);

	int queryOrdersByPage(dict req, uint64_t sessionid, int reqid);

	int queryOrdersByPageEx(dict req, uint64_t sessionid, int reqid);

	int queryTradesByPage(dict req, uint64_t sessionid, int reqid);

	bool isServerRestart(uint64_t session_id);

	uint64_t creditCashRepayDebtInterestFee(string debt_id, double amount, uint64_t session_id);

	uint64_t creditSellStockRepayDebtInterestFee(dict req, string debt_id, uint64_t session_id);

	uint64_t insertOptionCombinedOrder(dict req, uint64_t session_id);

	uint64_t insertOptionCombinedOrderExtra(dict req, uint64_t session_id);

	int queryOptionCombinedUnfinishedOrders(uint64_t session_id, int request_id);

	int queryOptionCombinedUnfinishedOrdersEx(uint64_t session_id, int request_id);

	int queryOptionCombinedOrderByXTPID(const uint64_t order_xtp_id, uint64_t session_id, int request_id);

	int queryOptionCombinedOrderByXTPIDEx(const uint64_t order_xtp_id, uint64_t session_id, int request_id);

	int queryOptionCombinedOrders(dict req, uint64_t session_id, int request_id);

	int queryOptionCombinedOrdersEx(dict req, uint64_t session_id, int request_id);

	int queryOptionCombinedOrdersByPage(dict req, uint64_t session_id, int request_id);

	int queryOptionCombinedOrdersByPageEx(dict req, uint64_t session_id, int request_id);

	int queryOptionCombinedTradesByXTPID(const uint64_t order_xtp_id, uint64_t session_id, int request_id);

	int queryOptionCombinedTrades(dict req, uint64_t session_id, int request_id);

	int queryOptionCombinedTradesByPage(dict req, uint64_t session_id, int request_id);

	int queryOptionCombinedPosition(dict req, uint64_t session_id, int request_id);

	int queryOptionCombinedStrategyInfo(uint64_t session_id, int request_id);

	uint64_t cancelOptionCombinedOrder(const uint64_t order_xtp_id, uint64_t session_id);

	int queryOptionCombinedExecPosition(dict req, uint64_t session_id, int request_id);

	int queryOtherServerFund(dict req, uint64_t session_id, int request_id);

	/////////////////////algo///////////////////////

	int loginALGO(string ip, int port, string user, string password, int socktype,string local_ip);

	int queryStrategy(uint32_t strategy_type, uint64_t client_strategy_id, uint64_t xtp_strategy_id, uint64_t session_id, int32_t request_id);

	int aLGOUserEstablishChannel(string oms_ip, int oms_port, string user, string password, uint64_t session_id);

	int insertAlgoOrder(uint32_t strategy_type, uint64_t client_strategy_id, string strategy_param, uint64_t session_id);

	int cancelAlgoOrder(bool cancel_flag, uint64_t xtp_strategy_id, uint64_t session_id);

	uint64_t getAlgorithmIDByOrder(uint64_t order_xtp_id, uint32_t order_client_id);
};

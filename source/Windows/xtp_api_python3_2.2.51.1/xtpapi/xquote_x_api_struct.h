
////////////////////////////////////////
///@author 中泰证券股份有限公司
///@file xquote_x_api_struct.h
///@brief 定义XTP4.0行情使用的数据基本类型
////////////////////////////////////////

#ifndef _XQUOTE_NG_API_STRUCT_H_
#define _XQUOTE_NG_API_STRUCT_H_

#include "xquote_x_api_data_type.h"

#pragma pack(push)
#pragma pack(1)

namespace XTPX {

	namespace API {
		///指定的合约
		typedef struct XTPSpecificTickerStruct
		{
			///合约代码（不包含交易所信息）例如"600000"，不带空格，以'\0'结尾
			char    ticker[XTP_QUOTE_TICKER_LEN];
			///交易所代码
			XTP_EXCHANGE_TYPE exchange_id;

		} XTPST;


		///股票、基金 等额外数据
		struct XTPMarketDataStockExData {
			///委托买入总量(SH,SZ)
			int64_t total_bid_qty;
			///委托卖出总量(SH,SZ)
			int64_t total_ask_qty;
			///加权平均委买价格(SH,SZ)
			double  ma_bid_price;
			///加权平均委卖价格(SH,SZ)
			double  ma_ask_price;
			///债券加权平均委买价格(SH)
			double  ma_bond_bid_price;
			///债券加权平均委卖价格(SH)
			double  ma_bond_ask_price;
			///债券到期收益率(SH)
			double  yield_to_maturity;
			///基金实时参考净值(SH,SZ)
			double  iopv;
			///ETF申购笔数(SH)
			int32_t etf_buy_count;
			///ETF赎回笔数(SH)
			int32_t etf_sell_count;
			///ETF申购数量(SH)
			double  etf_buy_qty;
			///ETF申购金额(SH)
			double  etf_buy_money;
			///ETF赎回数量(SH)
			double  etf_sell_qty;
			///ETF赎回金额(SH)
			double  etf_sell_money;
			///权证执行的总数量(SH)
			double  total_warrant_exec_qty;
			///权证跌停价格（元）(SH)
			double  warrant_lower_price;
			///权证涨停价格（元）(SH)
			double  warrant_upper_price;
			///买入撤单笔数(SH)
			int32_t cancel_buy_count;
			///卖出撤单笔数(SH)
			int32_t cancel_sell_count;
			///买入撤单数量(SH)
			double  cancel_buy_qty;
			///卖出撤单数量(SH)
			double  cancel_sell_qty;
			///买入撤单金额(SH)
			double  cancel_buy_money;
			///卖出撤单金额(SH)
			double  cancel_sell_money;
			///买入总笔数(SH)
			int64_t total_buy_count;
			///卖出总笔数(SH)
			int64_t total_sell_count;
			///买入委托成交最大等待时间(SH)
			int32_t duration_after_buy;
			///卖出委托成交最大等待时间(SH)
			int32_t duration_after_sell;
			///买方委托价位数(SH)
			int32_t num_bid_orders;
			///卖方委托价位数(SH)
			int32_t num_ask_orders;

			///基金T-1日净值(SZ)
			double  pre_iopv;
			///预留
			int64_t r1;
			///预留
			int64_t r2;
		};

		///债券额外数据
		struct XTPMarketDataBondExData {
			///委托买入总量(SH,SZ)
			int64_t total_bid_qty;
			///委托卖出总量(SH,SZ)
			int64_t total_ask_qty;
			///加权平均委买价格(SZ)
			double  ma_bid_price;
			///加权平均委卖价格(SZ)
			double  ma_ask_price;
			///债券加权平均委买价格(SH)
			double  ma_bond_bid_price;
			///债券加权平均委卖价格(SH)
			double  ma_bond_ask_price;
			///债券到期收益率(SH)
			double  yield_to_maturity;
			///匹配成交最近价(SZ)
			double  match_lastpx;
			///债券加权平均价格(SH)
			double  ma_bond_price;
			///匹配成交成交量(SZ)
			int64_t match_qty;
			///匹配成交成交金额(SZ)
			double  match_turnover;
			///预留
			double  r4;
			///预留
			double  r5;
			///预留
			double  r6;
			///预留
			double  r7;
			///预留
			double  r8;
			///买入撤单笔数(SH)
			int32_t cancel_buy_count;
			///卖出撤单笔数(SH)
			int32_t cancel_sell_count;
			///买入撤单数量(SH)
			double  cancel_buy_qty;
			///卖出撤单数量(SH)
			double  cancel_sell_qty;
			///买入撤单金额(SH)
			double  cancel_buy_money;
			///卖出撤单金额(SH)
			double  cancel_sell_money;
			///买入总笔数(SH)
			int64_t total_buy_count;
			///卖出总笔数(SH)
			int64_t total_sell_count;
			///买入委托成交最大等待时间(SH)
			int32_t duration_after_buy;
			///卖出委托成交最大等待时间(SH)
			int32_t duration_after_sell;
			///买方委托价位数(SH)
			int32_t num_bid_orders;
			///卖方委托价位数(SH)
			int32_t num_ask_orders;
			///时段(SHL2)
			char    instrument_status[8];
		};

		// 期权额外数据
		struct XTPMarketDataOptionExData {
			///波段性中断参考价(SH)
			double  auction_price;
			///波段性中断集合竞价虚拟匹配量(SH)
			int64_t auction_qty;
			///最近询价时间(SH)
			int64_t last_enquiry_time;
		};



		///行情
		typedef struct XTPMarketDataStruct
		{
			// 代码
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			char    ticker[XTP_QUOTE_TICKER_LEN];
			///交易所代码
			XTP_EXCHANGE_TYPE exchange_id;
			/// 决定了union是哪种数据类型
			XTP_MARKETDATA_TYPE_V2 data_type_v2;

			// 价格
			///最新价
			double  last_price;
			///昨收盘
			double  pre_close_price;
			///今开盘
			double  open_price;
			///最高价
			double  high_price;
			///最低价
			double  low_price;
			///今收盘
			double  close_price;

			// 期权数据
			///昨日持仓量(张)(目前未填写)
			int64_t pre_total_long_positon;
			///持仓量(张)
			int64_t total_long_positon;
			///昨日结算价
			double  pre_settl_price;
			///今日结算价
			double  settl_price;

			// 涨跌停
			///SH:上交所涨跌停价赋值为0
			///涨停价
			double  upper_limit_price;
			///跌停价
			double  lower_limit_price;
			///预留
			double  pre_delta;
			///预留
			double  curr_delta;

			/// 时间类，格式为YYYYMMDDHHMMSSsss
			int64_t data_time;

			// 量额数据
			///数量，为总成交量（单位股，与交易所一致）
			int64_t qty;
			///成交金额，为总成交金额（单位元，与交易所一致）
			double  turnover;
			///预留(无意义)
			double  avg_price;

			// 买卖盘
			///十档申买价
			double  bid[10];
			///十档申卖价
			double  ask[10];
			///十档申买量
			int64_t bid_qty[10];
			///十档申卖量
			int64_t ask_qty[10];

			// 额外数据
			///成交笔数
			int64_t trades_count;
			///当前交易状态说明，参见官网常见问题说明
			char    ticker_status[8];
			///数据
			union {
				XTPMarketDataStockExData  stk;
				XTPMarketDataOptionExData opt;
				XTPMarketDataBondExData  bond;
			};
			int64_t r1; // 预留字段,Rebuild查询md时为首次更新时间

		} XTPMD;

		///IOPV信息
		struct IOPV {
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			char    ticker[XTP_QUOTE_TICKER_LEN];
			///交易所代码
			XTP_EXCHANGE_TYPE exchange_id;
			///预留
			int32_t unused;
			/// 时间
			int64_t data_time;
			/// iopv值
			double  iopv;
		};


		///订单薄
		typedef struct OrderBookStruct {
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			char    ticker[XTP_QUOTE_TICKER_LEN];
			///交易所代码
			XTP_EXCHANGE_TYPE exchange_id;
			///预留
			int32_t unused;

			///最新价
			double  last_price;
			///数量，为总成交量
			int64_t qty;
			///成交金额，为总成交金额
			double  turnover;
			///成交笔数
			int64_t trades_count;

			// 买卖盘
			///十档申买价
			double  bid[10];
			///十档申卖价
			double  ask[10];
			///十档申买量
			int64_t bid_qty[10];
			///十档申卖量
			int64_t ask_qty[10];
			/// 时间类
			int64_t data_time;
		} XTPOB;



		///逐笔委托
		struct XTPTickByTickEntrust {
			///频道代码
			int32_t channel_no;
			///SH: 'B':买; 'S':卖
			///SZ: '1':买; '2':卖; 'G':借入; 'F':出借
			char    side;
			///SH: 'A': 增加; 'D': 删除
			///SZ: 订单类别: '1': 市价; '2': 限价; 'U': 本方最优
			char    ord_type;
			///预留
			char    unused[2];
			///SH: 委托序号(委托成交统一编号, 同一channel_no内连续)
			///SZ: 委托序号(委托成交统一编号, 同一channel_no内连续)
			int64_t seq;
			///委托价格
			double  price;
			///SH: 剩余委托数量(balance)
			///SZ: 委托数量
			int64_t qty;
			///SH: 原始订单号
			///SZ: 无意义
			int64_t order_no;

			/// SH: 已成交的委托数量
			/// SZ: 无意义
			int64_t traded_qty;
			};

		///逐笔成交
		struct XTPTickByTickTrade {
			///频道代码
			int32_t channel_no;
			/// SH: 内外盘标识('B':主动买; 'S':主动卖; 'N':未知)
			/// SZ: 成交标识('4':撤; 'F':成交)
			char    trade_flag;
			///预留
			char    unused[3];
			///SH: 成交序号(委托成交统一编号, 同一channel_no内连续)
			///SZ: 成交序号(委托成交统一编号, 同一channel_no内连续)
			int64_t seq;
			///成交价格
			double  price;
			///成交量
			int64_t qty;
			///成交金额(仅适用上交所)
			double  money;
			///买方订单号
			int64_t bid_no;
			///卖方订单号
			int64_t ask_no;
		};

		///逐笔状态订单
		struct XTPTickByTickStatus {
			///频道代码
			int32_t channel_no;
			///预留1
			int32_t unused1;
			///同一channel_no内连续
			int64_t seq;
			///状态信息
			char    flag[8];
		};

		///逐笔数据信息
		typedef struct XTPTickByTickStruct {
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			char    ticker[XTP_QUOTE_TICKER_LEN];
			///交易所代码
			XTP_EXCHANGE_TYPE exchange_id;
			///委托 or 成交
			XTP_TBT_TYPE type;

			/// SH: 业务序号（委托成交统一编号，同一个channel_no内连续）
			/// SZ: 无意义
			int64_t seq;
			///委托时间 or 成交时间
			int64_t data_time;

			union {
				XTPTickByTickEntrust   entrust;
				XTPTickByTickTrade     trade;
				XTPTickByTickStatus    state;
			};
		} XTPTBT;


		///供查询的最新信息
		typedef struct XTPTickerPriceInfo {
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			char    ticker[XTP_QUOTE_TICKER_LEN];
			///交易所代码
			XTP_EXCHANGE_TYPE exchange_id;
			///预留
			int32_t unused;
			///最新价
			double  last_price;
		} XTPTPI;


		///股票行情静态信息
		typedef struct XTPQuoteStaticInfo {
			///交易所代码
			XTP_EXCHANGE_TYPE exchange_id;
			/// 合约类型
			XTP_QUOTE_TICKER_TYPE ticker_type;
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			char    ticker[XTP_QUOTE_TICKER_LEN];
			/// 合约名称
			char    ticker_name[XTP_QUOTE_TICKER_NAME_LEN];
			///昨收盘
			double  pre_close_price;
			///涨停板价
			double  upper_limit_price;
			///跌停板价
			double  lower_limit_price;
			///最小变动价位
			double  price_tick;
			/// 合约最小交易量(买)
			int32_t buy_qty_unit;
			/// 合约最小交易量(卖)
			int32_t sell_qty_unit;
		} XTPQSI;


		///股票行情全量静态信息
		typedef struct XTPQuoteFullInfo {
			XTP_EXCHANGE_TYPE  exchange_id;                    ///<交易所代码
			XTP_SECURITY_TYPE  security_type;                  ///<合约详细类型
			char    ticker[XTP_QUOTE_TICKER_LEN];              ///<证券代码
			char    ticker_name[XTP_QUOTE_TICKER_NAME_LEN];    ///<证券名称
			XTP_QUALIFICATION_TYPE ticker_qualification_class; ///<合约适当性类别
			bool    is_registration;                           ///<是否注册制(仅适用创业板股票，创新企业股票及存托凭证)
			bool    is_VIE;                                    ///<是否具有协议控制架构(仅适用创业板股票，创新企业股票及存托凭证)
			bool    is_noprofit;                               ///<是否尚未盈利(仅适用创业板股票，创新企业股票及存托凭证)
			bool    is_weighted_voting_rights;                 ///<是否存在投票权差异(仅适用创业板股票，创新企业股票及存托凭证)
			bool    is_have_price_limit;                       ///<是否有涨跌幅限制(注：不提供具体幅度，可通过涨跌停价和昨收价来计算幅度)
			bool    is_inventory;                              ///<是否为存量科创板股票（即2025.07.13日前上市的）。1=是；0=否；
			char    unused[6];                                 ///预留
			double  upper_limit_price;                         ///<涨停价（仅在有涨跌幅限制时有效）
			double  lower_limit_price;                         ///<跌停价（仅在有涨跌幅限制时有效）
			double  pre_close_price;                           ///<昨收价
			double  price_tick;                                ///<价格最小变动价位
			int32_t bid_qty_upper_limit;                       ///<限价买委托数量上限
			int32_t bid_qty_lower_limit;                       ///<限价买委托数量下限
			int32_t bid_qty_unit;                              ///<限价买数量单位
			int32_t ask_qty_upper_limit;                       ///<限价卖委托数量上限
			int32_t ask_qty_lower_limit;                       ///<限价卖委托数量下限
			int32_t ask_qty_unit;                              ///<限价卖数量单位
			int32_t market_bid_qty_upper_limit;                ///<市价买委托数量上限
			int32_t market_bid_qty_lower_limit;                ///<市价买委托数量下限
			int32_t market_bid_qty_unit;                       ///<市价买数量单位
			int32_t market_ask_qty_upper_limit;                ///<市价卖委托数量上限
			int32_t market_ask_qty_lower_limit;                ///<市价卖委托数量上限
			int32_t market_ask_qty_unit;                       ///<市价卖数量单位
			XTP_SECURITY_STATUS security_status;               ///<证券状态
			uint32_t unknown1;                                 ///<保留字段
			uint64_t unknown[3];                               ///<保留字段

		}XTPQFI;


		///新三板全量静态信息
		typedef struct XTPQuoteNQFullInfo {
			XTP_EXCHANGE_TYPE  exchange_id;                     ///<交易所代码
			int32_t unused;                                     ///<预留
			char    ticker[XTP_QUOTE_TICKER_LEN];               ///<证券代码
			char    base_ticker[XTP_QUOTE_TICKER_LEN];          ///<基础证券
			char    ticker_name[XTP_QUOTE_TICKER_NAME_LEN];     ///<证券名称
			char    ticker_abbr_en[XTP_QUOTE_TICKER_NAME_LEN];  ///<英文简称
			XTP_QUALIFICATION_TYPE ticker_qualification_class;  ///<合约适当性类别
			XTP_SECURITY_TYPE      security_type;               ///<合约详细类型，目前均为255
			int32_t trade_unit;                                 ///<交易单位
			int32_t hang_out_date;                              ///<挂牌日期
			int32_t value_date;                                 ///<起息日期
			int32_t maturity_date;                              ///<到期日
			int32_t per_limit_vol;                              ///<每笔限量
			int32_t buy_vol_unit;                               ///<买数量单位
			int32_t sell_vol_unit;                              ///<卖数量单位
			int32_t mini_declared_vol;                          ///<最小申报数量
			int32_t limit_price_attr;                           ///<限价参数性质
			int32_t market_maker_quantity;                      ///<做市商数量
			double  price_gear;                                 ///<价格档位
			double  first_limit_trans;                          ///<首笔交易限价参数
			double  subsequent_limit_trans;                     ///<后续交易限价参数
			double  limit_upper_price;                          ///<涨停价格
			double  limit_lower_price;                          ///<跌停价格
			double  block_trade_upper;                          ///<大宗交易价格上限(预留，默认0)
			double  block_trade_lower;                          ///<大宗交易价格下限(预留，默认0)
			double  convert_into_ration;                        ///<折合比例
			XTP_TRADE_STATUS        trade_status : 8;           ///<交易状态
			XTP_SECURITY_LEVEL      security_level : 8;         ///<证券级别
			XTP_TRADE_TYPE          trade_type : 8;             ///<交易类型
			XTP_SUSPEND_FLAG        suspend_flag : 8;           ///<停牌标志
			XTP_EX_DIVIDEND_FLAG    ex_dividend_flag : 8;       ///<除权除息标志
			XTP_SECURITY_LAYER_TYPE layer_type : 8;             ///<分层信息
			char    reserved1[2];                               ///<保留字段
			char    industry_type[6];                           ///<行业种类
			char    currency_type[3];                           ///<货币种类
			char    trade_places[3];                            ///<交易场所 预留
			char    is_rzbd;                                    ///<是否融资标的 Y是 N否
			char    is_rqbd;                                    ///<是否融券标的 Y是 N否
			char    is_drrz;                                    ///<是否当日可融资 Y是 N否
			char    is_drrq;                                    ///<是否当日可融券 Y是 N否
			uint64_t unknown[3];                                ///<保留字段
		}XTPNQFI;

		///上海指数通快照行情
		typedef struct XTPIndexPress {

			// 代码
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			char    ticker[XTP_QUOTE_TICKER_LEN];
			///交易所代码(无意义)
			XTP_EXCHANGE_TYPE exchange_id;
			///数据来源, 7=指数通.
			char    data_source;
			char    unused[3];

			/// 时间类，格式为YYYYMMDDHHMMSSsss
			int64_t data_time;

			// 价格
			///最新价
			double  last_price;
			///昨收盘
			double  pre_close_price;
			///今开盘
			double  open_price;
			///最高价
			double  high_price;
			///最低价
			double  low_price;
			///今收盘
			double  close_price;


			// 量额数据
			///总成交量
			int64_t qty;
			///总成交金额
			double  turnover;
			///成交笔数
			int64_t trades_count;
		}XTPIP;

		///深交所港股通快照行情
		typedef struct XTPHKCMarketdata {
			// 代码
			///合约代码（不包含交易所信息），不带空格，以'\0'结尾
			char    ticker[XTP_QUOTE_TICKER_LEN];
			///交易所代码
			XTP_EXCHANGE_TYPE exchange_id;
			///预留
			int32_t unused;

			/// 时间类，格式为YYYYMMDDHHMMSSsss
			int64_t data_time;
			///当前交易状态说明
			char    ticker_status[8];

			///成交笔数
			int64_t trades_count;
			///数量，为总成交量（单位股，与交易所一致）
			int64_t qty;
			///成交金额，为总成交金额（单位元，与交易所一致）
			double  turnover;

			///涨停价
			double  upper_limit_price;
			///跌停价
			double  lower_limit_price;

			// 价格
			///最新价
			double  last_price;
			///昨收盘
			double  pre_close_price;
			///参考价
			double  ref_price;
			///最高价
			double  high_price;
			///最低价
			double  low_price;
			///按盘价/盘后为今收价
			double  nominal_price;

			///买盘上限价 如果处于开盘集合竞价时段，则表示开盘竞价时段的买盘价格上限
			double  bid_upper_price;
			///买盘下限价 如果处于开盘集合竞价时段，则表示开盘竞价时段的买盘价格下限
			double  bid_lower_price;
			///卖盘上限价 如果处于开盘集合竞价时段，则表示开盘竞价时段的卖盘价格上限
			double  ask_upper_price;
			///卖盘下限价 如果处于开盘集合竞价时段，则表示开盘竞价时段的卖盘价格下限
			double  ask_lower_price;

			///冷静期开始时间 大于0表示当前处于触发VCM的冷静期
			int64_t complex_event_start_time;
			///冷静期结束时间 大于0表示当前处于触发VCM的冷静期
			int64_t complex_event_end_time;

			// 买卖盘
			///十档申买价
			double  bid[10];
			///十档申卖价
			double  ask[10];
			///十档申买量
			int64_t bid_qty[10];
			///十档申卖量
			int64_t ask_qty[10];
		} XTPHKCMD;

		///港股通实时额度
		typedef struct XTPHKCRealtimeLimit {
			/// 时间类，格式为YYYYMMDDHHMMSSsss
			int64_t data_time;

			/// 市场代码
			char    market_id[8];
			/// 市场板块代码,预留
			char    market_segment_id[8];

			/// 每日初始额度
			int64_t threshold_amount;
			/// 日中剩余额度
			int64_t pos_amt;
			/// 额度状态:
			/// 1=额度不可用; pos_amt = 0.0000
			/// 2=额度可用(额度可用且低于当日初始额度的30%时); pos_amt = 当前实际剩余额度
			/// 3=额度充足(额度可用且大于或等于当日初始额度的30%时); pos_amt = 0.0000
			char    amount_status;
			char    unused[7];
		} XTPHKCRL;

        // 港股通静态信息
        typedef struct XTPHKCStaticInfo {
            char    ticker[XTP_QUOTE_TICKER_LEN];               ///<证券代码
            char    ticker_name[XTP_QUOTE_TICKER_NAME_LEN];     ///<证券名称
            XTP_EXCHANGE_TYPE exchange_id;                      ///<交易所代码(XTP_EXCHANGE_HK)
            char    unknown[4];                                 ///<预留
        } XTPHKCSI;

        // 指数通静态信息
        typedef struct XTPIndexPressStaticInfo {
            char    ticker[XTP_QUOTE_TICKER_LEN];               ///<证券代码
            char    ticker_name[XTP_QUOTE_TICKER_NAME_LEN];     ///<证券名称
            XTP_QUOTE_MKT_CODE_TYPE market_code;                ///<市场代码
            char    unknown[7];                                 ///<预留
        } XTPIPSI;
	}//API
}//XTPX
#pragma pack(pop)


#endif // !_XQUOTE_NG_API_STRUCT_H_

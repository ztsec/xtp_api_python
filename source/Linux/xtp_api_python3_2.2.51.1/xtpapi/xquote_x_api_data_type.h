
//////////////////////////////////////////////
///@author 中泰证券股份有限公司
///@file xquote_x_api_data_type.h
///@brief 定义XTP4.0行情使用的数据基本类型
//////////////////////////////////////////////

#ifndef _XQUOTE_NG_API_DATA_TYPE_H_
#define _XQUOTE_NG_API_DATA_TYPE_H_

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

namespace XTPX {

	namespace API {

		/// 存放证券代码的字符串长度
		constexpr int32_t XTP_QUOTE_TICKER_LEN = 16;

		/// 存放证券名称的字符串长度
		constexpr int32_t XTP_QUOTE_TICKER_NAME_LEN = 64;

		/// 股票逐笔查询最大序列号
		constexpr int64_t XTP_MAX_TBT_TICKER_QUERY_SEQ = (1024 * 1024 * 1024);

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_EXCHANGE_TYPE是交易所类型
		////////////////////////////////////////////////////////////////////////
		typedef uint32_t XTP_EXCHANGE_TYPE;
		///<上证
		constexpr uint32_t XTP_EXCHANGE_SH = 1;
		///<深证
		constexpr uint32_t XTP_EXCHANGE_SZ = 2;
		///<新三板，全国中小企业股份转让系统
		constexpr uint32_t XTP_EXCHANGE_NQ = 3;
		///<港交所
		constexpr uint32_t XTP_EXCHANGE_HK = 4;
		///<不存在的交易所类型
		constexpr uint32_t XTP_EXCHANGE_UNKNOWN = 5;


		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_TBT_TYPE是一个逐笔回报类型
		/////////////////////////////////////////////////////////////////////////
		typedef uint32_t XTP_TBT_TYPE;
		///<逐笔委托
		constexpr uint32_t XTP_TBT_ENTRUST = 1;
		///<逐笔成交
		constexpr uint32_t  XTP_TBT_TRADE = 2;
		///<逐笔状态订单
		constexpr uint32_t  XTP_TBT_STATE = 3;


		//////////////////////////////////////////////////////////////////////////
		///@brief XTP_MARKETDATA_TYPE_V2是快照数据中的Union类型
		//////////////////////////////////////////////////////////////////////////
		typedef uint32_t  XTP_MARKETDATA_TYPE_V2;
		// 指数
		constexpr uint32_t XTP_MARKETDATA_V2_INDEX = 0;
		// 期权
		constexpr uint32_t XTP_MARKETDATA_V2_OPTION = 1;
		// 现货(股票/基金等)
		constexpr uint32_t XTP_MARKETDATA_V2_ACTUAL = 2;
		// 债券
		constexpr uint32_t XTP_MARKETDATA_V2_BOND = 3;



		//////////////////////////////////////////////////////////////////////////
		///@brief XTP_QUOTE_TICKER_TYPE证券类型
		//////////////////////////////////////////////////////////////////////////
		typedef uint32_t  XTP_QUOTE_TICKER_TYPE;
		///<普通股票
		constexpr uint32_t  XTP_QUOTE_TICKER_TYPE_STOCK = 0;
		///<指数
		constexpr uint32_t XTP_QUOTE_TICKER_TYPE_INDEX = 1;
		///<基金
		constexpr uint32_t XTP_QUOTE_TICKER_TYPE_FUND = 2;
		///<债券
		constexpr uint32_t XTP_QUOTE_TICKER_TYPE_BOND = 3;
		///<期权
		constexpr uint32_t XTP_QUOTE_TICKER_TYPE_OPTION = 4;
		///<科创板股票（上海）
		constexpr uint32_t XTP_QUOTE_TICKER_TYPE_TECH_STOCK = 5;
		///<未知类型
		constexpr uint32_t XTP_QUOTE_TICKER_TYPE_UNKNOWN = 6;


		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_SECURITY_TYPE是一个证券详细分类枚举类型
		/////////////////////////////////////////////////////////////////////////
		typedef uint32_t  XTP_SECURITY_TYPE;
		/// 主板股票
		constexpr uint32_t XTP_SECURITY_MAIN_BOARD = 0;
		/// 中小板股票
		constexpr uint32_t XTP_SECURITY_SECOND_BOARD = 1;
		/// 创业板股票
		constexpr uint32_t XTP_SECURITY_STARTUP_BOARD = 2;
		/// 指数
		constexpr uint32_t XTP_SECURITY_INDEX = 3;
		/// 科创板股票(上海)
		constexpr uint32_t XTP_SECURITY_TECH_BOARD = 4;
		/// 国债
		constexpr uint32_t XTP_SECURITY_STATE_BOND = 5;
		/// 企业债
		constexpr uint32_t XTP_SECURITY_ENTERPRICE_BOND = 6;
		/// 公司债
		constexpr uint32_t XTP_SECURITY_COMPANEY_BOND = 7;
		/// 转换债券
		constexpr uint32_t XTP_SECURITY_CONVERTABLE_BOND = 8;
		/// 国债逆回购
		constexpr uint32_t XTP_SECURITY_NATIONAL_BOND_REVERSE_REPO = 12;
		/// 本市场股票 ETF
		constexpr uint32_t XTP_SECURITY_ETF_SINGLE_MARKET_STOCK = 14;
		/// 跨市场股票 ETF
		constexpr uint32_t XTP_SECURITY_ETF_INTER_MARKET_STOCK = 15;
		/// 跨境股票 ETF
		constexpr uint32_t XTP_SECURITY_ETF_CROSS_BORDER_STOCK = 16;
		/// 本市场实物债券 ETF
		constexpr uint32_t XTP_SECURITY_ETF_SINGLE_MARKET_BOND = 17;
		/// 现金债券ETF
		constexpr uint32_t XTP_SECURITY_TYPE_ETF_CASH_BOND = 18;
		/// 黄金 ETF
		constexpr uint32_t XTP_SECURITY_ETF_GOLD = 19;
		/// 商品期货ETF
		constexpr uint32_t XTP_SECURITY_ETF_COMMODITY_FUTURES = 22;
		/// 上市开放式基金LOF
		constexpr uint32_t XTP_SECURITY_LOF = 23;
		/// 分级基金子基金
		constexpr uint32_t XTP_SECURITY_STRUCTURED_FUND_CHILD = 24;
		/// 深交所仅申赎基金
		constexpr uint32_t XTP_SECURITY_SZSE_RECREATION_FUND = 26;
		/// 个股期权
		constexpr uint32_t XTP_SECURITY_STOCK_OPTION = 29;
		/// ETF期权
		constexpr uint32_t XTP_SECURITY_ETF_OPTION = 30;
		/// REITs基金
		constexpr uint32_t XTP_SECURITY_REITS = 38;
		/// 配股
		constexpr uint32_t XTP_SECURITY_ALLOTMENT = 100;
		/// 上交所申赎型货币基金
		constexpr uint32_t XTP_SECURITY_MONETARY_FUND_SHCR = 110;
		/// 上交所交易型货币基金
		constexpr uint32_t XTP_SECURITY_MONETARY_FUND_SHTR = 111;
		/// 深交所货币基金
		constexpr uint32_t XTP_SECURITY_MONETARY_FUND_SZ = 112;
		/// 跨境LOF
		constexpr uint32_t XTP_SECURITY_LOF_CROSS_BORDER = 113;
		/// 其他
		constexpr uint32_t XTP_SECURITY_OTHERS = 255;


		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_QUALIFICATION_TYPE是一个证券适当性枚举类型
		/////////////////////////////////////////////////////////////////////////
		typedef uint32_t   XTP_QUALIFICATION_TYPE;
		///<公众投资者，合格投资者与机构投资者均可
		constexpr uint32_t XTP_QUALIFICATION_PUBLIC = 0;
		///<仅合格投资者与公众投资者
		constexpr uint32_t XTP_QUALIFICATION_COMMON = 1;
		///<仅限机构投资者
		constexpr uint32_t XTP_QUALIFICATION_ORGANIZATION = 2;
		///<未知，期权等可能为此种类型
		constexpr uint32_t XTP_QUALIFICATION_UNKNOWN = 3;


		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_SECURITY_STATUS是一个证券状态枚举类型
		/////////////////////////////////////////////////////////////////////////
		typedef uint32_t   XTP_SECURITY_STATUS;
		///< 风险警示板
		constexpr uint32_t XTP_SECURITY_STATUS_ST = 0;
		///< 首日上市
		constexpr uint32_t XTP_SECURITY_STATUS_N_IPO = 1;
		///< 普通
		constexpr uint32_t XTP_SECURITY_STATUS_COMMON = 2;
		///< 恢复上市
		constexpr uint32_t XTP_SECURITY_STATUS_RESUME = 3;
		///< 退市整理期
		constexpr uint32_t XTP_SECURITY_STATUS_DELISTING = 10;
		///< 其他
		constexpr uint32_t XTP_SECURITY_STATUS_OTHERS = 255;


		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_TRADE_STATUS是一个交易状态枚举类型
		/////////////////////////////////////////////////////////////////////////
		typedef uint8_t    XTP_TRADE_STATUS;
		///< 未知状态
		constexpr uint8_t XTP_TRADE_STATUS_UNKNOW = 0;
		///< 正常状态
		constexpr uint8_t XTP_TRADE_STATUS_N = 1;
		///< 首日挂牌
		constexpr uint8_t XTP_TRADE_STATUS_Y = 2;
		///< 新增股票挂牌交易
		constexpr uint8_t XTP_TRADE_STATUS_D = 3;
		///< 询价
		constexpr uint8_t XTP_TRADE_STATUS_I = 4;
		///< 申购
		constexpr uint8_t XTP_TRADE_STATUS_F = 5;

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_SECURITY_LEVEL是一个证券级别枚举类型
		/////////////////////////////////////////////////////////////////////////
		typedef uint8_t  XTP_SECURITY_LEVEL;
		///< 未知类型
		constexpr uint8_t XTP_SECURITY_LEVEL_UNKNOW = 0;
		///< 挂牌公司股票
		constexpr uint8_t XTP_SECURITY_LEVEL_T = 1;
		///< 两网公司及退市股票
		constexpr uint8_t XTP_SECURITY_LEVEL_B = 2;
		///< 仅提供行权功能的期权
		constexpr uint8_t XTP_SECURITY_LEVEL_O = 3;
		///< 持有人数存在200人限制的证券
		constexpr uint8_t XTP_SECURITY_LEVEL_P = 4;
		///< 其他类型的业务
		constexpr uint8_t XTP_SECURITY_LEVEL_R = 5;
		///< 发行业务
		constexpr uint8_t XTP_SECURITY_LEVEL_F = 6;


		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_TRADE_TYPE是一个交易类型枚举类型
		/////////////////////////////////////////////////////////////////////////
		typedef  uint8_t   XTP_TRADE_TYPE;
		///< 未知类型
		constexpr uint8_t XTP_TRADE_TYPE_UNKNOW = 0;
		///< 协议交易方式
		constexpr uint8_t XTP_TRADE_TYPE_T = 1;
		///< 做市交易方式
		constexpr uint8_t XTP_TRADE_TYPE_M = 2;
		///< 集合竞价+连续交易方式
		constexpr uint8_t XTP_TRADE_TYPE_B = 3;
		///< 集合竞价交易方式
		constexpr uint8_t XTP_TRADE_TYPE_C = 4;
		///< 发行方式
		constexpr uint8_t XTP_TRADE_TYPE_P = 5;
		///< 其他类型
		constexpr uint8_t XTP_TRADE_TYPE_O = 6;


		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_SUSPEND_FLAG是一个停牌标志枚举类型
		/////////////////////////////////////////////////////////////////////////
		typedef  uint8_t  XTP_SUSPEND_FLAG;
		///< 未知状态
		constexpr uint8_t XTP_SUSPEND_FLAG_UNKNOW = 0;
		///< 正常交易
		constexpr uint8_t XTP_SUSPEND_FLAG_F = 1;
		///< 停牌，不接受申报
		constexpr uint8_t XTP_SUSPEND_FLAG_T = 2;
		///< 停牌，接受申报
		constexpr uint8_t XTP_SUSPEND_FLAG_H = 3;

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_EX_DIVIDEND_FLAG是一个除权除息标志枚举类型
		/////////////////////////////////////////////////////////////////////////
		typedef  uint8_t    XTP_EX_DIVIDEND_FLAG;
		///< 未知状态
		constexpr uint8_t XTP_EX_DIVIDEND_FLAG_UNKNOW = 0;
		///< 正常状态
		constexpr uint8_t XTP_EX_DIVIDEND_FLAG_N = 1;
		///< 除权
		constexpr uint8_t XTP_EX_DIVIDEND_FLAG_E = 2;
		///< 除息
		constexpr uint8_t XTP_EX_DIVIDEND_FLAG_D = 3;
		///< 除权除息
		constexpr uint8_t XTP_EX_DIVIDEND_FLAG_A = 4;


		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_SECURITY_LAYER_TYPE是一个股转代码分层类型枚举类型
		/////////////////////////////////////////////////////////////////////////
		typedef  uint8_t    XTP_SECURITY_LAYER_TYPE;
		///< 基础层
		constexpr uint8_t XTP_SECURITY_LAYER_BASIC = 0;
		///< 创新层
		constexpr uint8_t XTP_SECURITY_LAYER_INNOVATIVE = 1;
		///< 北交所
		constexpr uint8_t XTP_SECURITY_LAYER_NORTH_EX = 2;
		///< 其他
		constexpr uint8_t XTP_SECURITY_LAYER_OTHER = 8;


		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_QUOTE_DATA_TYPE是行情数据类型 逐笔，快照等
		/////////////////////////////////////////////////////////////////////////
		typedef  uint32_t  XTP_QUOTE_REBUILD_DATA_TYPE;
		///<未知类型
		constexpr uint32_t XTP_QUOTE_REBUILD_UNKNOW = 0;
		///<快照类型
		constexpr uint32_t XTP_QUOTE_REBUILD_MD = 1;
		///<逐笔类型
		constexpr uint32_t XTP_QUOTE_REBUILD_TBT = 2;
		///<指定股票逐笔类型
		constexpr uint32_t XTP_QUOTE_REBUILD_TICKER_TBT = 3;

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_REBUILD_RET_TYPE 实时行情回补返回结果类型
		/////////////////////////////////////////////////////////////////////////
		typedef  uint32_t   XTP_REBUILD_RET_TYPE;
		///<全部数据
		constexpr uint32_t XTP_REBUILD_RET_COMPLETE = 1;
		///<部分数据
		constexpr uint32_t XTP_REBUILD_RET_PARTLY = 2;
		///<没有数据
		constexpr uint32_t XTP_REBUILD_RET_NO_DATA = 3;
		///<参数错误
		constexpr uint32_t XTP_REBUILD_RET_PARAM_ERR = 4;
		///<请求频繁
		constexpr uint32_t XTP_REBUILD_RET_FREQUENTLY = 5;

		/////////////////////////////////////////////////////////////////////////
		///@brief XTP_QUOTE_MKT_CODE_TYPE 指数通的市场代码
		/////////////////////////////////////////////////////////////////////////
		typedef  uint8_t  XTP_QUOTE_MKT_CODE_TYPE;
		///<全球
		constexpr uint8_t XTP_QUOTE_MKT_CODE_GLB = 0;
		///<上交所
		constexpr uint8_t XTP_QUOTE_MKT_CODE_SH = 1;
		///<深交所
		constexpr uint8_t XTP_QUOTE_MKT_CODE_SZ = 2;
		///<沪深
		constexpr uint8_t XTP_QUOTE_MKT_CODE_HS = 3;
		///<香港
		constexpr uint8_t XTP_QUOTE_MKT_CODE_HK = 4;
		///<亚太
		constexpr uint8_t XTP_QUOTE_MKT_CODE_APAC = 5;
		///<债券市场
		constexpr uint8_t XTP_QUOTE_MKT_CODE_BOND = 6;
		///<其它
		constexpr uint8_t XTP_QUOTE_MKT_CODE_OTHER = 9;
        ///未知
		constexpr uint8_t XTP_QUOTE_MKT_CODE_UNKNOWN = 100;
	}//API
}//XTPX

#endif //_XQUOTE_NG_API_DATA_TYPE_H_

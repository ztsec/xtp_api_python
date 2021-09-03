# encoding: UTF-8

import os
from time import sleep
from vnxtpquote import *


def printFuncName(*args):
    """"""
    print('*' * 50)
    print(args)
    print('*' * 50)


class TestApi(QuoteApi):

    def __init__(self):
        """Constructor"""
        super(TestApi, self).__init__()
    #当客户端与行情后台通信连接断开时，该方法被调用。
    #@param reason 错误原因，请与错误代码表对应
    #@remark api不会自动重连，当断线发生时，请用户自行选择后续操作。可以在此函数中调用Login重新登录。注意用户重新登录后，需要重新订阅行情
    def onDisconnected(self, reason):
        """"""
        printFuncName('onDisconnected', reason)

    #错误应答
    #@param data 当服务器响应发生错误时的具体的错误代码和错误信息，当data为空，或者data.error_id为0时，表明没有错误
    #@remark 此函数只有在服务器发生错误时才会调用，一般无需用户处理
    def onError(self, data):
        """"""
        printFuncName('onError',data)

    #订阅行情应答，包括股票、指数和期权
    #@param data 详细的合约订阅情况
    #@param error 订阅合约发生错误时的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param last 是否此次订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@remark 每条订阅的合约均对应一条订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onSubMarketData(self, data, error, last):
        """"""
        printFuncName('onSubMarketData', data, error)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码（不包含交易所信息）例如"600000"
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #退订行情应答，包括股票、指数和期权
    #@param data 详细的合约取消订阅情况
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param last 是否此次取消订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@remark 每条取消订阅的合约均对应一条取消订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onUnSubMarketData(self, data, error, last):
        """"""
        printFuncName('onUnSubMarketData', data, error, last)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码（不包含交易所信息）例如"600000"
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #深度行情通知，包含买一卖一队列
    #@param data 行情数据
    #@param bid1_qty_list 买一队列数据
    #@param bid1_counts 买一队列的有效委托笔数
    #@param max_bid1_count 买一队列总委托笔数
    #@param ask1_qty_list 卖一队列数据
    #@param ask1_count 卖一队列的有效委托笔数
    #@param max_ask1_count 卖一队列总委托笔数
    #@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onDepthMarketData(self, data,bid1_qty_list,bid1_counts,max_bid1_count,ask1_qty_list,ask1_count,max_ask1_count):
        """"""
        printFuncName('onDepthMarketData', data,bid1_qty_list,bid1_counts,max_bid1_count,ask1_qty_list,ask1_count,max_ask1_count)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码（不包含交易所信息）
        print("data['last_price']:",data['last_price'])#最新价
        print("data['pre_close_price']:",data['pre_close_price'])#昨收盘价
        print("data['open_price']:",data['open_price'])#今开盘价
        print("data['high_price']:",data['high_price'])#最高价
        print("data['low_price']:",data['low_price'])#最低价
        print("data['close_price']:",data['close_price'])#今收盘价
        print("data['pre_total_long_positon']:",data['pre_total_long_positon'])#期权数据昨日持仓量
        print("data['total_long_positon']:",data['total_long_positon'])#持仓量
        print("data['pre_settl_price']:",data['pre_settl_price'])#昨日结算价
        print("data['settl_price']:",data['settl_price'])#今日结算价
        print("data['upper_limit_price']:",data['upper_limit_price'])#涨停价
        print("data['lower_limit_price']:",data['lower_limit_price'])#跌停价
        print("data['pre_delta']:",data['pre_delta'])#预留
        print("data['curr_delta']:",data['curr_delta'])#预留
        print("data['data_time']:",data['data_time'])#时间类，格式为YYYYMMDDHHMMSSsss
        print("data['qty']:",data['qty'])#数量，为总成交量（单位股，与交易所一致）
        print("data['turnover']:",data['turnover'])#成交金额，为总成交金额（单位元，与交易所一致）
        print("data['avg_price']:",data['avg_price'])#当日均价
        print("data['trades_count']:",data['trades_count'])#成交笔数
        print("data['ticker_status']:",data['ticker_status'])#当前交易状态说明
        print("data['ask']:",data['ask'])#十档申卖价
        print("data['bid']:",data['bid'])#十档申买价
        print("data['bid_qty']:",data['bid_qty'])#十档申买量
        print("data['ask_qty']:",data['ask_qty'])#十档申卖量
        print("data['data_type']:",data['data_type'])# 0-现货(股票/基金/债券等) 1-期权
        if data['data_type'] == 0 :
            print("data['total_bid_qty']:",data['total_bid_qty'])#委托买入总量
            print("data['total_ask_qty']:",data['total_ask_qty'])#委托卖出总量
            print("data['ma_bid_price']:",data['ma_bid_price'])#加权平均委买价格
            print("data['ma_ask_price']:",data['ma_ask_price'])#加权平均委卖价格
            print("data['ma_bond_bid_price']:",data['ma_bond_bid_price'])#债券加权平均委买价格
            print("data['ma_bond_ask_price']:",data['ma_bond_ask_price'])#债券加权平均委卖价格
            print("data['yield_to_maturity']:",data['yield_to_maturity'])#债券到期收益率
            print("data['iopv']:",data['iopv'])#基金实时参考净值
            print("data['etf_buy_count']:",data['etf_buy_count'])#ETF申购笔数(SH)
            print("data['etf_sell_count']:",data['etf_sell_count'])#ETF赎回笔数(SH)
            print("data['etf_buy_qty']:",data['etf_buy_qty'])# ETF申购数量(SH)
            print("data['etf_buy_money']:",data['etf_buy_money'])#ETF申购金额(SH)
            print("data['etf_sell_qty']:",data['etf_sell_qty'])#ETF赎回数量(SH)
            print("data['etf_sell_money']:",data['etf_sell_money'])#ETF赎回金额(SH)
            print("data['total_warrant_exec_qty']:",data['total_warrant_exec_qty'])#权证执行的总数量(SH)
            print("data['warrant_lower_price']:",data['warrant_lower_price'])#权证跌停价格（元）(SH)
            print("data['warrant_upper_price']:",data['warrant_upper_price'])#权证涨停价格（元）(SH)
            print("data['cancel_buy_count']:",data['cancel_buy_count'])#买入撤单笔数(SH)
            print("data['cancel_sell_count']:",data['cancel_sell_count'])#卖出撤单笔数(SH)
            print("data['cancel_buy_qty']:",data['cancel_buy_qty'])#买入撤单数量(SH)
            print("data['cancel_sell_qty']:",data['cancel_sell_qty'])#卖出撤单数量(SH)
            print("data['cancel_buy_money']:",data['cancel_buy_money'])# 买入撤单金额(SH)
            print("data['cancel_sell_money']:",data['cancel_sell_money'])# 卖出撤单金额(SH)
            print("data['total_buy_count']:",data['total_buy_count'])#买入总笔数(SH)
            print("data['total_sell_count']:",data['total_sell_count'])#卖出总笔数(SH)
            print("data['duration_after_buy']:",data['duration_after_buy'])#买入委托成交最大等待时间(SH)
            print("data['duration_after_sell']:",data['duration_after_sell'])#卖出委托成交最大等待时间(SH)
            print("data['num_bid_orders']:",data['num_bid_orders'])#买方委托价位数(SH)
            print("data['num_ask_orders']:",data['num_ask_orders'])#卖方委托价位数(SH)
            print("data['pre_iopv']:",data['pre_iopv'])#基金T-1日净值(SZ)
            print("data['r1']:",data['r1'])#预留
            print("data['r2']:",data['r2'])#预留
        else :
            print("data['auction_price']:",data['auction_price'])# 波段性中断参考价(SH)
            print("data['auction_qty']:",data['auction_qty'])# 波段性中断集合竞价虚拟匹配量(SH)
            print("data['last_enquiry_time']:",data['last_enquiry_time'])# 最近询价时间(SH)

    #订阅行情订单簿应答，包括股票、指数和期权
    #@param data 详细的合约订阅情况
    #@param error 订阅合约发生错误时的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param last 是否此次订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@remark 每条订阅的合约均对应一条订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onSubOrderBook(self, data, error, last):
        """"""
        printFuncName('onSubOrderBook', data, error, last)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码（不包含交易所信息）例如"600000"
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #退订行情订单簿应答，包括股票、指数和期权
    #@param data 详细的合约取消订阅情况
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param last 是否此次取消订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@remark 每条取消订阅的合约均对应一条取消订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onUnSubOrderBook(self, data, error, last):
        """"""
        printFuncName('onUnSubOrderBook', data, error, last)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码（不包含交易所信息）例如"600000"
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #行情订单簿通知，包括股票、指数和期权
    #@param data 行情订单簿数据，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onOrderBook(self, data):
        """"""
        printFuncName('onOrderBook', data)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码
        print("data['data_time']:",data['data_time'])#时间类
        print("data['last_price']:",data['last_price'])#最新价
        print("data['qty']:",data['qty'])#数量，为总成交量
        print("data['turnover']:",data['turnover'])#成交金额，为总成交金额
        print("data['trades_count']:",data['trades_count'])#成交笔数
        print("data['ask']:",data['ask'])#十档申卖价
        print("data['bid']:",data['bid'])#十档申买价
        print("data['bid_qty']:",data['bid_qty'])#十档申买量
        print("data['ask_qty']:",data['ask_qty'])#十档申卖量
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #订阅逐笔行情应答，包括股票、指数和期权
    #@param data 详细的合约订阅情况
    #@param error 订阅合约发生错误时的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param last 是否此次订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@remark 每条订阅的合约均对应一条订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onSubTickByTick(self, data, error, last):
        """"""
        printFuncName('onSubTickByTick', data, error, last)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #退订逐笔行情应答，包括股票、指数和期权
    #@param data 详细的合约取消订阅情况
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param last 是否此次取消订阅的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@remark 每条取消订阅的合约均对应一条取消订阅应答，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onUnSubTickByTick(self, data, error, last):
        """"""
        printFuncName('onUnSubTickByTick', data, error, last)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #逐笔行情通知，包括股票、指数和期权
    #@param data 逐笔行情数据，包括逐笔委托和逐笔成交，此为共用结构体，需要根据type来区分是逐笔委托还是逐笔成交，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onTickByTick(self, data):
        """"""
        printFuncName('onTickByTick', data)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码
        print("data['data_time']:",data['data_time'])#委托时间 or 成交时间
        print("data['type']:",data['type'])#1委托 or 2成交
        if data['type'] == 1 :
            print("data['channel_no']:",data['channel_no'])#频道代码
            print("data['seq']:",data['seq'])#委托序号
            print("data['price']:",data['price'])#委托价格
            print("data['qty']:",data['qty'])#委托数量
            print("data['side']:",data['side'])#'1':买; '2':卖; 'G':借入; 'F':出借
            print("data['ord_type']:",data['ord_type'])#订单类别: '1': 市价; '2': 限价; 'U': 本方最优
            print("data['order_no']:",data['order_no'])#SH: 原始订单号,SZ: 无意义
        else :
            print("data['channel_no']:",data['channel_no'])#频道代码
            print("data['seq']:",data['seq'])#委托序号(在同一个channel_no内唯一，从1开始连续)
            print("data['price']:",data['price'])#成交价格
            print("data['qty']:",data['qty'])#成交量
            print("data['money']:",data['money'])#成交金额(仅适用上交所)
            print("data['bid_no']:",data['bid_no'])#买方订单号
            print("data['ask_no']:",data['ask_no'])#卖方订单号
            print("data['trade_flag']:",data['trade_flag'])#SH: 内外盘标识('B':主动买; 'S':主动卖; 'N':未知)SZ: 成交标识('4':撤; 'F':成交)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #订阅全市场的股票行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllMarketData(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllMarketData', exchange_id,error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #退订全市场的股票行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllMarketData(self, exchange_id,error):
        """"""
        printFuncName('onUnSubscribeAllMarketData',exchange_id,error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #订阅全市场的股票行情订单簿应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllOrderBook(self, exchange_id,error):
        """"""
        printFuncName('onSubscribeAllOrderBook', exchange_id,error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #退订全市场的股票行情订单簿应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllOrderBook(self, exchange_id,error):
        """"""
        printFuncName('onUnSubscribeAllOrderBook', exchange_id,error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #订阅全市场的股票逐笔行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllTickByTick(self, exchange_id,error):
        """"""
        printFuncName('onSubscribeAllTickByTick', exchange_id,error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #退订全市场的股票逐笔行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllTickByTick(self,exchange_id, error):
        """"""
        printFuncName('onUnSubscribeAllTickByTick',exchange_id, error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #查询可交易合约的应答
    #@param data 可交易合约信息
    #@param error 查询可交易合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param last 是否此次查询可交易合约的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    def onQueryAllTickers(self, data, error, last):
        """"""
        printFuncName('onQueryAllTickers', data, error, last)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码
        print("data['ticker_name']:",data['ticker_name'])#合约名称
        print("data['ticker_type']:",data['ticker_type'])#合约类型
        print("data['pre_close_price']:",data['pre_close_price'])#昨收盘
        print("data['upper_limit_price']:",data['upper_limit_price'])#涨停板价
        print("data['lower_limit_price']:",data['lower_limit_price'])#跌停板价
        print("data['price_tick']:",data['price_tick'])#最小变动价位
        print("data['buy_qty_unit']:",data['buy_qty_unit'])#合约最小交易量
        print("data['sell_qty_unit']:",data['sell_qty_unit'])#合约最小交易量
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #查询合约的最新价格信息应答
    #@param data 可交易合约信息
    #@param error 查询可交易合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param last 是否此次查询可交易合约的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    def onQueryTickersPriceInfo(self, data, error, last):
        """"""
        printFuncName('onQueryTickersPriceInfo', data, error, last)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码
        print("data['last_price']:",data['last_price'])#最新价
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #订阅全市场的期权行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllOptionMarketData(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllOptionMarketData',exchange_id, error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #退订全市场的期权行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllOptionMarketData(self,exchange_id, error):
        """"""
        printFuncName('onUnSubscribeAllMarketData', exchange_id,error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #订阅全市场的期权行情订单簿应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllOptionOrderBook(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllOptionOrderBook', exchange_id,error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #退订全市场的期权行情订单簿应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllOptionOrderBook(self,exchange_id, error):
        """"""
        printFuncName('onUnSubscribeAllOptionOrderBook', exchange_id,error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #订阅全市场的期权逐笔行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllOptionTickByTick(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllOptionTickByTick', exchange_id,error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #退订全市场的期权逐笔行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllOptionTickByTick(self,exchange_id,error):
        """"""
        printFuncName('onUnSubscribeAllOptionTickByTick',exchange_id, error)
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])

    #查询合约完整静态信息的应答
    #@param ticker_info 合约完整静态信息
    #@param error_info 查询合约完整静态信息时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
    #@param is_last 是否此次查询合约完整静态信息的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    def onQueryAllTickersFullInfo(self,data, error, last):
        """"""
        printFuncName('onQueryAllTickersFullInfo', data, error, last)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#证券代码
        print("data['ticker_name']:",data['ticker_name'])#证券名称
        print("data['security_type']:",data['security_type'])#合约详细类型
        print("data['ticker_qualification_class']:",data['ticker_qualification_class'])#合约适当性类别
        print("data['is_registration']:",data['is_registration'])#是否注册制(仅适用创业板股票，创新企业股票及存托凭证)
        print("data['is_VIE']:",data['is_VIE'])#是否具有协议控制架构(仅适用创业板股票，创新企业股票及存托凭证)
        print("data['is_noprofit']:",data['is_noprofit'])#是否尚未盈利(仅适用创业板股票，创新企业股票及存托凭证)
        print("data['is_weighted_voting_rights']:",data['is_weighted_voting_rights'])#是否存在投票权差异(仅适用创业板股票，创新企业股票及存托凭证)
        print("data['is_have_price_limit']:",data['is_have_price_limit'])#是否有涨跌幅限制(注：不提供具体幅度，可通过涨跌停价和昨收价来计算幅度)
        print("data['upper_limit_price']:",data['upper_limit_price'])#涨停价（仅在有涨跌幅限制时有效）
        print("data['lower_limit_price']:",data['lower_limit_price'])#跌停价（仅在有涨跌幅限制时有效）
        print("data['pre_close_price']:",data['pre_close_price'])#昨收价
        print("data['price_tick']:",data['price_tick'])#价格最小变动价位
        print("data['bid_qty_upper_limit']:",data['bid_qty_upper_limit'])#限价买委托数量上限
        print("data['bid_qty_lower_limit']:",data['bid_qty_lower_limit'])#限价买委托数量下限
        print("data['bid_qty_unit']:",data['bid_qty_unit'])#限价买数量单位
        print("data['ask_qty_upper_limit']:",data['ask_qty_upper_limit'])#限价卖委托数量上限
        print("data['ask_qty_lower_limit']:",data['ask_qty_lower_limit'])#限价卖委托数量下限
        print("data['ask_qty_unit']:",data['ask_qty_unit'])#限价卖数量单位
        print("data['market_bid_qty_upper_limit']:",data['market_bid_qty_upper_limit'])#市价买委托数量上限
        print("data['market_bid_qty_lower_limit']:",data['market_bid_qty_lower_limit'])#市价买委托数量下限
        print("data['market_bid_qty_unit']:",data['market_bid_qty_unit'])#市价买数量单位
        print("data['market_ask_qty_upper_limit']:",data['market_ask_qty_upper_limit'])#市价卖委托数量上限
        print("data['market_ask_qty_lower_limit']:",data['market_ask_qty_lower_limit'])#市价卖委托数量下限
        print("data['market_ask_qty_unit']:",data['market_ask_qty_unit'])#市价卖数量单位
        print("data['security_status']:",data['security_status'])#证券状态
        print("error['error_id']):",error['error_id'])
        print("error['error_msg']):",error['error_msg'])



if __name__ == '__main__':

    ip = '119.3.103.38'
    port = 6002
    user = 'username'
    password = 'password'
    local_ip = '192.168.1.205'
    #创建QuoteApi
    #@param client_id （必须输入）用于区分同一用户的不同客户端，由用户自定义
    #@param save_file_path （必须输入）存贮订阅信息文件的目录，请设定一个有可写权限的真实存在的路径
    #@param log_level 日志输出级别
    #@return 创建出的UserApi
    #@remark 如果一个账户需要在多个客户端登录，请使用不同的client_id，系统允许一个账户同时登录多个客户端，但是对于同一账户，相同的client_id只能保持一个session连接，后面的登录在前一个session存续期间，无法连接
    api = TestApi()
    api.createQuoteApi(1, os.getcwd(),4)

    # 1.1.6测试函数
    #设置心跳检测时间间隔，单位为秒
    #@param interval 心跳检测时间间隔，单位为秒
    #@remark 此函数必须在Login之前调用
    api.setHeartBeatInterval(2)

    #设置采用UDP方式连接时的接收缓冲区大小
    #@remark 需要在Login之前调用，默认大小和最小设置均为64MB。此缓存大小单位为MB，请输入2的次方数，例如128MB请输入128。
    api.setUDPBufferSize(128)

    #使用UDP接收行情时，设置接收行情线程绑定的cpu
    #@param cpu_no 设置绑定的cpu，例如绑定cpu 0，可以设置0，绑定cpu 2，可以设置2，建议绑定后面的cpu
    #@remark 此函数可不调用，如果调用则必须在Login之前调用，否则不会生效
    api.setUDPRecvThreadAffinity(2)

    #使用UDP接收行情时，设置接收行情线程绑定的cpu集合
    #@param cpu_no_array 设置绑定的cpu集合数组
    #@param count cpu集合数组长度
    #@remark 此函数可不调用，如果调用则必须在Login之前调用，否则不会生效。绑核时，将从数组前面的核开始使用
    cpuRecvList = [{'cpu_no':'2'},{'cpu_no':'5'}]
    api.setUDPRecvThreadAffinityArray(cpuRecvList,2)

    #使用UDP接收行情时，设置解析行情线程绑定的cpu
    #@param cpu_no 设置绑定的cpu，例如绑定cpu 0，可以设置0，绑定cpu 2，可以设置2，建议绑定后面的cpu
    #@remark 此函数可不调用，如果调用则必须在Login之前调用，否则不会生效
    api.setUDPParseThreadAffinity(2)

    #使用UDP接收行情时，设置解析行情线程绑定的cpu集合
    #@param cpu_no_array 设置绑定的cpu集合数组
    #@param count cpu集合数组长度
    #@remark 此函数可不调用，如果调用则必须在Login之前调用，否则不会生效。绑核时，将从数组前面的核开始使用
    cpuParseList = [{'cpu_no':'3'},{'cpu_no':'4'}]
    api.setUDPParseThreadAffinityArray(cpuParseList,2)

    #设定UDP收行情时是否输出异步日志
    #@param flag 是否输出标识，默认为true，如果不想输出“udpseq”开头的异步日志，请设置此参数为false
    #@remark 此函数可不调用，如果调用则必须在Login之前调用，否则不会生效
    api.setUDPSeqLogOutPutFlag(1)

    #用户登录请求
    #@return 登录是否成功，“0”表示登录成功，“-1”表示连接服务器出错，此时用户可以调用GetApiLastError()来获取错误代码，“-2”表示已存在连接，不允许重复登录，如果需要重连，请先logout，“-3”表示输入有错误
    #@param ip 服务器ip地址，类似“127.0.0.1”
    #@param port 服务器端口号
    #@param user 登陆用户名
    #@param password 登陆密码
    #@param sock_type “1”代表TCP，“2”代表UDP
    #@param local_ip 本地网卡地址，类似“127.0.0.1”
    #@remark 此函数为同步阻塞式，不需要异步等待登录成功，当函数返回即可进行后续操作，此api只能有一个连接
    retLogin = api.login(ip, port, user, password, 1,local_ip)
    printFuncName('login', retLogin)

    #获取API的系统错误
    #@return 返回的错误信息，可以在Login、Logout、订阅、取消订阅失败时调用，获取失败的原因
    #@remark 可以在调用api接口失败时调用，例如login失败时
    getApiLastError = api.getApiLastError()
    printFuncName("getApiLastError", getApiLastError)

    #获取API的发行版本号
    #@return 返回api发行版本号
    getApiVersion = api.getApiVersion()
    printFuncName("getApiVersion", getApiVersion)

    #获取当前交易日
    #@return 获取到的交易日
    #@remark 只有登录成功后,才能得到正确的交易日
    getTradingDay = api.getTradingDay()
    printFuncName("getTradingDay", getTradingDay)

    tickerList = [{'ticker':'000001'},{'ticker':'000002'},{'ticker':'000004'}]
    count = 3

    #订阅行情，包括股票、指数和期权。
    #@return 订阅接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param tickerList 合约ID数组 
    #@param count 要订阅/退订行情的合约个数
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情
    retSubscribeMarketData = api.subscribeMarketData(tickerList,count, 2)
    printFuncName('subscribeMarketData', retSubscribeMarketData)

    #退订行情，包括股票、指数和期权。
    #@return 取消订阅接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param tickerList 合约ID数组
    #@param count 要订阅/退订行情的合约个数
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 可以一次性取消订阅同一证券交易所的多个合约，需要与订阅行情接口配套使用
    sleep(1)
    retUnSubscribeMarketData=api.unSubscribeMarketData(tickerList,count, 1)
    printFuncName('unSubscribeMarketData', retSubscribeMarketData)

    #订阅行情订单簿，包括股票、指数和期权。
    #@return 订阅行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param tickerList 合约ID数组 
    #@param count 要订阅/退订行情订单簿的合约个数
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情(仅支持深交所)
    sleep(1)
    retSubscribeOrderBook = api.subscribeOrderBook(tickerList,count, 2)
    printFuncName('subscribeOrderBook', retSubscribeOrderBook)

    #退订行情订单簿，包括股票、指数和期权。
    #@return 取消订阅行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param tickerList 合约ID数组
    #@param count 要订阅/退订行情订单簿的合约个数
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 可以一次性取消订阅同一证券交易所的多个合约，需要与订阅行情订单簿接口配套使用
    sleep(1)
    retUnSubscribeOrderBook = api.unSubscribeOrderBook(tickerList,count, 2)
    printFuncName('unSubscribeOrderBook', retUnSubscribeOrderBook)

    #订阅逐笔行情，包括股票、指数和期权。
    #@return 订阅逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param tickerList 合约ID数组
    #@param count 要订阅/退订行情订单簿的合约个数
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 可以一次性订阅同一证券交易所的多个合约，无论用户因为何种问题需要重新登录行情服务器，都需要重新订阅行情
    sleep(1)
    retSubscribeTickByTick = api.subscribeTickByTick(tickerList,count, 2)
    printFuncName('subscribeTickByTick', retSubscribeTickByTick)

    #退订逐笔行情，包括股票、指数和期权。
    #@return 取消订阅逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param tickerList 合约ID数组 
    #@param count 要订阅/退订行情订单簿的合约个数
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 可以一次性取消订阅同一证券交易所的多个合约，需要与订阅逐笔行情接口配套使用
    sleep(1)
    retUnSubscribeTickByTick = api.unSubscribeTickByTick(tickerList,count, 2)
    printFuncName('unSubscribeTickByTick', retUnSubscribeTickByTick)

    #订阅全市场的股票行情
    #@return 订阅全市场行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与全市场退订行情接口配套使用
    sleep(1)
    retSubscribeAllMarketData = api.subscribeAllMarketData(2)
    printFuncName('subscribeAllMarketData', retSubscribeAllMarketData)

    #退订全市场的股票行情
    #@return 退订全市场行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与订阅全市场行情接口配套使用
    sleep(1)
    retUnSubscribeAllMarketData = api.unSubscribeAllMarketData(3)
    printFuncName('unSubscribeAllMarketData', retUnSubscribeAllMarketData)

    #订阅全市场的股票行情订单簿
    #@return 订阅全市场行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与全市场退订行情订单簿接口配套使用
    sleep(1)
    retSubscribeAllOrderBook = api.subscribeAllOrderBook(1)
    printFuncName('subscribeAllOrderBook', retSubscribeAllOrderBook)

    #退订全市场的股票行情订单簿
    #@return 退订全市场行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与订阅全市场行情订单簿接口配套使用
    sleep(1)
    retUnSubscribeAllOrderBook = api.unSubscribeAllOrderBook(1)
    printFuncName('unSubscribeAllOrderBook', retUnSubscribeAllOrderBook)

    #订阅全市场的股票逐笔行情
    #@return 订阅全市场逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与全市场退订逐笔行情接口配套使用
    sleep(1)
    retSubscribeAllTickByTick = api.subscribeAllTickByTick(1)
    printFuncName('subscribeAllTickByTick', retSubscribeAllTickByTick)

    #退订全市场的股票逐笔行情
    #@return 退订全市场逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与订阅全市场逐笔行情接口配套使用
    sleep(1)
    retUnSubscribeAllTickByTick = api.unSubscribeAllTickByTick(1)
    printFuncName('unSubscribeAllTickByTick', retUnSubscribeAllTickByTick)

    #获取当前交易日可交易合约
    #@return 查询是否成功，“0”表示查询成功，非“0”表示查询不成功
    #@param exchange_id 交易所代码
    sleep(1)
    retQueryAllTickers = api.queryAllTickers(1)
    printFuncName('queryAllTickers', retQueryAllTickers)

    #获取合约的最新价格信息
    #@return 查询是否成功，“0”表示查询成功，非“0”表示查询不成功
    #@param tickerList 合约ID数组  
    #@param count 要查询的合约个数
    #@param exchange_id 交易所代码
    sleep(1)
    retQueryTickersPriceInfo = api.queryTickersPriceInfo(tickerList,count, 1)
    printFuncName('queryTickersPriceInfo', retQueryTickersPriceInfo)

    #获取所有合约的最新价格信息
    #@return 查询是否成功，“0”表示查询成功，非“0”表示查询不成功
    sleep(1)
    retQueryAllTickersPriceInfo = api.queryAllTickersPriceInfo()
    printFuncName('queryAllTickersPriceInfo', retQueryAllTickersPriceInfo)

    # 1.1.8更新后新增的测试函数 141-158
    #订阅全市场的期权行情
    #@return 订阅全市期权场行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与全市场退订期权行情接口配套使用
    sleep(1)
    retSubscribeAllOptionMarketData = api.subscribeAllOptionMarketData(1)
    printFuncName('subscribeAllOptionMarketData', retSubscribeAllOptionMarketData)

    #退订全市场的期权行情
    #@return 退订全市场期权行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与订阅全市场期权行情接口配套使用
    sleep(1)
    ret_unSubscribeAllOptionMarketData = api.unSubscribeAllOptionMarketData(1)
    printFuncName('unSubscribeAllOptionMarketData', ret_unSubscribeAllOptionMarketData)

    #订阅全市场的期权行情订单簿
    #@return 订阅全市场期权行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与全市场退订期权行情订单簿接口配套使用
    sleep(1)
    subcribeAllOptionOrderBook = api.subscribeAllOptionOrderBook(1)
    printFuncName('subcribleAllOptionOrderBook', subcribeAllOptionOrderBook)

    #退订全市场的期权行情订单簿
    #@return 退订全市场期权行情订单簿接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与订阅全市场期权行情订单簿接口配套使用
    sleep(1)
    unSubcribleAllOptionOrderBook = api.unSubscribeAllOptionOrderBook(2)
    printFuncName('unSubcribleAllOptionOrderBook', unSubcribleAllOptionOrderBook)

    #订阅全市场的期权逐笔行情
    #@return 订阅全市场期权逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与全市场退订期权逐笔行情接口配套使用
    sleep(1)
    subscribeAllOptionTickByTick = api.subscribeAllOptionTickByTick(2)
    printFuncName('subscribeAllOptionTickByTick', subscribeAllOptionTickByTick)

    #退订全市场的期权逐笔行情
    #@return 退订全市场期权逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@remark 需要与订阅全市场期权逐笔行情接口配套使用
    sleep(1)
    unSubscribeAllOptionTickByTick = api.unSubscribeAllOptionTickByTick(2)
    printFuncName('unsubscribeAllOptionTickByTick', unSubscribeAllOptionTickByTick)

    #获取当前交易日合约的详细静态信息
    #@return 发送查询请求是否成功，“0”表示发送查询请求成功，非“0”表示发送查询请求不成功
    #@param exchange_id 交易所代码，必须提供 1-上海 2-深圳
    sleep(1)
    queryAllTickersFullInfo = api.queryAllTickersFullInfo(2)
    printFuncName('queryAllTickersFullInfo', queryAllTickersFullInfo)

    #sleep为了删除接口对象前将回调数据输出，不sleep直接删除回调对象会自动析构，无法返回回调的数据
    sleep(5)

    #删除接口对象本身
    #@remark 不再使用本接口对象时,调用该函数删除接口对象
    release = api.release()
    printFuncName('release', release)

    exit = api.exit()
    printFuncName('exit', exit)
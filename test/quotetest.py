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

    #逐笔丢包应答
	#@param begin_seq 当逐笔出现丢包时，丢包区间下限（可能与上限一致）
	#@param end_seq 当逐笔出现丢包时，丢包区间上限（可能与下限一致）
	#@remark 此函数只有在逐笔发生丢包时才会有调用，如果丢包的上下限一致，表示仅丢失了一个包，注意此包仅为数据包，包含1个或者多个逐笔数据
    def onTickByTickLossRange(self, begin_seq, end_seq):
        """"""
        printFuncName('onTickByTickLossRange',begin_seq, end_seq)

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
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

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
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

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
        if data['data_type_v2'] == 2 :#现货
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
        elif data['data_type_v2'] == 1:  #期权
            print("data['auction_price']:",data['auction_price'])# 波段性中断参考价(SH)
            print("data['auction_qty']:",data['auction_qty'])# 波段性中断集合竞价虚拟匹配量(SH)
            print("data['last_enquiry_time']:",data['last_enquiry_time'])# 最近询价时间(SH)
        elif data['data_type_v2'] == 3: #债券
            print("data['total_bid_qty']",data['total_bid_qty'])#委托买入总量(SH,SZ)
            print("data['total_ask_qty']",data['total_ask_qty'])#委托卖出总量(SH,SZ)
            print("data['ma_bid_price']",data['ma_bid_price'])#加权平均委买价格(SZ)
            print("data['ma_ask_price']",data['ma_ask_price'])#加权平均委卖价格(SZ)
            print("data['ma_bond_bid_price']",data['ma_bond_bid_price'])#债券加权平均委买价格(SH)
            print("data['ma_bond_ask_price']",data['ma_bond_ask_price'])#债券加权平均委卖价格(SH)
            print("data['yield_to_maturity']",data['yield_to_maturity'])#债券到期收益率(SH)
            print("data['match_lastpx']",data['match_lastpx'])#匹配成交最近价(SZ)
            print("data['ma_bond_price']",data['ma_bond_price'])#债券加权平均价格(SH)
            print("data['match_qty']",data['match_qty'])#匹配成交成交量(SZ)
            print("data['match_turnover']",data['match_turnover'])#配成交成交金额(SZ)
            print("data['r4']",data['r4'])#预留
            print("data['r5']",data['r5'])#预留
            print("data['r6']",data['r6'])#预留
            print("data['r7']",data['r7'])#预留
            print("data['r8']",data['r8'])#预留
            print("data['cancel_buy_count']",data['cancel_buy_count'])#买入撤单笔数(SH)
            print("data['cancel_sell_count']",data['cancel_sell_count'])#卖出撤单笔数(SH)
            print("data['cancel_buy_qty']",data['cancel_buy_qty'])#买入撤单数量(SH)
            print("data['cancel_sell_qty']",data['cancel_sell_qty'])#卖出撤单数量(SH)
            print("data['cancel_buy_money']",data['cancel_buy_money'])#买入撤单金额(SH)
            print("data['cancel_sell_money']",data['cancel_sell_money'])#卖出撤单金额(SH)
            print("data['total_buy_count']",data['total_buy_count'])#买入总笔数(SH)
            print("data['total_sell_count']",data['total_sell_count'])#卖出总笔数(SH)
            print("data['duration_after_buy']",data['duration_after_buy'])#买入委托成交最大等待时间(SH)
            print("data['duration_after_sell']",data['duration_after_sell'])#卖出委托成交最大等待时间(SH)
            print("data['num_bid_orders']",data['num_bid_orders'])#买方委托价位数(SH)
            print("data['num_ask_orders']",data['num_ask_orders'])#卖方委托价位数(SH)
            print("data['instrument_status']",data['instrument_status'])#时段(SHL2)，L1快照数据没有此字段，具体字段说明参阅《上海新债券Level2行情说明.doc》文档

    #ETF的IOPV通知
	#@param iopv ETF的参考单位基金净值数据，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onETFIOPVData(self, data):
        """"""
        printFuncName('onETFIOPVData', data)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码（不包含交易所信息），不带空格，以'\0'结尾
        print("data['data_time']:",data['data_time'])
        print("data['iopv']:",data['iopv'])
    
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
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

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
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

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
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

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
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #逐笔行情通知，包括股票、指数和期权
    #@param data 逐笔行情数据，包括逐笔委托和逐笔成交，此为共用结构体，需要根据type来区分是逐笔委托还是逐笔成交，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onTickByTick(self, data):
        """"""
        printFuncName('onTickByTick', data)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码
        print("data['data_time']:",data['data_time'])#委托时间 or 成交时间
        print("data['struct_seq']:",data['struct_seq'])#SH: 业务序号（委托成交统一编号，同一个channel_no内连续，此seq区别于联合体内的seq，channel_no等同于联合体内的channel_no）;SZ: 无意义
        print("data['type']:",data['type'])#1委托 or 2成交 or 3逐笔状态订单，2.2.32版本新增字段，为上海新债券Level2行情中独有
        if data['type'] == 1 :
            print("data['channel_no']:",data['channel_no'])#频道代码
            print("data['seq']:",data['seq'])#SH: 委托序号(委托单独编号, 同一channel_no内连续);SZ: 委托序号(委托成交统一编号, 同一channel_no内连续)
            print("data['price']:",data['price'])#委托价格
            print("data['qty']:",data['qty'])#委托数量
            print("data['side']:",data['side'])#'1':买; '2':卖; 'G':借入; 'F':出借
            print("data['ord_type']:",data['ord_type'])#订单类别: '1': 市价; '2': 限价; 'U': 本方最优
            print("data['order_no']:",data['order_no'])#SH: 原始订单号,SZ: 无意义
        elif data['type'] == 2 :
            print("data['channel_no']:",data['channel_no'])#频道代码
            print("data['seq']:",data['seq'])#SH: 成交序号(成交单独编号, 同一channel_no内连续,从1开始连续);SZ: 成交序号(委托成交统一编号, 同一channel_no内连续)
            print("data['price']:",data['price'])#成交价格
            print("data['qty']:",data['qty'])#成交量
            print("data['money']:",data['money'])#成交金额(仅适用上交所)
            print("data['bid_no']:",data['bid_no'])#买方订单号
            print("data['ask_no']:",data['ask_no'])#卖方订单号
            print("data['trade_flag']:",data['trade_flag'])#SH: 内外盘标识('B':主动买; 'S':主动卖; 'N':未知)SZ: 成交标识('4':撤; 'F':成交)
        else :
            print("data['channel_no']:",data['channel_no'])#频道代码
            print("data['seq']:",data['seq'])#同一channel_no内连续
            print("data['flag']:",data['flag'])#状态信息
        

    #订阅全市场的股票行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllMarketData(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllMarketData', exchange_id,error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #退订全市场的股票行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllMarketData(self, exchange_id,error):
        """"""
        printFuncName('onUnSubscribeAllMarketData',exchange_id,error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #订阅全市场的股票行情订单簿应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllOrderBook(self, exchange_id,error):
        """"""
        printFuncName('onSubscribeAllOrderBook', exchange_id,error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #退订全市场的股票行情订单簿应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllOrderBook(self, exchange_id,error):
        """"""
        printFuncName('onUnSubscribeAllOrderBook', exchange_id,error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #订阅全市场的股票逐笔行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllTickByTick(self, exchange_id,error):
        """"""
        printFuncName('onSubscribeAllTickByTick', exchange_id,error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #退订全市场的股票逐笔行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllTickByTick(self,exchange_id, error):
        """"""
        printFuncName('onUnSubscribeAllTickByTick',exchange_id, error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

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
        if('error_id' in error):
            print("error['error_id']:",error['error_id'])
            print("error['error_msg']:",error['error_msg'])

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
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #订阅全市场的期权行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllOptionMarketData(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllOptionMarketData',exchange_id, error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #退订全市场的期权行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllOptionMarketData(self,exchange_id, error):
        """"""
        printFuncName('onUnSubscribeAllMarketData', exchange_id,error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #订阅全市场的期权行情订单簿应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllOptionOrderBook(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllOptionOrderBook', exchange_id,error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #退订全市场的期权行情订单簿应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllOptionOrderBook(self,exchange_id, error):
        """"""
        printFuncName('onUnSubscribeAllOptionOrderBook', exchange_id,error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #订阅全市场的期权逐笔行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #@param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onSubscribeAllOptionTickByTick(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllOptionTickByTick', exchange_id,error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #退订全市场的期权逐笔行情应答
    #@param exchange_id 表示当前全订阅的市场，如果为XTP_EXCHANGE_UNKNOWN(3)，表示沪深全市场，XTP_EXCHANGE_SH(1)表示为上海全市场，XTP_EXCHANGE_SZ(2)表示为深圳全市场
    #param error 取消订阅合约时发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@remark 需要快速返回
    def onUnSubscribeAllOptionTickByTick(self,exchange_id,error):
        """"""
        printFuncName('onUnSubscribeAllOptionTickByTick',exchange_id, error)
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

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
        print("data['is_inventory']:",data['is_inventory'])#是否为存量科创板股票（即2025.07.13日前上市的）。1=是；0=否；(注：暂时不启用，待交易所对科创板成长层股票新增特殊标识后启用，启用前该字段无意义)
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
        if('error_id' in error):
            print("error['error_id']:",error['error_id'])
            print("error['error_msg']:",error['error_msg'])

    #查询新三板合约完整静态信息的应答
	#@param ticker_info 合约完整静态信息
	#@param error_info 查询合约完整静态信息时发生错误时返回的错误信息，当error_info为空，或者error_info.error_id为0时，表明没有错误
	#@param is_last 是否此次查询合约完整静态信息的最后一个应答，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    def onQueryAllNQTickersFullInfo(self, data, error, last):
        """"""
        printFuncName('onQueryAllNQTickersFullInfo', data, error, last)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#证券代码
        print("data['ticker_name']:",data['ticker_name'])#证券名称
        print("data['security_type']:",data['security_type'])#合约详细类型
        print("data['ticker_qualification_class']:",data['ticker_qualification_class'])#合约适当性类别
        print("data['ticker_abbr_en']:",data['ticker_abbr_en'])#英文简称
        print("data['base_ticker']:",data['base_ticker'])#基础证券
        print("data['currency_type']:",data['currency_type'])#行业种类
        print("data['security_type']:",data['security_type'])#货币种类
        print("data['trade_unit']:",data['trade_unit'])#交易单位
        print("data['hang_out_date']:",data['hang_out_date'])#挂牌日期
        print("data['value_date']:",data['value_date'])#起息日期
        print("data['maturity_date']:",data['maturity_date'])#到期日
        print("data['per_limit_vol']:",data['per_limit_vol'])#每笔限量
        print("data['buy_vol_unit']:",data['buy_vol_unit'])#买数量单位
        print("data['sell_vol_unit']:",data['sell_vol_unit'])#卖数量单位
        print("data['mini_declared_vol']:",data['mini_declared_vol'])#最小申报数量
        print("data['limit_price_attr']:",data['limit_price_attr'])#限价参数性质
        print("data['market_maker_quantity']:",data['market_maker_quantity'])#做市商数量
        print("data['price_gear']:",data['price_gear'])#价格档位
        print("data['first_limit_trans']:",data['first_limit_trans'])#首笔交易限价参数
        print("data['subsequent_limit_trans']:",data['subsequent_limit_trans'])#后续交易限价参数
        print("data['limit_upper_price']:",data['limit_upper_price'])#涨停价格
        print("data['limit_lower_price']:",data['limit_lower_price'])#跌停价格
        print("data['block_trade_upper']:",data['block_trade_upper'])#大宗交易价格上限(预留，默认0)
        print("data['block_trade_lower']:",data['block_trade_lower'])#大宗交易价格下限(预留，默认0)
        print("data['convert_into_ration']:",data['convert_into_ration'])#折合比例
        print("data['trade_status']:",data['trade_status'])#交易状态
        print("data['security_level']:",data['security_level'])#证券级别
        print("data['trade_type']:",data['trade_type'])#交易类型
        print("data['suspend_flag']:",data['suspend_flag'])#停牌标志
        print("data['ex_dividend_flag']:",data['ex_dividend_flag'])#除权除息标志
        print("data['layer_type']:",data['layer_type'])#分层信息
        print("data['reserved1']:",data['reserved1'])#保留字段
        print("data['trade_places']:",data['trade_places'])#交易场所 预留
        print("data['is_rzbd']:",data['is_rzbd'])#是否融资标的 Y是 N否
        print("data['is_rqbd']:",data['is_rqbd'])#是否融券标的 Y是 N否
        print("data['is_drrz']:",data['is_drrz'])#是否当日可融资 Y是 N否
        print("data['is_drrq']:",data['is_drrq'])#是否当日可融券 Y是 N否
        print("data['reserved']:",data['reserved'])#保留字段
        if('error_id' in error):
            print("error['error_id']:",error['error_id'])
            print("error['error_msg']:",error['error_msg'])
   
        
    #当客户端与回补行情服务器通信连接断开时，该方法被调用。
    #@param reason 错误原因，请与错误代码表对应
    #@remark api不会自动重连，当断线发生时，请用户自行选择后续操作。回补服务器会在无消息交互后会定时断线，请注意仅在需要回补数据时才保持连接，无回补需求时，无需登陆
    def onRebuildQuoteServerDisconnected(self, reason):
        """"""
        printFuncName('onRebuildQuoteServerDisconnected', reason)
        
    #请求回补指定频道的逐笔行情的总体结果应答
    #@param data 当回补结束时被调用，如果回补结果失败，则msg参数表示失败原因
    #@remark 需要快速返回，仅在回补数据发送结束后调用，如果请求数据太多，一次性无法回补完，data['result_code'] = XTP_REBUILD_RET_PARTLY，此时需要根据回补结果继续发起回补数据请求
    def onRequestRebuildQuote(self, data):
        """"""
        printFuncName('onRequestRebuildQuote',data)
        print("data['request_id']",data['request_id'])#请求id 请求包带过来的id
        print("data['exchange_id']",data['exchange_id'])#市场类型 上海 深圳
        print("data['size']",data['size'])#总共返回的数据条数
        print("data['channel_number']",data['channel_number'])#逐笔数据 通道号
        print("data['begin']",data['begin'])#逐笔 表示序列号begin; 快照 表示时间begin(格式为YYYYMMDDHHMMSSsss)
        print("data['end']",data['end'])#逐笔 表示序列号end; 快照 表示时间end(格式为YYYYMMDDHHMMSSsss)
        print("data['result_code']",data['result_code'])#结果类型码
        print("data['msg']",data['msg'])#结果信息文本
        
    #回补的逐笔行情数据
    #@param data 回补的逐笔行情数据
    #@remark 需要快速返回，此函数调用与onTickByTick不在一个线程内，会在onRequestRebuildQuote()之前回调
    def onRebuildTickByTick(self, data):
        """"""
        printFuncName('onRebuildTickByTick', data)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['ticker']:",data['ticker'])#合约代码
        print("data['data_time']:",data['data_time'])#委托时间 or 成交时间
        print("data['struct_seq']:",data['struct_seq'])#SH: 业务序号（委托成交统一编号，同一个channel_no内连续，此seq区别于联合体内的seq，channel_no等同于联合体内的channel_no）;SZ: 无意义
        print("data['type']:",data['type'])#1委托 or 2成交
        if data['type'] == 1 :
            print("data['channel_no']:",data['channel_no'])#频道代码
            print("data['seq']:",data['seq'])#SH: 委托序号(委托单独编号, 同一channel_no内连续);SZ: 委托序号(委托成交统一编号, 同一channel_no内连续)
            print("data['price']:",data['price'])#委托价格
            print("data['qty']:",data['qty'])#委托数量
            print("data['side']:",data['side'])#'1':买; '2':卖; 'G':借入; 'F':出借
            print("data['ord_type']:",data['ord_type'])#订单类别: '1': 市价; '2': 限价; 'U': 本方最优
            print("data['order_no']:",data['order_no'])#SH: 原始订单号,SZ: 无意义
        elif data['type'] == 2 :
            print("data['channel_no']:",data['channel_no'])#频道代码
            print("data['seq']:",data['seq'])#SH: 成交序号(成交单独编号, 同一channel_no内连续,从1开始连续);SZ: 成交序号(委托成交统一编号, 同一channel_no内连续)
            print("data['price']:",data['price'])#成交价格
            print("data['qty']:",data['qty'])#成交量
            print("data['money']:",data['money'])#成交金额(仅适用上交所)
            print("data['bid_no']:",data['bid_no'])#买方订单号
            print("data['ask_no']:",data['ask_no'])#卖方订单号
            print("data['trade_flag']:",data['trade_flag'])#SH: 内外盘标识('B':主动买; 'S':主动卖; 'N':未知)SZ: 成交标识('4':撤; 'F':成交)
        else :
            print("data['channel_no']:",data['channel_no'])#频道代码
            print("data['seq']:",data['seq'])#同一channel_no内连续
            print("data['flag']:",data['flag'])#状态信息
        
        
    #回补的快照行情数据
    #@param data 回补的逐笔行情数据
    #@remark 需要快速返回，此函数调用与onDepthMarketData不在一个线程内，会在onRequestRebuildQuote()之前回调
    def onRebuildMarketData(self, data):
        """"""
        printFuncName('onRebuildMarketData', data)
        
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
        if data['data_type_v2'] == 2 :#现货
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
        elif data['data_type_v2'] == 1:  #期权
            print("data['auction_price']:",data['auction_price'])# 波段性中断参考价(SH)
            print("data['auction_qty']:",data['auction_qty'])# 波段性中断集合竞价虚拟匹配量(SH)
            print("data['last_enquiry_time']:",data['last_enquiry_time'])# 最近询价时间(SH)
        elif data['data_type_v2'] == 3: #债券
            print("data['total_bid_qty']",data['total_bid_qty'])#委托买入总量(SH,SZ)
            print("data['total_ask_qty']",data['total_ask_qty'])#委托卖出总量(SH,SZ)
            print("data['ma_bid_price']",data['ma_bid_price'])#加权平均委买价格(SZ)
            print("data['ma_ask_price']",data['ma_ask_price'])#加权平均委卖价格(SZ)
            print("data['ma_bond_bid_price']",data['ma_bond_bid_price'])#债券加权平均委买价格(SH)
            print("data['ma_bond_ask_price']",data['ma_bond_ask_price'])#债券加权平均委卖价格(SH)
            print("data['yield_to_maturity']",data['yield_to_maturity'])#债券到期收益率(SH)
            print("data['match_lastpx']",data['match_lastpx'])#匹配成交最近价(SZ)
            print("data['ma_bond_price']",data['ma_bond_price'])#债券加权平均价格(SH)
            print("data['match_qty']",data['match_qty'])#匹配成交成交量(SZ)
            print("data['match_turnover']",data['match_turnover'])#配成交成交金额(SZ)
            print("data['r4']",data['r4'])#预留
            print("data['r5']",data['r5'])#预留
            print("data['r6']",data['r6'])#预留
            print("data['r7']",data['r7'])#预留
            print("data['r8']",data['r8'])#预留
            print("data['cancel_buy_count']",data['cancel_buy_count'])#买入撤单笔数(SH)
            print("data['cancel_sell_count']",data['cancel_sell_count'])#卖出撤单笔数(SH)
            print("data['cancel_buy_qty']",data['cancel_buy_qty'])#买入撤单数量(SH)
            print("data['cancel_sell_qty']",data['cancel_sell_qty'])#卖出撤单数量(SH)
            print("data['cancel_buy_money']",data['cancel_buy_money'])#买入撤单金额(SH)
            print("data['cancel_sell_money']",data['cancel_sell_money'])#卖出撤单金额(SH)
            print("data['total_buy_count']",data['total_buy_count'])#买入总笔数(SH)
            print("data['total_sell_count']",data['total_sell_count'])#卖出总笔数(SH)
            print("data['duration_after_buy']",data['duration_after_buy'])#买入委托成交最大等待时间(SH)
            print("data['duration_after_sell']",data['duration_after_sell'])#卖出委托成交最大等待时间(SH)
            print("data['num_bid_orders']",data['num_bid_orders'])#买方委托价位数(SH)
            print("data['num_ask_orders']",data['num_ask_orders'])#卖方委托价位数(SH)
            print("data['instrument_status']",data['instrument_status'])#时段(SHL2)，L1快照数据没有此字段，具体字段说明参阅《上海新债券Level2行情说明.doc》文档
        
        



if __name__ == '__main__':

    ip = '119.3.103.38'
    port = 6002
    user = 'username'
    password = 'password'
    local_ip = '10.25.61.57'

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
    api.setHeartBeatInterval(15)

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
    if retLogin != 0 :
       retGetApiLastError = api.getApiLastError()
       printFuncName('getApiLastError', retGetApiLastError)   

    #获取API的系统错误
    #@return 返回的错误信息，可以在Login、Logout、订阅、取消订阅失败时调用，获取失败的原因
    #@remark 可以在调用api接口失败时调用，例如login失败时
    #getApiLastError = api.getApiLastError()
    #printFuncName("getApiLastError", getApiLastError)

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
    
    #获取新三板所有合约的详细静态信息，包括指数等非可交易的
	#@return 发送查询请求是否成功，“0”表示发送查询请求成功，非“0”表示发送查询请求不成功
    sleep(1)
    retQueryAllNQTickersFullInfo = api.queryAllNQTickersFullInfo()
    printFuncName('queryAllNQTickersFullInfo', retQueryAllNQTickersFullInfo)
    if retQueryAllNQTickersFullInfo != 0 :
       retGetApiLastError = api.getApiLastError()
       printFuncName('getApiLastError', retGetApiLastError)   

    ########行情回补部分
    sleep(1)
    rebuild_ip = '10.25.134.104'
    rebuild_port = 6662
    newuser = 'testshopt02tgt'
    newpassword = '123456'
    local_ip = '10.25.171.32'
	
    #用户登录回补服务器请求
    #@return 登录是否成功，“0”表示登录成功，“-1”表示连接服务器出错，此时用户可以调用GetApiLastError()来获取错误代码，“-2”表示已存在连接，不允许重复登录，如果需要重连，请先logout，“-3”表示输入有错误
    #@param ip 服务器ip地址，类似“127.0.0.1”
    #@param port 服务器端口号
    #@param user 登陆用户名
    #@param password 登陆密码
    #@param sock_type “1”代表TCP，“2”代表UDP
    #@param local_ip 本地网卡地址，类似“127.0.0.1”
    #@remark 此函数为同步阻塞式，不需要异步等待登录成功，当函数返回即可进行后续操作，此api只能有一个连接。回补服务器会在无消息交互后定时断线，请注意仅在需要回补数据时才保持连接，回补完成后请及时logout
    retLoginToRebuildQuoteServer = api.loginToRebuildQuoteServer(rebuild_ip,rebuild_port,newuser,newpassword,1,local_ip)
    printFuncName('loginToRebuildQuoteServer', retLoginToRebuildQuoteServer)
    if retLoginToRebuildQuoteServer != 0 :
        getApiLastError = api.getApiLastError()
        printFuncName("getApiLastError", getApiLastError)
    
    rebuild_param = {}
    rebuild_param['request_id'] = 1
    rebuild_param['data_type'] = 1
    rebuild_param['exchange_id'] = 1
    rebuild_param['ticker'] = '600000'
    #请求回补快照行情
    rebuild_param['begin'] = 20221214101000000
    rebuild_param['end'] =  20221214103000000
    #请求回补逐笔行情
    #rebuild_param['channel_number'] = 801
    #rebuild_param['begin'] = 1000
    #rebuild_param['end'] =  1200        
    #请求回补指定行情，包括快照和逐笔
    #@return 请求回补指定频道的逐笔行情接口调用是否成功，“0”表示接口调用成功，非“0”表示接口调用出错
    #@param rebuild_param 指定回补的参数信息，注意一次性回补最多1000个数据，超过1000需要分批次请求，一次只能指定一种类型的数据
    #@remark 仅在逐笔行情丢包时或者确实快照行情时使用，回补的行情数据将从OnRebuildTickByTick或者OnRebuildMarketData()接口回调提供，与订阅的行情数据不在同一个线程内
    rebuildquote = api.requestRebuildQuote(rebuild_param)
    printFuncName('requestRebuildQuote', rebuildquote)
    if rebuildquote != 0 :
        getApiLastError = api.getApiLastError()
        printFuncName("getApiLastError", getApiLastError)
    
    #登出回补服务器请求
    #@return 登出是否成功，“0”表示登出成功，非“0”表示登出出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@remark 此函数为同步阻塞式，不需要异步等待登出，当函数返回即可进行后续操作
    logoutRebuildQuoteServer = api.logoutFromRebuildQuoteServer()
    printFuncName('logoutFromRebuildQuoteServer', logoutRebuildQuoteServer)
    
    
    #sleep为了删除接口对象前将回调数据输出，不sleep直接删除回调对象会自动析构，无法返回回调的数据
    sleep(20)
    
    #登出请求
    #@return 登出是否成功，“0”表示登出成功，非“0”表示登出出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@remark 此函数为同步阻塞式，不需要异步等待登出，当函数返回即可进行后续操作
    logout = api.logout()
    printFuncName('logout:',logout )
    
    #删除接口对象本身
    #@remark 不再使用本接口对象时,调用该函数删除接口对象
    #release = api.release()
    #printFuncName('release', release)

    exit = api.exit()
    printFuncName('exit', exit)


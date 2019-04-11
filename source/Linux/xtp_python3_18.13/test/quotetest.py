# encoding: UTF-8

import os

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

    def onDisconnected(self, reason):

        printFuncName('onDisconnected', reason)

    def onError(self, data):
        """"""
        printFuncName('onError',data)

    def onSubMarketData(self, data, error, last):
        """"""
        printFuncName('onSubMarketData', data, error)

    def onUnSubMarketData(self, data, error, last):
        """"""
        printFuncName('onUnSubMarketData', data, error, last)

    def onMarketData(self, data):
        """"""
        printFuncName('onMarketData',data)

    def onSubOrderBook(self, data, error):
        """"""
        printFuncName('onSubOrderBook', data, error)

    def onDepthMarketData(self, data,bid1_qty_list,bid1_counts,max_bid1_count,ask1_qty_list,ask1_count,max_ask1_count):
        """"""
        printFuncName('onDepthMarketData', data,bid1_qty_list,bid1_counts,max_bid1_count,ask1_qty_list,ask1_count,max_ask1_count)

    def onUnSubOrderBook(self, data, error, last):
        """"""
        printFuncName('onUnSubOrderBook', data, error, last)

    def onOrderBook(self, data):
        """"""
        printFuncName('onOrderBook', data)

    def onSubTickByTick(self, data, error, last):
        """"""
        printFuncName('onSubTickByTick', data, error, last)

    def onUnSubTickByTick(self, data, error, last):
        """"""
        printFuncName('onUnSubTickByTick', data, error, last)

    def onTickByTick(self, data):
        """"""
        printFuncName('onTickByTick', data)

    def onSubscribeAllMarketData(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllMarketData', exchange_id,error)

    def onUnSubscribeAllMarketData(self, exchange_id,error):
        """"""
        printFuncName('onUnSubscribeAllMarketData',exchange_id,error)

    def onSubscribeAllOrderBook(self, exchange_id,error):
        """"""
        printFuncName('onSubscribeAllOrderBook', exchange_id,error)

    def onUnSubscribeAllOrderBook(self, exchange_id,error):
        """"""
        printFuncName('onUnSubscribeAllOrderBook', exchange_id,error)

    def onSubscribeAllTickByTick(self, exchange_id,error):
        """"""
        printFuncName('onSubscribeAllTickByTick', exchange_id,error)

    def onUnSubscribeAllTickByTick(self,exchange_id, error):
        """"""
        printFuncName('onUnSubscribeAllTickByTick',exchange_id, error)

    def onQueryAllTickers(self, data, error, last):
        """"""
        printFuncName('onQueryAllTickers', data, error, last)

    def onSubscribeAllOptionMarketData(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllOptionMarketData',exchange_id, error)

    def onUnSubscribeAllOptionMarketData(self,exchange_id, error):
        """"""
        printFuncName('onUnSubscribeAllMarketData', exchange_id,error)

    def onSubscribeAllOptionOrderBook(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllOptionOrderBook', exchange_id,error)

    def onUnSubscribeAllOptionOrderBook(self,exchange_id, error):
        """"""
        printFuncName('onUnSubscribeAllOptionOrderBook', exchange_id,error)

    def onSubscribeAllOptionTickByTick(self,exchange_id, error):
        """"""
        printFuncName('onSubscribeAllOptionTickByTick', exchange_id,error)

    def onUnSubscribeAllOptionTickByTick(self,exchange_id,error):
        """"""
        printFuncName('onUnSubscribeAllOptionTickByTick',exchange_id, error)

    def onQueryTickersPriceInfo(self, data, error, last):
        """"""
        printFuncName('onQueryTickersPriceInfo', data, error, last)



if __name__ == '__main__':

    ip = '120.27.164.138'
    port = 6002
    user = '15006594'
    password = 'SitHAKln'
    # 创建API并初始化
    api = TestApi()
    createQuoteAp = api.createQuoteApi(1, os.getcwd())
    printFuncName("createQuoteAp", createQuoteAp)
    # 登录
    retLogin = api.login(ip, port, user, password, 1)
    printFuncName('login', retLogin)

    # 1.1.8更新后新增的测试函数 141-158
    retSubscribeAllOptionMarketData = api.subscribeAllOptionMarketData(1)
    printFuncName('subscribeAllOptionMarketData', retSubscribeAllOptionMarketData)

    ret_unSubscribeAllOptionMarketData = api.unSubscribeAllOptionMarketData(1)
    printFuncName('unSubscribeAllOptionMarketData', ret_unSubscribeAllOptionMarketData)

    subcribeAllOptionOrderBook = api.subscribeAllOptionOrderBook(1)
    printFuncName('subcribleAllOptionOrderBook', subcribeAllOptionOrderBook)

    unSubcribleAllOptionOrderBook = api.unSubscribeAllOptionOrderBook(2)
    printFuncName('unSubcribleAllOptionOrderBook', unSubcribleAllOptionOrderBook)

    subscribeAllOptionTickByTick = api.subscribeAllOptionTickByTick(2)
    printFuncName('subscribeAllOptionTickByTick', subscribeAllOptionTickByTick)

    unSubscribeAllOptionTickByTick = api.unSubscribeAllOptionTickByTick(2)
    printFuncName('unsubscribeAllOptionTickByTick', unSubscribeAllOptionTickByTick)
    # 1.1.6测试函数
    #printFuncName("release", release)
    setHeartBeatInterval = api.setHeartBeatInterval(2)
    printFuncName("setHeartBeatInterval", setHeartBeatInterval)
    setUDPBufferSize = api.setUDPBufferSize(1)
    printFuncName("setUDPBufferSize", setUDPBufferSize)
    getApiLastError = api.getApiLastError()
    printFuncName("getApiLastError", getApiLastError)
    getApiVersion = api.getApiVersion()
    printFuncName("getApiVersion", getApiVersion)
    getTradingDay = api.getTradingDay()
    printFuncName("getTradingDay", getTradingDay)
	
    tickerList = [{'ticker':'000001'},{'ticker':'000002'},{'ticker':'000004'}]
    count = 3

    # 订阅行情
    retSubscribeMarketData = api.subscribeMarketData(tickerList,count, 2)
    printFuncName('subscribeMarketData', retSubscribeMarketData)
    # 取消订阅
    retUnSubscribeMarketData=api.unSubscribeMarketData(tickerList,count, 1)
    printFuncName('unSubscribeMarketData', retSubscribeMarketData)
    # 订阅行情订单簿
    retSubscribeOrderBook = api.subscribeOrderBook(tickerList,count, 2)
    printFuncName('subscribeOrderBook', retSubscribeOrderBook)
    # 取消订阅行情订单簿
    retUnSubscribeOrderBook = api.unSubscribeOrderBook(tickerList,count, 2)
    printFuncName('unSubscribeOrderBook', retUnSubscribeOrderBook)

    # 订阅逐笔行情
    retSubscribeTickByTick = api.subscribeTickByTick(tickerList,count, 2)
    printFuncName('subscribeTickByTick', retSubscribeTickByTick)
    # 取消订阅逐笔行情
    retUnSubscribeTickByTick = api.unSubscribeTickByTick(tickerList,count, 2)
    printFuncName('unSubscribeTickByTick', retUnSubscribeTickByTick)

    # 订阅全市场的行情
    retSubscribeAllMarketData = api.subscribeAllMarketData(2)
    printFuncName('subscribeAllMarketData', retSubscribeAllMarketData)
    # 取消订阅全市场的行情
    retUnSubscribeAllMarketData = api.unSubscribeAllMarketData(3)
    printFuncName('unSubscribeAllMarketData', retUnSubscribeAllMarketData)

    # 订阅全市场的行情订单簿
    retSubscribeAllOrderBook = api.subscribeAllOrderBook(1)
    printFuncName('subscribeAllOrderBook', retSubscribeAllOrderBook)
    # 退订全市场的行情订单簿
    retUnSubscribeAllOrderBook = api.unSubscribeAllOrderBook(5)
    printFuncName('unSubscribeAllOrderBook', retUnSubscribeAllOrderBook)

    # 订阅全市场的逐笔行情
    retSubscribeAllTickByTick = api.subscribeAllTickByTick(6)
    printFuncName('subscribeAllTickByTick', retSubscribeAllTickByTick)
    # 退订全市场的逐笔行情
    retUnSubscribeAllTickByTick = api.unSubscribeAllTickByTick(6)
    printFuncName('unSubscribeAllTickByTick', retUnSubscribeAllTickByTick)

    # 获取当前交易日可交易合约
    retQueryAllTickers = api.queryAllTickers(1)
    printFuncName('queryAllTickers', retQueryAllTickers)

    # 获取合约的最新价格信息
    retQueryTickersPriceInfo = api.queryTickersPriceInfo(tickerList,count, 1)
    printFuncName('queryTickersPriceInfo', retQueryTickersPriceInfo)

    # 获取当前交易日可交易合约
    retQueryAllTickersPriceInfo = api.queryAllTickersPriceInfo()
    printFuncName('queryAllTickersPriceInfo', retQueryAllTickersPriceInfo)
	
    api.release()
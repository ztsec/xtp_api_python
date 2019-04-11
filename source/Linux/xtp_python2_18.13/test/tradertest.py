# encoding: UTF-8

import os
from time import sleep

from vnxtptrader import *

    
def printFuncName(*args):
    """"""
    print('*' * 50)
    print(args)
    print('*' * 50)


class TestApi(TraderApi):
    """"""

    def __init__(self):
        """Constructor"""
        super(TestApi, self).__init__()

    def onDisconnected(self, reason):
        """"""
        printFuncName("onDisconnected", reason)

    def onError(self, data):
        """"""
        printFuncName('onError', data)

    def onOrderEvent(self, data, error,session):
        """"""
        printFuncName('onOrderEvent', data, error,session)

    def onTradeEvent(self, data,session):
        """"""
        printFuncName('onTradeEvent', data,session)

    def onCancelOrderError(self, data, error,session):
        """"""
        printFuncName('onCancelOrderError', data, error,session)

    def onQueryOrder(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryOrder', data, error, reqid, last,session)

    def onQueryTrade(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryTrade', data, error, reqid, last,session)

    def onQueryPosition(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryPosition', data, error, reqid, last,session)

    def onQueryAsset(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryAsset', data, error, reqid, reqid, last,session)

    def onQueryStructuredFund(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryStructuredFund', data, error, reqid, last,session)

    def onQueryFundTransfer(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryFundTransfer', data, error, reqid, last,session)

    def onFundTransfer(self, data, error,session):
        """"""
        printFuncName('onFundTransfer', data, error,session)

    def onQueryETF(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryETF', data, error, reqid, last,session)

    def onQueryETFBasket(self, data, error, reqid, last, session):
        """"""
        printFuncName('onQueryETFBasket', data, error, reqid, last, session)

    def onQueryIPOInfoList(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryIPOInfoList', data, error, reqid, last,session)

    def onQueryIPOQuotaInfo(self, data, error, reid, last,session):
        """"""
        printFuncName('onQueryIPOQuotaInfo', data, error, reqid, last,session)


if __name__ == '__main__':
    ip = '120.27.164.69'
    port = 6001
    user = '15006594'
    password = 'SitHAKln'
    reqid = 0
    
    # 创建API并初始化
    api = TestApi()
    createTraderApi = api.createTraderApi(1, os.getcwd())
    printFuncName('createTraderApi', createTraderApi)

    subscribePublicTopic = api.subscribePublicTopic(0)
    printFuncName('subscribePublicTopic', subscribePublicTopic)

    setSoftwareKey = api.setSoftwareKey("b8aa7173bba3470e390d787219b2112e")
    printFuncName('setSoftwareKey', setSoftwareKey)

    setSoftwareVersion = api.setSoftwareVersion("test")
    printFuncName('setSoftwareVersion', setSoftwareVersion)

    # 登录
    session = api.login(ip, port, user, password, 1)
    printFuncName('login', session)

    # 获取当前交易日
    retGetTradingDay = api.getTradingDay()
    printFuncName('getTradingDay', retGetTradingDay)

    # 获取API的发行版本号
    retGetApiVersion = api.getApiVersion()
    printFuncName('getApiVersion',retGetApiVersion)

    # 获取API的系统错误
    retGetApiLastError = api.getApiLastError()
    printFuncName('getApiLastError',retGetApiLastError)

    # 通过报单在xtp系统中的ID获取下单的客户端id
    order_xtp_id = 36989101307593706
    retGetClientIdByXTPID = api.getClientIDByXTPID(order_xtp_id)
    printFuncName('getClientIDByXTPID',retGetClientIdByXTPID)

    # 通过报单在xtp系统中的ID获取相关资金账户名
    retGetAccountByXTPID = api.getAccountByXTPID(order_xtp_id)
    printFuncName('getAccountByXTPID',retGetAccountByXTPID)

    # 报单录入请求
    sleep(2)
    order = {}
    order['ticker'] = '000001'  # 平安银行
    order['market'] = 1  # 深圳A股
    order['price'] = 8.5
    order['quantity'] = 100
    order['price_type'] = 1  # 限价单
    order['side'] = 1  # 买
    order['position_effect'] = 1  # 开仓

    retInsertOrder = api.insertOrder(order, session)
    printFuncName('insertOrder', retInsertOrder)

    # 撤单
    sleep(2)
    retCancelOrder = api.cancelOrder(retInsertOrder, session)
    printFuncName('cancelOrder',retCancelOrder)

    #根据报单ID请求查询报单
    reqid += 1
    retQueryOrderByXTPID = api.queryOrderByXTPID(order_xtp_id,session,reqid)
    printFuncName('queryOrderByXTPID',retQueryOrderByXTPID)

    # 请求查询报单
    sleep(1)
    reqid += 1
    retQueryOrders = api.queryOrders({}, session, reqid)
    printFuncName('queryOrders',retQueryOrders)

    # 根据委托编号请求查询相关成交
    sleep(1)
    reqid += 1
    retQueryTradesByXTPID = api.queryTradesByXTPID(order_xtp_id, session, reqid)
    printFuncName('queryTradesByXTPID', retQueryTradesByXTPID)

    # 请求查询已成交
    reqid += 1
    retQueryTrades = api.queryTrades({}, session, reqid)
    printFuncName('queryTrades',retQueryTrades)

    # 查询持仓
    reqid += 1
    retQueryPosition = api.queryPosition('', session, reqid)
    printFuncName('queryPosition',retQueryPosition)

    # 查询资产
    reqid += 1
    retQueryAsset = api.queryAsset(session, reqid)
    printFuncName('queryAsset',retQueryAsset)

    #请求查询分级基金
    reqid += 1
    retQueryStructuredFund = api.queryStructuredFund({},session, reqid)
    printFuncName('queryStructuredFund',retQueryStructuredFund)

    #资金划拨请求
    reqid += 1
    fundTransferInfo = {}
    fundTransferInfo['serial_id'] = 30000
    fundTransferInfo['fund_account'] = '15006594'
    fundTransferInfo['password'] = 'SitHAKln'
    fundTransferInfo['amount'] = 20000
    fundTransferInfo['transfer_type'] = 0
    retFundTransfer = api.fundTransfer(fundTransferInfo, session)
    printFuncName('fundTransfer', retFundTransfer)


    #请求查询资金划拨
    reqid += 1
    retQueryFundTransfer = api.queryFundTransfer({'amount': 20000}, session, reqid)
    printFuncName('queryFundTransfer', retQueryFundTransfer)


    #请求查询资金划拨
    reqid += 1
    retQueryETF = api.queryETF({}, session, reqid)
    printFuncName('queryETF', retQueryETF)

    # 请求查询资金划拨
    reqid += 1
    retQueryETFTickerBasket = api.queryETFTickerBasket(fundTransferInfo,session, reqid)
    printFuncName('queryETFTickerBasket',retQueryETFTickerBasket)

    # 请求查询今日新股申购信息列表
    reqid += 1
    retQueryIPOInfoList = api.queryIPOInfoList(session, reqid)
    printFuncName('queryIPOInfoList',retQueryIPOInfoList)

    # 请求查询用户新股申购额度信息
    reqid += 1
    retQueryIPOQuotaInfo = api.queryIPOQuotaInfo(session, reqid)
    printFuncName('queryIPOQuotaInfo',retQueryIPOQuotaInfo)

    # 登出
    sleep(5)
    logout = api.logout(session)
    printFuncName('logout:',logout )
    release = api.release()
    printFuncName('release', release)
    exit = api.exit()
    printFuncName('exit', exit)


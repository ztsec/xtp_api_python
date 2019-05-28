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

    #当客户端的某个连接与交易后台通信连接断开时，该方法被调用。
    #@param reason 错误原因，请与错误代码表对应
    #@param session_id 资金账户对应的session_id，登录时得到
    #@remark 用户主动调用logout导致的断线，不会触发此函数。api不会自动重连，当断线发生时，请用户自行选择后续操作，可以在此函数中调用Login重新登录，并更新session_id，此时用户收到的数据跟断线之前是连续的
    def onDisconnected(self, reason):
        """"""
        printFuncName("onDisconnected", reason)

    #错误应答
    #@param error 当服务器响应发生错误时的具体的错误代码和错误信息,当error为空，或者error.error_id为0时，表明没有错误
    #@remark 此函数只有在服务器发生错误时才会调用，一般无需用户处理
    def onError(self, data):
        """"""
        printFuncName('onError', data)

    #报单通知
    #@param data 订单响应具体信息，用户可以通过data.order_xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单，data.qty_left字段在订单为未成交、部成、全成、废单状态时，表示此订单还没有成交的数量，在部撤、全撤状态时，表示此订单被撤的数量。data.order_cancel_xtp_id为其所对应的撤单ID，不为0时表示此单被撤成功
    #@param error 订单被拒绝或者发生错误时错误代码和错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param session 资金账户对应的session，登录时得到
    #@remark 每次订单状态更新时，都会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线，在订单未成交、全部成交、全部撤单、部分撤单、已拒绝这些状态时会有响应，对于部分成交的情况，请由订单的成交回报来自行确认。所有登录了此用户的客户端都将收到此用户的订单响应
    def onOrderEvent(self, data, error,session):
        """"""
        printFuncName('onOrderEvent', data, error,session)
        print("data['cancel_time']:",data['cancel_time'])#撤销时间
        print("data['update_time']:",data['update_time'])#最后修改时间
        print("data['order_cancel_xtp_id']:",data['order_cancel_xtp_id'])#撤单在XTP系统中的id，在XTP系统中唯一
        print("data['order_client_id']:",data['order_client_id'])#报单引用
        print("data['trade_amount']:",data['trade_amount'])#成交金额
        print("data['price_type']:",data['price_type'])#报单价格条件
        print("data['order_type']:",data['order_type'])#报单类型
        print("data['price']:",data['price'])#价格
        print("data['qty_traded']:",data['qty_traded'])#今成交数量，为此订单累计成交数量
        print("data['qty_left']:",data['qty_left'])#剩余数量，当撤单成功时，表示撤单数量
        print("data['order_local_id']:",data['order_local_id'])#本地报单编号 OMS生成的单号，不等同于order_xtp_id，为服务器传到报盘的单号
        print("data['side']:",data['side'])#买卖方向
        print("data['position_effect']:",data['position_effect'])#开平标志
        print("data['reserved1']:",data['reserved1'])#预留字段1
        print("data['reserved2']:",data['reserved2'])#预留字段2
        print("data['order_submit_status']:",data['order_submit_status'])#报单提交状态，OMS内部使用，用户无需关心
        print("data['insert_time']:",data['insert_time'])#委托时间，格式为YYYYMMDDHHMMSSsss
        print("data['order_xtp_id']:",data['order_xtp_id'])#XTP系统订单ID，在XTP系统中唯一
        print("data['order_status']:",data['order_status'])#报单状态，订单响应中没有部分成交状态的推送，在查询订单结果中，会有部分成交状态
        print("data['ticker']:",data['ticker'])#合约代码
        print("data['order_cancel_client_id']:",data['order_cancel_client_id'])#报单操作引用，用户自定义（暂未使用）
        print("data['market']:",data['market'])#交易市场
        print("data['quantity']:",data['quantity'])#数量，此订单的报单数量
        print("data['business_type']:",data['business_type'])#业务类型
        print("error['error_id']:",error['error_id'])#
        print("error['error_msg']:",error['error_msg'])#

    #成交通知
    #@param data 成交回报的具体信息，用户可以通过data.order_xtp_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。对于上交所，exec_id可以唯一标识一笔成交。当发现2笔成交回报拥有相同的exec_id，则可以认为此笔交易自成交了。对于深交所，exec_id是唯一的，暂时无此判断机制。report_index+market字段可以组成唯一标识表示成交回报。
    #@param session 资金账户对应的session，登录时得到
    #@remark 订单有成交发生的时候，会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。所有登录了此用户的客户端都将收到此用户的成交回报。相关订单为部成状态，需要用户通过成交回报的成交数量来确定，OnOrderEvent()不会推送部成状态。
    def onTradeEvent(self, data,session):
        """"""
        printFuncName('onTradeEvent', data,session)
        print("data['branch_pbu']:",data['branch_pbu'])#交易所交易员代码
        print("data['trade_amount']:",data['trade_amount'])#成交金额，此次成交的总金额 = price*quantity
        print("data['exec_id']:",data['exec_id'])#成交编号，深交所唯一，上交所每笔交易唯一，当发现2笔成交回报拥有相同的exec_id，则可以认为此笔交易自成交
        print("data['trade_type']:",data['trade_type'])#成交类型  --成交回报中的执行类型
        print("data['order_client_id']:",data['order_client_id'])#报单引用
        print("data['order_exch_id']:",data['order_exch_id'])#报单编号 --交易所单号，上交所为空，深交所有此字段
        print("data['price']:",data['price'])#价格，此次成交的价格
        print("data['report_index']:",data['report_index'])#成交序号 --回报记录号，每个交易所唯一,report_index+market字段可以组成唯一标识表示成交回报
        print("data['local_order_id']:",data['local_order_id'])#订单号，引入XTPID后，该字段实际和order_xtp_id重复。接口中暂时保留
        print("data['trade_time']:",data['trade_time'])#成交时间，格式为YYYYMMDDHHMMSSsss
        print("data['order_xtp_id']:",data['order_xtp_id'])#XTP系统订单ID，此成交回报相关的订单ID，在XTP系统中唯一
        print("data['ticker']:",data['ticker'])#合约代码
        print("data['side']:",data['side'])#买卖方向
        print("data['position_effect']:",data['position_effect'])#开平标志
        print("data['reserved1']:",data['reserved1'])#预留字段
        print("data['reserved2']:",data['reserved2'])#预留字段
        print("data['market']:",data['market'])#交易市场
        print("data['quantity']:",data['quantity'])#数量，此次成交的数量，不是累计数量
        print("data['business_type']:",data['business_type'])#业务类型
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #撤单出错响应
    #@param data 撤单具体信息，包括撤单的order_cancel_xtp_id和待撤单的order_xtp_id
    #@param error 撤单被拒绝或者发生错误时错误代码和错误信息，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线，当error为空，或者error.error_id为0时，表明没有错误
    #@param session 资金账户对应的session，登录时得到
    #@remark 此响应只会在撤单发生错误时被回调
    def onCancelOrderError(self, data, error,session):
        """"""
        printFuncName('onCancelOrderError', data, error,session)

    #请求查询报单响应
    #@param data 查询到的一个报单
    #@param error 查询报单时发生错误时，返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param reqid 此消息响应函数对应的请求ID
    #@param last 此消息响应函数是否为reqid这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@param session 资金账户对应的session，登录时得到
    #@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。此对应的请求函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
    def onQueryOrder(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryOrder', data, error, reqid, last,session)
        print("data['cancel_time']:",data['cancel_time'])#撤销时间
        print("data['update_time']:",data['update_time'])#最后修改时间
        print("data['order_cancel_xtp_id']:",data['order_cancel_xtp_id'])#撤单在XTP系统中的id，在XTP系统中唯一
        print("data['order_client_id']:",data['order_client_id'])#报单引用
        print("data['trade_amount']:",data['trade_amount'])#成交金额
        print("data['price_type']:",data['price_type'])#报单价格条件
        print("data['order_type']:",data['order_type'])#报单类型
        print("data['price']:",data['price'])#价格
        print("data['qty_traded']:",data['qty_traded'])#今成交数量，为此订单累计成交数量
        print("data['qty_left']:",data['qty_left'])#剩余数量，当撤单成功时，表示撤单数量
        print("data['order_local_id']:",data['order_local_id'])#本地报单编号 OMS生成的单号，不等同于order_xtp_id，为服务器传到报盘的单号
        print("data['side']:",data['side'])#买卖方向
        print("data['position_effect']:",data['position_effect'])#开平标志
        print("data['reserved1']:",data['reserved1'])#预留字段1
        print("data['reserved2']:",data['reserved2'])#预留字段2
        print("data['order_submit_status']:",data['order_submit_status'])#报单提交状态，OMS内部使用，用户无需关心
        print("data['insert_time']:",data['insert_time'])#委托时间，格式为YYYYMMDDHHMMSSsss
        print("data['order_xtp_id']:",data['order_xtp_id'])#XTP系统订单ID，在XTP系统中唯一
        print("data['order_status']:",data['order_status'])#报单状态，订单响应中没有部分成交状态的推送，在查询订单结果中，会有部分成交状态
        print("data['ticker']:",data['ticker'])#合约代码
        print("data['order_cancel_client_id']:",data['order_cancel_client_id'])#报单操作引用，用户自定义（暂未使用）
        print("data['market']:",data['market'])#交易市场
        print("data['quantity']:",data['quantity'])#数量，此订单的报单数量
        print("data['business_type']:",data['business_type'])#业务类型
        print("error['error_id']:",error['error_id'])#
        print("error['error_msg']:",error['error_msg'])#

    #请求查询成交响应
    #@param data 查询到的一个成交回报
    #@param error 查询成交回报发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param reqid 此消息响应函数对应的请求ID
    #@param last 此消息响应函数是否为reqid这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@param session 资金账户对应的session，登录时得到
    #@remark 由于支持分时段查询，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。此对应的请求函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
    def onQueryTrade(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryTrade', data, error, reqid, last,session)
        print("data['branch_pbu']:",data['branch_pbu'])#交易所交易员代码
        print("data['trade_amount']:",data['trade_amount'])#成交金额，此次成交的总金额 = price*quantity
        print("data['exec_id']:",data['exec_id'])#成交编号，深交所唯一，上交所每笔交易唯一，当发现2笔成交回报拥有相同的exec_id，则可以认为此笔交易自成交
        print("data['trade_type']:",data['trade_type'])#成交类型  --成交回报中的执行类型
        print("data['order_client_id']:",data['order_client_id'])#报单引用
        print("data['order_exch_id']:",data['order_exch_id'])#报单编号 --交易所单号，上交所为空，深交所有此字段
        print("data['price']:",data['price'])#价格，此次成交的价格
        print("data['report_index']:",data['report_index'])#成交序号 --回报记录号，每个交易所唯一,report_index+market字段可以组成唯一标识表示成交回报
        print("data['local_order_id']:",data['local_order_id'])#订单号，引入XTPID后，该字段实际和order_xtp_id重复。接口中暂时保留
        print("data['trade_time']:",data['trade_time'])#成交时间，格式为YYYYMMDDHHMMSSsss
        print("data['order_xtp_id']:",data['order_xtp_id'])#XTP系统订单ID，此成交回报相关的订单ID，在XTP系统中唯一
        print("data['ticker']:",data['ticker'])#合约代码
        print("data['side']:",data['side'])#买卖方向
        print("data['position_effect']:",data['position_effect'])#开平标志
        print("data['reserved1']:",data['reserved1'])#预留字段
        print("data['reserved2']:",data['reserved2'])#预留字段
        print("data['market']:",data['market'])#交易市场
        print("data['quantity']:",data['quantity'])#数量，此次成交的数量，不是累计数量
        print("data['business_type']:",data['business_type'])#业务类型
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #请求查询投资者持仓响应
    #@param data 查询到的一只股票的持仓情况
    #@param error 查询账户持仓发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param reqid 此消息响应函数对应的请求ID
    #@param last 此消息响应函数是否为reqid这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@param session 资金账户对应的session，登录时得到
    #@remark 由于用户可能持有多个股票，一个查询请求可能对应多个响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onQueryPosition(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryPosition', data, error, reqid, last,session)
        print("data['ticker']:",data['ticker'])#证券代码
        print("data['ticker_name']:",data['ticker_name'])#证券名称
        print("data['market']:",data['market'])#交易市场
        print("data['total_qty']:",data['total_qty'])#总持仓
        print("data['sellable_qty']:",data['sellable_qty'])#可卖持仓
        print("data['avg_price']:",data['avg_price'])#持仓成本
        print("data['unrealized_pnl']:",data['unrealized_pnl'])#浮动盈亏（保留字段）
        print("data['yesterday_position']:",data['yesterday_position'])#昨日持仓
        print("data['purchase_redeemable_qty']:",data['purchase_redeemable_qty'])#今日申购赎回数量（申购和赎回数量不可能同时存在，因此可以共用一个字段）
        print("data['position_direction']:",data['position_direction'])#持仓方向
        print("data['reserved1']:",data['reserved1'])#保留字段
        print("data['executable_option']:",data['executable_option'])#可行权合约
        print("data['lockable_position']:",data['lockable_position'])#可锁定标的
        print("data['executable_underlying']:",data['executable_underlying'])#可行权标的
        print("data['locked_position']:",data['locked_position'])#已锁定标的
        print("data['usable_locked_position']:",data['usable_locked_position'])#可用已锁定标的
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])


    #请求查询资金账户响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    #@param data 查询到的资金账户情况
    #@param error 查询资金账户发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param reqid 此消息响应函数对应的请求ID
    #@param last 此消息响应函数是否为reqid这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@param session 资金账户对应的session，登录时得到
    #@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onQueryAsset(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryAsset', data, error, reqid, reqid, last,session)
        print("data['total_asset']:",data['total_asset'])#总资产(=可用资金 + 证券资产（目前为0）+ 预扣的资金)
        print("data['buying_power']:",data['buying_power'])#可用资金
        print("data['security_asset']:",data['security_asset'])#证券资产（保留字段，目前为0)
        print("data['fund_buy_amount']:",data['fund_buy_amount'])#累计买入成交证券占用资金
        print("data['fund_buy_fee']:",data['fund_buy_fee'])#累计买入成交交易费用
        print("data['fund_sell_amount']:",data['fund_sell_amount'])#累计卖出成交证券所得资金
        print("data['fund_sell_fee']:",data['fund_sell_fee'])#累计卖出成交交易费用
        print("data['withholding_amount']:",data['withholding_amount'])#XTP系统预扣的资金（包括购买卖股票时预扣的交易资金+预扣手续费）
        print("data['account_type']:",data['account_type'])#账户类型
        print("data['frozen_margin']:",data['frozen_margin'])#冻结的保证金
        print("data['frozen_exec_cash']:",data['frozen_exec_cash'])#行权冻结资金
        print("data['frozen_exec_fee']:",data['frozen_exec_fee'])#行权费用
        print("data['pay_later']:",data['pay_later'])#垫付资金
        print("data['preadva_pay']:",data['preadva_pay'])#预垫付资金
        print("data['orig_banlance']:",data['orig_banlance'])#昨日余额
        print("data['banlance']:",data['banlance'])#当前余额
        print("data['deposit_withdraw']:",data['deposit_withdraw'])#当天出入金
        print("data['trade_netting']:",data['trade_netting'])#当日交易资金轧差
        print("data['captial_asset']:",data['captial_asset'])#资金资产
        print("data['force_freeze_amount']:",data['force_freeze_amount'])#强锁资金
        print("data['preferred_amount']:",data['preferred_amount'])#可取资金
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #请求查询分级基金信息响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    #@param data 查询到的分级基金情况
    #@param error 查询分级基金发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param reqid 此消息响应函数对应的请求ID
    #@param last 此消息响应函数是否为reqid这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@param session 资金账户对应的session，登录时得到
    #@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onQueryStructuredFund(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryStructuredFund', data, error, reqid, last,session)
        print("data['exchange_id']:",data['exchange_id'])#交易所代码
        print("data['sf_ticker']:",data['sf_ticker'])#分级基金母基金代码
        print("data['sf_ticker_name']:",data['sf_ticker_name'])#分级基金母基金名称
        print("data['ticker']:",data['ticker'])#分级基金子基金代码
        print("data['ticker_name']:",data['ticker_name'])#分级基金子基金名称
        print("data['split_merge_status']:",data['split_merge_status'])#基金允许拆分合并状态
        print("data['ratio']:",data['ratio'])#拆分合并比例
        print("data['min_split_qty']:",data['min_split_qty'])#最小拆分数量
        print("data['min_merge_qty']:",data['min_merge_qty'])#最小合并数量
        print("data['net_price']:",data['net_price'])#基金净值
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #请求查询资金划拨订单响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    #@param data 查询到的资金账户情况
    #@param error 查询资金账户发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param reqid 此消息响应函数对应的请求ID
    #@param last 此消息响应函数是否为reqid这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@param session 资金账户对应的session，登录时得到
    #@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onQueryFundTransfer(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryFundTransfer', data, error, reqid, last,session)
        print("data['serial_id']:",data['serial_id'])#资金内转编号
        print("data['transfer_type']:",data['transfer_type'])#内转类型
        print("data['amount']:",data['amount'])#金额
        print("data['oper_status']:",data['oper_status'])#操作结果
        print("data['transfer_time']:",data['transfer_time'])#操作时间
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #资金划拨通知
    #@param data 资金划拨通知的具体信息，用户可以通过data.serial_id来管理订单，通过GetClientIDByXTPID() == client_id来过滤自己的订单。
    #@param error 资金划拨订单被拒绝或者发生错误时错误代码和错误信息，当error为空，或者error.error_id为0时，表明没有错误。当资金划拨方向为一号两中心节点之间划拨，且error.error_id=11000382时，error.error_msg为结点中可用于划拨的资金（以整数为准），用户需进行stringToInt的转化，可据此填写合适的资金，再次发起划拨请求
    #@param session 资金账户对应的session，登录时得到
    #@remark 当资金划拨订单有状态变化的时候，会被调用，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线。所有登录了此用户的客户端都将收到此用户的资金划拨通知。
    def onFundTransfer(self, data, error,session):
        """"""
        printFuncName('onFundTransfer', data, error,session)
        print("data['serial_id']:",data['serial_id'])#资金内转编号
        print("data['transfer_type']:",data['transfer_type'])#内转类型
        print("data['amount']:",data['amount'])#金额
        print("data['oper_status']:",data['oper_status'])#操作结果
        print("data['transfer_time']:",data['transfer_time'])#操作时间
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #请求查询ETF清单文件的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    #@param data 查询到的ETF清单文件情况
    #@param error 查询ETF清单文件发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param reqid 此消息响应函数对应的请求ID
    #@param last 此消息响应函数是否为reqid这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@param session 资金账户对应的session，登录时得到
    #@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onQueryETF(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryETF', data, error, reqid, last,session)
        print("data['market']:",data['market'])#交易市场
        print("data['etf']:",data['etf'])#etf代码,买卖,申赎统一使用该代码
        print("data['subscribe_redemption_ticker']:",data['subscribe_redemption_ticker'])#etf申购赎回代码
        print("data['unit']:",data['unit'])#最小申购赎回单位对应的ETF份数,例如上证"50ETF"就是900000
        print("data['subscribe_status']:",data['subscribe_status'])#是否允许申购,1-允许,0-禁止
        print("data['redemption_status']:",data['redemption_status'])#是否允许赎回,1-允许,0-禁止
        print("data['max_cash_ratio']:",data['max_cash_ratio'])#最大现金替代比例,小于1的数值   TODO 是否采用double
        print("data['estimate_amount']:",data['estimate_amount'])#T日预估金额
        print("data['cash_component']:",data['cash_component'])#T-X日现金差额
        print("data['net_value']:",data['net_value'])#基金单位净值
        print("data['total_amount']:",data['total_amount'])#最小申赎单位净值总金额=net_value*unit
        print("error['error_id']:",error['error_id'])#
        print("error['error_msg']:",error['error_msg'])#

    #请求查询ETF股票篮的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    #@param data 查询到的ETF合约的相关成分股信息
    #@param error 查询ETF股票篮发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param reqid 此消息响应函数对应的请求ID
    #@param last 此消息响应函数是否为reqid这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@param session 资金账户对应的session，登录时得到
    #@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onQueryETFBasket(self, data, error, reqid, last, session):
        """"""
        printFuncName('onQueryETFBasket', data, error, reqid, last, session)
        print("data['market']:",data['market'])#交易市场
        print("data['ticker']:",data['ticker'])#ETF代码
        print("data['component_ticker']:",data['component_ticker'])#成份股代码
        print("data['component_name']:",data['component_name'])#成份股名称
        print("data['quantity']:",data['quantity'])#成份股数量
        print("data['component_market']:",data['component_market'])#成份股交易市场
        print("data['replace_type']:",data['replace_type'])#成份股替代标识
        print("data['premium_ratio']:",data['premium_ratio'])#溢价比例
        print("data['amount']:",data['amount'])#成分股替代标识为必须现金替代时候的总金额
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #请求查询今日新股申购信息列表的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    #@param data 查询到的今日新股申购的一只股票信息
    #@param error 查询今日新股申购信息列表发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param reqid 此消息响应函数对应的请求ID
    #@param last 此消息响应函数是否为reqid这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@param session 资金账户对应的session，登录时得到
    #@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onQueryIPOInfoList(self, data, error, reqid, last,session):
        """"""
        printFuncName('onQueryIPOInfoList', data, error, reqid, last,session)
        print("data['market']:",data['market'])#交易市场
        print("data['ticker']:",data['ticker'])#申购代码
        print("data['ticker_name']:",data['ticker_name'])#申购股票名称
        print("data['price']:",data['price'])#申购价格
        print("data['unit']:",data['unit'])#申购单元
        print("data['qty_upper_limit']:",data['qty_upper_limit'])#最大允许申购数量
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #请求查询用户新股申购额度信息的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    #@param data 查询到的用户某个市场的今日新股申购额度信息
    #@param error 查查询用户新股申购额度信息发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param reid 此消息响应函数对应的请求ID
    #@param last 此消息响应函数是否为reid这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@param session 资金账户对应的session，登录时得到
    #@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onQueryIPOQuotaInfo(self, data, error, reid, last,session):
        """"""
        printFuncName('onQueryIPOQuotaInfo', data, error, reqid, last,session)
        print("data['market']:",data['market'])#交易市场
        print("data['quantity']:",data['quantity'])#可申购额度
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])

    #请求查询期权合约的响应，需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    #@param data 查询到的期权合约情况
    #@param error 查查询用户新股申购额度信息发生错误时返回的错误信息，当error为空，或者error.error_id为0时，表明没有错误
    #@param reid 此消息响应函数对应的请求ID
    #@param last 此消息响应函数是否为reid这条请求所对应的最后一个响应，当为最后一个的时候为true，如果为false，表示还有其他后续消息响应
    #@param session 资金账户对应的session，登录时得到
    #@remark 需要快速返回，否则会堵塞后续消息，当堵塞严重时，会触发断线
    def onQueryOptionAuctionInfo(self, data, error, reid, last,session):
        """"""
        printFuncName('onQueryOptionAuctionInfo', data, error, reqid, last,session)
        print("data['ticker']:",data['ticker'])#合约编码，报单ticker采用本字段
        print("data['security_id_source']:",data['security_id_source'])#证券代码源
        print("data['symbol']:",data['symbol'])#合约简称
        print("data['contract_id']:",data['contract_id'])#合约交易代码
        print("data['underlying_security_id']:",data['underlying_security_id'])#基础证券代码
        print("data['underlying_security_id_source']:",data['underlying_security_id_source'])#基础证券代码源

        print("data['list_date']:",data['list_date'])#上市日期，格式为YYYYMMDD
        print("data['last_trade_date']:",data['last_trade_date'])#最后交易日，格式为YYYYMMDD
        print("data['ticker_type']:",data['ticker_type'])#证券类别
        print("data['day_trading']:",data['day_trading'])#是否支持当日回转交易，1-允许，0-不允许

        print("data['call_or_put']:",data['call_or_put'])#认购或认沽
        print("data['delivery_day']:",data['delivery_day'])#行权交割日，格式为YYYYMMDD
        print("data['delivery_month']:",data['delivery_month'])#交割月份，格式为YYYYMM

        print("data['exercise_type']:",data['exercise_type'])#行权方式
        print("data['exercise_begin_date']:",data['exercise_begin_date'])#行权起始日期，格式为YYYYMMDD
        print("data['exercise_end_date']:",data['exercise_end_date'])#行权结束日期，格式为YYYYMMDD
        print("data['exercise_price']:",data['exercise_price'])#行权价格

        print("data['qty_unit']:",data['qty_unit'])#数量单位，对于某一证券申报的委托，其委托数量字段必须为该证券数量单位的整数倍
        print("data['contract_unit']:",data['contract_unit'])#合约单位
        print("data['contract_position']:",data['contract_position'])#合约持仓量
        print("data['prev_close_price']:",data['prev_close_price'])#合约前收盘价
        print("data['prev_clearing_price']:",data['prev_clearing_price'])#合约前结算价
        print("data['lmt_buy_max_qty']:",data['lmt_buy_max_qty'])#限价买最大量
        print("data['lmt_buy_min_qty']:",data['lmt_buy_min_qty'])#限价买最小量
        print("data['lmt_sell_max_qty']:",data['lmt_sell_max_qty'])#限价卖最大量
        print("data['lmt_sell_min_qty']:",data['lmt_sell_min_qty'])#限价卖最小量
        print("data['mkt_buy_max_qty']:",data['mkt_buy_max_qty'])#市价买最大量
        print("data['mkt_buy_min_qty']:",data['mkt_buy_min_qty'])#市价买最小量
        print("data['mkt_sell_max_qty']:",data['mkt_sell_max_qty'])#市价卖最大量
        print("data['mkt_sell_min_qty']:",data['mkt_sell_min_qty'])#市价卖最小量
        print("data['price_tick']:",data['price_tick'])#最小报价单位
        print("data['upper_limit_price']:",data['upper_limit_price'])#涨停价
        print("data['lower_limit_price']:",data['lower_limit_price'])#跌停价
        print("data['sell_margin']:",data['sell_margin'])#今卖开每张保证金
        print("data['margin_ratio_param1']:",data['margin_ratio_param1'])#交易所保证金比例计算参数一
        print("data['margin_ratio_param2']:",data['margin_ratio_param2'])#交易所保证金比例计算参数二
        print("error['error_id']:",error['error_id'])
        print("error['error_msg']:",error['error_msg'])


if __name__ == '__main__':
    ip = '120.27.164.69'
    port = 6001
    user = 'username'
    password = 'password'
    reqid = 0
    
    #创建TraderApi
    #@param client_id （必须输入）客户端id，用于区分同一用户的不同客户端，由用户自定义
    #@param save_file_path （必须输入）存贮订阅信息文件的目录，请设定一个真实存在的有可写权限的路径
    #@param log_level 日志输出级别
    #@return 创建出的UserApi
    #@remark 如果一个账户需要在多个客户端登录，请使用不同的client_id，系统允许一个账户同时登录多个客户端，但是对于同一账户，相同的client_id只能保持一个session连接，后面的登录在前一个session存续期间，无法连接。系统不支持过夜，请确保每天开盘前重新启动
    api = TestApi()
    createTraderApi = api.createTraderApi(1, os.getcwd())
    printFuncName('createTraderApi', createTraderApi)

    #订阅公共流。
    #@param resume_type 公共流（订单响应、成交回报）重传方式  
    #        XTP_TERT_RESTART(0):从本交易日开始重传
    #        XTP_TERT_RESUME(1):(保留字段，此方式暂未支持)从上次收到的续传
    #        XTP_TERT_QUICK(2):只传送登录后公共流的内容
    #@remark 该方法要在Login方法前调用。若不调用则不会收到公共流的数据。注意在用户断线后，如果不登出就login()，公共流订阅方式不会起作用。用户只会收到断线后的所有消息。如果先logout()再login()，那么公共流订阅方式会起作用，用户收到的数据会根据用户的选择方式而定。
    subscribePublicTopic = api.subscribePublicTopic(0)
    printFuncName('subscribePublicTopic', subscribePublicTopic)

    #设置软件开发Key
    #@param key 用户开发软件Key
    #@remark 此函数必须在Login之前调用
    setSoftwareKey = api.setSoftwareKey("key")
    printFuncName('setSoftwareKey', setSoftwareKey)

    #设置软件开发版本号
    #@param version 用户开发软件版本号，非api发行版本号，长度不超过15位
    #@remark 此函数必须在Login之前调用，标识的是客户端版本号，而不是API的版本号，由用户自定义
    setSoftwareVersion = api.setSoftwareVersion("test")
    printFuncName('setSoftwareVersion', setSoftwareVersion)

    #用户登录请求
    #@return session表明此资金账号登录是否成功，“0”表示登录失败，可以调用GetApiLastError()来获取错误代码，非“0”表示登录成功，此时需要记录下这个返回值session，与登录的资金账户对应
    #@param ip 服务器地址，类似“127.0.0.1”
    #@param port 服务器端口号
    #@param user 登录用户名
    #@param password 登录密码
    #@param sock_type “1”代表TCP，“2”代表UDP，目前暂时只支持TCP
    #@remark 此函数为同步阻塞式，不需要异步等待登录成功，当函数返回即可进行后续操作，此api可支持多个账户连接，但是同一个账户同一个client_id只能有一个session连接，后面的登录在前一个session存续期间，无法连接
    session = api.login(ip, port, user, password, 1)
    printFuncName('login', session)

    #获取当前交易日
    #return 获取到的交易日
    #@remark 只有登录成功后,才能得到正确的交易日
    retGetTradingDay = api.getTradingDay()
    printFuncName('getTradingDay', retGetTradingDay)

    #获取API的发行版本号
    #@return 返回api发行版本号
    retGetApiVersion = api.getApiVersion()
    printFuncName('getApiVersion',retGetApiVersion)

    #获取API的系统错误
    #@return 返回的错误信息，可以在Login、InsertOrder、CancelOrder返回值为0时调用，获取失败的原因
    #@remark 可以在调用api接口失败时调用，例如login失败时
    retGetApiLastError = api.getApiLastError()
    printFuncName('getApiLastError',retGetApiLastError)

    #通过报单在xtp系统中的ID获取下单的客户端id
    #@return 返回客户端id，可以用此方法过滤自己下的订单
    #@param order_xtp_id 报单在xtp系统中的ID
    #@remark 由于系统允许同一用户在不同客户端上登录操作，每个客户端通过不同的client_id进行区分
    order_xtp_id = 36989101307593706
    retGetClientIdByXTPID = api.getClientIDByXTPID(order_xtp_id)
    printFuncName('getClientIDByXTPID',retGetClientIdByXTPID)

    #通过报单在xtp系统中的ID获取相关资金账户名
    #@return 返回资金账户名
    #@param order_xtp_id 报单在xtp系统中的ID
    #@remark 只有资金账户登录成功后,才能得到正确的信息
    retGetAccountByXTPID = api.getAccountByXTPID(order_xtp_id)
    printFuncName('getAccountByXTPID',retGetAccountByXTPID)

    #报单录入请求
    #@return 报单在XTP系统中的ID,如果为‘0’表示报单发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示报单发送成功，用户需要记录下返回的order_xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
    #@param order 报单录入信息，其中order.order_client_id字段是用户自定义字段，用户输入什么值，订单响应OnOrderEvent()返回时就会带回什么值，类似于备注，方便用户自己定位订单。当然，如果你什么都不填，也是可以的。order.order_xtp_id字段无需用户填写，order.ticker必须不带空格，以'\0'结尾
    #@param session 资金账户对应的session,登录时得到
    #@remark 交易所接收订单后，会在报单响应函数OnOrderEvent()中返回报单未成交的状态，之后所有的订单状态改变（除了部成状态）都会通过报单响应函数返回

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

    #报单操作请求
    #@return 撤单在XTP系统中的ID,如果为‘0’表示撤单发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示撤单发送成功，用户需要记录下返回的order_cancel_xtp_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
    #@param retInsertOrder 需要撤销的委托单在XTP系统中的ID
    #@param session 资金账户对应的session,登录时得到
    #@remark 如果撤单成功，会在报单响应函数OnOrderEvent()里返回原单部撤或者全撤的消息，如果不成功，会在OnCancelOrderError()响应函数中返回错误原因
    sleep(2)
    retCancelOrder = api.cancelOrder(retInsertOrder, session)
    printFuncName('cancelOrder',retCancelOrder)

    #根据报单ID请求查询报单
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param order_xtp_id 需要查询的报单在xtp系统中的ID，即InsertOrder()成功时返回的order_xtp_id
    #@param session 资金账户对应的session，登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    reqid += 1
    retQueryOrderByXTPID = api.queryOrderByXTPID(order_xtp_id,session,reqid)
    printFuncName('queryOrderByXTPID',retQueryOrderByXTPID)

    #请求查询报单
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param queryOrdersInfo 需要查询的订单相关筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
    #@param session 资金账户对应的session，登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    #@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有报单，否则查询时间段内所有跟股票代码相关的报单，此函数查询出的结果可能对应多个查询结果响应。此函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
    queryOrdersInfo = {}
    queryOrdersInfo['ticker'] = '000001'
    queryOrdersInfo['begin_time'] = 0
    queryOrdersInfo['end_time'] = 0
    sleep(1)
    reqid += 1
    retQueryOrders = api.queryOrders(queryOrdersInfo, session, reqid)
    printFuncName('queryOrders',retQueryOrders)

    #根据委托编号请求查询相关成交
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param order_xtp_id 需要查询的委托编号，即InsertOrder()成功时返回的order_xtp_id
    #@param session 资金账户对应的session，登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    #@remark 此函数查询出的结果可能对应多个查询结果响应
    sleep(1)
    reqid += 1
    retQueryTradesByXTPID = api.queryTradesByXTPID(order_xtp_id, session, reqid)
    printFuncName('queryTradesByXTPID', retQueryTradesByXTPID)

    sleep(1)
    #请求查询已成交
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param queryTradesInfo 需要查询的成交回报筛选条件，其中合约代码可以为空，则默认所有存在的合约代码，如果不为空，其中起始时间格式为YYYYMMDDHHMMSSsss，为0则默认当前交易日0点，结束时间格式为YYYYMMDDHHMMSSsss，为0则默认当前时间
    #@param session 资金账户对应的session,登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    #@remark 该方法支持分时段查询，如果股票代码为空，则默认查询时间段内的所有成交回报，否则查询时间段内所有跟股票代码相关的成交回报，此函数查询出的结果可能对应多个查询结果响应。此函数不建议轮询使用，当报单量过多时，容易造成用户线路拥堵，导致api断线
    queryTradesInfo = {}
    queryTradesInfo['ticker'] = '000001'
    queryTradesInfo['begin_time'] = 0
    queryTradesInfo['end_time'] = 0
    reqid += 1
    retQueryTrades = api.queryTrades(queryTradesInfo, session, reqid)
    printFuncName('queryTrades',retQueryTrades)

    sleep(1)
    #请求查询投资者持仓
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param ticker 需要查询的持仓合约代码，可以为空，如果不为空，请不带空格
    #@param session_id 资金账户对应的session_id,登录时得到
    #@param request_id 用于用户定位查询响应的ID，由用户自定义
    #@remark 该方法如果用户提供了合约代码，则会查询此合约的持仓信息，如果合约代码为空，则默认查询所有持仓信息
    reqid += 1
    retQueryPosition = api.queryPosition('', session, reqid)
    printFuncName('queryPosition',retQueryPosition)

    sleep(1)
    #请求查询资产
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param session 资金账户对应的session,登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    reqid += 1
    retQueryAsset = api.queryAsset(session, reqid)
    printFuncName('queryAsset',retQueryAsset)

    sleep(1)
    #请求查询分级基金
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param query_param 需要查询的分级基金筛选条件，其中母基金代码可以为空，则默认所有存在的母基金，如果不为空，请不带空格，其中交易市场不能为空
    #@param session 资金账户对应的session,登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    #@remark 此函数查询出的结果可能对应多个查询结果响应
    reqid += 1
    retQueryStructuredFund = api.queryStructuredFund({},session, reqid)
    printFuncName('queryStructuredFund',retQueryStructuredFund)

    sleep(1)
    #资金划拨请求
    #@return 资金划拨订单在XTP系统中的ID,如果为‘0’表示消息发送失败，此时用户可以调用GetApiLastError()来获取错误代码，非“0”表示消息发送成功，用户需要记录下返回的serial_id，它保证一个交易日内唯一，不同的交易日不保证唯一性
    #@param fundTransferInfo 资金划拨的请求信息
    #@param session 资金账户对应的session,登录时得到
    #@remark 此函数支持一号两中心节点之间的资金划拨，注意资金划拨的方向。
    reqid += 1
    fundTransferInfo = {}
    fundTransferInfo['serial_id'] = 30000
    fundTransferInfo['fund_account'] = '15006594'
    fundTransferInfo['password'] = 'SitHAKln'
    fundTransferInfo['amount'] = 20000
    fundTransferInfo['transfer_type'] = 0
    retFundTransfer = api.fundTransfer(fundTransferInfo, session)
    printFuncName('fundTransfer', retFundTransfer)

    sleep(1)
    #请求查询资金划拨
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param query_param 需要查询的资金划拨订单筛选条件，其中serial_id可以为0，则默认所有资金划拨订单，如果不为0，则请求特定的资金划拨订单
    #@param session 资金账户对应的session,登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    reqid += 1
    retQueryFundTransfer = api.queryFundTransfer({'amount': 20000}, session, reqid)
    printFuncName('queryFundTransfer', retQueryFundTransfer)

    sleep(1)
    #请求查询ETF清单文件
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param queryETFInfo 需要查询的ETF清单文件的筛选条件，其中合约代码可以为空，则默认所有存在的ETF合约代码，market字段也可以为初始值，则默认所有市场的ETF合约
    #@param session 资金账户对应的session,登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    queryETFInfo = {}
    queryETFInfo['ticker'] = '510050'
    queryETFInfo['market'] = 2
    reqid += 1
    retQueryETF = api.queryETF(queryETFInfo, session, reqid)
    printFuncName('queryETF', retQueryETF)

    sleep(1)
    #请求查询ETF股票篮
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param queryETFTickerBasketInfo 需要查询股票篮的的ETF合约，其中合约代码不可以为空，market字段也必须指定
    #@param session 资金账户对应的session,登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    queryETFTickerBasketInfo = {}
    queryETFTickerBasketInfo['ticker'] = '510050'
    queryETFTickerBasketInfo['market'] = 2
    reqid += 1
    retQueryETFTickerBasket = api.queryETFTickerBasket(queryETFInfo,session, reqid)
    printFuncName('queryETFTickerBasket',retQueryETFTickerBasket)

    #请求查询今日新股申购信息列表
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param session 资金账户对应的session,登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    reqid += 1
    retQueryIPOInfoList = api.queryIPOInfoList(session, reqid)
    printFuncName('queryIPOInfoList',retQueryIPOInfoList)

    #请求查询用户新股申购额度信息
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param session 资金账户对应的session_id,登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    reqid += 1
    retQueryIPOQuotaInfo = api.queryIPOQuotaInfo(session, reqid)
    printFuncName('queryIPOQuotaInfo',retQueryIPOQuotaInfo)

    #请求查询期权合约
    #@return 查询是否成功，“0”表示成功，非“0”表示出错，此时用户可以调用GetApiLastError()来获取错误代码
    #@param query_param 需要查询的期权合约的筛选条件，可以为NULL（为NULL表示查询所有的期权合约）
    #@param session 资金账户对应的session,登录时得到
    #@param reqid 用于用户定位查询响应的ID，由用户自定义
    queryOptionAuctionInfo = {}
    queryOptionAuctionInfo['ticker'] = '10000001'
    queryOptionAuctionInfo['market'] = 2
    reqid += 1
    retQueryOptionAuctionInfo = api.queryOptionAuctionInfo(queryOptionAuctionInfo,session, reqid)
    printFuncName('queryOptionAuctionInfo',retQueryOptionAuctionInfo)

    #sleep为了删除接口对象前将回调数据输出，不sleep直接删除回调对象会自动析构，无法返回回调的数据
    sleep(5)

    #登出请求
    #@return 登出是否成功，“0”表示登出成功，“-1”表示登出失败
    #@param session 资金账户对应的session,登录时得到
    logout = api.logout(session)
    printFuncName('logout:',logout )

    #删除接口对象本身
    #@remark 不再使用本接口对象时,调用该函数删除接口对象
    release = api.release()
    printFuncName('release', release)

    exit = api.exit()
    printFuncName('exit', exit)


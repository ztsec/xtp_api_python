中泰证券量化交易平台XTP PYTHON API接口

本项目是中泰证券XTP极速交易PYTHON接口的开源实现，供客户在量化交易中使用PYTHON接口快速接入XTP系统。中泰证券XTP是为股票交易而生的极速交易系统，为投资者提供极速交易、极速行情、Level2行情。

目前支持xtp版本为1.1.19.2，支持windows、linux平台运行

请先到中泰证券xtp官方网站申请测试账号 https://xtp.zts.com.cn/register 及测试环境的连接ip、端口等信息

API参考官方C++版本的接口文档https://xtp.zts.com.cn/home

由于python版api的封装用的是vs2010,如果未安装vs环境,运行时会提示ImportError:DLL load failed。需要根据python的位数选择对应的运行库,32位的选x86,64位选x64,下载链接分别
x86网址:https://www.microsoft.com/zh-cn/download/confirmation.aspx?id=5555
x64网址:https://www.microsoft.com/zh-cn/download/confirmation.aspx?id=14632

##事项说明:

1.当前xtp的api的python封装支持windows系统和Linux系统，在bin目录下有Linux文件夹下存放Linux下python2和python3的封装库，Windows文件夹下还分别包括32位和64位下的python2和python3的封装库。test文件夹下包含行情和交易的测试脚本。

2.当前bin目录下编译时用的python2的版本为python2.7.15，python3用的版本的python3.6.5，在Windows下发现如果python的版本不对应时会导致调用python封装库会失败，如果使用当前的封装库请注意python的版本是否一致。

3.如果客户需要按自己的python版本编译封装库，可以自行用source目录下的代码，根据自己的python的版本，及该python版本对应的编译后的boost库，可自行编译所需的python封装库。（具体过程在doc文件夹下有编译过程文档，source文件夹下有源码,包括Linux和Windows下编译python2和python3封装库)

4.XTP_API_1.1.19.2_20190627文件夹下存放最新的xtp系统的api，当前python封装库的分别包含行情和交易库，python封装后的接口与当前xtp系统的api的所有接口都保持一致，只有方法名有所区别，python封装后的接口方法名首字母小写，xtp的api的方法名首字母大写。因此所有封装后的python接口方法使用和功能都可以参见xtp的api的接口方法。既在以下两个头文件中了解各个方法使用及功能。

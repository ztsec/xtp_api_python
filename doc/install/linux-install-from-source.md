# 环境配置

## Python

代码基于 Python 3.6.5, 测试下来最新的 3.6的最新版本3.6.9也可以支持，你可以通过你喜欢的方式安装 Python 3.6的版本


如 Ubuntu 下

```bash
apt install python3.6 python3.6-dev
```

注意，需要python3.6-dev, 后续编译 boost 用到

对于有多个 Python 版本，可以考虑virtualenv，在当前项目创建一个干净的环境

```bash
virtualenv --python=/usr/bin/python3.6 env/python3.6
```

> /usr/bin/python3.6 换成你的当前安装的路径

```bash
which python3.6
```

应用Python 3.6

```bash
source ./env/python3.6/bin/activate
 ```

## Boost

如果系统没有安装 boost，会看到如下错误

> ImportError: libboost_python.so.1.66.0: cannot open shared object file: No such file or directory


在当前以及 active Python 3.6的环境下安装

```bash
wget https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
tar zxf boost_1_66_0.tar.gz
cd boost_1_66_0
./bootstrap.sh --with-python=/usr/bin/python3.6
sudo ./b2 install --with-python include="/usr/include/python3.6m"  --with-atomic  --with-log  --with-test --with-thread --with-date_time  --with-chrono
```

安装后的文件再 /usr/local/lib/ 目录下

## 编译源码

Python 2以及不再被支持了，我们这里以 3 为例

```bash
cd source/Linux/xtp_python3_2.2.25.5/
```

主要修改两个地方

### Python

* PYTHON_INCLUDE_PATH
* PYTHON_LIBRARY

```shell
	set(PYTHON_LIBRARY /usr/lib/python3.6/)
	set(PYTHON_INCLUDE_PATH /usr/include/python3.6m)
```

### Boost

* BOOST_ROOT

```shell
    set(BOOST_ROOT   /usr/local/lib/)
```


调用 build.sh 编译

```bash
./build.sh
```

编译的结果在当前 build/lib 目录下，考虑文件至 /usr/local/lib

```bash
cp build/lib/*.so /usr/local/lib
```

## 测试代码

好了，到此我们的环境已经配置好，可以尝试运行系统的测试代码。在运行之前，请确认你已经拥有了测试账号，如果没有，请先至官网注册： https://xtp.zts.com.cn/registration-form.html

如果 你的 /usr/local/lib 不再当前的环境中，可手工加载进来，在测试代码最上方，加上

```python
# encoding: UTF-8

import sys
sys.path.append(r"/usr/local/lib")
```

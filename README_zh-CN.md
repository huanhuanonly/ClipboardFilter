# The ClipboardFilter
---

[![一键安装](https://img.shields.io/badge/%E4%B8%80%E9%94%AE%E5%AE%89%E8%A3%85-PRESS-green?style=plastic&logo=AdGuard)](https://github.com/huanhuanonly/ClipboardFilter/raw/refs/heads/main/ClipboardFilter.exe)
[![English](https://img.shields.io/badge/English-SWITCH-green?logo=googletranslate)](./README.md)

## 描述

> 监听剪贴板，在剪贴板写入时过滤指定字符，并且支持正则表达式替换匹配的文本。

## 更新日志

### 2023 年 12 月
* **发布** 版本 $\text{v}1.0.0$;

### 2024 年 9 月
* **修复** 一些 Bug 和逻辑缺陷；
* **添加** 状态栏和日志框；
* **移除** C 风格的转义字符（可以用 _变量_ 替代）；
* **添加** 对 _变量_ 的支持；
* **发布** 版本 $\text{v}1.1.0$;

[什么是变量？](#about-variables)

### 2024 年 10 月
* **修复** 在某些控件中未解析变量的问题；
* **添加** `before` 和 `after` 按钮；
* **取消** 日志栏中每条日志日期的显示；
* **添加** 变量解析的结果将在日志栏中显示；
* **添加** 图标的背景颜色；
* **发布** 版本 $\text{v}1.1.1$;

## 下载

```git
git clone https://github.com/huanhuanonly/ClipboardFilter
```

### 下载封装后的文件

点击 [![一键安装](https://img.shields.io/badge/%E4%B8%80%E9%94%AE%E5%AE%89%E8%A3%85-PRESS-green?style=plastic&logo=AdGuard)](https://github.com/huanhuanonly/ClipboardFilter/raw/refs/heads/main/ClipboardFilter.exe) 下载可执行文件 `ClipboardFilter.exe`，可以直接 **双击** 运行。

它使用 `Enigma Virtual Box` 进行打包，防火墙可能报毒属于正常现象，如果介意请选择第二种方法。

### 下载未封装的文件

需要下载 `./bin/v1.1.1/` 目录下的所有文件。

---

**_另外的，无论使用哪种方法您的配置文件都将存储在 `%temp%/ClipboardFilter.ini`。_**

如果您是 Windows 以外的系统，则需要手动编译。

在目前，编写、测试和维护工作都在 Windows 平台上进行。

## 展示

![Home.png](https://github.com/huanhuanonly/ClipboardFilter/blob/main/data/Screenshot/home.png?raw=true)

## 使用教程

* 过滤字符

只需在 `Character set to use for filtering` 的输入框中添加需要过滤的字符，输入顺序并不重要。

* 替换匹配的模式字符串

在 `Replacer's Rule Item` 组中使用 `insert` 按钮插入并设置对应的规则，按顺序匹配和替换。

### 关于变量

变量在每次使用前被解析，将会被解析为一段指定的字符串，同时也用于特殊字符的替代品。

#### 语法
```
${ variable-name [: parameter] }
```

变量名 `variable-names` 不区分大小写。

#### 内置变量

##### 从其他地方获取文本

* `env`: 获取环境变量；
  * `param`: 要获取的环境变量名称。
* `file`: 获取文件中的文本内容；
  * `param`: 包含文件名的文件路径。
* `clipboard`: 获取剪贴板中的文本数据（原始）；

* `escape`: 获取对应于C风格转义字符的字符；
  * `param`: C风格转义字符，例如，`\n` 是换行符。
* `linebreak`: 换行符的替代品；
  * `param`: （可选）`CRLF`（在**Windows**上定义）或 `LF`（在其他系统上定义）。
* `unicode`: 获取对应于**Unicode**编码的字符；
  * `param`: 一个非负整数，Unicode编码。

##### 函数变量

* `random`: 获取指定范围内的随机数（**整数**）；
  * `param`: （可选）有以下两种格式：
    * `lowerBound, upperBound`: 返回范围 $\left[lowerBound, upperBound\right]$ 内的随机数。
    * `upperBound`: 返回范围 $\left[0, upperBound\right]$ 内的随机数。
  * 例如：
    * `${random}` 返回一个64位随机数；
    * `${random:-10,99}` 返回一个范围在 $-10$ 到 $99$ 之间的随机数（包含两端）；
    * `${random:99}` 返回一个范围在 $0$ 到 $99$ 之间的随机数（包含两端）；
* `index`: 返回上一次调用的结果**加一**。此外，第一次调用返回整数 $1$。
  * `param`: （可选）一个字符串，可以使用多个索引器。
  * 例如：
    * `${index}, ${index} and ${index}` 返回 `1, 2 and 3`；
    * `${index:a}, ${index:a} and ${index:b}` 返回 `1, 2 and 1`；
    * `${index:a}, ${index:b}, ${index:a}, ${index:b}` 返回 `1, 1, 2, 2`；
* `iota`: 返回一个连续的子序列；
  * `param`: 一个字符串，如果两端是数字，则返回一系列数字，否则返回一系列字符；
  * 例如：
    * `${iota:15}` 返回 `15`，数字序列必须用_非数字_字符分隔，否则将被视为单个数字；
    * `${iota:1-5}` 返回 `1-2-3-4-5`；
    * `${iota:5-1}` 返回 `5-4-3-2-1`；
    * `${iota:10+=13}` 返回 `10+=11+=12+=13`；
    * `${iota:ae}` 返回 `abcde`；
    * `${iota:a,e}` 返回 `a,b,c,d,e`；
    * `${iota:e,a}` 返回 `e,d,c,b,a`；
    * `${iota:aUc}` 返回 `aUbUc`，除了首尾字符外，其余字符均视为分隔符；

##### 当前时间

* `hour`
* `minute`
* `second`
* `msec`
* `year`
* `month`
* `day`
* `week`
* `date`
* `time`
* `datetime`

## 项目结构
```
.
│  .gitignore
│  ClipboardFilter.exe
│  ClipboardFilter.pro
│  ClipboardFilter.pro.user
│  README.md
│  README_zh-CN.md
│
├─bin
│  ├─v1.0.0
│  │      ClipboardFilter_1.0.0.exe
│  │
│  ├─v1.1.0
│  │  │   ClipboardFilter_1.1.0.exe
│  │  │
│  │  └─...
│  │
│  └─v1.1.1
│      │  ClipboardFilter_1.1.1.exe
│      │  
│      └─...
│
├─build
│
├─data
│  │  res.qrc
│  │
│  ├─Font
│  │      CascadiaCode.ttf
│  │
│  ├─Icon
│  │      exit.svg
│  │      MainIcon.ico
│  │      MainIcon.png
│  │      MainIcon.svg
│  │      topmost.svg
│  │      topmost2.svg
│  │
│  ├─Screenshot
│  │      home.png
│  │
│  ├─Source
│  │      MainIcon.ai
│  │
│  ├─Ts
│  │      ClipboardFilter_zh_CN.ts
│  │
│  └─Ui
│         maindialog.ui
│
├─include
│      Logger.h
│      maindialog.h
│      StatusBarLabel.h
│      TextEditWithVariables.h
│      TextReplacer.h
│      TextReplaceRuleListView.h
│      Titlebar.h
│      Trie.h
│      VariableParser.h
│      VariablesShader.h
│
└─src
       Logger.cpp
       main.cpp
       maindialog.cpp
       TextReplaceRuleListView.cpp
       VariableParser.cpp
```

_**版权归 [huanhuanonly](https://github.com/huanhuanonly) 所有。**_
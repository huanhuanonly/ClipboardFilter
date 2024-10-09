# The ClipboardFilter
---

[![Install](https://img.shields.io/badge/Install-PRESS-green?style=plastic&logo=AdGuard
)](https://github.com/huanhuanonly/ClipboardFilter/raw/refs/heads/main/ClipboardFilter.exe)
[![简体中文](https://img.shields.io/badge/%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87-SWITCH-green?logo=googletranslate)](./README_zh-CN.md)

## Description

> Monitor the clipboard, filter specified characters when the clipboard is written, and support regular expressions to replace matching text.

## Update Log

### In December 2023
* **Release** version $\text{v}1.0.0$;

### In September 2024
* **Fixed** some bugs and logic flaws;
* **Added** status bar and log box;
* **Removed** C-style escape characters (can be replaced by _variables_);
* **Added** support for _variables_;
* **Release** version $\text{v}1.1.0$;

[What's variable?](#about-variables)

### In October 2024
* **Fixed** an issue where variables were not being parsed in some controls;
* **Added** `before` and `after` buttons;
* **Cancel** the display of the date of each log in the log bar;
* The result of variable parsing will be displayed in the log bar;
* **Added** background color for icons;
* **Release** version $\text{v}1.1.1$;

## Download

```git
git clone https://github.com/huanhuanonly/ClipboardFilter
```

If you are using *Windows* system, you can directly download the compiled executable file `ClipboardFilter.exe` and **double-click** it to _run_.

The executable file on the homepage is packaged using `Enigma Virtual Box`. It is normal for the firewall to report a virus. Please feel free to use it. You can also choose to use the unpackaged executable file `./bin/v1.1.0/ClipboardFilter.exe`.  

Your configuration file will be saved in `%temp%/ClipboardFilter.ini`.

## Display

![Home.png](https://github.com/huanhuanonly/ClipboardFilter/blob/main/data/Screenshot/home.png?raw=true)

## Tutorial

* Filtering characters

Just add the characters that need to be filtered in the input field of `Character set to use for filtering`. The order of input is not important.

<span class="ts">在</span> `Character set to use for filtering` <span class="ts">的输入栏中加入需要过滤的字符即可，输入顺序并不重要。</span>

* Replace the matched pattern string

Use the `insert` button in the `Replacer's Rule Item` group to insert and set the corresponding rules, which will be matched and replaced in order.

### About Variables

Variables are parsed before each use and will be parsed as a specific string, which is also used for special character replacement.

#### Syntax
```
${ variable-name [: parameter] }
```

`variable-names` are ***case-insensitive***.

#### Built-in variables

##### Get text from elsewhere

* `env`: Get environment variables;
  * `param`: The name of the environment variable to get.
* `file`: Get the text content in a file;
  * `param`: The filepath with the filename.
* `clipboard`: Get the text data (raw) from the clipboard;

* `escape`: Get the characters corresponding to C-style escape characters;
  * `param`: C-style escape characters, For example, `\n` is a linebreak character.
* `linebreak`: Alternatives to linebreaks;
  * `param`: (_optional_) `CRLF` (define on **Windows**) or `LF` (define on Other systems).
* `unicode`: Get the characters corresponding to **Unicode** code;
  * `param`: A non-negative integer, Unicode code.

##### Function variables

* `random`: Get a random number (**integer**) in a specified range;
  * `param`: (_optional_) There are the following two formats:
    * `lowerBound, upperBound`: Return a random number in range $\left[lowerBound, upperBound\right]$.
    * `upperBound`: Return a random number in range $\left[0, upperBound\right]$.
  * For example:
    * `${random}` Returns a 64-bit random number;
    * `${random:-10,99}` Returns a random number between $-10$ and $99$ (inclusive);
    * `${random:99}` Returns a random number between $0$ and $99$ (inclusive);
* `index`: Returns the result of the previous call **plus one**. In addition, the first call returns the integer $1$.
  * `param`: (_optional_) A string, Multiple indexers can be used.
  * For example:
    * `${index}, ${index} and ${index}` Return `1, 2 and 3`;
    * `${index:a}, ${index:a} and ${index:b}` Return `1, 2 and 1`;
    * `${index:a}, ${index:b}, ${index:a}, ${index:b}` Return `1, 1, 2, 2`;
* `iota`: Returns a contiguous subsequence;
  * `param`: A string, if both ends are numbers, return a sequence of numbers, otherwise return a sequence of characters;
  * For example:
    * `${iota:15}` Return `15`, A numeric sequence must be separated by _non-numeric_ characters, otherwise it will be considered a single number;
    * `${iota:1-5}` Return `1-2-3-4-5`;
    * `${iota:5-1}` Return `5-4-3-2-1`;
    * `${iota:10+=13}` Return `10+=11+=12+=13`;
    * `${iota:ae}` Return `abcde`;
    * `${iota:a,e}` Return `a,b,c,d,e`;
    * `${iota:e,a}` Return `e,d,c,b,a`;
    * `${iota:aUc}` Return `aUbUc`, Except for the first and last characters, all other characters are considered separators;

##### Current Time

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

## Project Structure
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

_**Copyright [huanhuanonly](https://github.com/huanhuanonly). All rights reserved.**_
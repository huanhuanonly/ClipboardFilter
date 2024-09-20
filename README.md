<!-- <style type="text/css">
    .ts
    {
        font-style: italic;
        text-decoration: underline;
        text-decoration-color: #72ece9;
        text-decoration-style: wavy;
    }
</style> -->

[![Install](https://img.shields.io/badge/Install-green
)](https://github.com/huanhuanonly/ClipboardFilter/raw/refs/heads/main/ClipboardFilter.exe)
---

# The ClipboardFilter

## Description

> Monitor the clipboard, filter specified characters when the clipboard is written, and support regular expressions to replace matching text.
> <p class="ts">监听剪贴板，在剪贴板写入时，过滤指定字符，并且支持正则表达式替换匹配的文本。</p>

## Update Log

### In December 2023
* **Release** version $\text{v}1.0.0$

### In September 2024
* **Fixed** some bugs and logic flaws;
* **Added** status bar and log box;
* **Removed** C-style escape characters (can be replaced by _variables_);
* **Added** support for _variables_;
* **Changed** version number to $\text{v}1.1.0$

[What's variable?](#about-variables)

## Download

```git
clone https://github.com/huanhuanonly/ClipboardFilter
```

If you are using *Windows* system, you can directly download the compiled executable file `ClipboardFilter.exe` and **double-click** it to _run_.

<p class="ts">如果你是 Windows 系统，可以直接下载编译后的可执行文件 <b>ClipboardFilter.exe</b>，双击运行。</p>

> Your configuration file will be saved in `%temp%/ClipboardFilter.ini`.  
> <span class="ts">您的配置文件将存储在</span> `%temp%/ClipboardFilter.ini`<span class="ts">。</span>


## Tutorial

* Filtering characters

Just add the characters that need to be filtered in the input field of `Character set to use for filtering`. The order of input is not important.

<span class="ts">在</span> `Character set to use for filtering` <span class="ts">的输入栏中加入需要过滤的字符即可，输入顺序并不重要。</span>

* Replace the matched pattern string

Use the `insert` button in the `Replacer's Rule Item` group to insert and set the corresponding rules, which will be matched and replaced in order.

<span class="ts">在</span> `Replacer's Rule Item` <span class="ts">组中使用</span> `insert` <span class="ts">按钮插入并设置对应的规则即可，按顺序匹配和替换。</span>

> Note: First filter the characters, then do the text replacement.
> <p class="ts">注意：先过滤字符，再进行文本替换。</p>

### About Variables

Variables are parsed before each use and will be parsed as a specific string, which is also used for special character replacement.

<p class="ts">变量在每次使用前被解析，将会被解析为一段指定的字符串，同时也用于特殊字符的替代品。</p>

#### Syntax
```
${ variable-name : parameter }
```

> `variable-names` are ***case-insensitive***
> <p class="ts">变量名不区分大小写</p>

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
    * `lower_bound, upper_bound`: Return a random number in range $\left[lower\_bound, upper\_bound\right]$.
    * `upper_bound`: Return a random number in range $\left[0, upper\_bound\right]$.
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
│  ClipboardFilter.pro
│  ClipboardFilter.pro.user
│  README.md
│
├─bin
│  ├─v1.0.0
│  │      ClipboardFilter_1.0.0.exe
│  │
│  └─v1.1.0
│      │  ClipboardFilter.exe
│      │  D3Dcompiler_47.dll
│      │  libEGL.dll
│      │  libgcc_s_seh-1.dll
│      │  libGLESv2.dll
│      │  libstdc++-6.dll
│      │  libwinpthread-1.dll
│      │  opengl32sw.dll
│      │  Qt5Core.dll
│      │  Qt5Gui.dll
│      │  Qt5Svg.dll
│      │  Qt5Widgets.dll
│      │
│      ├─iconengines
│      │      qsvgicon.dll
│      │
│      ├─imageformats
│      │      qgif.dll
│      │      qicns.dll
│      │      qico.dll
│      │      qjpeg.dll
│      │      qsvg.dll
│      │      qtga.dll
│      │      qtiff.dll
│      │      qwbmp.dll
│      │      qwebp.dll
│      │
│      ├─platforms
│      │      qwindows.dll
│      │
│      ├─styles
│      │      qwindowsvistastyle.dll
│      │
│      └─translations
│              qt_ar.qm
│              qt_bg.qm
│              qt_ca.qm
│              qt_cs.qm
│              qt_da.qm
│              qt_de.qm
│              qt_en.qm
│              qt_es.qm
│              qt_fi.qm
│              qt_fr.qm
│              qt_gd.qm
│              qt_he.qm
│              qt_hu.qm
│              qt_it.qm
│              qt_ja.qm
│              qt_ko.qm
│              qt_lv.qm
│              qt_pl.qm
│              qt_ru.qm
│              qt_sk.qm
│              qt_tr.qm
│              qt_uk.qm
│              qt_zh_TW.qm
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
       TextReplacer.cpp
       VariableParser.cpp
```

_**Copyright huanhuanonly. All rights reserved**_
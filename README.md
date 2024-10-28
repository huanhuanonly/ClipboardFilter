# The ClipboardFilter
---

[![Install](https://img.shields.io/badge/Install-PRESS-green?style=plastic&logo=AdGuard
)](https://github.com/huanhuanonly/ClipboardFilter/raw/refs/heads/main/ClipboardFilter.exe)
[![简体中文](https://img.shields.io/badge/%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87-SWITCH-green?logo=googletranslate)](./README_zh-CN.md)

## Description

> Monitor the clipboard, filter the specified characters when writing to the clipboard, and support **variable** and **regular expression** to replace the matched text.

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
* **Added** the result of variable parsing will be displayed in the log bar;
* **Added** background color for icons;
* **Release** version $\text{v}1.1.1$;

### In End of October 2024
* **Fixed** some bugs and logic vulnerabilities;
* **Added** a small floating window;
* **Added** a `shrink to floating window` button in the title bar;
* **Improved** log output;
* **Added** multiple variables for base conversion, text conversion, random selection, etc., and `parse`, `buffer`;
* **Added** `MatchOptions` to choose whether to be case-sensitive;
* **Released** version $\text{v}1.1.2$;

## Download

```git
git clone https://github.com/huanhuanonly/ClipboardFilter
```

### Download the Packaged File

Click [![Install](https://img.shields.io/badge/Install-PRESS-green?style=plastic&logo=AdGuard
)](https://github.com/huanhuanonly/ClipboardFilter/raw/refs/heads/main/ClipboardFilter.exe) to download the executable file `ClipboardFilter.exe`, which can be run by **double-clicking** it.

It is packaged using `Enigma Virtual Box`, and it's normal for the firewall to report a virus. If you are concerned, please choose the second method.

### Download the Unpackaged File

You need to download all files in the `./bin/v1.1.2/` directory.

---

**_Additionally, regardless of the method used, your configuration file will be stored in `%temp%/ClipboardFilter.ini`._**

If you are using a system other than Windows, you will need to compile it manually.

Currently, writing, testing, and maintenance work are being done on the Windows platform.

## Display

![Home.png](https://github.com/huanhuanonly/ClipboardFilter/blob/main/data/Screenshot/home.png?raw=true)

## Tutorial

* Filtering characters

Just add the characters that need to be filtered in the input field of `Character set to use for filtering`. The order of input is not important.

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

##### Get External Text
* `env`: Get an environment variable;
  * `param`: The name of the environment variable to retrieve.
  * For example: `${env:WINDIR}` might return `C:\WINDOWS`.
* `file`: Get the text content from a file;
  * `param`: The file path that includes the filename.
  * For example: `${file:C:\file.txt}`, note that only the first `:` acts as a separator.
* `clipboard`: Get text data from the clipboard (before replacement);

##### Get Current Time
* `hour`
* `minute`
* `second`
* `msec`
* `year`
* `month`
* `day`
* `week`
* `date`
  * `param`: (optional) format.
* `time`
  * `param`: (optional) format.
* `datetime`
  * `param`: (optional) format.

##### Encode to Text
* `escape`: Get the character corresponding to C-style escape sequences;
  * `param`: C-style escape sequence, for example, `\n` is a newline.
* `linebreak`: Alternative for newline;
  * `param`: (optional) `CRLF` (default on **Windows**) or `LF` (default on other systems).
* `unicode`: Get the character corresponding to **Unicode** encoding;
  * `param`: A non-negative integer, Unicode encoding.

##### Text to Encoding
* `tounicode`: Input a string of text and convert it to Unicode codes, separated by spaces.
  * `param`: Input text.
  * For example: `${tounicode:abc}` returns `97 98 99`.
* `tohtml`: Input a string of text and convert it to HTML escape sequences.
  * `param`: Input text.
  * For example: `${tohtml:<abc>}` returns `&lt;abc&gt;`.
* `hex`: Input a non-negative integer and convert it to hexadecimal.
  * `param`: Non-negative integer.
  * For example: `${hex:5201314}` returns `4f5da2`.
* `oct`: Same as above, convert to octal.
* `bin`: Same as above, convert to binary.
* `dec`: Same as above, convert to decimal.

##### Dynamic Variables
* `random`: Get a random number within a specified range (**integer**);
  * `param`: (optional) Two formats:
    * `lowerBound, upperBound`: Returns a random number in the range $\left[lowerBound, upperBound\right]$.
    * `upperBound`: Returns a random number in the range $\left[0, upperBound\right]$.
  * For example:
    * `${random}` returns a $64$ bit random number;
    * `${random:-10,99}` returns a random number in the range of $-10$ to $99$ (inclusive);
    * `${random:99}` returns a random number in the range of $0$ to $99$ (inclusive).
* `rsoc`: Short for `RandomlySelectOneChar`, randomly select one character.
  * `param`: Character set.
  * For example: `${rsoc:abc}` might return any of the characters `a`, `b`, or `c`.
* `rsow`: Short for `RandomlySelectOneWord`, randomly select one word.
  * `param`: Word set, can be separated by any English punctuation and whitespace.
  * For example: `${rsow:like,love}` might return either `like` or `love`.
* `rsol`: Short for `RandomlySelectOneLine`, randomly select one line.
  * `param`: Line set, separated by newline characters.
* `index`: Returns the last result **plus one**. The first call returns the integer $1$.
  * `param`: (optional) a string, multiple indexers can be used.
  * For example:
    * `${index}, ${index} and ${index}` returns `1, 2 and 3`;
    * `${index:a}, ${index:a} and ${index:b}` returns `1, 2 and 1`;
    * `${index:a}, ${index:b}, ${index:a}, ${index:b}` returns `1, 1, 2, 2`.
* `iota`: Returns a continuous subsequence;
  * `param`: A string, if both ends are digits, returns a series of numbers, otherwise returns a series of characters;
  * For example:
    * `${iota:15}` returns `15`, numeric sequences must be separated by _non-numeric_ characters, or they will be treated as a single number;
    * `${iota:1-5}` returns `1-2-3-4-5`;
    * `${iota:5-1}` returns `5-4-3-2-1`;
    * `${iota:10+=13}` returns `10+=11+=12+=13`;
    * `${iota:ae}` returns `abcde`;
    * `${iota:a,e}` returns `a,b,c,d,e`;
    * `${iota:e,a}` returns `e,d,c,b,a`;
    * `${iota:aUc}` returns `aUbUc`, treating all characters except the first and last as separators.

##### Text Transformation
* `shuffle`: Shuffle the order of the text.
  * `param`: Text.
  * For example: `${shuffle:ILoveYou}` might return `oYeLIuov`.
* `sort`: Sort the text.
  * `param`: Text.
  * For example: `${sort:ILoveYou1314}` returns `1134ILYeoouv`.
* `reverse`: Reverse the text.
  * `param`: Text.
  * For example: `${reverse:ILoveYou1314}` returns `4131uoYevoLI`.
* `hash`: Map the text to a hexadecimal hash value based on the `md5` algorithm.
  * `param`: Text.
  * For example: `${hash:ILoveYou1314}` returns `c52393bda98d849cc966a830f17f4cab`.
* `tolower`: Convert text to lowercase.
  * `param`: Text.
  * For example: `${tolower:ILoveYou1314}` returns `iloveyou1314`.
* `toupper`: Convert text to uppercase.
  * `param`: Text.
  * For example: `${toupper:ILoveYou1314}` returns `ILOVEYOU1314`.
* `trimmed`: Trim whitespace from both ends of the text.
  * `param`: Text.
  * For example: `${trimmed: ILoveYou1314 }` returns `ILoveYou1314`.
* `simplified`: Trim whitespace from both ends and replace each sequence of internal spaces with a single space.
  * `param`: Text.
  * For example: `${simplified: I   Love You 1314 }` returns `I Love You 1314`.

Here's the translation:

##### Special Variables

* `parse`: Parse variables within the text, supporting nesting.
  * `param`: Text.
  * For example: `${parse:${buffer}}`.
* `buffer`: Get the text from the buffer.

_After each rule replacement, the result is temporarily stored in the buffer. After all rule replacements are completed, the final result is written to the clipboard at once._

## Project Structure
```
.
│  .gitignore
│  ClipboardFilter.exe
│  ClipboardFilter.pro
│  ClipboardFilter.pro.user
│  LICENSE
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
│  ├─v1.1.1
│  │  │   ClipboardFilter_1.1.1.exe
│  │  │
│  │  └─...
│  │
│  └─v1.1.2
│      │  ClipboardFilter_1.1.2.exe
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
│  │      shrink.svg
│  │      topmost.svg
│  │      topmost2.svg
│  │
│  ├─Screenshot
│  │      home.png
│  │      home_1.1.1.png
│  │
│  ├─Source
│  │      MainIcon.ai
│  │      Shrink.ai
│  │
│  ├─Ts
│  │      ClipboardFilter_zh_CN.ts
│  │
│  └─Ui
│         maindialog.ui
│
├─include
│      ClickableLabel.h
│      Exception.h
│      Logger.h
│      MainDialog.h
│      MainFloatingWindow.h
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
       MainDialog.cpp
       MainFloatingWindow.cpp
       TextReplaceRuleListView.cpp
       VariableParser.cpp
```

_**Copyright [huanhuanonly](https://github.com/huanhuanonly). All rights reserved.**_
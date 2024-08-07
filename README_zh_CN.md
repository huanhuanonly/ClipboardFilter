[![PickLanguage](https://img.shields.io/badge/%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87-English-green
)](README.md)

---

# The ClipboardFilter

> 监听剪贴板并过滤其内容，支持正则表达式替换文本。

## 描述
- 如果你想删除一些字符：
> 演示如何删除剪贴板中的字符 `i`  
> 它将删除剪贴板中所有在 `CharacterList` 中出现的字符
![](https://github.com/huanhuanonly/ClipboardFilter/blob/main/Demo/RemoveCharacter.gif?raw=true)

- 如果你想使用像 `C/C++` 一样的转义字符：
> 演示如何删除剪贴板中所有的换行
![](https://github.com/huanhuanonly/ClipboardFilter/blob/main/Demo/RemoveEscapeCharacters.gif?raw=true)

- 如果你想进行简单的文本替换：
> 演示如何将 `this` 替换为 `you`
![](https://github.com/huanhuanonly/ClipboardFilter/blob/main/Demo/TextReplacement.gif?raw=true)

当然，它还支持其余两种的文本匹配方式，只需要更改 `MatchPattern` 的选项即可。

`MatchPattern` 支持的选项：
- `None`: 正常的文本匹配；
- `Wildcard`: 模式串 `Pattern` 使用 **通配符**；
- `Regex`: 模式串 `Pattern` 使用 **正则表达式**；

## 下载
如果你是 *Windows* 系统，可以直接下载编译后的可执行文件 `ClipboardFilter.exe`，双击运行。

> 您的配置文件将存储在 `%temp%/ClipboardFilter.ini`。

_**By huanhuanonly.**_
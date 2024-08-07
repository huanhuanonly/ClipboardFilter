[![PickLanguage](https://img.shields.io/badge/English-%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87-green
)](README_zh_CN.md)
---

# The ClipboardFilter

> Monitor the clipboard and filter its contents, supporting regular expressions to replace text.

## Description
- If you want to delete some characters:
> Demonstrates how to delete ***all*** characters `i` from the clipboard  
> It will delete ***all*** occurrences of characters in `CharacterList` from the clipboard
![](https://github.com/huanhuanonly/ClipboardFilter/blob/main/Demo/RemoveCharacter.gif?raw=true)

- If you want to use escape characters like `C/C++`:
> Demonstrates how to remove ***all*** line breaks from the clipboard
![](https://github.com/huanhuanonly/ClipboardFilter/blob/main/Demo/RemoveEscapeCharacters.gif?raw=true)

- If you want to do a simple text replacement:
> Demonstrates how to replace `this` with `you`
![](https://github.com/huanhuanonly/ClipboardFilter/blob/main/Demo/TextReplacement.gif?raw=true)

In addition, it also supports the other two text matching methods, just need to change the `MatchPattern` option.

`MatchPattern` supports the following options:
- `None`: Normal text matching;
- `Wildcard`: The pattern string `Pattern` uses *wildcards*;
- `Regex`: The pattern string `Pattern` uses *regular expression*;

## Download
If you are using *Windows* system, you can directly download the compiled executable file `ClipboardFilter.exe` and **double-click** it to _run_.

> Your configuration file will be saved in `%temp%/ClipboardFilter.ini`.

_**By huanhuanonly.**_
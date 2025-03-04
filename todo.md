- add `(a, b)` compatibility: `command(abc, def)` -> `command "abc" "def"`
- with `(a, b)`, make a scope system? Maybe `"abc "def"` is converted to `(abc, def)` instead of the other way around

- add comments: `//` and `/**/`

- add more context to Context: runningFrom, filePath, lineIndex, columnIndex, lineCount

- add Context::runningFrom variable right after making a good file exec function/struct/class
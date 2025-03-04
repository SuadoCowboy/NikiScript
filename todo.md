- add `(a, b)` compatibility: `command(abc, def)` -> `command "abc" "def"`
- with `(a, b)`, make a scope system? Maybe `"abc "def"` is converted to `(abc, def)` instead of the other way around

- look for better name

- add comments: `//` and `/**/`

- add more context to SweatContext: runningFrom, filePath, lineIndex, columnIndex, lineCount

- add SweatContext::runningFrom variable right after making a good file exec function/struct/class
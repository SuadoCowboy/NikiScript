- add `(a, b)` compatibility: `command(abc, def)` -> `command "abc" "def"`
- with `(a, b)`, make a scope system? Maybe `"abc "def"` is converted to `(abc, def)` instead of the other way around

- look for better name

- add comments: `//` and `/**/`

- explain somewhere each first char meaning in commands parameters names

- add more context to SweatContext: runningFrom, filePath, lineIndex, columnIndex, lineCount

- in parse function, get variables in another way rather than using a command and then remove the pData variable from SweatContext

- add SweatContext::runningFrom variable right after making a good file exec function/struct/class
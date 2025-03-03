- add `(a, b)` compatibility: `command(abc, def)` -> `command "abc" "def"`
- with `(a, b)`, make a scope system? Maybe `"abc "def"` is converted to `(abc, def)` instead of the other way around

- look for better name

- add comments: `//` and `/**/`

- explain somewhere each first char meaning in commands parameters names

- add more context to SweatContext: runningFrom, filePath, lineIndex, columnIndex, lineCount

- loop variables: `var !a "echo 4ever lmao"`
- toggle variables and commands: `var +a "echo hi"; var -a "echo bye"`
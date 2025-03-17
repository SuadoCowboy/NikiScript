- create a parser version that uses next() or something(see BindHandler on JikaruEngine to have an idea)

- lineIndex should be on lexer and not on  Context because this makes the Context a requirement for lexer
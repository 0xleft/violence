When functions

```
void io : func_test->arg1 ~
```

tokens will be:

````
Token(void, 12, 5, TYPE)
Token(io, 12, 8, IDENTIFIER)
Token(:, 12, 9, DELIMITER)
Token(fun_test, 12, 19, IDENTIFIER)
Token(>one, 12, 24, IDENTIFIER)
````

where >one is the argument name
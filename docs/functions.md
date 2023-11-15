# Functions (more in depth)

## Builtin functions

### say

Print to stdout

```vio
( say -> "Hello World" )
```

### ask

Read from stdin until newline

```vio
( ask -> "What is your name?" )
```

### read

Read from a file

```vio
( read -> "file.txt" )
```

### write

Write to a file

```vio
( write -> "file.txt" -> "Hello World" )
```

### sheesh

Which stands for execute shell command

```vio
( sheesh -> "ls" )
```

### exit

```vio
( exit )
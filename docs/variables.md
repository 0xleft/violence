# Variables

In Violence variables are declared using a type before them.

Types are:

- `lemon` for numbers
- `word` for strings
- `mood` for booleans

> **Note**
>
> In the future I plan to change lemon to char and have arrays as new type

```c
lemon a : 5
word b : "Hello World"
mood c : "good"
```

You can easely change between types for example when you want to print a number as a string you can do:

```c
lemon a : 5
word b : a
~ here b is now "5"
```

You can also change a string to a number:

```c
word a : "5"
lemon b : a
~ here b is now 5
```

This works for all types.
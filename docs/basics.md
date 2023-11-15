# Basics

Indentations are not required but are cool 😎

## Hello World

A basic hello world would be like this:

```vio
( say -> "Hello World" )
```

## Comments

Comments start with ~

```vio
( say -> "Hello World" ) ~ This is a comment
```

## Variables

More in depth about variables [here](variables.md)

```vio
word a : "Hello World"
lemon b : 5
mood c : "good"
```

## Functions

There are a number of builtin functions in violence.

More in depth about functions [here](functions.md)

In general functions are called like this:

```vio
( function_name -> argument1 -> argument2 -> argument3 )
```

And defiled like this:

```vio
f-> function_name -> argument1 -> argument2 -> argument3 -> void
( say -> argument1 )
( say -> argument2 )
( say -> argument3 )
<-f
```

## If statements

If statement evaluates boolean expression and if it is true then it executes the next line else it skips it.

```vio
mood a : "good"
if a != "bad"
( say -> "a is good" )
```
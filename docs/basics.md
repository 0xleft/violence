# Basics

## Hello World

```vio
( say -> "Hello World" )
```

## Comments

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


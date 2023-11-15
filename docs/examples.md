# Examples

## Hello World

```vio
( say -> "Hello World" )
```

## FizzBuzz

```vio
f-> mod -> number -> mod_number -> lemon
    lemon output : number % mod_number
<-f

f-> fizzbuzz -> count -> max_count -> void
    lemon count = count + 1
    word output = ""

    if ( mod -> count -> 3 ) == 0
    word output = output + "fizz"

    if ( mod -> count -> 5 ) == 0
    word output = output + "buzz"

    word temp : count
    if output == ""
    ( say -> temp )

    if output != ""
    ( say -> output )

    if count <= max_count
    ( fizzbuzz -> count -> max_count )
<-f

( fizzbuzz -> 0 -> 10 )
```

## Importing a file

```vio
( get -> "example/simple.vio" )

( function_from_simple_vio )
```

You can find more examples in [examples folder](https://github.com/0xleft/violence/tree/master/example)
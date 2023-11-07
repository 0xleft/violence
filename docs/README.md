## Documentation for violence (programming language)

### What is violence?

Violence is an interpreted language that has basic types such as 
* `float` which is called `num`
* `bool` which is called `mood`
* `string` which is called `word`

Keywords are:
* `nah` which inverts the value of a `mood`
* `good` which is a `mood` that is `true`
* `bad` which is a `mood` that is `false`
* `say` which prints a `word` to the console
* `ask` which asks the user for input and stores it in a variable
* `cya` deletes variable `use this to free memory`
* `get` imports a file
* `~` makes a line a comment

### Special features

* all variables are global
* all functions return void
* `if` a keyword can be used `if <mood>` and then the next line is executed if the mood is `good`
* there is no recursion limit
* there is no limit on the number of variables
* strings can be concatenated with `+`
* if number is used in a string context it is converted to a string so 2 + "2" : "22"
* if string is used in a number context it is converted to a number so "2" + 2 : 4 (not at all confusing)
* declare variable using `word name : value`
* functions start with `f->` and end with `<-f`
* indentation is not required but is recommended
```violence
f->test : name : num
    say "Hello " + name + "!" + " Your number is " + num
<-f
test "John" 2
```

### How to use violence?

``
violence <filename>.vio
``

### Example

in [example.md](example.md)
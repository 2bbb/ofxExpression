# ofxExpression

## API

### ofxExpression

* `bool parsePN(string PolishNotation)`

Parse expression.

* `float eval(float x = 0.0f, float y = 0.0f, float z = 0.0f)`

Evaluate expression

### Expression (Polish Notation)

#### definition

```
<expr>      ::= <constant> | <var> | <unary_op> <space> <expr> | <binary_op> <space> <expr>
<space>     ::= " "
<constant>  ::= ["-"] <digits> ["."] [<digits>]
<digits>    ::= <digit> | <digit> <digits>
<digit>     ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
<var>       ::= "x" | "y" | "z"
<unary_op>  ::= "sin" | "cos" | "tan" | "asin" | "acos" | "atan" | "abs" | "log" | "sqrt"
<binary_op> ::= "add" | "+" | "sub" | "-" | "mul" | "*" | "div" | "/" | "pow"
```

#### example

```
	x
	3
	-1
	-2.01
    sin x
    sin * 3.141592 x
    add sin x cos y
```

## Update history

### 2015/02/22 ver 0.01 release

## License

MIT License.

## Author

* ISHII 2bit [bufferRenaiss co., ltd.]
* ishii[at]buffer-renaiss.com

## At the last

Please create new issue, if there is a problem.
And please throw pull request, if you have a cool idea!!
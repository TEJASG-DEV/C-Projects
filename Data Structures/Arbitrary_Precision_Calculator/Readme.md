# Arbitrary Precision Calculator Project

This is a program that tries to implement a fraction of the functionality of the bc command in Linux.<br/>
The precision of the calculation is strictly limited to the size of data types. But in Arbitrary Precision Calculators,<br/>
the precision will be limited to the computer's memory on which the program is running.

## Functionalities
* Add<br/>
* Subtract<br/>
* Multiply<br/>
* Divide (Strictly limited to integer division)<br/>

## Build Instructions
To compile the project using makefile, type: 

`make`


### Execution

type: <br/>
`./APC <operand1> <operator> <operand2>` where <operator> will be {+, -, x, /} and the operands can be integer numbers.

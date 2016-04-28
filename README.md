# Tiny C Compiler

## Introduction

Tiny C is a simple compiler which accepts code according to the following BNF:
```
<program> ::= <function>*
<function> ::= <identifier> <parameter list> <top level declaration>* <statement>*  
<parameter list> ::= '(' <variable list> ')'
<top level declaration> ::= <variable list> ';'         
<variable list>  ::= <data type> <identifier> [',' <identifier>]* | <data type> <identifier> '[' <array size> ']'
<data type> ::= 'byte' | 'word'
<statement> ::= '{' <compound statement> '}' | <statement> ';'
<compound statement> ::= <statement>* 
<statement> ::= <if statement>            |
                <while statement>         |  
                <for statement>           |
                <read statement>          |
                <print statement>         |
                <assignment statement>    |
		      <function call statement> |
		      <return statement>        |
		      <null statement> 
<if statement> ::= 'if' '(' <boolean expression> ')' <statement> ['else' <statement>]
<while statement> ::= 'while' '(' <boolean expression> ') ' 						<statement>
<for statement> ::= 'for' '(' identifier '=' <expression> 'to' 
<expression> ')' <statement>
<read statement> ::= 'read' <identifier>
<print statement> :: 'print' <expression> | <string>
                      [',' <expression> | <string> ]*
<assignment statement> ::= <identifier> '=' <expression> ';'
<function call statement> ::= <identifier> '(' [<argument list>] ')' ';'
<argument list> ::= <identifier> [',' <identifier>]*
<return statement> ::= 'return' <expression> ';'
<null statement> ::= ';'
<boolean expression> ::= <relation> [<'|' | '&'> <relation>]*
<relation> ::= <expression> [<'>' | '<' | '=' | '>=' | '<=' > <expression>]
<expression> ::= <term> [<'+' | '-'>  <term>]*
<term> = <factor> [<'*' | '/'> <factor>]*
<factor> = '(' <boolean expression> ')' | <number> | <identifier> | <'true' | 'false'>
```



To begin with, we’ll write the typical “hello world” program:

>Go to the DOS command prompt and type in “edit hello.tc”. Now type in the following two lines of code:

```
Main()

Print(“hello, world”);
```

That’s it. Now compile the program by typing in the command “TCC hello.tc”. If everything is OK, you get the message “Compilation successful”. Now run the program “hello.exe”. 

REM: Unlike C, Tiny C is not case sensitive.

## Variables

Variables in Tiny C are declared the same ways as in C:
```
Byte counter, year = 2004;
Word numbers[5] = {647,7878,34,898,454};
```

REM: identifiers in Tiny C must be entirely alphabetic (so “last_year2” is not allowed) .

## Data types

There are two data types in Tiny C:
-	the “byte” is an integer containing values from 0 to 255
-	the “word” is also an integer containing values from 0 to 65535    

## Expressions

Arithmetic expressions in Tiny C are formed in exactly the same way as in C.

REM: All arithmetic is done unsigned.
REM: Unlike C, there is no “%” operator.

Boolean expressions are also formed like in C, except that “||” = “|”, “&&” = “&” and “= =” = “=”:

```year = 2004 & numbers[counter] = 13```

## Control Constructs

You can choose among three different control constructs in Tiny C:

```
if(year = 2003 | year = 2004)
	counter = counter + 1;

while(true)
	;

for(counter = 0 to 4)
	numbers[counter] = counter * 5;
```	

##Functions

```<function> ::= <identifier> <parameter list> <top level declaration>* <statement>*, ```
e.g.

```
printArr(byte arr[],byte size)
byte counter;
{
	for(counter = 0 to size – 1)
print(“the value of entry no ”,counter,“ is ”            ,arr[counter],endl);
}
		
max(word numberA, word numberB)
	if(numberA > numberB)
		return numberA;
	else
		return numberB;
		```


REM: you don’t have to declare the return type of a function; it is always a “word”.
REM: “print” is a standard library function, as is “read”:





## Example program

```
factorial(byte b)
{

 if(b = 0 | b = 1)
      return 1;
 else
      return b * factorial(b-1);
}


selectionSort(word numbers[], word arraySize)
word i,j,min,temp;
{
  for (i = 0 to arraySize-2)
  {
    min = i;
    for (j = i+1 to arraySize-1)
    {
      if (numbers[j] < numbers[min])
        min = j;
    }
    temp = numbers[i];
    numbers[i] = numbers[min];
    numbers[min] = temp;
  }
}

printarr(word arr[],byte size)
byte i;
{
 for(i = 0 to size-1)
 {
  print(arr[i],endl);
 }
}

main()
byte b; byte counter = 4;word arr[5];
{
for(b = 0 to 4)
{
  		print("enter a value less than 65536:");
  		read(arr[b]);
}

selectionSort(arr,5);

print("the numbers sorted:",endl);
printarr(arr,5);

print("enter a number:");
read(b);

print("the factorial of b is ",factorial(b));

}
```


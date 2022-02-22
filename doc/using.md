# Using

Ranch accepts non-command inputs as expression for compute.

## Navigation
+ [Operators](#operators)
  + [Precedences](#precedences)
+ [Numeric Literals](#numeric-literals)
  + [Integer](#integer)
  + [Float](#float)
+ [Examples](#examples)

<h2 id="operators">Operators</h2>

> Ranch supports process priority.
>
+ `+` Addition.
+ `-` Subtraction.
+ `*` Multiplication.
+ `/` Division.
+ `\` Division with greatest.
+ `^` Exponentiation.
+ `%` Modulo.

<h3 id="precedences">Precedences</h3>
<table>
  <tr>
    <th>Precedence</th>
    <th>Operator(s)</th>
  </tr>
  <tr>
    <td>5</td>
    <td>* / \ %</td>
  </tr>
  <tr>
    <td>4</td>
    <td>+ - ^</td>
  </tr>
  <tr>
    <td>3</td>
    <td>N/A</td>
  </tr>
  <tr>
    <td>2</td>
    <td>N/A</td>
  </tr>
  <tr>
    <td>1</td>
    <td>N/A</td>
  </tr>
</table>

<h2 id="numeric-literals">Numeric Literals</h2>
<h3 id="integer">Integer</h3>

```
35363635
0xFF94
```

<h3 id="float">Float</h3>

```
3.4
0.
363.
```

<h2 id="examples">Examples</h2>

```
Ranch> 5+5+5+5+5
25,000000
Ranch>
```
```
Ranch> 5+5+5+5+5/0
[WARNING FOR ZERO DIVISION]
20,000000
Ranch>
```
```
Ranch> 5.5-5
0,500000
Ranch>
```
```
Ranch> (5-2)*(10+2)
36,000000
Ranch>
```

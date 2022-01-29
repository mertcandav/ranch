# Using

Ranch accepts non-command inputs as expression for compute.

## Operators

> Ranch supports process priority.
>
+ `+` Addition.
+ `-` Subtraction.
+ `*` Multiplication.
+ `/` Division.
+ `^` Exponentiation.
+ `%` Modulo.

### Precedences
  <table>
    <tr>
      <th>Precedence</th>
      <th>Operator(s)</th>
    </tr>
    <tr>
      <td>5</td>
      <td>* / %</td>
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

## Exmaples

```
Ranch> 5+5+5+5+5
25,00000
Ranch>
```
```
Ranch> 5+5+5+5+5/0
[WARNING FOR ZERO DIVISION]
20,00000
Ranch>
```

```
Ranch> 5.5-5
0,50000
Ranch>
```

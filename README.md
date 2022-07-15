# Polynomial Algebraic Expression Class

This is an implementation of a Class for Polynomial Algebraic Expressions (referred to in the following as polynomials).

Requirements are to implement functions for:
1. getting variables in a polynomial
2. getting a map, relating the degrees of a given variable in a polynomial, to the respective coefficients (as polynomials)
3. replacing all occurrences of a variable in a polynomial, with another polynomial
4. printing in a stream a polynomial
5. testing whether two polynomials are equivalent


## Var Class
```cpp
class Var {
private:
    std::string Name;
    int Value;
```
A Var object consists of a Name and a Value. Although the Value member hasn't been used in the code, the initial idea was to provide a means of substituting variables with numerical values, a task more extendedly performed by 
```cpp
friend Expr replace(const Expr& e, const std::map<Var, Expr>& repl);
```
The Var Class also provides operators == and < for comparing Vars, and << for printing Vars.


## Expr Class

First off, a few methods and base utilities were implemented (, along with overloaded operators for sums, subtractions and products of polynomials,
plus a function for raising a polynomial to powers of natural numbers.

Private members of class Expr are:
```cpp
class Expr {
private:
    std::vector<Var> Variables;
    std::vector<int> Coefficients;
    std::vector<std::vector<unsigned> > Exponents;
```
Thus, the polynomial is given by a vector of variables, a vector of coefficients (as many as there are monomials), and a vector (of same size as Coefficients) of a vector of unsigned integers (of same size as Variables), containing the exponents of each variable in each monomial.
In order to implement the required methods/friend functions, some preliminary functionality must be in place. 


## Public methods of class Expr:

```cpp
Expr(std::vector<Var> v, std::vector<int> c, std::vector<std::vector<unsigned> > e)
```
Construct an Expr given variables, coefficients and exponents vectors.

```cpp
Expr()
```
Construct an Expr with all empty vectors.

```cpp
int get_xdegree(const Var x)
```
Get maximum degree of variable x in Expr.

```cpp
int get_xindex(const Var x)
```
Get index of variable x in Expr.Variables 

```cpp
void sort_variables()
```
Sort Variables by variable name, and Exponents accordingly.
```cpp
void sort_xdegree(const Var x)
```
Sort Expr (Coefficients and Exponents) by degree of given variable, in ascending order. 

```cpp
void group()
```
Group all similar monomials in an Expr

```cpp
Expr operator+(const Expr e)
```
Sum one Expr with another
```cpp
Expr operator-(Expr e)
```
Subtract an Expr from another
```cpp
Expr operator*(const Expr e) const
```
Multiply and Expr with another
```cpp
Expr power(const unsigned p) const
```
Raise an Expr to a natural power.


## Friend functions of class Expr:
```cpp
friend std::vector<Var> merge_var(Expr e1, Expr e2);
```
Returns a vector with the variables of e1 and e2 (used for arithmetic operations).
```cpp
friend std::vector<std::vector<unsigned> > merge_var_exponents(Expr e1, Expr e2);
```
Returns an Exponents vector with e1's exponents, plus exponents of 0 for all variables of e2 which are not already in e1 (used for arithmetic operations).
```cpp
friend std::vector<int> vecsum(std::vector<int> a, std::vector<int> b);
```
Sum two vectors of integers of equal size (included in utils.h)
```cpp
friend std::map<unsigned, Expr> get_xcoeffs (Expr& e, const Var& x);
```
```cpp
friend Expr replace(const Expr& e, const std::map<Var, Expr>& repl);
```
Code for this function is quite short thanks to get_xcoeffs, which gives a map of x-coefficients by x-degrees, of the variable to be replaced, in the destinataion Expr. Replacements are then done one by one in a loop across the x-degrees. The same procedure is perforemd for all Vars in the repl map.

```cpp
friend std::ostream &operator<<(std::ostream& os, const Expr& e);
```
```cpp
friend bool equivalent(const Expr& e1, const Expr& e2);
```

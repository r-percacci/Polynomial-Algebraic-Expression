#include <iostream>
#include "polynomial.h"
using namespace std;

//polynomials 1, x, x-1, xy+1, x^2-1, x^2-y, xz, y 

int main()  {
    Var x = Var("x");
    Var y = Var("y");
    Var z = Var("z");
    
    // e = 1
    vector<Var> v1;
    // v1.push_back(x);
    vector<int> c1;
    c1.push_back(1);
    vector<vector<unsigned> > e1;
    Expr E1 = Expr(v1, c1, e1);
    cout << endl << "~ creating a few Expr objects: " << endl;
    cout << "e1 = " << E1 << endl;

    // e = x
    vector<Var> v2;
    v2.push_back(x);
    vector<int> c2;
    c2.push_back(1);
    vector<vector<unsigned> > e2;
    vector<unsigned> e21;
    e21.push_back(1);
    e2.push_back(e21);
    Expr E2 = Expr(v2, c2, e2);
    cout <<  "e2 = " << E2 << endl;

    // e = x - 1
    vector<Var> v3;
    v3.push_back(x);
    vector<int> c3;
    c3.push_back(1);
    c3.push_back(-1);
    vector<vector<unsigned> > e3;
    vector<unsigned> e31;
    e31.push_back(1);
    e3.push_back(e31);
    vector<unsigned> e32;
    e32.push_back(0);
    e3.push_back(e32);
    Expr E3 = Expr(v3, c3, e3);
    cout <<  "e3 = " << E3 << endl;

    // e = xy - 1
    vector<Var> v4;
    v4.push_back(x);
    v4.push_back(y);
    vector<int> c4;
    c4.push_back(1);
    c4.push_back(1);
    vector<vector<unsigned> > e4;
    vector<unsigned> e41;
    e41.push_back(1);
    e41.push_back(1);
    e4.push_back(e41);
    vector<unsigned> e42;
    e42.push_back(0);
    e42.push_back(0);
    e4.push_back(e42);
    Expr E4 = Expr(v4, c4, e4);
    cout <<  "e4 = " << E4 << endl;

    // e = x^2 - 1
    vector<Var> v5;
    v5.push_back(x);
    vector<int> c5;
    c5.push_back(1);
    c5.push_back(-1);
    vector<vector<unsigned> > e5;
    vector<unsigned> e51;
    e51.push_back(2);
    e5.push_back(e51);
    vector<unsigned> e52;
    e52.push_back(0);
    e5.push_back(e52);
    Expr E5 = Expr(v5, c5, e5);
    cout <<  "e5 = " << E5 << endl;

    // e = x^2 - y
    vector<Var> v6;
    v6.push_back(x);
    v6.push_back(y);
    vector<int> c6;
    c6.push_back(1);
    c6.push_back(-1);
    vector<vector<unsigned> > e6;
    vector<unsigned> e61;
    e61.push_back(2);
    e61.push_back(0);
    e6.push_back(e61);
    vector<unsigned> e62;
    e62.push_back(0);
    e62.push_back(1);
    e6.push_back(e62);
    Expr E6 = Expr(v6, c6, e6);
    cout <<  "e6 = " << E6 << endl;

    // e = xz
    vector<Var> v7;
    v7.push_back(x);
    v7.push_back(z);
    vector<int> c7;
    c7.push_back(1);
    vector<vector<unsigned> > e7;
    vector<unsigned> e71;
    e71.push_back(1);
    e71.push_back(1);
    e7.push_back(e71);
    Expr E7 = Expr(v7, c7, e7);
    cout <<  "e7 = " << E7 << endl;

    // e = y
    vector<Var> v8;
    v8.push_back(y);
    vector<int> c8;
    c8.push_back(1);
    vector<vector<unsigned> > e8;
    vector<unsigned> e81;
    e81.push_back(1);
    e8.push_back(e81);
    Expr E8 = Expr(v8, c8, e8);
    cout <<  "e8 = " << E8 << endl;

    cout << endl << "~ examples of overloaded operators +, -, *, power:" << endl;
    cout << "(" << E1 << ")" << " + " << "(" << E2 << ")" << " = " << E1 + E2 << endl;
    cout << "(" << E3 << ")" << " + " << "(" << E1 << ")" << " = " << E3 + E1 << endl;
    cout << "(" << E5 << ")" << " - " << "(" << E3 << ")" << " = " << E5 - E3 << endl;
    cout << "(" << E2 << ")" << " * " << "(" << E5 << ")" << " + " << "(" << E3 << ")" << " = " << E2 * E5 + E3 << endl;
    cout << "(" << E1 << ")" << "^" << "4" << " = " << E1.power(4) << endl;  
    cout << "(" << E1 << " + " << E1 << ")" << "^" << 4 << " = " << (E1 + E1).power(4) << endl;
    cout << "(" << E3 << ")" << "^" << "3" << " = " << E3.power(3) << endl;
    cout << "(" << E4 << ")" << "^" << "0" << " = " << E4.power(0) << endl;
    Expr prod = (E4 + E7).power(2) * E6;
    cout << "(" << E4 << " + " << E7 << ")^2" << " * (" << E6 << ")" << " = " << prod << endl;

    cout << endl << "~ order last polynomial by x-degree: " << endl;
    prod.sort_xdegree(x);
    cout << prod << endl;

    cout << endl << "~ get index of variables in Variables vector, in last polynomial: " << endl;
    cout << "x-index = " << prod.get_xindex(x) << endl;
    cout << "y-index = " << prod.get_xindex(y) << endl;
    cout << "z-index = " << prod.get_xindex(z) << endl;

    cout << "~ get maximum degree of variables in last polynomial: " << endl;
    cout << "x-degree = " << prod.get_xdegree(x) << endl;
    cout << "y-degree = " << prod.get_xdegree(y) << endl;
    cout << "z-degree = " << prod.get_xdegree(z) << endl;

    cout << endl << "ask for index of a variable which is not present in a polynomial: " << endl;
    Var w = Var("w");
    cout << "index of w in " << E6 << " ?" << E6.get_xindex(w) << endl;

    cout << endl << "~ get variables in polynomial " << E4 + E7 << ":" << endl;
    Expr E9 = E4 + E7;
    std::vector<Var> V = E9.get_variables();
    print_vector(V);
    cout << endl;

    cout << endl << "~ equivalence of two expression may be tested:" << endl;
    cout << "are " << "(" << E2 << ")" << " and " << "(" << E3 << ")" << " equivalent? " << equivalent(E2, E3) << endl;
    cout << "are " << "(" << "(" << E2 << ")" << " - " << "(" << E1 << ")" << ")" << " and " << "(" << E3 << ")" << " equivalent? " << equivalent(E2-E1, E3) << endl;
    cout << "are " << "(" << E4 << ")" << " and " << "(" << "(" << E1 << ")" << " + " << "(" << E2 << ")" << " * " << "(" << E8 << ")" << ")" << " equivalent? " << equivalent(E4, E1+E2*E8) << endl;
    cout << "are " << "(" << E3 << ")^2" << " and " << "(" << "(" << E5 << ")" << " + " << "(" << E2 - E2 - E2 - E2 << ")" << " + " << "(" << E1 + E1 << ")" <<")" << " equivalent? " << equivalent(E3.power(2), E5 + E2 - E2 - E2 - E2 + E1 + E1) << endl;
    
    cout << endl << "~ get a map relating x-degrees to x-coefficients in " << "(" << E6 << ")^5 = " << E6.power(5) << ": " << endl;
    Expr E6pow = E6.power(5);
    map<unsigned, Expr> xmap = get_xcoeffs(E6pow, x);
    print_map(xmap);

    cout << endl << "~ replace certain variables with expressions, given a map:" << endl;
    map<Var, Expr> map1;
    map1.insert(pair<Var, Expr>(y, E5));
    cout << "replace y with " << endl;
    print_map(map1);
    cout << "in " << E6 << ": " << endl << replace(E6, map1) << endl;
    map<Var, Expr> map2;
    map2.insert(pair<Var, Expr>(x, E3));
    map2.insert(pair<Var, Expr>(z, E4));
    cout << endl << "replace x and z with " << endl;
    print_map(map2);
    Expr E = replace(E7, map2);
    cout << "in " << E7 << ": " << endl << E << endl;

    return 0;
}
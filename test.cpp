#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>

class Var {
private:
    std::string Name;
    int Value;

public:
    Var();

    Var(std::string name) {
        Name = name;
        Value = 0;
    }

    void setname(std::string name) {
        Name = name;
    }
    void setvalue(int v) {
        Value = v;
    }
    std::string getname() const{
        return Name;
    }
    int getvalue() {
        return Value;
    }

    bool operator<(const Var &v) const
    {
        return Name < v.Name;
    }

    friend class Expr;
};

class Expr {
private:
    std::vector<Var> Variables;
    std::vector<int> Coefficients;
    std::vector<std::vector<int> > Exponents;
    
public:
    Expr(std::vector<Var> v, std::vector<int> c, std::vector<std::vector<int> > e) {
        Variables = v;
        Coefficients = c;
        Exponents = e;
    }

    void print() {
        for (int i = 0; i < Coefficients.size()-1; ++i) {
            std::cout << Coefficients[i] << "*";
            for (int j = 0; j < Variables.size()-1; ++j) {
                std::cout << Variables[j].Name << "^" << Exponents[i][j] << "*";  
            }
            std::cout << Variables[Variables.size()-1].Name << "^" << Exponents[i][Variables.size()-1];
            std::cout <<  " + ";
        }
        std::cout << Coefficients[Coefficients.size()-1] << "*";
        for (int j = 0; j < Variables.size()-1; ++j) {
                std::cout << Variables[j].Name << "^" << Exponents[Coefficients.size()-1][j] << "*";  
            }
        std::cout << Variables[Variables.size()-1].Name << "^" << Exponents[Coefficients.size()-1][Variables.size()-1] << std::endl;
    }

    std::vector<Var> get_variables() const {
        return Variables;
    }

    std::vector<std::vector<int> > get_exponents() const {
        return Exponents;
    }

    std::vector<int> get_coefficients() const {
        return Coefficients;
    }

    friend std::vector<Var> merge_var(Expr e1, Expr e2);
    friend std::vector<std::vector<int> > merge_var_exponents(Expr e1, Expr e2);
    friend std::ostream &operator<<(std::ostream& os, const Expr& e);

    void sort_variables() { // sort Variables and Exponents 
    // getting sorted index list
        std::vector<int> indices(Variables.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(),
        [&](int i, int j) -> bool {
        return Variables[i].getname() < Variables[j].getname();
        });

    // sorting Variables
        std::sort(Variables.begin(), Variables.end());
    // sorting Exponents according to sorted index list
        std::vector<int> * E  = new std::vector<int>[Coefficients.size()];
        for (int i = 0; i < Coefficients.size(); ++i) {
            for (int j = 0; j < Variables.size(); ++j) {
                E[i].push_back(Exponents.at(i).at(indices[j]));
            }
        }
        for (int i = 0; i < Coefficients.size(); ++i) {
            for (int j = 0; j < Variables.size(); ++j) {
                Exponents.at(i).at(j) = E[i].at(j);
            }
        }
        delete[] E;
    }

    Expr operator+(Expr e) {
        //sort variables and relative exponents in both expr
        Expr e1 = *this;
        Expr e2 = e;
        e1.Variables = merge_var(*this, e);
        e2.Variables = merge_var(e, *this);
        e1.Exponents = merge_var_exponents(*this, e);
        e2.Exponents = merge_var_exponents(e, *this);
        e1.sort_variables();
        e2.sort_variables();
        Expr E = e1;
        E.Variables = merge_var(*this, e);
        // push back non-duplicate exponent vector and add coefficients for duplicate exponent vectors
        for (int i = 0; i < e2.Exponents.size(); ++i) {
            bool isduplicate = false;
            for (int j = 0; j < e1.Exponents.size(); ++j) {
                isduplicate = e2.Exponents[i] == e1.Exponents[j];
                if (isduplicate == true) {
                    E.Coefficients.at(j) += e2.Coefficients[i];
                    break;
                }
            }
            if (isduplicate == false) {
                E.Coefficients.push_back(e2.Coefficients[i]);
                E.Exponents.push_back(e2.Exponents[i]);
            }
        }
        return E;
    }

};

std::ostream &operator<<(std::ostream& os, const Expr& e) {
    for (int i = 0; i < e.Coefficients.size()-1; ++i) {
            os << e.Coefficients[i] << "*";
            for (int j = 0; j < e.Variables.size()-1; ++j) {
                os << e.Variables[j].getname() << "^" << e.Exponents[i][j] << "*";  
            }
            os << e.Variables[e.Variables.size()-1].getname() << "^" << e.Exponents[i][e.Variables.size()-1];
            os <<  " + ";
        }
        os << e.Coefficients[e.Coefficients.size()-1] << "*";
        for (int j = 0; j < e.Variables.size()-1; ++j) {
                os << e.Variables[j].getname() << "^" << e.Exponents[e.Coefficients.size()-1][j] << "*";  
            }
        os << e.Variables[e.Variables.size()-1].getname() << "^" << e.Exponents[e.Coefficients.size()-1][e.Variables.size()-1] << std::endl;
    return os;  
}

std::vector<Var> merge_var(Expr e1, Expr e2) {
    std::vector<Var> V = e1.get_variables();

    for (int i = 0; i < e2.get_variables().size(); ++i) {
        bool isduplicate = false;
        for (int j = 0; j < e1.get_variables().size(); ++j) {
            isduplicate = (e2.get_variables()[i].getname() == e1.get_variables()[j].getname());
            if (isduplicate == true)
                break;
        }
        if (isduplicate == false)
            V.push_back(e2.get_variables()[i]);   
    }
    //std::sort(V.begin(),V.end());
    return V;
}

std::vector<std::vector<int> > merge_var_exponents(Expr e1, Expr e2) { // returns vector of exponent vectors for e1 including e2's variables, same ordering as merge_var
    std::vector<std::vector<int> > E = e1.get_exponents();

    for (int i = 0; i < e2.get_variables().size(); ++i) {
        bool isduplicate = false;
        for (int j = 0; j < e1.get_variables().size(); ++j) {
            isduplicate = (e1.get_variables().at(j).getname() == e2.get_variables().at(i).getname());
            if (isduplicate == true)
                break;
        }
        if (isduplicate == false) {
            for (int j = 0; j < e1.get_exponents().size(); ++j) 
                E.at(j).push_back(0);  
        }
    }
    return E;
}

int main() {Var x = Var("x");
Var y = Var("y");
Var z = Var("z");
Var b = Var("b");

std::vector<Var> vars;
vars.push_back(y);
vars.push_back(b);
vars.push_back(z);
vars.push_back(x);

std::vector<int> coef;
coef.push_back(2);
coef.push_back(-1);
coef.push_back(3);

std::vector<int> e1;
e1.push_back(1);
e1.push_back(0);
e1.push_back(2);
e1.push_back(2);

std::vector<int> e2;
e2.push_back(0);
e2.push_back(0);
e2.push_back(5);
e2.push_back(1);

std::vector<int> e3;
e3.push_back(3);
e3.push_back(2);
e3.push_back(0);
e3.push_back(1);

std::vector<std::vector<int> > exp;
exp.push_back(e1);
exp.push_back(e2);
exp.push_back(e3);

Expr E1 = Expr(vars, coef, exp);
std::cout << "unsorted polynomial 1 : ";
std::cout << E1;

Var w = Var("w");

std::vector<Var> vars1;
vars1.push_back(y);
vars1.push_back(w);
vars1.push_back(z);
vars1.push_back(x);

std::vector<int> coef1;
coef1.push_back(1);
coef1.push_back(-5);
coef1.push_back(2);

std::vector<int> e11;
e11.push_back(1);
e11.push_back(0);
e11.push_back(2);
e11.push_back(2);

std::vector<int> e21;
e21.push_back(0);
e21.push_back(1);
e21.push_back(0);
e21.push_back(3);

std::vector<int> e31;
e31.push_back(2);
e31.push_back(2);
e31.push_back(1);
e31.push_back(0);

std::vector<std::vector<int> > exp1;
exp1.push_back(e11);
exp1.push_back(e21);
exp1.push_back(e31);

Expr E2 = Expr(vars1, coef1, exp1);
std::cout << "unsorted polynomial 2: ";
E2.print();

Expr EE = E1 + E2;
std::cout << "sum of polynomials 1 and 2: ";
EE.sort_variables();
EE.print();
return 0;
}
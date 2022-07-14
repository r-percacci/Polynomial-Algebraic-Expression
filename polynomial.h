#include <iostream>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include "utils.h"


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
    std::string getname() const {
        return Name;
    }
    int getvalue() {
        return Value;
    }
    bool operator<(const Var &v) const {
        return Name < v.Name;
    }
    bool operator==(const Var &v) const {
        return Name == v.Name;
    }
    friend std::ostream &operator<<(std::ostream& os, const Var& v);
    friend class Expr;
};

class Monomial {

private:
    int N; // number of Variables
    std::vector<Var> Variables; 
    std::vector<int> Exponents;
    int Coefficient;

public:
    void setnumvar(int n) {
        N = n;
    }
    void setvar(std::vector<Var> v) {
        Variables = v;
    }
    void setexp(std::vector<int> e) {
        Exponents = e;
    }
    void setcoeff(int c) {
        Coefficient = c;
    }
    int getnumvar() {
        return N;
    }
    std::vector<Var> getvar() {
        return Variables;
    }
    std::vector<int> getexp() {
        return Exponents;
    }
    int getcoeff() {
        return Coefficient;
    }

    Monomial();

    Monomial(const Monomial& m) {
        N = m.N;
        Variables = m.Variables;
        Exponents = m.Exponents;
        Coefficient = m.Coefficient;
    }

    Monomial(int n, std::vector<Var> vars, std::vector<int> exp, int coef) {
    N = n; // number of Variables
    Variables = vars;
    Exponents = exp;
    Coefficient = coef;
    }

    friend std::vector<Var> merge_var_mono(Monomial m1, Monomial m2);
    friend std::vector<int> mono_prod_exp(Monomial m1, Monomial m2);
    friend class Expr;

    Monomial operator*(const Monomial& m1) {
        Monomial m2 = m1;
        m2.Variables = merge_var_mono(*this, m1);
        m2.N = m2.Variables.size();
        m2.Exponents = mono_prod_exp(*this, m1);
        m2.Coefficient = this->Coefficient * m1.Coefficient;
        return m2;
    }

    void print() {
        std::cout << Coefficient << "*";
        for (int i = 0; i < N-1; ++i) {
            std::cout << Variables[i].getname() << "^" << Exponents[i] << "*";
        }
        std::cout << Variables[N-1].getname() << "^" << Exponents[N-1];
    }
};

class Expr {
private:
    std::vector<Var> Variables;
    std::vector<int> Coefficients;
    std::vector<std::vector<unsigned> > Exponents;
    
public:
    Expr(std::vector<Var> v, std::vector<int> c, std::vector<std::vector<unsigned> > e) {
        Variables = v;
        Coefficients = c;
        Exponents = e;
    }

    Expr() {
        const std::vector<Var> v0;
        const std::vector<int> c0;
        const std::vector<std::vector<unsigned> > e0;
        Variables = v0;
        Coefficients = c0;
        Exponents = e0;
    }

    friend std::vector<Var> merge_var(Expr e1, Expr e2);
    friend std::vector<std::vector<unsigned> > merge_var_exponents(Expr e1, Expr e2);
    friend std::ostream &operator<<(std::ostream& os, const Expr& e);
    friend std::vector<int> vecsum(std::vector<int> a, std::vector<int> b);
    friend std::map<unsigned, Expr> get_xcoeffs (Expr& e, const Var& x);
    friend Expr replace(const Expr& e, const std::map<Var, Expr>& repl);
    friend bool equivalent(const Expr& e1, const Expr& e2);

    std::vector<Var> get_variables() const {
        return Variables;
    }

    std::vector<std::vector<unsigned> > get_exponents() const {
        return Exponents;
    }

    int get_xdegree(const Var x) {
        int xi = this->get_xindex(x);
        int xdeg = 0;
        for (auto& vec : Exponents) {
            if (vec[xi] >= xdeg)
                xdeg = vec[xi];
        }
        return xdeg;
    }
    
    int get_xindex(const Var x) {
        bool isvar = 0;
        try {
            
            for (auto var : this->get_variables())
                isvar += (x.Name == var.Name);
            if (isvar == 0) { 
                throw (x.Name);
            }
        }
        catch (std::string varname) {
            std::cout << std::endl << "Warning: variable " << varname << " is not among this polynomial's variables. These are: {";
            for (int i = 0; i < this->get_variables().size()-1; ++i) {
                std::cout << this->get_variables()[i].Name << ", ";
            }
            std::cout << this->get_variables()[this->get_variables().size()-1].Name << "}. -1 returned" << std::endl;
        }
        if (isvar == 0)
            return -1;

        int xi = 0;
        while (this->Variables[xi].getname() != x.getname()) 
            xi++;

        return xi;
        }
    
    std::vector<int> get_coefficients() const {
        return Coefficients;
    }

    void sort_variables() { // sort Variables and Exponents by variable name
    // get sorted index list
        std::vector<int> indices(Variables.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(),
        [&](int i, int j) -> bool {
        return Variables[i].getname() < Variables[j].getname();
        });

    // sorting Variables
        std::sort(Variables.begin(), Variables.end());
    // sorting Exponents according to sorted index list
        std::vector<std::vector<unsigned> > E(Coefficients.size());
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
    }

    void sort_xdegree(const Var x) { // sort Monomials by x-degree
        try {
            bool isvar = 0;
            for (auto var : this->get_variables())
                isvar += (x.Name == var.Name);
            if (isvar == 0) { 
                throw (x.Name);
            }
        }
        catch (std::string varname) {
            std::cout << "Error: variable " << varname << " is not among this polynomial's variables. These are: {";
            for (int i = 0; i < this->get_variables().size()-1; ++i) {
                std::cout << this->get_variables()[i].Name << ", ";
            }
            std::cout << this->get_variables()[this->get_variables().size()-1].Name << "} " << std::endl;
        }

        //this->group();

    // getting sorted index list
        int xi = this->get_xindex(x);
        std::vector<int> indices(Coefficients.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(),
        [&](int i, int j) -> bool {
        return Exponents[i][xi] < Exponents[j][xi];
        });

    // sorting Coefficients
        std::vector<int> C(Coefficients.size());
        for (int i = 0; i < Coefficients.size(); ++i) {
            C[i] = Coefficients[indices[i]];
        }
        Coefficients = C;
        
        // for (int i = 0; i < Coefficients.size(); ++i) {
        //     C[i] = Coefficients[i];
        // }
        // for (int i = 0; i < Coefficients.size(); ++i) {
        //     Coefficients[i] = C[indices[i]];
        // }

    // sorting Exponent vectors according to sorted index list
        std::vector<std::vector<unsigned> > E(Coefficients.size());
        for (int i = 0; i < Coefficients.size(); ++i) {
            E[i] = Exponents[indices[i]];
        }
        Exponents = E;

        // for (int i = 0; i < Coefficients.size(); ++i) {
        //     E[i] = Exponents[i];
        // }
        // for (int i = 0; i < Coefficients.size(); ++i) {
        //     Exponents[i] = E[indices[i]];
        // }
    }

    void group() { //group similar monomials in Expr
        bool issimilar = false;
        for (int i = 0; i < Coefficients.size(); ++i) {
            for (int j = i + 1; j < Coefficients.size(); ++j) {
                issimilar = (Exponents[i] == Exponents[j]);
                if (issimilar == true) {
                    Coefficients[i] += Coefficients[j];
                    Coefficients.erase(Coefficients.begin() + j);
                    Exponents.erase(Exponents.begin() + j);
                    --j;
                }
            }
        }
    }

    Expr operator+(const Expr e) {
        // //sort variables and relative exponents in both expr
        Expr e1 = Expr(merge_var(*this, e), this->get_coefficients(), merge_var_exponents(*this, e));
        Expr e2 = Expr(merge_var(e, *this), e.get_coefficients(), merge_var_exponents(e, *this));
        e1.sort_variables();
        e2.sort_variables();
        Expr E = e1;
        // push back non-duplicate exponent vector and add coefficients for similar monomials
        for (int i = 0; i < e2.Exponents.size(); ++i) {
            bool isduplicate = false;
            for (int j = 0; j < e1.Exponents.size(); ++j) {
                isduplicate = e2.Exponents[i] == e1.Exponents[j];
                if (isduplicate == true) {
                    E.Coefficients[j] += e2.Coefficients[i];
                    break;
                }
            }
            if (isduplicate == false) {
                E.Coefficients.push_back(e2.Coefficients[i]);
                E.Exponents.push_back(e2.Exponents[i]);
            }
        }
        E.sort_variables();
        return E;
    }

    Expr operator-(Expr e) {
        // //sort variables and relative exponents in both expr
        Expr e1 = Expr(merge_var(*this, e), this->get_coefficients(), merge_var_exponents(*this, e));
        Expr e2 = Expr(merge_var(e, *this), e.get_coefficients(), merge_var_exponents(e, *this));
        e1.sort_variables();
        e2.sort_variables();
        Expr E = e1;
        // push back non-duplicate exponent vector and add coefficients for similar monomials
        for (int i = 0; i < e2.Exponents.size(); ++i) {
            bool isduplicate = false;
            for (int j = 0; j < e1.Exponents.size(); ++j) {
                isduplicate = e2.Exponents[i] == e1.Exponents[j];
                if (isduplicate == true) {
                    E.Coefficients[j] -= e2.Coefficients[i];
                    break;
                }
            }
            if (isduplicate == false) {
                E.Coefficients.push_back(-e2.Coefficients[i]);
                E.Exponents.push_back(e2.Exponents[i]);
            }
        }
        E.sort_variables();
        return E;
    }
    
    Expr operator*(const Expr e) const {
        //sort variables and relative exponents in both expr
        Expr e1 = Expr(merge_var(*this, e), this->get_coefficients(), merge_var_exponents(*this, e));
        Expr e2 = Expr(merge_var(e, *this), e.get_coefficients(), merge_var_exponents(e, *this));
        e1.sort_variables();
        e2.sort_variables();
        std::vector<int> coef;
        std::vector<std::vector<unsigned> > exp;
        // push back non-duplicate exponent vector and add coefficients for duplicate exponent vectors
        for (int i = 0; i < e1.Coefficients.size(); ++i) {
            for (int j = 0; j < e2.Coefficients.size(); ++j) {
                //E.Coefficients.push_back(e1.Coefficients[i]*e2.Coefficients[j]);
                coef.push_back(e1.Coefficients[i]*e2.Coefficients[j]);
                //std::vector<int> eij = vecsum(e1.Exponents.at(i), e2.Exponents.at(j));
                exp.push_back( vecsum(e1.Exponents[i], e2.Exponents[j]) );
                //E.Exponents.push_back(eij);
            }
        }
        Expr E = Expr(e1.get_variables(), coef, exp);
        E.sort_variables();
        E.group();
        return E;
    }
    
    Expr power(const unsigned p) const {
        Expr result = *this;
        if (p == 0) {
            std::vector<int> C;
            C.push_back(1);
            std::vector<std::vector<unsigned> > E;
            std::vector<unsigned> e(this->Variables.size(), 0);
            E.push_back(e);
            return Expr(this->Variables, C, E);
        }
        else {
            for (unsigned i = 1; i < p; ++i) {
                result = result * (*this);
            }
            return result;
        }
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

};

bool equivalent(const Expr& e1, const Expr& e2) {
    Expr E1 = e1;
    Expr E2 = e2;
    E1.group();
    E2.group();
    E1.sort_variables();
    E1.sort_variables();
    if (E1.get_variables() != E2.get_variables())
        return false;
    else {
        E1.sort_xdegree(E1.get_variables().front());
        E2.sort_xdegree(E1.get_variables().front());
        if (E1.get_coefficients() == E2.get_coefficients() && E1.get_exponents() == E2.get_exponents())
            return true;
        else
            return false;
    }
}

std::map<unsigned, Expr> get_xcoeffs (Expr& e, const Var& x) {
    //each x-coefficient is an Expr without the variable x
    //we look for all monomials with a certain x-degree and sum up the coefficients
    int xdeg = e.get_xdegree(x);
    int xi = e.get_xindex(x);
    std::map<unsigned, Expr> m;
    std::vector<Var> var = e.get_variables();
    var.erase(var.begin()+xi);
    //scanning across all degrees of x
    for (unsigned d = 0; d <= xdeg; ++d) {
        Expr expr_sum = Expr();
        //scanning across all monomials
        int match = 0;
        for (int i = 0; i < e.get_coefficients().size(); ++i) {
            //check that x-degree matches d
            if (e.get_coefficients()[i] != 0 && e.get_exponents()[i][xi] == d) {
                std::vector<int> coef;
                coef.push_back(e.get_coefficients()[i]);
                std::vector<std::vector<unsigned> > exp;
                exp.push_back(e.get_exponents()[i]);
                exp.back().erase(exp.back().begin() + xi);
                //create x-coefficient for this monomial and sum it up
                Expr expr = Expr(var, coef, exp);
                expr_sum = expr_sum + expr;
                match += 1;
            }
        }

        if (match != 0)
            m.insert(std::pair<unsigned, Expr>(d, expr_sum));
    }
    return m;
}

std::ostream &operator<<(std::ostream& os, const Expr& e) {
    if (e.Variables.size() == 0) {  // if variables vector empty, print coefficients
        for (int i = 0; i < e.Coefficients.size()-1; ++i) 
            os << e.Coefficients[i] << " + ";
        os << e.Coefficients[e.Coefficients.size()-1];
        return os;
    }
    if (e.get_coefficients().size() == 0) { // if coefficients vector empty, print 0
        os << 0;
        return os;
    }

    if (e.get_coefficients()[0] < 0) // check sign of first monomial
        os << "- ";

    for (int i = 0; i < e.Coefficients.size(); ++i) {
        if (i > 0) {
            if (e.Coefficients[i-1] == 0) {
                if (e.Coefficients[i] < 0)
                os << " - ";
                else if (e.Coefficients[i] > 0)
                os << " + ";
            }
        }
        if (i == e.Coefficients.size()-1) { // last monomial, no operator at the end (duplicate code)
                ////////////// Coefficient
            if (e.Coefficients[i] == 0) // don't write 0 coefficients
                continue;
            if (e.Coefficients[i] != 1 && e.Coefficients[i] != -1) // write |coefficient| only if != +1 && != -1
                        os << abs(e.Coefficients[i]);
            else { // if |coefficient| = 1, write 1 where all exponents are zero
                int expsum = 0;
                for (int k = 0; k < e.Variables.size(); ++k)
                    expsum += e.Exponents[i][k];
                if (expsum == 0)
                    os << "1";
            }
            
            ////////////// Exponents
            for (int j = 0; j < e.Variables.size(); ++j) { // write variables with exponents for i-th monomial
                if (e.Exponents[i][j] == 0)
                    continue;

                if (e.Exponents[i][j] != 1) 
                    os  << e.Variables[j] << "^" << e.Exponents[i][j];
                else
                    os << e.Variables[j];
            }
            return os;
        }

        ////////////// Coefficient
        if (e.Coefficients[i] == 0) // don't write 0 coefficients
            continue;
        if (e.Coefficients[i] != 1 && e.Coefficients[i] != -1) // write |coefficient| only if != +1 && != -1
                    os << abs(e.Coefficients[i]);
        else { // if |coefficient| = 1, write 1 where all exponents are zero
            int expsum = 0;
            for (int k = 0; k < e.Variables.size(); ++k)
                expsum += e.Exponents[i][k];
            if (expsum == 0)
                os << "1";
        }
        
        ////////////// Exponents
        for (int j = 0; j < e.Variables.size(); ++j) { // write variables with exponents for i-th monomial
            if (e.Exponents[i][j] == 0)
                continue;

            if (e.Exponents[i][j] != 1) 
                os  << e.Variables[j] << "^" << e.Exponents[i][j];
            else
                os << e.Variables[j];
        }

        ////////////// Sign of next monomial
        if (e.Coefficients[i+1] < 0)
            os << " - ";
        else if (e.Coefficients[i+1] > 0)
            os << " + ";
    }

    return os;  
}

std::ostream &operator<<(std::ostream& os, const Var& v) {
    os << v.getname();
    return os;
}

Expr replace(Expr& e, const std::map<Var, Expr>& repl) {
        Expr E = e;
        Expr E0 = Expr();
        for (auto sub : repl) {
            std::map<unsigned, Expr> map = get_xcoeffs(E, sub.first); // coefficients of variable to replace
            for (int i = 0; i <= e.get_xdegree(sub.first); ++i) { 
                if (i == 0)
                    E = E0 + map[i] * (sub.second.power(i));
                else
                    E = E + map[i] * (sub.second.power(i));
            }
        }
        return E;
    }

std::vector<Var> merge_var_mono(Monomial m1, Monomial m2) {
    std::vector<Var> V = m1.getvar();

    // for (int i = 0; i < m2.getnumvar(); ++i) {
    //     bool duplicate = false;
    //     for (int j = 0; j < m1.getnumvar(); ++j) {
    //         duplicate = (m1.getvar().at(j).getname() == m2.getvar().at(i).getname());
    //         if (duplicate == true)
    //             break;
    //     }
    //     if (duplicate == false)
    //         V.push_back(m2.getvar().at(i));     

    for (auto& var2 : m2.getvar()) {
        bool isduplicate = false;
        for (auto& var1 : m1.getvar()) {
            isduplicate = (var1.getname() == var2.getname());
            if (isduplicate == true)
                break;
        }
        if (isduplicate == false)
            V.push_back(var2);   
    }

    return V;
}

std::vector<Var> merge_var(const Expr e1, const Expr e2) {
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

std::vector<std::vector<unsigned> > merge_var_exponents(const Expr e1, const Expr e2) { // returns vector of exponent vectors for e1 including e2's variables, same ordering as merge_var
    std::vector<std::vector<unsigned> > E = e1.get_exponents();
    if (e1.get_variables().size() == 0) {
        for (int i = 0; i < e1.get_coefficients().size(); ++i) {
            std::vector<unsigned> exp_i;
            for (int j = 0; j < e2.get_variables().size(); ++j) {
                exp_i.push_back(0);
            }
        E.push_back(exp_i);
        }
    }
    else {
        for (int i = 0; i < e2.get_variables().size(); ++i) {
            bool isduplicate = false;
            for (int j = 0; j < e1.get_variables().size(); ++j) {
                isduplicate = (e1.get_variables()[j].getname() == e2.get_variables()[i].getname());
                if (isduplicate == true)
                    break;
            }
            if (isduplicate == false) {
                //for (int j = 0; j < e1.get_exponents().size(); ++j) 
                for (auto& vec : E)
                    vec.push_back(0);  
            }
        }
    }
    return E;
}

std::vector<int> mono_prod_exp(Monomial m1, Monomial m2) {

    std::vector<int> c = m1.getexp();

    for (int i = 0; i < m2.getnumvar(); ++i) {
        bool duplicate = false;
        for (int j = 0; j < m1.getnumvar(); ++j) {
            duplicate = (m1.getvar().at(j).getname() == m2.getvar().at(i).getname());
            if (duplicate == true) {
                c.at(j) += m2.getexp().at(i);
                break;
            }
        }
        if (duplicate == false)
            c.push_back(m2.getexp().at(i));     
    }

    return c;
}



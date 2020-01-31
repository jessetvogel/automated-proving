#include "heyting.hpp"

Heyting::Heyting() : True(&T), False(&F) {
    True->name = "True";
    False->name = "False";
}

Heyting::~Heyting() {
    // Delete all elements
    for(auto x : elements)
        delete x;
}

Heyting::Element* Heyting::createElement() {
    auto x = new Heyting::Element();
    elements.push_back(x);
    return x;
}

Heyting::Element* Heyting::createElement(std::string s) {
    auto x = createElement();
    x->name = s;
    return x;
}

Heyting::Element* Heyting::product(std::set<Element*> factors) {
    // If any of the factors is False, we can just return False
    if(std::find(factors.begin(), factors.end(), False) != factors.end())
        return False;
    
    // Any factor of True can be omitted
    factors.erase(True);
    
    // If the list is empty, we can just return True
    if(factors.empty())
        return True;
    
    // If there is only one factor, simply return that element
    if(factors.size() == 1)
        return *(factors.begin());
    
    // If any of the factors are products themselves, replace that factor with its factors
    std::set<Element*> new_factors;
    for(auto f : factors) {
        if(f->type == Element::PRODUCT) {
            auto other = ((Product*) f)->factors;
            new_factors.insert(other.begin(), other.end());
        }
        else {
            new_factors.insert(f);
        }
    }
    
    // If a product with these factors already exists, return it
    for(auto x : elements) {
        if(x->type != Element::PRODUCT)
            continue;
        
        Product* prod = (Product*) x;
        if(new_factors == prod->factors)
            return prod;
    }
    
    // Finally, create the actual product, and return it
    Product* prod = new Heyting::Product(new_factors);
    elements.push_back(prod);
    return prod;
}

Heyting::Element* Heyting::coproduct(std::set<Element*> factors) {
    // If any of the factors is True, we can just return True
    if(std::find(factors.begin(), factors.end(), True) != factors.end())
        return True;
    
    // Any factor of False can be omitted
    factors.erase(False);
    
    // If the list is empty, we can just return False
    if(factors.empty())
        return False;
    
    // If there is only one factor, simply return that element
    if(factors.size() == 1)
        return *(factors.begin());
    
    // If any of the factors are coproducts themselves, replace that factor with its factors
    std::set<Element*> new_factors;
    for(auto f : factors) {
        if(f->type == Element::COPRODUCT) {
            auto other = ((Coproduct*) f)->factors;
            new_factors.insert(other.begin(), other.end());
        }
        else {
            new_factors.insert(f);
        }
    }
    
    // If a coproduct with these factors already exists, return it
    for(auto x : elements) {
        if(x->type != Element::COPRODUCT)
            continue;
        
        Coproduct* coprod = (Coproduct*) x;
        if(new_factors == coprod->factors)
            return coprod;
    }
    
    // Finally, create the actual coproduct, and return it
    Coproduct* coprod = new Heyting::Coproduct(new_factors);
    elements.push_back(coprod);
    return coprod;
}

Heyting::Element* Heyting::exponential(Heyting::Element* b, Heyting::Element* e) {
    // If the base is True, simply return True
    if(b == True)
        return True;
    
    // If the exponent is False, simply return True
    if(e == False)
        return True;
    
    // If the exponent is True, simply return the base
    if(e == True)
        return b;
    
    // If the base is an exponent itself, do some rewriting (i.e. "P => (Q => R)" == "(P ^ Q) => R)")
    if(b->type == Element::EXPONENTIAL) {
        auto exp = (Exponential*) b;
        b = exp->base;
        e = product({ e, exp->exponent });
    }
    
    // If an exponential with same base and exponent is already constructed before, return it
    for(auto x : elements) {
        if(x->type != Element::EXPONENTIAL)
            continue;
        
        Exponential* exp = (Exponential*) x;
        if(b == exp->base && e == exp->exponent)
            return exp;
    }
    
    // Finally, create the actual exponential, and return it
    Exponential* exp = new Heyting::Exponential(b, e);
    elements.push_back(exp);
    return exp;
}

Heyting::Element::Element() : type(ELEMENT) {
}

Heyting::Element::Element(Type t) : type(t) {
}

void Heyting::Element::addArrowFrom(Heyting::Element* x) {
//    if(std::find(arrowsFrom.begin(), arrowsFrom.end(), x) == arrowsFrom.end())
    if(arrowsFrom.find(x) == arrowsFrom.end())
        arrowsFrom.insert(x);
}

void Heyting::Element::addArrowTo(Heyting::Element* x) {
//    if(std::find(arrowsTo.begin(), arrowsTo.end(), elem) == arrowsTo.end())
    if(arrowsTo.find(x) == arrowsTo.end())
        arrowsTo.insert(x);
}

Heyting::Product::Product(std::set<Heyting::Element*> f) : Element(PRODUCT), factors(f) {
    for(auto x : factors) {
        addArrowTo(x);
        x->addArrowFrom(this);
    }
}

Heyting::Coproduct::Coproduct(std::set<Heyting::Element*> f) : Element(COPRODUCT), factors(f) {
    for(auto x : factors) {
        addArrowFrom(x);
        x->addArrowTo(this);
    }
}

Heyting::Exponential::Exponential(Element* b, Element* e) : Element(EXPONENTIAL), base(b), exponent(e) {
}

Heyting::Element* Heyting::negate(Heyting::Element* x) {
    return exponential(False, x);
}

void Heyting::putArrow(Heyting::Element* x, Heyting::Element* y) {
    x->addArrowTo(y);
    y->addArrowFrom(x);
}

bool Heyting::isArrowHelper(std::unordered_set<Heyting::Element*>& marked, Heyting::Element* x, Heyting::Element* y) {
    // Identity arrows
    if(x == y)
        return true;
    
    // By definition of True and False
    if(x == False || y == True)
        return true;
    
    // If x is isomorphic to False, or y is isomorphic to True, there is also an arrow
    if(std::find(x->arrowsTo.begin(), x->arrowsTo.end(), False) != x->arrowsTo.end() ||
       std::find(y->arrowsFrom.begin(), y->arrowsFrom.end(), True) != y->arrowsFrom.end())
        return true;
        
    // Otherwise, try to find a connection
    for(auto e : y->arrowsFrom) {
        if(e == x)
            return true;
        
        if(marked.find(e) == marked.end()) {
            marked.insert(e);
            if(isArrowHelper(marked, x, e))
                return true;
        }
    }
    return false;
}

bool Heyting::isArrow(Heyting::Element* x, Heyting::Element* y) {
    std::unordered_set<Heyting::Element*> marked;
    return isArrowHelper(marked, x, y);
}

void Heyting::clearArrows() {
    // Clear all arrows
    for(auto x : elements) {
        x->arrowsFrom.clear();
        x->arrowsTo.clear();
    }
}










#include <iostream>

std::string Heyting::Element::to_string() {
    return name;
}

std::string Heyting::Product::to_string() {
    std::string s;
    for(auto f : factors) {
        std::string str_f = f->to_string();
        if(str_f.find(" ") != std::string::npos)
            s += "(" + str_f + ")";
        else
            s += str_f;
        s +=  + " ^ ";
    }
    s.pop_back();
    s.pop_back();
    s.pop_back();
    return s;
}

std::string Heyting::Coproduct::to_string() {
    std::string s;
    for(auto f : factors) {
        std::string str_f = f->to_string();
        if(str_f.find(" ") != std::string::npos)
            s += "(" + str_f + ")";
        else
            s += str_f;
        s +=  + " v ";
    }
    s.pop_back();
    s.pop_back();
    s.pop_back();
    return s;
}

std::string Heyting::Exponential::to_string() {
    std::string str_b = base->to_string();
    std::string str_e = exponent->to_string();
    
    if(str_b.find(" ") != std::string::npos)
        str_b = "(" + str_b + ")";
    
    if(str_e.find(" ") != std::string::npos)
        str_e = "(" + str_e + ")";
    
    return str_e + " => " + str_b;
}



#include "prover.hpp"
#include <functional>
#include <utility>
#include <iostream>

bool Prover::implication(Heyting::Element* x, Heyting::Element* y) {
    // Keep track of which implications are tried to be shown, and with what pay
    std::unordered_map<std::pair<Heyting::Element*, Heyting::Element*>, int, hash_pair> map;
    for(int pay = 0;pay < 3; ++pay)
        if(implicationHelper(x, y, map, pay)) {
            std::cout << "Showed (" << x->to_string() << ") => (" << y->to_string() << ") with pay " << pay << std::endl;
            return true;
        }
    
    return false;
}

bool Prover::implicationHelper(Heyting::Element* x, Heyting::Element* y, std::unordered_map<std::pair<Heyting::Element*, Heyting::Element*>, int, hash_pair>& map, int pay) {
    // Require enough pay
    if(pay < 0)
        return false;
    
    // If there is already an arrow, nothing new is to be shown
    if(heyting.isArrow(x, y))
        return true;
    
    // If the implication "x => y" has tried to be shown before (with at least this amount of pay), then don't even bother trying
    auto pair = std::pair<Heyting::Element*, Heyting::Element*>(x, y);
    auto pos = map.find(pair);
    if(pos != map.end() && pos->second >= pay)
        return false;
    map[pair] = pay;
    
    // std::cout << "Question [" << std::to_string(pay) << "]: (" << x->to_string() << ") =(?)> (" << y->to_string() << ")" << std::endl;
  
    // Arrows to PRODUCTS (use definition products)
    if(y->type == Heyting::Element::PRODUCT) {
        auto factors = ((Heyting::Product*) y)->factors;
        bool flag = true;
        for(auto e : factors) {
            if(!implicationHelper(x, e, map, pay)) { // Equivalence, so zero pay
                flag = false;
                break;
            }
        }
        
        if(flag) {
            heyting.putArrow(x, y);
            return true;
        }
    }
    
    // Arrows from COPRODUCTS (use definition coproducts)
    if(x->type == Heyting::Element::COPRODUCT) {
        auto factors = ((Heyting::Coproduct*) x)->factors;
        bool flag = true;
        for(auto e : factors) {
            if(!implicationHelper(e, y, map, pay)) { // Equivalence, so zero pay
                flag = false;
                break;
            }
        }
        
        if(flag) {
            heyting.putArrow(x, y);
            return true;
        }
    }
    
    // Arrows to EXPONENTS (use adjunction)
    if(y->type == Heyting::Element::EXPONENTIAL) {
        auto exp = (Heyting::Exponential*) y;
        auto prod = heyting.product({ x, exp->exponent });
        if(implicationHelper(prod, exp->base, map, pay)) { // Equivalence, so zero pay
            heyting.putArrow(x, y);
            return true;
        }
    }
    
    // Arrows from PRODUCTS (use adjunction)
    if(x->type == Heyting::Element::PRODUCT) {
        auto factors = ((Heyting::Product*) x)->factors;
        for(auto f : factors) {
            auto subset = factors;
            subset.erase(f);
            auto prod = heyting.product(subset);
            auto exp = heyting.exponential(y, f);
            if(implicationHelper(prod, exp, map, pay)) { // Equivalence, so zero pay
                heyting.putArrow(x, y);
                return true;
            }
        }
    }
    
    // Use functoriality of exponentiation
    if(x->type == Heyting::Element::EXPONENTIAL && y->type == Heyting::Element::EXPONENTIAL) {
        auto exp_x = (Heyting::Exponential*) x;
        auto exp_y = (Heyting::Exponential*) y;
        if(exp_x->exponent == exp_y->exponent && implicationHelper(exp_x->base, exp_y->base, map, pay - 1))
            return true;
    }
    
    // If z => y, then it suffices to check that x => z
    for(auto z : y->arrowsFrom)
        if(implicationHelper(x, z, map, pay - 1))
            return true;
    if(implicationHelper(x, heyting.False, map, pay - 1))
        return true;
    
    // If x => z, then it suffices to check that z => y
    for(auto z : x->arrowsTo)
        if(implicationHelper(z, y, map, pay - 1))
            return true;
    if(implicationHelper(heyting.True, y, map, pay - 1))
        return true;
    
    // If x => z_i, then it suffices to show that prod(z_i) => y
    if(implicationHelper(heyting.product(x->arrowsTo), y, map, pay - 1))
        return true;
    
    return false;
}

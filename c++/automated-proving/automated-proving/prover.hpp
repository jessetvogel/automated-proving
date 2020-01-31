#ifndef prover_hpp
#define prover_hpp

#include "heyting.hpp"
#include <unordered_set>
#include "hashpair.hpp"
#include <unordered_map>

class Prover {

    Heyting& heyting;
    
    bool implicationHelper(Heyting::Element*, Heyting::Element*, std::unordered_map<std::pair<Heyting::Element*, Heyting::Element*>, int, hash_pair>&, int);
    
public:

    Prover(Heyting& h) : heyting(h) {};
    
    bool implication(Heyting::Element*, Heyting::Element*);
    
};

#endif

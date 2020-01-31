#ifndef heyting_hpp
#define heyting_hpp

#include <vector>
#include <set>
#include <string>
#include <unordered_set>
#include <algorithm>

class Heyting {

public:
    
    struct Element {
        enum Type { ELEMENT, PRODUCT, COPRODUCT, EXPONENTIAL };
        const Type type;
        std::set<Element*> arrowsFrom, arrowsTo;
        Element();
        void addArrowFrom(Element*);
        void addArrowTo(Element*);
        
        std::string name;
        virtual std::string to_string();
        
    protected:
        Element(Type);
        
    };
    
    struct Product : Element {
        const std::set<Element*> factors;
        Product(std::set<Element*>);
        
        std::string to_string();
    };
    
    struct Coproduct : Element {
        const std::set<Element*> factors;
        Coproduct(std::set<Element*>);
        
        std::string to_string();
    };
    
    struct Exponential : Element {
        Element* const base;
        Element* const exponent;
        Exponential(Element*, Element*);
        
        std::string to_string();
    };
    
private:
    
    Element T, F;
    
    std::vector<Element*> elements;
    
    bool isArrowHelper(std::unordered_set<Heyting::Element*>&, Heyting::Element*, Heyting::Element*);
    
public:
    
    Element* const True;
    Element* const False;
    
    Heyting();
    ~Heyting();
    
    Element* createElement();
    Element* createElement(std::string);
    Element* product(std::set<Element*>);
    Element* coproduct(std::set<Element*>);
    Element* exponential(Element*, Element*);
    
    Element* negate(Element*);
    
    void putArrow(Element*, Element*);
    bool isArrow(Element*, Element*);
    void clearArrows();
    
};

#endif

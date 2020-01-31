#include "tests.hpp"
#include <iostream>
#include <vector>

void Tests::run() {
    std::vector<bool (*)(void)> tests = { &test_1, &test_2, &test_3, &test_4, &test_5, &test_6, &test_7, &test_8, &test_9 };
    size_t total = tests.size();
    size_t succeeded = 0;
    
    for(int i = 0;i < total; ++i) {
        std::cout << "Running test " << (i + 1) << "... " << std::endl;
        if(tests[i]()) {
            succeeded ++;
            std::cout << "Succeeded!" << std::endl;
        }
        else {
            std::cout << "Failed!" << std::endl;
        }
    }
    
    std::cout << "Tests completed." << std::endl << succeeded << " / " << total << " tests succeeded." << std::endl;
}

// ----------------------------------------------------------------
#include "heyting.hpp"

bool Tests::test_1() {
    /*
     * Check for given arrows, non-given arrows, trivial arrows, and identity arrows
     */
    
    Heyting h;
    
    auto P = h.createElement("P");
    auto Q = h.createElement("Q");
    
    h.putArrow(P, Q);
    
    return h.isArrow(P, Q) && h.isArrow(h.False, P) && h.isArrow(P, h.True)
        && !h.isArrow(Q, P) && !h.isArrow(h.True, P) && !h.isArrow(P, h.False)
        && h.isArrow(P, P);
}

// ----------------------------------------------------------------

bool Tests::test_2() {
    /*
     * Check arrows from and to coproducts
     */
    
    Heyting h;
    
    auto P = h.createElement("P");
    auto Q = h.createElement("Q");
    
    auto prod = h.product({ P, Q });
    auto coprod = h.coproduct({ P, Q });
    
    return h.isArrow(prod, P) && h.isArrow(prod, Q) && h.isArrow(P, coprod) && h.isArrow(Q, coprod)
        && !h.isArrow(P, prod) && !h.isArrow(Q, prod) && !h.isArrow(coprod, P) && !h.isArrow(coprod, Q);
}

bool Tests::test_3() {
    /*
     * Check if expressions are optimized
     */
    Heyting h;
    
    auto P = h.createElement("P");
    auto Q = h.createElement("Q");
    auto R = h.createElement("R");
    
    auto P_and_Q_and_R = h.product({ P, Q, R });
    auto P_or_Q_or_R = h.coproduct({ P, Q, R });
    
    bool flag = true;
    
    // Products & coproducts
    flag &= h.product({ h.product({ P, Q }) , R, h.True}) == P_and_Q_and_R;
    flag &= h.coproduct({ h.coproduct({ P, Q }) , R, h.False}) == P_or_Q_or_R;
    
    // Exponentials
    flag &= h.exponential(h.True, P) == h.True;
    flag &= h.exponential(P, h.False) == h.True;
    flag &= h.exponential(P, h.True) == P;
    
    return flag;
}

// ----------------------------------------------------------------
#include "prover.hpp"

bool Tests::test_4() {
    /*
     * Given:
     *  P => R
     *  Q => S
     *
     * Prove:
     *  (P ^ Q) => (R ^ S)
     *  (P v Q) => (R v S)
     *
     * Also check if implied arrows are stored in the Heyting algebra
     */
    
    Heyting h;
    
    auto P = h.createElement("P");
    auto Q = h.createElement("Q");
    auto R = h.createElement("R");
    auto S = h.createElement("S");
    
    h.putArrow(P, R);
    h.putArrow(Q, S);
    
    auto P_and_Q = h.product({ P, Q });
    auto R_and_S = h.product({ R, S });
    
    auto P_or_Q = h.coproduct({ P, Q });
    auto R_or_S = h.coproduct({ R, S });
    
    
    Prover prover(h);
    return prover.implication(P_and_Q, R_and_S) && h.isArrow(P_and_Q, R_and_S) &&
           prover.implication(P_or_Q, R_or_S)   && h.isArrow(P_or_Q, R_or_S);
}

bool Tests::test_5() {
    /*
     * Distributive law
     *
     * Prove:
     *  P ^ (Q v R) = (P ^ Q) v (P ^ R)
     */
    Heyting h;
    
    auto P = h.createElement("P");
    auto Q = h.createElement("Q");
    auto R = h.createElement("R");
    
    auto P_and_Q = h.product({ P, Q });
    auto P_and_R = h.product({ P, R });
    auto Q_or_R  = h.coproduct({ Q, R });
    auto LHS = h.product({ P, Q_or_R });
    auto RHS = h.coproduct({ P_and_Q, P_and_R });
    
    Prover prover(h);
    return prover.implication(LHS, RHS) && prover.implication(RHS, LHS);
}

bool Tests::test_6() {
    /*
     * Prove:
     *  P v ~P = False
     */
    Heyting h;
    
    auto P = h.createElement("P");
    auto not_P = h.negate(P);
    auto P_and_not_P = h.product({ P, not_P });
    
    Prover prover(h);
    return prover.implication(P_and_not_P, h.False) && prover.implication(h.False, P_and_not_P);
}

bool Tests::test_7() {
    /*
     *
     */
    Heyting h;
    
    auto P = h.createElement("P");
    auto Q = h.createElement("Q");
    
    auto not_P = h.negate(P);
    auto P_or_Q = h.coproduct({ P, Q });
    
    h.putArrow(h.True, P_or_Q);
    h.putArrow(h.True, not_P);
    
    Prover prover(h);
    return prover.implication(h.True, Q);
}

bool Tests::test_8() {
    /*
     * Given:
     *  P => Q
     *
     * Prove:
     *  (R => P) => (R => Q)
     */
    Heyting h;
    
    auto P = h.createElement("P");
    auto Q = h.createElement("Q");
    auto R = h.createElement("R");
    
    h.putArrow(P, Q);
    
    Prover prover(h);
    return prover.implication(h.exponential(P, R), h.exponential(Q, R));
}

bool Tests::test_9() {
    /*
     * Exercise 1.4.2 of Logic Primer - Colin Allen & Michael Hand
     *
     */
    Heyting h;
    
    auto P = h.createElement("P");
    auto Q = h.createElement("Q");
    auto R = h.createElement("R");
    auto S = h.createElement("S");
    auto T = h.createElement("T");
    auto U = h.createElement("U");
    
    Prover prover(h);
    bool flag = true;
    
    // S1
    h.clearArrows();
    h.putArrow(h.True, h.coproduct({ P, h.negate(R) }));
    h.putArrow(h.negate(R), S);
    h.putArrow(h.True, h.negate(P));
    if(!prover.implication(h.True, S)) {
        std::cout << "Failed S1." << std::endl;
        flag = false;
    }
    
    // S2
    h.clearArrows();
    h.putArrow(h.True, h.coproduct({ P, h.negate(R) }));
    h.putArrow(h.negate(R), S);
    h.putArrow(h.True, h.negate(P));
    if(!prover.implication(h.True, h.product({ S, h.negate(P) }))) {
        std::cout << "Failed S2." << std::endl;
        flag = false;
    }
    
    // S3
    h.clearArrows();
    h.putArrow(P, h.negate(Q));
    h.putArrow(h.coproduct({ h.negate(Q), R }), h.negate(S));
    h.putArrow(h.True, h.product({ P, T }));
    if(!prover.implication(h.True, h.negate(S))) {
        std::cout << "Failed S3." << std::endl;
        flag = false;
    }
    
    // S4
    h.clearArrows();
    h.putArrow(h.True, h.product({ P, h.product({ Q, R }) }));
    h.putArrow(h.product({ P, R }), h.negate(S));
    h.putArrow(h.True, h.coproduct({ S, T }));
    if(!prover.implication(h.True, T)) {
        std::cout << "Failed S4." << std::endl;
        flag = false;
    }
    
    // S5
    h.clearArrows();
    h.putArrow(P, Q);
    h.putArrow(P, R);
    h.putArrow(h.True, P);
    if(!prover.implication(h.True, h.product({ Q, R }))) {
        std::cout << "Failed S5." << std::endl;
        flag = false;
    }
    
    // S6
    h.clearArrows();
    h.putArrow(h.True, P);
    h.putArrow(h.True, h.coproduct({ Q, R }));
    h.putArrow(h.True, h.coproduct({ h.negate(R), S }));
    h.putArrow(h.True, h.negate(Q));
    if(!prover.implication(h.True, h.product({ P, S }))) {
        std::cout << "Failed S6." << std::endl;
        flag = false;
    }
    
    // S7
    h.clearArrows();
    h.putArrow(h.True, h.negate(P));
    h.putArrow(h.coproduct({ R, h.negate(P) }), h.coproduct({ P, Q }));
    h.putArrow(h.coproduct({ P, Q }), h.coproduct({ R, h.negate(P) }));
    if(!prover.implication(h.True, Q)) {
        std::cout << "Failed S7." << std::endl;
        flag = false;
    }
    
    // S8
    h.clearArrows();
    h.putArrow(h.product({ h.exponential(P, Q), h.exponential(Q, P) }), R);
    h.putArrow(h.True, h.exponential(P, Q));
    h.putArrow(h.True, h.exponential(Q, P));
    if(!prover.implication(h.True, R)) {
        std::cout << "Failed S8." << std::endl;
        flag = false;
    }
    
    // S9
    h.clearArrows();
    h.putArrow(h.negate(P), h.product({ Q, R }));
    h.putArrow(h.coproduct({ h.negate(P), S }), h.negate(T));
    h.putArrow(h.True, h.product({ U, h.negate(P) }));
    if(!prover.implication(h.True, h.product({ h.product({ U, R}), h.negate(T) }))) {
        std::cout << "Failed S9." << std::endl;
        flag = false;
    }
    
    // S10
    h.clearArrows();
    h.putArrow(h.product({ h.coproduct({ Q, R }), h.negate(S) }), T);
    h.putArrow(h.True, h.product({ Q, U }));
    h.putArrow(h.True, h.coproduct({ h.negate(S), h.negate(U) }));
    if(!prover.implication(h.True, h.product({ T, U }))) {
        std::cout << "Failed S10." << std::endl;
        flag = false;
    }
    
    return flag;
}

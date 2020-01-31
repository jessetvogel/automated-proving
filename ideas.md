## Automated Theorem Proving

#### Category of Statements

The category of <u>Statements</u>. Arrows are implications. Isomorphisms are equivalences. It is a *preorder* (if not, things will get quite confusing). It is even a *Heyting algebra*: $\textbf{False}​$ is the initial object, and $\textbf{True}​$ is the terminal object, and it is cartesian closed, where exponentiation is given by implication. Products are given by the *and* operation, coproducts by *or*. We consider a statement $P​$ *true* if there is an arrow $\textbf{True} \to P​$, and *false* if there is an arrow $P \to \textbf{False}​$. The statement $P \implies Q​$ corresponds to $Q^P​$, i.e. $R \to (Q^P) \iff R \wedge P \to R​$. *Negation* is simply defined as $\neg P := \textbf{False} \to  P​$, which is a contravariant functor. DeMorgan's laws can (partially) be inferred, and so can (partially) the equalities $P \vee \neg P \simeq \textbf{True}​$ and $\neg(P \wedge \neg P) \simeq \textbf{True}​$. (whether partially or completely depends on whether or not we are working intuitively or not.. The *Double-Negation-Elimination*-rule can be added optionally to obtain classical logic.)

Theorems, propositions, lemmas, etc. are a rule to obtain arrows in <u>Statements</u>. They consist of a *setting* (a list of types to which the rule can be applied), and two statements containing possible placeholder variables (domain and codomain of the arrow).

Note that even if $P \vee \neg P \simeq \textbf{True}​$, it need not be that $P \simeq \textbf{True}​$ or $\neg P \simeq \textbf{True}​$. Taking a category with objects $\{ \textbf{True}, \textbf{False}, A, \neg A \}​$ with obvious arrows does satisfy all assumptions, but we do not have $A \simeq \textbf{True}​$ or $\neg A \simeq \textbf{True}​$.

**Remark**: when deducing, it seems only necessary to store (i.e. in memory) a newly deduced arrow, when it cannot be immediately obtained from composition of other arrows.

#### Introducing quantifiers

- Statements of the form $\forall_x \varphi(x)$ can be characterized by $Q \overset{X}{\to} \forall_x \varphi(x) \iff \phi(Q) \overset{Y}{\to} \varphi(x)$, where $\phi : X \to Y$ and the *context* $Y$ is $X$ with an additional element $x$. Similarly, statements of the form $\exists_x \varphi(x)$ can be characterized by $\exists_x \varphi(x) \overset{X}{\to} Q \iff \varphi(x) \overset{Y}{\to} \phi(Q)$. Note that we thus can have multiple *contexts*, each having their own elements and arrows (i.e. an arrow $P \to Q$ may exist in $X$ but not in $Y$). Let's write $\phi : X \to Y$ if $Y$ is an extension of $X$. This is a functor: $\phi(P \to Q) = \phi(P) \to \phi(Q)$. Hence, $\forall_x$ and $\exists_x$ are actually functors $Y \to X$, and we have adjunctions $\phi \dashv \forall_x$ and $\exists_x \dashv \phi$. Note that $\phi$ should be *full* i.e. $\phi(P) \to \phi(Q)$ should imply $P \to Q$.

- Alternatively, we can regard $\forall_x P(x)​$ as the product $\prod_{x} P(x)​$, as it is implied by any $Q​$ whenever $Q​$ implies all $P(x)​$. Similarly, we regard/define $\exists_x P(x)​$ as the coproduct $\coprod_x P(x)​$, as it implies some $Q​$ iff all $P(x)​$ imply $Q​$. I think this is somewhat easier.

#### Category of Types

There is the category of <u>Types</u>. Objects in here can be specified explicitly, and some might exist by default by the program (such as <u>Statements</u> and <u>Types</u> itself *(?)*). *Products* and *coproducts* exist (pairs of types and joined types). Arrows are *function symbols*, i.e. maps from some type $T_1$ to a type $T_2$. We can view a type $T$ as a discrete category itself (objects are all "things" of type $T$, and only identity arrows). Then $\forall$- and $\exists-$statements naturally become limits and colimits of some diagram $T \Rightarrow \;   $<u>Statements</u>.

Note that $\forall_{x \in T_1 \amalg T_2} P(x) \iff \forall_{x \in T_1} P(x) \prod \forall_{x \in T_2} P(x)​$ and that $\exists_{x \in T_1 \amalg T_2} P(x) \iff \exists_{x \in T_1} P(x) \coprod \exists_{x \in T_2} P(x)​$. *The limit of a coproduct of functors is the product of the limits* and *the colimit of a coproduct of functors is the coproduct of the colimits*. 


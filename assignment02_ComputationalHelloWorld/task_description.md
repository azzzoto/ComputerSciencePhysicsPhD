# Computational Hello World
1. Using an interpreted language of your choice, create a code that produces the following vector sum:
$\vec{d} = a\vec{x}+\vec{y}$
    
    * $a$ is a scalar and $d, x, y$ are vectors of dimension $N$, with $N = 10, 10^6, 10^8$.
    * Choose the following values $a = 3, x = 0.1$ (for all the elements) and $y = 7.1$ (for all the elements).

    * Find a way to test that all the elements of $d$ are equal to the sum of $ax+y = 7.4$

2) Repeat point 1 using a compiled language of your choice

3) Use both a compiled and interpreted language to solve the following matrix multiplication
$C = AB \to c_{ij} = \sum_{k=1}^{N}a_{ik}b_{kj}$

    * $C, A, B$ are $N \times N$ matrices with $N = 10, 100, 10000$. $A$ and $B$ are matrices with values $a_{ij} = 3$ and $b_{ij} = 7.1$
    
    * Find a way to test that all the elements of $C$ are equal to the product $ab = 21.3$

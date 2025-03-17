# Task02: Computational Hello World
Author: Giovanni Piccolo
Date: 17/03/25

## Exercise 1
### Task Description
> Using an interpreted language of your choice, create a code that produces the following vector sum: $\vec{d} = a\vec{x}+\vec{y}$

### Source Code
Here is the source code for my Julia implementation

```julia
# function to compute the product of a, x, and y
function compute_product(a, x, y)
    return a .* x .+ y
end

# Parameters
N = [10, 1e6, 1e8]
a = 3.0
x = 0.1
y = 7.1

# loop over the different values of N
for n in N
    # Measure execution time
    time_result = @timed begin
        d = compute_product(a, x, y)
        if all(d .== a * x + y)
            println("N = $n: all the elements of d are equal to $(a * x + y)")
        else
            println("N = $n: NOT all the elements of d are equal to $(a * x + y)")
        end
    end
    println("Execution time: $(time_result.time) s\n")
end


```

### Ex.1 Ouput

```
N = 10.0: all the elements of d are equal to 7.3999999999999995
Execution time: 0.047342666 s

N = 1.0e6: all the elements of d are equal to 7.3999999999999995
Execution time: 0.000223357 s

N = 1.0e8: all the elements of d are equal to 7.3999999999999995
Execution time: 3.0802e-5 s

```
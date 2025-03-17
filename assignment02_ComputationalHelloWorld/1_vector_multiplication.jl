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

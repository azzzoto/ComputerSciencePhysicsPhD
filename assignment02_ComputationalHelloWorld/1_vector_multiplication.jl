# function to compute the product of a, x, and y
function compute_product(a, x, y)
    return a .* x .+ y
end

# Parameters
DIMS = [Int(10), Int(1e6), Int(1e8)]
a = 3.0
expected_value = a*0.1+7.1
# loop over the different values of N
for N in DIMS
    # Measure execution time
    time_result = @timed begin

        x = fill(3, N)
        y = fill(7.1, N)

        d = compute_product(a, x, y)
        if all(d .== a * x + y)
            println("N = $N: all the elements of d are equal to $(expected_value)")
        else
            println("N = $N: NOT all the elements of d are equal to $(expected_value)")
        end
    end
    println("Execution time: $(time_result.time) s\n")
end

# parameters
N = [10, 100, 10_000]

# loops over the different values of N
for n in N 
    time_result = @timed begin

        # matrix definition and multiplication
        A = 3 .* ones(n, n)
        B = 7.1 .* ones(n, n)
        C = A .* B

        expected_value = 3 * 7.1  

        # test the result
        if all(C .== expected_value) 
            println("N = $n: all the elements of d are equal to $(expected_value)")
        else
            println("N = $n: NOT all the elements of d are equal to $(expected_value)")
        end
    end

    println("Execution time: $(time_result.time) s\n")
end
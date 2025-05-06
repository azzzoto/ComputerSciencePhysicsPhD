using Test

# function to compute the product of a, x, and y
function daxpy(a, x, y)
    return a .* x .+ y
end

# ------- PARAMETERS ------
a = 3.0
DIMS = [Int(10), Int(1e6), Int(1e8)]
expected_value = 3.0*0.1+7.1

# ------- UNIT TESTS -------
test_x = [0.1, 0.2, 0.3]
test_y = [7.1, 7.2, 7.3]
expected_result = [a * 0.1 + 7.1, a * 0.2 + 7.2, a * 0.3 + 7.3]

test_d = daxpy(a, test_x, test_y)

# Output length should match input length
@test length(test_d) == length(test_x)
# Output values should match expected results
@test test_d == expected_result 

# Edge case: empty input
empty_x = Float64[]
empty_y = Float64[]
empty_result = daxpy(a, empty_x, empty_y)

# Output should be empty for empty input
@test isempty(empty_result) 

# Edge case: single element input
single_x = [0.1]
single_y = [7.1]
single_result = daxpy(a, single_x, single_y)

# Output should match expected result for single element input
@test single_result == [a * 0.1 + 7.1] 

# ------- MAIN PROGRAM -------
# loop over the different values of N
for N in DIMS
    # Measure execution time
    time_result = @timed begin

        x = fill(0.1, N)
        y = fill(7.1, N)

        d = daxpy(a, x, y)

        # Check if all elements of d are equal to expected_value
        @test all(d .== expected_value)

    end
    println("Execution time: $(time_result.time) s\n")
end

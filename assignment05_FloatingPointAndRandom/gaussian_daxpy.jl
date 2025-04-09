using Random

# daxpy: d = a * x + y
function daxpy(a, x, y)
    return a .* x .+ y
end

# write the result vector d to a file
function write_daxpy_vector_to_file(filename, d)
    open(filename, "w") do file
        write(file, join(d, "\n"))
    end
end

# write the results to a file: N, time, d, a, x, y
function write_results_to_file(filename, N, time_result, d, a, x, y)
    open(filename, "w") do file
        write(file, "N = $N\n")
        write(file, "t = $(time_result.time) s\n")
        write(file, "d = $(d[1])\n")
        write(file, "a = $a\n")
        write(file, "x = $(x[1])\n")
        write(file, "y = $(y[1])\n")
    end
end

# fill a vector of size N with a gaussian random variable

# NB: here to be honest I was not shure if x and y should 
# have the same gaussian random variable in all N elements
# or if they should have different ones
# I decided to go with the same one for all N elements
# because on the task05 pdf it says <<fill the vectors x and y 
# with a gaussian random variable>> (SINGULAR)
function fill_vector_with_gaussian_random_numbers(N)
    random_number = randn()
    return fill(random_number, N)
end

function main(N, a)
    time_result = @timed begin
        x = fill_vector_with_gaussian_random_numbers(N)
        y = fill_vector_with_gaussian_random_numbers(N)
        d = daxpy(a, x, y)

        # take first element of the vectors
        expected_value = a * x[1] + y[1] 

        if all(d .== expected_value)
            println("N = $N: all the elements of d are equal to $(expected_value)")
        else
            println("N = $N: NOT all the elements of d are equal to $(expected_value)")
        end

    end
        write_daxpy_vector_to_file("daxpy_result.dat", d)
        write_results_to_file("results.dat", N, time_result, d, a, x, y)

        println("File 'daxpy_result.dat' written")
        println("File 'results.dat' written")
end

if abspath(PROGRAM_FILE) == @__FILE__
    if length(ARGS) == 0
        N = Int(1e6)
        a = 3.0
        d = main(N, a)

    elseif length(ARGS) == 2
        N = Int(parse(Int, ARGS[1]))
        a = Float64(parse(Float64, ARGS[2]))
        d = main(N, a) 

    else
        println("Usage WITHOUT arguments: julia gaussian_daxpy.jl\n")
        println("Usage WITH arguments: julia gaussian_daxpy.jl [<N> <a>]\n")
    end
end
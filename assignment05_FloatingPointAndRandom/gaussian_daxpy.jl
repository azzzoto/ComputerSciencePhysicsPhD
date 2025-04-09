function daxpy(a, x, y)
    return a .* x .+ y
end

# write the result vector d to a file
function write_to_file(filename, d)
    open(filename, "w") do file
        write(file, join(d, "\n"))
    end
end

#function fill_vector_with_gaussian_random_numbers(N)
#    return randn!(zeros(N))
#end

function assert_gaussian_distribution(d)
    pass
end

function main(N, a, x, y)
    time_result = @timed begin
        x = fill_vector_with_gaussian_random_numbers(N)
        y = fill_vector_with_gaussian_random_numbers(N)
        d = daxpy(a, x, y)

        expected_result = a * x[0] + y[0]

        if all(d .== expected_value)
            println("N = $N: all the elements of d are equal to $(expected_value)")
        else
            println("N = $N: NOT all the elements of d are equal to $(expected_value)")
        end
        
        write_to_file("daxpy_result.dat", d)
        println("daxpy_result.dat written")
    end
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
        println("Usage WITH arguments: julia gaussian_daxpy.jl <N> <a> <x elemnts> <y elements>\n")
    end
end

# TASK: compute z = a*x+y, where
# a = 3, x = [1,....,1], y = [4,....,4] and N = 20
# and write the result to a file called output.txt

# function to comput the requested product
function compute_scalar_product(a, x, y)
    return a .* x .+ y
end

# function to write the result to a file
function write_to_file(filename, z)
    open(filename, "w") do file
        write(file, join(z, "\n"))
    end
end

# Parameters
a = 3.0
N = 20
x = ones(N)   # Vector x with all ones
y = fill(4.0, N)  # vector y with all elements equal to 4

# compute the scalar product
z = compute_scalar_product(a, x, y)

# write the result to a file
write_to_file("output.txt", z)

println("Result saved in 'output.txt'")

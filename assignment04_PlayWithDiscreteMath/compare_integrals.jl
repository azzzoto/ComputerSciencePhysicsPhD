using DataFrames
using Printf

# Set precision to 20 significant digits
setprecision(20)

function f(x::BigFloat)
    return exp(x) * cos(x)
end

function true_value()
    return (exp(BigFloat(pi)/2) - 1) / 2
end

# Compute the integral using trapezoidal method
function ComputeIntegralTrapeziodal(x_inf::BigFloat, x_sup::BigFloat, N::Int)
    h = (x_sup - x_inf) / BigFloat(N)
    sum = BigFloat(0.0)

    # Sum the internal points
    for i in 1:N-1
        sum += f(x_inf + BigFloat(i) * h)    
    end

    # Add the extreme points with weight 1/2
    return h * (f(x_inf) / BigFloat(2) + sum + f(x_sup) / BigFloat(2))
end

function read_integration_results(file_path::String)
    # Open file and read all lines
    lines = readlines(file_path)

    # Extract N and integration interval
    N = nothing
    x_inf = nothing
    x_sup = nothing

    for line in lines
        if occursin("N =", line)
            N = parse(Int, split(line, "=")[end])
        elseif occursin("Integration interval", line)
            parts = split(line, "[")[2]
            interval_str = split(parts, "]")[1]
            x_bounds = split(interval_str, ",")
            x_inf = parse(BigFloat, strip(x_bounds[1]))
            x_sup = parse(BigFloat, strip(x_bounds[2]))
        end
    end

    # Find index where tabular data starts
    start_idx = findfirst(x -> occursin("----", x), lines)
    data_lines = lines[(start_idx + 1):end]

    # Parse data into DataFrame
    method = String[]
    result = BigFloat[]
    rel_error = BigFloat[]
    time = Float64[]

    for line in data_lines
        if isempty(strip(line))
            continue
        end
        fields = split(strip(line))
        push!(method, fields[1])
        push!(result, parse(BigFloat, fields[2]))
        push!(rel_error, parse(BigFloat, fields[3]))
        push!(time, parse(Float64, fields[4]))
    end

    df = DataFrame(Method=method, Result=result, RelativeError=rel_error, Time=time)

    return df, N, x_inf, x_sup
end

# Read results from C file
df, N, x_inf, x_sup = read_integration_results("c_integration_results.dat")

# Compute integral with Julia
start_time = time()
julia_integral = ComputeIntegralTrapeziodal(x_inf, x_sup, N)
julia_time = time() - start_time

# Get C result from DataFrame
c_integral = df[df.Method .== "Trapezoidal", :Result][1]
true_val = true_value()

# Compute errors
absolute_error_c_true = abs(c_integral - true_val)
relative_error_c_true = abs(c_integral / true_val - 1)

absolute_error_julia_true = abs(julia_integral - true_val)
relative_error_julia_true = abs(julia_integral / true_val - 1)

absolute_error_c_julia = abs(julia_integral - c_integral)

# Print results in a clear format
println("\nIntegration Results Comparison")
println("--------------------------------")
println("Parameters:")
println("N = $N")
println("Integration interval: [$x_inf, $x_sup]")
println("\nC vs True Value:")
println("C Integral: $(@sprintf("%.20f", c_integral))")
println("True Value: $(@sprintf("%.20f", true_val))")
println("Absolute Error: $(@sprintf("%.20f", absolute_error_c_true))")
println("Relative Error: $(@sprintf("%.20f", relative_error_c_true))")
println("\nJulia vs True Value:")
println("Julia Integral: $(@sprintf("%.20f", julia_integral))")
println("True Value: $(@sprintf("%.20f", true_val))")
println("Absolute Error: $(@sprintf("%.20f", absolute_error_julia_true))")
println("Relative Error: $(@sprintf("%.20f", relative_error_julia_true))")
println("\nC vs Julia Comparison:")
println("Absolute Error: $(@sprintf("%.20f", absolute_error_c_julia))")
println("\nExecution Times:")
println("C: $(df[df.Method .== "Trapezoidal", :Time][1]) seconds")
println("Julia: $julia_time seconds")

# Save results to file
open("julia_integration_results.dat", "w") do io
    println(io, "# Integration Results Comparison")
    println(io, "# N = $N")
    println(io, "# Integration interval: [$x_inf, $x_sup]")
    println(io, "#")
    println(io, "# C vs True Value:")
    println(io, "C_Integral=$(@sprintf("%.20f", c_integral))")
    println(io, "True_Value=$(@sprintf("%.20f", true_val))")
    println(io, "C_Absolute_Error=$(@sprintf("%.20f", absolute_error_c_true))")
    println(io, "C_Relative_Error=$(@sprintf("%.20f", relative_error_c_true))")
    println(io, "#")
    println(io, "# Julia vs True Value:")
    println(io, "Julia_Integral=$(@sprintf("%.20f", julia_integral))")
    println(io, "Julia_Absolute_Error=$(@sprintf("%.20f", absolute_error_julia_true))")
    println(io, "Julia_Relative_Error=$(@sprintf("%.20f", relative_error_julia_true))")
    println(io, "#")
    println(io, "# C vs Julia Comparison:")
    println(io, "C_Julia_Absolute_Error=$(@sprintf("%.20f", absolute_error_c_julia))")
    println(io, "#")
    println(io, "# Execution Times:")
    println(io, "C_Time=$(df[df.Method .== "Trapezoidal", :Time][1])")
    println(io, "Julia_Time=$julia_time")
end

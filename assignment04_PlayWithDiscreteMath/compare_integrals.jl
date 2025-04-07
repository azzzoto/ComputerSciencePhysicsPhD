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
absolute_error_c_julia = abs(julia_integral - c_integral)
relative_error_c_julia = abs(julia_integral / c_integral - 1)

# Compute errors with respect to true value
absolute_error_julia_true = abs(julia_integral - true_val)
relative_error_julia_true = abs(julia_integral / true_val - 1)

# Create final DataFrame with all results
final_df = DataFrame(
    Method = ["C", "Julia", "True Value"],
    Result = [c_integral, julia_integral, true_val],
    AbsoluteError_C_Julia = [BigFloat(0.0), absolute_error_c_julia, BigFloat(0.0)],
    RelativeError_C_Julia = [df[df.Method .== "Trapezoidal", :RelativeError][1], relative_error_c_julia, BigFloat(0.0)],
    AbsoluteError_Julia_True = [BigFloat(0.0), absolute_error_julia_true, BigFloat(0.0)],
    RelativeError_Julia_True = [BigFloat(0.0), relative_error_julia_true, BigFloat(0.0)],
    Time = [df[df.Method .== "Trapezoidal", :Time][1], julia_time, 0.0]
)

# Print results with 20 significant digits
println("\nIntegration Results:")
println("----------------------------------------")
println("Parameters:")
println("N = $N")
println("Interval: [$x_inf, $x_sup]")
println("\nResults Comparison:")
for row in eachrow(final_df)
    println("$(row.Method):")
    println("  Result: $(@sprintf("%.20f", row.Result))")
    println("  Absolute Error (C-Julia): $(@sprintf("%.20f", row.AbsoluteError_C_Julia))")
    println("  Relative Error (C-Julia): $(@sprintf("%.20f", row.RelativeError_C_Julia))")
    println("  Absolute Error (Julia-True): $(@sprintf("%.20f", row.AbsoluteError_Julia_True))")
    println("  Relative Error (Julia-True): $(@sprintf("%.20f", row.RelativeError_Julia_True))")
    println("  Time: $(row.Time) s")
    println()
end

# Save results to file
open("julia_integration_results.dat", "w") do io
    println(io, "# Julia Integration Results")
    println(io, "# N = $N")
    println(io, "# Interval: [$x_inf, $x_sup]")
    println(io, "#")
    println(io, "# Method\tResult\t\tAbsError(C-J)\tRelError(C-J)\tAbsError(J-T)\tRelError(J-T)\tTime (s)")
    println(io, "#------------------------------------------------------------------------------------------------")
    for row in eachrow(final_df)
        println(io, "$(row.Method)\t$(@sprintf("%.20f", row.Result))\t$(@sprintf("%.20f", row.AbsoluteError_C_Julia))\t$(@sprintf("%.20f", row.RelativeError_C_Julia))\t$(@sprintf("%.20f", row.AbsoluteError_Julia_True))\t$(@sprintf("%.20f", row.RelativeError_Julia_True))\t$(row.Time)")
    end
end

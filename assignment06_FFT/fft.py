import numpy as np
import time

def print_on_terminal_and_file(message, file):
    print(message)
    file.write(message+"\n")

def generate_gaussian_matrix(N, mean=1.0, std=1.0):
    """Generate a NxN matrix with Gaussian random numbers"""
    return np.random.normal(mean, std, (N, N))

def calculate_errors(original, reconstructed):
    """Calculate mean and median absolute and relative errors"""
    # Calculate absolute errors
    abs_errors = np.abs(original - reconstructed)
    mean_abs_error = np.sqrt(np.mean(abs_errors**2))
    median_abs_error = np.sqrt(np.median(abs_errors**2))
    
    # Calculate relative errors (avoid division by zero)
    mask = np.abs(original) > 1e-10  # threshold for non-zero values
    rel_errors = np.zeros_like(original)
    rel_errors[mask] = abs_errors[mask] / np.abs(original[mask])
    mean_rel_error = np.sqrt(np.mean(rel_errors[mask]**2))
    median_rel_error = np.sqrt(np.median(rel_errors[mask]**2))
    
    return mean_abs_error, median_abs_error, mean_rel_error, median_rel_error, np.sum(mask)

def print_errors(mean_abs, median_abs, mean_rel, median_rel, count, file=None):
    """Print error statistics to file and console"""
    output = (f"Mean absolute error: {mean_abs:.6e}\n"
             f"Median absolute error: {median_abs:.6e}\n"
             f"Mean relative error: {mean_rel:.6e} (calculated over {count} non-zero values)\n"
             f"Median relative error: {median_rel:.6e}\n")
    
    print(output)
    if file:
        file.write(output)

def main():
    # Redirect output to both console and file
    with open('results_PYTHON.txt', 'w') as f:
        # Set random seed for reproducibility
        np.random.seed(42)
        
        # 1) Generate 1000x1000 Gaussian matrix
        print_on_terminal_and_file("\n1) Generating 1000x1000 Gaussian matrix...", f)
        start_time = time.time()
        A = generate_gaussian_matrix(1000)
        end_time = time.time()
        print_on_terminal_and_file(f"Matrix generation time: {end_time - start_time:.6f} seconds", f)
        print_on_terminal_and_file(f"Matrix A shape: {A.shape}", f)
        print_on_terminal_and_file(f"A[0,0]: {A[0,0]:.6e}", f)
        f.write(f"Matrix A shape: {A.shape}\n")
        f.write(f"A[0,0]: {A[0,0]:.6e}\n")
        
        # 2) Perform c2c FFT and reconstruct
        print_on_terminal_and_file("\n2) Performing complex-to-complex FFT...", f)
        start_time = time.time()
        C = np.fft.fft2(A)
        end_time = time.time()
        print_on_terminal_and_file(f"c2c FFT time: {end_time - start_time:.6f} seconds", f)
        
        start_time = time.time()
        A_reconstructed_c2c = np.fft.ifft2(C).real
        end_time = time.time()
        print_on_terminal_and_file(f"Inverse c2c FFT time: {end_time - start_time:.6f} seconds", f)
        
        print_on_terminal_and_file("Reconstruction completed", f)
        
        # Calculate errors for c2c
        print_on_terminal_and_file("\nErrors for complex-to-complex FFT:", f)
        mean_abs, median_abs, mean_rel, median_rel, count = calculate_errors(A, A_reconstructed_c2c)
        print_errors(mean_abs, median_abs, mean_rel, median_rel, count, f)
        
        # 3) Perform r2c FFT
        print_on_terminal_and_file("\n3) Performing real-to-complex FFT...", f)
        start_time = time.time()
        R = np.fft.rfft2(A)
        end_time = time.time()
        print_on_terminal_and_file(f"r2c FFT time: {end_time - start_time:.6f} seconds", f)
        
        print_on_terminal_and_file(f"Matrix R shape: {R.shape}", f)
        
        # 4) Reconstruct from r2c
        print_on_terminal_and_file("\n4) Reconstructing from real-to-complex FFT...", f)
        start_time = time.time()
        A_reconstructed_r2c = np.fft.irfft2(R)
        end_time = time.time()
        print_on_terminal_and_file(f"Inverse r2c FFT time: {end_time - start_time:.6f} seconds", f)
        
        print_on_terminal_and_file("Reconstruction completed", f)
        
        # Calculate errors for r2c
        print_on_terminal_and_file("\nErrors for real-to-complex FFT:", f)
        mean_abs, median_abs, mean_rel, median_rel, count = calculate_errors(A, A_reconstructed_r2c)
        print_errors(mean_abs, median_abs, mean_rel, median_rel, count, f)
        
        # 5) Machine precision analysis
        print_on_terminal_and_file("\n5) Machine precision analysis:", f)
        print_on_terminal_and_file(f"Machine epsilon for float64: {np.finfo(np.float64).eps:.6e}", f)
        print_on_terminal_and_file("For complex-to-complex FFT, we are close to machine precision", f)
        print_on_terminal_and_file("For real-to-complex FFT, we have higher errors due to the reconstruction process", f)
        
        # 6) DC component analysis
        print_on_terminal_and_file("\n6) DC component analysis:", f)
        print_on_terminal_and_file(f"C[0,0]: {C[0,0]:.6e}", f)
        print_on_terminal_and_file(f"R[0,0]: {R[0,0]:.6e}", f)
        print_on_terminal_and_file("These values represent the sum of all elements in the original matrix", f)
        print_on_terminal_and_file(f"Sum of A: {np.sum(A):.6e}", f)
        
        # 7) Bonus: 6x6 case
        print_on_terminal_and_file("\n7) Bonus: 6x6 case", f)
        start_time = time.time()
        A6 = generate_gaussian_matrix(6)
        end_time = time.time()
        print_on_terminal_and_file(f"6x6 matrix generation time: {end_time - start_time:.6f} seconds", f)
        print_on_terminal_and_file(f"Matrix A6 shape: {A6.shape}", f)
            
        # Perform r2c FFT
        start_time = time.time()
        R6 = np.fft.rfft2(A6)
        end_time = time.time()
        print_on_terminal_and_file(f"\n6x6 r2c FFT time: {end_time - start_time:.6f} seconds", f)
        
        print_on_terminal_and_file(f"Matrix R6 shape: {R6.shape}", f)
        
        # Reconstruct C from R
        start_time = time.time()
        C6_from_R = np.zeros((6, 6), dtype=complex)
        C6_from_R[:, :4] = R6  # Copy first half + 1 columns
        
        # Fill remaining columns using conjugate symmetry
        for i in range(6):
            for j in range(4, 6):
                C6_from_R[i, j] = np.conj(C6_from_R[i, 6-j])
        end_time = time.time()
        print_on_terminal_and_file(f"6x6 C reconstruction time: {end_time - start_time:.6f} seconds", f)
        
        # Get original C for comparison
        start_time = time.time()
        C6_original = np.fft.fft2(A6)
        end_time = time.time()
        print_on_terminal_and_file(f"6x6 c2c FFT time: {end_time - start_time:.6f} seconds", f)
        
        # Calculate errors
        print_on_terminal_and_file("\nErrors for C reconstruction from R (6x6):", f)
        mean_abs, median_abs, mean_rel, median_rel, count = calculate_errors(
            np.abs(C6_original), np.abs(C6_from_R))
        print_errors(mean_abs, median_abs, mean_rel, median_rel, count, f)
        
        # Print some values for comparison
        print_on_terminal_and_file("\nComparison of some values:", f)
        print_on_terminal_and_file(f"C6_original[0,0]: {C6_original[0,0]:.6e}", f)
        print_on_terminal_and_file(f"C6_from_R[0,0]: {C6_from_R[0,0]:.6e}", f)
        print_on_terminal_and_file(f"C6_original[1,1]: {C6_original[1,1]:.6e}", f)
        print_on_terminal_and_file(f"C6_from_R[1,1]: {C6_from_R[1,1]:.6e}", f)


if __name__ == "__main__":
    main()

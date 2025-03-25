# Vector Processing Program

This program consists of two parts:
1. A program to generate input vectors
2. A program to process these vectors using configuration files

## Prerequisites
- GCC compiler
- Make

## Compilation
```bash
make clean
make
```

## Usage

### 1. Generate Input Vectors
```bash
./input <N> <filename_prefix>
```
This will create two files:
- `{filename_prefix}N{N}_x.dat`: Contains vector x with all elements = 0.1
- `{filename_prefix}N{N}_y.dat`: Contains vector y with all elements = 7.1

Example:
```bash
./input 1000 vectors
```
This creates:
- `vectorsN1000_x.dat`
- `vectorsN1000_y.dat`

### 2. Process Vectors
```bash
./process <config_file>
```

The configuration file should have the following format:
```
N=1000
x_filename=vectorsN1000_x.dat
y_filename=vectorsN1000_y.dat
a=3.0
output_prefix=result_
```

This will:
1. Read vectors x and y from the specified files
2. Calculate d = ax + y
3. Save the result in `{output_prefix}N{N}_d.dat`

Example:
```bash
./process config.txt
```
This creates:
- `result_N1000_d.dat`

## File Format
- Input files: One number per line
- Output file: One number per line
- Configuration file: Key=value pairs, one per line 
# Vector Processing Program

This program has two versions:
1. Text file version (in `1_txt_IO/`)
2. HDF5 version (in `2_HDF5_IO/`)

## Prerequisites
- GCC compiler
- Make
- HDF5 library (for the HDF5 version)

### Installing HDF5
On Ubuntu/Debian:
```bash
sudo apt-get install libhdf5-dev
```

On CentOS/RHEL:
```bash
sudo yum install epel-release
sudo yum install hdf5-devel
```

## Compilation

### Text File Version
```bash
cd 1_txt_IO
make clean
make
```

### HDF5 Version
```bash
cd 2_HDF5_IO
make clean
make
```

## Usage

### Text File Version

#### 1. Generate Input Vectors and Configuration
```bash
./input <N> <filename_prefix>
```
This will create:
- `{filename_prefix}N{N}_x.dat`: Contains vector x with all elements = 0.1
- `{filename_prefix}N{N}_y.dat`: Contains vector y with all elements = 7.1
- `config.conf`: Configuration file for the process program

Example:
```bash
./input 1000 vectors
```
This creates:
- `vectorsN1000_x.dat`
- `vectorsN1000_y.dat`
- `config.conf` with the following content:
  ```
  N=1000
  x_filename=vectorsN1000_x.dat
  y_filename=vectorsN1000_y.dat
  a=3.0
  output_prefix=result_
  ```

#### 2. Process Vectors
```bash
./process config.conf
```

This will:
1. Read vectors x and y from the files specified in config.conf
2. Calculate d = ax + y
3. Save the result in `{output_prefix}N{N}_d.dat`

### HDF5 Version

#### 1. Generate Input Vectors and Configuration
```bash
./input <N> <filename_prefix>
```
This will create:
- `{filename_prefix}N{N}.h5`: Contains vectors x and y in HDF5 format
- `config.conf`: Configuration file for the process program

Example:
```bash
./input 1000 vectors
```
This creates:
- `vectorsN1000.h5` with datasets "vector_x" and "vector_y"
- `config.conf` with the following content:
  ```
  N=1000
  x_filename=vectorsN1000.h5
  y_filename=vectorsN1000.h5
  a=3.0
  output_prefix=result_
  ```

#### 2. Process Vectors
```bash
./process config.conf
```

This will:
1. Read vectors x and y from the HDF5 files specified in config.conf
2. Calculate d = ax + y
3. Save the result in `{output_prefix}N{N}.h5` with dataset "vector_d"

## File Format
- Text version:
  - Input files: One number per line
  - Output file: One number per line
  - Configuration file: Key=value pairs, one per line
- HDF5 version:
  - Input/Output files: HDF5 format with datasets
  - Configuration file: Key=value pairs, one per line 
# Vector Processing Program

## Table of Contents
1. [Prerequisites](#1-prerequisites)
   - [1.1 GCC Compiler](#11-gcc-compiler)
   - [1.2 HDF5 Libraries](#12-hdf5-libraries)
   - [1.3 GSL Libraries](#13-gsl-libraries)
2. [Compilation](#2-compilation)
   - [2.1 Text File Version](#21-text-file-version)
   - [2.2 HDF5 Version](#22-hdf5-version)
   - [2.3 GSL and HDF5 Version](#23-gsl-and-hdf5-version)
3. [Usage](#3-usage)
   - [3.1 Text File Version](#31-text-file-version)
   - [3.2 HDF5 Version](#32-hdf5-version)
   - [3.3 GSL and HDF5 Version](#33-gsl-and-hdf5-version)
4. [File Format](#4-file-format)
   - [4.1 Text File Version](#41-text-file-version)
   - [4.2 HDF5 Version](#42-hdf5-version)
   - [4.3 GSL and HDF5 Version](#43-gsl-and-hdf5-version)
5. [Notes](#5-notes)

This program is available in three versions:
1. Text file version (in `1_txt_IO/`)
2. HDF5 version (in `2_HDF5_IO/`)
3. GSL and HDF5 version (in `3_GSL_IO/`)

## 1. Prerequisites

### 1.1 GCC Compiler
On AlmaLinux:
```bash
sudo dnf groupinstall "Development Tools"
```

### 1.2 HDF5 Libraries
On AlmaLinux:
```bash
sudo dnf install epel-release
sudo dnf install hdf5-devel
```

### 1.3 GSL Libraries (GNU Scientific Library)
On AlmaLinux:
```bash
sudo dnf install gsl-devel
```

## 2. Compilation

### 2.1 Text File Version
```bash
cd 1_txt_IO
make clean
make
```

### 2.2 HDF5 Version
```bash
cd 2_HDF5_IO
make clean
make
```

### 2.3 GSL and HDF5 Version
```bash
cd 3_GSL_IO
make clean
make
```

## 3. Usage

### 3.1 Text File Version

#### 3.1.1 Generate Input Vectors and Configuration File
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

#### 3.1.2 Process Vectors
```bash
./process config.conf
```

This will:
1. Read vectors x and y from the files specified in config.conf
2. Calculate d = ax + y
3. Save the result in `{output_prefix}N{N}_d.dat`

### 3.2 HDF5 Version

#### 3.2.1 Generate Input Vectors and Configuration File
```bash
./input <N> <filename_prefix>
```
This will create:
- `{filename_prefix}N{N}_x.h5`: Contains vector x in HDF5 format
- `{filename_prefix}N{N}_y.h5`: Contains vector y in HDF5 format
- `config.conf`: Configuration file for the process program

Example:
```bash
./input 1000 vectors
```
This creates:
- `vectorsN1000_x.h5` with dataset "vector_x"
- `vectorsN1000_y.h5` with dataset "vector_y"
- `config.conf` with the following content:
  ```
  N=1000
  x_filename=vectorsN1000_x.h5
  y_filename=vectorsN1000_y.h5
  a=3.0
  output_prefix=result_
  ```

#### 3.2.2 Process Vectors
```bash
./process config.conf
```

This will:
1. Read vectors x and y from the HDF5 files specified in config.conf
2. Calculate d = ax + y
3. Save the result in `{output_prefix}N{N}_d.h5` with dataset "vector_d"

### 3.3 GSL and HDF5 Version

#### 3.3.1 Generate Input Vectors and Configuration File
```bash
./input <N> <filename_prefix>
```
This will create:
- `{filename_prefix}N{N}_x.h5`: Contains vector x in HDF5 format
- `{filename_prefix}N{N}_y.h5`: Contains vector y in HDF5 format
- `config.conf`: Configuration file for the process program

Example:
```bash
./input 1000 vectors
```
This creates:
- `vectorsN1000_x.h5` with dataset "vector_x"
- `vectorsN1000_y.h5` with dataset "vector_y"
- `config.conf` with the following content:
  ```
  N=1000
  x_filename=vectorsN1000_x.h5
  y_filename=vectorsN1000_y.h5
  a=3.0
  output_prefix=result_
  ```

#### 3.3.2 Process Vectors
```bash
./process config.conf
```

This will:
1. Read vectors x and y from the HDF5 files specified in config.conf
2. Calculate d = ax + y using the optimized `gsl_vector_axpby` function
3. Save the result in `{output_prefix}N{N}_d.h5` with dataset "vector_d"

## 4. File Format

### 4.1 Text File Version
- Input files: One number per line
- Output file: One number per line
- Configuration file: Key=value pairs, one per line

### 4.2 HDF5 Version
- Input/Output files: HDF5 format with datasets
- Configuration file: Key=value pairs, one per line

### 4.3 GSL and HDF5 Version
- Input/Output files: HDF5 format with datasets
- Configuration file: Key=value pairs, one per line
- Uses GSL's optimized data structures and functions for vector operations

## 5. Notes
- The GSL version is optimized for vector operations and uses highly performant functions
- The HDF5 version is more efficient for handling large amounts of data
- The text file version is the simplest and most portable 
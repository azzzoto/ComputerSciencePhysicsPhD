# Some raccomandations

On AlmaLinux, in order to write and compile a C program, you need to install the GCC compiler:

### **1. Install GCC (GNU Compiler Collection)**
Write in the terminal
```bash
sudo dnf install gcc
```
and verify the correct installation via
```bash
gcc --version
```

### **2. Write a program in C**

```c
#include <stdio.h>

int main() {
    printf("Hi, AlmaLinux!\n");
    return 0;
}
```

### **3. Compile the program**
```bash
gcc hello.c -o hello
```
This will generate an executable file named `hello`.

### **4. Run the program**
Run the compiled c program with:
```bash
./hello
```
And you should be able to see:  
```
Hi, AlmaLinux!
```

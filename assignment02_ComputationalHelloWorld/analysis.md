# Analisi dei Problemi di Esecuzione

## Codici Sorgente Analizzati

### 1. Moltiplicazione Vettoriale (Julia)
```julia
# 1_vector_multiplication.jl
function compute_product(a, x, y)
    return a .* x .+ y
end

N = [10, 1e6, 1e8]
a = 3.0
x = 0.1
y = 7.1
```

### 2. Moltiplicazione Vettoriale (C)
```c
// 2_vector_multiplication.c
void compute_product(double a, double *x, double *y, double *d, int N) {
    for (int i = 0; i < N; i++) {
        d[i] = a * x[i] + y[i];
    }
}
```

### 3. Moltiplicazione Matriciale (Julia)
```julia
# 3a_matmul.jl
A = 3 .* ones(n, n)
B = 7.1 .* ones(n, n)
C = A .* B  # Errore: moltiplicazione elemento per elemento invece di matriciale
```

### 3. Moltiplicazione Matriciale (C)
```c
// 3b_matmul.c
void compute_product(double **A, double **B, double **C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = A[i][j] * B[i][j];  // Errore: moltiplicazione elemento per elemento
        }
    }
}
```

## Problemi Riscontrati nell'Esecuzione

### i. Problemi con Diversi Valori di N

1. **Moltiplicazione Vettoriale (1 e 2)**:
   - Per N = 10 e N = 10^6: nessun problema significativo
   - Per N = 10^8:
     - In C: possibili problemi di allocazione memoria su sistemi con RAM limitata
     - In Julia: gestione memoria più efficiente grazie al garbage collector

2. **Moltiplicazione Matriciale (3a e 3b)**:
   - Per N = 10 e N = 100: esecuzione corretta ma risultati errati
   - Per N = 10^4:
     - Problemi di memoria: una matrice 10000x10000 richiede circa 800MB di memoria (10000 * 10000 * 8 bytes)
     - Possibili crash per sistemi con memoria limitata
     - Tempi di esecuzione molto lunghi dovuti all'implementazione inefficiente

### ii. Correttezza dei Test

1. **Moltiplicazione Vettoriale**:
   - Test implementati correttamente in entrambi i linguaggi
   - Valore atteso corretto: `a * x + y = 3.0 * 0.1 + 7.1 = 7.4`
   - Tolleranza epsilon implementata per gestire errori di arrotondamento

2. **Moltiplicazione Matriciale**:
   - Test implementati **incorrettamente** in entrambi i linguaggi:
     - Valore atteso errato: usa `3 * 7.1` invece di `3 * 7.1 * N`
     - Implementazione errata: usa moltiplicazione elemento per elemento invece di moltiplicazione matriciale
   - Correzione necessaria:
     ```c
     // Implementazione corretta in C
     for (int i = 0; i < N; i++) {
         for (int j = 0; j < N; j++) {
             C[i][j] = 0.0;
             for (int k = 0; k < N; k++) {
                 C[i][j] += A[i][k] * B[k][j];
             }
         }
     }
     ```
     ```julia
     # Implementazione corretta in Julia
     C = A * B  # usa l'operatore * invece di .*
     ```

## Conclusioni

1. **Gestione della Memoria**:
   - Julia gestisce meglio la memoria per grandi array grazie al garbage collector
   - In C è necessaria una gestione manuale attenta della memoria

2. **Correttezza Algoritmica**:
   - La moltiplicazione vettoriale è implementata correttamente
   - La moltiplicazione matriciale contiene errori sia concettuali che implementativi

3. **Performance**:
   - Per N grandi, l'implementazione in C della moltiplicazione matriciale è inefficiente
   - Julia offre ottimizzazioni automatiche per operazioni matriciali 
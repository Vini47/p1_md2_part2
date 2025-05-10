#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Função gcd: calcula o máximo divisor comum de a e b (algoritmo de Euclides)
// -----------------------------------------------------------------------------
int gcd(int a, int b) {
    if (b == 0) return abs(a);
    return gcd(b, a % b);
}

// -----------------------------------------------------------------------------
// Função is_prime: teste de primalidade por tentativa de divisão até sqrt(n)
// -----------------------------------------------------------------------------
bool is_prime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Função extended_gcd: estende Euclides para encontrar x, y tais que a·x + b·y = d
// Retorna d = gcd(a,b) e preenche x, y
// -----------------------------------------------------------------------------
int extended_gcd(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = (a >= 0) ? 1 : -1;
        *y = 0;
        return abs(a);
    }
    int x1, y1;
    int d = extended_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return d;
}

// -----------------------------------------------------------------------------
// Função modinv: calcula inverso modular de g em Z_n, se existir
// -----------------------------------------------------------------------------
bool modinv(int g, int n, int *inv) {
    int x, y;
    int d = extended_gcd(g, n, &x, &y);
    if (d != 1) return false;
    *inv = (x % n + n) % n;
    return true;
}

// -----------------------------------------------------------------------------
// Função modexp: exponenciação modular rápida (log exp)
// -----------------------------------------------------------------------------
long long modexp(long long base, long long exp, long long m) {
    long long res = 1;
    base %= m;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % m;
        base = (base * base) % m;
        exp >>= 1;
    }
    return res;
}

// -----------------------------------------------------------------------------
// Função euler_phi: calcula φ(n) fazendo fatoração simples
// -----------------------------------------------------------------------------
int euler_phi(int n) {
    int result = n;
    int temp = n;
    for (int p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1) {
        result -= result / temp;
    }
    return result;
}

int main() {
    int H, G, n, x, n1;

    printf("=== Validação de acesso com verificação modular ===\n");
    // Inputs mais claros e intuitivos
    printf("Informe o valor de H (numerador): ");
    if (scanf("%d", &H) != 1) { printf("Entrada inválida para H.\n"); return 1; }

    printf("Informe o valor de G (denominador): ");
    if (scanf("%d", &G) != 1) { printf("Entrada inválida para G.\n"); return 1; }

    printf("Informe o valor de n (módulo para divisão H⊘G): ");
    if (scanf("%d", &n) != 1) { printf("Entrada inválida para n.\n"); return 1; }

    printf("Informe o expoente x: ");
    if (scanf("%d", &x) != 1) { printf("Entrada inválida para x.\n"); return 1; }

    printf("Informe o valor de n1 (módulo final para a^x): ");
    if (scanf("%d", &n1) != 1) { printf("Entrada inválida para n1.\n"); return 1; }

    // 1) Verificar primalidade de G e n
    printf("\n1) Verificando primalidade de G e n...\n");
    bool Gprimo = is_prime(G);
    bool nprimo = is_prime(n);
    printf("   - G = %d %s primo\n", G, Gprimo ? "é" : "não é");
    printf("   - n = %d %s primo\n", n, nprimo ? "é" : "não é");
    if (!Gprimo || !nprimo) {
        printf("   * Falha: G ou n não são primos. Encerrando.\n");
        return 0;
    }

    // 2) Calcular o inverso de G em Z_n
    printf("\n2) Calculando inverso de G em Z_%d...\n", n);
    int invG;
    if (!modinv(G, n, &invG)) {
        printf("   * Falha: não existe inverso de %d em Z_%d.\n", G, n);
        return 0;
    }
    printf("   - G⁻¹ ≡ %d mod %d\n", invG, n);

    // 3) Divisão modular
    int a = (int)((1LL * H * invG) % n);
    if (a < 0) a += n;
    printf("\n3) Calculando base modular: a = H ⊘ G ≡ %d * %d mod %d = %d\n",
           H, invG, n, a);

    // 4) Verificar coprimalidade de a e n1
    printf("\n4) Verificando coprimalidade de a e n1...\n");
    int g_an1 = gcd(a, n1);
    printf("   - gcd(%d, %d) = %d\n", a, n1, g_an1);
    if (g_an1 != 1) {
        printf("   * Falha: a e n1 não são coprimos.\n");
        return 0;
    }
    printf("   - a e n1 são coprimos.\n");

    // 5) Teste de primalidade de n1
    printf("\n5) Verificando primalidade de n1 = %d...\n", n1);
    bool n1primo = is_prime(n1);
    printf("   - n1 %s primo\n", n1primo ? "é" : "não é");

    // 6–7) Definir x1 via Fermat ou Euler
    int x1;
    if (n1primo) {
        x1 = n1 - 1;
        printf("\n6) n1 é primo: usando Fermat -> x1 = n1 - 1 = %d\n", x1);
    } else {
        x1 = euler_phi(n1);
        printf("\n7) n1 não é primo: usando Euler -> φ(%d) = %d -> x1 = %d\n", n1, x1, x1);
    }

    // 8) Decompor x = x1 * q + r
    int q = x / x1;
    int r = x % x1;
    printf("\n8) Decompondo x = %d em x1 * q + r:\n", x);
    printf("   - %d = %d * %d + %d (q = %d, r = %d)\n", x, x1, q, r, q, r);

    // 9) Mostrar reescrita
    printf("\n9) Reescrevendo: a^x ≡ ((a^%d)^%d mod %d) * (a^%d mod %d) mod %d\n",
           x1, q, n1, r, n1, n1);

    // 10) Cálculo intermediários
    printf("\n10) Calculando valores intermediários...\n");
    long long x2 = modexp(a, x1, n1);
    printf("    - a^x1 mod n1 = %d^%d mod %d = %lld\n", a, x1, n1, x2);
    long long x2q = modexp(x2, q, n1);
    printf("    - (a^x1)^q mod %d = %lld^%d mod %d = %lld\n", n1, x2, q, n1, x2q);
    long long ar = modexp(a, r, n1);
    printf("    - a^r mod n1 = %d^%d mod %d = %lld\n", a, r, n1, ar);

    // 11) Resultado final
    long long resultado = (x2q * ar) % n1;
    printf("\n11) Resultado final: ( (a^x1)^q * a^r ) mod %d = %lld\n", n1, resultado);

    printf("\n=== Fim do cálculo ===\n");
    return 0;
}

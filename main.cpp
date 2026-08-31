#include <iostream>
using namespace std;

const int MAX_N = 1000;  // adjust if you need larger numbers

// Sieve of Eratosthenes using a plain bool array
void sieveOfEratosthenes(bool isComposite[], int n)
{
    for (int i = 0; i <= n; i++)
    {
        isComposite[i] = false;
    }
    for (int i = 2; i <= n; i++)
    {
        if (!isComposite[i])
        {
            for (int j = i * i; j <= n; j += i)
            {
                isComposite[j] = true;
            }
        }
    }
}

int middleSchoolGCD(int m, int n)
{
    int limit = max(m, n);
    bool isComposite[MAX_N + 1];
    sieveOfEratosthenes(isComposite, limit);

    // Arrays to hold exponents of each prime (index = prime value) for m and n
    int expM[MAX_N + 1] = {0};
    int expN[MAX_N + 1] = {0};

    // Factorize m
    int num = m;
    for (int p = 2; p <= limit; p++) {
        if (!isComposite[p]) {
            while (num % p == 0) {
                expM[p]++;
                num /= p;
            }
        }
    }
    if (num > 1) expM[num]++;   // leftover prime factor larger than sieve range

    // Factorize n
    num = n;
    for (int p = 2; p <= limit; p++)
    {
        if (!isComposite[p])
        {
            while (num % p == 0)
            {
                expN[p]++;
                num /= p;
            }
        }
    }
    if (num > 1) expN[num]++;

    // Multiply common prime factors using the minimum exponent
    int gcd = 1;
    for (int p = 2; p <= limit; p++)
    {
        int minExp = min(expM[p], expN[p]);
        for (int i = 0; i < minExp; i++) gcd *= p;
    }
    return gcd;
}

int main() {
    int m, n;
    cout << "Enter two integers: ";
    cin >> m >> n;
    cout << "GCD of " << m << " and " << n << " is " << middleSchoolGCD(m, n) << endl;
    return 0;
}

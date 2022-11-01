#include "mappingmath.hpp"

double MappingMath::polyBernstein(double u, int m, int i)
{
    return (coefBin(m, i) * puissance(u, i) * puissance(1.0 - u, m - i));
}

double MappingMath::coefBin(int n, int k)
{
    int factN = factorielle(n);
    int factK = factorielle(k);
    int factNK = factorielle(n - k);

    return static_cast<double>(factN) / static_cast<double>(factK * factNK);
}

double MappingMath::puissance(double nb, int p)
{
    if(p == 0)
        return 1.0;
    else
        return nb * puissance(nb, p - 1);
}

int MappingMath::factorielle(int n)
{
    if(n == 0)
        return 1;
    else
        return n * factorielle(n - 1);
}

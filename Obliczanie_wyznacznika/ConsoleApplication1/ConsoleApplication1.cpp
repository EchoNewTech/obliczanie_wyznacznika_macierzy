#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <string>

#define MAX_STOPIEN 100    /* Maksymalny stopień macierzy */

std::vector<std::vector<long double>> nowaMacierz(int stopien);
void usunMacierz(std::vector<std::vector<long double>>& macierz);
long double metodaLaplace(const std::vector<std::vector<long double>>& macierz, int stopien);
void anuluj(int nr_bledu);
void wyswietlMacierz(const std::vector<std::vector<long double>>& macierz);

int main()
{
    std::vector<std::vector<long double>> macierz;
    int stopien;
    int nr_wiersza, nr_kolumny;

    std::cout << "Proszę podać stopień macierzy n=";
    if (!(std::cin >> stopien)) anuluj(EINVAL);
    std::cin.ignore();  // Ignorujemy znak nowej linii pozostawiony przez std::cin
    macierz = nowaMacierz(stopien);

    for (nr_wiersza = 1; nr_wiersza <= stopien; nr_wiersza++)
    {
        for (nr_kolumny = 1; nr_kolumny <= stopien; nr_kolumny++)
        {
            std::cout << "A[" << nr_wiersza << "," << nr_kolumny << "]=";
            std::string input;
            std::getline(std::cin, input);

            if (input.empty())
            {
                macierz[nr_wiersza - 1][nr_kolumny - 1] = 0;
            }
            else
            {
                try
                {
                    macierz[nr_wiersza - 1][nr_kolumny - 1] = std::stold(input);
                }
                catch (const std::invalid_argument&)
                {
                    anuluj(EINVAL);
                }
            }
        }
    }

    std::cout << "\nPodana macierz:\n";
    wyswietlMacierz(macierz);

    std::cout << "\ndet(A) = " << metodaLaplace(macierz, stopien) << std::endl;
    usunMacierz(macierz);
    return 0;
}

// Tworzy nową macierz kwadratową danego stopnia
std::vector<std::vector<long double>> nowaMacierz(int stopien)
{
    if (stopien < 1 || stopien > MAX_STOPIEN) anuluj(EINVAL);
    std::vector<std::vector<long double>> macierz(stopien, std::vector<long double>(stopien));
    return macierz;
}

// Zwalnia pamięć zajętą przez macierz
void usunMacierz(std::vector<std::vector<long double>>& macierz)
{
    macierz.clear();
}

// Oblicza wartość wyznacznika metodą rozwinięcia Laplace'a
long double metodaLaplace(const std::vector<std::vector<long double>>& macierz, int stopien)
{
    if (stopien == 1)
        return macierz[0][0];

    if (stopien == 2)
        return macierz[0][0] * macierz[1][1] - macierz[0][1] * macierz[1][0];

    long double det = 0.0;
    std::vector<std::vector<long double>> dopelnienie(stopien - 1, std::vector<long double>(stopien - 1));

    for (int nr_kolumny = 0; nr_kolumny < stopien; nr_kolumny++)
    {
        for (int nr_wiersza = 1; nr_wiersza < stopien; nr_wiersza++)
        {
            int k = 0;
            for (int j = 0; j < stopien; j++)
            {
                if (j == nr_kolumny)
                    continue;
                dopelnienie[nr_wiersza - 1][k] = macierz[nr_wiersza][j];
                k++;
            }
        }
        det += macierz[0][nr_kolumny] * pow(-1, nr_kolumny) * metodaLaplace(dopelnienie, stopien - 1);
    }
    return det;
}

// Wyświetla treść błędu i wychodzi z aplikacji 
void anuluj(const int nr_bledu)
{
    char buffer[256];
    strerror_s(buffer, sizeof(buffer), nr_bledu);
    std::cerr << "Nie udało się obliczyć wyznacznika: " << buffer << std::endl;
    exit(nr_bledu);
}

// Wyświetla macierz
void wyswietlMacierz(const std::vector<std::vector<long double>>& macierz)
{
    int stopien = macierz.size();
    for (int nr_wiersza = 0; nr_wiersza < stopien; nr_wiersza++)
    {
        for (int nr_kolumny = 0; nr_kolumny < stopien; nr_kolumny++)
        {
            std::cout << macierz[nr_wiersza][nr_kolumny] << " ";
        }
        std::cout << std::endl;
    }
}



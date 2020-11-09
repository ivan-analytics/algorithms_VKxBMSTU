#include <iostream>

// TODO: implement 256 alphabet
int get_digit(std::string& str, int digit) {
    return (int) str[digit];
}


int* counting_sort(std::string* a, int n, int k, int digit) {
    int* c = new int[k];
    for( int i = 0; i < k; ++i )
        c[i] = 0;
    for( int i = 0; i < n; ++i )
        ++c[get_digit(a[i], digit)];
    int sum = 0;
    for( int i = 0; i < k; ++i ) {
        int tmp = c[i];
        c[i] = sum; // Начала групп
        sum += tmp;
    }
    auto* b = new std::string[n];
    for( int i = 0; i < n; ++i ) {
        b[c[get_digit(a[i], digit)]++] = a[i];
    }
    for (int i = 0; i < n; i++)
        a[i] = b[i];
    delete[] b;

    return c; // массив с расположением концов групп
}

void get_lines() {

}

void msd_sort(std::string* a, int n, int k, int digit) {
    // TODO: сделать внешнее выделение памяти для с
    int* c = counting_sort(a, n, k, digit);
    if (c[0] > 1) {
        if ((int) a[c[0] - 1][digit] != 0) {
            msd_sort(a + 0, c[0], k, digit + 1);
        }
    }
    for (int i = 1; i < k; i++) {
        int group_len = c[i] - c[i-1];
        if (group_len > 1) {
            if ((int) a[c[i] - 1][digit] != 0) {
                msd_sort(a + (c[i] - group_len), group_len, k, digit + 1);
            }
        }
    }
}

#define ALPHABET_POWER 128

int main() {
    size_t num_of_str  = 5;
    auto* str = new std::string[num_of_str];
    for (int i = 0; i < num_of_str; i++)
        std::getline(std::cin, str[i]);

    msd_sort(str, num_of_str, ALPHABET_POWER, 0);

    std::cout << std::endl;
    for (int i = 0; i < num_of_str; i++)
        std::cout << str[i] << std::endl;
    return 0;
}


/*
Hello from here
meeek mike
ddd
sldjfksdf
nkedmkek
 */
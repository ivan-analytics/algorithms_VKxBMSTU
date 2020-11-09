#include <iostream>

// TODO: implement 256 alphabet
int get_digit(std::string& str, int digit) {
    return (int) str[digit];
}

void counting_sort(std::string* a, int n, int k, int digit) {
    int* c = new int[k];
    for( int i = 0; i < k; ++i )
        c[i] = 0;
    for( int i = 0; i < n; ++i )
        ++c[get_digit(a[i], digit)];
    int sum = 0;
    for( int i = 0; i < k; ++i ) {
        int tmp = c[i];
        c[i] = sum; // Начала групп.
        sum += tmp;
    }
    auto* b = new std::string[n];
    for( int i = 0; i < n; ++i ) {
        b[c[get_digit(a[i], digit)]++] = a[i];
    }
    delete[] c;
    for (int i = 0; i < n; i++)
        a[i] = b[i];
    delete[] b;
}

void get_lines() {

}

int main() {
    size_t num_of_str  = 5;
    auto* str = new std::string[num_of_str];
    for (int i = 0; i < num_of_str; i++)
        std::getline(std::cin, str[i]);

    counting_sort(str, num_of_str, 128, 0);

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
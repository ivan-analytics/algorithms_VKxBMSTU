#include <iostream>

/*
7_1. MSD для строк.
Дан массив строк. Количество строк не больше 105. Отсортировать массив методом поразрядной сортировки MSD по символам. 
Размер алфавита - 256 символов. Последний символ строки = ‘\0’.
*/

int get_digit(std::string& str, int digit) {
    return (int) str[digit];
}

void counting_sort(std::string* a, int n, int k, int digit, int* c) {
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
}

void msd_sort(std::string* a, int n, int k, int digit = 0) {
    int* c = new int[k];
    counting_sort(a, n, k, digit, c); // c - массив с расположением концов групп
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
    delete[] c;
}

#define ALPHABET_POWER 256

int main() {
    // input
    std::string val;
    int num_of_str = 0, capacity = 2;
    auto* str = new std::string[capacity];
    while(std::getline(std::cin, val)) {
        if (num_of_str >= capacity ) {
            auto* new_string = new std::string [capacity * 2];
            for( int i = 0; i < capacity; ++i )
                new_string[i] = str[i];

            capacity *= 2;
            delete [] str;
            str = new_string;
        }

        str[num_of_str] = val;
        num_of_str++;
    }

    // logic
    msd_sort(str, num_of_str, ALPHABET_POWER);

    // output
    for (int i = 0; i < num_of_str; i++)
        std::cout << str[i] << std::endl;

    delete[] str;
    return 0;
}
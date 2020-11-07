#include <iostream>

// TODO: implement 256 alphabet
void counting_sort(int* a, int n, int k) {
    int* c = new int[k];
    for( int i = 0; i < k; ++i )
        c[i] = 0;
    for( int i = 0; i < n; ++i )
        ++c[a[i]];
    int sum = 0;
    for( int i = 0; i < k; ++i ) {
        int tmp = c[i];
        c[i] = sum; // Начала групп.
        sum += tmp;
    }
    int* b = new int[n];
    for( int i = 0; i < n; ++i ) {
        b[c[a[i]]++] = a[i];
    }
    delete[] c;
    memcpy( a, b, n * sizeof( int ) );
    delete[] b;
}

void get_lines(std::string* lines, int& capacity) {
    int num_of_lines = 0;
    lines = new std::string[capacity];

    while (getline(std::cin, lines[num_of_lines])) {
        if (lines[num_of_lines].empty()) {
            break;
        }
        num_of_lines++;
        if (num_of_lines == capacity) {
            capacity *= 2;
            auto* temp = lines;
            lines = new std::string[capacity];
            for (int i = 0; i < num_of_lines; i++)
                lines[i] = temp[i];
            delete[] temp;
        }
    }
    for (int i = 0; i < num_of_lines; i++)
        std::cout << lines[i] << std::endl;
}

int main() {
    /*
    int n;
    std::cin >> n;
    int* a = new int[n];
    for (int i = 0; i < n; i++) std::cin >> a[i];

    counting_sort(a, n, 10);

    for (int i = 0; i < n; i++) std::cout << a[i] << " ";
     */

    int capacity = 3;
    auto* lines = new std::string[capacity];
    get_lines(lines, capacity);
    delete[] lines;
    std::cout << capacity;


    return 0;
}

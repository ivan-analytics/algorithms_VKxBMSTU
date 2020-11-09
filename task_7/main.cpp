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

void get_lines() {

}

int main() {
    int n;
    std::cin >> n;
    int* a = new int[n];
    for (int i = 0; i < n; i++) std::cin >> a[i];

    counting_sort(a, n, 10);

    for (int i = 0; i < n; i++) std::cout << a[i] << " ";
    return 0;
}

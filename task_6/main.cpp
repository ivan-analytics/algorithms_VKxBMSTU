#include <iostream>

int& three_median(int& a, int& b, int& c) {
    if(b < c && b < a) return a < c ? a : c;
    else return a > c ? a : c;
}

int partition(int* a, int n) {
    int& res = three_median(*a, *(a + n - 1), *(a + n / 2));
    // опорный элемент меняем с последним
    int temp = a[n - 1];
    a[n - 1] = res;
    res = temp;
    int pivot = a[n - 1];

    int i = 0;
    int j = 0;

    while (j < n - 1) {
        if (a[j] > pivot)
            j++;
        else {
            std::swap(a[i], a[j]);
            i++;
            j++;
        }
    }

    std::swap(a[i], a[n - 1]);
    return i; // возвращаем индекс последнего элемента левой части
}


int main() {
    int n, k;
    std::cin >> n >> k;
    int b[n];
    for (int i = 0; i < n; i++)
        std::cin >> b[i];


    partition(b, n);


    for (int i = 0; i < n; i++)
        std::cout << b[i] << " ";
    return 0;
}

/*
10 4
2 9 3 6 9 4 5 8 3 3
 */
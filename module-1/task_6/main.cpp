#include <iostream>

/*
Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n. Требуется найти k-ю порядковую статистику.
т.е. напечатать число, которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.

6_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition реализуйте методом прохода двумя
итераторами от конца массива к началу.
 */

template<class T>
class CmpDefault {
public:
    bool operator()( const T& l, const T& r ) { return l < r; }
};

template<class T, class Compare = CmpDefault<T>>
T& three_median(T& a, T& b, T& c, Compare cmp = CmpDefault<T>()) {
    if(cmp(b, c) && cmp(b, a)) return cmp(a, c) ? a : c;
    else return cmp(c, a) ? a : c;
}

template<class T, class Compare = CmpDefault<T>>
int partition(T* a, int n, Compare cmp = CmpDefault<T>()) {
    T& res = three_median(*a, *(a + n - 1), *(a + n / 2));
    // опорный элемент меняем с последним
    T temp = a[n - 1];
    a[n - 1] = res;
    res = temp;
    T pivot = a[n - 1];

    int i = n - 2;
    int j = n - 2;

    while (j >= 0) {
        if (cmp(pivot, a[j])) {
            std::swap(a[i], a[j]);
            i--;
            j--;
        }
        else {
            j--;
        }
    }

    std::swap(a[i + 1], a[n - 1]);
    return i + 1; // возвращаем индекс последнего элемента левой части
}

template<class T>
int k_stat(T* a, int n, int k) {
    int start = 0;
    int end = n;

    int p = partition(a + start, end);
    while(p != k) {
        if (p > k)
            end = p;
        else {
            start = start + (p + 1);
            end = end - (p + 1);
            k = k - (p + 1);
        }
        p  = partition(a + start, end);
    }

    return a[start + p];
}


int main() {
    int n, k;
    std::cin >> n >> k;
    int b[n];
    for (int i = 0; i < n; i++)
        std::cin >> b[i];

    std::cout << k_stat(b, n, k) << std::endl;
    return 0;
}
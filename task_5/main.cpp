#include <iostream>

/*
 *
Слияние двух отсортированных массивов:
 - Выберем массив, крайний элемент которого меньше,
 - Извлечем этот элемент в массив-результат,
 - Продолжим, пока один из массивов не опустеет,
 - Копируем остаток второго массива в конец массива-результата
 */

void merge(const int* arr_first, int len_first, const int* arr_second, int len_second, int* c) {
    int ind_first = 0;
    int ind_second = 0;
    int ind_c = 0;
    while (true) {
        if (ind_first >= len_first) {
            // копируем остаток
            while (ind_second < len_second) {
                c[ind_c] = arr_second[ind_second];
                ind_second++;
                ind_c++;
            }
            break;
        }
        if (ind_second >= len_second) {
            // копируем остаток
            while (ind_first < len_first) {
                c[ind_c] = arr_first[ind_first];
                ind_first++;
                ind_c++;
            }
            break;
        }

        // выберем массив, крайний элемент которого меньше
        // извлечем этот элемент в массив-результат
        if (arr_first[ind_first] <= arr_second[ind_second]) {
            c[ind_c] = arr_first[ind_first];
            ind_first++;
            ind_c++;
        } else {
            c[ind_c] = arr_second[ind_second];
            ind_second++;
            ind_c++;
        }

    }
}

void MergeSort(int* a, int aLen ) {
    if( aLen <= 1 ) {
        return;
    }
    int firstLen = aLen / 2;
    int secondLen = aLen - firstLen;
    MergeSort( a, firstLen );
    MergeSort( a + firstLen, secondLen );
    int* c = new int[aLen];
    merge(a, firstLen, a + firstLen, secondLen, c);
    memcpy( a, c, sizeof( int ) * aLen );
    delete[] c;
}

int main() {

    int n;
    std::cin >> n;
    int* arr = (int*)(malloc(n * sizeof(int)));
    for (int i = 0; i < n; i++) std::cin >> arr[i];
    for (int i = 0; i < n; i++) std::cout << arr[i] << " ";
    MergeSort(arr, n);

    std::cout << std::endl;
    for (int i = 0; i < n; i++) std::cout << arr[i] << " ";
    

    return 0;
}


/*
5
5 2 4 3 1
 */
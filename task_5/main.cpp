#include <iostream>
#include <cassert>
#include <cstring>

/*
 * 5_3. Закраска прямой 1.
 * На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка (Li и Ri).
 * Найти длину окрашенной части числовой прямой.
 */

template<class T>
class CmpDefault {
public:
    bool operator()( const T& l, const T& r ) { return l < r; }
};

template<class T, class Compare = CmpDefault<T>>
void merge
(
    const std::pair<T, int>* arr_first, int len_first,
    const std::pair<T, int>* arr_second, int len_second,
    std::pair<T, int>* c,
    Compare cmp = CmpDefault<T>()
)
{
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
        if (!cmp(arr_second[ind_second].first, arr_first[ind_first].first)) {
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

template<class T>
void MergeSort(std::pair<T, int>* a, int aLen ) {
    if( aLen <= 1 ) {
        return;
    }
    int firstLen = aLen / 2;
    int secondLen = aLen - firstLen;
    MergeSort( a, firstLen );
    MergeSort( a + firstLen, secondLen );
    auto* c = new std::pair<T, int>[aLen];
    merge(a, firstLen, a + firstLen, secondLen, c);
    memcpy( a, c, sizeof( std::pair<T, int> ) * aLen );
    delete[] c;
}

#define UP 23
#define DOWN 32

template<class T>
void run(std::pair<T, int>* arr, size_t num_of_elements) {

    MergeSort(arr, num_of_elements);

    int level = 0;
    int counter = 0;
    for (int i = 0; i < num_of_elements; i++) {
        if (level > 0) {
            assert(i != 0);
            counter += arr[i].first - arr[i-1].first;
        }

        switch(arr[i].second) {
            case UP:
                level++;
                break;
            case DOWN:
                level--;
                break;
            default:
                assert(false);
        }

        assert(level >= 0);
    }

    std::cout << counter;
}

int main() {
    // input
    size_t N = 0;
    std::cin >> N;
    size_t num_of_elements = N * 2;
    auto* arr = (std::pair<int, int>*)(malloc(num_of_elements * sizeof(std::pair<int, int>)));
    for (int i = 0; i < num_of_elements; i++) {
        std::cin >> arr[i].first;
        if (i % 2 == 0)
            arr[i].second = UP;
        else
            arr[i].second = DOWN;
    }

    run(arr, num_of_elements);

    delete[] arr;
    return 0;
}


/*
3
1 4
7 8
2 5
 */
#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <cmath>

using std::vector;
using std::string;
using std::cin;
using std::cout;

template<class T, class H>
class HashTable {
public:
	explicit HashTable( H& hasher );
	HashTable( const HashTable& ) = delete;
	HashTable& operator=( const HashTable& ) = delete;
	~HashTable() = default;

	// Каждый метод должен работать не более чем за один проход по таблице
	// Для каждого метода вычислять хэш надо один раз (за исключением growTable)

	// Пробируемся не более М раз, если больше - ассертим что firstDelPos нашли и вставляем в него
	// Если Del - идём дальше, но запоминаем firstDelPos если раньше Del не встречали
	// Если data - или идём дальше или return false
	// Если Empty - вставляем в firstDelPos если такой находили, если нет - вставляем в текущую ячейку
	bool Add( const T& data );

	// Пробируемся не более М раз, если больше - return false
	// Если Del - идём дальше
	// Если data - или идём дальше или return true
	// Если Empty - return false
	bool Has( const T& data ) const;

	// Пробируемся не более М раз, если больше - return false
	// Если Del - идём дальше
	// Если data - или идём дальше или помечаем Del и return true
	// Если Empty - return false
	bool Delete( const T& data );

private:
	struct HashTableCell {
	    enum CellType {
	        DATA,
	        EMPTY,
	        DELETED
	    };

		T data;
		CellType type; // TCellType = Key/Del/Empty

        HashTableCell() : type(CellType::EMPTY) {}
		HashTableCell(const T& data) : data(data), type(CellType::DATA) {}
	};
	H hasher;
	vector<HashTableCell> table; // vector<HashTableCell>
	size_t keysCount;

	void growTable(); // при alpha = 3/4
    #define ALPHA 0.75
};


template<class T, class H>
HashTable<T, H>::HashTable( H& _hasher ) :
	hasher( _hasher ),
	table( 8, HashTableCell()),
	keysCount( 0 )
{
}

template<class T, class H>
bool HashTable<T, H>::Add( const T& data )
{
	if (keysCount / (float)table.size() > ALPHA)
		growTable();

	int firstDelPos = -1;
	for (int i = 0; i < table.size(); i++) {
	    int hash = hasher(data, i) % table.size();
	    if ((table[hash].type == HashTableCell::CellType::DELETED) && (firstDelPos == -1))
	        firstDelPos = hash;
        if ((table[hash].type == HashTableCell::CellType::DATA) && (table[hash].data == data))
            return false;
        if (table[hash].type == HashTableCell::CellType::EMPTY) {
            if (firstDelPos != -1) {
                table[firstDelPos].data = data;
                table[firstDelPos].type = HashTableCell::CellType::DATA;
            }
            else {
                table[hash].data = data;
                table[hash].type = HashTableCell::CellType::DATA;
            }
            ++keysCount;
            return true;
        }
	}
	assert(firstDelPos >= 0);
	table[firstDelPos].data = data;
	table[firstDelPos].type = HashTableCell::CellType::DATA;
	++keysCount;
	return true;
}

template<class T, class H>
void HashTable<T, H>::growTable()
{
	vector<HashTableCell> newTable( table.size() * 2, HashTableCell() );
	for( size_t i = 0; i < table.size(); ++i ) {
	    if ((table[i].type == HashTableCell::CellType::DELETED) || (table[i].type == HashTableCell::CellType::EMPTY))
            continue;
        int firstDelPos = -1;
        bool isAdded = false;
        T data = table[i].data;
        for (int j = 0; j < newTable.size(); j++) {
            int hash = hasher(data, j) % newTable.size();
            if ((newTable[hash].type == HashTableCell::CellType::DELETED) && (firstDelPos == -1))
                firstDelPos = hash;
            if (newTable[hash].type == HashTableCell::CellType::EMPTY) {
                if (firstDelPos != -1) {
                    newTable[firstDelPos].data = data;
                    newTable[firstDelPos].type = HashTableCell::CellType::DATA;
                }
                else {
                    newTable[hash].data = data;
                    newTable[hash].type = HashTableCell::CellType::DATA;
                }
                isAdded = true;
                break;
            }

            if ((newTable[hash].type == HashTableCell::CellType::DATA) && (newTable[hash].data == data))
                assert(false);
        }
        if (!isAdded) {
            assert(firstDelPos >= 0);
            newTable[firstDelPos].data = data;
            newTable[firstDelPos].type = HashTableCell::CellType::DATA;
        }
	}
	table = std::move( newTable );
}

template<class T, class H>
bool HashTable<T, H>::Has( const T& data ) const
{
    for (int i = 0; i < table.size(); i++) {
        int hash = hasher(data, i) % table.size();
        if ((table[hash].type == HashTableCell::CellType::DATA) && (table[hash].data == data))
            return true;
        if (table[hash].type == HashTableCell::CellType::EMPTY) {
            return false;
        }
    }
    return false;
}

template<class T, class H>
bool HashTable<T, H>::Delete( const T& data )
{
    for (int i = 0; i < table.size(); i++) {
        int hash = hasher(data, i) % table.size();
        if ((table[hash].type == HashTableCell::CellType::DATA) && (table[hash].data == data)) {
            table[hash].type = HashTableCell::CellType::DELETED;
            --keysCount;
            return true;
        }
        if (table[hash].type == HashTableCell::CellType::EMPTY) {
            return false;
        }
    }
    return false;
}

struct ProbHasher
{
    // вычисление хэша строки методом Горнера
    unsigned int hash( const string& data ) const {
        int hash = 0;
        int a = 7;
        for (int i = 0; i < data.size(); i++) {
            hash += data[i] * (int)pow(a, i);
        }
        return hash;
    }

    // квадратичное пробирование, c = 0.5
	unsigned int operator()( const string& data, unsigned int i ) const {
        return hash(data) + (i * (i + 1)) / 2;
	}
};

int main()
{
    ProbHasher hasher;
	HashTable<string, ProbHasher> table( hasher );
	char operation;
	string word;
	while( cin >> operation >> word ) {
		if( operation == '+' )
			cout << ( table.Add( word ) ? "OK" : "FAIL" ) << std::endl;
		if( operation == '-' )
			cout << ( table.Delete( word ) ? "OK" : "FAIL" ) << std::endl;
		if( operation == '?' )
			cout << ( table.Has( word ) ? "OK" : "FAIL" ) << std::endl;
	}

	return 0;
}
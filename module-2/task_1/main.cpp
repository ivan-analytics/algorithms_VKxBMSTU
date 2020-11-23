#include <iostream>
#include <vector>
#include <assert.h>
#include <string>

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
	if (keysCount / table.size() > ALPHA)
		growTable();

	unsigned int hash = hasher( data ) % table.size();

	int firstDelPos = -1;
	for (int i = 0; i < table.size(); i++) {
	    int hash = hasher(data, i);
	    
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
	vector<HashTableNode*> newTable( table.size() * 2, nullptr );
	for( size_t i = 0; i < table.size(); ++i ) {
		HashTableNode* node = table[i];
		while( node != nullptr ) {
			HashTableNode* nextNode = node->Next;
			unsigned int newHash = hasher( node->Data ) % newTable.size();
			node->Next = newTable[newHash];
			newTable[newHash] = node;
			node = nextNode;
		}
	}
	table = std::move( newTable );
}

template<class T, class H>
bool HashTable<T, H>::Has( const T& data ) const
{
	int hash = hasher( data ) % table.size();
	HashTableNode* node = table[hash];
	while( node != nullptr && node->Data != data )
		node = node->Next;

	return node != nullptr;
}

template<class T, class H>
bool HashTable<T, H>::Delete( const T& data )
{
	unsigned int hash = hasher( data ) % table.size();
	HashTableNode* prevNode = nullptr;
	HashTableNode* node = table[hash];
	while( node != nullptr && node->Data != data ) {
		prevNode = node;
		node = node->Next;
	}

	if( node == nullptr ) // ключа нет
		return false;

	if( prevNode == nullptr ) // удаление из начала списка
		table[hash] = node->Next;
	else
		prevNode->Next = node->Next;
	delete node;

	--keysCount;
	return true;
}

struct ProbHasher
{
    // TODO: сделать через вычисление значения многочлена методом Горнера
    unsigned int hash( const string& data ) const {
        int hash = 0;
        for (char i : data)
            hash = hash * 7 + i;
        return hash;
    }

    // квадратичное пробирование, c = 0.5
	unsigned int operator()( const string& data, unsigned int i ) const {
        return hash(data) + (i * (i + 1)) / 2;
	}
};

int main()
{
	Hasher hasher;
	HashTable<string, Hasher> table( hasher );
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

/*
+ hello
+ bye
? bye
+ bye
- bye
? bye
? hello
*/
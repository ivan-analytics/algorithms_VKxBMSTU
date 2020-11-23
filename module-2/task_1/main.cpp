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
	~HashTable();

	// Каждый метод должен работать не более чем за один проход по таблице
	// Для каждого метода вычислять хэш надо один раз (за исключением growTable)

	// Пробируемся не более М раз, если больше - ассертим что firstDelPos нашли и вставляем в него
	// Если Del - идём дальше, но запоинаем firstDelPos если раньше Del не встречали
	// Если Data - или идём дальше или return false
	// Если Empty - вставляем в firstDelPos если такой находили, если нет - вставляем в текущую ячейку
	bool Add( const T& data );

	// Пробируемся не более М раз, если больше - return false
	// Если Del - идём дальше
	// Если Data - или идём дальше или return true
	// Если Empty - return false
	bool Has( const T& data ) const;

	// Пробируемся не более М раз, если больше - return false
	// Если Del - идём дальше
	// Если Data - или идём дальше или помечаем Del и return true
	// Если Empty - return false
	bool Delete( const T& data );

private:
	struct HashTableNode {
		T Data;
		HashTableNode* Next; // TCellType = Key/Del/Empty

		HashTableNode() : Next( nullptr ) {}
		HashTableNode( const T& data, HashTableNode* next ) : Data( data ), Next( next ) {}
	};
	H hasher;
	vector<HashTableNode*> table; // vector<HashTableNode>
	size_t keysCount;

	void growTable(); // при alpha = 2/3
};

template<class T, class H>
HashTable<T, H>::HashTable( H& _hasher ) :
	hasher( _hasher ),
	table( 8, nullptr ),
	keysCount( 0 )
{
}

template<class T, class H>
HashTable<T, H>::~HashTable()
{
	for( size_t i = 0; i < table.size(); ++i ) {
		HashTableNode* node = table[i];
		while( node != nullptr ) {
			HashTableNode* nextNode = node->Next;
			delete node;
			node = nextNode;
		}
	}
}

template<class T, class H>
bool HashTable<T, H>::Add( const T& data )
{
	if( keysCount > 3 * table.size() )
		growTable();

	unsigned int hash = hasher( data ) % table.size();
	HashTableNode* node = table[hash];
	while( node != nullptr && node->Data != data )
		node = node->Next;

	if( node != nullptr )
		return false;

	table[hash] = new HashTableNode( data, table[hash] );
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

struct Hasher
{
	unsigned int operator()( const string& data ) const {
		int hash = 0;
		for( size_t i = 0; i < data.size(); ++i )
			hash = hash * 7 + data[i];
		return hash;
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
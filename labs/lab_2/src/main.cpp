import std;

template<typename T>
struct Node
{
	T value;
	Node *link_next;
};


template<typename T>
concept ListValue = std::floating_point<T> || (std::integral<T> && !std::same_as<T, bool> && !std::same_as<T, char>);
//грубо говоря - булевое значение, показывающее удовлетворяет ли тип данных или нет


template<ListValue T>
class LinkedList final
{
private:
	Node<T> *_tail = nullptr;
	std::size_t _size = 0;

	void check_empty() const
	{
		if (_size == 0)
		{
			throw std::out_of_range("LinkedList is empty.");
		}
	}

public:
	LinkedList() = default;

	//Конструктор, заполняющий список случайными значениями согласно заданию
	LinkedList(std::size_t size, long double seed)
	{

	}

	//Конструктор копирования
	LinkedList(const LinkedList &other)
	{

	}

	~LinkedList()
	{
		delete[] _tail;
	}

	LinkedList operator=(LinkedList&& other)
	{

	}

	void push_tail()
	{
	
	}

	void push_tail()
	{
	
	}

	void push_head()
	{
	
	}

	void push_head()
	{
	
	}

	void pop_head()
	{
	
	}

	void pop_tail()
	{

	}

	void delete_node()
	{

	}

	LinkedList operator[]()
	{

	}
};

int main()
{
	return 0;
}

import std;

template<typename T>
struct Node
{
	T value;
	Node *link_next;
};

template<typename T>
class LinkedList
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

	LinkedList(std::size_t size, long double seed)
	{

	}

	LinkedList(const LinkedList &other)
	{

	}

	~LinkedList() {}

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

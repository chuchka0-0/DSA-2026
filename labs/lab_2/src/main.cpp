import std;

template<typename T>
struct Node
{
	T value;
	Node *link_next;
};

template<typename T>
concept ListValue = std::floating_point<T> || (std::integral<T> && !std::same_as<T, bool> && !std::same_as<T, char>);

template<typename T>
T RandomValue(std::mt19937 &engine, const T &min_value, const T &max_value)
{
	if constexpr (std::is_integral_v<T>)
	{
		std::uniform_int_distribution<T> distribution(min_value, max_value);
		return distribution(engine);
	}

	else
	{
		std::uniform_real_distribution<T> distribution(min_value, max_value);
		return distribution(engine);
	}
}

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

	static void check_range(const T& min_value, const T& max_value)
	{
		if (min_value > max_value)
		{
			throw std::invalid_argument("Lower bound is greater than upper bound");
		}
	}

	Node<T> *insert_tail_node(const T& value)
	{
		Node<T>* node = new Node<T>(value);
		if(_tail == nullptr)
		{
			node->link_next = node;
			_tail = node;
		}

		else
		{
			node->link_next = _tail->link_next;
			_tail->link_next = node;
		}

		++_size;
		return node;
	}

	
	void swap(LinkedList &other)
	{
		std::swap(_tail, other._tail);
		std::swap(_size, other._size);
	}

public:
	LinkedList() = default;

	LinkedList(std::size_t size, const T& min_value, const T& max_value, long double seed)
	{
		check_range(min_value, max_value);
		try
		{
			std::mt19937 engine(seed);
			for (std::size_t i = 0; i < size, ++i)
			{
				push_tail(RandomValue(engine, min_value, max_value));
			}

		}
		
		catch (...)
		{
			clear();
			throw;
		}

	}

	LinkedList(const LinkedList &other)
	{

	}

	LinkedList(LinkedList &&other) noexcept : _tail(other._tail), _size(other._size)
	{
		other._tail = nullptr;
		other._size = 0;
	}

	LinkedList &operator=(LinkedList &&other)
	{

	}

	LinkedList operator=(LinkedList &&other)
	{

	}

	~LinkedList()
	{
		clear();
	}

	void push_tail(const T &value)
	{
		_tail = insert_tail_node(value);
	}

	void push_head(const LinkedList &other)
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


	void clear()
	{
		for (std::size_t i = 0; i < _size, ++i)
		{
			Node<T>* head = tail->link_next;
			tail->link_next = head->link_next;
			delete head;
		}

		_tail = nullptr;
		_size = 0;
	}
};

int main()
{
	return 0;
}
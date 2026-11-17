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

	void append(LinkedList &other) noexcept
	{
		if (other._tail == nullptr)
		{
			return;
		}

		if (_tail != nullptr)
		{
			Node<T>* head = _tail->link_next;
			_tail->link_next = other._tail->link_next;
			other._tail->link_next = head;
		}

		_tail = other._tail;
		_size += other._size;
		other._tail = nullptr;
		other._size = 0;
	}

	void swap(LinkedList &other) noexcept
	{
		std::swap(_tail, other._tail);
		std::swap(_size, other._size);
	}

public:
	LinkedList() = default;

	LinkedList(std::size_t size, const T& min_value, const T& max_value, std::mt19937::result_type seed)
	{
		check_range(min_value, max_value);
		try
		{
			std::mt19937 engine(seed);
			for (std::size_t i = 0; i < size; ++i)
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
		try
		{
			const Node<T>* node = other._tail;
			for (std::size_t i = 0; i < other._size; ++i)
			{
				node = node->link_next;
				push_tail(node->value);
			}
		}

		catch (...)
		{
			clear();
			throw;
		}
	}

	LinkedList(LinkedList &&other) noexcept : _tail(other._tail), _size(other._size)
	{
		other._tail = nullptr;
		other._size = 0;
	}

	LinkedList &operator=(const LinkedList &other)
	{
		if (this != &other)
		{
			LinkedList copy(other);
			swap(copy);
		}

		return *this;
	}

	LinkedList &operator=(LinkedList &&other) noexcept
	{
		if (this != &other)
		{
			swap(other);
		}

		return *this;
	}

	~LinkedList()
	{
		clear();
	}

	void push_tail(const T &value)
	{
		_tail = insert_tail_node(value);
	}

	void push_tail(const LinkedList &other)
	{
		LinkedList copy(other);
		append(copy);
	}
	
	void push_head(const T& value)
	{
		insert_tail_node(value);
	}

	void push_head(const LinkedList &other)
	{
		LinkedList copy(other);
		copy.append(*this);
		swap(copy);
	}

	void pop_head(const T& value)
	{
	
	}

	void pop_tail(const T& value)
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
		for (std::size_t i = 0; i < _size; ++i)
		{
			Node<T>* head = _tail->link_next;
			_tail->link_next = head->link_next;
			delete head;
		}

		_tail = nullptr;
		_size = 0;
	}






	friend std::ostream& operator<<(std::ostream& stream, const LinkedList& list)
	{
		stream << '(';
		if (list._tail != nullptr)
		{
			const Node<T>* node = list._tail->link_next;
			for (std::size_t i = 0; i < list._size; ++i)
			{
				if (i != 0)
				{
					stream << ", ";
				}
				stream << node->value;
				node = node->link_next;
			}
		}
		return stream << ')';
	}
};

int main()
{
	LinkedList<int> a(2, -3, 3, 400);
	LinkedList<int> b(4, -12, 20, 340);
	std::cout << a;
	std::cout << b;
	a.push_tail(b);
	std::cout << a;
	return 0;
}
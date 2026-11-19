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

	Node<T> *node_at(std::size_t index) const
	{
		check_empty();
		index %= _size;

		if (index == _size - 1)
		{
			return _tail;
		}

		Node<T>* node = _tail->link_next;
		for (std::size_t i = 0; i < index; ++i)
		{
			node = node->link_next;
		}

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

	T pop_head()
	{
		check_empty();
		Node<T>* head = _tail->link_next;
		T value = std::move(head->value);
		if (_size == 1)
		{
			_tail = nullptr;
		}
		else
		{
			_tail->link_next = head->link_next;
		}
		delete head;
		--_size;

		return value;
	}

	T pop_tail()
	{
		check_empty();
		Node<T>* old_tail = _tail;
		T value = std::move(old_tail->value);
		if (_size == 1)
		{
			_tail = nullptr;
		}
		else
		{
			Node<T>* previous = node_at(_size - 2);
			previous->link_next = old_tail->link_next;
			_tail = previous;
		}

		delete old_tail;
		--_size;

		return value;
	}

	std::size_t delete_node(const T &value)
	{
		const std::size_t initial_size = _size;
		Node<T>* previous = _tail;

		for (std::size_t i = 0; i < initial_size; ++i)
		{
			Node<T>* current = previous->link_next;
			if (current->value != value)
			{
				previous = current;
				continue;
			}

			previous->link_next = current->link_next;

			if (current == _tail)
			{
				_tail = previous;
			}

			delete current;
			--_size;
		}

		if (_size == 0)
		{
			_tail = nullptr;
		}

		return initial_size - _size;
	}

	std::size_t size() const { return _size; }

	T& operator[](std::size_t index)
	{
		return node_at(index)->value;
	}

	const T& operator[](std::size_t index) const
	{
		return node_at(index)->value;
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

	friend std::ostream& operator<<(std::ostream &stream, const LinkedList &list)
	{
		stream << '(';
		if(list._tail != nullptr)
		{
			const Node<T> *node = list._tail->link_next;
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

template<ListValue T>
void SieveOfEratosthenes(LinkedList<T>& list)
{
	if (list.size() == 0)
	{
		return;
	}

	const T max_value = list[list.size() - 1]; // список упорядочен, последний элемент максимальный

	list.delete_node(T(1)); // 1 не простое
	for (T i = 2; i * i <= max_value; ++i)
	{
		for (T j = i * i; j <= max_value; j += i)
		{
			list.delete_node(j);
		}
	}
}

template<ListValue T>
LinkedList<T> PrimesUpTo(T n)
{
	LinkedList<T> list;
	for (T i = 1; i <= n; ++i)
	{
		list.push_tail(i);
	}

	SieveOfEratosthenes(list);
	return list;
}

template<typename Bebe>
void RunTest(const char *title, Bebe &&test)
{
	std::cout << "--- " << title << " ---\n";
	test();
	std::cout << '\n';
}

template<ListValue T>
void FillSequence(LinkedList<T> &list, T first, T last)
{
	for(T i = first; i <= last; ++i)
	{
		list.push_tail(i);
	}
}

void TestConstructors()
{
	LinkedList<int> empty;
	std::cout << "Default list: " << empty << ", size = " << empty.size() << '\n';

	LinkedList<double> random_list(5, -1.0, 1.0, 67);
	std::cout << "Random doubles (seed 67): " << random_list << '\n';

	LinkedList<int> random_ints(6, 1, 9, 52);
	std::cout << "Random ints (seed 52): " << random_ints << '\n';

	LinkedList<int> copy(random_ints);
	std::cout << "Copy: " << copy << '\n';
	std::cout << "Original: " << random_ints << '\n';

	LinkedList<int> moved(std::move(copy));
	std::cout << "Moved-from list: " << moved << '\n';
}

void TestAssignment()
{
	LinkedList<int> a(6, 0, 13, 42);
	LinkedList<int> b;
	b = a;
	std::cout << "Copy assignment: " << b << '\n';
	b = b;
	std::cout << "Self-assignment is safe: " << b << '\n';

	LinkedList<int> c;
	c = std::move(a);
	std::cout << "Move assignment: " << c << '\n';
}

void TestPushPop()
{
	LinkedList<int> list;
	list.push_tail(2);
	list.push_tail(3);
	list.push_head(1);
	std::cout << "After push_tail(2), push_tail(3), push_head(1): " << list << '\n';

	std::cout << "pop_head() -> " << list.pop_head() << ", list: " << list << '\n';
	std::cout << "pop_tail() -> " << list.pop_tail() << ", list: " << list << '\n';
	std::cout << "pop_tail() -> " << list.pop_tail() << ", list: " << list << '\n';

	list.push_tail(7);
	std::cout << "Single element pop: " << list.pop_tail() << ", list: " << list << ", size = " << list.size() << '\n';
}

void TestPushLists()
{
	LinkedList<int> head;
	FillSequence(head, 1, 3);
	LinkedList<int> tail;
	FillSequence(tail, 4, 6);
	LinkedList<int> list;
	list.push_head(head);
	std::cout << "push_head(list 1..3) into empty: " << list << '\n';
	list.push_tail(tail);
	std::cout << "push_tail(list 4..6): " << list << '\n';

	LinkedList<int> empty;
	list.push_tail(empty);
	std::cout << "push_tail(empty list) changes nothing: " << list << '\n';
	empty.push_tail(list);
	std::cout << "push_tail into empty list: " << empty << '\n';
}

void TestDeleteNode()
{
	LinkedList<int> list;
	FillSequence(list, 1, 5);
	list.push_tail(3);
	list.push_tail(3);
	std::cout << "Before delete_node(3): " << list << '\n';
	std::cout << "delete_node(3) removed " << list.delete_node(3) << " element(s): " << list << '\n';
	std::cout << "delete_node(42) removed " << list.delete_node(42) << " element(s): " << list << '\n';
	std::cout << "delete_node(1) removed " << list.delete_node(1) << " element(s): " << list << '\n';
	std::cout << "delete_node(2) removed " << list.delete_node(2) << " element(s): " << list << '\n';
	std::cout << "delete_node on empty list removed " << list.delete_node(1) << " element(s)\n";
}

void TestIndexing()
{
	LinkedList<int> list;
	FillSequence(list, 10, 50);
	std::cout << "List: " << list << '\n';
	std::cout << "list[0] = " << list[0] << ", list[4] = " << list[4] << '\n';
	std::cout << "Cyclic index list[5] = " << list[5] << ", list[12] = " << list[12] << '\n';
	std::cout << "Cyclic index list[47] = " << list[47] << " (same as list[2])\n";
	list[1] = 99;
	std::cout << "After list[1] = 99: " << list << '\n';

	const LinkedList<int>& const_view = list;
	std::cout << "Const access list[3] = " << const_view[3] << '\n';
}

void TestClearAndErrors()
{
	LinkedList<int> list;
	FillSequence(list, 1, 4);
	list.clear();
	std::cout << "After clear(): " << list << ", size = " << list.size() << '\n';

	try
	{
		list.pop_head();
	}
	catch (const std::exception& error)
	{
		std::cout << "pop_head on empty list: " << error.what() << '\n';
	}

	try
	{
		list.pop_tail();
	}
	catch (const std::exception& error)
	{
		std::cout << "pop_tail on empty list: " << error.what() << '\n';
	}

	try
	{
		(void)list[0];
	}
	catch (const std::exception& error)
	{
		std::cout << "operator[] on empty list: " << error.what() << '\n';
	}

	try
	{
		LinkedList<int> bad(3, 10, 1, 1);
	}
	catch (const std::exception& error)
	{
		std::cout << "random constructor with min > max: " << error.what() << '\n';
	}
}


void TestSieve()
{
	std::cout << "PrimesUpTo(50): " << PrimesUpTo(50) << '\n';
	std::cout << "PrimesUpTo<long long>(100): " << PrimesUpTo<long long>(100) << '\n';

	LinkedList<int> list;
	for (int i = 1; i <= 30; ++i)
	{
		list.push_tail(i);
	}
	SieveOfEratosthenes(list);
	std::cout << "Sieve on ordered list 1..30: " << list << '\n';

	LinkedList<double> empty_list;
	SieveOfEratosthenes(empty_list);
	std::cout << "Sieve on empty list: " << empty_list << '\n';
}

int main()
{
	RunTest("Constructors", TestConstructors);
	RunTest("Assignment operators", TestAssignment);
	RunTest("push_head / push_tail / pop_head / pop_tail", TestPushPop);
	RunTest("push_head / push_tail with another list", TestPushLists);
	RunTest("delete_node", TestDeleteNode);
	RunTest("Index access with cyclic modulo", TestIndexing);
	RunTest("clear() and error handling", TestClearAndErrors);
	RunTest("Task 5: Sieve of Eratosthenes", TestSieve);
	return 0;
}
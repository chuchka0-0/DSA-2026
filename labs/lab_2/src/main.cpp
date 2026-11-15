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
//именованное ограничение на параметры шаблона: предикат, который проверяет, подходит ли тип.


template<typename T>
T RandomValue(std::mt19937 &engine, const T &min_value, const T &max_value) // Передаётся по ссылке, тк при каждом вызове его внутреннее состояние меняется
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


	void clear()
	{
		for (std::size_t i = 0; i < _size, ++i)
		{
			Node<T>* head = tail->li_next;
			tail->li_next = head->li_next;
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

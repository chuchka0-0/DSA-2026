// Структура односвязного узла
import std;

template<typename T>
struct Node
{
	T info;
	Node* li_next;

	Node() = default;
};

// Реализовать функцию, которая выводит на экран значения всех четных элементов.
template<typename T>
void Print(Node<T>* point)
{
	while (point)
	{
		if (point->info % 2 == 0)
		{
			std::cout << point->info << '\n';
		}

		point = point->li_next;
	}
}


// Подсчитать и вернуть количество элементов с отрицательными значениями в односвязном списке.
template<typename T>
int count_elem(Node<T>* point)
{
	int count = 0;
	while (point)
	{
		if (point->info < 0)
		{
			++count;
		}

		point = point->li_next;
	}
	return count;
}

// Вычислить среднее арифметическое всех числовых значений односвязного списка.

template<typename T>
T avg(Node<T> *point)
{
	T sum = 0;
	int count = 0;

	while (point)
	{
		sum += point->info;
		++count;
		point = point->li_next;
	}

	return sum / count;
}

// Проверить, упорядочены ли элементы односвязного списка по неубыванию.
template<typename T>
bool sorted(Node<T> *point)
{
	while (point->li_next)
	{
		if (point->info > point->li_next->info)
		{
			return false;
		}

		point = point->li_next;
	}

	return true;
}


int main()
{
	Node<int> one;
	one.info = 1;

	Node<int> two;
	two.info = 2;
	one.li_next = &two;

	two.li_next = nullptr;

	Print(&one);
	std::cout << count_elem(&one) << '\n';
	std::cout << avg(&one) << '\n';
	std::cout << sorted(&one) << '\n';

	return 0;
}
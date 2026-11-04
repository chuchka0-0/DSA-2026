import std;

// Задача 1: Подсчет вхождений символа в C-строку
// Написать функцию, которая принимает C-подобную строку (const char*) и символ (const char),
// подсчитывает и возвращает количество вхождений указанного символа в строку.

int char_count(const char* string, const char ch)
{
	const char* p = string;
	int count = 0;
	while (*p != '\0')
	{
		if (*p == ch)
		{
			++count;
		}

		++p;
	}

	return count;
}

int main()
{
	const char* str1 = "I love Nikitka";
	std::cout << char_count(str1, 'i');
	return 0;
}
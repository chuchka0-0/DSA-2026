import std;

// Задача 1: Подсчет вхождений символа в C-строку
// Написать функцию, которая принимает C-подобную строку (const char*) и символ (const char),
// подсчитывает и возвращает количество вхождений указанного символа в строку.

int char_count(const char* string, const char ch)
{
	const char* p = string;
	int count = 0;

	while(*p != '\0')
	{
		if (*p == ch)
		{
			++count;
		}

		++p;
	}

	return count;
}

// Задание 2: Проверка C-строки на палиндром
// Написать функцию, которая проверяет, является ли переданная C-строка палиндромом (симметричной относительно центра).

bool is_palindrom(const char* string)
{
	const char* end = string;
	while(*end != '\0')
	{
		++end;
	}
	--end;

	const char* start = string;
	while(start < end)
	{
		if (*start != *end)
		{
			return false;
		}

		++start;
		--end;
	}
	
	return true;
}

// Задание 3:
// Определить, есть ли в переданной C - строке повторяющиеся символы.
// Функция возвращает логическое значение(true, если есть хотя бы один повтор, иначе false).

bool repeat_char(const char* string)
{
	const char* p = string;
	const char* q;

	while (*p != '\0')
	{
		q = p + 1;
		while (*q != '\0')
		{
			if (*p == *q)
			{
				return true;
			}

			++q;
		}

		++p;
	}

	return false;
}

int main()
{
	const char* str1 = "I love Nikitka";
	std::cout << char_count(str1, 'i') << '\n';
	
	const char* str2 = "asdfghjhgfdsa";
	std::cout << is_palindrom(str2);
	return 0;
}
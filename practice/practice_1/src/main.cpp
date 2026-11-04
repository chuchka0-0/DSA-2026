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

// Задание 3: Поиск повторяющихся символов
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

//Задание4: Самая длинная последовательность одинаковых символов
std::string_view longest_repeat(const char* string)
{
	const char* p = string;
	const char* long_rep = string;

	int count = 1;
	int max_count = 1;

	while (*p != '\0')
	{
		if (*p == *(p + 1))
		{
			++count;
		}
		else
		{
			if (count > max_count)
			{
				max_count = count;
				long_rep = p - (count - 1);
			}
			count = 1;
		}
		++p;
	}

	return std::string_view(long_rep, max_count);
}

// Задание 5: Поиск подстроки в строке
const char* find_sub_str(const char* string, const char* sub_string)
{
	if (!string || !sub_string)
	{
		return nullptr;
	}

	if (*sub_string == '\0')
	{
		return string;
	}

	while (*string != '\0')
	{
		const char* current_str = string;
		const char* current_sub = sub_string;

		while (*current_str != '\0' && *current_sub != '\0' && *current_str == *current_sub)
		{
			++current_str;
			++current_sub;
		}

		if (*current_sub == '\0')
		{
			return string;
		}

		++string;
	}

	return nullptr;
	
}

int main()
{
	const char* str1 = "I love Nikitka";
	std::cout << char_count(str1, 'i') << '\n';
	
	const char* str2 = "asdfghjhgfdsa";
	std::cout << is_palindrom(str2) << '\n';

	std::cout << repeat_char(str1) << '\n';
	
	const char* str4 = "mmaasssshka";
	std::cout << longest_repeat(str4) << '\n';

	const char* str5 = "Nikitka";
	std::cout << find_sub_str(str1, str5) << '\n';
	return 0;
}
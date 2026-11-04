import std;

class PairContainer {
private:
    struct Pair {
        char* key;
        float val;

        Pair() : key(nullptr), val(0.0f) {}

        Pair(const char* k, float v) : key(nullptr), val(v) {
            if (k != nullptr) {
                key = strdup(k);
                if (!key) throw std::bad_alloc();
            }
        }

        Pair(const Pair& p) : key(nullptr), val(p.val) {
            if (p.key != nullptr) {
                key = strdup(p.key);
                if (!key) throw std::bad_alloc();
            }
        }

        Pair& operator=(const Pair& p) {
            if (this == &p) return *this;
            free(this->key);
            this->key = nullptr;
            this->val = p.val;
            if (p.key != nullptr) {
                this->key = strdup(p.key);
                if (!this->key) throw std::bad_alloc();
            }
            return *this;
        }

        ~Pair() {
            free(key);
        }
    };

    Pair* m;        // Сплошной динамический массив объектов Pair (эффективнее, чем Pair**)
    int count;      // Текущее количество элементов
    int capacity;   // Максимальная вместимость

    // --- Вспомогательный метод реаллокации ---
    void reallocate(int newCapacity) {
        if (newCapacity <= capacity) return;

        Pair* newM = new Pair[newCapacity];
        for (int i = 0; i < count; ++i) {
            newM[i] = m[i]; // Вызывается Pair::operator= (глубокое копирование)
        }
        delete[] m;
        m = newM;
        capacity = newCapacity;
    }

public:
    // --- 1. Конструкторы и деструктор (Правило трёх для контейнера) ---

    PairContainer(int initialCapacity = 4)
        : m(new Pair[initialCapacity]), count(0), capacity(initialCapacity) {
    }

    PairContainer(const PairContainer& other)
        : m(new Pair[other.capacity]), count(other.count), capacity(other.capacity) {
        for (int i = 0; i < count; ++i) {
            m[i] = other.m[i];
        }
    }

    PairContainer& operator=(const PairContainer& other) {
        if (this == &other) return *this;

        delete[] m;
        capacity = other.capacity;
        count = other.count;
        m = new Pair[capacity];
        for (int i = 0; i < count; ++i) {
            m[i] = other.m[i];
        }
        return *this;
    }

    ~PairContainer() {
        delete[] m; // Вызывает деструкторы для всех объектов Pair в массиве
    }

    // --- 2. Операторы доступа ---

    // Доступ по строковому ключу (создает элемент, если его нет - как в std::map)
    float& operator[](const char* key) {
        if (key == nullptr) throw std::invalid_argument("Key cannot be null");

        // Ищем существующий ключ
        for (int i = 0; i < count; ++i) {
            if (strcmp(m[i].key, key) == 0) {
                return m[i].val;
            }
        }

        // Если ключа нет - расширяем массив при необходимости и добавляем новый
        if (count == capacity) {
            reallocate(capacity * 2);
        }
        m[count++] = Pair(key, 0.0f);
        return m[count - 1].val;
    }

    // Доступ по числовому индексу
    float& operator[](int index) {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of bounds");
        }
        return m[index].val;
    }

    // Константные версии для чтения (опционально, но желательно)
    const float& operator[](int index) const {
        if (index < 0 || index >= count) throw std::out_of_range("Index out of bounds");
        return m[index].val;
    }

    // --- 3. Методы управления и получения данных ---

    int size() const {
        return count;
    }

    int getCapacity() const {
        return capacity;
    }

    // Явный метод добавления (если нужно добавить без поиска через operator[])
    void add(const char* key, float val) {
        if (key == nullptr) throw std::invalid_argument("Key cannot be null");
        if (count == capacity) {
            reallocate(capacity * 2);
        }
        m[count++] = Pair(key, val);
    }

    // --- 4. Оператор составного сложения (слияние) ---
    PairContainer& operator+=(const PairContainer& other) {
        for (int i = 0; i < other.count; ++i) {
            (*this)[other.m[i].key] = other.m[i].val;
        }
        return *this;
    }
};

int main() {
    try {
        PairContainer dict1;
        dict1["apple"] = 1.5f;
        dict1["banana"] = 2.3f;
        dict1.add("orange", 3.1f);

        std::cout << "Dict1 size: " << dict1.size() << "\n";
        std::cout << "Value at index 1: " << dict1[1] << "\n";

        PairContainer dict2;
        dict2["banana"] = 5.0f; // Перезапишет значение при слиянии
        dict2["grape"] = 4.2f;

        dict1 += dict2;

        std::cout << "Merged size: " << dict1.size() << "\n";
        std::cout << "Updated banana: " << dict1["banana"] << "\n";

        try {
            std::cout << dict1[100] << "\n"; // Выбросит std::out_of_range
        }
        catch (const std::out_of_range& e) {
            std::cout << "Caught exception: " << e.what() << "\n";
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
    }

    return 0;
}
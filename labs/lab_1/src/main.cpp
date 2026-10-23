import std;

template <typename T>
T saturate(long double value)
{
    const T low = std::numeric_limits<T>::lowest();
    const T high = std::numeric_limits<T>::max();
    if (value < static_cast<long double>(low))
    {
        return low;
    }
    if (value > static_cast<long double>(high))
    {
        return high;
    }
    return static_cast<T>(value);
}

template <typename T>
class Image
{
private:
    std::size_t _rows;
    std::size_t _cols;
    T *_data;

    static constexpr double _epsilon = 1e-6;

    void checkIndex(const std::size_t rowIndex, const std::size_t colIndex) const
    {
        if (rowIndex >= _rows || colIndex >= _cols)
        {
            throw std::out_of_range("Image index is out of range");
        }
    }

    T valueAt(const std::size_t rowIndex, const std::size_t colIndex) const
    {
        return (rowIndex < _rows && colIndex < _cols) ? _data[rowIndex * _cols + colIndex] : T{};
    }

    static T add(T a, T b) { return saturate<T>(a + b); }

    static T multiply(T a, T b) { return saturate<T>(a * b); }

    static T invert(T value)
    {
        if constexpr (std::is_same_v<T, bool>)
        {
            return !value;
        }
        else if constexpr (std::is_signed_v<T>)
        {
            return saturate<T>(-static_cast<long double>(value));
        }
        else
        {
            return static_cast<T>(std::numeric_limits<T>::max() - value);
        }
    }

    static bool equal(T a, T b)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            return std::fabs(static_cast<double>(a) - static_cast<double>(b)) < _epsilon;
        }
        else
        {
            return a == b;
        }
    }

    template <typename Operation>
    Image combine(const Image &other, Operation operation) const
    {
        Image result(std::max(_rows, other._rows), std::max(_cols, other._cols), false);
        for (std::size_t i = 0; i < result._rows; ++i)
        {
            for (std::size_t j = 0; j < result._cols; ++j)
            {
                result._data[i * result._cols + j] = operation(valueAt(i, j), other.valueAt(i, j));
            }
        }
        return result;
    }

    template <typename Operation>
    Image mapped(Operation operation) const
    {
        Image result(_rows, _cols, false);
        for (std::size_t i = 0; i < _rows * _cols; ++i)
        {
            result._data[i] = operation(_data[i]);
        }
        return result;
    }

    void fillRandom()
    {
        std::mt19937 generator(std::random_device{}());
        for (std::size_t i = 0; i < _rows * _cols; ++i)
        {
            if constexpr (std::is_same_v<T, bool>)
            {
                std::bernoulli_distribution distribution(0.5);
                _data[i] = distribution(generator);
            }
            else if constexpr (std::is_floating_point_v<T>)
            {
                std::uniform_real_distribution<T> distribution(-100, 100);
                _data[i] = distribution(generator);
            }
            else
            {
                const int low = std::max(static_cast<int>(std::numeric_limits<T>::lowest()), -100);
                const int high = std::min(static_cast<int>(std::numeric_limits<T>::max()), 100);
                std::uniform_int_distribution<int> distribution(low, high);
                _data[i] = static_cast<T>(distribution(generator));
            }
        }
    }

public:
    Image(std::size_t rows, std::size_t cols, bool randomFill)
        : _rows(rows), _cols(cols), _data(nullptr)
    {
        if (rows == 0 || cols == 0)
        {
            throw std::invalid_argument("Image size must be positive");
        }
        _data = new T[_rows * _cols]();
        if (randomFill)
        {
            fillRandom();
        }
    }

    Image(const Image &other)
        : _rows(other._rows), _cols(other._cols), _data(new T[other._rows * other._cols])
    {
        std::copy(other._data, other._data + _rows * _cols, _data);
    }

    Image &operator=(const Image &other)
    {
        if (this != &other)
        {
            Image temp(other);
            std::swap(_rows, temp._rows);
            std::swap(_cols, temp._cols);
            std::swap(_data, temp._data);
        }
        return *this;
    }

    ~Image() { delete[] _data; }

    std::size_t rows() const { return _rows; }
    std::size_t cols() const { return _cols; }

    T &operator()(std::size_t rowIndex, std::size_t colIndex)
    {
        checkIndex(rowIndex, colIndex);
        return _data[rowIndex * _cols + colIndex];
    }

    const T &operator()(std::size_t rowIndex, std::size_t colIndex) const
    {
        checkIndex(rowIndex, colIndex);
        return _data[rowIndex * _cols + colIndex];
    }

    friend std::ostream &operator<<(std::ostream &out, const Image &image)
    {
        for (std::size_t i = 0; i < image._rows; ++i)
        {
            for (std::size_t j = 0; j < image._cols; ++j)
            {
                out << std::setw(9) << +image._data[i * image._cols + j];
            }
            out << '\n';
        }
        return out;
    }

    Image operator+(const Image &other) const { return combine(other, add); }
    Image operator*(const Image &other) const { return combine(other, multiply); }

    Image operator+(T constant) const
    {
        return mapped([constant](T value)
                      { return add(value, constant); });
    }

    Image operator*(T constant) const
    {
        return mapped([constant](T value)
                      { return multiply(value, constant); });
    }

    friend Image operator+(T constant, const Image &image) { return image + constant; }
    friend Image operator*(T constant, const Image &image) { return image * constant; }

    Image operator!() const { return mapped(invert); }

    long double fillRatio() const
    {
        long double sum = 0;
        for (std::size_t i = 0; i < _rows * _cols; ++i)
        {
            sum += static_cast<long double>(_data[i]);
        }
        const long double maxValue = static_cast<long double>(std::numeric_limits<T>::max());
        return sum / (static_cast<long double>(_rows * _cols) * maxValue);
    }

    bool operator==(const Image &other) const
    {
        if (_rows != other._rows || _cols != other._cols)
        {
            return false;
        }
        for (std::size_t i = 0; i < _rows * _cols; ++i)
        {
            if (!equal(_data[i], other._data[i]))
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Image &other) const { return !(*this == other); }
};

// Рисование отрезка алгоритмом Брезенхэма (x — столбец, y — строка)
template <typename T>
void drawLine(Image<T> &image, int x1, int y1, int x2, int y2, T value)
{
    const int width = static_cast<int>(image.cols());
    const int height = static_cast<int>(image.rows());
    if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 ||
        x1 >= width || x2 >= width || y1 >= height || y2 >= height)
    {
        throw std::out_of_range("Line goes outside of the image");
    }

    const int dx = std::abs(x2 - x1);
    const int dy = -std::abs(y2 - y1);
    const int stepX = (x1 < x2) ? 1 : -1;
    const int stepY = (y1 < y2) ? 1 : -1;
    int error = dx + dy;

    while (true)
    {
        image(static_cast<std::size_t>(y1), static_cast<std::size_t>(x1)) = value;
        if (x1 == x2 && y1 == y2)
        {
            break;
        }
        const int Р = 2 * error;
        if (doubledError >= dy)
        {
            error += dy;
            x1 += stepX;
        }
        if (doubledError <= dx)
        {
            error += dx;
            y1 += stepY;
        }
    }
}

template <typename T>
void demonstrate(const char *typeName)
{
    std::cout << "=========== Type: " << typeName << " ===========\n";

    Image<T> a(3, 4, true);
    Image<T> b(2, 5, true);
    const T big = std::numeric_limits<T>::max();

    std::cout << "Image a (3x4):\n"
              << a;
    std::cout << "\nImage b (2x5):\n"
              << b;
    std::cout << "\na + b (size 3x5):\n"
              << a + b;
    std::cout << "\na * b (size 3x5):\n"
              << a * b;
    std::cout << "\na + max value (overflow is clamped):\n"
              << a + big;
    std::cout << "\nmax value * a:\n"
              << big * a;
    std::cout << "\n!a (inverted):\n"
              << !a;
    std::cout << "\nFill ratio of a: " << a.fillRatio() << '\n';

    a(0, 0) = b(1, 1);
    std::cout << "\nAfter a(0,0) = b(1,1):\n"
              << a;

    Image<T> copy = a;
    Image<T> assigned(1, 1, false);
    assigned = a;
    std::cout << std::boolalpha
              << "copy == a: " << (copy == a)
              << ", assigned != a: " << (assigned != a)
              << ", a == b: " << (a == b) << "\n\n";
}

int main()
{
    try
    {
        demonstrate<bool>("bool");
        demonstrate<char>("char");
        demonstrate<float>("float");
        demonstrate<short>("short");

        try
        {
            Image<short> small(2, 2, false);
            small(5, 5) = 1;
        }
        catch (const std::out_of_range &error)
        {
            std::cout << "Caught exception: " << error.what() << "\n\n";
        }

        int rows = 0;
        int cols = 0;
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;
        short fillValue = 0;

        std::cout << "Enter image height and width: ";
        std::cin >> rows >> cols;
        std::cout << "Enter x1 y1 x2 y2 (x - column, y - row, from 0): ";
        std::cin >> x1 >> y1 >> x2 >> y2;
        std::cout << "Enter fill value: ";
        std::cin >> fillValue;

        if (!std::cin || rows <= 0 || cols <= 0)
        {
            std::cout << "Input error\n";
            return 1;
        }

        Image<short> canvas(static_cast<std::size_t>(rows), static_cast<std::size_t>(cols), false);
        drawLine(canvas, x1, y1, x2, y2, fillValue);
        std::cout << "Result:\n"
                  << canvas;
    }
    catch (const std::exception &error)
    {
        std::cout << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
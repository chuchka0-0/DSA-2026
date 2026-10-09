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
class Image
{
private:
    std::size_t _rows;
    std::size_t _cols;
    T *_data;

    static constexpr double kEpsilon = 1e-6;

    void checkIndex(std::size_t row, std::size_t col) const
    {
        if (row >= _rows || col >= _cols)
        {
            throw std::out_of_range("Image index is out of range");
        }
    }

    static T add(T a, T b)
    {
        if constexpr (std::is_same_v<T, bool>)
        {
            return a || b;
        }
        else
        {
            return saturate<T>(static_cast<long double>(a) + static_cast<long double>(b));
        }
    }

    static T multiply(T a, T b)
    {
        if constexpr (std::is_same_v<T, bool>)
        {
            return a && b;
        }
        else
        {
            return saturate<T>(static_cast<long double>(a) * static_cast<long double>(b));
        }
    }

    Image combine(const Image &other, bool isSum) const
    {
        Image result(std::max(_rows, other._rows), std::max(_cols, other._cols), false);
        for (std::size_t i = 0; i < result._rows; ++i)
        {
            for (std::size_t j = 0; j < result._cols; ++j)
            {
                T a = (i < _rows && j < _cols) ? data_[i * _cols + j] : T{};
                T b = (i < other._rows && j < other._cols) ? other._data[i * other._cols + j] : T{};
                result._data[i * result._cols + j] = isSum ? add(a, b) : multiply(a, b);
            }
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
                std::uniform_int_distribution<int> dist(0, 1);
                data_[i] = (dist(generator) == 1);
            }
            else if constexpr (std::is_floating_point_v<T>)
            {
                std::uniform_real_distribution<T> dist(-100, 100);
                data_[i] = dist(generator);
            }
            else
            {
                const int low = std::max(static_cast<int>(std::numeric_limits<T>::lowest()), -100);
                const int high = std::min(static_cast<int>(std::numeric_limits<T>::max()), 100);
                std::uniform_int_distribution<int> dist(low, high);
                data_[i] = static_cast<T>(dist(generator));
            }
        }
    }

    static bool equal(T a, T b)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            return std::fabs(static_cast<double>(a) - static_cast<double>(b)) < kEpsilon;
        }
        else
        {
            return a == b;
        }
    }

public:
    Image(std::size_t rows, std::size_t cols, bool randomFill)
        : rows_(rows), cols_(cols), data_(nullptr)
    {
        if (rows == 0 || cols == 0)
        {
            throw std::invalid_argument("Image size must be positive");
        }
        data_ = new T[rows_ * cols_]();
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

    T &operator()(std::size_t row, std::size_t col)
    {
        checkIndex(row, col);
        return _data[row * _cols + col];
    }

    const T &operator()(std::size_t row, std::size_t col) const
    {
        checkIndex(row, col);
        return _data[row * _cols + col];
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

    Image operator*(const Image &other) const { return combine(other, false); }

    Image operator+(const Image &other) const { return combine(other, true); }

    Image operator*(T constant) const
    {
        Image result(_rows, _cols, false);
        for (std::size_t i = 0; i < _rows * _cols; ++i)
        {
            result._data[i] = multiply(_data[i], constant);
        }
        return result;
    }

    Image operator+(T constant) const
    {
        Image result(_rows, _cols, false);
        for (std::size_t i = 0; i < _rows * _cols; ++i)
        {
            result._data[i] = add(_data[i], constant);
        }
        return result;
    }

    friend Image operator*(T constant, const Image &image) { return image * constant; }
    friend Image operator+(T constant, const Image &image) { return image + constant; }

    Image operator!() const
    {
        Image result(_rows, _cols, false);
        for (std::size_t i = 0; i < _rows * _cols; ++i)
        {
            if constexpr (std::is_same_v<T, bool>)
            {
                result._data[i] = !_data[i];
            }
            else if constexpr (std::is_signed_v<T>)
            {
                result._data[i] = saturate<T>(-static_cast<long double>(_data[i]));
            }
            else
            {
                result._data[i] = static_cast<T>(std::numeric_limits<T>::max() - _data[i]);
            }
        }
        return result;
    }

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

int main()
{
    Image<short> a(2, 3, true);
    Image<short> b(3, 2, true);
    std::cout << a << '\n'
              << b << '\n';
    std::cout << a + b << '\n'
              << a * b;

    std::cout << '\n'
              << a + 10 << '\n'
              << 2 * a;

    std::cout << '\n'
              << !a << "Fill ratio: " << a.fillRatio() << '\n';

    return 0;
}
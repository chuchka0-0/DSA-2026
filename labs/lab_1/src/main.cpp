import std;

template <typename T>
class Image
{
private:
    std::size_t _rows;
    std::size_t _cols;
    T *_data;

    void checkIndex(std::size_t row, std::size_t col) const
    {
        if (row >= _rows || col >= _cols)
        {
            throw std::out_of_range("Image index is out of range");
        }
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
};

int main()
{
    Image<short> image(2, 3);
    image(0, 1) = 5;
    std::cout << image(0, 1) << '\n';
    return 0;
}
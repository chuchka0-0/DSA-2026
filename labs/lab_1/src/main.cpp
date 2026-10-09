import std;

template <typename T>
class Image
{
private:
    std::size_t _rows;
    std::size_t _cols;
    T *_data;

public:
    Image(std::size_t rows, std::size_t cols)
        : rows_(rows), cols_(cols), _data(nullptr)
    {
        if (rows == 0 || cols == 0)
        {
            throw std::invalid_argument("Image size must be positive");
        }
        _data = new T[_rows * _cols]();
    }

    Image(const Image &other)
        : rows_(other._rows), cols_(other._cols), data_(new T[other._rows * other._cols])
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
};

int main()
{
    Image<short> image(2, 3);
    std::cout << "Image size: " << image.rows() << "x" << image.cols() << '\n';
    return 0;
}
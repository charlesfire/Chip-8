#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>

template<typename T, unsigned int c, unsigned int r>
class Matrix
{
    public:
        Matrix() = default;
        virtual ~Matrix() = default;
        std::array<T, r>& operator[](const unsigned int col)
        {
            return cells[col];
        }

        const std::array<T, r>& operator[](const unsigned int col) const
        {
            return cells[col];
        }

        unsigned int size(unsigned int dimension) const
        {
            if (dimension == 0)
                return c;
            else
                return r;
        }

        void fill(T value)
        {
            for (std::array<T, r> subArray : cells)
                subArray.fill(value);
        }

    private:
        std::array<std::array<T, r>, c> cells;
};

#endif // MATRIX_HPP

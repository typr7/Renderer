#ifndef _MATH_UTILS_HPP_
#define _MATH_UTILS_HPP_

#include <eigen3/Eigen/Eigen>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

inline constexpr bool eq(size_t t1, size_t t2) {
    return (t1 == t2);
}

template <typename _T, typename _U>
inline void unpacker(_T& container, _U&& t, int& pos) {
    container[pos++] = t;
}

template <typename _T, size_t _Rows, size_t _Cols,
        bool = eq(_Rows, _Cols),
        bool = std::is_arithmetic<_T>::value>
class Matrix;

template <typename _T, size_t _Rows, size_t _Cols>
class Matrix<_T, _Rows, _Cols, false, true> {
public:
    typedef _T value_type;
    typedef _T* iterator;
    typedef const _T* const_iterator;
    typedef _T& reference;
    typedef const _T& const_reference;

    template <typename _U>
    using IsNumber = typename std::enable_if<std::is_arithmetic<_U>::value, int>::type;

private:
    static constexpr size_t size_ = (_Rows * _Cols);

    _T data_[size_];

public:
    Matrix() = default;
    ~Matrix() = default;

    Matrix(const Matrix<_T, _Rows, _Cols>& other) {
        std::copy(other.begin(), other.end(), data_);
    }

    template<typename ...Args>
    Matrix(_T&& first, Args&& ...args) {
        assert((sizeof...(Args) + 1) == (_Rows * _Cols));
        int cnt = 0;
        unpacker(*this, first, cnt);
        (unpacker(*this, args, cnt), ...);
    }

    static Matrix<_T, _Rows, _Cols> Zero() {
        Matrix<_T, _Rows, _Cols> ret;
        for(int i = 0; i < ret.size(); i++) {
            ret[i] = 0;
        }
        return ret;
    }

public:
    constexpr size_t size() const noexcept { return size_; }

    constexpr _T* data() const noexcept { return data_; }

    constexpr size_t rows() const noexcept { return _Rows; }

    constexpr size_t cols() const noexcept { return _Cols;}

    iterator begin() noexcept { return data_; }

    const_iterator begin() const noexcept { return data_; }

    iterator end() noexcept { return (data_ + size_); }

    const_iterator end() const noexcept { return (data_ + size_); }

    reference at(int row_index, int col_index) noexcept {
        assert((row_index < _Rows) && (col_index < _Cols));
        return data_[row_index * _Cols + col_index];
    }

    const_reference at(int row_index, int col_index) const noexcept {
        assert((row_index < _Rows) && (col_index < _Cols));
        return data_[row_index * _Cols + col_index];
    }

    reference at(int index) noexcept {
        assert(index < size_);
        return data_[index];
    }

    const_reference at(int index) const noexcept {
        assert(index < size_);
        return data_[index];
    }

    reference operator() (size_t row_index, size_t col_index) noexcept {
        return data_[row_index * _Cols + col_index];
    }

    const_reference operator() (size_t row_index, size_t col_index) const noexcept {
        return data_[row_index * _Cols + col_index];
    }

    reference operator[] (size_t index) noexcept {
        return data_[index];
    }

    const_reference operator[] (size_t index) const noexcept {
        return data_[index];
    }

    template<typename _U, size_t _OtherCols, IsNumber<_U> = 0>
    Matrix<_T, _Rows, _OtherCols> operator* 
            (const Matrix<_U, _Cols, _OtherCols>& other_matrix) 
            const noexcept {
        Matrix<_T, _Rows, _OtherCols> ret;

        for(int i = 0; i < _Rows; i++) {
            for(int j = 0; j < _OtherCols; j++) {
                _T t{};
                for(int k = 0; k < _Cols; k++) {
                    t += (*this)(i, k) * other_matrix(k, j);
                }
                ret(i, j) = t;
            }
        }

        return ret;
    }


    template <typename _U, IsNumber<_U> = 0>
    Matrix<_T, _Rows, _Cols> operator* (_U number) const noexcept {
        Matrix<_T, _Rows, _Cols> ret(*this);

        for(int i = 0; i < ret.size(); i++) {
            ret[i] *= number;
        }

        return ret;
    }

    template <typename _U, IsNumber<_U> = 0>
    Matrix<_T, _Rows, _Cols> operator/ (_U number) const noexcept {
        Matrix<_T, _Rows, _Cols> ret(*this);

        for(int i = 0; i < ret.size(); i++) {
            ret[i] /= number;
        }
        
        return ret;
    }

    template <typename _U, IsNumber<_U> = 0>
    Matrix<_T, _Rows, _Cols> operator+ (const Matrix<_U, _Rows, _Cols>& other_matrix) const noexcept {
        Matrix<_T, _Rows, _Cols> ret(*this);

        for(int i = 0; i < ret.size(); i++) {
            ret[i] += other_matrix[i];
        }

        return ret;
    }

    template <typename _U, IsNumber<_U> = 0>
    Matrix<_T, _Rows, _Cols> operator+ (_U number) const noexcept {
        Matrix<_T, _Rows, _Cols> ret(*this);

        for(int i = 0; i < ret.size(); i++) {
            ret[i] += number;
        }

        return ret;
    }

    template <typename _U, IsNumber<_U> = 0>
    Matrix<_T, _Rows, _Cols> operator- (const Matrix<_U, _Rows, _Cols>& other_matrix) const noexcept {
        Matrix<_T, _Rows, _Cols> ret(*this);

        for(int i = 0; i < ret.size(); i++) {
            ret[i] -= other_matrix[i];
        }

        return ret;
    }

    template <typename _U, IsNumber<_U> = 0>
    Matrix<_T, _Rows, _Cols> operator- (_U number) const noexcept {
        Matrix<_T, _Rows, _Cols> ret(*this);

        for(int i = 0; i < ret.size(); i++) {
            ret[i] -= number;
        }
        return ret;
    }

    Matrix<_T, _Cols, _Rows> transpose() const noexcept {
        Matrix<_T, _Cols, _Rows> ret;
        for(int i = 0; i < rows(); i++) {
            for(int j = 0; j < cols(); j++) {
                ret(j, i) = (*this)(i, j);
            }
        }
        return ret;
    }

    template <typename _U, IsNumber<_U> = 0>
    Matrix<_T, _Rows, _Cols> cwiseProduct
            (const Matrix<_U, _Rows, _Cols>& other_matrix) 
            const noexcept {
        Matrix<_T, _Rows, _Cols> ret(*this);

        for(int i = 0; i < ret.size(); i++) {
            ret[i] *= other_matrix[i];
        }

        return ret;
    }

};

template <typename _T, size_t _Rows, size_t _Cols>
class CommaInitializer {
private:
    Matrix<_T, _Rows, _Cols>& target_;
    Matrix<_T, _Rows, _Cols> tmp_;
    int init_count_;
public:
    template<typename _U>
    CommaInitializer(Matrix<_T, _Rows, _Cols>& t, _U val): 
            target_(t), init_count_(1) {
        tmp_.at(0) = val;
    }

    ~CommaInitializer() {
        assert(init_count_ == target_.size());
        std::swap(target_, tmp_);
    }

    template<typename _U>
    CommaInitializer& operator, (_U e) {
        tmp_.at(init_count_) = e;
        init_count_++;
        return *this;
    }
};

template <typename _T, size_t _Rows, size_t _Cols>
inline std::ostream& operator<< (std::ostream& os, const Matrix<_T, _Rows, _Cols>& matrix) {
    for(int i = 0; i < _Rows; i++) {
        if(i != 0) os << "\n";
        for(int j = 0; j < _Cols; j++) {
            os << matrix(i, j) << " ";
        }
    }
    return os;
}

template <typename _T, size_t _Rows, size_t _Cols, typename _U>
inline CommaInitializer<_T, _Rows, _Cols> operator<< (Matrix<_T, _Rows, _Cols>& matrix, _U val) {
    return CommaInitializer<_T, _Rows, _Cols>(matrix, val);
}

template <typename _T, size_t _Rows, size_t _Cols>
class Matrix<_T, _Rows, _Cols, true, true>: public Matrix<_T, _Rows, _Cols, false, true> {
private:
    typedef Matrix<_T, _Rows, _Cols, false, true> Base;
public:
    
    template<typename ...Args>
    Matrix(Args&& ...args): Matrix<_T, _Rows, _Cols, false, true>::Matrix(std::forward<Args>(args)...) {}

    static Matrix<_T, _Rows, _Cols> Identity() {
        Matrix<_T, _Rows, _Cols> ret;

        for(int i = 0; i < _Rows; i++) {
            for(int j = 0; j < _Cols; j++) {
                ret(i, j) = 0;
                if(i == j)  ret(i, j) = 1;
            }
        }
        return ret;
    }
};

template <typename _T, size_t _Size>
class Vector: public Matrix<_T, _Size, 1> {
private:
    typedef Matrix<_T, _Size, 1> Base;

public:
    template <typename ...Args>
    Vector(Args&& ...args): Base::Matrix(std::forward<Args>(args)...) {}

public:

    _T squareNorm() const noexcept {
        _T ret{};
        for(auto it: *this) {
            ret += it * it;
        }
        return ret;
    }

    _T norm() const noexcept {
        return std::sqrt(squareNorm());
    }

    void normalize() noexcept {
        auto n = norm();
        for(auto& it: *this) {
            it /= n;
        }
    }

    Vector<_T, _Size> normalized() const noexcept {
        Vector<_T, _Size> ret(*this);
        auto n = norm();

        for(auto& it: ret) {
            it /= n;
        }

        return ret;
    }

    template <typename _U>
    _T dot(const Vector<_U, _Size>& other_vector) const noexcept {
        _T ret{};
        for(int i = 0; i < Base::size(); i++) {
            ret += (*this)[i] * other_vector[i];
        }
        return ret;
    }
};

typedef Matrix<int, 2, 2> Matrix2i;
typedef Matrix<int, 3, 3> Matrix3i;
typedef Matrix<int, 4, 4> Matrix4i;

typedef Matrix<float, 2, 2> Matrix2f;
typedef Matrix<float, 3, 3> Matrix3f;
typedef Matrix<float, 4, 4> Matrix4f;

typedef Matrix<double, 2, 2> Matrix2d;
typedef Matrix<double, 3, 3> Matrix3d;
typedef Matrix<double, 4, 4> Matrix4d;

typedef Vector<int, 2> Vector2i;
typedef Vector<int, 3> Vector3i;
typedef Vector<int, 4> Vector4i;

typedef Vector<float, 2> Vector2f;
typedef Vector<float, 3> Vector3f;
typedef Vector<float, 4> Vector4f;

typedef Vector<double, 2> Vector2d;
typedef Vector<double, 3> Vector3d;
typedef Vector<double, 4> Vector4d;

#endif // _MATH_UTILS_HPP_
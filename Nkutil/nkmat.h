#ifndef NKMAT_H
#define NKMAT_H

#include <QVector>
#include <cmath>
#include <Nkutil/nkutil.h>

namespace nkmat {

template<typename Num=float>
class Matrix
{
public:
    Matrix(int nRows, int nCols);
    Matrix(const Matrix &mat, int wndTop, int wndLeft, int wndRows, int wndCols);

    Num operator()(int i, int j) const;
    Num &operator()(int i, int j);

    Matrix<Num> operator+(const Matrix<Num> &rhs) const;
    Matrix<Num> operator-(const Matrix<Num> &rhs) const;

    // template<typename LeftModule>
    // Matrix<LeftModule> operator* (Matrix<LeftModule> &rhs);

    int rows() const { return wrows; }
    int cols() const { return wcols; }

    Num frobNorm() const;

protected:
    int nRows, nCols;
    int wleft, wtop, wrows, wcols;
    QVector<Num> data;

    inline int eltIdx(int i, int j) const { return (i + wtop) * nCols + j + wleft; }
};

template<typename Mat1, typename Mat2>
void eliminationForwardNoPivot(Mat1 &A, Mat2 &f);
template<typename Mat1, typename Mat2>
void eliminationBackwardNoPivot(Mat1 &A, Mat2 &f);
template<typename Mat1, typename Mat2>
void solveEliminationNoPivot(Mat1 &A, Mat2 &f);

/*================*
 *                *
 * Implementation *
 *                *
 *================*/

template<typename Mat1, typename Mat2>
void eliminationForwardNoPivot(Mat1 &A, Mat2 &f)
{
    Q_ASSERT(f.cols() == 1);
    Q_ASSERT(A.rows() == f.rows());
    const int maxrank = std::min(A.cols(), A.rows());
    for (int i = 0; i < maxrank - 1; ++i) {
        const auto invDiag = 1./A(i, i);
        Q_ASSERT(!std::isnan(invDiag));
        Q_ASSERT(!std::isinf(invDiag));
        for (int i1 = i + 1; i1 < A.rows(); ++i1) {
            const auto leading = A(i1, i);
            A(i1, i) = 0;
            for (int j = i + 1; j < A.cols(); ++j) {
                A(i1, j) = A(i1, j) - leading*invDiag*A(i, j);
            }
            f(i1, 0) = f(i1, 0) - leading*invDiag*f(i, 0);
        }
    }
}

template<typename Mat1, typename Mat2>
void eliminationBackwardNoPivot(Mat1 &A, Mat2 &f)
{
    Q_ASSERT(f.cols() == 1);
    Q_ASSERT(A.rows() == f.rows());
    for (int i = A.rows() - 1; i > 0; --i) {
        const auto invDiag = 1./A(i, i);
        Q_ASSERT(!std::isnan(invDiag));
        Q_ASSERT(!std::isinf(invDiag));
        for (int i1 = i - 1; i1 >= 0; --i1) {
            const auto last = A(i1, i);
            for (int j = i1 + 1; j < A.cols(); ++j) {
                A(i1, j) = A(i1, j) - invDiag*last*A(i, j);
            }
            f(i1, 0) = f(i1, 0) - invDiag*last*f(i, 0);
        }
    }
}

template<typename Mat1, typename Mat2>
void solveEliminationNoPivot(Mat1 &A, Mat2 &f)
{
    eliminationForwardNoPivot(A, f);
    eliminationBackwardNoPivot(A, f);
    for (int i = 0; i < f.rows(); ++i) {
        const auto invDiag = 1./A(i, i);
        Q_ASSERT(!std::isnan(invDiag));
        Q_ASSERT(!std::isinf(invDiag));
        f(i, 0) *= invDiag;
        A(i, i) *= invDiag;
    }
}

template<typename Num>
Matrix<Num>::Matrix(int m, int n)
    : nRows(m), nCols(n),
      wleft(0), wtop(0),
      wrows(m), wcols(n),
      data(m*n)
{
}

template<typename Num>
Matrix<Num>::Matrix(const Matrix &mat, int wndTop, int wndLeft, int wndRows, int wndCols)
    : data(mat.data),
      nRows(mat.nRows), nCols(mat.nCols),
      wtop(mat.wtop + wndTop), wleft(mat.wleft + wndLeft),
      wrows(wndRows), wcols(wndCols)
{
    Q_ASSERT(mat.wtop + wndTop < nRows);
    Q_ASSERT(mat.wleft + wndLeft < nCols);
    Q_ASSERT(wndCols <= mat.wcols);
    Q_ASSERT(wndRows <= mat.wrows);
}

template<typename Num>
Num Matrix<Num>::operator() (int i, int j) const
{
    const int idx = eltIdx(i, j);
    return data[idx];
}

template<typename Num>
Num &Matrix<Num>::operator() (int i, int j)
{
    const int idx = eltIdx(i, j);
    return data[idx];
}

template<typename Num>
Matrix<Num> Matrix<Num>::operator+(const Matrix<Num> &rhs) const
{
    Q_ASSERT(rows() == rhs.rows() && cols() == rhs.cols());
    Matrix<Num> ret(rows(), cols());
    for (int i = 0; i < rows(); ++i) {
        for (int j = 0; j < cols(); ++j) {
            ret(i, j) = (*this)(i, j) + rhs(i, j);
        }
    }
    return ret;
}

template<typename Num>
Matrix<Num> Matrix<Num>::operator-(const Matrix<Num> &rhs) const
{
    Q_ASSERT(rows() == rhs.rows() && cols() == rhs.cols());
    Matrix<Num> ret(rows(), cols());
    for (int i = 0; i < rows(); ++i) {
        for (int j = 0; j < cols(); ++j) {
            ret(i, j) = (*this)(i, j) - rhs(i, j);
        }
    }
    return ret;
}

template<typename Num>
Num Matrix<Num>::frobNorm() const
{
    Num norm;
    for (int i = 0; i < rows(); ++i) {
        for (int j = 0; j < cols(); ++j) {
            norm += (*this)(i, j)*(*this)(i, j);
        }
    }
    norm = std::sqrt(norm);
    return norm;
}

}
#endif // NKMAT_H

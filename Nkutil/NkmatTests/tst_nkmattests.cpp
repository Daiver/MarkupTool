#include <QString>
#include <QtTest>

#include "Nkutil/nkmat.h"
#include "Nkutil/nkutil.h"

class NkmatTests : public QObject
{
    Q_OBJECT

public:
    NkmatTests();

private Q_SLOTS:
    void testMatrixMul();
    void testMatrixFrobNorm();
    void testElimintationForwardId();
    void testSolveNoPivot();
};

NkmatTests::NkmatTests()
{
}

void NkmatTests::testMatrixMul()
{
    nkmat::Matrix<float> m(3, 3);
    m(1, 1) = 1.f;
    m(2, 2) = 2.f;
    QVERIFY(m(1, 1) == 1.f);
    QVERIFY(m(2, 2) == 2.f);
}

void NkmatTests::testMatrixFrobNorm()
{
    nkmat::Matrix<float> m(3, 3);
    m(0, 0) = 1.f;
    m(1, 1) = 1.f;
    m(2, 2) = 1.f;
    const float norm = m.frobNorm();
    QVERIFY2(nkutil::eq(norm, std::sqrt(3.f)),
             qPrintable(QString(".frobNorm() of an identity 3x3 matrix failed to be sqrt 3: %1")
                        .arg(norm)));
}

void NkmatTests::testElimintationForwardId()
{
    nkmat::Matrix<float> A(3, 3);
    A(0, 0) = A(1, 1) = A(2, 2) = 1.0;
    nkmat::Matrix<float> f(3, 1);
    f(0, 0) = f(1, 0) = f(2, 0) = 1.0;

    auto Acopy = A;
    auto fcopy = f;

    nkmat::eliminationForwardNoPivot(A, f);
    auto diff1 = A - Acopy;
    auto diff2 = f - fcopy;
    QVERIFY2(diff1.frobNorm() < 1e-6,
             qPrintable(
                 QString("Gaussian elimintation's forward step shall not spoil an identity; error = %1\n"
                         "( %2 %3 %4 )\n"
                         "( %5 %6 %7 )\n"
                         "( %8 %9 %10 )\n"
                         )
                 .arg(diff1.frobNorm())
                 .arg(A(0, 0)).arg(A(0, 1)).arg(A(0, 2))
                 .arg(A(1, 0)).arg(A(1, 1)).arg(A(1, 2))
                 .arg(A(2, 0)).arg(A(2, 1)).arg(A(2, 2))
                 ));
    QVERIFY2(diff2.frobNorm() < 1e-6,
             qPrintable(
                 QString("Gaussian elimintation's forward step shall not spoil an identity; error = %1\n"
                         "( %2 ) \n ( %3 ) \n %4 )\n"
                         )
                 .arg(diff1.frobNorm())
                 .arg(f(0, 0))
                 .arg(f(1, 0))
                 .arg(f(2, 0))
                 ));
}

void NkmatTests::testSolveNoPivot()
{
    nkmat::Matrix<float> A(2, 2);
    A(0, 0) = 1; A(0, 1) = -1;
    A(1, 0) = 1; A(1, 1) = 1;
    nkmat::Matrix<float> f(2, 1);
    f(0, 0) = 1;
    f(1, 0) = 5;
    nkmat::Matrix<float> x(2, 1);
    x(0, 0) = 3;
    x(1, 0) = 2;

    nkmat::solveEliminationNoPivot(A, f);
    auto diff = f - x;
    QVERIFY2(std::abs(diff.frobNorm()) < 1e-6,
             qPrintable(
                 QString("solveEliminationNoPivot(); error = %1")
                 .arg(diff.frobNorm())));
}

QTEST_APPLESS_MAIN(NkmatTests)

#include "tst_nkmattests.moc"

#include "spline.h"

bool buildNaturalSpline( int size, double* t, double* p, double* a, double* b, double* c )
{
    int i;
    //  set up tridiagonal equation system; use coefficient
    //  vectors as temporary buffers
    double* h = new double[size-1];
    for (i = 0; i < size - 1; i++)
    {
        h[i] = t[i+1] - t[i];
        if (h[i] <= 0) {
            delete[] h;
            return false;
        }
    }

    double* d = new double[size-1];
    double dy1 = (p[1] - p[0]) / h[0];
    for (i = 1; i < size - 1; i++)
    {
        b[i] = c[i] = h[i];
        a[i] = 2.0 * (h[i-1] + h[i]);

        const double dy2 = (p[i+1] - p[i]) / h[i];
        d[i] = 6.0 * ( dy1 - dy2);
        dy1 = dy2;
    }

    //
    // solve it
    //

    // L-U Factorization
    for(i = 1; i < size - 2;i++)
    {
        c[i] /= a[i];
        a[i+1] -= b[i] * c[i];
    }

    // forward elimination
    double* s = new double[size];
    s[1] = d[1];
    for ( i = 2; i < size - 1; i++)
        s[i] = d[i] - c[i-1] * s[i-1];

    // backward elimination
    s[size - 2] = - s[size - 2] / a[size - 2];
    for (i = size -3; i > 0; i--)
        s[i] = - (s[i] + b[i] * s[i+1]) / a[i];
    s[size - 1] = s[0] = 0.0;

    //
    // Finally, determine the spline coefficients
    //
    for (i = 0; i < size - 1; i++)
    {
        a[i] = ( s[i+1] - s[i] ) / ( 6.0 * h[i]);
        b[i] = 0.5 * s[i];
        c[i] = ( p[i+1] - p[i] ) / h[i]
        - (s[i+1] + 2.0 * s[i] ) * h[i] / 6.0;
    }

    delete[] s;
    delete[] d;
    delete[] h;
    return true;
}



bool buildPeriodicSpline( int size, double* t, double* p, double* a, double* b, double* c )
{
    int i;

    //
    //  setup equation system; use coefficient
    //  vectors as temporary buffers
    //
    double* h = new double[size-1];
    for (i = 0; i < size - 1; i++)
    {
        h[i] = t[i+1] - t[i];
        if (h[i] <= 0) {
            delete[] h;
            return false;
        }
    }

    double* d = new double[size-1];
    const int imax = size - 2;
    double htmp = h[imax];
    double dy1 = (p[0] - p[imax]) / htmp;
    for (i = 0; i <= imax; i++)
    {
        b[i] = c[i] = h[i];
        a[i] = 2.0 * (htmp + h[i]);
        const double dy2 = (p[i+1] - p[i]) / h[i];
        d[i] = 6.0 * ( dy1 - dy2);
        dy1 = dy2;
        htmp = h[i];
    }

    //
    // solve it
    //

    // L-U Factorization
    a[0] = sqrt(a[0]);
    c[0] = h[imax] / a[0];
    double sum = 0;

    for( i = 0; i < imax - 1; i++)
    {
        b[i] /= a[i];
        if (i > 0)
            c[i] = - c[i-1] * b[i-1] / a[i];
        a[i+1] = sqrt( a[i+1] - b[i]*b[i] );
        sum += c[i]*c[i];
    }
    b[imax-1] = (b[imax-1] - c[imax-2] * b[imax-2]) / a[imax-1];
    a[imax] = sqrt(a[imax] - b[imax-1]*b[imax-1] - sum);


    // forward elimination
    double* s = new double[size];
    s[0] = d[0] / a[0];
    sum = 0;
    for( i = 1; i < imax; i++)
    {
        s[i] = (d[i] - b[i-1] * s[i-1]) / a[i];
        sum += c[i-1] * s[i-1];
    }
    s[imax] = (d[imax] - b[imax-1] * s[imax-1] - sum) / a[imax];


    // backward elimination
    s[imax] = - s[imax] / a[imax];
    s[imax-1] = -(s[imax-1] + b[imax-1] * s[imax]) / a[imax-1];
    for (i= imax - 2; i >= 0; i--)
        s[i] = - (s[i] + b[i] * s[i+1] + c[i] * s[imax]) / a[i];

    //
    // Finally, determine the spline coefficients
    //
    s[size-1] = s[0];
    for ( i=0; i < size-1; i++)
    {
        a[i] = ( s[i+1] - s[i] ) / ( 6.0 * h[i]);
        b[i] = 0.5 * s[i];
        c[i] = ( p[i+1] - p[i] )
            / h[i] - (s[i+1] + 2.0 * s[i] ) * h[i] / 6.0;
    }

    delete[] s;
    delete[] d;
    delete[] h;
    return true;
}



QPainterPath Spline::build(const QPolygonF &points)
{
    QPainterPath path;

    int i;
    int n = points.size();
    if ( n < 2 )
        return path;

    double* t = new double[n];

    double* px = new double[n];
    double* ax = new double[n];
    double* bx = new double[n];
    double* cx = new double[n];

    double* py = new double[n];
    double* ay = new double[n];
    double* by = new double[n];
    double* cy = new double[n];

    const QPointF* data = points.data();
    for (i=0; i < n; i++) {
        t[i] = i;
        px[i] = data[i].x();
        py[i] = data[i].y();
    }

    if (points.isClosed()) {
        buildPeriodicSpline( n, t, px, ax, bx, cx );
        buildPeriodicSpline( n, t, py, ay, by, cy );
    } else {
        buildNaturalSpline( n, t, px, ax, bx, cx );
        buildNaturalSpline( n, t, py, ay, by, cy );
    }

    path.moveTo( points[0] );
    for (i=0; i < n - 1; i++) {
        QPointF c1 = QPointF(px[i] + cx[i] / 3, py[i] + cy[i] / 3);
        QPointF c2 = QPointF(px[i] + cx[i] * 2/3 + bx[i] / 3, py[i] + cy[i] * 2/3 + by[i] / 3);
        path.cubicTo( c1, c2, points[i+1] );
    }

    delete[] cy;
    delete[] by;
    delete[] ay;
    delete[] py;

    delete[] cx;
    delete[] bx;
    delete[] ax;
    delete[] px;

    delete[] t;
    return path;
}

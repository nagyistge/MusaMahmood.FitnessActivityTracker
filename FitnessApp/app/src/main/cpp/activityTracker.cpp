//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: activityTracker.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 08-Apr-2017 10:04:52
//
/**
 *
 * NOTE: Much of the code below was generated using MATLAB Coder v. 3.1
 * The JNI functions were implemented by Musa Mahmood, and the original MATLAB source code was
 * written by Musa Mahmood. Training data were contributed by friends and family.
 *
 * The main function 'activityTracker' takes roughly 6 seconds of data sampled at 16.6 Hz (100 data
 * points) and tests them against training data using a k-Nearest Neighbor algorithm, and returns a
 * vector 'Y' with 5 data points corresponding with a match for each second.
 *
 * The trained data uses the following classes:
 * - 1 = Idle (Sitting or Standing)
 * - 2 = Transitioning (not fully classifiable data)
 * - 3 = Walking
 * - 4 = Running
 * - 5 = Free-fall
 * - 6 = Jumping
 *
 * As of 4/08 the accuracy of the classifier is about 80%, and requires fine-tuning.
 */
// Include Files
#include "rt_nonfinite.h"
#include "activityTracker.h"

// Type Definitions
#ifndef struct_emxArray__common
#define struct_emxArray__common

struct emxArray__common
{
    void *data;
    int *size;
    int allocatedSize;
    int numDimensions;
    boolean_T canFreeData;
};

#endif                                 //struct_emxArray__common

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
    double *data;
    int *size;
    int allocatedSize;
    int numDimensions;
    boolean_T canFreeData;
};

#endif                                 //struct_emxArray_real_T


// Function Declarations
static void Fintegrate(const double X[100], double Y[5]);
static void Fmax(const double X[100], double Y[5]);
static void Fmean(const double X[100], double Y[5]);
static void Fmin(const double X[100], double Y[5]);
static void Fpca(const double X[100], emxArray_real_T *Y);
static void Fstddev(const double X[100], double Y[5]);
static void b_localSVD(emxArray_real_T *x, int DOF, const emxArray_real_T
*Weights, emxArray_real_T *coeffOut, emxArray_real_T *scoreOut,
                       emxArray_real_T *latentOut, emxArray_real_T *tsquared, emxArray_real_T
                       *explained);
static void b_sqrt(emxArray_real_T *x);
static void b_sum(const boolean_T x[100], double y[5]);
static void b_xaxpy(int n, double a, const double x[100], int ix0, double y[5],
                    int iy0);
static double b_xdotc(int n, const double x[25], int ix0, const double y[25],
                      int iy0);
static double b_xnrm2(int n, const double x[20], int ix0);
static void b_xrot(double x[25], int ix0, int iy0, double c, double s);
static void b_xscal(double a, double x[400], int ix0);
static void b_xswap(double x[25], int ix0, int iy0);
static void c_xaxpy(int n, double a, const double x[5], int ix0, double y[100],
                    int iy0);
static double c_xdotc(int n, const double x[400], int ix0, const double y[400],
                      int iy0);
static double c_xnrm2(int n, const emxArray_real_T *x, int ix0);
static void c_xrot(int n, emxArray_real_T *x, int ix0, int iy0, double c, double
s);
static void c_xscal(int n, double a, emxArray_real_T *x, int ix0);
static void c_xswap(int n, emxArray_real_T *x, int ix0, int iy0);
static void d_xaxpy(int n, double a, int ix0, double y[25], int iy0);
static double d_xdotc(int n, const emxArray_real_T *x, int ix0, const
emxArray_real_T *y, int iy0);
static void e_xaxpy(int n, double a, int ix0, double y[400], int iy0);
static double e_xdotc(int n, const emxArray_real_T *x, int ix0, const
emxArray_real_T *y, int iy0);
static void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
elementSize);
static void emxFree_real_T(emxArray_real_T **pEmxArray);
static void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
static void emxInit_real_T1(emxArray_real_T **pEmxArray, int numDimensions);
static double eps(double x);
static void f_xaxpy(int n, double a, int ix0, emxArray_real_T *y, int iy0);
static void g_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
                    emxArray_real_T *y, int iy0);
static void h_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
                    emxArray_real_T *y, int iy0);
static void i_xaxpy(int n, double a, int ix0, emxArray_real_T *y, int iy0);
static void knnclassify(const emxArray_real_T *testSamples, double Y[5]);
static void localSVD(const double x[100], int DOF, emxArray_real_T *coeffOut,
                     emxArray_real_T *scoreOut, emxArray_real_T *latentOut,
                     double tsquared[5], emxArray_real_T *explained);
static void merge(int idx[500], double x[500], int offset, int np, int nq, int
iwork[500], double xwork[500]);
static void merge_block(int idx[500], double x[500], int offset, int n, int
preSortLevel, int iwork[500], double xwork[500]);
static void merge_pow2_block(int idx[500], double x[500], int offset);
static void sort(double x[500], int idx[500]);
static double sum(const emxArray_real_T *x);
static void xaxpy(int n, double a, int ix0, double y[100], int iy0);
static double xdotc(int n, const double x[100], int ix0, const double y[100],
                    int iy0);
static double xnrm2(int n, const double x[100], int ix0);
static void xrot(double x[400], int ix0, int iy0, double c, double s);
static void xrotg(double *a, double *b, double *c, double *s);
static void xscal(double a, double x[25], int ix0);
static void xswap(double x[400], int ix0, int iy0);

// Function Definitions

//
// Arguments    : const double X[100]
//                double Y[5]
// Return Type  : void
//
static void Fintegrate(const double X[100], double Y[5])
{
    int iz;
    int iystart;
    int j;
    double s;
    int iy;
    double ylast;
    int k;
    iz = -1;
    iystart = -1;
    for (j = 0; j < 5; j++) {
        iystart++;
        s = 0.0;
        iy = iystart;
        ylast = X[iystart];
        for (k = 0; k < 19; k++) {
            iy += 5;
            s += (ylast + X[iy]) / 2.0;
            ylast = X[iy];
        }
        iz++;
        Y[iz] = s;
    }
}

//
// Arguments    : const double X[100]
//                double Y[5]
// Return Type  : void
//
static void Fmax(const double X[100], double Y[5])
{
    int j;
    int ixstart;
    double mtmp;
    int ix;
    boolean_T exitg1;
    for (j = 0; j < 5; j++) {
        ixstart = j + 1;
        mtmp = X[j];
        if (rtIsNaN(X[j])) {
            ix = j + 6;
            exitg1 = false;
            while ((!exitg1) && (ix <= j + 96)) {
                ixstart = ix;
                if (!rtIsNaN(X[ix - 1])) {
                    mtmp = X[ix - 1];
                    exitg1 = true;
                } else {
                    ix += 5;
                }
            }
        }

        if (ixstart < j + 96) {
            for (ix = ixstart + 4; ix + 1 <= j + 96; ix += 5) {
                if (X[ix] > mtmp) {
                    mtmp = X[ix];
                }
            }
        }

        Y[j] = mtmp;
    }
}

//
// Arguments    : const double X[100]
//                double Y[5]
// Return Type  : void
//
static void Fmean(const double X[100], double Y[5])
{
    int j;
    double s;
    int k;
    for (j = 0; j < 5; j++) {
        s = X[j];
        for (k = 0; k < 19; k++) {
            s += X[j + (k + 1) * 5];
        }

        Y[j] = s;
        Y[j] /= 20.0;
    }
}

//
// Arguments    : const double X[100]
//                double Y[5]
// Return Type  : void
//
static void Fmin(const double X[100], double Y[5])
{
    int j;
    int ixstart;
    double mtmp;
    int ix;
    boolean_T exitg1;
    for (j = 0; j < 5; j++) {
        ixstart = j + 1;
        mtmp = X[j];
        if (rtIsNaN(X[j])) {
            ix = j + 6;
            exitg1 = false;
            while ((!exitg1) && (ix <= j + 96)) {
                ixstart = ix;
                if (!rtIsNaN(X[ix - 1])) {
                    mtmp = X[ix - 1];
                    exitg1 = true;
                } else {
                    ix += 5;
                }
            }
        }

        if (ixstart < j + 96) {
            for (ix = ixstart + 4; ix + 1 <= j + 96; ix += 5) {
                if (X[ix] < mtmp) {
                    mtmp = X[ix];
                }
            }
        }
        Y[j] = mtmp;
    }
}

//
// Arguments    : const double X[100]
//                emxArray_real_T *Y
// Return Type  : void
//
static void Fpca(const double X[100], emxArray_real_T *Y)
{
    double x[100];
    int p;
    int naninfo_nRowsWithNaNs;
    int naninfo_nNaNsInRow[5];
    int i;
    boolean_T naninfo_isNaN[100];
    int j;
    boolean_T noNaNs;
    int irow;
    int DOF;
    double mu[20];
    double wcol;
    double xcol;
    emxArray_real_T *coeff;
    emxArray_real_T *score;
    emxArray_real_T *latent;
    emxArray_real_T *explained;
    emxArray_real_T *xNoNaNs;
    double tsquared[5];
    emxArray_real_T *WNoNaNs;
    emxArray_real_T *varargout_1;
    static const signed char iv0[2] = { 20, 1 };

    emxArray_real_T *b_tsquared;
    emxArray_real_T *b_x;
    static const signed char iv1[2] = { 5, 1 };

    double absc;
    memcpy(&x[0], &X[0], 100U * sizeof(double));
    p = 0;
    naninfo_nRowsWithNaNs = 0;
    for (i = 0; i < 5; i++) {
        naninfo_nNaNsInRow[i] = 0;
    }

    for (i = 0; i < 100; i++) {
        naninfo_isNaN[i] = rtIsNaN(X[i]);
    }

    for (j = 0; j < 20; j++) {
        for (i = 0; i < 5; i++) {
            irow = naninfo_nNaNsInRow[i];
            if (naninfo_isNaN[i + 5 * j]) {
                irow = naninfo_nNaNsInRow[i] + 1;
                p++;
            }

            naninfo_nNaNsInRow[i] = irow;
        }
    }

    for (i = 0; i < 5; i++) {
        if (naninfo_nNaNsInRow[i] > 0) {
            naninfo_nRowsWithNaNs++;
        }
    }

    noNaNs = !(p > 0);
    DOF = 5 - naninfo_nRowsWithNaNs;
    if (5 - naninfo_nRowsWithNaNs >= 1) {
        DOF = 4 - naninfo_nRowsWithNaNs;
    }

    if (!noNaNs) {
        for (j = 0; j < 20; j++) {
            wcol = 0.0;
            xcol = 0.0;
            for (i = 0; i < 5; i++) {
                if (!rtIsNaN(X[i + 5 * j])) {
                    wcol++;
                    xcol += X[i + 5 * j];
                }
            }

            mu[j] = xcol / wcol;
        }
    } else {
        for (j = 0; j < 20; j++) {
            wcol = 0.0;
            xcol = 0.0;
            for (i = 0; i < 5; i++) {
                wcol++;
                xcol += X[i + 5 * j];
            }

            mu[j] = xcol / wcol;
        }
    }

    for (j = 0; j < 20; j++) {
        for (i = 0; i < 5; i++) {
            x[i + 5 * j] -= mu[j];
        }
    }

    emxInit_real_T(&coeff, 2);
    emxInit_real_T(&score, 2);
    emxInit_real_T1(&latent, 1);
    emxInit_real_T1(&explained, 1);
    if (noNaNs) {
        localSVD(x, DOF, coeff, score, latent, tsquared, explained);
    } else {
        emxInit_real_T(&xNoNaNs, 2);
        emxInit_real_T1(&WNoNaNs, 1);
        i = xNoNaNs->size[0] * xNoNaNs->size[1];
        xNoNaNs->size[0] = 5 - naninfo_nRowsWithNaNs;
        xNoNaNs->size[1] = 20;
        emxEnsureCapacity((emxArray__common *)xNoNaNs, i, (int)sizeof(double));
        i = WNoNaNs->size[0];
        WNoNaNs->size[0] = 5 - naninfo_nRowsWithNaNs;
        emxEnsureCapacity((emxArray__common *)WNoNaNs, i, (int)sizeof(double));
        irow = -1;
        for (i = 0; i < 5; i++) {
            if (naninfo_nNaNsInRow[i] == 0) {
                irow++;
                for (j = 0; j < 20; j++) {
                    xNoNaNs->data[irow + xNoNaNs->size[0] * j] = x[i + 5 * j];
                }

                WNoNaNs->data[irow] = 1.0;
            }
        }

        emxInit_real_T(&b_tsquared, 2);
        emxInit_real_T(&b_x, 2);
        b_localSVD(xNoNaNs, DOF, WNoNaNs, coeff, b_x, latent, b_tsquared, explained);
        p = b_x->size[1];
        i = score->size[0] * score->size[1];
        score->size[0] = 5;
        score->size[1] = b_x->size[1];
        emxEnsureCapacity((emxArray__common *)score, i, (int)sizeof(double));
        irow = -1;
        emxFree_real_T(&b_tsquared);
        emxFree_real_T(&WNoNaNs);
        emxFree_real_T(&xNoNaNs);
        for (i = 0; i < 5; i++) {
            if (naninfo_nNaNsInRow[i] > 0) {
                for (j = 1; j <= p; j++) {
                    score->data[i + score->size[0] * (j - 1)] = rtNaN;
                }
            } else {
                irow++;
                for (j = 0; j + 1 <= p; j++) {
                    score->data[i + score->size[0] * j] = b_x->data[irow + b_x->size[0] *
                                                                           j];
                }
            }
        }

        emxFree_real_T(&b_x);
    }

    emxFree_real_T(&explained);
    emxFree_real_T(&latent);
    emxInit_real_T(&varargout_1, 2);
    if (1 < DOF) {
        for (i = 0; i < 2; i++) {
            irow = varargout_1->size[0] * varargout_1->size[1];
            varargout_1->size[i] = iv0[i];
            emxEnsureCapacity((emxArray__common *)varargout_1, irow, (int)sizeof
                    (double));
        }

        for (i = 0; i < 20; i++) {
            varargout_1->data[i] = coeff->data[i];
        }

        for (i = 0; i < 2; i++) {
            irow = Y->size[0] * Y->size[1];
            Y->size[i] = iv1[i];
            emxEnsureCapacity((emxArray__common *)Y, irow, (int)sizeof(double));
        }

        for (i = 0; i < 5; i++) {
            Y->data[i] = score->data[i];
        }
    } else {
        i = varargout_1->size[0] * varargout_1->size[1];
        varargout_1->size[0] = 20;
        varargout_1->size[1] = coeff->size[1];
        emxEnsureCapacity((emxArray__common *)varargout_1, i, (int)sizeof(double));
        irow = coeff->size[0] * coeff->size[1];
        for (i = 0; i < irow; i++) {
            varargout_1->data[i] = coeff->data[i];
        }

        i = Y->size[0] * Y->size[1];
        Y->size[0] = 5;
        Y->size[1] = score->size[1];
        emxEnsureCapacity((emxArray__common *)Y, i, (int)sizeof(double));
        irow = score->size[0] * score->size[1];
        for (i = 0; i < irow; i++) {
            Y->data[i] = score->data[i];
        }
    }

    emxFree_real_T(&score);
    emxFree_real_T(&coeff);
    irow = varargout_1->size[1];
    for (j = 0; j + 1 <= irow; j++) {
        wcol = 0.0;
        xcol = 1.0;
        for (i = 0; i < 20; i++) {
            absc = std::abs(varargout_1->data[i + varargout_1->size[0] * j]);
            if (absc > wcol) {
                wcol = absc;
                if (varargout_1->data[i + varargout_1->size[0] * j] < 0.0) {
                    xcol = -1.0;
                } else if (varargout_1->data[i + varargout_1->size[0] * j] > 0.0) {
                    xcol = 1.0;
                } else if (varargout_1->data[i + varargout_1->size[0] * j] == 0.0) {
                    xcol = 0.0;
                } else {
                    xcol = varargout_1->data[i + varargout_1->size[0] * j];
                }
            }
        }

        if (xcol < 0.0) {
            for (i = 0; i < 20; i++) {
                varargout_1->data[i + varargout_1->size[0] * j] = -varargout_1->data[i +
                                                                                     varargout_1->size[0] * j];
            }

            for (i = 0; i < 5; i++) {
                Y->data[i + Y->size[0] * j] = -Y->data[i + Y->size[0] * j];
            }
        }
    }

    emxFree_real_T(&varargout_1);
}

//
// Arguments    : const double X[100]
//                double Y[5]
// Return Type  : void
//
static void Fstddev(const double X[100], double Y[5])
{
    int j;
    int ix;
    double xbar;
    int k;
    double r;
    double y;
    for (j = 0; j < 5; j++) {
        ix = j;
        xbar = X[j];
        for (k = 0; k < 19; k++) {
            ix += 5;
            xbar += X[ix];
        }

        xbar /= 20.0;
        ix = j;
        r = X[j] - xbar;
        y = r * r;
        for (k = 0; k < 19; k++) {
            ix += 5;
            r = X[ix] - xbar;
            y += r * r;
        }

        y /= 19.0;
        Y[j] = y;
        Y[j] = std::sqrt(Y[j]);
    }
}

//
// Arguments    : emxArray_real_T *x
//                int DOF
//                const emxArray_real_T *Weights
//                emxArray_real_T *coeffOut
//                emxArray_real_T *scoreOut
//                emxArray_real_T *latentOut
//                emxArray_real_T *tsquared
//                emxArray_real_T *explained
// Return Type  : void
//
static void b_localSVD(emxArray_real_T *x, int DOF, const emxArray_real_T
*Weights, emxArray_real_T *coeffOut, emxArray_real_T *scoreOut,
                       emxArray_real_T *latentOut, emxArray_real_T *tsquared, emxArray_real_T
                       *explained)
{
    emxArray_real_T *OmegaSqrt;
    int m;
    int ns;
    int nrows;
    int nct;
    emxArray_real_T *A;
    int iter;
    int n;
    int minnp;
    emxArray_real_T *s;
    double e[20];
    emxArray_real_T *work;
    emxArray_real_T *U;
    double Vf[400];
    int nrt;
    int qp1jj;
    int q;
    emxArray_real_T *coeff;
    int qq;
    int mm;
    boolean_T apply_transform;
    double ztest0;
    emxArray_real_T *score;
    double ztest;
    double snorm;
    emxArray_real_T *standScores;
    boolean_T exitg3;
    boolean_T exitg2;
    double f;
    double varargin_1[5];
    double mtmp;
    boolean_T exitg1;
    double sqds;
    double b;
    emxInit_real_T1(&OmegaSqrt, 1);
    m = OmegaSqrt->size[0];
    OmegaSqrt->size[0] = Weights->size[0];
    emxEnsureCapacity((emxArray__common *)OmegaSqrt, m, (int)sizeof(double));
    ns = Weights->size[0];
    for (m = 0; m < ns; m++) {
        OmegaSqrt->data[m] = Weights->data[m];
    }

    b_sqrt(OmegaSqrt);
    nrows = x->size[0];
    for (nct = 0; nct < 20; nct++) {
        for (iter = 0; iter + 1 <= nrows; iter++) {
            x->data[iter + x->size[0] * nct] *= OmegaSqrt->data[iter];
        }
    }

    emxInit_real_T(&A, 2);
    m = A->size[0] * A->size[1];
    A->size[0] = x->size[0];
    A->size[1] = 20;
    emxEnsureCapacity((emxArray__common *)A, m, (int)sizeof(double));
    ns = x->size[0] * x->size[1];
    for (m = 0; m < ns; m++) {
        A->data[m] = x->data[m];
    }

    n = x->size[0];
    if (x->size[0] + 1 <= 20) {
        ns = x->size[0] + 1;
    } else {
        ns = 20;
    }

    if (x->size[0] <= 20) {
        minnp = x->size[0];
    } else {
        minnp = 20;
    }

    emxInit_real_T1(&s, 1);
    m = s->size[0];
    s->size[0] = ns;
    emxEnsureCapacity((emxArray__common *)s, m, (int)sizeof(double));
    for (m = 0; m < ns; m++) {
        s->data[m] = 0.0;
    }

    memset(&e[0], 0, 20U * sizeof(double));
    emxInit_real_T1(&work, 1);
    ns = x->size[0];
    m = work->size[0];
    work->size[0] = ns;
    emxEnsureCapacity((emxArray__common *)work, m, (int)sizeof(double));
    for (m = 0; m < ns; m++) {
        work->data[m] = 0.0;
    }

    emxInit_real_T(&U, 2);
    ns = x->size[0];
    m = U->size[0] * U->size[1];
    U->size[0] = ns;
    U->size[1] = minnp;
    emxEnsureCapacity((emxArray__common *)U, m, (int)sizeof(double));
    ns *= minnp;
    for (m = 0; m < ns; m++) {
        U->data[m] = 0.0;
    }

    memset(&Vf[0], 0, 400U * sizeof(double));
    if (x->size[0] == 0) {
        for (ns = 0; ns < 20; ns++) {
            Vf[ns + 20 * ns] = 1.0;
        }
    } else {
        if (18 <= x->size[0]) {
            nrt = 18;
        } else {
            nrt = x->size[0];
        }

        if (x->size[0] > 1) {
            ns = x->size[0] - 1;
        } else {
            ns = 0;
        }

        if (ns <= 20) {
            nct = ns;
        } else {
            nct = 20;
        }

        if (nct >= nrt) {
            m = nct;
        } else {
            m = nrt;
        }

        for (q = 0; q + 1 <= m; q++) {
            qq = q + n * q;
            mm = n - q;
            apply_transform = false;
            if (q + 1 <= nct) {
                ztest0 = c_xnrm2(mm, A, qq + 1);
                if (ztest0 > 0.0) {
                    apply_transform = true;
                    if (A->data[qq] < 0.0) {
                        ztest0 = -ztest0;
                    }

                    s->data[q] = ztest0;
                    if (std::abs(s->data[q]) >= 1.0020841800044864E-292) {
                        ztest0 = 1.0 / s->data[q];
                        ns = qq + mm;
                        for (qp1jj = qq; qp1jj + 1 <= ns; qp1jj++) {
                            A->data[qp1jj] *= ztest0;
                        }
                    } else {
                        ns = qq + mm;
                        for (qp1jj = qq; qp1jj + 1 <= ns; qp1jj++) {
                            A->data[qp1jj] /= s->data[q];
                        }
                    }

                    A->data[qq]++;
                    s->data[q] = -s->data[q];
                } else {
                    s->data[q] = 0.0;
                }
            }

            for (iter = q + 1; iter + 1 < 21; iter++) {
                ns = q + n * iter;
                if (apply_transform) {
                    ztest0 = d_xdotc(mm, A, qq + 1, A, ns + 1);
                    ztest0 = -(ztest0 / A->data[q + A->size[0] * q]);
                    f_xaxpy(mm, ztest0, qq + 1, A, ns + 1);
                }

                e[iter] = A->data[ns];
            }

            if (q + 1 <= nct) {
                for (ns = q; ns + 1 <= n; ns++) {
                    U->data[ns + U->size[0] * q] = A->data[ns + A->size[0] * q];
                }
            }

            if (q + 1 <= nrt) {
                ztest0 = b_xnrm2(19 - q, e, q + 2);
                if (ztest0 == 0.0) {
                    e[q] = 0.0;
                } else {
                    if (e[q + 1] < 0.0) {
                        e[q] = -ztest0;
                    } else {
                        e[q] = ztest0;
                    }

                    ztest0 = e[q];
                    if (std::abs(e[q]) >= 1.0020841800044864E-292) {
                        ztest0 = 1.0 / e[q];
                        for (qp1jj = q + 1; qp1jj + 1 < 21; qp1jj++) {
                            e[qp1jj] *= ztest0;
                        }
                    } else {
                        for (qp1jj = q + 1; qp1jj + 1 < 21; qp1jj++) {
                            e[qp1jj] /= ztest0;
                        }
                    }

                    e[q + 1]++;
                    e[q] = -e[q];
                    if (q + 2 <= n) {
                        for (ns = q + 1; ns + 1 <= n; ns++) {
                            work->data[ns] = 0.0;
                        }

                        for (iter = q + 1; iter + 1 < 21; iter++) {
                            g_xaxpy(mm - 1, e[iter], A, (q + n * iter) + 2, work, q + 2);
                        }

                        for (iter = q + 1; iter + 1 < 21; iter++) {
                            h_xaxpy(mm - 1, -e[iter] / e[q + 1], work, q + 2, A, (q + n * iter)
                                                                                 + 2);
                        }
                    }
                }

                for (ns = q + 1; ns + 1 < 21; ns++) {
                    Vf[ns + 20 * q] = e[ns];
                }
            }
        }

        if (20 <= x->size[0] + 1) {
            m = 20;
        } else {
            m = x->size[0] + 1;
        }

        if (nct < 20) {
            s->data[nct] = A->data[nct + A->size[0] * nct];
        }

        if (x->size[0] < m) {
            s->data[m - 1] = 0.0;
        }

        if (nrt + 1 < m) {
            e[nrt] = A->data[nrt + A->size[0] * (m - 1)];
        }

        e[m - 1] = 0.0;
        if (nct + 1 <= minnp) {
            for (iter = nct; iter + 1 <= minnp; iter++) {
                for (ns = 1; ns <= n; ns++) {
                    U->data[(ns + U->size[0] * iter) - 1] = 0.0;
                }

                U->data[iter + U->size[0] * iter] = 1.0;
            }
        }

        for (q = nct - 1; q + 1 > 0; q--) {
            mm = n - q;
            qq = q + n * q;
            if (s->data[q] != 0.0) {
                for (iter = q + 1; iter + 1 <= minnp; iter++) {
                    ns = (q + n * iter) + 1;
                    ztest0 = e_xdotc(mm, U, qq + 1, U, ns);
                    ztest0 = -(ztest0 / U->data[qq]);
                    i_xaxpy(mm, ztest0, qq + 1, U, ns);
                }

                for (ns = q; ns + 1 <= n; ns++) {
                    U->data[ns + U->size[0] * q] = -U->data[ns + U->size[0] * q];
                }

                U->data[qq]++;
                for (ns = 1; ns <= q; ns++) {
                    U->data[(ns + U->size[0] * q) - 1] = 0.0;
                }
            } else {
                for (ns = 1; ns <= n; ns++) {
                    U->data[(ns + U->size[0] * q) - 1] = 0.0;
                }

                U->data[qq] = 1.0;
            }
        }

        for (q = 19; q >= 0; q += -1) {
            if ((q + 1 <= nrt) && (e[q] != 0.0)) {
                ns = (q + 20 * q) + 2;
                for (iter = q + 1; iter + 1 < 21; iter++) {
                    qp1jj = (q + 20 * iter) + 2;
                    e_xaxpy(19 - q, -(c_xdotc(19 - q, Vf, ns, Vf, qp1jj) / Vf[ns - 1]), ns,
                            Vf, qp1jj);
                }
            }

            memset(&Vf[q * 20], 0, 20U * sizeof(double));
            Vf[q + 20 * q] = 1.0;
        }

        for (q = 0; q + 1 <= m; q++) {
            if (s->data[q] != 0.0) {
                ztest = std::abs(s->data[q]);
                ztest0 = s->data[q] / ztest;
                s->data[q] = ztest;
                if (q + 1 < m) {
                    e[q] /= ztest0;
                }

                if (q + 1 <= n) {
                    c_xscal(n, ztest0, U, 1 + n * q);
                }
            }

            if ((q + 1 < m) && (e[q] != 0.0)) {
                ztest = std::abs(e[q]);
                ztest0 = ztest / e[q];
                e[q] = ztest;
                s->data[q + 1] *= ztest0;
                b_xscal(ztest0, Vf, 1 + 20 * (q + 1));
            }
        }

        mm = m;
        iter = 0;
        snorm = 0.0;
        for (ns = 0; ns + 1 <= m; ns++) {
            ztest0 = std::abs(s->data[ns]);
            ztest = std::abs(e[ns]);
            if ((ztest0 >= ztest) || rtIsNaN(ztest)) {
            } else {
                ztest0 = ztest;
            }

            if ((snorm >= ztest0) || rtIsNaN(ztest0)) {
            } else {
                snorm = ztest0;
            }
        }

        while ((m > 0) && (!(iter >= 75))) {
            q = m - 1;
            exitg3 = false;
            while (!(exitg3 || (q == 0))) {
                ztest0 = std::abs(e[q - 1]);
                if ((ztest0 <= 2.2204460492503131E-16 * (std::abs(s->data[q - 1]) + std::
                abs(s->data[q]))) || (ztest0 <= 1.0020841800044864E-292) || ((iter
                                                                              > 20) && (ztest0 <= 2.2204460492503131E-16 * snorm))) {
                    e[q - 1] = 0.0;
                    exitg3 = true;
                } else {
                    q--;
                }
            }

            if (q == m - 1) {
                ns = 4;
            } else {
                nct = m;
                ns = m;
                exitg2 = false;
                while ((!exitg2) && (ns >= q)) {
                    nct = ns;
                    if (ns == q) {
                        exitg2 = true;
                    } else {
                        ztest0 = 0.0;
                        if (ns < m) {
                            ztest0 = std::abs(e[ns - 1]);
                        }

                        if (ns > q + 1) {
                            ztest0 += std::abs(e[ns - 2]);
                        }

                        ztest = std::abs(s->data[ns - 1]);
                        if ((ztest <= 2.2204460492503131E-16 * ztest0) || (ztest <=
                                                                           1.0020841800044864E-292)) {
                            s->data[ns - 1] = 0.0;
                            exitg2 = true;
                        } else {
                            ns--;
                        }
                    }
                }

                if (nct == q) {
                    ns = 3;
                } else if (nct == m) {
                    ns = 1;
                } else {
                    ns = 2;
                    q = nct;
                }
            }

            switch (ns) {
                case 1:
                    f = e[m - 2];
                    e[m - 2] = 0.0;
                    for (qp1jj = m - 3; qp1jj + 2 >= q + 1; qp1jj--) {
                        xrotg(&s->data[qp1jj + 1], &f, &ztest0, &ztest);
                        if (qp1jj + 2 > q + 1) {
                            f = -ztest * e[qp1jj];
                            e[qp1jj] *= ztest0;
                        }

                        xrot(Vf, 1 + 20 * (qp1jj + 1), 1 + 20 * (m - 1), ztest0, ztest);
                    }
                    break;

                case 2:
                    f = e[q - 1];
                    e[q - 1] = 0.0;
                    for (qp1jj = q; qp1jj + 1 <= m; qp1jj++) {
                        xrotg(&s->data[qp1jj], &f, &ztest0, &ztest);
                        f = -ztest * e[qp1jj];
                        e[qp1jj] *= ztest0;
                        c_xrot(n, U, 1 + n * qp1jj, 1 + n * (q - 1), ztest0, ztest);
                    }
                    break;

                case 3:
                    varargin_1[0] = std::abs(s->data[m - 1]);
                    varargin_1[1] = std::abs(s->data[m - 2]);
                    varargin_1[2] = std::abs(e[m - 2]);
                    varargin_1[3] = std::abs(s->data[q]);
                    varargin_1[4] = std::abs(e[q]);
                    ns = 1;
                    mtmp = varargin_1[0];
                    if (rtIsNaN(varargin_1[0])) {
                        qp1jj = 2;
                        exitg1 = false;
                        while ((!exitg1) && (qp1jj < 6)) {
                            ns = qp1jj;
                            if (!rtIsNaN(varargin_1[qp1jj - 1])) {
                                mtmp = varargin_1[qp1jj - 1];
                                exitg1 = true;
                            } else {
                                qp1jj++;
                            }
                        }
                    }

                    if (ns < 5) {
                        while (ns + 1 < 6) {
                            if (varargin_1[ns] > mtmp) {
                                mtmp = varargin_1[ns];
                            }

                            ns++;
                        }
                    }

                    f = s->data[m - 1] / mtmp;
                    ztest0 = s->data[m - 2] / mtmp;
                    ztest = e[m - 2] / mtmp;
                    sqds = s->data[q] / mtmp;
                    b = ((ztest0 + f) * (ztest0 - f) + ztest * ztest) / 2.0;
                    ztest0 = f * ztest;
                    ztest0 *= ztest0;
                    if ((b != 0.0) || (ztest0 != 0.0)) {
                        ztest = std::sqrt(b * b + ztest0);
                        if (b < 0.0) {
                            ztest = -ztest;
                        }

                        ztest = ztest0 / (b + ztest);
                    } else {
                        ztest = 0.0;
                    }

                    f = (sqds + f) * (sqds - f) + ztest;
                    b = sqds * (e[q] / mtmp);
                    for (qp1jj = q + 1; qp1jj < m; qp1jj++) {
                        xrotg(&f, &b, &ztest0, &ztest);
                        if (qp1jj > q + 1) {
                            e[qp1jj - 2] = f;
                        }

                        f = ztest0 * s->data[qp1jj - 1] + ztest * e[qp1jj - 1];
                        e[qp1jj - 1] = ztest0 * e[qp1jj - 1] - ztest * s->data[qp1jj - 1];
                        b = ztest * s->data[qp1jj];
                        s->data[qp1jj] *= ztest0;
                        xrot(Vf, 1 + 20 * (qp1jj - 1), 1 + 20 * qp1jj, ztest0, ztest);
                        s->data[qp1jj - 1] = f;
                        xrotg(&s->data[qp1jj - 1], &b, &ztest0, &ztest);
                        f = ztest0 * e[qp1jj - 1] + ztest * s->data[qp1jj];
                        s->data[qp1jj] = -ztest * e[qp1jj - 1] + ztest0 * s->data[qp1jj];
                        b = ztest * e[qp1jj];
                        e[qp1jj] *= ztest0;
                        if (qp1jj < n) {
                            c_xrot(n, U, 1 + n * (qp1jj - 1), 1 + n * qp1jj, ztest0, ztest);
                        }
                    }

                    e[m - 2] = f;
                    iter++;
                    break;

                default:
                    if (s->data[q] < 0.0) {
                        s->data[q] = -s->data[q];
                        b_xscal(-1.0, Vf, 1 + 20 * q);
                    }

                    ns = q + 1;
                    while ((q + 1 < mm) && (s->data[q] < s->data[ns])) {
                        ztest = s->data[q];
                        s->data[q] = s->data[ns];
                        s->data[ns] = ztest;
                        xswap(Vf, 1 + 20 * q, 1 + 20 * (q + 1));
                        if (q + 1 < n) {
                            c_xswap(n, U, 1 + n * q, 1 + n * (q + 1));
                        }

                        q = ns;
                        ns++;
                    }

                    iter = 0;
                    m--;
                    break;
            }
        }
    }

    emxFree_real_T(&A);
    m = work->size[0];
    work->size[0] = minnp;
    emxEnsureCapacity((emxArray__common *)work, m, (int)sizeof(double));
    for (qp1jj = 0; qp1jj + 1 <= minnp; qp1jj++) {
        work->data[qp1jj] = s->data[qp1jj];
    }

    emxFree_real_T(&s);
    emxInit_real_T(&coeff, 2);
    m = coeff->size[0] * coeff->size[1];
    coeff->size[0] = 20;
    coeff->size[1] = minnp;
    emxEnsureCapacity((emxArray__common *)coeff, m, (int)sizeof(double));
    for (nct = 0; nct + 1 <= minnp; nct++) {
        for (iter = 0; iter < 20; iter++) {
            coeff->data[iter + coeff->size[0] * nct] = Vf[iter + 20 * nct];
        }
    }

    emxInit_real_T(&score, 2);
    m = score->size[0] * score->size[1];
    score->size[0] = U->size[0];
    score->size[1] = U->size[1];
    emxEnsureCapacity((emxArray__common *)score, m, (int)sizeof(double));
    ns = U->size[0] * U->size[1];
    for (m = 0; m < ns; m++) {
        score->data[m] = U->data[m];
    }

    ns = U->size[1];
    for (nct = 0; nct + 1 <= ns; nct++) {
        for (iter = 0; iter + 1 <= nrows; iter++) {
            score->data[iter + score->size[0] * nct] = score->data[iter + score->size
                                                                          [0] * nct] / OmegaSqrt->data[iter] * work->data[nct];
        }
    }

    emxFree_real_T(&OmegaSqrt);
    for (nct = 0; nct + 1 <= ns; nct++) {
        work->data[nct] = work->data[nct] * work->data[nct] / (double)DOF;
    }

    if ((score->size[0] == 0) || (score->size[1] == 0)) {
        m = tsquared->size[0] * tsquared->size[1];
        tsquared->size[0] = score->size[0];
        tsquared->size[1] = score->size[1];
        emxEnsureCapacity((emxArray__common *)tsquared, m, (int)sizeof(double));
        ns = score->size[0] * score->size[1];
        for (m = 0; m < ns; m++) {
            tsquared->data[m] = score->data[m];
        }
    } else {
        if (DOF > 1) {
            if (DOF >= 20) {
                ns = DOF;
            } else {
                ns = 20;
            }

            ztest0 = (double)ns * eps(work->data[0]);
            q = 0;
            for (qp1jj = 0; qp1jj < work->size[0]; qp1jj++) {
                if (work->data[qp1jj] > ztest0) {
                    q++;
                }
            }
        } else {
            q = 0;
        }

        emxInit_real_T(&standScores, 2);
        m = standScores->size[0] * standScores->size[1];
        standScores->size[0] = score->size[0];
        standScores->size[1] = q;
        emxEnsureCapacity((emxArray__common *)standScores, m, (int)sizeof(double));
        ns = score->size[0];
        m = tsquared->size[0] * tsquared->size[1];
        tsquared->size[0] = ns;
        tsquared->size[1] = 1;
        emxEnsureCapacity((emxArray__common *)tsquared, m, (int)sizeof(double));
        ns = score->size[0];
        for (m = 0; m < ns; m++) {
            tsquared->data[m] = 0.0;
        }

        for (nct = 0; nct + 1 <= q; nct++) {
            ztest0 = std::sqrt(work->data[nct]);
            for (iter = 0; iter + 1 <= score->size[0]; iter++) {
                standScores->data[iter + standScores->size[0] * nct] = score->data[iter
                                                                                   + score->size[0] * nct] / ztest0;
                tsquared->data[iter] += standScores->data[iter + standScores->size[0] *
                                                                 nct] * standScores->data[iter + standScores->size[0] * nct];
            }
        }

        emxFree_real_T(&standScores);
    }

    if (DOF < 20) {
        qp1jj = U->size[1];
        if (DOF <= qp1jj) {
            qp1jj = DOF;
        }

        m = scoreOut->size[0] * scoreOut->size[1];
        scoreOut->size[0] = nrows;
        scoreOut->size[1] = qp1jj;
        emxEnsureCapacity((emxArray__common *)scoreOut, m, (int)sizeof(double));
        ns = nrows * qp1jj;
        for (m = 0; m < ns; m++) {
            scoreOut->data[m] = 0.0;
        }

        for (nct = 0; nct + 1 <= qp1jj; nct++) {
            for (iter = 0; iter + 1 <= nrows; iter++) {
                scoreOut->data[iter + scoreOut->size[0] * nct] = score->data[iter +
                                                                             score->size[0] * nct];
            }
        }

        m = latentOut->size[0];
        latentOut->size[0] = qp1jj;
        emxEnsureCapacity((emxArray__common *)latentOut, m, (int)sizeof(double));
        for (m = 0; m < qp1jj; m++) {
            latentOut->data[m] = 0.0;
        }

        for (nct = 0; nct + 1 <= qp1jj; nct++) {
            latentOut->data[nct] = work->data[nct];
        }

        m = coeffOut->size[0] * coeffOut->size[1];
        coeffOut->size[0] = 20;
        coeffOut->size[1] = qp1jj;
        emxEnsureCapacity((emxArray__common *)coeffOut, m, (int)sizeof(double));
        ns = 20 * qp1jj;
        for (m = 0; m < ns; m++) {
            coeffOut->data[m] = 0.0;
        }

        for (nct = 0; nct + 1 <= qp1jj; nct++) {
            for (iter = 0; iter < 20; iter++) {
                coeffOut->data[iter + coeffOut->size[0] * nct] = coeff->data[iter +
                                                                             coeff->size[0] * nct];
            }
        }
    } else {
        m = scoreOut->size[0] * scoreOut->size[1];
        scoreOut->size[0] = score->size[0];
        scoreOut->size[1] = score->size[1];
        emxEnsureCapacity((emxArray__common *)scoreOut, m, (int)sizeof(double));
        ns = score->size[0] * score->size[1];
        for (m = 0; m < ns; m++) {
            scoreOut->data[m] = score->data[m];
        }

        m = latentOut->size[0];
        latentOut->size[0] = work->size[0];
        emxEnsureCapacity((emxArray__common *)latentOut, m, (int)sizeof(double));
        ns = work->size[0];
        for (m = 0; m < ns; m++) {
            latentOut->data[m] = work->data[m];
        }

        m = coeffOut->size[0] * coeffOut->size[1];
        coeffOut->size[0] = 20;
        coeffOut->size[1] = coeff->size[1];
        emxEnsureCapacity((emxArray__common *)coeffOut, m, (int)sizeof(double));
        ns = coeff->size[0] * coeff->size[1];
        for (m = 0; m < ns; m++) {
            coeffOut->data[m] = coeff->data[m];
        }
    }

    emxFree_real_T(&work);
    emxFree_real_T(&U);
    emxFree_real_T(&coeff);
    emxFree_real_T(&score);
    ztest0 = sum(latentOut);
    m = explained->size[0];
    explained->size[0] = latentOut->size[0];
    emxEnsureCapacity((emxArray__common *)explained, m, (int)sizeof(double));
    ns = latentOut->size[0];
    for (m = 0; m < ns; m++) {
        explained->data[m] = 100.0 * latentOut->data[m] / ztest0;
    }
}

//
// Arguments    : emxArray_real_T *x
// Return Type  : void
//
static void b_sqrt(emxArray_real_T *x)
{
    int nx;
    int k;
    nx = x->size[0];
    for (k = 0; k + 1 <= nx; k++) {
        x->data[k] = std::sqrt(x->data[k]);
    }
}

//
// Arguments    : const boolean_T x[100]
//                double y[5]
// Return Type  : void
//
static void b_sum(const boolean_T x[100], double y[5])
{
    int j;
    double s;
    int k;
    for (j = 0; j < 5; j++) {
        s = x[j];
        for (k = 0; k < 19; k++) {
            s += (double)x[j + (k + 1) * 5];
        }

        y[j] = s;
    }
}

//
// Arguments    : int n
//                double a
//                const double x[100]
//                int ix0
//                double y[5]
//                int iy0
// Return Type  : void
//
static void b_xaxpy(int n, double a, const double x[100], int ix0, double y[5],
                    int iy0)
{
    int ix;
    int iy;
    int k;
    if ((n < 1) || (a == 0.0)) {
    } else {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y[iy] += a * x[ix];
            ix++;
            iy++;
        }
    }
}

//
// Arguments    : int n
//                const double x[25]
//                int ix0
//                const double y[25]
//                int iy0
// Return Type  : double
//
static double b_xdotc(int n, const double x[25], int ix0, const double y[25],
                      int iy0)
{
    double d;
    int ix;
    int iy;
    int k;
    d = 0.0;
    if (n < 1) {
    } else {
        ix = ix0;
        iy = iy0;
        for (k = 1; k <= n; k++) {
            d += x[ix - 1] * y[iy - 1];
            ix++;
            iy++;
        }
    }

    return d;
}

//
// Arguments    : int n
//                const double x[20]
//                int ix0
// Return Type  : double
//
static double b_xnrm2(int n, const double x[20], int ix0)
{
    double y;
    double scale;
    int kend;
    int k;
    double absxk;
    double t;
    y = 0.0;
    if (n < 1) {
    } else if (n == 1) {
        y = std::abs(x[ix0 - 1]);
    } else {
        scale = 2.2250738585072014E-308;
        kend = (ix0 + n) - 1;
        for (k = ix0; k <= kend; k++) {
            absxk = std::abs(x[k - 1]);
            if (absxk > scale) {
                t = scale / absxk;
                y = 1.0 + y * t * t;
                scale = absxk;
            } else {
                t = absxk / scale;
                y += t * t;
            }
        }

        y = scale * std::sqrt(y);
    }

    return y;
}

//
// Arguments    : double x[25]
//                int ix0
//                int iy0
//                double c
//                double s
// Return Type  : void
//
static void b_xrot(double x[25], int ix0, int iy0, double c, double s)
{
    int ix;
    int iy;
    int k;
    double temp;
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < 5; k++) {
        temp = c * x[ix] + s * x[iy];
        x[iy] = c * x[iy] - s * x[ix];
        x[ix] = temp;
        iy++;
        ix++;
    }
}

//
// Arguments    : double a
//                double x[400]
//                int ix0
// Return Type  : void
//
static void b_xscal(double a, double x[400], int ix0)
{
    int k;
    for (k = ix0; k <= ix0 + 19; k++) {
        x[k - 1] *= a;
    }
}

//
// Arguments    : double x[25]
//                int ix0
//                int iy0
// Return Type  : void
//
static void b_xswap(double x[25], int ix0, int iy0)
{
    int ix;
    int iy;
    int k;
    double temp;
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < 5; k++) {
        temp = x[ix];
        x[ix] = x[iy];
        x[iy] = temp;
        ix++;
        iy++;
    }
}

//
// Arguments    : int n
//                double a
//                const double x[5]
//                int ix0
//                double y[100]
//                int iy0
// Return Type  : void
//
static void c_xaxpy(int n, double a, const double x[5], int ix0, double y[100],
                    int iy0)
{
    int ix;
    int iy;
    int k;
    if ((n < 1) || (a == 0.0)) {
    } else {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y[iy] += a * x[ix];
            ix++;
            iy++;
        }
    }
}

//
// Arguments    : int n
//                const double x[400]
//                int ix0
//                const double y[400]
//                int iy0
// Return Type  : double
//
static double c_xdotc(int n, const double x[400], int ix0, const double y[400],
                      int iy0)
{
    double d;
    int ix;
    int iy;
    int k;
    d = 0.0;
    if (n < 1) {
    } else {
        ix = ix0;
        iy = iy0;
        for (k = 1; k <= n; k++) {
            d += x[ix - 1] * y[iy - 1];
            ix++;
            iy++;
        }
    }

    return d;
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
// Return Type  : double
//
static double c_xnrm2(int n, const emxArray_real_T *x, int ix0)
{
    double y;
    double scale;
    int kend;
    int k;
    double absxk;
    double t;
    y = 0.0;
    if (n < 1) {
    } else if (n == 1) {
        y = std::abs(x->data[ix0 - 1]);
    } else {
        scale = 2.2250738585072014E-308;
        kend = (ix0 + n) - 1;
        for (k = ix0; k <= kend; k++) {
            absxk = std::abs(x->data[k - 1]);
            if (absxk > scale) {
                t = scale / absxk;
                y = 1.0 + y * t * t;
                scale = absxk;
            } else {
                t = absxk / scale;
                y += t * t;
            }
        }

        y = scale * std::sqrt(y);
    }

    return y;
}

//
// Arguments    : int n
//                emxArray_real_T *x
//                int ix0
//                int iy0
//                double c
//                double s
// Return Type  : void
//
static void c_xrot(int n, emxArray_real_T *x, int ix0, int iy0, double c, double
s)
{
    int ix;
    int iy;
    int k;
    double temp;
    if (n < 1) {
    } else {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 1; k <= n; k++) {
            temp = c * x->data[ix] + s * x->data[iy];
            x->data[iy] = c * x->data[iy] - s * x->data[ix];
            x->data[ix] = temp;
            iy++;
            ix++;
        }
    }
}

//
// Arguments    : int n
//                double a
//                emxArray_real_T *x
//                int ix0
// Return Type  : void
//
static void c_xscal(int n, double a, emxArray_real_T *x, int ix0)
{
    int i3;
    int k;
    i3 = (ix0 + n) - 1;
    for (k = ix0; k <= i3; k++) {
        x->data[k - 1] *= a;
    }
}

//
// Arguments    : int n
//                emxArray_real_T *x
//                int ix0
//                int iy0
// Return Type  : void
//
static void c_xswap(int n, emxArray_real_T *x, int ix0, int iy0)
{
    int ix;
    int iy;
    int k;
    double temp;
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 1; k <= n; k++) {
        temp = x->data[ix];
        x->data[ix] = x->data[iy];
        x->data[iy] = temp;
        ix++;
        iy++;
    }
}

//
// Arguments    : int n
//                double a
//                int ix0
//                double y[25]
//                int iy0
// Return Type  : void
//
static void d_xaxpy(int n, double a, int ix0, double y[25], int iy0)
{
    int ix;
    int iy;
    int k;
    if ((n < 1) || (a == 0.0)) {
    } else {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y[iy] += a * y[ix];
            ix++;
            iy++;
        }
    }
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
//                const emxArray_real_T *y
//                int iy0
// Return Type  : double
//
static double d_xdotc(int n, const emxArray_real_T *x, int ix0, const
emxArray_real_T *y, int iy0)
{
    double d;
    int ix;
    int iy;
    int k;
    d = 0.0;
    if (n < 1) {
    } else {
        ix = ix0;
        iy = iy0;
        for (k = 1; k <= n; k++) {
            d += x->data[ix - 1] * y->data[iy - 1];
            ix++;
            iy++;
        }
    }

    return d;
}

//
// Arguments    : int n
//                double a
//                int ix0
//                double y[400]
//                int iy0
// Return Type  : void
//
static void e_xaxpy(int n, double a, int ix0, double y[400], int iy0)
{
    int ix;
    int iy;
    int k;
    if ((n < 1) || (a == 0.0)) {
    } else {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y[iy] += a * y[ix];
            ix++;
            iy++;
        }
    }
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
//                const emxArray_real_T *y
//                int iy0
// Return Type  : double
//
static double e_xdotc(int n, const emxArray_real_T *x, int ix0, const
emxArray_real_T *y, int iy0)
{
    double d;
    int ix;
    int iy;
    int k;
    d = 0.0;
    if (n < 1) {
    } else {
        ix = ix0;
        iy = iy0;
        for (k = 1; k <= n; k++) {
            d += x->data[ix - 1] * y->data[iy - 1];
            ix++;
            iy++;
        }
    }

    return d;
}

//
// Arguments    : emxArray__common *emxArray
//                int oldNumel
//                int elementSize
// Return Type  : void
//
static void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
elementSize)
{
    int newNumel;
    int i;
    void *newData;
    newNumel = 1;
    for (i = 0; i < emxArray->numDimensions; i++) {
        newNumel *= emxArray->size[i];
    }

    if (newNumel > emxArray->allocatedSize) {
        i = emxArray->allocatedSize;
        if (i < 16) {
            i = 16;
        }

        while (i < newNumel) {
            if (i > 1073741823) {
                i = MAX_int32_T;
            } else {
                i <<= 1;
            }
        }

        newData = calloc((unsigned int)i, (unsigned int)elementSize);
        if (emxArray->data != NULL) {
            memcpy(newData, emxArray->data, (unsigned int)(elementSize * oldNumel));
            if (emxArray->canFreeData) {
                free(emxArray->data);
            }
        }

        emxArray->data = newData;
        emxArray->allocatedSize = i;
        emxArray->canFreeData = true;
    }
}

//
// Arguments    : emxArray_real_T **pEmxArray
// Return Type  : void
//
static void emxFree_real_T(emxArray_real_T **pEmxArray)
{
    if (*pEmxArray != (emxArray_real_T *)NULL) {
        if (((*pEmxArray)->data != (double *)NULL) && (*pEmxArray)->canFreeData) {
            free((void *)(*pEmxArray)->data);
        }

        free((void *)(*pEmxArray)->size);
        free((void *)*pEmxArray);
        *pEmxArray = (emxArray_real_T *)NULL;
    }
}

//
// Arguments    : emxArray_real_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
static void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions)
{
    emxArray_real_T *emxArray;
    int i;
    *pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));
    emxArray = *pEmxArray;
    emxArray->data = (double *)NULL;
    emxArray->numDimensions = numDimensions;
    emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
    emxArray->allocatedSize = 0;
    emxArray->canFreeData = true;
    for (i = 0; i < numDimensions; i++) {
        emxArray->size[i] = 0;
    }
}

//
// Arguments    : emxArray_real_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
static void emxInit_real_T1(emxArray_real_T **pEmxArray, int numDimensions)
{
    emxArray_real_T *emxArray;
    int i;
    *pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));
    emxArray = *pEmxArray;
    emxArray->data = (double *)NULL;
    emxArray->numDimensions = numDimensions;
    emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
    emxArray->allocatedSize = 0;
    emxArray->canFreeData = true;
    for (i = 0; i < numDimensions; i++) {
        emxArray->size[i] = 0;
    }
}

//
// Arguments    : double x
// Return Type  : double
//
static double eps(double x)
{
    double r;
    double absxk;
    int exponent;
    absxk = std::abs(x);
    if ((!rtIsInf(absxk)) && (!rtIsNaN(absxk))) {
        if (absxk <= 2.2250738585072014E-308) {
            r = 4.94065645841247E-324;
        } else {
            frexp(absxk, &exponent);
            r = std::ldexp(1.0, exponent - 53);
        }
    } else {
        r = rtNaN;
    }

    return r;
}

//
// Arguments    : int n
//                double a
//                int ix0
//                emxArray_real_T *y
//                int iy0
// Return Type  : void
//
static void f_xaxpy(int n, double a, int ix0, emxArray_real_T *y, int iy0)
{
    int ix;
    int iy;
    int k;
    if ((n < 1) || (a == 0.0)) {
    } else {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y->data[iy] += a * y->data[ix];
            ix++;
            iy++;
        }
    }
}

//
// Arguments    : int n
//                double a
//                const emxArray_real_T *x
//                int ix0
//                emxArray_real_T *y
//                int iy0
// Return Type  : void
//
static void g_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
                    emxArray_real_T *y, int iy0)
{
    int ix;
    int iy;
    int k;
    if ((n < 1) || (a == 0.0)) {
    } else {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y->data[iy] += a * x->data[ix];
            ix++;
            iy++;
        }
    }
}

//
// Arguments    : int n
//                double a
//                const emxArray_real_T *x
//                int ix0
//                emxArray_real_T *y
//                int iy0
// Return Type  : void
//
static void h_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
                    emxArray_real_T *y, int iy0)
{
    int ix;
    int iy;
    int k;
    if ((n < 1) || (a == 0.0)) {
    } else {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y->data[iy] += a * x->data[ix];
            ix++;
            iy++;
        }
    }
}

//
// Arguments    : int n
//                double a
//                int ix0
//                emxArray_real_T *y
//                int iy0
// Return Type  : void
//
static void i_xaxpy(int n, double a, int ix0, emxArray_real_T *y, int iy0)
{
    int ix;
    int iy;
    int k;
    if ((n < 1) || (a == 0.0)) {
    } else {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y->data[iy] += a * y->data[ix];
            ix++;
            iy++;
        }
    }
}

//
// KNNCLASSIFY - Simple K Nearest Neighbor Algorithm using 2norm method:
//  Classify using the Nearest neighbor algorithm
//  Inputs:
//   tX          - Train samples
//  tY          - Train labels
//    testSamples - Test  samples
//  KNN      - Number of nearest neighbors
//
//  Outputs
//  Y           - Predicted targets
// Arguments    : const emxArray_real_T *testSamples
//                double Y[5]
// Return Type  : void
//
static void knnclassify(const emxArray_real_T *testSamples, double Y[5])
{
    int iwork[500];
    int idx[500];
    int k;
    int i;
    static const signed char a[500] = { 1, 1, 1, 1, 1, 1, 3, 1, 5, 1, 1, 1, 1, 1,
                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                        1, 1, 1, 3, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1,
                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                        1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1,
                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                        5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1,
                                        1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 5, 1,
                                        1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3,
                                        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                        4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1,
                                        1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

    emxArray_real_T *Uc;
    int nb;
    int nbins;
    int low_ip1;
    int high_i;
    int mid_i;
    int qEnd;
    emxArray_real_T *nn;
    int kEnd;
    int exitg5;
    int i2;
    boolean_T eok;
    emxArray_real_T *edges;
    emxArray_real_T *b_testSamples;
    emxArray_real_T *r8;
    static double x[32000];
    static double y[32000];
    int b_k;
    static const double tX[32000] = { 0.1473388671875, 0.149072265625,
                                      0.15677490234375, 0.07354736328125, 0.13431396484375, 0.19755859375,
                                      0.16580810546875, 0.137060546875, -0.0632568359375, -0.12227783203125,
                                      -0.15037841796875, -0.15947265625, -0.1347412109375, -0.14752197265625,
                                      -0.1497314453125, -0.1416748046875, -0.12935791015625, -0.122216796875,
                                      -0.121240234375, -0.11046142578125, -0.11273193359375, 0.0302490234375,
                                      0.21964111328125, 0.3101318359375, 0.3598876953125, 0.23592529296875,
                                      0.0573486328125, 0.19151611328125, 0.3626953125, 0.2203369140625,
                                      0.298681640625, 0.158251953125, 0.29359130859375, 0.23529052734375,
                                      0.1240234375, 0.17109375, 0.1475341796875, 0.13590087890625,
                                      0.18018798828125, 0.17686767578125, 0.19810791015625, 0.22342529296875,
                                      0.2441162109375, 0.2157470703125, -0.093017578125, -0.1679931640625,
                                      -0.17056884765625, -0.10587158203125, -0.17310791015625, 0.03297119140625,
                                      0.12713623046875, 0.2618408203125, 0.16649169921875, 0.189892578125,
                                      0.22017822265625, 0.1641357421875, 0.088818359375, 0.21141357421875,
                                      0.18734130859375, 0.16737060546875, -0.28538818359375, -0.15711669921875,
                                      -0.66092529296875, -0.7395263671875, -0.22896728515625, -0.17242431640625,
                                      -0.2353271484375, -0.22701416015625, -0.04376220703125, 0.198046875,
                                      0.20858154296875, 0.1795654296875, 0.22381591796875, 0.21142578125,
                                      0.1507080078125, 0.17335205078125, 0.19847412109375, 0.03455810546875,
                                      0.24725341796875, 0.4451171875, 0.27607421875, 0.35743408203125,
                                      0.25982666015625, 0.3378173828125, 0.42115478515625, 0.44117431640625,
                                      0.439892578125, 0.4583740234375, 0.32242431640625, 0.2593994140625,
                                      0.238818359375, 0.2820556640625, 0.2882080078125, 0.22166748046875,
                                      0.23115234375, 0.1816162109375, 0.22528076171875, 0.10384521484375,
                                      0.0576416015625, 0.0380126953125, -0.00789794921875, 0.04942626953125,
                                      0.04620361328125, 0.0813232421875, -0.04036865234375, 0.21845703125,
                                      0.1253173828125, 0.07806396484375, 0.1237548828125, 0.03978271484375,
                                      0.11046142578125, 0.127294921875, 0.0966552734375, 0.07689208984375,
                                      0.116943359375, 0.136083984375, 0.214013671875, -0.01964111328125,
                                      0.1257568359375, -0.00560302734375, 0.03685302734375, 0.07237548828125,
                                      0.0780029296875, -4.8828125E-5, 0.0349365234375, 0.08944091796875,
                                      -0.1112548828125, -0.1643310546875, 0.04815673828125, 0.066162109375,
                                      0.0150146484375, 0.17935791015625, 0.08565673828125, -0.00362548828125,
                                      0.01990966796875, 0.10047607421875, 0.08909912109375, 0.09375, 0.06630859375,
                                      0.1036865234375, 0.08597412109375, -0.0832275390625, -0.129931640625,
                                      -0.2888916015625, -0.10238037109375, 0.1132568359375, 0.109814453125,
                                      0.08922119140625, 0.11275634765625, 0.2509521484375, 0.17138671875,
                                      0.07213134765625, 0.10858154296875, 0.05802001953125, 0.1156494140625,
                                      0.1070068359375, 0.10950927734375, -0.0859893798828125, -0.19334716796875,
                                      -0.19283447265625, -0.18707275390625, -0.197509765625, -0.1846923828125,
                                      -0.1934814453125, -0.20123291015625, -0.1377685546875, 0.05611572265625,
                                      0.1368896484375, 0.04010009765625, 0.07900390625, 0.10574951171875,
                                      0.14180908203125, 0.28150634765625, 0.2510009765625, 0.0614501953125,
                                      0.004345703125, -0.01761474609375, -0.01634521484375, -0.00126953125,
                                      -0.0123779296875, 0.00159912109375, -0.01861572265625, -0.0166015625,
                                      -0.08031005859375, 0.1319091796875, 0.1639892578125, 0.16407470703125,
                                      0.15911865234375, 0.1518798828125, 0.14805908203125, 0.15452880859375,
                                      0.15860595703125, 0.15074462890625, 0.15595703125, 0.14447021484375,
                                      0.07587890625, 0.1275146484375, 0.0840087890625, 0.10887451171875,
                                      0.17730712890625, -0.23870849609375, 0.05791015625, 0.077001953125,
                                      0.0134521484375, -0.08424072265625, 0.03768310546875, 0.0611083984375,
                                      0.16446533203125, 0.12374267578125, 0.1901123046875, 0.2099365234375,
                                      -0.112747192382813, -0.788623046875, -0.27908935546875, -0.19158935546875,
                                      0.14200439453125, 0.0828857421875, 0.06859130859375, 0.08739013671875,
                                      0.15986328125, 0.08892822265625, 0.06563720703125, -0.82913818359375,
                                      -0.258251953125, -0.37276611328125, 0.0025146484375, 0.0451416015625,
                                      0.17122802734375, 0.10750732421875, 0.05411376953125, 0.09061279296875,
                                      0.08033447265625, 0.1347900390625, 0.1821044921875, -0.033740234375,
                                      -0.74371337890625, -0.54580078125, -0.16695556640625, -0.0053955078125,
                                      0.0087646484375, -0.01427001953125, -0.008056640625, 0.053515625,
                                      -0.759426879882813, -0.543310546875, -0.08172607421875, -0.26270751953125,
                                      0.10477294921875, 0.1015869140625, 0.02838134765625, 0.08548583984375,
                                      0.0742431640625, 0.07451171875, 0.09010009765625, 0.0705078125,
                                      -0.4374267578125, -0.654443359375, -0.32926025390625, -0.05784912109375,
                                      -0.09254150390625, -0.00872802734375, -0.16171875, 0.0338623046875,
                                      -0.04244384765625, -0.01832275390625, -0.04984130859375, -0.04365234375,
                                      -0.0200927734375, -0.02408447265625, -0.02198486328125, 0.0149658203125,
                                      -0.024267578125, -0.011865234375, 0.02506103515625, 0.12799072265625,
                                      0.1267578125, 0.13392333984375, 0.05478515625, 0.13753662109375,
                                      0.11934814453125, 0.0921630859375, 0.1225830078125, 0.0976318359375,
                                      0.09364013671875, 0.0917724609375, 0.129541015625, -0.05858154296875,
                                      0.2279296875, 0.07196044921875, 0.12655029296875, 0.11834716796875,
                                      0.00325927734375, -0.10528564453125, -0.09105224609375, 0.16175537109375,
                                      0.0632568359375, 0.12335205078125, 0.12696533203125, 0.13580322265625,
                                      0.11385498046875, 0.09444580078125, 0.0343994140625, 0.09849853515625,
                                      0.0984619140625, 0.12283935546875, 0.11024169921875, 0.107357788085938,
                                      0.1202880859375, 0.096923828125, 0.04765625, 0.1447998046875,
                                      0.11473388671875, 0.1865234375, 0.20299072265625, 0.06337890625,
                                      0.0889892578125, 0.11444091796875, 0.10574951171875, 0.10101318359375,
                                      0.09820556640625, 0.09930419921875, 0.1311279296875, 0.11353759765625,
                                      0.10809326171875, 0.11522216796875, 0.11444091796875, 0.10399169921875,
                                      0.13287353515625, 0.11300048828125, 0.101318359375, 0.1255859375,
                                      0.13035888671875, 0.12645263671875, 0.14713134765625, 0.128515625,
                                      0.0876220703125, 0.13033447265625, 0.09217529296875, 0.0996826171875,
                                      0.0803955078125, 0.1237060546875, 0.1341064453125, 0.1355224609375,
                                      0.113232421875, 0.11224365234375, 0.14228515625, 0.128515625,
                                      0.14053955078125, 0.11197509765625, 0.09818115234375, 0.105517578125,
                                      0.13388671875, 0.04508056640625, 0.13006591796875, 0.0873779296875,
                                      0.10330810546875, 0.0918701171875, 0.1124755859375, 0.1273681640625,
                                      0.14857177734375, 0.07822265625, 0.09884033203125, 0.09801025390625,
                                      0.1198974609375, 0.1009033203125, 0.12908935546875, 0.10380859375,
                                      0.10372314453125, 0.17200927734375, 0.09095458984375, 0.10042724609375,
                                      0.06710205078125, 0.11729736328125, 0.12838134765625, 0.09835205078125,
                                      0.16824951171875, 0.117578125, 0.1413818359375, 0.108935546875,
                                      0.12049560546875, 0.07623291015625, 0.11483154296875, 0.05316162109375,
                                      0.12945556640625, 0.11103515625, 0.1219970703125, 0.113671875,
                                      0.1010009765625, 0.0883544921875, 0.10604248046875, 0.11746826171875,
                                      0.0283203125, 0.06832275390625, 0.13211669921875, 0.0870361328125,
                                      0.110498046875, 0.1645263671875, 0.09857177734375, 0.11043701171875,
                                      0.12908935546875, 0.12274169921875, 0.1447998046875, 0.0860595703125,
                                      0.1396728515625, 0.12919921875, 0.12445068359375, 0.12215576171875,
                                      0.1123291015625, 0.1573486328125, 0.09866943359375, 0.14434814453125,
                                      0.118798828125, 0.14014892578125, 0.13843994140625, 0.156396484375,
                                      0.13673095703125, 0.1069580078125, 0.1561279296875, 0.16220703125,
                                      0.160595703125, 0.1201171875, 0.1244140625, 0.12088623046875, 0.16220703125,
                                      0.064501953125, 0.13629150390625, 0.21983642578125, 0.14964599609375,
                                      0.09649658203125, 0.1507080078125, 0.1392578125, 0.15325927734375,
                                      0.106298828125, 0.15513916015625, 0.15487060546875, 0.15396728515625,
                                      0.19522705078125, -0.08580322265625, 0.14613037109375, 0.13048095703125,
                                      0.12816162109375, 0.1449462890625, 0.14576416015625, 0.132666015625,
                                      0.139404296875, 0.124609375, 0.139013671875, 0.1478759765625, 0.231982421875,
                                      0.17689208984375, 0.15567626953125, 0.145166015625, 0.16732177734375,
                                      0.14398193359375, 0.1258544921875, 0.1511474609375, 0.14466552734375,
                                      0.14033203125, 0.146630859375, 0.1266845703125, 0.1252685546875,
                                      0.1125732421875, 0.13028564453125, 0.15572509765625, 0.12767333984375,
                                      0.1454833984375, 0.13331298828125, 0.13876953125, 0.08203125,
                                      0.14603271484375, 0.1145751953125, 0.12213134765625, 0.11741943359375,
                                      0.11103515625, 0.08587646484375, 0.11064453125, 0.1444091796875,
                                      0.11923828125, 0.13294677734375, 0.12694091796875, 0.1256103515625,
                                      0.10521240234375, 0.11251220703125, 0.09681396484375, 0.11446533203125,
                                      0.12523193359375, 0.10794677734375, -0.44310302734375, -0.9746337890625,
                                      -0.8364990234375, -0.17120361328125, 0.12603759765625, 0.1036376953125,
                                      0.1373046875, 0.16260986328125, 0.09237060546875, -0.66658935546875,
                                      -0.91392822265625, -0.60703125, -0.04141845703125, -0.97171630859375,
                                      -0.969384765625, -0.97200927734375, -0.99605712890625, -0.925732421875,
                                      -0.9465576171875, -0.9884033203125, -0.96390380859375, -0.205960083007813,
                                      -0.018994140625, 0.03956298828125, 0.10892333984375, 0.16759033203125,
                                      0.20439453125, 0.2154296875, 0.205615234375, 0.205859375, 0.15240478515625,
                                      0.2108642578125, 0.2224853515625, 0.23958740234375, -0.30096435546875,
                                      -0.92984619140625, -0.9552734375, -0.86798095703125, -0.9099365234375,
                                      -0.7312255859375, -0.97705078125, -0.93238525390625, -0.96514892578125,
                                      -0.96011962890625, -0.9922607421875, -0.94639892578125, -0.97216796875,
                                      -0.98055419921875, -0.99326171875, -1.00302734375, -1.01309814453125,
                                      -0.9803955078125, -0.89378662109375, -0.97540283203125, -0.9876953125,
                                      -0.976318359375, -0.96007080078125, -0.0024322509765625, 0.1646484375,
                                      0.179052734375, 0.2657470703125, 0.36136474609375, -0.54820556640625,
                                      -0.8228759765625, -0.89202880859375, -0.968115234375, -0.982861328125,
                                      -0.97593994140625, -0.98349609375, -1.00714111328125, -0.97681884765625,
                                      -0.97423095703125, -0.83104248046875, 0.339352416992188, 0.3390380859375,
                                      0.023876953125, -0.0424072265625, 0.36790771484375, 0.3512939453125,
                                      0.03026123046875, 0.05189208984375, -0.36151123046875, -0.7564208984375,
                                      -0.96314697265625, -0.9701416015625, -0.94393310546875, -0.97335205078125,
                                      -0.97615966796875, -0.96795654296875, -0.605615234375, -0.58231201171875,
                                      -0.6499267578125, -0.7061279296875, -0.940087890625, -0.9478759765625,
                                      -0.96231689453125, -0.94375, -0.9045166015625, -0.8858642578125,
                                      -0.87908935546875, -0.86580810546875, -0.9296875, -0.97486572265625,
                                      -0.981005859375, -0.9652587890625, -0.9669677734375, -0.97178955078125,
                                      -0.96510009765625, -0.9668212890625, -0.95592041015625, -0.98231201171875,
                                      -0.989697265625, -0.97828369140625, -0.96319580078125, -0.9934814453125,
                                      -0.95947265625, -0.9710693359375, -1.08448486328125, -0.94580078125,
                                      -0.9915771484375, -1.0089111328125, -1.00096435546875, -1.009423828125,
                                      -1.0077880859375, -0.96761474609375, -1.00592041015625, -1.01865234375,
                                      -0.9989013671875, -0.9669677734375, -0.97640380859375, -1.00748291015625,
                                      -0.986279296875, -1.0103515625, -0.90919189453125, -0.97877197265625,
                                      -0.97203369140625, -0.992724609375, -1.02525634765625, -0.9675048828125,
                                      -0.26763916015625, 0.0039794921875, -0.9414306640625, -0.9939697265625,
                                      -1.00557861328125, -0.8065673828125, -1.00889892578125, -1.0173828125,
                                      -1.015380859375, -0.9995361328125, -0.9592529296875, -0.9966796875,
                                      -1.0068115234375, -0.98687744140625, -0.96312255859375, 0.0231903076171875,
                                      0.30550537109375, 0.2354736328125, -0.59656982421875, -0.81121826171875,
                                      -0.94105224609375, -0.9663330078125, -1.02138671875, -0.9504638671875,
                                      -0.9749267578125, -0.93270263671875, -1.0023193359375, -0.94951171875,
                                      -0.98052978515625, -0.98114013671875, -0.90247802734375, 0.40260009765625,
                                      0.2501220703125, 0.23192138671875, 0.207275390625, 0.1566162109375,
                                      0.22462158203125, 0.22628173828125, 0.2203125, -0.10374755859375,
                                      -0.94749755859375, -0.80732421875, -0.9853271484375, -0.9125244140625,
                                      -0.99632568359375, -0.994775390625, -0.9698974609375, -0.95576171875,
                                      -0.998486328125, -1.009765625, -0.98712158203125, -1.011328125,
                                      -0.99600830078125, -0.990673828125, -1.0017578125, -1.00006103515625,
                                      -1.0054443359375, -0.996337890625, -1.01014404296875, -0.99951171875,
                                      -0.9988037109375, -0.99892578125, -1.00228271484375, -1.0011962890625,
                                      -1.00301513671875, -1.001025390625, -1.001953125, -1.00155029296875,
                                      -1.004443359375, -1.00355224609375, -1.00133056640625, -1.00986328125,
                                      -1.0063720703125, -0.98878173828125, -0.9407470703125, -0.99415283203125,
                                      -1.00362548828125, -0.99219970703125, -0.9946533203125, -1.01087646484375,
                                      -1.0130126953125, -0.99005126953125, -1.00830078125, -0.9884521484375,
                                      -0.995654296875, -0.35184326171875, -0.18226318359375, -0.73851318359375,
                                      -0.7210205078125, -0.94404296875, -1.001025390625, -1.0124755859375,
                                      -0.9739501953125, -1.00260009765625, -1.0078125, -0.73592529296875,
                                      -0.2220458984375, -0.74063720703125, -0.72105712890625, -0.8482421875,
                                      -0.988671875, -0.99404296875, -1.001708984375, -1.00157470703125,
                                      -1.0081787109375, -1.00723876953125, -0.992919921875, -0.9885498046875,
                                      -0.546182250976563, -0.201123046875, -0.061376953125, -0.75003662109375,
                                      -0.96602783203125, -1.00584716796875, -1.017724609375, -1.01302490234375,
                                      -0.814990234375, -0.168505859375, 0.0226318359375, -0.91785888671875,
                                      -0.93382568359375, -0.9714111328125, -0.99083251953125, -1.02294921875,
                                      -0.9934814453125, -0.999609375, -1.013037109375, -1.00045166015625,
                                      -1.00491943359375, -0.163284301757813, -0.23770751953125, -0.37994384765625,
                                      -0.9108642578125, -1.005712890625, -0.9974853515625, -0.99024658203125,
                                      -1.00032958984375, -1.01104736328125, -1.00980224609375, -1.014013671875,
                                      -1.00672607421875, -1.0038818359375, -1.016357421875, -1.01214599609375,
                                      -0.9962158203125, -1.00018310546875, -1.01607666015625, -1.0299072265625,
                                      -1.00250244140625, -1.001318359375, -1.00662841796875, -1.0195068359375,
                                      -1.00494384765625, -1.00838623046875, -1.00855712890625, -1.00604248046875,
                                      -0.98157958984375, -0.94652099609375, -0.88948974609375, -0.19368896484375,
                                      -0.0886474609375, -0.0495849609375, -0.6958740234375, -0.977099609375,
                                      -0.9836669921875, -0.165383911132813, -0.033984375, 0.02799072265625,
                                      -0.6870361328125, -0.8804443359375, -0.99149169921875, -0.97049560546875,
                                      -0.32288818359375, -0.36185302734375, -0.35947265625, -0.67333984375,
                                      -0.96197509765625, -1.00146484375, -0.98499755859375, -0.85340576171875,
                                      -0.2537841796875, -0.17357177734375, -0.778515625, -0.8112548828125,
                                      -0.99822998046875, -0.99886474609375, -0.6131103515625, -0.203564453125,
                                      -0.2723876953125, -0.772265625, -0.97569580078125, -1.0163818359375,
                                      -1.01185302734375, -1.011962890625, -1.01014404296875, -1.01822509765625,
                                      -1.01019287109375, -1.0038330078125, -1.0036865234375, -0.9980224609375,
                                      -1.00374755859375, -0.991064453125, -1.01234130859375, -1.0041259765625,
                                      -1.0026123046875, -0.9993408203125, -1.01619873046875, -1.00380859375,
                                      -0.99439697265625, -0.99676513671875, -1.01953125, -0.9954345703125,
                                      -0.9916259765625, -1.03868408203125, -0.9872802734375, -1.00069580078125,
                                      -0.98697509765625, -0.99979248046875, -0.99244384765625, -0.9935302734375,
                                      -0.9897216796875, -0.99127197265625, -0.992138671875, -0.99422607421875,
                                      -0.99549560546875, -0.98924560546875, -1.028564453125, -1.016650390625,
                                      -1.01773681640625, -0.97720947265625, -0.984375, -1.0121337890625,
                                      -0.98492431640625, -1.00550537109375, -1.00111083984375, -1.02110595703125,
                                      -1.0122314453125, -0.99908447265625, -1.010400390625, -1.00469970703125,
                                      -1.022216796875, -1.01097412109375, -0.9888916015625, -1.019775390625,
                                      -0.98919677734375, -1.00767822265625, -1.0018798828125, -1.0142333984375,
                                      -0.993212890625, -0.99578857421875, -1.005810546875, -1.006884765625,
                                      -1.0046142578125, -1.0134033203125, -1.01475830078125, -1.0061279296875,
                                      -1.0222900390625, -1.0169677734375, -1.00841064453125, -0.99852294921875,
                                      -0.99974365234375, -1.00042724609375, -1.02032470703125, -1.00806884765625,
                                      -1.00589599609375, -0.99759521484375, -1.01689453125, -0.818505859375,
                                      -0.86766357421875, -0.83658447265625, -0.90731201171875, -0.913232421875,
                                      -1.0083251953125, -0.90869140625, -1.00458984375, -1.04056396484375,
                                      -0.86915283203125, -1.0757568359375, -0.896826171875, -1.01075439453125,
                                      -0.98284912109375, -0.9948974609375, -1.02518310546875, -0.887451171875,
                                      -1.0460205078125, -0.939404296875, -1.0127685546875, -0.97320556640625,
                                      -1.00723876953125, -0.982275390625, -0.9553466796875, -1.03248291015625,
                                      -0.909423828125, -1.0830078125, -0.93450927734375, -0.99969482421875,
                                      -1.02113037109375, -0.98760986328125, -0.97889404296875, -1.0338134765625,
                                      -0.96964111328125, -1.06990966796875, -0.928125, -1.071533203125,
                                      -0.91322021484375, -1.05665283203125, -0.95960693359375, -1.0580322265625,
                                      -0.94541015625, -1.05816650390625, -0.78551025390625, -0.42574462890625,
                                      -0.93975830078125, -0.94945068359375, -0.9241943359375, -0.972412109375,
                                      -0.92669677734375, -0.92608642578125, -0.983935546875, -0.93162841796875,
                                      -0.91466064453125, -0.991552734375, -0.955224609375, -0.90372314453125,
                                      -0.94583740234375, -0.9298828125, -1.02012939453125, -0.9282958984375,
                                      -0.88240966796875, -0.90003662109375, -0.95537109375, -1.00028076171875,
                                      -0.959033203125, -0.9251953125, -0.95872802734375, -0.9447021484375,
                                      -0.9294189453125, -1.0010986328125, -0.94573974609375, -0.9993896484375,
                                      -0.93408203125, -0.81800537109375, -0.5437744140625, -0.836083984375,
                                      -0.6718017578125, -0.68814697265625, -0.8114501953125, -0.7418212890625,
                                      -0.654638671875, -0.8135498046875, -0.56414794921875, -0.6962890625,
                                      -0.97222900390625, -1.0099853515625, -1.00999755859375, -1.00963134765625,
                                      -1.01021728515625, -1.011962890625, -1.0085693359375, -0.9898681640625,
                                      -0.86746826171875, -0.54503173828125, -0.19339599609375, -0.351025390625,
                                      -0.189697265625, -0.78585205078125, -0.992431640625, -1.00206298828125,
                                      -0.98028564453125, -0.81427001953125, -0.36405029296875, -0.06922607421875,
                                      -0.23525390625, -0.78253173828125, 0.16204833984375, 0.1572265625,
                                      0.16025390625, 0.053955078125, -0.35123291015625, -0.01947021484375,
                                      0.1060302734375, 0.1763671875, 0.42891845703125, 0.8909912109375,
                                      0.90174560546875, 0.90504150390625, 0.87086181640625, 0.8796630859375,
                                      0.87401123046875, 0.8690185546875, 0.878662109375, 0.8931640625,
                                      0.88773193359375, 0.87301025390625, 0.8583740234375, 0.74163818359375,
                                      0.04393310546875, 0.08251953125, 0.23570556640625, -0.0906005859375,
                                      -0.56361083984375, -0.0464599609375, -0.01318359375, -0.11817626953125,
                                      -0.05010986328125, -0.0001708984375, 0.01558837890625, -0.045263671875,
                                      -0.23253173828125, -0.1323974609375, -0.0086181640625, -0.0453857421875,
                                      -0.1770263671875, -0.4880859375, -0.15811767578125, -0.0485107421875,
                                      -0.073876953125, -0.09012451171875, 0.561508178710938, 0.87613525390625,
                                      0.88179931640625, 0.86341552734375, 0.82333984375, 0.45943603515625,
                                      -0.53232421875, 0.06873779296875, 0.134765625, 0.02071533203125,
                                      -0.078759765625, -0.28349609375, -0.19508056640625, 0.0251220703125,
                                      0.123779296875, 0.06009521484375, 0.57646484375, 0.84447021484375,
                                      0.61348876953125, 0.26015625, 0.81409912109375, 0.8323486328125,
                                      0.872509765625, 0.8926513671875, 0.57330322265625, -0.5013671875,
                                      0.0227294921875, 0.07701416015625, 0.17056884765625, 0.102392578125,
                                      0.08985595703125, 0.09727783203125, -0.793798828125, -0.9031982421875,
                                      -0.778271484375, -0.63331298828125, -0.2416259765625, -0.0477294921875,
                                      0.096484375, -0.0799072265625, -0.21365966796875, -0.25272216796875,
                                      -0.2977783203125, -0.3008056640625, -0.28394775390625, -0.0640380859375,
                                      -0.18653564453125, -0.16673583984375, -0.15928955078125, 0.10311279296875,
                                      0.11824951171875, 0.121630859375, 0.10042724609375, 0.13719482421875,
                                      0.20584716796875, 0.1848876953125, 0.20709228515625, 0.16617431640625,
                                      0.21202392578125, 0.17418212890625, 0.065972900390625, 0.2439453125,
                                      0.1124755859375, 0.00689697265625, 0.03765869140625, 0.04508056640625,
                                      0.03463134765625, 0.0913818359375, 0.0149658203125, 0.09324951171875,
                                      0.05040283203125, -0.0613525390625, -0.2212158203125, -0.03392333984375,
                                      -0.0185302734375, -0.2347900390625, 0.19896240234375, 0.20362548828125,
                                      0.18643798828125, 0.06343994140625, -0.1864501953125, 0.17032470703125,
                                      0.382077026367188, 0.88900146484375, 0.0251708984375, 0.0464599609375,
                                      0.07108154296875, -0.50341796875, -0.09459228515625, -0.07774658203125,
                                      -0.07818603515625, -0.10482177734375, 0.184228515625, 0.05987548828125,
                                      0.03565673828125, 0.1068603515625, 0.18399658203125, 0.36573486328125,
                                      0.8532958984375, 0.85968017578125, 0.4722900390625, -0.5254638671875,
                                      -0.3554931640625, 0.12530517578125, 0.04715576171875, -0.2294921875,
                                      -0.2401123046875, 0.16292724609375, 0.08094482421875, -0.41417236328125,
                                      -0.11778564453125, 0.1443603515625, 0.121923828125, 0.590463256835938,
                                      0.861572265625, 0.86849365234375, 0.8658935546875, 0.88203125,
                                      0.86016845703125, 0.8576416015625, 0.86162109375, 0.8255126953125,
                                      0.12069091796875, -0.65968017578125, 0.0420654296875, 0.30074462890625,
                                      0.15914306640625, 0.02044677734375, -0.076123046875, -0.12747802734375,
                                      0.10289306640625, 0.087109375, 0.13961181640625, 0.0538330078125,
                                      0.1035888671875, 0.096044921875, 0.08984375, 0.0672119140625,
                                      0.06839599609375, 0.0568115234375, -0.0795166015625, -0.155517578125,
                                      -0.16290283203125, -0.162890625, -0.161962890625, -0.15648193359375,
                                      -0.14056396484375, -0.1421875, -0.1373779296875, -0.14378662109375,
                                      -0.1181640625, -0.04466552734375, -0.01397705078125, -0.00401611328125,
                                      -0.009423828125, 0.00555419921875, -0.32972412109375, -0.06309814453125,
                                      -0.01864013671875, 0.11883544921875, 0.07462158203125, 0.0001220703125,
                                      -0.101318359375, -0.01824951171875, -0.02410888671875, -0.145458984375,
                                      -0.09732666015625, 0.223886108398438, 0.47078857421875, -0.53616943359375,
                                      -0.66470947265625, -0.15087890625, 0.00308837890625, -0.13555908203125,
                                      0.0502197265625, -0.22669677734375, -0.16109619140625, -0.14310302734375,
                                      0.11534423828125, -0.59080810546875, -0.705712890625, -0.36392822265625,
                                      -0.2881103515625, -0.107080078125, -0.02578125, -0.230810546875,
                                      -0.1005859375, -0.1306884765625, -0.13936767578125, -0.2349365234375,
                                      -0.05185546875, 0.371533203125, -0.8822998046875, -0.61466064453125,
                                      -0.2728271484375, -0.15220947265625, -0.15570068359375, -0.17919921875,
                                      -0.2337646484375, 0.270382690429688, -0.9154052734375, -0.44405517578125,
                                      -0.15115966796875, -0.1735595703125, -0.1788330078125, -0.1791748046875,
                                      -0.270556640625, -0.22567138671875, -0.103125, -0.01666259765625,
                                      -0.1670166015625, 0.111328125, -0.5849853515625, -0.82156982421875,
                                      -0.247705078125, -0.2439453125, -0.26556396484375, -0.25179443359375,
                                      -0.158544921875, -0.177197265625, -0.12547607421875, -0.23839111328125,
                                      -0.2330322265625, -0.0361083984375, -0.15989990234375, -0.20059814453125,
                                      -0.2838134765625, -0.2650390625, -0.180322265625, -0.16268310546875,
                                      -0.1776123046875, -0.20650634765625, -0.19906005859375, -0.07764892578125,
                                      -0.16087646484375, -0.1435546875, -0.2011962890625, -0.160888671875,
                                      -0.3044189453125, -0.42032470703125, -0.3932861328125, -1.079638671875,
                                      -1.0574462890625, -1.00791015625, -0.721484375, -0.22869873046875,
                                      -0.19697265625, -0.79139404296875, -1.05924072265625, -1.04871826171875,
                                      -0.7868408203125, -0.34251708984375, -0.2330322265625, -0.26231689453125,
                                      -0.73924560546875, -1.047607421875, -0.9377685546875, -0.76680908203125,
                                      -0.2403564453125, -0.26490478515625, -0.19224853515625, -0.23253173828125,
                                      -0.99306640625, -1.07904052734375, -0.67161865234375, -0.46553955078125,
                                      -0.18017578125, -0.17550048828125, -0.586669921875, -1.00537109375,
                                      -0.90975341796875, -0.634326171875, -0.12491455078125, -0.066943359375,
                                      -0.0695068359375, -0.08944091796875, -0.07362060546875, -0.13819580078125,
                                      -0.06231689453125, -0.1580322265625, -0.1734375, -0.16539306640625,
                                      -0.22938232421875, -0.1488525390625, -0.15911865234375, -0.1638671875,
                                      -0.20594482421875, -0.14036865234375, -0.16844482421875, -0.18792724609375,
                                      -0.1477294921875, -0.10006103515625, -0.10927734375, -0.114306640625,
                                      -0.14429931640625, -0.13387451171875, -0.20888671875, -0.15316162109375,
                                      -0.1453125, -0.22947998046875, -0.21336669921875, -0.27772216796875,
                                      -0.195556640625, -0.2191650390625, -0.19190673828125, -0.22579345703125,
                                      -0.1832763671875, -0.16717529296875, -0.15399169921875, -0.16875,
                                      -0.2030517578125, -0.21676025390625, -0.22564697265625, -0.14833984375,
                                      -0.1703369140625, -0.127978515625, -0.17149658203125, -0.13076171875,
                                      -0.15189208984375, -0.1204345703125, -0.1754150390625, -0.15379638671875,
                                      -0.134814453125, -0.09061279296875, -0.13970947265625, -0.12447509765625,
                                      -0.144970703125, -0.10452880859375, -0.06224365234375, -0.1203369140625,
                                      -0.10540771484375, -0.1748779296875, -0.047705078125, -0.16864013671875,
                                      -0.13209228515625, -0.07977294921875, -0.078466796875, -0.031689453125,
                                      -0.1393310546875, -0.06822509765625, -0.09530029296875, -0.04368896484375,
                                      -0.10953369140625, -0.10682373046875, -0.09012451171875, -0.071044921875,
                                      -0.1995361328125, -0.2083251953125, -0.22684326171875, -0.12374267578125,
                                      -0.06676025390625, -0.046484375, -0.1314453125, -0.090087890625,
                                      -0.05238037109375, -0.10255126953125, -0.1020751953125, -0.0252685546875,
                                      -0.064208984375, -0.03123779296875, -0.06370849609375, -0.0974365234375,
                                      -0.0814208984375, -0.06761474609375, -0.0956298828125, -0.02772216796875,
                                      -0.052294921875, -0.10218505859375, -0.04154052734375, -0.08154296875,
                                      -0.1111572265625, -0.0317138671875, -0.12301025390625, -0.1474365234375,
                                      -0.14112548828125, -0.08946533203125, -0.14368896484375, -0.0786865234375,
                                      -0.04080810546875, -0.0763916015625, -0.05399169921875, -0.07498779296875,
                                      -0.1172607421875, -0.08311767578125, -0.11029052734375, -0.1154296875,
                                      -0.06337890625, -0.1010009765625, -0.166650390625, -0.04154052734375,
                                      -0.09248046875, -0.12593994140625, -0.5704345703125, -0.99404296875,
                                      -0.4348876953125, -0.44910888671875, -0.18773193359375, -0.39539794921875,
                                      -0.38935546875, -0.37762451171875, -0.4272216796875, -0.132470703125,
                                      -0.44915771484375, -0.3958251953125, -0.15421142578125, -0.40718994140625,
                                      -0.36912841796875, -0.13671875, -0.45804443359375, -0.17139892578125,
                                      -0.548095703125, -0.46787109375, -0.16314697265625, -0.35245361328125,
                                      -0.290673828125, -0.38951416015625, -0.480712890625, -0.13468017578125,
                                      -0.3639892578125, -0.41226806640625, -0.01949462890625, -0.12349853515625,
                                      -0.17286376953125, -0.2058349609375, -0.16619873046875, -0.18245849609375,
                                      -0.126220703125, -0.17548828125, -0.1573486328125, -0.1594970703125,
                                      -0.16314697265625, -0.1755859375, -0.16966552734375, -0.16107177734375,
                                      -0.171142578125, -0.0825439453125, -0.07525634765625, -0.10556640625,
                                      -0.1131103515625, -0.11297607421875, -0.11258544921875, -0.23291015625,
                                      -0.410498046875, -0.508251953125, 0.1837158203125, -0.1466064453125,
                                      -0.8989990234375, -0.55863037109375, -0.12900390625, -0.1631591796875,
                                      -0.14862060546875, -0.481884765625, -0.11280517578125, 0.39189453125,
                                      -0.7767822265625, -0.671729633826336, 0.996135114399843, 0.99335686641764,
                                      0.997771709211425, 1.00722629282465, 1.04725078696766, 1.00901165171957,
                                      1.0203742827785, 0.992417692081506, 1.0464779414696, 0.901287573233808,
                                      0.916151831535386, 0.926722553342017, 0.898251358008922, 0.916129209283611,
                                      0.912694183251079, 0.904622889751409, 0.912316920809908, 0.921818898900792,
                                      0.924010496573847, 0.909127387790697, 0.900918509628005, 0.946706627815677,
                                      0.97623340145634, 1.02460028131022, 0.987495974321834, 1.0350961712023,
                                      1.06568621455451, 1.0169426986382, 1.00688715866719, 1.01496754530824,
                                      1.02253573230237, 1.01846335928651, 1.00301001737059, 1.01291464666688,
                                      1.03243666394863, 1.03037625549578, 1.01616535932425, 1.02455703950545,
                                      1.03273703681596, 1.05709370807974, 1.02534669663851, 1.01412448464606,
                                      1.01133564111956, 1.01095872969264, 0.973044443711271, 0.91275920776746,
                                      0.917882220227113, 0.920279010768102, 0.919974871514265, 1.00264749669222,
                                      1.09062719097355, 0.969673902619144, 1.01649198877047, 1.00710970801279,
                                      1.01731301998439, 1.04578368133105, 1.04033416667363, 1.01087767224818,
                                      1.00091258263436, 0.859987699282896, 1.11366704219307, 0.925340999586415,
                                      0.952635479901249, 1.01577213123261, 0.924574190638058, 0.922811244905521,
                                      0.911185633882847, 0.926698201760526, 0.962033404039333, 1.0461125893908,
                                      1.01355882307503, 0.998670681827948, 0.985908475428122, 1.01031519762603,
                                      0.993781925556108, 0.993235578019662, 1.09469066104025, 1.07881742969149,
                                      1.06641417884635, 1.04939195629848, 1.03246831866819, 1.04110541373341,
                                      1.00899900721992, 1.02007724211145, 1.0223805153623, 1.02177901743209,
                                      1.02887473481677, 1.02624830782669, 1.03669641322552, 1.01180773133197,
                                      1.02908938349548, 1.02088363545544, 1.02305051239294, 1.0133378487705,
                                      1.00993401218451, 0.992560880156281, 0.990207989763864, 0.998732272467384,
                                      1.01871141440176, 0.999144061786363, 0.991470282092076, 1.01843568176755,
                                      0.987006509841325, 0.992442382730431, 1.40011187765134, 1.10049992999149,
                                      1.00818765405818, 1.09264012708042, 1.03275826754933, 1.02244175655995,
                                      1.01677349978967, 1.00730703579567, 1.02370799275158, 1.0278878933396,
                                      1.00772127095367, 0.990508223163475, 1.03654684470954, 1.01471017202778,
                                      1.0088167258068, 1.06152623365514, 0.996407694507511, 1.00990368030147,
                                      0.99308273949185, 1.01142209811066, 1.0539134257655, 0.987950550189063,
                                      0.907728737449421, 0.943380719927319, 0.997548830552401, 1.01202482727284,
                                      1.05384449132074, 1.05645115789865, 1.03638057804724, 1.02232615466474,
                                      1.02155473740135, 1.02490211259218, 0.990012139601719, 1.01550098995206,
                                      1.01716326110522, 0.999992841260911, 0.986268683282699, 0.978006469199462,
                                      0.917672222079992, 0.969935271795464, 0.998608773301736, 1.05837999186784,
                                      1.04805554788864, 0.992844669993423, 1.04805961922644, 1.0401407222967,
                                      1.03706234298565, 0.994711744607323, 1.02242553387652, 1.05703852898853,
                                      1.02679293254591, 0.997965771541219, 0.917473535443585, 1.07440126109941,
                                      0.9179177859099, 0.919502136779142, 0.909942167932662, 0.918999766735172,
                                      0.910522348305699, 0.910464673852149, 0.912132577307152, 0.93050714648443,
                                      1.00572010846845, 1.08835661908728, 1.02485882059796, 0.97169352444713,
                                      1.01819935716157, 1.01659396218221, 1.03144898760036, 1.03231009036758,
                                      1.01914297615352, 1.01787730424132, 0.997666518610012, 1.01393686117525,
                                      1.00294635134308, 1.0021467434977, 1.00869946257037, 1.00555026737276,
                                      1.01266810889103, 1.00873984914465, 1.02591321937449, 1.02496201058778,
                                      1.02528139325692, 1.02460875190955, 1.02663131819817, 1.02416011283716,
                                      1.02458999944919, 1.02348446671238, 1.02255006626692, 1.02385243807688,
                                      1.02188695650639, 1.01043176512904, 1.0172412278076, 1.01987004417504,
                                      1.01676244500535, 1.00682288143374, 1.05321191441119, 1.01863892284441,
                                      1.01248908601116, 1.00547415443856, 1.01123942242253, 1.01680586420231,
                                      1.03010782551809, 1.01924245993494, 1.01831205835725, 1.02548033237273,
                                      1.02289365756085, 1.02371720873201, 0.986314878675544, 1.07739563906159,
                                      1.13550827650651, 0.984131104740498, 1.00511227457725, 1.03979805401024,
                                      1.00417834329961, 1.04919633441873, 1.02556187476117, 0.830356042270722,
                                      1.17315491931469, 1.0543865934696, 1.13233449365852, 1.00637856607788,
                                      1.04775993890215, 1.01967163064842, 1.01978380031208, 1.04436595858624,
                                      1.01983968248389, 1.02505016634166, 1.02656653835816, 1.034627763313,
                                      0.989299875613032, 1.02585095018446, 1.10027124581792, 1.09195808084766,
                                      1.02460622845342, 1.01900743859982, 1.03032532509538, 1.0306366110151,
                                      0.880304188387915, 1.15984128162327, 1.10595698201519, 1.06813627713607,
                                      1.02820004589795, 1.02361660037235, 1.0803024967394, 1.05184436013119,
                                      1.04472723628786, 1.03307585256408, 1.02530302082136, 1.00960641386819,
                                      1.02179211945921, 0.990054264651694, 1.06472264262881, 1.16968066582964,
                                      0.986615526491474, 1.04111572963718, 1.06399397734115, 1.0450685732614,
                                      1.03482624048209, 1.04021515242698, 1.03128424624614, 1.05267584553694,
                                      1.04270356878724, 1.01139831983517, 1.03020002654948, 1.03292606855417,
                                      1.03947484801557, 1.03638732091419, 1.03341383505357, 1.04909382568328,
                                      1.03497153379646, 1.03071447248799, 1.0386730364669, 1.05707580701379,
                                      1.02772478887651, 1.02579090605751, 1.0334458857475, 1.02644426175172,
                                      1.04041977201228, 1.05220239899258, 0.982323748565849, 1.19614870481645,
                                      1.078992874894, 1.11539083620872, 1.07049299839616, 1.02165436872218,
                                      1.01228821688063, 1.10580759713596, 1.1052177546795, 1.11638830932239,
                                      1.14453234488613, 0.987339595056673, 1.02901061204467, 1.01526022752835,
                                      1.00123802587661, 1.12235861581172, 1.05216758650576, 1.10676159715688,
                                      1.00973459626533, 1.0514324448342, 1.02157182411899, 0.894341491975593,
                                      1.14087655654238, 1.11351132389876, 1.11777287614524, 0.982691354651264,
                                      1.03240303896469, 1.02118760452149, 1.05164409953533, 1.08174251820789,
                                      1.04039517049471, 1.08809992133257, 1.00202510172505, 1.02464458567167,
                                      1.01945613860744, 1.02088257891634, 1.01783683581512, 1.04304351714748,
                                      1.02094864018209, 1.02634084183707, 1.03327688876985, 1.02395773970426,
                                      1.04332858414669, 1.01725581155337, 1.04294188054171, 1.03407958307694,
                                      1.04603911382918, 1.02759592744356, 1.04970096695316, 1.04613870803068,
                                      1.02836488546049, 1.01965260316049, 1.0458391297722, 1.02250004301883,
                                      1.02438877458349, 1.06622098982581, 1.02949667426697, 1.03328360312094,
                                      1.01716108550373, 1.04733749019038, 1.03391921717942, 1.05498320983952,
                                      1.02993198768164, 1.03778464709126, 1.02674287242063, 1.03488151087686,
                                      1.03493094075973, 1.02409557106704, 1.05357049056109, 1.05123497176824,
                                      1.05215044947753, 1.02248250030939, 1.02401783330703, 1.03845737316818,
                                      1.01951784815085, 1.03495789189137, 1.03036353007423, 1.0516515136974,
                                      1.04027092123822, 1.02680237975708, 1.05024525294763, 1.03869850220381,
                                      1.05135737460624, 1.03347260560675, 1.03304292606522, 1.04190995330257,
                                      1.01660580299208, 1.02813344962141, 1.02012976473574, 1.04302869654736,
                                      1.01621684578151, 1.03889675169637, 1.02194628969237, 1.05076798102126,
                                      1.03036964752211, 1.0325016863085, 1.03232932131755, 1.02067521256949,
                                      1.03809808000477, 1.03657441625044, 1.02807937468182, 1.01573919272889,
                                      1.02495999408312, 1.02119683825215, 1.03867831113724, 1.02919776212525,
                                      1.0413093599632, 1.02954689062779, 1.05787119941651, 1.04820300298612,
                                      1.10660129319641, 0.981080593186796, 1.05867499557928, 1.05541062633835,
                                      1.10864256362621, 0.99560971407142, 1.07388054136339, 1.07734769877409,
                                      0.912792524986366, 1.12851761966775, 0.955432145555799, 1.04938339708609,
                                      1.01865808452961, 1.03548258544447, 1.07378758976732, 0.93655045048777,
                                      1.08904994117365, 0.999179282422365, 1.06572765363337, 1.01066818582444,
                                      1.04343989435533, 1.03645749977744, 1.01022892250234, 1.082996905233,
                                      0.978649644877084, 1.12580559089988, 0.980800170439686, 1.03239715443664,
                                      1.05783325868668, 1.01964488633555, 1.00028466887619, 1.06041018669072,
                                      1.0256613966458, 1.10131329865828, 0.961605882972882, 1.11163175270576,
                                      0.945931570363018, 1.09013046116144, 1.00442669311866, 1.10802710792882,
                                      0.976516476523375, 1.09368042525757, 1.0530199661453, 1.09077509595525,
                                      1.0599438125539, 1.06902206146674, 0.977745950966265, 1.06893365165991,
                                      1.05678902974228, 1.04749891166018, 1.09073047295974, 0.979936540273519,
                                      1.05080226068183, 1.08825025235009, 1.01212928970529, 1.02164895394523,
                                      1.04111844128208, 0.970381063240161, 1.14132925446944, 0.976963497130447,
                                      1.0603556489753, 1.05269597789572, 0.996722926858579, 1.07624131724596,
                                      1.0401820407265, 1.05098140799356, 1.09195786858085, 0.98187687762382,
                                      1.03908476052528, 1.10369924333789, 0.958769236485961, 1.02119099058496,
                                      1.0321245367865, 1.03309951346348, 0.802882777728048, 1.07490325693652,
                                      0.881106410971995, 0.942183020936798, 1.01842412804306, 0.959242030358977,
                                      0.866635949487715, 1.00991004247993, 0.866778978444599, 0.956004714986372,
                                      1.03705769507871, 1.02145160754755, 1.02066031052491, 1.02070053322264,
                                      1.02291625399304, 1.02304377781658, 1.02155413325955, 1.04317761078525,
                                      1.01102825587469, 1.15209881981748, 1.01535439815813, 1.05859601588101,
                                      1.11106902299906, 1.03645985779166, 1.02211189714157, 1.04229955676201,
                                      1.01741004413468, 1.05658502388074, 1.09508727960744, 1.01039569049483,
                                      1.1162598749492, 1.1182409539066, -1.66030534351145, -2.27709923664122,
                                      -1.99274809160305, -8.41717557251908, -19.1416030534351, 26.3503816793893,
                                      -2.54999999999999, 10.9637404580153, 19.7057251908397, -2.07175572519084,
                                      -0.647328244274805, 2.04274809160305, -0.429007633587788, -0.834351145038169,
                                      -2.27442748091603, -2.86679389312977, -1.2324427480916, -2.57480916030535,
                                      1.15267175572519, -1.49503816793893, 1.68473282442748, -58.0312977099237,
                                      -17.8889312977099, 2.47824427480916, -5.67442748091604, -3.93358778625959,
                                      -0.987786259542033, -6.87595419847329, -5.69618320610687, -19.6568702290076,
                                      2.92671755725192, -1.65725190839695, 0.439312977099245, 8.80419847328244,
                                      -7.08625954198474, 0.328625954198505, -0.404961832061072, 2.56717557251909,
                                      -2.95954198473283, -1.14007633587784, 12.0099236641221, -3.21335877862595,
                                      -10.8049618320611, 24.2083969465649, 36.4187022900764, -1.76793893129771,
                                      -2.22977099236641, -1.9412213740458, 3.55992366412214, -75.5351145038168,
                                      -32.0942748091604, 1.25496183206105, -1.76984732824427, -2.89427480916032,
                                      10.6759541984733, -14.6675572519084, 4.43015267175572, 10.5519083969466,
                                      3.64007633587786, 29.1824427480916, 14.0687022900763, -0.496564885496181,
                                      -17.8912213740458, 14.6572519083969, -3.21870229007633, -7.43015267175573,
                                      -8.8118320610687, 3.62022900763359, -61.5625954198473, -32.812213740458,
                                      -4.29656488549619, 15.1835877862595, -5.83549618320611, -7.81717557251909,
                                      -1.48244274809161, -6.19694656488549, -43.2041984732825, -5.04236641221374,
                                      9.29770992366413, 1.96870229007634, 24.1114503816793, 6.55038167938934,
                                      -5.23091603053435, -6.12557251908396, -10.0763358778626, -1.95343511450382,
                                      -1.65, -1.91641221374046, 30.6351145038168, -5.05152671755725,
                                      -2.71717557251908, -5.7026717557252, 4.57938931297709, -4.41603053435113,
                                      9.38778625954194, 0.653816793893127, -4.67175572519085, 2.60839694656489,
                                      -2.52213740458015, -3.22977099236641, -1.74083969465648, -2.56030534351146,
                                      -0.9824427480916, -0.95114503816794, 0.0488549618320674, -1.65534351145038,
                                      -2.33320610687023, -4.63473282442749, -10.8515267175573, -0.707251908396955,
                                      -4.2206106870229, 1.45267175572519, -1.17748091603053, -4.68511450381679,
                                      -2.94389312977099, -16.5568702290076, 11.3045801526718, -2.28854961832062,
                                      -8.47404580152672, 0.54427480916029, 18.5732824427481, -5.01984732824428,
                                      -1.89007633587786, -16.0473282442748, 8.29961832061069, 12.3770992366412,
                                      56.8446564885497, -35.0152671755725, -34.8251908396946, -2.97824427480917,
                                      2.94694656488548, -12.6965648854962, 1.36641221374046, -1.20992366412214,
                                      -3.02900763358778, 15.4263358778626, -6.48816793893131, -11.4484732824428,
                                      6.85114503816794, 0.889694656488551, 12.6293893129771, 37.2171755725191,
                                      -2.43396946564886, -4.8618320610687, -71.3568702290077, -20.4244274809161,
                                      20.1522900763359, -9.20763358778626, -8.38511450381679, -10.8698473282443,
                                      15.1492366412214, 20.7412213740458, -24.669465648855, -17.2175572519084,
                                      22.9729007633588, 0.6118320610687, 29.5969465648855, 33.5240458015267,
                                      -2.2263358778626, -1.79274809160305, -3.96259541984733, 3.53167938931298,
                                      -0.718702290076336, -2.34847328244275, -0.477480916030534, -47.0335877862595,
                                      -32.8309160305344, -4.93816793893125, 16.2328244274809, -6.78358778625954,
                                      -7.28587786259542, -12.4167938931298, 9.25648854961832, -3.45343511450381,
                                      -7.48282442748092, 1.09274809160306, -1.53206106870229, -6.16297709923665,
                                      2.19923664122137, -2.65267175572519, -1.68015267175573, -0.706488549618316,
                                      -3.54351145038168, -4.18015267175573, -8.26908396946565, -3.18053435114504,
                                      -1.46641221374046, -1.26068702290076, -1.77404580152672, -0.750763358778626,
                                      -1.16335877862595, -2.05648854961832, -1.8881679389313, -1.43320610687023,
                                      0.725954198473285, -0.25114503816794, -0.524427480916037, 0.543129770992371,
                                      -3.18053435114504, -3.73129770992366, -11.3809160305343, 11.8996183206107,
                                      -2.09351145038168, 0.293129770992368, -7.58129770992367, -4.85992366412214,
                                      -2.34503816793893, 9.2267175572519, -10.6385496183206, -4.39732824427482,
                                      1.03282442748091, 31.3103053435114, -25.4080152671756, -44.2656488549618,
                                      30.9893129770992, 5.99274809160306, -2.59045801526718, -5.08969465648856,
                                      -2.26832061068702, -3.75229007633588, -0.433206106870236, 1.54312977099237,
                                      -44.1606870229008, -34.3706106870229, 9.26870229007632, 18.4942748091603,
                                      1.20954198473283, -2.76603053435114, -0.163358778625958, -3.30267175572519,
                                      1.05763358778626, -0.686641221374044, -4.2706106870229, -3.95725190839694,
                                      21.1725190839694, -47.4660305343512, 1.58740458015268, 36.0900763358779,
                                      2.56145038167939, 4.91641221374045, -4.68473282442748, -3.31603053435115,
                                      -0.870610687022897, -35.2874045801527, 14.8236641221374, 29.3,
                                      0.601908396946574, -0.00267175572518283, 5.30419847328244, -5.72404580152672,
                                      -1.36946564885496, 5.51297709923664, 1.68625954198474, -7.89541984732825,
                                      -4.99236641221374, -8.96793893129769, -62.3400763358779, 59.685496183206,
                                      11.8576335877863, -1.45267175572519, -4.39847328244275, -0.869465648854969,
                                      3.57442748091602, -5.92022900763359, 1.78931297709923, -7.2293893129771,
                                      6.90534351145037, -3.50114503816793, -1.53740458015268, -4.93549618320611,
                                      -2.08435114503817, 0.484351145038166, 2.66641221374046, -7.1881679389313,
                                      4.25610687022901, -2.25343511450382, -0.996946564885497, 4.62595419847329,
                                      -1.43320610687024, -0.603435114503817, -1.6763358778626, -2.41832061068702,
                                      -12.0419847328244, 1.56641221374046, -32.5549618320611, -56.4110687022902,
                                      -2.26221374045803, 19.0908396946565, 15.7503816793893, 8.99160305343511,
                                      -14.4618320610687, -35.1400763358779, -6.71641221374045, -6.10877862595421,
                                      17.1774809160305, 25.5362595419847, 1.87213740458015, -14.5232824427481,
                                      -22.7324427480916, -2.04007633587785, 32.5263358778626, -9.75687022900766,
                                      15.5267175572519, -9.28473282442748, 6.09923664122137, -39.6980916030535,
                                      -6.63854961832059, -6.55343511450382, 12.574427480916, 31.6996183206107,
                                      5.22213740458016, -7.0118320610687, -52.9900763358779, -15.1320610687023,
                                      45.8526717557252, -0.0732824427481965, 8.98053435114504, -3.5324427480916,
                                      -1.23740458015267, -2.31297709923664, -3.94045801526717, 3.15038167938932,
                                      -2.62671755725191, -8.22977099236641, -2.31832061068702, -2.25839694656489,
                                      -7.27442748091603, 5.09961832061068, 1.06259541984733, -6.88053435114503,
                                      -0.8942748091603, 1.88091603053435, -2.97709923664122, -2.93320610687022,
                                      -0.817557251908388, 0.685114503816795, 2.48778625954198, -0.483206106870229,
                                      -9.29045801526718, 1.15190839694657, -1.24580152671756, -3.3030534351145,
                                      0.261832061068706, -5.58091603053435, 0.940839694656493, -1.46374045801527,
                                      -0.0232824427480828, -3.25877862595419, -0.756106870229004,
                                      0.333206106870234, 5.46450381679389, -14.0568702290076, 3.05038167938931,
                                      0.418702290076332, -7.64732824427481, -4.95076335877864, -0.162213740458011,
                                      7.90534351145038, 0.332824427480913, -6.14007633587787, -4.03358778625954,
                                      2.63091603053435, 1.0473282442748, -8.64236641221375, -8.3557251908397,
                                      5.27557251908396, 5.54541984732825, -5.13511450381679, -12.2610687022901,
                                      4.77709923664122, 1.47671755725191, 1.90190839694657, -6.4854961832061,
                                      -4.69732824427482, 0.353435114503815, -0.525190839694658, -7.68702290076336,
                                      -7.28931297709923, 8.26946564885496, -1.17748091603053, 2.87824427480915,
                                      -2.38396946564886, -4.79007633587786, 0.227099236641224, -3.62862595419848,
                                      -3.69274809160304, -4.35648854961831, 1.85801526717558, 3.00343511450381,
                                      -10.2919847328244, -11.0427480916031, 4.79083969465649, -0.827480916030531,
                                      0.313358778625956, -1.05305343511451, -4.65038167938932, 3.55076335877863,
                                      -4.08473282442748, -6.26297709923668, 5.24274809160305, -13.4618320610687,
                                      5.13282442748091, 3.24045801526718, -14.0927480916031, 2.25229007633588,
                                      4.31183206106869, -5.88435114503817, 4.85458015267175, -2.26793893129771,
                                      -1.05305343511451, 3.79160305343511, 3.85458015267175, -7.60648854961833,
                                      0.52290076335879, 0.188549618320611, -3.60839694656488, -8.62175572519082,
                                      -3.04083969465648, -1.80916030534351, -4.88435114503817, 5.056106870229,
                                      -8.93549618320611, 4.34274809160306, 4.34580152671756, -5.76832061068702,
                                      0.0213740458015261, -1.9087786259542, -4.9293893129771, 8.04427480916031,
                                      -3.16564885496183, -8.90801526717557, 2.81603053435114, 1.28091603053436,
                                      -8.34541984732824, 6.07061068702291, -12.7828244274809, -38.7606870229007,
                                      8.72938931297712, 20.9580152671756, 5.9587786259542, -19.8599236641221,
                                      5.50114503816794, 15.3572519083969, -25.8, 0.625190839694653,
                                      19.7198473282443, -15.8530534351145, 0.0236641221374033, 3.57824427480916,
                                      -11.4221374045801, 17.4309160305344, -24.4274809160305, -0.920992366412224,
                                      13.6209923664122, -15.0164122137405, 19.7889312977099, -18.1419847328244,
                                      -2.91145038167938, 15.1011450381679, -28.6931297709924, 2.89007633587787,
                                      19.769465648855, -19.7625954198473, 5.67022900763358, 9.0851145038168,
                                      -2.26221374045801, 0.271755725190842, -6.8358778625954, -1.04045801526717,
                                      -4.01488549618321, -8.92900763358778, 3.24847328244275, -1.4083969465649,
                                      -11.9874045801527, 2.93167938931298, -3.70458015267177, -6.35916030534351,
                                      3.78167938931298, -6.21908396946567, -1.5469465648855, -2.15343511450382,
                                      -2.62519083969466, -2.25343511450382, -1.51641221374046, -3.21412213740458,
                                      -11.5221374045802, -26.9702290076335, 44.0091603053436, -2.16832061068702,
                                      -51.4862595419848, 27.1225190839695, 22.6606870229008, -3.88740458015268,
                                      3.87366412213741, -8.38587786259542, -41.0687022900763, 52.9564885496183,
                                      -18.5610687022901, -15.9790076335878, 18.5759541984733, -0.395038167938932,
                                      -0.0183206106870232, 1.77519083969466, 30.4774809160306, -28.7587786259542,
                                      5.72442748091603, -3.46526717557252, 4.9206106870229, -2.81870229007634,
                                      0.808396946564886, 0.493893129770991, -0.0129770992366411, -1.33129770992366,
                                      0.357251908396947, -0.248854961832061, -0.813740458015267,
                                      -0.664885496183206, -0.432442748091604, 0.0423664122137394,
                                      -0.493511450381679, -1.8, -10.9515267175572, 2.13358778625955,
                                      7.55610687022901, 11.3484732824427, -22.7717557251908, 20.4946564885496,
                                      11.5187022900763, 16.513358778626, 41.6862595419847, 12.4706106870229,
                                      0.507633587786259, -28.6515267175573, -54.3068702290076, 12.9828244274809,
                                      50.0404580152672, -1.26832061068702, -17.6049618320611, -39.1553435114503,
                                      -22.2290076335878, 2.5175572519084, 3.11030534351145, 11.9893129770992,
                                      -12.9809160305344, 9.793893129771, -0.212977099236643, -1.91374045801527,
                                      0.372137404580155, -0.847328244274809, -13.8496183206107, 2.59427480916031,
                                      100.924045801527, -40.093893129771, 2.68206106870227, -72.8003816793893,
                                      28.2671755725191, -10.6881679389313, -13.919465648855, 2.01793893129771,
                                      6.1206106870229, 7.85419847328244, 4.07557251908397, 39.4396946564886,
                                      -42.9652671755726, -3.28091603053436, 0.733206106870229, -0.604961832061069,
                                      0.499236641221379, -13.0606870229008, 22.567175572519, 27.6477099236641,
                                      -57.1103053435114, 3.23969465648855, 19.7477099236641, 5.85114503816794,
                                      9.00419847328245, 8.77290076335876, -9.42633587786259, 14.5969465648855,
                                      -7.15229007633589, -0.7912213740458, 20.1374045801527, -2.87099236641222,
                                      -12.4996183206107, 2.84618320610687, -1.73282442748091, -2.09809160305344,
                                      -3.24045801526718, -59.6343511450382, 0.713740458015269, 3.73625954198473,
                                      8.83091603053436, -4.3530534351145, 1.30916030534352, 5.7732824427481,
                                      -9.44770992366413, 3.49236641221374, 2.36259541984733, 5.83511450381679,
                                      -1.34274809160305, 2.38473282442749, -4.13167938931298, -7.76412213740458,
                                      -18.4099236641221, -16.8854961832061, -7.90992366412215, -0.911068702290076,
                                      10.5251908396947, 46.8732824427481, 5.06870229007634, -0.824045801526715,
                                      1.85801526717557, 1.43435114503817, -2.4087786259542, -0.751908396946564,
                                      12.6026717557252, -16.0290076335878, -0.97824427480916, 21.5187022900763,
                                      6.66526717557253, -28.3351145038168, -0.283969465648857, -1.40267175572519,
                                      49.9916030534351, -45.9480916030534, -0.291221374045806, 3.37862595419848,
                                      -3.04122137404581, 3.36145038167939, 40.007251908397, -49.9782442748092,
                                      10.6354961832061, 38.231679389313, -2.86908396946566, -12.4511450381679,
                                      -29.6969465648855, -0.903435114503815, 42.0416030534352, -42.2580152671756,
                                      -1.65343511450382, 3.25992366412214, 33.7118320610687, 1.81488549618321,
                                      0.291221374045798, -6.03206106870229, 4.71793893129771, -4.6297709923664,
                                      93.8354961832061, -7.71106870229009, -37.3354961832061, -50.1702290076336,
                                      1.79045801526717, 47.7656488549618, -23.8297709923664, -20.0709923664122,
                                      -1.9706106870229, 6.35992366412214, 2.13129770992365, -0.553053435114504,
                                      -0.456870229007634, -0.203435114503817, -0.554198473282442,
                                      -0.529389312977101, 0.3469465648855, 0.659541984732824, -6.50801526717558,
                                      -5.92290076335878, -12.6721374045802, 61.0221374045801, 36.9820610687023,
                                      16.6583969465649, 2.4881679389313, -32.9435114503817, 21.0202290076336,
                                      43.4030534351145, -16.6187022900763, -0.933587786259546, -7.17480916030534,
                                      -1.82251908396947, 2.64885496183207, 1.22137404580152, -17.5679389312977,
                                      -15.8538167938931, -18.6515267175573, 20.7580152671756, -2.84198473282443,
                                      0.266793893129771, -0.933969465648855, -0.638931297709923,
                                      -0.769847328244275, 0.37175572519084, -0.266793893129771, -0.822519083969466,
                                      0.316412213740458, -1.66297709923664, -10.6103053435115, -11.8492366412214,
                                      4.05496183206108, 0.59618320610687, -1.36221374045802, 9.25992366412214,
                                      -16.680534351145, 10.9522900763359, 18.2637404580153, 6.03206106870229,
                                      -43.6591603053435, 15.2889312977099, -5.63511450381679, -1.04427480916031,
                                      -3.63053435114503, -0.927099236641221, -13.8641221374046, 13.9469465648855,
                                      51.906106870229, -12.7396946564886, -0.513740458015263, 3.28053435114504,
                                      -15.2629770992366, 0.329389312977094, 0.544656488549614, 0.350381679389313,
                                      -4.31335877862596, -8.71755725190838, 47.5984732824428, -6.63091603053435,
                                      -12.4980916030534, 3.47328244274809, 26.5248091603053, -11.5393129770992,
                                      -16.2167938931298, 2.92442748091603, 0.862977099236647, -1.55801526717557,
                                      -3.87824427480916, -22.8973282442748, 17.613358778626, 33.2160305343511,
                                      -9.58625954198474, -10.5408396946565, 2.59160305343511, -3.9324427480916,
                                      -0.0706106870229023, -9.93167938931298, -4.95572519083968, 30.1534351145038,
                                      4.68625954198474, 41.7545801526718, -31.5335877862596, -11.5522900763359,
                                      -16.2473282442748, -5.27862595419848, 5.95801526717558, -7.87671755725191,
                                      3.7706106870229, 0.0499999999999998, -39.3232824427481, 39.1977099236642,
                                      19.6064885496183, 7.3912213740458, -2.76908396946565, -10.6942748091603,
                                      9.91412213740457, -5.93702290076335, -20.7080152671756, 47.3538167938932,
                                      -5.2885496183206, 2.72595419847329, -0.253435114503817, 0.415648854961833,
                                      -0.261832061068702, -2.56297709923664, 2.01374045801527, 4.89732824427482,
                                      6.58702290076336, 7.14580152671756, -4.69732824427481, 0.869083969465644,
                                      -1.72633587786259, -8.6354961832061, -18.4301526717557, 12.7297709923664,
                                      -0.548091603053429, -11.4988549618321, -1.78549618320611, -1.94427480916031,
                                      -14.6492366412214, 1.40725190839695, 3.4404580152671, -0.0194656488549635,
                                      4.00992366412214, -1.70839694656489, -0.393511450381684, -2.24923664122139,
                                      6.87290076335881, -9.82290076335878, -4.29541984732825, 1.19427480916031,
                                      -1.6293893129771, 5.42137404580153, 1.24847328244275, 1.92022900763358,
                                      -5.6263358778626, 5.73473282442748, -0.595419847328247, -0.58053435114504,
                                      -6.20992366412214, -1.44770992366411, 4.37137404580152, -1.77175572519084,
                                      3.60801526717557, -0.0255725190839703, -1.70763358778626, -6.14847328244275,
                                      4.59198473282445, -3.19618320610681, -5.63702290076336, 3.6969465648855,
                                      -1.56068702290076, -0.812213740458015, -0.135114503816794, -2.71221374045801,
                                      -18.3828244274809, 5.95343511450383, 18.3286259541985, -8.54274809160306,
                                      1.52862595419846, 8.98282442748092, -3.03358778625954, -13.9007633587786,
                                      6.82824427480917, 4.69198473282443, -1.99427480916031, 3.75229007633587,
                                      -4.06984732824428, 3.61946564885496, -0.700763358778629, -4.69351145038168,
                                      0.109541984732824, -2.18740458015267, 3.03053435114503, -7.18015267175572,
                                      7.33358778625954, -4.02213740458016, 2.32290076335877, 3.59503816793894,
                                      -1.36641221374046, -5.65534351145037, -0.54847328244275, 1.81221374045802,
                                      -2.40916030534351, -1.99160305343511, 2.73511450381679, -0.825954198473291,
                                      -9.1145038167939, 4.26946564885497, 1.61374045801527, 3.16793893129771,
                                      0.595801526717557, -1.18664122137405, 3.52519083969465, -3.22977099236641,
                                      -3.59465648854962, -6.97404580152672, 7.10190839694656, 1.96259541984732,
                                      -7.36946564885497, -9.03396946564886, 10.8229007633588, 6.68244274809159,
                                      -1.53435114503817, -20.9778625954198, 18.1499999999999, -0.38549618320611,
                                      -2.68854961832061, 1.88282442748091, -2.9145038167939, 2.66793893129772,
                                      4.58167938931298, -6.59465648854963, -4.74580152671756, 6.83167938931297,
                                      -0.945801526717557, -1.24198473282442, 7.77519083969465, -9.77824427480915,
                                      -1.45916030534351, 7.86679389312978, -10.7988549618321, -4.84198473282443,
                                      9.54923664122138, 5.15534351145039, -2.20114503816793, -8.52442748091603,
                                      2.69809160305343, 9.51870229007634, -11.3171755725191, 1.21335877862595,
                                      2.70801526717557, 5.76335877862596, -1.46335877862595, -10.3301526717557,
                                      11.5977099236641, 0.779770992366412, -8.99503816793893, 3.86068702290078,
                                      5.15610687022901, -3.00305343511451, -1.36259541984733, 2.33549618320611,
                                      2.15267175572519, -7.59083969465649, -6.64999999999999, 9.48625954198473,
                                      -13.5057251908397, 1.18282442748091, 0.993893129770985, -0.803816793893127,
                                      -0.330152671755723, -8.97213740458016, 12.4954198473282, 1.79503816793893,
                                      0.779007633587801, -4.96946564885496, 7.5526717557252, 5.44389312977099,
                                      -13.8904580152672, 8.09045801526717, -7.57137404580152, -3.49541984732825,
                                      -1.23053435114503, -4.78931297709923, 17.0969465648855, 3.65229007633589,
                                      -17.3099236641221, -1.88625954198473, -5.98053435114504, 4.84236641221375,
                                      -2.36412213740458, 7.5881679389313, 0.643129770992366, -0.411832061068705,
                                      0.547328244274808, 2.38435114503817, -1.80305343511451, -0.0179389312977091,
                                      1.44961832061069, 0.395419847328243, -2.29770992366412, -11.5114503816794,
                                      11.1717557251908, 0.607633587786257, -2.59427480916031, -1.10725190839695,
                                      1.62709923664122, 0.169083969465655, 1.52328244274809, -0.831679389312977,
                                      -0.187786259541987, -0.680534351145038, -2.55992366412214, 0.870610687022903,
                                      0.5293893129771, 0.125572519083972, -0.171755725190837, 0.556106870229007,
                                      -2.00496183206107, -8.53206106870229, -2.08473282442749, 0.639312977099239,
                                      1.24732824427481, -3.92022900763358, 3.84045801526718, -1.16183206106871,
                                      -4.00114503816794, 2.64198473282443, -0.989694656488551, -2.03091603053435,
                                      2.29427480916031, -4.52328244274809, 0.549618320610688, 0.232824427480916,
                                      0.390076335877862, -0.59732824427481, -0.409541984732825, 1.40343511450382,
                                      -9.32099236641221, -37.1606870229007, -40.1503816793893, 2.4206106870229,
                                      30.1416030534351, 38.4213740458015, 8.12442748091603, 7.33358778625954,
                                      13.2522900763359, -32.5736641221374, -33.1282442748092, -61.3419847328245,
                                      8.40801526717557, 34.6545801526718, 30.7541984732824, 0.542366412213741,
                                      -0.0137404580152673, -0.144656488549618, -7.15114503816794,
                                      -21.6541984732825, 2.68435114503817, 3.25229007633588, 0.861832061068701,
                                      -7.55572519083969, 0.34618320610687, 0.201526717557253, 0.413740458015267,
                                      1.24465648854962, 0.645038167938932, 0.296564885496183, 0.693893129770993,
                                      0.599618320610687, 0.151526717557252, 1.37251908396947, -0.206488549618319,
                                      1.76946564885496, -4.65229007633588, -1.51145038167939, -13.9843511450382,
                                      2.25229007633588, 14.7690839694657, 3.8, 0.183969465648863,
                                      0.200763358778628, 4.88053435114504, -3.89160305343511, 2.60114503816794,
                                      6.67442748091604, 2.59083969465649, 9.16374045801526, 3.98053435114504,
                                      1.41335877862596, -0.0423664122137405, 7.34198473282443, -16.206106870229,
                                      0.451145038167934, 0.846946564885495, -2.13015267175573, 0.123664122137405,
                                      12.8591603053435, 0.122519083969465, 3.92977099236641, -2.04007633587786,
                                      -1.17022900763359, -7.61641221374045, -5.24541984732825, -12.2843511450382,
                                      7.29465648854962, 1.16603053435114, 1.60152671755725, 4.44770992366412,
                                      2.09656488549618, -3.08740458015267, 1.37824427480916, 1.08435114503817,
                                      7.58740458015265, 0.370229007633586, -13.6793893129771, 16.206106870229,
                                      -1.08893129770992, 2.59312977099237, -1.50687022900763, 2.50114503816794,
                                      -5.39541984732825, 11.9652671755725, 10.5660305343511, 6.97595419847329,
                                      -0.425954198473283, -5.39236641221375, 1.49618320610687, -2.62213740458015,
                                      10.8538167938931, -2.85038167938931, -9.86335877862596, -3.73625954198473,
                                      2.67442748091603, 4.75419847328244, -0.842366412213742, -2.69885496183207,
                                      -3.41412213740458, 0.11526717557252, 0.155343511450381, -0.889694656488549,
                                      -5.80190839694657, 0.766030534351147, 1.28435114503817, 0.131679389312978,
                                      3.53129770992366, -1.53396946564885, 5.8824427480916, 0.326717557251909,
                                      4.46793893129771, 0.92557251908397, 0.976335877862593, 0.0801526717557254,
                                      0.653435114503818, -1.72442748091603, 5.8732824427481, 4.52862595419847,
                                      3.61908396946565, 1.03282442748092, 0.49351145038168, 1.36793893129771,
                                      -11.2942748091603, 3.88664122137405, -3.4030534351145, -1.2824427480916,
                                      1.74274809160305, 1.83740458015267, -1.41641221374046, -1.66488549618321,
                                      4.91030534351145, 2.09961832061069, 4.56335877862595, -2.8263358778626,
                                      1.65190839694657, 0.505343511450382, 0.160687022900763, -4.0851145038168,
                                      -5.57480916030534, 2.54007633587786, 14.7770992366412, -6.84351145038168,
                                      -4.99122137404579, -1.94999999999999, 2.06641221374045, -0.0824427480915986,
                                      5.48969465648856, 0.728244274809161, -1.95687022900763, -0.909923664122132,
                                      -1.18435114503817, -2.28969465648855, 2.99198473282443, 0.484351145038168,
                                      1.24465648854962, 8.52404580152672, 3.14885496183207, 1.00839694656489,
                                      -18.3770992366412, -3.75725190839695, 6.71450381679389, -25.4267175572519,
                                      0.781679389312973, -7.17099236641221, 2.29618320610687, 1.64618320610687,
                                      -5.63549618320611, -3.99083969465649, -4.68129770992367, 0.216793893129772,
                                      4.51717557251909, 9.27862595419847, 0.476717557251908, 0.599618320610687,
                                      0.340076335877862, 0.927099236641222, 0.596946564885496, 1.0557251908397,
                                      0.500381679389313, -9.00381679389314, 0.13206106870229, 1.16832061068703,
                                      -21.0030534351145, -16.4465648854962, -3.18702290076336, -8.70190839694656,
                                      -1.99885496183206, 6.04770992366413, -1.59618320610686, 4.28664122137405,
                                      1.72824427480916, 0.0515267175572538, 1.40190839694657, 0.0660305343511492,
                                      0.533206106870228, 1.51221374045801, 8.77709923664122, -1.06297709923664,
                                      -5.82137404580153, 0.0423664122137409, 0.356106870229008, 0.886641221374045,
                                      0.504961832061069, 0.422137404580153, 0.315648854961832, 0.553435114503818,
                                      0.61030534351145, 0.541603053435115, 1.80992366412214, 3.62442748091602,
                                      -4.15038167938932, 5.26183206106871, -2.14160305343512, 0.674809160305347,
                                      15.4400763358779, -14.1652671755725, 3.45801526717557, -5.20839694656489,
                                      4.86679389312977, 1.00343511450382, -5.16793893129771, -1.11297709923663,
                                      1.96870229007633, -2.71221374045802, 4.62709923664122, 65.3492366412214,
                                      -25.9076335877863, -3.03854961832061, -28.2946564885496, 2.55419847328244,
                                      0.850381679389313, 3.83015267175572, -4.38893129770993, 2.09351145038168,
                                      1.22977099236641, 49.7564885496183, -25.8125954198474, 10.95,
                                      -12.125572519084, -22.2503816793893, 1.10381679389313, 1.36679389312977,
                                      0.758396946564881, 1.92328244274809, 0.586259541984731, 0.234732824427484,
                                      -1.15152671755725, -0.383969465648861, 49.3232824427481, -13.9458015267175,
                                      32.7206106870229, -16.9320610687023, -8.23625954198473, 2.80572519083969,
                                      0.414885496183206, 0.80725190839695, 28.9068702290076, 4.83664122137402,
                                      21.5809160305343, -22.8221374045802, 14.7011450381679, -16.1057251908397,
                                      10.9637404580153, -10.4435114503817, -0.222900763358772, 1.97595419847328,
                                      0.248091603053434, 0.00801526717557138, 8.75076335877862, 28.3194656488549,
                                      6.16946564885496, 2.70610687022901, 4.85076335877863, -5.99656488549619,
                                      -6.22328244274809, 14.6751908396947, -12.3992366412214, 2.7206106870229,
                                      0.567938931297713, 4.5175572519084, -2.38053435114503, 0.516412213740458,
                                      -0.0622137404580147, 1.11221374045802, -0.311832061068708, 0.692748091603054,
                                      1.26984732824428, -7.44198473282443, 1.7148854961832, 0.445419847328244,
                                      -1.5706106870229, 2.00229007633588, 0.757251908396947, 1.55496183206107,
                                      -0.104198473282443, 0.630916030534351, 0.756106870229007, -0.694274809160305,
                                      -1.47175572519084, -3.40725190839694, -0.153816793893131, -3.09198473282443,
                                      3.58435114503816, -1.8645038167939, 1.28893129770992, -4.94580152671755,
                                      -2.97022900763358, 16.3381679389313, 6.30038167938931, 1.84503816793893,
                                      -0.935496183206105, -0.161068702290076, -0.201145038167936,
                                      0.725954198473282, 11.568320610687, 1.14351145038168, -2.88129770992367,
                                      -1.26450381679389, 3.3, -3.73206106870229, 0.325954198473287,
                                      -0.93282442748092, 7.01603053435115, 1.81374045801527, -1.06679389312978,
                                      -0.461832061068702, -0.674427480916032, -6.92061068702291, 14.0828244274809,
                                      -1.12977099236641, -2.17480916030534, 1.14083969465649, 0.584351145038168,
                                      0.544656488549619, -0.510687022900765, -1.60801526717558, 2.2824427480916,
                                      1.71717557251908, 2.6145038167939, -0.353816793893133, -1.92404580152672,
                                      2.50839694656489, 2.27290076335878, -1.71106870229007, 3.84618320610687,
                                      -2.60763358778627, -2.83282442748091, 2.83778625954199, -2.6297709923664,
                                      3.30687022900763, 2.93549618320609, -0.511450381679393, 1.13931297709923,
                                      3.3557251908397, -1.2763358778626, -0.466793893129771, 1.16297709923664,
                                      -5.06564885496184, 2.4767175572519, -1.77251908396947, 0.551908396946562,
                                      1.6851145038168, 3.03320610687022, 0.365648854961827, -3.1057251908397,
                                      3.96564885496184, -0.174427480916024, 1.29083969465648, 3.0675572519084,
                                      -2.8793893129771, 2.95839694656488, -1.05229007633588, -1.13969465648856,
                                      0.777480916030539, 5.14580152671756, -2.58015267175573, -0.361068702290074,
                                      1.15076335877864, 1.45152671755726, 1.63549618320611, 1.52748091603054,
                                      -2.15267175572519, 2.3736641221374, -0.630534351145037, -0.648854961832061,
                                      -5.9057251908397, -0.469465648854952, 2.50687022900763, 2.72442748091603,
                                      -1.02137404580153, 0.595038167938926, 0.623664122137401, 2.46946564885496,
                                      0.650763358778631, -5.84007633587786, 1.9118320610687, -0.225572519083969,
                                      -0.238167938931296, -0.95305343511451, -0.0446564885496058, 5.60114503816794,
                                      0.979770992366415, 1.4851145038168, 0.0110687022900757, 0.20267175572519,
                                      -3.04618320610687, 3.20267175572518, -3.83129770992366, -2.01717557251908,
                                      3.88931297709924, 2.28435114503816, -5.01259541984733, -1.79351145038167,
                                      4.91488549618321, 1.22900763358778, -3.00305343511451, 0.658396946564872,
                                      3.6442748091603, -0.674809160305358, -1.40610687022901, 2.77022900763358,
                                      0.943511450381685, -4.19847328244275, 0.859923664122142, 2.78778625954198,
                                      -3.19083969465649, -1.37709923664122, 2.32900763358778, 0.493511450381683,
                                      -2.90038167938932, -2.29389312977098, 5.6030534351145, -0.764122137404577,
                                      -2.19847328244274, 3.00954198473282, 0.911068702290081, 2.55190839694656,
                                      2.00152671755726, -2.79885496183206, 1.27137404580152, 1.98015267175573,
                                      -3.56068702290075, 2.20839694656488, 0.328244274809155, -1.50152671755725,
                                      -0.985877862595417, 4.30038167938932, -1.26908396946565, 1.33129770992366,
                                      -1.24770992366412, 3.89885496183207, 3.0175572519084, 0.778244274809157,
                                      0.540076335877862, -1.07137404580153, 1.63664122137404, 0.353435114503815,
                                      0.912213740458015, -0.108015267175573, 2.19961832061069, 0.635496183206107,
                                      0.342748091603054, -10.587786259542, 11.8148854961832, 0.887786259541984,
                                      -1.46564885496183, 1.05610687022901, 1.51221374045802, 1.86870229007633,
                                      1.06717557251908, -1.40839694656489, 1.87404580152672, 1.07251908396946,
                                      1.5381679389313, 0.603053435114502, 1.31564885496183, 2.22328244274809,
                                      -2.73053435114504, 2.39427480916031, 1.09656488549618, -0.147709923664129,
                                      -0.0877862595419808, 1.97633587786259, -0.0656488549618349, 3.57519083969465,
                                      -0.178625954198479, 0.69580152671756, -0.46603053435115, 1.95,
                                      -0.222900763358778, 1.0293893129771, 0.390839694656493, -0.879007633587787,
                                      0.132061068702291, 1.0175572519084, 0.817938931297711, 0.402290076335877,
                                      0.575572519083969, 0.451526717557252, -0.485877862595412, -15.4389312977099,
                                      24.3954198473283, 0.832824427480914, -5.18053435114505, 26.0786259541985,
                                      -0.25038167938932, 5.21908396946565, -0.804580152671758, 1.11679389312977,
                                      -12.3473282442748, 23.7209923664122, -15.1259541984733, 10.9343511450382,
                                      0.482061068702295, 2.30756371857987, 2.40826711341274, 3.81391715670619,
                                      76.0025316364163, 63.968965633645, 36.3185415224149, 27.2810978020505,
                                      16.9218879775225, 85.6421824981141, 20.8114565525957, 8.718047765759,
                                      8.06730101632763, 8.37577198026754, 5.26938812596044, 2.76413517052878,
                                      4.84806858818721, 4.52482787820472, 11.3174017627839, 10.8547353356229,
                                      5.1440642178733, 7.67479186815294, 61.0438684691319, 47.0992587475193,
                                      35.6964441113757, 41.8343987379484, 93.7027604198908, 99.3646169287847,
                                      61.2568952344377, 52.2032817987582, 73.3572535795942, 43.7718608490576,
                                      24.0012029190269, 41.9582850887752, 60.7154251341841, 43.1140634634354,
                                      64.6861505678767, 14.4857279028118, 20.4815140009281, 96.1630134570351,
                                      59.9931926384735, 27.4407849988381, 6.78200891597099, 39.8950949432553,
                                      47.685584793568, 91.1772414853209, 4.15616681832455, 8.68926148890656,
                                      23.3590981263096, 16.4040875710351, 80.2256309811269, 93.1587461846912,
                                      110.055932845224, 75.5417093607329, 92.7300941731158, 79.3410609610416,
                                      77.2848853466952, 40.4805555633773, 58.231217875449, 8.50910848216343,
                                      32.2971567064403, 59.0983839245266, 11.6880395901734, 47.213113501785,
                                      85.8398044692426, 14.0080722134841, 17.2402729595588, 23.6919906067259,
                                      18.1999913536849, 79.7871485127519, 104.405954826834, 86.2367944872042,
                                      66.1550660962809, 10.7345792683443, 24.3606172042661, 13.0332831792338,
                                      23.038736941686, 76.2188527466195, 15.8456071595129, 37.1317173009255,
                                      22.5351238215028, 67.930488878637, 102.461543533612, 62.3921019597442,
                                      50.7190756306768, 17.6948171182825, 7.41862562344111, 17.6426850341928,
                                      13.5530536841065, 70.7478189718383, 15.2240414650977, 17.6845918269947,
                                      24.158559979729, 12.8715706553796, 74.4739870605064, 39.5038649172236,
                                      20.6223193485846, 22.8886761960998, 10.8995031800423, 23.7540533263509,
                                      9.71002666021911, 25.3717881165956, 29.6319433383186, 18.4346313795685,
                                      23.858709039124, 48.6056213263839, 16.8995698602306, 4.65288250298453,
                                      25.1687290114514, 97.2292364448988, 21.0838748775583, 12.0257873771963,
                                      15.8602420358623, 9.50278517940029, 19.2599885187252, 8.9972755142107,
                                      25.076868069028, 34.3805831834182, 22.5451231602411, 41.2542833476543,
                                      63.0451986198189, 66.2579949827486, 21.2813616717112, 5.47097938546294,
                                      60.1001064457784, 54.8408740541914, 14.5707599083785, 85.1594125462449,
                                      40.9917171037571, 68.6483621046362, 52.263217828547, 73.1242044610542,
                                      73.4617764140307, 58.725347241768, 16.0174185295996, 21.0017963709174,
                                      48.1192790522453, 21.3382955040629, 59.4099178685408, 49.5475551947313,
                                      11.1287322627243, 14.8150163170276, 84.8443091795594, 11.0498846242774,
                                      38.6310854225411, 80.6304492242225, 61.9178839928678, 65.238402150431,
                                      101.403373870421, 47.6428689316652, 70.2827063857293, 59.5639640834746,
                                      54.454380110583, 70.6836619245085, 49.7127946853339, 42.92266670463,
                                      7.68195690512235, 31.5584820355819, 60.9767739898739, 4.01728886573709,
                                      3.04006901610252, 4.28434394539664, 8.82120884490763, 8.22694085891054,
                                      9.26652602915838, 2.79725740346999, 49.0380480055329, 50.6740590946773,
                                      67.4605760075466, 78.4128794601833, 49.7773003860925, 25.7092268756383,
                                      30.8002135652137, 42.9005473347706, 43.6391474474616, 60.9102776941681,
                                      23.9041310794446, 8.4917004821193, 14.2123039116765, 20.1283088619697,
                                      19.7777785013321, 11.1753275423439, 24.4476732261914, 23.4963645258692,
                                      35.559676380053, 25.5030681417312, 4.5655523061132, 2.04802342790293,
                                      2.20598729175281, 2.67646574336092, 1.85175034171219, 2.1056310489285,
                                      2.65824891154533, 2.79133508798881, 2.12025559234027, 4.05661855502982,
                                      27.8682680384267, 16.521336858322, 15.9512126914787, 20.94426536167,
                                      13.6303233307168, 38.4932130011757, 28.0352485134822, 20.8228900475943,
                                      33.3673124837732, 26.0659304721004, 52.4473065409968, 30.5965969670274,
                                      33.1846886999638, 19.381090275605, 14.9182810229964, 11.5113664469928,
                                      106.811462795294, 46.9983158805318, 101.950750408571, 67.9719443188732,
                                      31.7830728184244, 11.4684499901182, 26.841503012645, 52.5163590195419,
                                      21.6542789456908, 11.1794230013516, 53.4575134382472, 89.8356238526146,
                                      89.130511113407, 63.7026051164981, 53.904110088838, 34.9591044403466,
                                      35.0656149359383, 46.0171155924215, 35.4077935212804, 14.0389581156031,
                                      19.1216790761272, 38.7825773865552, 19.2269777959514, 96.3922069042634,
                                      76.4168411297224, 81.068641360211, 67.1863509099468, 34.3388915373237,
                                      14.9507881453614, 7.71892658140429, 14.6338835435477, 35.5810984519093,
                                      117.485465920059, 81.7025654375851, 75.3089259506692, 74.3285403468303,
                                      54.5033596978455, 61.8138347139009, 39.0120946487452, 23.4136251635822,
                                      26.7843840768449, 23.6131118845115, 24.9118593309402, 11.4729796018852,
                                      115.224542359052, 98.0250585881169, 78.9292124732519, 42.9426231030298,
                                      16.9889107252414, 33.6802464423996, 53.4645424240741, 25.3931128153143,
                                      30.6287743914404, 60.6513006583794, 22.3025455360599, 26.3690892465182,
                                      24.9815242483478, 7.55900662076605, 6.084744693583, 19.6728750422381,
                                      13.2274826785214, 28.8383541926178, 20.4287468908896, 22.4224768423738,
                                      7.10709699272148, 11.9787340525496, 40.5120837667559, 11.188765898897,
                                      19.9985237041426, 17.5508065268931, 20.0732486971025, 26.3499014760768,
                                      20.0939688262794, 33.1747139649488, 80.8472692315722, 32.8592886715286,
                                      101.325825861416, 96.9088796497298, 28.8084375785664, 19.5012723424761,
                                      69.8261843820193, 66.0902125389916, 95.5160972086825, 102.242024253857,
                                      48.312198088996, 13.5740240833264, 20.4636618445093, 69.1217221305302,
                                      33.6223927423082, 58.7359853051189, 79.9077126419152, 27.9340595908597,
                                      26.8677439137404, 25.6143761108399, 40.6674794940673, 52.182559006563,
                                      37.5629651923197, 90.430424037154, 51.9291797589837, 27.7326419141636,
                                      8.52143758965785, 64.826396201877, 52.0099881366318, 93.9445476685782,
                                      87.6042511917402, 34.3475706661837, 7.45296756033053, 4.03187892590546,
                                      3.10787852807068, 6.72696789280777, 27.1482637947776, 24.5767948093451,
                                      48.2636767675837, 43.5142302622511, 41.4253158822274, 40.7747216821163,
                                      23.1154795194974, 34.4270646224119, 45.5518534922245, 40.2081460837293,
                                      30.2931043524051, 26.3056223931889, 30.2037234374595, 42.7023095833336,
                                      33.0034730407213, 22.2960861173415, 33.4489937467609, 37.7893071759493,
                                      32.0331111434321, 25.7218024449077, 34.0050957898115, 30.5258866299687,
                                      37.2240811843642, 20.6343687793268, 22.9983052070085, 27.3824290805111,
                                      30.8284247918266, 20.4122441766227, 21.3054363410894, 30.8296114429616,
                                      35.7144653889254, 25.4454697767676, 36.1693450464955, 34.6083494207817,
                                      34.7148570712761, 33.8392993576625, 22.3497333271234, 37.7268054136263,
                                      33.1317221937399, 31.1905260450988, 25.2902263168559, 31.2645828762863,
                                      31.4375341245589, 42.5161184412456, 28.3642507597311, 30.7676975110448,
                                      35.7750016163604, 41.081690360636, 25.3407474633406, 43.2655367492749,
                                      90.6870408336845, 30.6135113572428, 36.2478112816301, 25.0246553011942,
                                      29.5141666005652, 32.2245121942184, 36.7980656250512, 23.335700843172,
                                      32.6069554892646, 27.8804794502931, 34.4929471067593, 25.9178503062689,
                                      21.4733591584301, 28.0785873771998, 27.9293380150169, 33.0287802811433,
                                      28.3249143584346, 34.2866733288433, 32.9021350035148, 34.4890677644797,
                                      22.3715483289765, 32.0244463802081, 54.2449452650092, 55.0560869326604,
                                      44.1193230058268, 53.6151436000121, 44.9688543251647, 51.0987881897494,
                                      44.1906639187283, 49.9287991064745, 40.8658760350806, 36.0507576718569,
                                      52.8911076834052, 44.5867765001669, 45.7868591100845, 39.3417319385877,
                                      43.8913457916399, 45.360530172614, 46.08474199676, 44.5553622075098,
                                      40.7289581056561, 45.2046522178263, 46.235123074504, 45.676201599256,
                                      47.3525455111463, 61.9647603325618, 53.3310079919531, 51.7935217455106,
                                      42.1964305254955, 43.3333788756867, 42.3587507358604, 44.1333801241915,
                                      43.3457158570143, 46.082975932307, 43.9186527249491, 49.6976298264917,
                                      52.988004537036, 48.4086359174947, 40.3137549218198, 53.0982151437313,
                                      45.3156096120134, 43.9900977069379, 53.6661179559696, 48.4811354309709,
                                      48.6189730013753, 60.6381302590788, 32.7488867737259, 29.1570626793389,
                                      29.0956469416969, 33.6806131392451, 15.5407758740268, 33.9654967413711,
                                      34.890678067526, 22.6096367213637, 29.4256014496108, 38.3083210530581,
                                      24.8156001327507, 36.2104469296546, 38.7971895749232, 33.2838686514454,
                                      27.2854185648366, 25.7760637569981, 32.6191484005338, 36.0588825874945,
                                      39.0860786690421, 20.6909053535264, 18.5581392880773, 30.7470611290756,
                                      34.6677879368388, 24.6179908049554, 30.201941383864, 36.0056766766373,
                                      27.8096430684427, 21.6410110505608, 21.9910967631791, 37.5383690110224,
                                      32.0840602254542, 33.2114453283279, 32.9192374579878, 37.5692166159078,
                                      44.0994000453353, 34.6208738491539, 41.9609786853178, 28.8277012934845,
                                      39.4813922515454, 42.6139739758071, 34.5362530813828, 26.7579743444694,
                                      3.77658766601018, 3.8389060542353, 3.25539728470792, 3.33051614925767,
                                      4.19354716218155, 4.87008201088321, 30.9903771479985, 65.542312591014,
                                      124.577987582389, 16.1103449726583, 81.3156537555038, 92.5356496013867,
                                      70.4297046735821, 38.1198891794325, 56.6779349504269, 48.2432509642532,
                                      71.5171400914965, 116.588059069703, 32.6881642547529, 100.531229847056,
                                      80.2441171140614, 0.0061045434989758, 0.00561294334341005, 0.021393546527443,
                                      0.104328945559826, 0.117804133671884, 0.0815636869091484, 0.14712153000397,
                                      0.0232403679818394, 0.540715195385601, 0.0183563665678314,
                                      0.0246902392886189, 0.028577091168593, 0.0229459241187366,
                                      0.0328583547521273, 0.0112959500020401, 0.0149779825746198,
                                      0.0205141253781699, 0.0328126414972376, 0.0339601184382758,
                                      0.0180859260866928, 0.026736742620455, 0.0958330364644612,
                                      0.0815758274356916, 0.147576135982971, 0.178346529969757, 0.0932013965408888,
                                      0.14212867177324, 0.157081189621361, 0.085880402940003, 0.156710328078344,
                                      0.140677111155692, 0.151802648504833, 0.125316031516757, 0.128182403354083,
                                      0.108791285188172, 0.116207660289718, 0.0567146330084964, 0.0477155495866374,
                                      0.131550976672661, 0.17262555880943, 0.073318793673618, 0.0185805840088467,
                                      0.0424712293930919, 0.09460620737383, 0.489458253238899, 0.0478402785160685,
                                      0.045616473079775, 0.0602595807476666, 0.0472925213336259, 0.137441105869402,
                                      0.105045977059483, 0.16157193011623, 0.19094056865925, 0.0721194268456686,
                                      0.154936950725221, 0.103790255291998, 0.120060568423021, 0.092574817514521,
                                      0.0210416563264955, 0.0658517593502897, 0.376638051210184,
                                      0.0334816672594907, 0.20209714586525, 0.395433142679524, 0.043417618450691,
                                      0.0472393183366043, 0.0786208988026907, 0.054175208875068, 0.179297609068552,
                                      0.161380084797352, 0.184366618279837, 0.108015996489135, 0.0274673622100094,
                                      0.124171548390563, 0.0505498703655817, 0.0738366124525635, 0.14281097463575,
                                      0.0791530419308184, 0.193708055729259, 0.047697553270816, 0.0973302148985453,
                                      0.113445529103265, 0.0738306810472483, 0.124867949254019, 0.0509240909845285,
                                      0.0250701415987777, 0.0448703496559413, 0.0474598521537525,
                                      0.119920085551597, 0.0216150280945599, 0.0517346297075626, 0.038222761370244,
                                      0.0293339544854873, 0.0778572100739835, 0.117353184341007,
                                      0.0376435110384205, 0.053402239002122, 0.0200499512831175, 0.07737208105575,
                                      0.0203379390663358, 0.0728470389801433, 0.105237457351876,
                                      0.0573567708029476, 0.060208052300667, 0.631396305981373, 0.35881213665993,
                                      0.0477730965088108, 0.170096285390684, 0.185828300780367, 0.0621758878591575,
                                      0.0693126992999489, 0.106236137454244, 0.0450057305485166,
                                      0.0437735411759938, 0.0326823202495539, 0.0693532821644627,
                                      0.0671953213083578, 0.0877294827758277, 0.131333442206987, 0.138288018398153,
                                      0.076596843216075, 0.0845779296812806, 0.00802582073577491,
                                      0.105521164152643, 0.109425666374534, 0.0144533843753814, 0.478145213269053,
                                      0.0591341443434036, 0.10492570585665, 0.148947392130774, 0.155587659721158,
                                      0.124196539051366, 0.180527706863234, 0.0446954796707348, 0.0567666799013531,
                                      0.105687870446361, 0.035780202788466, 0.122987533703145, 0.0836147133829277,
                                      0.045317635357696, 0.0181748285430618, 0.388518779944136, 0.0490818602638287,
                                      0.210364250259426, 0.164291857776878, 0.106261957753482, 0.187810934481004,
                                      0.118241279663169, 0.160564822565122, 0.205657109554729, 0.173695970286224,
                                      0.110398728161394, 0.11069095197931, 0.122784845307122, 0.122933193585067,
                                      0.0173380740171906, 0.0144818331511496, 0.656280409697232,
                                      0.0123705320409688, 0.00827097546123537, 0.00963004511026463,
                                      0.0362127729784663, 0.0234210977548247, 0.0457393542112695,
                                      0.0100978749005034, 0.101721021361786, 0.0979605429241016, 0.135646454961984,
                                      0.155765824798002, 0.0709792265480324, 0.0694641003341807, 0.108464264722163,
                                      0.186082981210542, 0.258752267867838, 0.146565499311958, 0.0903419838600595,
                                      0.0228309823533308, 0.0278294922294879, 0.0346831314714845,
                                      0.0612480405418266, 0.0603577475675121, 0.0712767785015064, 0.09176929226079,
                                      0.111582780215378, 0.0848773043888664, 0.00761159261514915,
                                      0.00617402478313924, 0.00814199785463378, 0.00681198628034524,
                                      0.00675908535400501, 0.00648236976053681, 0.00412520095725074,
                                      0.00692777962525726, 0.00643287931833562, 0.00806029022638354,
                                      0.0672877055885687, 0.124140219439313, 0.106949167508714, 0.083236463506775,
                                      0.058288629720519, 0.192242441987503, 0.17520182809575, 0.0950060376496843,
                                      0.0924726399890465, 0.11987233962665, 0.0713560629744241, 0.129581468060554,
                                      0.114669033031543, 0.0567038002018773, 0.118747674702626, 0.0407774393401727,
                                      0.799116731923732, 0.168832128367637, 0.277670352262322, 0.292315608326971,
                                      0.128910272100509, 0.0282260551622098, 0.170242210243436, 0.0618595372783608,
                                      0.131450951460424, 0.0347330487811824, 0.131378708141161, 0.683752525189671,
                                      0.198914358928171, 0.152132271561899, 0.353830990302112, 0.175944949660393,
                                      0.0989303284184351, 0.123594241597224, 0.165152450382574, 0.0457419449800084,
                                      0.10087574031451, 0.103061711970217, 0.0514566030698454, 0.556084620809164,
                                      0.201861259259476, 0.19032246936826, 0.238451501422459, 0.192817100373051,
                                      0.0377746213767825, 0.013464125095979, 0.0451105202828569,
                                      0.0837023553525139, 0.718140751029104, 0.148969282062156, 0.21466218677705,
                                      0.303654612135663, 0.1844091690859, 0.370968276180502, 0.148585172094526,
                                      0.153431565129775, 0.0870200027541478, 0.0726685492527888,
                                      0.0312243381340561, 0.0276096125221438, 0.55691112648771, 0.178744276402295,
                                      0.29201568605655, 0.254658136039204, 0.0546616500336268, 0.247472178724754,
                                      0.135337179744807, 0.191558287456235, 0.148516547271147, 0.146427857856596,
                                      0.127574318871967, 0.1238472538841, 0.0133275079759112, 0.0375963441589821,
                                      0.0171754411438352, 0.0779438971469063, 0.0143414148250149,
                                      0.0265144740696288, 0.0861876019710591, 0.109242290639457,
                                      0.0259615203575933, 0.0770233573710531, 0.24144001813757, 0.0313649597797064,
                                      0.0194243507227132, 0.0393839011651968, 0.0191757617179024,
                                      0.0180846121205798, 0.0291814813658493, 0.0137972482636581,
                                      0.416205552249616, 0.127533423621256, 0.298525319511134, 0.141580777451495,
                                      0.117955798597929, 0.0227120372079211, 0.415773067170133, 0.254995022517009,
                                      0.320499238168359, 0.238789520395921, 0.19384346312259, 0.0692311563394662,
                                      0.0211662541060878, 0.360652589458544, 0.0948377803947093, 0.071697198284509,
                                      0.113181952349445, 0.146832300688239, 0.120130378413457, 0.0906983770812776,
                                      0.0326094302268185, 0.460340488200461, 0.0950630689532863,
                                      0.0947728160546839, 0.153212600710242, 0.11072855473946, 0.0267015901870293,
                                      0.403146223227841, 0.227661878289356, 0.212374979923118, 0.0899945179067157,
                                      0.114561784275472, 0.0182734641974536, 0.0136323227648443,
                                      0.0084334596920976, 0.0137133149559573, 0.107813098827244,
                                      0.0502496282134745, 0.0808156467259409, 0.105433035263434,
                                      0.0944115196177817, 0.116099327376593, 0.0788366679864806,
                                      0.0922949211884059, 0.131479810711909, 0.167054477203748, 0.124317173256182,
                                      0.100458717885098, 0.153495630025012, 0.164598424288682, 0.15009450068901,
                                      0.134215518133486, 0.14925710847838, 0.182216387624057, 0.156592908331338,
                                      0.138910360823707, 0.137437011582755, 0.141780011674729, 0.17285106521612,
                                      0.135121214654215, 0.141986515575628, 0.153636505814799, 0.1572660094275,
                                      0.112708560826349, 0.109070670722551, 0.154997809350898, 0.152161676534351,
                                      0.132436977876151, 0.135134424386031, 0.126515484976483, 0.158398667139212,
                                      0.125304381055838, 0.107891871771766, 0.1093088997178, 0.140475533342117,
                                      0.1396410331125, 0.150037396834574, 0.109652360437512, 0.156685731568989,
                                      0.167018243289737, 0.155706804115037, 0.160845556880875, 0.161352317029717,
                                      0.196036696338031, 0.117849241042559, 0.10299260388024, 0.13761366373849,
                                      0.12634990191918, 0.129815172547479, 0.119187676368738, 0.139686941960733,
                                      0.118785163977705, 0.172535333266415, 0.116911944952548, 0.0929334947364813,
                                      0.141850775935447, 0.119400735127188, 0.0833661402782039, 0.10549473953525,
                                      0.120999896687866, 0.130474188324902, 0.158177689657245, 0.124276428570159,
                                      0.114794717463271, 0.145463204831043, 0.127089041413574, 0.120669621187478,
                                      0.103275031598076, 0.299061196750306, 0.232743908618856, 0.260663194381808,
                                      0.282824563681211, 0.196144274351696, 0.213791386547665, 0.252379423693346,
                                      0.189084466916619, 0.216679372910401, 0.174688042899429, 0.177775635130383,
                                      0.188711960936087, 0.171909436056991, 0.139377600087629, 0.151895394066224,
                                      0.190714774332701, 0.175548953664867, 0.206912219577935, 0.198066543019082,
                                      0.215206624267214, 0.151401037501797, 0.151221802195689, 0.211627411601534,
                                      0.151756708215671, 0.205014731951472, 0.22192768577062, 0.16921372452844,
                                      0.159690168953485, 0.153992729136617, 0.165955793624071, 0.125712706940705,
                                      0.131206991085196, 0.140813842932251, 0.173434152928085, 0.150715984578415,
                                      0.119416671626436, 0.167769391502096, 0.179090865001331, 0.125620028903612,
                                      0.169429898087918, 0.258289091031473, 0.14000162804435, 0.157843371478261,
                                      0.140905853250443, 0.0960479481898838, 0.0323489334410683,
                                      0.0547541434565845, 0.0312140885859283, 0.0227424353118236,
                                      0.0353129036591143, 0.0415301562934168, 0.0408465502607805,
                                      0.0351145000064777, 0.0423784659463237, 0.0360236960804133,
                                      0.208045974446921, 0.124473806303753, 0.0421480552135369, 0.0253284511660968,
                                      0.0376572903258023, 0.038742916434457, 0.04718766588386, 0.0533856131958517,
                                      0.0303464543292671, 0.0216798206251099, 0.0444557853779848,
                                      0.0263103623817272, 0.040782978062019, 0.0283432218796511, 0.058022983047055,
                                      0.0769033902655808, 0.0289196124603157, 0.0309465532781093,
                                      0.202809872760667, 0.262148177733434, 0.238407091905629, 0.218850525366611,
                                      0.279008478199209, 0.261303855243794, 0.228132199611794, 0.226764327360803,
                                      0.176479427639542, 0.200516881097664, 0.351480891699711, 0.240560515414066,
                                      0.160719647662667, 0.00782913645178976, 0.0100163449173464,
                                      0.0113024397917265, 0.0101738271851532, 0.0165169701302217,
                                      0.0177304588290684, 0.138625660541382, 0.121918292342354, 0.472405548278411,
                                      0.0564101332381582, 0.192643313158157, 0.378406170853799, 0.196087264267476,
                                      0.161310456349827, 0.148514986815481, 0.131485548280501, 0.174754310085281,
                                      0.43198165830398, 0.0602180854123675, 0.161752377138767, 0.285686187795504,
                                      0.00499415294181728, 0.0048798562185518, 0.00908230585223919,
                                      0.0401702631261317, 0.0800809299041416, 0.0358174831847436,
                                      0.431943699363438, 0.0744055207555598, 0.835908372429498, 0.0550383933913922,
                                      0.0391804150180877, 0.042522635189708, 0.0393662120059812,
                                      0.0287500144906084, 0.01320675011607, 0.0216944304793534, 0.0264226438087599,
                                      0.11202415852692, 0.0759996575469335, 0.0532842805226964, 0.0637324573701228,
                                      0.46686455093863, 0.117183197344043, 0.11363805390885, 0.0793658792272474,
                                      0.199067993126093, 0.339952573989478, 0.098935488303251, 0.0908379779899437,
                                      0.068663821210945, 0.117508492792949, 0.0836519870651777, 0.132226874603138,
                                      0.0914723296874684, 0.0605747579674031, 0.0874768032790179,
                                      0.0660667314276429, 0.054835600837409, 0.162321004084241, 0.0882786258685952,
                                      0.0432880620073274, 0.017499854295167, 0.0267924321864004, 0.104014356506184,
                                      0.749486015182538, 0.0912925613805314, 0.0448159791914278, 0.12441343162192,
                                      0.0741158943000597, 0.524996522035049, 0.247838001545523, 0.180697188182529,
                                      0.146988437670438, 0.0932330754249671, 0.105063165336085, 0.0656093052809088,
                                      0.0764313011090424, 0.0601813990602135, 0.0387681390602638, 0.23313175835656,
                                      0.748913033881786, 0.0535883827676935, 0.182479460316067, 0.316293361421829,
                                      0.0429396444060052, 0.063418807345915, 0.0833280347302398,
                                      0.0796759785057776, 0.568907874095158, 0.294683734298929, 0.177060873370945,
                                      0.123237891189535, 0.0396014547651441, 0.0721033164659006,
                                      0.0485317042766446, 0.0603988145402609, 0.18108881933662, 0.0339998974805946,
                                      0.067910273436512, 0.0435542394935988, 0.0977320776229674, 0.074196281096438,
                                      0.0590715316861092, 0.189495914323538, 0.0392289400370763,
                                      0.0195871456802615, 0.0255436014816181, 0.0380730260876311,
                                      0.097411094934988, 0.0192160581728499, 0.0331043012816192, 0.019434655903804,
                                      0.0186950111489884, 0.056567550293952, 0.0373379733068488, 0.048948370392199,
                                      0.0704380932606883, 0.0223750832692688, 0.0455706021670064,
                                      0.0296425725126005, 0.0657734139190373, 0.0643075811590743,
                                      0.0511595193453341, 0.0519950151351479, 0.229876277868065, 0.122628810778943,
                                      0.0198178594396902, 0.113286137691779, 0.137775313321191, 0.0484014172700353,
                                      0.0312561129564102, 0.0908595528751698, 0.0502503399109513,
                                      0.0637511162123773, 0.0248024336750933, 0.144577563748489,
                                      0.0783224431739558, 0.0758864384239403, 0.134152425820993, 0.125668405054916,
                                      0.160159412886565, 0.063727593977247, 0.0169962403344481, 0.101389365946479,
                                      0.0974305598733712, 0.0466981798684667, 0.631770866765244, 0.268385360559684,
                                      0.132803568820224, 0.165189673250877, 0.150367912929405, 0.226131147662612,
                                      0.104095063864123, 0.0263067493471131, 0.0502706954982677,
                                      0.0899301597870569, 0.0531148528810169, 0.0741257610720425,
                                      0.0581574859681996, 0.0921927456550108, 0.0390556309332303,
                                      0.843607523452545, 0.031337842954856, 0.120961439195715, 0.452296803277169,
                                      0.279391532809859, 0.144891682333131, 0.115358390398658, 0.181263926917734,
                                      0.161268471492917, 0.104794407366609, 0.164939796345038, 0.0688594339569631,
                                      0.0995990948642587, 0.0763941467819009, 0.0526548905728115,
                                      0.134963703439521, 0.520707209708853, 0.0160212221758767, 0.0132055801964786,
                                      0.0133596932281418, 0.046680683460544, 0.0648901558656191,
                                      0.0449690076138984, 0.0208639555627315, 0.365421233194343, 0.189443230538765,
                                      0.224359209427527, 0.11860106419524, 0.121458610763249, 0.0804832883945716,
                                      0.101397249402257, 0.120690211906638, 0.110868499539951, 0.136034098214009,
                                      0.0974488636235421, 0.0409484063897133, 0.0369985855125185,
                                      0.033867049500909, 0.0693615232050785, 0.0507523674624106,
                                      0.0593213266728637, 0.0360431105747398, 0.0656611445805272,
                                      0.0597682806631607, 0.00644963315856711, 0.00523317669429334,
                                      0.0047303499940274, 0.00424956417506409, 0.00625751445172531,
                                      0.00538991159567548, 0.00563814912910251, 0.00543784681484574,
                                      0.00410079375211755, 0.0145402304808288, 0.0618558858301071,
                                      0.0361958308246663, 0.0581350690651009, 0.0555473873061997,
                                      0.0417436113052997, 0.0762505828663993, 0.0217156587052429,
                                      0.0421591438489962, 0.0672326633029202, 0.0569132920976361,
                                      0.0494637947120795, 0.14960792019729, 0.0990663851704939, 0.0634852600316643,
                                      0.0634023923407654, 0.087120549896667, 0.543432314581218, 0.248937299099251,
                                      0.207478642737328, 0.401789842694328, 0.23717434025463, 0.0384441707178258,
                                      0.130961361778323, 0.106533162747223, 0.0831814874707728, 0.0927010124048598,
                                      0.41131942582714, 0.614637636490413, 0.209075776774656, 0.321832046751864,
                                      0.187016304927576, 0.0970780580907162, 0.065798045152141, 0.121758158657053,
                                      0.158484601516097, 0.138913576681427, 0.0486920695825747, 0.116657087343886,
                                      0.0488568929953846, 0.694580021746094, 0.40149739347193, 0.301189564217896,
                                      0.394070850356922, 0.173338509477439, 0.0383457050956547, 0.0195725496225519,
                                      0.0342131968133536, 0.296781135154722, 0.455505367693271, 0.262671667627257,
                                      0.20187046853074, 0.0915458801916183, 0.248696440959248, 0.177244898617966,
                                      0.105303212438002, 0.0947693436833514, 0.111740557213361, 0.0312344763495956,
                                      0.0494260950677106, 0.0181439057421125, 0.78780047958962, 0.38173295728822,
                                      0.613588445456289, 0.221104760061676, 0.0612176240442524, 0.137170080513466,
                                      0.0745812216583824, 0.0946919908530381, 0.12599271984289, 0.186595849502957,
                                      0.112265584672035, 0.08350279908218, 0.0533698036695282, 0.0605443361329699,
                                      0.026805294201631, 0.0209271760166169, 0.021125218971517, 0.0157777000438017,
                                      0.124096933618567, 0.0684400441018424, 0.0106407818345251,
                                      0.0475028487722314, 0.0849855584610829, 0.0124007741390958,
                                      0.0372918858325877, 0.0152509951312064, 0.0138453612267484,
                                      0.0253309374579588, 0.20450984031249, 0.127649837820166, 0.397310126677159,
                                      0.133109970714854, 0.296594174144769, 0.241314323958684, 0.14728972266995,
                                      0.0428310469301579, 0.742273565906638, 0.164643668023318, 0.193651760483256,
                                      0.233843524055579, 0.232170581237699, 0.0603458337496562, 0.0608800596750109,
                                      0.501213123392235, 0.11616093986957, 0.257012452515441, 0.294684882957444,
                                      0.0928603296377033, 0.119145443467529, 0.0783475206496938, 0.202907375792033,
                                      0.52197856710976, 0.165383747453745, 0.28291438010011, 0.229631547296405,
                                      0.166513757403965, 0.0342063191998818, 0.651436532133436, 0.176012453316252,
                                      0.317058118286396, 0.25069727532302, 0.16104094105396, 0.0306999485063178,
                                      0.0105686238635021, 0.00938993903364864, 0.0399802935989777,
                                      0.0792072336400845, 0.0486202190625822, 0.0714076501818208, 0.11286264328774,
                                      0.113255711496222, 0.0956875297713936, 0.152902346623027, 0.244315052243231,
                                      0.0963566413948671, 0.160885427293126, 0.178679637784634, 0.227967178040968,
                                      0.119855404329493, 0.15100966245448, 0.217175386156723, 0.177185446446586,
                                      0.132558884004265, 0.150938777645906, 0.207189103514221, 0.176069821854788,
                                      0.120214362414453, 0.126452074838436, 0.142472347414612, 0.14674512657389,
                                      0.116433579481775, 0.139917700803147, 0.116973303073578, 0.134163766821102,
                                      0.112136240317585, 0.108960692807005, 0.141664610186926, 0.244203425311094,
                                      0.153591908574931, 0.141492416493439, 0.138302191363197, 0.172492856038529,
                                      0.189263484961614, 0.143904320638362, 0.110394835105334, 0.111416500022977,
                                      0.257793924432106, 0.169668552738139, 0.163171396547896, 0.131304524504516,
                                      0.169486147765228, 0.251502129505319, 0.12120886093383, 0.152019727580508,
                                      0.162124333148122, 0.209621853206232, 0.0743763473554044, 0.181077189718379,
                                      0.135105361851663, 0.208960154678364, 0.112536908252467, 0.153018678492791,
                                      0.138271460071743, 0.182109543115678, 0.162795620721644, 0.0949761854779546,
                                      0.0991229578603963, 0.139371586429874, 0.166627259154832, 0.125795979056744,
                                      0.115189896097581, 0.0663051000209444, 0.144445203776933, 0.167747564373516,
                                      0.140066845569881, 0.13703302444658, 0.148071414061769, 0.31224531134934,
                                      0.932734081755896, 0.963629427576362, 0.885598863086995, 0.869580548897056,
                                      0.828512252376298, 0.840733607065809, 0.737853969494571, 0.824955716534075,
                                      0.689009584602476, 0.643430238403266, 0.705661846730168, 0.750858920982181,
                                      0.633975637369331, 0.58004098070581, 0.650024817162858, 0.575854945545501,
                                      0.630714617452277, 0.675019731381937, 0.707512483938934, 0.669405664000968,
                                      0.644704533817688, 0.63417667770766, 0.616774751520683, 0.614013249859251,
                                      0.521331045543085, 0.64384959799793, 0.642462654583043, 0.663231461945309,
                                      0.547718463895099, 0.631002809088856, 0.497592966411437, 0.490976917753867,
                                      0.377478419491805, 0.489615353868174, 0.567642663985454, 0.545857562355883,
                                      0.63522281383136, 0.637922398779146, 0.540771443622049, 0.536425143774442,
                                      0.549879018468593, 0.403614976585403, 0.447121345501998, 0.207704141517768,
                                      0.0896740067162556, 0.0901817015068672, 0.165480932615081, 0.132535675477513,
                                      0.0675271218078021, 0.117534508496897, 0.135964926937304, 0.0891626093038743,
                                      0.107976635602753, 0.102219360182543, 0.0879872073612653, 0.242540919618065,
                                      0.205037741066953, 0.142538564924086, 0.0933286534417225, 0.0835180129952712,
                                      0.10998768256094, 0.0932744420891729, 0.118412414906399, 0.0966995308126496,
                                      0.0636817870217174, 0.113626955774547, 0.128459085474443, 0.0846855366268667,
                                      0.106527756133033, 0.101678724621639, 0.074983979070019, 0.108662597688642,
                                      0.0774143422100022, 0.570160948096893, 0.985210731506387, 0.90777780034736,
                                      0.986658484558734, 0.931609144187183, 0.979156330147707, 0.936502136755316,
                                      0.988638230715761, 0.918466992466846, 0.956431813403751, 0.966777074470133,
                                      0.994970365696913, 0.545580842052763, 0.0239513224012103, 0.0091987216025248,
                                      0.00888401959070275, 0.00988163356363563, 0.0110627311798087,
                                      0.00945051464994149, 0.135182716556597, 0.374619477227255, 0.412573556828828,
                                      0.0762731735703755, 0.223186275589073, 0.482402268699595, 0.208630402689354,
                                      0.139555652108111, 0.172578266753878, 0.115191922292549, 0.426348577639977,
                                      0.381022341684765, 0.141119382966383, 0.298066728763095, 0.354705784912534,
                                      0.00697894130721514, 0.0096492864664852, 0.00857164377884082,
                                      0.0571480674716577, 0.297031300946414, 0.285898200486724, 0.132173283728452,
                                      0.072406632314791, 0.602525573881462, 0.0517144272317465, 0.0468679372586421,
                                      0.0528509313105184, 0.0540718169347061, 0.0243537108699399,
                                      0.0116423340637295, 0.0374751778483427, 0.0234547601689789,
                                      0.0646709260481292, 0.0475214023796458, 0.0248881391043325,
                                      0.0586342066249274, 0.229676952932559, 0.180646549423297, 0.0992909035910502,
                                      0.0582604535896976, 0.407567377842083, 0.531485735405297, 0.134135272198117,
                                      0.0700647999264965, 0.124674208742413, 0.111208381961266, 0.069028955549844,
                                      0.101668924487482, 0.0870555229549263, 0.159106386556114, 0.131240185153129,
                                      0.0435516464399789, 0.0276082831012225, 0.156183788927529, 0.140054708633197,
                                      0.180879445949977, 0.0144657129029604, 0.0523839180633289, 0.211790390992213,
                                      0.608635990937411, 0.0229236798149176, 0.0250883792666194,
                                      0.0587262432490644, 0.06473143628867, 0.504603564676654, 0.484624331538925,
                                      0.203174221777791, 0.141196556161528, 0.0876586091396657, 0.0643186196984296,
                                      0.0919200451076457, 0.0935554317423386, 0.123235722358789,
                                      0.0434264126500745, 0.100654941723184, 0.61430505968733, 0.0184457343342457,
                                      0.163902688169792, 0.447536234264981, 0.0609848926978547, 0.0433680837177817,
                                      0.0414397300367858, 0.0433280632801569, 0.39147383868432, 0.488686126856807,
                                      0.147802349780745, 0.0835009957680056, 0.0321712499854103,
                                      0.0550294895602538, 0.0377550648648514, 0.069749574211633, 0.356920729949229,
                                      0.0414329844048848, 0.109060785507052, 0.044135962188018, 0.192408217949678,
                                      0.214453347057615, 0.103021759239628, 0.127263779575823, 0.0438238077053106,
                                      0.0157528266362072, 0.0426740331520319, 0.0276024874416673,
                                      0.114192076463448, 0.0415203125830747, 0.0468584315816796,
                                      0.0404974130860225, 0.050576882190438, 0.136620261491629, 0.107341309775915,
                                      0.0401814544969085, 0.0513833540202303, 0.0520060536450662,
                                      0.0870190068622888, 0.0759686930083734, 0.0831024622357121,
                                      0.097939436846907, 0.0614964310369918, 0.0443534220468376, 0.93992200181374,
                                      0.316302947295554, 0.0534970404016945, 0.427006518201884, 0.14348983255709,
                                      0.146064327915925, 0.0256924348141451, 0.195670818712403, 0.178148150502661,
                                      0.0486272256754985, 0.0214367900799326, 0.122661016808321, 0.153994898702464,
                                      0.0748011795947152, 0.112168760816475, 0.204373006118683, 0.352899522891993,
                                      0.102053070619063, 0.0226962026315595, 0.155212537382253, 0.112140515618007,
                                      0.0573193696073787, 0.531562555445171, 0.0740962520750093, 0.314219511409018,
                                      0.0992042040764265, 0.269248613730542, 0.464800272903031, 0.0968030573377339,
                                      0.0473940355670746, 0.0530064204242234, 0.140477427075078, 0.129116158792099,
                                      0.10470203844092, 0.0951801264555036, 0.0449511330166014, 0.0572529436813117,
                                      0.545231450241515, 0.0455148281758457, 0.151577542676721, 0.464969403755862,
                                      0.466533440216666, 0.224525095755362, 0.121759572497635, 0.1276403758176,
                                      0.144195252909244, 0.0790337364102723, 0.270578142518926, 0.107545173374108,
                                      0.163113432260789, 0.238058682060059, 0.0291065229354013, 0.0247725530361811,
                                      0.700627251288303, 0.0159504771274688, 0.011121917493822, 0.0182571177721886,
                                      0.039996777831349, 0.0279346241271969, 0.0382239760429106,
                                      0.0193928846834105, 0.160966015503363, 0.301959254582541, 0.322955707501072,
                                      0.231943779803593, 0.0912707683400295, 0.0604765387958013, 0.111641900451238,
                                      0.0866106527155589, 0.0970764705987463, 0.0783371093859053,
                                      0.0339672490865493, 0.025154065065002, 0.0377912770264515,
                                      0.0439475325531153, 0.102685668465599, 0.0510552923178749,
                                      0.0367368220061867, 0.0404882558206139, 0.0576413623685204,
                                      0.0397810135981535, 0.0201583573583285, 0.0104370211116599,
                                      0.00758909742348696, 0.00775300511385001, 0.00857825639339033,
                                      0.00868994612021335, 0.00908994475638954, 0.00771150052737796,
                                      0.0119728448511824, 0.0208831325766889, 0.0457343454328456,
                                      0.0316462119600057, 0.048847040628367, 0.046746690718351, 0.0368736047506311,
                                      0.166414019754619, 0.117845407773811, 0.055400320080643, 0.0638008698028092,
                                      0.0847450749562806, 0.078771804843865, 0.0636709841997543, 0.138612407831143,
                                      0.0381138399492202, 0.0628129552448613, 0.016592730839187, 0.619924874259409,
                                      0.148265057413194, 0.393920308323369, 0.350183105569532, 0.10096531172012,
                                      0.0232213104980691, 0.0682258793933127, 0.209849312851285,
                                      0.0390903153017047, 0.0294859317435281, 0.0561560462411897,
                                      0.394584166977089, 0.283284801467869, 0.224893314879426, 0.193835878931054,
                                      0.0738059486219824, 0.042231121154262, 0.0979612514521193,
                                      0.0404502042597835, 0.0558912268544281, 0.056534911157982, 0.122550639980093,
                                      0.0371535695843233, 0.525914556755198, 0.383886153907372, 0.184449220567256,
                                      0.290437289035106, 0.0769718596808054, 0.0415829291818104,
                                      0.0340252247109936, 0.0431123369540202, 0.042190029670765, 0.492377346744519,
                                      0.166011382718993, 0.24160996017401, 0.0729545765996693, 0.113169007677802,
                                      0.120235504002109, 0.0727292336009646, 0.0326426149951364,
                                      0.0658790410973002, 0.0627225827492108, 0.0966822468217595,
                                      0.0241786683403543, 0.494582231037445, 0.397757067506298, 0.212107963960344,
                                      0.114085570672087, 0.0351241651322569, 0.0786074897800171,
                                      0.0678888068869186, 0.0938585690049824, 0.0709798099493249,
                                      0.0694423834037555, 0.0676873223398401, 0.0525481148901606,
                                      0.117813816491909, 0.0348750665207441, 0.0395672977456749,
                                      0.0387846450850033, 0.0564025416742361, 0.0491674833431244,
                                      0.0752776325686275, 0.0876206025779386, 0.020040404733792,
                                      0.0466781817581682, 0.132403460713147, 0.0255987586592179,
                                      0.0145161325673831, 0.0188846405236288, 0.0142893908365992, 0.1347839412856,
                                      0.108710378541564, 0.0561466952697511, 0.543146884041356, 0.135207582108224,
                                      0.176969727173574, 0.390525866327069, 0.0743256905360911, 0.0581106299192227,
                                      0.497576861875911, 0.136020853042514, 0.151573602059687, 0.430433466484931,
                                      0.128578718433589, 0.0396670095489926, 0.0668877063600769, 0.552149523565181,
                                      0.133322595385613, 0.202368046325703, 0.364336315860737, 0.0715756421481251,
                                      0.0905027501339254, 0.116353287501116, 0.0719064427699128, 0.395418025502756,
                                      0.156018132785132, 0.385453237618142, 0.203892563016093, 0.0629941080823104,
                                      0.023886975082097, 0.621602455876383, 0.131060497416368, 0.258185246788189,
                                      0.393877957362374, 0.0988608589979645, 0.0302612694733788,
                                      0.0156047288002949, 0.0208710024576996, 0.0125680264515565,
                                      0.0629770979492238, 0.0527720697730921, 0.0529649301190077,
                                      0.0800968251166669, 0.0613708631074255, 0.07607818380811, 0.0791270234275934,
                                      0.101881010409737, 0.0886564952975112, 0.0602458458734853,
                                      0.0738463837861472, 0.0908958746789294, 0.082485494897545,
                                      0.0637753959044222, 0.0553259771706344, 0.0860831892033367,
                                      0.111975694008765, 0.0423713644565031, 0.0741362083667762, 0.060015976769684,
                                      0.0769561985063522, 0.03102660388267, 0.058024237371081, 0.0696890996785843,
                                      0.0856594158608563, 0.0563423068600623, 0.0483606775259386,
                                      0.0687249514473902, 0.108013985982861, 0.115455954794816, 0.0483221805634872,
                                      0.0802413931182659, 0.0840295079706569, 0.0841792857656281,
                                      0.0890398623719908, 0.0551769681109068, 0.0774209836981165,
                                      0.111907608968805, 0.0459628175870208, 0.0626728516121545, 0.087050694068202,
                                      0.104910378935392, 0.0441392049929697, 0.11311956037048, 0.063354299073234,
                                      0.0818874695701424, 0.0304441731285611, 0.0705651490196752,
                                      0.0744025693560824, 0.0932078361692133, 0.092699668064984,
                                      0.0519537889070589, 0.101698530273723, 0.0953182384945653,
                                      0.0981295102419575, 0.0485397270463686, 0.10840323787097, 0.0819056981823686,
                                      0.0907540330467716, 0.0718138462975074, 0.0341751531590544,
                                      0.0448814297308832, 0.0839678062801527, 0.0670380794999244,
                                      0.0309706271566662, 0.0254914017413504, 0.06135792917584, 0.0943951834206914,
                                      0.0680326296612003, 0.0460365055647071, 0.0564153160487724,
                                      0.0868077824513299, 0.167860395858527, 0.160598076398437, 0.158538787472324,
                                      0.117670140128083, 0.116003026824134, 0.145807343452055, 0.0973375271168671,
                                      0.105379982637528, 0.121045723217131, 0.0821061712412997, 0.174723614065636,
                                      0.116157677452966, 0.0850498737842399, 0.0756290880702, 0.136502075017561,
                                      0.118173914262641, 0.189879320021028, 0.122559914528818, 0.0947900427956281,
                                      0.186803583087126, 0.115249537509165, 0.0728155448351043, 0.158830256578524,
                                      0.181013305748335, 0.161982376386175, 0.113585863225559, 0.119047125924973,
                                      0.110745200266056, 0.0959683718059746, 0.130476765186556, 0.132026541123104,
                                      0.10625387943496, 0.110541732665645, 0.0990286386257106, 0.124248945574703,
                                      0.107950641317242, 0.128252078538001, 0.0986888191753191, 0.107108213266155,
                                      0.1158570480289, 0.173469905710859, 0.08724145480234, 0.108616320811279,
                                      0.32209765667106, 0.0811652030616352, 0.182026479282426, 0.180686920586635,
                                      0.239746047625239, 0.157753496435797, 0.327159393918186, 0.311456875533838,
                                      0.174706133075082, 0.251661992670647, 0.243708561902683, 0.185081377336666,
                                      0.106424924481117, 0.194664231047658, 0.217904622493142, 0.193960032584573,
                                      0.19685893993156, 0.220685192884576, 0.20346650670475, 0.274189943718786,
                                      0.175254610655155, 0.13224285956214, 0.268217929090732, 0.322684455796715,
                                      0.18772608528105, 0.216342343188715, 0.290054214354873, 0.1677809502356,
                                      0.085685479995505, 0.0900119725498418, 0.085576351728312, 0.189563239433539,
                                      0.125184472013521, 0.141387529012997, 0.0740772282312752, 0.129131080915346,
                                      0.184953165556217, 0.103026798710246, 0.103692484202791, 0.166701573347113,
                                      0.11116935066523, 0.106659350305714, 0.163057966265119, 0.0168576917318321,
                                      0.0111384341487056, 0.0123420778243623, 0.0166417818678264,
                                      0.012652212023028, 0.017104573033191, 0.13200066520213, 0.19637257292096,
                                      0.5827936233213, 0.0687976298000398, 0.469269224007718, 0.179334824269145,
                                      0.261315985331561, 0.0762558200382477, 0.0929152519106964, 0.092374375551155,
                                      0.365775891192313, 0.641620516097905, 0.0968541859674123, 0.380544696493273,
                                      0.219460844067651, 0.00472300994104048, 0.00586673198309239,
                                      0.0100681333193276, 0.0311404407721104, 0.0706626475596285,
                                      0.0509379326913952, 0.445937900170855, 0.0715374086621729, 0.666270927069251,
                                      0.0522869478016069, 0.0471738972212181, 0.0535910271224194,
                                      0.0516719522753952, 0.0212415833725806, 0.0117023371715983,
                                      0.0354587824226638, 0.0210799308416265, 0.0568850535246393,
                                      0.0477235791827055, 0.0313251613022514, 0.0568130133788171,
                                      0.108440555046174, 0.112966368886247, 0.0920885674277961, 0.0537497351770566,
                                      0.159387148936129, 0.351362645188681, 0.0980063293303626, 0.0843018599257722,
                                      0.0823535229302353, 0.110688840950721, 0.0755599246464152, 0.135448879171146,
                                      0.0875554621237, 0.0631839237737732, 0.092663231431502, 0.0674078304728947,
                                      0.0554719001114469, 0.155869030361213, 0.0744459763026527,
                                      0.0491111723170618, 0.0138949624055706, 0.021781628878549, 0.128803732187548,
                                      0.71839275812863, 0.0206605667821274, 0.0271196373928567, 0.0439373790167939,
                                      0.0599075973772068, 0.174944398185171, 0.28863201732371, 0.158174812611904,
                                      0.158853636617809, 0.0958810112604283, 0.0996632258289389,
                                      0.0585941743778631, 0.0763760062745105, 0.0618844808239509,
                                      0.037808859507712, 0.224908144272843, 0.575606490898408, 0.0256041107148124,
                                      0.0481473669213171, 0.139353912018242, 0.0540731092467737,
                                      0.0468907832848307, 0.0385062672613151, 0.0551326932971082,
                                      0.142816057365112, 0.328511803266725, 0.169212645821902, 0.122012069630684,
                                      0.0379933112003749, 0.0674805103616278, 0.0469202688190089,
                                      0.0580697196554335, 0.0999064843051333, 0.0304795796806388,
                                      0.059238796331496, 0.0504906226038127, 0.0768995880801217, 0.077816013872463,
                                      0.0595610272013624, 0.191734928611641, 0.0418044953766064,
                                      0.0211186691199929, 0.0262545315211769, 0.0365567296381221,
                                      0.0985540778568024, 0.0212654166308876, 0.0298587495881054,
                                      0.0138472932252116, 0.0207984257325165, 0.0634154849671303,
                                      0.0666451003954592, 0.0513088487254528, 0.0652115217742996,
                                      0.0230307694697946, 0.0489575271937111, 0.0338561652630952,
                                      0.0597744200310848, 0.059942064259792, 0.0533045656457197,
                                      0.0549409947244925, 0.718174645127921, 0.153251649101586, 0.020473385236345,
                                      0.21294127244548, 0.153800280082643, 0.059208826739012, 0.0373991893073599,
                                      0.0340693084178991, 0.0914044913021769, 0.0618540653623159,
                                      0.0241413691712562, 0.125335258026501, 0.0815583094287705,
                                      0.0726893607157659, 0.132306010640925, 0.152109097528735, 0.155594508100101,
                                      0.074964344622555, 0.0179101172350163, 0.101474896378044, 0.09592421251533,
                                      0.0504815360919495, 0.53392290447889, 0.0676201998003556, 0.126290224700727,
                                      0.167540177356466, 0.142257601249385, 0.305269866178101, 0.103792003192597,
                                      0.0264341313377429, 0.0466224980724092, 0.0844355291792002,
                                      0.0417349938397086, 0.0688721186826506, 0.0578373219297809,
                                      0.0920899829293254, 0.0324820094713157, 0.549289386734537, 0.038346078616055,
                                      0.125443526462808, 0.136066607879998, 0.353182071015237, 0.168390724538124,
                                      0.111114003458546, 0.180416989579407, 0.151387934126715, 0.0860830138884389,
                                      0.142186837733442, 0.0708160261616276, 0.092731114874363, 0.0924674710492245,
                                      0.0534590836456189, 0.136317266170179, 0.723203686506344, 0.0180293795266633,
                                      0.0111391600684278, 0.0180792502590547, 0.0434436533593353,
                                      0.0265092098263905, 0.0238038706728978, 0.0179191255354587,
                                      0.0838030142743101, 0.18869394913438, 0.300095613558552, 0.109699450704124,
                                      0.11199632426041, 0.08370771664406, 0.0996150617458109, 0.128613253970705,
                                      0.109079688069526, 0.13180090324623, 0.0974975406784105, 0.0405422328275075,
                                      0.0366312420891912, 0.0312988115160874, 0.0700806586119156,
                                      0.0513275745844328, 0.0589938778008406, 0.0350525223995874,
                                      0.0639963953183552, 0.0594444634543658, 0.00609964291690629,
                                      0.00566374605367174, 0.00466485129167754, 0.00417967525702836,
                                      0.00715737481525056, 0.00574332294331087, 0.00556069717839129,
                                      0.00545270146052112, 0.00433255464451558, 0.0131695891701455,
                                      0.0634161018705002, 0.0351661532310815, 0.0571811293680323,
                                      0.0514061500290542, 0.041554616291406, 0.0939828816940496,
                                      0.0299404909793088, 0.040856954310999, 0.0654513098984978,
                                      0.0556838343953365, 0.0508172928415558, 0.145597577380819,
                                      0.0962230026422636, 0.0638085235725776, 0.0623383475576244,
                                      0.0895225342009456, 0.641491767768562, 0.106646409819086, 0.110235342049234,
                                      0.251464773780798, 0.216535233646876, 0.0377517032045326, 0.126726009567446,
                                      0.0834512993140819, 0.0780144534406875, 0.0912192632289941,
                                      0.244947382094607, 0.584654879381282, 0.0912720630992809, 0.213145562801906,
                                      0.168659389757876, 0.0932072608773461, 0.067132446160618, 0.118750204544595,
                                      0.145026025729148, 0.13752329570916, 0.0491086667946587, 0.085406806896191,
                                      0.0359492520768317, 0.596721703546535, 0.107984832701797, 0.153667787702691,
                                      0.246114833300184, 0.169666814473981, 0.0323149041769073, 0.0170269272667122,
                                      0.0323771355802013, 0.202108151965902, 0.513361270798718, 0.156726877817132,
                                      0.213641478882656, 0.087739109665892, 0.205655898433394, 0.181549024581859,
                                      0.0999511775583129, 0.091580978448974, 0.110460112538771, 0.0301849321785795,
                                      0.0472526291018694, 0.0166108546397645, 0.621383904329727, 0.114527728092212,
                                      0.207769612837075, 0.208714724128276, 0.0578604649938655, 0.12740166876835,
                                      0.0728111630747201, 0.0907520669810921, 0.122218738811986, 0.17847498050376,
                                      0.108574954716547, 0.0814006194409966, 0.0525203341322399,
                                      0.0632368837026699, 0.0266821487335519, 0.018599389017708,
                                      0.0279797556004893, 0.0185089720663608, 0.122582768933865,
                                      0.0714757848656999, 0.0106598136948734, 0.0443554469670328, 0.10213445466422,
                                      0.0143141035421417, 0.0373814983350342, 0.0139307725960377,
                                      0.0134093288188349, 0.0384804564633394, 0.165378459699911,
                                      0.0916725291816554, 0.635718091217725, 0.125270746599556, 0.159539942159977,
                                      0.296030172136878, 0.138941932308154, 0.0343115518198459, 0.610181657714099,
                                      0.136977584737649, 0.13609858674401, 0.306894502459404, 0.159843815138591,
                                      0.0582222516522455, 0.0667728175590579, 0.579066451748005, 0.144886077530268,
                                      0.133909283312155, 0.201090635673811, 0.0848891651807872, 0.113637228056893,
                                      0.0737541533355551, 0.204460729191901, 0.623409604930143, 0.167681665911713,
                                      0.212387476839199, 0.156737989150161, 0.164782029639921, 0.0355690069246808,
                                      0.777000524713697, 0.138547695055763, 0.162595031171409, 0.198693417282236,
                                      0.155660945997944, 0.0304773478380749, 0.0104923690367283,
                                      0.00899189351032305, 0.0401010985385818, 0.078844089234547, 0.04831739820339,
                                      0.070167106110723, 0.111630713100329, 0.113519788744635, 0.0995095653546896,
                                      0.149646885471434, 0.229079520574309, 0.0961150572824442, 0.158175589087145,
                                      0.176570310519182, 0.210159247401642, 0.115546112113918, 0.148313103508764,
                                      0.20687115818842, 0.173420571920811, 0.132643692978644, 0.14363791786202,
                                      0.193909486705703, 0.162219616599928, 0.114522576797433, 0.12220285654627,
                                      0.141485976457845, 0.131772739333186, 0.105077714586717, 0.136055655375337,
                                      0.11297056382465, 0.117696214984921, 0.114519772363352, 0.109636355177768,
                                      0.141375120570568, 0.236448784703933, 0.146337261463067, 0.13988484322312,
                                      0.13244226853055, 0.164837367844072, 0.181974904657564, 0.140741539000179,
                                      0.106061619114426, 0.103378991214827, 0.241211680887827, 0.160106903110052,
                                      0.156252010386852, 0.120879149595579, 0.162153426103506, 0.250398997327541,
                                      0.114903032377578, 0.15569428114961, 0.15063146018084, 0.195204928069956,
                                      0.073951817069589, 0.176549759739432, 0.125231976062631, 0.200438768638203,
                                      0.10708919393975, 0.149623128610737, 0.128712921841817, 0.17096417599247,
                                      0.155651593682, 0.0950537563620956, 0.0956604278846219, 0.132338725838898,
                                      0.161563077664092, 0.123831384496934, 0.111463086771478, 0.0505376049547291,
                                      0.136475753929861, 0.164971974204579, 0.131323326710827, 0.131034808539026,
                                      0.134228318266111, 0.292038952052014, 0.755482821285161, 0.724330443295347,
                                      0.78487142739676, 0.763837366546346, 0.680075714736887, 0.752316396238751,
                                      0.687007790659534, 0.77629723509368, 0.690705359018264, 0.617688656103021,
                                      0.677667562532427, 0.721436462808659, 0.617445205362714, 0.555729238752326,
                                      0.628960528935614, 0.556996730720413, 0.617623548986795, 0.664522978290034,
                                      0.672799410010287, 0.660254968768923, 0.633079356840297, 0.625267606587174,
                                      0.596679415583193, 0.58925833630764, 0.525461944172634, 0.622714980428382,
                                      0.625803311929877, 0.649025685110461, 0.534582502588087, 0.616072763735439,
                                      0.49697625215143, 0.482128018078557, 0.376004136469474, 0.476753860798827,
                                      0.565192957570511, 0.529037044754508, 0.62760463280102, 0.639830862946377,
                                      0.528674995109781, 0.52758785298328, 0.558866609418323, 0.401791021803591,
                                      0.444787740239056, 0.177970317030006, 0.100595597606486, 0.10434437593864,
                                      0.198018440341958, 0.152294687729483, 0.0961372655702218, 0.180054478669622,
                                      0.18335588681428, 0.139630073742124, 0.117636027441975, 0.150587700389025,
                                      0.139785346689334, 0.277979700913836, 0.252774800822655, 0.198066953106255,
                                      0.0884454925937071, 0.147335022552203, 0.132065512554605, 0.145999706822101,
                                      0.180918938628188, 0.0778593481284445, 0.0864245239719306, 0.166210825447506,
                                      0.1904232846571, 0.127472240314497, 0.126487636339264, 0.167111082966605,
                                      0.129633225580255, 0.106880903202136, 0.0837673536686381, 0.469875848515799,
                                      0.849144750514891, 0.751953350663251, 0.786429624899887, 0.799799686497743,
                                      0.809851312329775, 0.777941578998067, 0.830593763189673, 0.762744644298959,
                                      0.8069683095638, 0.81668463235306, 0.806086292554536, 0.512248401352869,
                                      0.0231177583845988, 0.00932653874140294, 0.00922487069223317,
                                      0.00964889603226701, 0.0109720679055882, 0.00908956902699604,
                                      0.120172980916922, 0.315497791725767, 0.356546265643471, 0.0577303729761584,
                                      0.128135093776212, 0.167588312407869, 0.121471398161366, 0.13113598620035,
                                      0.147487394322089, 0.110037887826096, 0.349748752203773, 0.318999676256313,
                                      0.0650058102787357, 0.171730010503712, 0.246331699996389, 1.15664533450148,
                                      0.977654961993681, 1.47250154687855, 20.2315798867951, 38.0881213208805,
                                      27.9544713163768, 21.7555343820268, 15.2254584475175, 110.483330440982,
                                      25.2115612206589, 10.9785907051791, 7.32334751692441, 7.28786484728601,
                                      4.32540801291361, 0.998701769402603, 4.64719414046516, 2.7036726153075,
                                      10.0171555204674, 10.8067088466796, 4.58504444475349, 10.3075042301276,
                                      32.0951356153141, 45.1579169131862, 22.684502151199, 15.2613961087698,
                                      96.5689239274519, 111.809233979661, 42.2461044954299, 39.8709820824142,
                                      46.5740565216765, 37.5077250065047, 16.7153036964567, 25.1733687269729,
                                      25.1817216018104, 41.1408832842699, 40.0787269351192, 13.8218369286296,
                                      7.4957401438004, 36.725907985254, 45.0808338043322, 29.1714091741115,
                                      3.63455449578522, 11.5129450512723, 37.4260659423258, 104.10877391787,
                                      2.8772048786276, 5.25944425337542, 19.6846208091582, 18.1097274795012,
                                      39.2120529031822, 104.453568459222, 38.6443649401452, 39.0489974546714,
                                      21.780790544802, 26.1879804385788, 35.6657376402677, 23.6933489306903,
                                      53.3645999235168, 9.60765965349692, 37.5006638841964, 102.936683205915,
                                      6.457621311405, 10.5198690710858, 35.3711950688435, 9.83523735187375,
                                      19.9883561213577, 15.4876745309041, 19.3800406605441, 62.6887113154306,
                                      86.9304101824487, 28.5859337523574, 20.5213362377793, 6.67961199192609,
                                      9.41318437667644, 11.1855525958882, 19.9280787068912, 92.1501945496755,
                                      10.0125283724328, 25.3583419023119, 17.7739899949905, 64.7497509651332,
                                      80.1257898597242, 53.0745517501836, 56.1658143747207, 16.7290317643866,
                                      5.02386900365352, 15.0669425248468, 10.4110391441027, 19.8930213273141,
                                      13.9252420010117, 11.5041313721627, 15.5260914811991, 17.2154731213019,
                                      83.9616376971427, 49.3081434964633, 16.563958492927, 21.1261112212224,
                                      14.0446800476347, 19.9721777657816, 10.9994709140378, 22.1265107337567,
                                      31.8759512933961, 11.5898666634894, 10.0590228927361, 27.6841189765665,
                                      11.3290197020455, 2.01979305242346, 11.2688264218102, 16.6549260517091,
                                      12.4910784057345, 4.72885686288048, 16.177307403033, 9.52397911465278,
                                      18.908804285558, 5.14505263811564, 13.79784020028, 23.4633127478908,
                                      12.0389877771445, 24.5736848443738, 39.8764421625956, 52.9618665686128,
                                      20.2960158857897, 5.36272983164621, 25.4688605786712, 22.1684752896352,
                                      15.1539006700086, 86.7365654078939, 40.1420719364924, 60.8148277055543,
                                      23.5433699408629, 60.8309942988438, 81.1739930838932, 16.8338691088053,
                                      5.20447686350343, 16.618145673764, 25.6413390424558, 24.0136673911628,
                                      17.828819607994, 16.7732737109294, 6.68016524219501, 13.8185851896145,
                                      95.5358491500568, 6.27092678230899, 16.721733262321, 34.8253530558865,
                                      65.2305375257179, 39.155641307412, 24.5020855767098, 27.8557325433509,
                                      21.3322093103615, 15.7316857469551, 49.846940004653, 30.9571294657737,
                                      26.055642801419, 24.7521479200103, 7.86314901100277, 33.6547583339884,
                                      66.235478773641, 3.68985064004042, 2.45068051506506, 2.87737062762265,
                                      9.54669077201044, 7.0624401282932, 4.83474688247316, 3.14996130898497,
                                      32.725253902274, 44.6319556581344, 69.0744905540993, 15.9366891872979,
                                      27.3019208886128, 21.3436757599818, 14.9187822869998, 19.896645837928,
                                      18.9009566801369, 13.3939704914835, 14.6508393330935, 4.64633719500883,
                                      10.7902417316178, 7.28063368399626, 15.0132410426372, 9.01934938222996,
                                      11.6853264765556, 7.08208055890567, 11.6910991487653, 8.24866253602224,
                                      1.84459066703955, 1.217089965507, 0.962161310804759, 1.18645182683404,
                                      1.32695816844198, 1.96077326577203, 1.88264896455537, 1.44662615643495,
                                      1.54492731059443, 3.47119650915833, 25.7041481881285, 8.77702820543195,
                                      8.60591409330425, 7.1255943414605, 7.05494706027639, 28.7961556900652,
                                      12.0943000437184, 7.96835796853668, 13.7165575805839, 11.7569185044968,
                                      15.1387208087868, 14.6397186686822, 28.752078930409, 16.57458401117,
                                      11.2326628181988, 12.0746979916806, 79.0643919651599, 24.466308218454,
                                      48.120529707529, 43.5126764891777, 21.9176991120528, 9.66838110366229,
                                      15.4066865768618, 55.8429972808194, 20.1520257933351, 9.61646141800834,
                                      25.6815340082702, 53.4473360571588, 42.4942178381913, 52.269655424861,
                                      27.4615356056625, 18.4018853908874, 13.4116710514278, 14.0433335823336,
                                      19.9170876337019, 13.5118917458833, 14.0354225795779, 36.5028480245741,
                                      16.0080491256137, 86.2635694313602, 32.8266127889636, 53.1468419647168,
                                      46.318673156482, 18.8465346141599, 13.5594113456791, 3.33914060720173,
                                      11.6586605506649, 17.4887253316542, 74.0368805962988, 62.5991506840228,
                                      55.3060140408938, 13.4373001424918, 22.3240849762377, 23.8910495085072,
                                      11.1635658022247, 9.85829581618571, 19.6051157658497, 14.7691994389634,
                                      24.4236208907812, 5.27066222863947, 54.9622945719569, 23.3124677767124,
                                      49.9308300392291, 21.5701322686401, 8.7621208754663, 21.7774228848835,
                                      16.4670424850079, 8.41439032382966, 18.6022264667051, 16.4538273674495,
                                      13.5099507002198, 18.0882021848735, 31.7290555673038, 6.49693047007952,
                                      4.06212394193221, 7.92783532261174, 12.0601078478176, 5.59833702815066,
                                      14.9581835801705, 16.0108265232911, 4.18849070734209, 6.88910998678166,
                                      25.3874348909056, 5.68069646036228, 3.25570079265811, 4.35921759319674,
                                      4.85532164093951, 11.6062551090141, 24.3401392273443, 32.037278581015,
                                      73.3568735394449, 19.3981393049638, 55.5389461511217, 106.578597302291,
                                      24.9341995812994, 19.6564256709101, 72.8407996263396, 38.5129838995531,
                                      37.670241626582, 109.42085210395, 46.118638452693, 10.001626528846,
                                      21.7930940792248, 81.5225870494421, 34.0158737777469, 53.2882150253661,
                                      88.2213410121546, 29.3436342589539, 23.8545443241889, 21.3183950951984,
                                      24.4603054463263, 81.4006471216464, 41.8689249450732, 103.381249775314,
                                      45.387572171301, 24.1177050792096, 5.3347224252231, 60.9048703415174,
                                      36.1102277727839, 52.6101554940368, 99.4011399033183, 22.7441935115512,
                                      4.39602287279451, 2.16918334691993, 1.43698535125897, 9.26240131503871,
                                      12.5788705332588, 11.0816490691854, 13.4489320678212, 21.2006884016796,
                                      28.4425943578635, 33.8474700021297, 20.3223336687465, 16.4796615703715,
                                      28.0717714636585, 37.2202396916589, 26.1723602770569, 13.3234079173293,
                                      22.0680438834819, 34.5340300934364, 25.5924226945136, 11.4940172709947,
                                      24.7461315257406, 31.1418795492792, 21.0670478298352, 8.5525821356527,
                                      25.3173011978523, 22.3740606879021, 29.7942620991554, 6.08443492127209,
                                      9.3994555242904, 20.5099524037768, 27.036620929927, 17.3792728463426,
                                      19.9799000195639, 23.0556603316506, 35.9149082974378, 22.1308378063143,
                                      16.5900151120715, 25.9036755200511, 38.019870403305, 34.3829376447212,
                                      15.0789012570907, 31.9518121945501, 32.1474867281215, 25.7077523086558,
                                      16.2857164167114, 18.7831932371039, 25.4331643450847, 38.1123763032872,
                                      19.4922716185459, 13.1453306187309, 30.1629708242676, 28.2637851456999,
                                      14.4327426042777, 9.93996543336002, 16.7398872818597, 30.6219451107968,
                                      32.3173506691007, 15.878606447174, 17.5372357123587, 31.6530635531875,
                                      30.4788414577203, 16.0146907682333, 15.4938032667051, 18.5666136573465,
                                      25.0624621356757, 25.0546275970357, 12.2783420783152, 15.3846357920399,
                                      25.1953589180743, 20.1064735061058, 19.5126514668945, 14.2473522149206,
                                      25.0376281035936, 28.2693867711838, 15.2792678909497, 15.9316742352286,
                                      39.4426449844201, 33.8567585492251, 21.1077244731839, 38.0006552902675,
                                      18.1009995889644, 40.7642294523929, 21.312157805313, 27.5306857586399,
                                      26.9133219864486, 18.6182031603674, 23.7843280223451, 27.8316318014247,
                                      23.4558532012595, 17.8594225933956, 21.8350467963462, 22.181202279069,
                                      20.7481904150755, 26.3740494761608, 20.4388414305759, 15.9216230793907,
                                      38.6061814880314, 23.734446769505, 24.2997256969378, 50.889894613467,
                                      40.5197997179663, 15.7654418993306, 22.7574045444666, 19.5373212022509,
                                      18.251900159109, 23.4875356322291, 16.8495888054967, 19.1521224637205,
                                      27.4136129105453, 26.4717731610354, 28.448596723957, 32.4130905588604,
                                      17.4462858563779, 24.8284210520023, 32.027575715696, 22.7475293369294,
                                      31.6379931458121, 33.0028502046156, 25.557859889936, 41.5806191073682,
                                      28.8631201226275, 28.6163343150286, 36.7192497661632, 34.5552413935659,
                                      18.35316049733, 36.807078719455, 32.372960138767, 25.3439541050569,
                                      29.648303105275, 39.6032560125237, 30.2174541861435, 29.3008842065383,
                                      26.9262162688628, 35.830089619838, 21.2688825123422, 27.0112162460496,
                                      36.1459885483666, 37.9582922334385, 39.1518495507579, 12.2311219731833,
                                      19.9556457069925, 34.2060470107227, 26.1510934385528, 30.2287209567148,
                                      32.2439551815245, 36.3356751971957, 32.8145387292709, 27.8569304989259,
                                      25.251182007238, 29.8896221407053, 38.0244487597937, 36.7419697762955,
                                      31.899145092012, 41.9326843526796, 52.5344103111774, 33.3209414879914,
                                      49.8600297648813, 32.9295582977475, 43.1063683970009, 46.4435171149233,
                                      39.1447985324694, 41.45169447736, 3.40428671892886, 2.44495167281817,
                                      1.96347346341904, 2.22073899626007, 2.60751827106251, 4.07949723777214,
                                      22.1861525861236, 58.7047214211325, 80.8127482631969, 11.6341096179109,
                                      46.8662126038668, 64.7658004272583, 51.5965040546421, 25.091396286408,
                                      29.9748988941402, 17.6827107800115, 63.351016868356, 52.9188655003961,
                                      37.8370519096924, 73.320924879837, 54.095809741808, 1.20132927612014,
                                      0.693232948605804, 5.63755859356411, 78.5256255073827, 34.2456130297465,
                                      17.2248015354049, 23.6550395969728, 6.91437421907966, 26.8836629411425,
                                      3.31029870643822, 2.24720839309565, 3.41983755307677, 5.09298941529081,
                                      2.79303194063684, 1.15474498190904, 1.926311098574, 4.15002020196394,
                                      7.51065875740839, 5.51700748691349, 2.15815598789842, 3.86052456965665,
                                      12.6955681703031, 14.8095870221375, 15.8724185344679, 48.2295046755724,
                                      48.8310032467115, 30.6148279166019, 47.2693542608894, 35.0982010013387,
                                      38.9635788065118, 26.7538755938139, 16.7906884520424, 33.0119850364749,
                                      21.6577935262517, 25.114023873731, 40.4046105604011, 8.72854591342948,
                                      19.2759872784762, 90.6030197465635, 41.1195722233883, 17.1430971225772,
                                      10.8842252930231, 45.6462789664609, 32.7654243259753, 42.9896631691273,
                                      3.38212683650957, 4.30477651139325, 7.85626634648753, 6.49238087914437,
                                      11.4545162215353, 25.2864344271036, 28.5207627711309, 53.6288327573206,
                                      98.0776266757847, 35.4478806884017, 78.8237253374319, 35.2653688995964,
                                      36.4498087645526, 3.2495443461759, 9.41660156633729, 25.0882909580457,
                                      9.21892982301766, 17.0180228919238, 71.3504695465058, 6.5419111765598,
                                      8.09090996485868, 15.5770916476268, 8.26712582439244, 21.5422846293126,
                                      58.3156839382712, 89.2343321875718, 40.101163824005, 5.53256806177044,
                                      11.3485282407717, 9.15496077513001, 15.5646941627086, 34.4362185570647,
                                      7.63541331124846, 20.3165176768481, 17.7023635300402, 36.6415851625004,
                                      79.291692344912, 58.1164874697848, 16.3125038778001, 7.1954944612618,
                                      5.39684569579488, 10.9363260224805, 8.62973656394433, 31.2873564095691,
                                      6.07541290276107, 8.32083754450216, 24.3213639300431, 6.4485596191326,
                                      44.8393225375462, 33.812806189903, 13.3456645209728, 11.6539692697086,
                                      5.4057090135847, 19.1735305279874, 5.96686792724368, 21.6994799082263,
                                      41.1581574747055, 11.9286170033469, 13.7364547313163, 41.6778594768223,
                                      14.7255836671177, 4.2072513334276, 23.4092832984186, 97.1155270970699,
                                      20.4665282744136, 10.392009570844, 16.1790079229072, 5.09281688592871,
                                      9.14049500943517, 7.57614607310018, 22.5444981889249, 19.870521819019,
                                      18.098160050327, 50.4106105054824, 61.2847448760084, 33.6797663005374,
                                      15.2387337725131, 2.56530023407481, 67.7887601790466, 33.4547772361898,
                                      6.97477539889059, 24.6683432476064, 7.35868543524461, 17.614987481101,
                                      51.684511300117, 44.4528093154237, 15.8271189556302, 44.8554806725572,
                                      15.2152018659958, 12.7089563442395, 25.3212333285808, 7.75999686026971,
                                      61.0635323482071, 37.9143675416556, 13.3186293064769, 3.86232344286438,
                                      61.1801690066971, 6.37908028515897, 32.4937840981548, 14.1668891442028,
                                      25.031056677502, 52.6315415568828, 27.1026850012037, 47.3070611412515,
                                      71.2122628715911, 46.5282526605704, 29.2183357000673, 66.5613640881522,
                                      42.5404208592293, 32.2848262479115, 3.75626596645071, 6.67382642710814,
                                      65.2736359887901, 1.33580892894521, 1.41277127322045, 1.16684281196404,
                                      4.4079669992909, 4.11879116222758, 8.43435091959149, 1.49788032739782,
                                      8.02500412040932, 16.1913127546241, 24.1381664879373, 55.7706775337815,
                                      19.7424546776205, 14.0543563295332, 22.4776245645566, 25.5717491653856,
                                      41.411163337681, 55.1453262342386, 12.1897797502094, 8.14732216298476,
                                      5.81007326539827, 18.8408872040454, 13.9982405671816, 7.65659650141585,
                                      18.9328843669527, 12.0833414873093, 25.0259354428081, 20.1178548450797,
                                      1.69878694834374, 1.24943526602462, 1.11494753051082, 1.67068143089193,
                                      1.11107049996135, 0.918538484581192, 0.745757195239818, 1.48382262920829,
                                      0.739048311611416, 3.76155224402566, 18.4396762870299, 7.47854148283548,
                                      11.8337420493599, 21.1534295188092, 10.8351029141377, 16.6354766651713,
                                      12.6409412191916, 16.3604994766926, 18.1180924471319, 22.8273758628657,
                                      33.096858034449, 25.2240484170265, 15.8153235718931, 11.6883110846355,
                                      10.4737030605676, 5.7034285601977, 80.0414482701744, 19.0896244695008,
                                      46.880444057326, 30.6460665165709, 20.3794709162032, 6.35582780582549,
                                      16.0886761481759, 20.1398610735182, 12.6568746272985, 5.76621588987646,
                                      13.7839827755768, 44.4801281739744, 32.1808710233055, 33.029742205914,
                                      29.7043816913432, 24.3859826041188, 21.7085054050866, 46.6683886974296,
                                      24.6065297211372, 7.30544443549085, 13.0846704249917, 21.0851360291056,
                                      7.28527877535179, 89.6953966092746, 45.4643645610935, 40.9103428837706,
                                      33.5127735900884, 20.9099676946972, 10.1986824341078, 5.06068738727646,
                                      11.6400548388381, 20.4458356005988, 86.4876218099144, 44.2238882929821,
                                      48.8234206578199, 55.4858535674999, 39.3051171818772, 43.8574094194328,
                                      25.0165149785282, 15.5292347226568, 13.2846129249584, 21.0914959987,
                                      15.887973392308, 2.6063326400545, 109.291880439173, 41.6303480225962,
                                      36.799258128503, 27.720500631044, 12.7128718814315, 21.2042983283046,
                                      50.7555864768352, 19.0137884924959, 12.8242327834902, 44.2756688155138,
                                      12.4345425696691, 10.8686548047347, 4.73103272245725, 4.85586223586774,
                                      2.21534846906346, 11.4334203759812, 7.67836909052321, 34.2315809898426,
                                      6.66411495804523, 20.1821633527509, 4.65041478290516, 12.4673972156379,
                                      39.0843408852474, 8.02444016800778, 20.0430007793876, 24.0503166410223,
                                      28.5238867762702, 24.3575592881458, 4.98657975267773, 4.30914729858137,
                                      54.9554153141778, 39.3074452083147, 95.7056894065602, 15.7619660469789,
                                      11.8196640818998, 3.45440731962218, 20.8172915628356, 61.2640347990567,
                                      94.4724082391224, 18.0985232563611, 17.8441645323643, 7.76272152388692,
                                      5.13861330825024, 56.5181655286762, 20.9611400719515, 19.757327458198,
                                      22.8792386421915, 9.90254447489055, 10.3881021419976, 11.7309866534108,
                                      6.72478951597806, 21.0309389988626, 11.3730424905528, 13.3367302177797,
                                      24.0099980599594, 15.2853903546307, 3.21017372018913, 25.8639608339332,
                                      42.0057012159143, 61.2369226434801, 10.2972348721948, 23.9677926371174,
                                      5.49553948899893, 3.37489140764686, 1.91272967380407, 6.27355441476734,
                                      21.2718164898493, 27.3152383698305, 51.7215400241623, 43.9568714176496,
                                      30.4582902477842, 21.0511247319635, 12.937372226285, 28.9913086583173,
                                      40.4721741562347, 15.4127020178794, 16.1245519917372, 20.1038293575228,
                                      19.9958325707296, 24.2721222559154, 20.1179128515144, 10.6609882837041,
                                      18.6500975184452, 13.0641681161879, 17.0888987575219, 20.6650894890517,
                                      22.4432854290755, 21.3090672609618, 19.6520596491983, 13.5749695727624,
                                      17.5444060120797, 10.5694903871706, 11.0519914516747, 11.058217114397,
                                      10.1591941276979, 19.4779362191158, 11.9960588849877, 12.3850495195747,
                                      28.2453516348646, 23.2288710611883, 12.0178537303116, 14.1360096048427,
                                      10.8721652980323, 22.9345639770676, 13.7165726060416, 16.0447389476451,
                                      13.1465233415878, 23.2816376705811, 12.8917768617909, 20.7533699207926,
                                      20.1111318715216, 23.4071926079887, 15.1380079491768, 27.4453146921339,
                                      17.2956305706464, 50.2690563962549, 97.2374649798215, 12.5946108071368,
                                      15.1433300725504, 15.6195475202505, 20.1624932846613, 12.7897552833052,
                                      19.196430484138, 9.49677740981149, 29.3573162112291, 18.1756158807604,
                                      23.9698384838945, 10.5192186978411, 15.6130279010817, 19.7822509869596,
                                      12.9578377307544, 27.6926387694916, 25.468101046844, 31.1920766448789,
                                      17.341852217746, 19.3704093312564, 13.0322518775829, 22.4687869568046,
                                      38.1355148315356, 42.1958057795074, 30.8986957413054, 38.1788907931464,
                                      34.7104730199992, 29.9270740343608, 34.9117247570534, 34.1835944031738,
                                      27.8667613962327, 27.2672543372236, 46.5636962006209, 35.9639869569992,
                                      40.3641516349612, 33.837473658816, 35.3933617518051, 36.7189377263546,
                                      38.9477328451857, 35.1715578730783, 31.094536004833, 40.130707160746,
                                      28.3999746605627, 35.2439497886675, 38.7102418524366, 40.6471677737666,
                                      38.6233194231093, 45.596467254927, 32.3904625257627, 37.605192161734,
                                      37.0609317430839, 37.6860855258988, 37.0448804959587, 37.8792164747921,
                                      32.1958444703742, 43.7265397903247, 44.0606516382855, 38.8082913977214,
                                      34.3574657458821, 44.8169824176953, 30.9847572333694, 33.0630896293879,
                                      42.6967363134633, 39.7731332375143, 39.5446658194207, 37.7204284507384,
                                      21.2403493932984, 14.119246221156, 6.2396546737327, 6.50260216996133,
                                      5.86436235635304, 7.61087028442689, 6.97877977744055, 5.50106490215343,
                                      3.36316224771025, 7.03494286108746, 4.60662032400285, 22.1003034858659,
                                      18.7496101943068, 5.51416715359425, 5.22257878429994, 7.05776690852862,
                                      6.56778191825568, 8.90553032668627, 6.78107433866923, 4.45174939202877,
                                      4.61123469830139, 3.93327778989302, 5.61269701863563, 6.06065602871078,
                                      6.56216845817786, 8.08539429733563, 6.06665520330431, 4.62106826678058,
                                      4.30554244957806, 26.2451453032626, 12.0244749267608, 9.81423826585553,
                                      16.0443798709565, 17.7088339177545, 13.0922939820745, 19.6836517664531,
                                      23.8269697429761, 8.49182993145687, 13.8684026405664, 17.6318894051126,
                                      8.32574232377718, 12.510082721523, 1.96987516025554, 1.70933795139901,
                                      1.35280647272657, 1.59590684434465, 3.30102217014786, 4.04780174105098,
                                      16.7559656960915, 37.1030454253825, 45.8228313983283, 13.498900977836,
                                      44.2754698191936, 55.0476108225973, 47.3796978631141, 21.6128259792211,
                                      51.636902648625, 41.0687839336137, 34.6739302273129, 69.2107456834708,
                                      19.2134299873412, 47.3204642366405, 45.1058584278254, 0.510820207466886,
                                      0.430174319257454, 2.01196277998339, 6.34877629128149, 28.6110238390975,
                                      7.97428935654204, 8.70498907029543, 3.87942023996025, 24.4876419419616,
                                      3.37674554168994, 3.25818011205241, 3.83390427532285, 3.91186103124461,
                                      2.80348123263476, 1.06439836733669, 2.02019482899829, 1.42834214732602,
                                      3.77746493073786, 2.56936512139147, 3.83988073915273, 2.21733903493686,
                                      6.47233408524593, 10.4615511056767, 22.0813941374869, 9.4513943119761,
                                      25.0460316604723, 14.9760332649694, 11.915654858827, 13.0035667273085,
                                      36.101580715283, 15.9326833538546, 13.3380708180637, 16.9787664323478,
                                      7.1645138449286, 12.6490375320853, 8.60048201332169, 6.4308250610483,
                                      2.23207722042303, 14.9323402157256, 19.8843336445684, 6.55695976666877,
                                      2.06471780145448, 4.58276306688795, 11.5022267020137, 27.9604642814407,
                                      1.516295230894, 9.55227299111653, 19.8969156596478, 5.08276468314188,
                                      10.6042853863174, 17.5764246766056, 25.6023658297965, 16.6982723560201,
                                      25.1567841285155, 14.1850955869902, 14.8069049064746, 13.0799549869156,
                                      8.59805179937968, 2.39721713509492, 2.79739978340571, 38.980266391053,
                                      7.06568990842562, 8.52594473971279, 26.9181042416389, 11.1387606428406,
                                      7.25447676072704, 11.6997426227958, 12.5207512620898, 12.9811849499066,
                                      28.4399125518029, 24.8839542930501, 13.9053013749542, 4.39831459455594,
                                      6.19752390966564, 5.16161924215504, 15.7412918269396, 21.3180114485211,
                                      5.38445451872145, 16.1961872669585, 7.05415229222725, 19.8513543641793,
                                      22.1836336688306, 17.1002294227307, 18.5077542420101, 7.07200151528163,
                                      3.19596618944414, 7.28906060248343, 5.79536131499221, 9.24981096952012,
                                      5.96221929655839, 12.4084158547234, 3.13917085921614, 6.05235581835748,
                                      25.9960438876982, 12.8154714848729, 4.69268388429408, 8.77910320640971,
                                      4.28707122923429, 8.29677896773616, 3.78198470103592, 8.66371253697702,
                                      8.01631778685287, 10.2524537156398, 7.08931895580964, 19.4851429341136,
                                      6.19503756210647, 1.92274761020901, 7.79368146099279, 16.7976575413168,
                                      6.79479152920733, 4.33930045033006, 7.22245992632529, 2.78431562870528,
                                      5.53929350809003, 3.83428893171683, 7.12229604295219, 9.10045901004813,
                                      11.1409607105586, 14.3861221608811, 17.5153192678525, 9.3255615137515,
                                      5.47742524679198, 1.28902982936524, 10.3779136917625, 10.5474775998146,
                                      3.52557631297545, 19.3112364929116, 8.14156723921795, 13.4948051893077,
                                      23.8197195969328, 17.9729030629713, 11.914767022234, 23.0427118431662,
                                      5.46726601159402, 5.42960787660409, 13.7440615336382, 4.02193226826587,
                                      8.94586604108559, 5.38328563659047, 3.27133654655751, 1.76402474384233,
                                      14.4101920502458, 7.47885504690318, 20.6553504583264, 14.3320835430713,
                                      7.48094661401903, 17.55857618986, 18.9051099182043, 12.8446234632149,
                                      25.7574489479357, 18.4339173300563, 7.82487539997105, 7.82730571565632,
                                      11.9365534896347, 11.0411435298755, 1.84674608732673, 5.3223468101016,
                                      21.7264547621002, 0.788821902190495, 0.758139954633769, 0.897555335667451,
                                      3.18485897142605, 5.98312738283181, 3.52608536855951, 0.598684147755363,
                                      6.98758852414714, 10.3020008864033, 17.1360211501621, 22.5222061560058,
                                      9.15013312933411, 9.51295914830501, 11.1133162412068, 6.88198506474163,
                                      19.0681394972728, 19.5757002026107, 5.37421898975382, 4.14392586925643,
                                      4.8202063574407, 8.73675773149765, 6.72234173228139, 4.45436502993798,
                                      6.89702595080672, 8.40122087055837, 20.142763137905, 7.99798339296236,
                                      1.2339287320865, 0.399428798224598, 0.412483543387143, 0.408666455081824,
                                      0.404789321067249, 0.338871162940852, 0.549637850641808, 0.545785230997971,
                                      0.473058663512668, 2.35223255986659, 8.49915934008535, 6.7650441341121,
                                      9.20310314941052, 7.17455070599171, 6.13979593180421, 27.577567000313,
                                      16.0555845667261, 4.59493596530914, 20.7912577407723, 6.45395664779131,
                                      18.3497950345709, 17.7584667074568, 14.3944864537201, 9.97172461374401,
                                      5.51143318688201, 5.13149033907517, 61.6228383354348, 20.5220041859388,
                                      58.9596815694416, 29.0884092828779, 21.2466026970749, 4.87460895177441,
                                      15.6675559504133, 11.9804913702098, 7.27026239891399, 4.43302315866499,
                                      45.162157464497, 82.9395954219955, 55.8688058112773, 25.0638037037655,
                                      28.6032535248472, 26.9047363252644, 15.42894259267, 10.0411105788073,
                                      25.1113549903521, 4.91841785274322, 9.08642088897977, 32.8550062911269,
                                      18.9961904269469, 68.0535254175543, 26.8136832318383, 25.7606708893535,
                                      26.9953146199567, 23.0316593233863, 6.0817278110382, 1.51325984070977,
                                      6.31740391952093, 35.286200893401, 83.0967710584638, 29.7096514373333,
                                      17.2382336463943, 35.185796518004, 28.2710714397547, 44.2201893507465,
                                      28.1350015988544, 18.1286512690528, 15.1292243340488, 9.13579837480433,
                                      4.90642206058538, 7.61868021052885, 80.7334933769875, 54.5557213298933,
                                      18.0408750351329, 28.0133981240051, 13.143912254181, 20.8862049733883,
                                      19.1245746364384, 12.9813976999359, 14.7752209950803, 13.3749163617735,
                                      13.3592611059057, 16.8453951652413, 3.93440045365586, 2.83792965081105,
                                      1.04166010392779, 20.1291151990086, 1.84366865715715, 1.99897580497097,
                                      33.482188628229, 8.6529772498012, 1.22982530837491, 5.86928191651357,
                                      11.7345818273117, 2.31791780911891, 2.07696959936496, 3.17280749584889,
                                      1.47456790314416, 3.36481020489776, 2.61053133324439, 3.71584674663277,
                                      37.6473006680447, 4.20118928549966, 27.1339064293579, 20.6500713652122,
                                      15.1073646399182, 3.64091879327643, 15.2051717834474, 14.578602501948,
                                      40.1726899322505, 24.4824928305887, 23.8987189649932, 8.27642292153165,
                                      3.73819505015876, 18.0689644938433, 7.55408373149603, 16.8464280155528,
                                      10.911753426291, 10.6013791186544, 14.1081453220874, 10.4827335136871,
                                      3.55194556437303, 8.69101074864842, 8.88224506620091, 22.4050553424077,
                                      17.1824642929622, 10.5019499480341, 2.33872198621384, 9.08643452513692,
                                      23.8487872957193, 36.220649906121, 10.8010092339417, 13.9668945155968,
                                      2.82831812103995, 0.98371690239425, 0.563653195297309, 3.49997855333371,
                                      8.10745102178658, 3.95946990676697, 6.62157333101493, 12.5929263270732,
                                      14.8841610747592, 15.9254822727341, 9.85592742785385, 12.0387440491848,
                                      13.3611585494106, 18.062560628069, 17.543066115688, 14.7318164138473,
                                      15.8447012229328, 22.5143558920977, 20.6152831207158, 18.0040479728422,
                                      18.2106228706323, 25.7383777076385, 22.7758720358281, 15.7455721442193,
                                      13.3927496147344, 14.0398172473345, 20.2145995673521, 18.6553102470264,
                                      17.8600097712076, 18.4357316354128, 17.0168044897246, 12.0746640005669,
                                      10.8421366485217, 13.7421978441831, 18.1083145536666, 16.6660831521373,
                                      20.7083211810326, 15.091546512186, 11.466506551615, 11.7487784192217,
                                      15.3011663148883, 18.1850647528899, 17.1582765956687, 13.701556444401,
                                      16.5480727026468, 11.9780547349467, 18.1124095891263, 18.3935721187501,
                                      16.1315563881592, 20.7154140193466, 21.3965398481871, 17.5023489527043,
                                      15.0987543090655, 10.3337329003668, 11.5254308374279, 13.5605539876291,
                                      18.1976280000315, 15.2301511486265, 19.6992603337672, 13.1057016047905,
                                      19.2589215038813, 15.8196729991514, 13.2096609627287, 11.9332801586809,
                                      15.0083732381399, 11.6878647130246, 11.7571624924232, 13.2421156871185,
                                      12.8283231458491, 12.5241145795662, 10.809899485965, 14.9942070819642,
                                      13.0298183457525, 13.0459089562521, 12.6574496722613, 20.2080322851014,
                                      22.9804213757419, 28.319502920661, 26.5644548045606, 27.7500830590987,
                                      26.2402694788508, 24.0055478784926, 23.8518154646613, 25.3574922591647,
                                      17.8082785470377, 21.3142059486977, 19.2107252711041, 19.4695508736879,
                                      22.3952196910895, 17.7062619862081, 22.6494519724865, 23.1613689310856,
                                      21.0249428050128, 21.6916209078883, 23.2233829298718, 20.462271074403,
                                      18.9821361640191, 26.315908287649, 23.5118569038911, 19.9304910805437,
                                      25.7095685361719, 25.811091285194, 19.7491770606153, 20.4409733464289,
                                      15.630922762716, 20.4643941501271, 23.4770863331708, 24.5381249311809,
                                      16.0898671390793, 26.9792201324379, 19.1335656822066, 17.9825553875148,
                                      20.4620204099255, 20.7429336265211, 20.4255651138001, 26.8287591430786,
                                      21.2055995021771, 18.0427724465059, 16.627293826985, 18.2137683889369,
                                      18.0518685089462, 9.86003785342822, 4.82418519042611, 3.41120672316081,
                                      2.7670581152944, 5.80326151348035, 3.63964681142901, 4.53837601540403,
                                      3.44138820433912, 3.66655359512838, 3.67441878887459, 20.7159785434213,
                                      20.81954809277, 4.07225905623119, 2.47687466365466, 5.74029146888753,
                                      5.14937108396105, 5.0794597033634, 5.4181888487251, 3.51074066578387,
                                      3.86573880737239, 6.71294012955489, 6.12177244628754, 4.74089114125726,
                                      3.24439465191173, 7.63550794451947, 7.26296131744588, 5.4218502939393,
                                      4.13183039278235, 12.3968276358574, 10.0250541052532, 9.16447538002099,
                                      10.7186567343376, 8.15439085278587, 15.8347528002251, 8.79118506128393,
                                      11.480500491217, 8.98554484848068, 11.5188819123145, 10.9620881360743,
                                      11.7379158927213, 7.38909518711551, 1.34059112283456, 1.15411721342024,
                                      0.839297451618237, 1.06853611748325, 1.20668243052539, 0.980174433935454,
                                      17.1655184670921, 18.6269051618214, 78.8775014235183, 10.6413547866274,
                                      47.1936250339539, 29.8632156372792, 17.4512932079701, 24.0358168764227,
                                      11.9703174277324, 16.6925802102072, 22.2991382785919, 68.0951829027869,
                                      27.6815188994442, 49.2436283330963, 33.5641362073606, 0.901367297215081,
                                      0.987455222215524, 5.49599995483591, 40.5174752672991, 29.7409669845892,
                                      22.9399613494749, 18.8634603312374, 12.0851313804919, 79.2813868249368,
                                      14.4014962009172, 7.54527392505454, 4.01144112417097, 4.91918671051937,
                                      2.53908359984021, 1.0120601584359, 3.80819899752442, 2.73197509078326,
                                      6.58767074664883, 5.7602762279915, 3.90190773073144, 8.58815246754119,
                                      31.6953483549069, 19.4155520350783, 13.6097976223561, 31.3192532585085,
                                      62.34374799843, 61.0593406753015, 20.0167957418794, 21.0546478515228,
                                      39.8386974241372, 23.4156445081431, 11.9641342408894, 32.7081677307669,
                                      21.615870843881, 29.0741496167594, 39.5083861858288, 9.57468109621503,
                                      18.0096414682265, 41.550476232005, 33.5833046888662, 23.5358918359976,
                                      10.4561129649336, 28.9952003299521, 31.737515383725, 79.9303617399283,
                                      2.68086487201566, 9.12722655257352, 16.7098810800298, 11.3237857460639,
                                      35.7093991395384, 61.9989260397707, 33.0871539103041, 20.5973319829366,
                                      41.1444521419629, 34.9109096294894, 50.0551755984631, 19.9178399098131,
                                      31.8141383057299, 7.2547269690634, 36.6319398635414, 96.9653687016643,
                                      7.12050459236919, 17.258309373712, 41.9056660226447, 8.97391534095415,
                                      16.4472194385612, 10.632746581911, 16.5127340402175, 45.3986913584546,
                                      45.9588447601134, 50.1249890493948, 36.6456168019048, 4.51220499559244,
                                      11.685994328153, 9.87283487482026, 21.481630355573, 78.6560068229856,
                                      7.21753400977234, 16.866547652572, 14.7347015485495, 41.1331724629411,
                                      51.4230536832555, 49.2688615150689, 35.5955949911102, 13.4065729341413,
                                      3.71997489840016, 8.92326077585017, 6.23279807351048, 30.9559075553475,
                                      7.10042432022461, 7.10967467775662, 18.5587937265002, 15.9792855260555,
                                      62.6293482644712, 47.543913638303, 11.015850518975, 12.8281401928686,
                                      11.5646525241606, 16.880046912752, 9.1942039672383, 19.1710073902915,
                                      43.3366563348367, 11.1709130261917, 10.9653217463643, 26.6494329196859,
                                      12.3615665795549, 3.09017913053045, 14.495119629155, 51.0966190422463,
                                      14.0221407257435, 5.34888294448171, 17.8445888162382, 6.01346524266738,
                                      10.6804270697921, 5.01890018149883, 23.3683010158218, 14.638920779406,
                                      8.52808289589115, 46.3195501642028, 39.0426660008987, 24.8234163558214,
                                      14.9764905495122, 3.36940635254319, 66.8497013795621, 29.7265587731692,
                                      15.3418701364979, 67.187215684636, 36.2311436705153, 23.3442592010258,
                                      51.1460120758509, 54.8984054581748, 39.8719455075518, 27.7211794655805,
                                      5.40215835142615, 13.407096316073, 15.1625804442332, 14.8462143762567,
                                      49.1462488092597, 33.1995631513183, 10.3103345563063, 12.596011449591,
                                      90.7256434662271, 5.2501921619614, 14.7390959404039, 23.0012809011161,
                                      37.139468105952, 25.0799706467018, 30.1129446011532, 30.4437150820804,
                                      51.3406384515103, 43.7519654015794, 27.1546989565435, 57.1539286347861,
                                      30.9222979554905, 28.986376728953, 4.66496959624072, 33.8126346728405,
                                      80.5256663778179, 2.19368265562429, 1.68207446784444, 2.79617542617397,
                                      7.29115159306025, 5.7104589262147, 4.86456186608997, 2.29113272584745,
                                      33.3373832923201, 28.7894938413713, 32.4540938454246, 45.3961616786097,
                                      19.5835561331614, 20.1618216882636, 10.0625046473235, 19.9153721749762,
                                      30.7778829592444, 41.7478193052805, 10.097275562945, 5.98059197624376,
                                      7.41427287675389, 8.27656556056037, 8.34190817407194, 5.76138293557799,
                                      15.5316689805335, 6.67001523989571, 14.947878029014, 20.3302807975778,
                                      2.24091766378655, 1.12193383102092, 0.79699320543769, 0.914531404527783,
                                      0.976957532652818, 1.3457838820461, 1.34666013254838, 1.11702531672795,
                                      1.00051826591227, 4.61770921561265, 19.6937163180398, 7.59117404264844,
                                      8.8162467116104, 10.1890294762068, 5.13740135843919, 28.0055599878413,
                                      19.7397727652023, 6.27115121736017, 12.5090005473847, 10.4646862877765,
                                      28.251893641939, 21.4585705135435, 15.78504761823, 15.4833928715786,
                                      8.59944810586816, 9.47302930992552, 100.874125235162, 25.4931706860454,
                                      44.2373272456421, 28.9786886250901, 18.1313146269841, 6.19204389719904,
                                      16.4690094301615, 28.1117268221113, 12.0547344962035, 3.96719171652308,
                                      50.0913961348692, 77.8012655403922, 36.8090946305365, 21.7060931084405,
                                      21.0154734835802, 19.6527207895688, 18.5872728844951, 19.6913395540821,
                                      24.601956718835, 7.95032026805569, 8.19879256542592, 36.0054225481597,
                                      17.706678620068, 117.991722522107, 24.8912659951228, 23.1266686645319,
                                      31.8629927077285, 16.6410708962215, 11.4178441642735, 3.9866862076604,
                                      9.9080374328424, 40.2563142408321, 81.8447067148264, 37.5375361324702,
                                      35.2811718802444, 27.2921081273829, 32.1760010941184, 26.911925284092,
                                      19.1611404754216, 11.0135198096534, 10.3943412774264, 15.0958255434034,
                                      17.3301600544777, 7.80412186662827, 100.322291459804, 29.7734489304213,
                                      42.1574033195224, 17.5385148469209, 12.4693464969682, 18.6432067345601,
                                      23.1321148276416, 11.5965660523921, 15.4074928621151, 30.0312464846432,
                                      9.89756820045725, 7.81879130092167, 20.0230856564647, 4.04616090239452,
                                      3.24598079826706, 14.2461741327858, 5.35241042748389, 19.1348927794505,
                                      33.2907759280496, 16.8886001279536, 4.03649168363785, 9.5257569273928,
                                      24.7402822495702, 7.17772744267039, 18.8711733514001, 21.372635584506,
                                      20.545303101312, 17.345994878018, 14.3370387380309, 31.9709650303571,
                                      80.7846562706238, 28.4595959734772, 50.7866183892786, 49.2230243826115,
                                      14.8907383597345, 15.3695188043098, 46.5161744632937, 30.2218026568483,
                                      52.195951486162, 48.4789351596366, 35.6230863054523, 6.39935781142208,
                                      17.302193595966, 75.5803498210896, 21.6823938672875, 33.7741486868431,
                                      42.8850561364189, 23.3932037558535, 14.4541461903545, 7.89472025080507,
                                      25.0877280005928, 65.7743945605164, 23.2862081778564, 54.5988166104727,
                                      34.1774760207399, 12.079905608067, 4.77962625910574, 55.0751693570412,
                                      33.2178473779839, 31.1814621921803, 45.3833999406639, 12.2163730472331,
                                      4.01546648457246, 1.61769160590898, 1.35528945593338, 10.683063787268,
                                      16.4168431252779, 17.287239043504, 29.5951406201473, 25.2345878125062,
                                      12.6416592225803, 15.3908179846589, 12.5995809228596, 14.8132189455052,
                                      22.8550990416265, 16.9397880568044, 17.373837181513, 10.1437206520745,
                                      14.5800004584018, 19.7723142157625, 18.7258448871893, 9.24171988643507,
                                      10.706841751138, 19.9253665813361, 14.0492914164497, 10.4546323991881,
                                      13.2344822563222, 13.7638253433073, 17.0131847090571, 11.9217247048484,
                                      12.880360435345, 10.8155192900395, 12.520739306752, 12.0530059272053,
                                      12.1865689851183, 12.0446219701069, 25.5238186268238, 15.8087648805811,
                                      14.634643151566, 16.3771623659415, 22.1391037090779, 18.288929543947,
                                      11.0102323456512, 19.553828504143, 20.3452336460589, 11.7466095378217,
                                      8.15665973708705, 7.86674517798793, 15.2929735472184, 19.964617604702,
                                      16.9422093877655, 16.5554823554475, 20.1370318064074, 17.1192707680214,
                                      9.2942266088118, 35.0443987092029, 39.9809526941394, 18.3947686561668,
                                      16.102133525365, 8.97447211402219, 13.9835196546349, 17.726997139172,
                                      17.9287341102015, 12.1608509342784, 13.5220631708008, 9.64327170794558,
                                      13.6499243698513, 13.9188648226246, 10.5418550083996, 9.43071268046893,
                                      12.75820795414, 17.2579075436621, 20.7145321185805, 14.1352871441338,
                                      12.8592129226577, 15.7748474997607, 8.70097139246582, 13.2474533417256,
                                      21.5400526248871, 25.2760185024211, 15.1815508242243, 25.8104870691143,
                                      12.0048571261435, 21.5579825945106, 15.5873619617778, 15.8128759869402,
                                      15.456847910864, 13.7304575324099, 21.5969187535299, 19.3899150811186,
                                      22.8877414255368, 14.2207876767135, 15.5664194095395, 15.5104700185469,
                                      12.6768493635911, 19.9363769353519, 15.8332941903159, 17.0085449342405,
                                      24.5992543100616, 17.4250402191458, 17.4668275808885, 26.1141100703215,
                                      29.0448028436653, 16.088464167698, 16.6545357126684, 16.6407314515463,
                                      12.1585136441633, 19.8275195614321, 17.7146681518687, 15.5824767846245,
                                      14.879441631792, 28.5947501160052, 16.1406193895056, 22.3482087094526,
                                      14.3186277897333, 14.0541060193996, 23.6326349479829, 18.1289197209889,
                                      25.1481218737453, 23.8098264205415, 14.7147884044991, 35.1448366814827,
                                      24.0176333297426, 27.2799271516508, 23.6117624315456, 21.6900642996189,
                                      12.6352309676034, 22.0536734466341, 23.0976124675158, 12.4698808727532,
                                      20.1837132780082, 18.754266336497, 17.4720678389394, 25.8239631605816,
                                      18.472295262219, 22.0388109071663, 18.4268958618592, 10.7866640113576,
                                      21.3697250066299, 20.453143471715, 20.3944321342985, 8.96483479685275,
                                      9.19569564695268, 21.8447810088819, 19.0725647944885, 18.5745360162862,
                                      23.3797809871383, 22.2632162732163, 19.8492208125631, 20.6644647150121,
                                      13.2308219123103, 18.1469777575954, 25.712931684351, 19.362534093706,
                                      16.363536943135, 27.681570647404, 34.0817554646129, 17.8141895397168,
                                      38.736246888949, 19.5660182573, 23.6195765705798, 26.9174376516773,
                                      22.4641488931408, 35.1956521190864, 2.26026836588842, 0.864348831906949,
                                      1.83114747827901, 1.68386198411077, 1.89377342136678, 4.70998273089509,
                                      16.9747114990746, 55.8402379991323, 53.6441356951717, 13.017480327313,
                                      56.5569208790314, 46.3386654451011, 24.4999667446943, 15.6843392417218,
                                      22.9789982889412, 31.9418837370575, 58.4817937560326, 73.6510401629201,
                                      46.0457213734867, 32.5643298316756, 25.8553642664196, 0.377216328308165,
                                      0.385670756759603, 0.417869056651679, 0.0369374053916456, 0.312549520320394,
                                      0.592673715588129, 0.470086389286381, 0.334102653055028, -0.592169714117505,
                                      -0.826425504450816, -0.952089992886534, -0.992101767458642,
                                      -0.880675931287729, -0.939335535292113, -0.949491902703624,
                                      -0.912438804455929, -0.857958636868918, -0.825910188473532,
                                      -0.821687627793468, -0.771945865218278, -0.785574097035722,
                                      -0.140631946159195, 0.703866105283597, 1.10803738833653, 1.31552186328126,
                                      0.768100420412857, -0.0182329128907713, 0.577106104648739, 1.34083158733864,
                                      0.731102542432025, 1.05600457551077, 0.410280687372112, 1.0387999875443,
                                      0.789689685039357, 0.278086321968164, 0.483534120782965, 0.37852309835101,
                                      0.32145625755737, 0.508020536779811, 0.51850676885781, 0.612707042572876,
                                      0.715765733060397, 0.813299811566573, 0.683833208860081, -0.709225878542661,
                                      -1.03057088105218, -1.0442664516802, -0.75693130320349, -1.05656280407914,
                                      -0.129313706398595, 0.28159409677796, 0.89465487584753, 0.479125648004589,
                                      0.564680072052779, 0.72629511441533, 0.445948721450905, 0.102589575396104,
                                      0.665599795799316, 0.556730715053054, 0.472804020454947, -1.5102790264045,
                                      -0.97876510074384, -3.24648708614374, -3.57532210180935, -1.30098803345175,
                                      -1.05256909806155, -1.32890295473732, -1.29572589434013, -0.453020208028827,
                                      0.608713729946688, 0.669149754480751, 0.517806301586373, 0.716067310396115,
                                      0.667710554482401, 0.396741353228218, 0.485743741679463, 0.588156416855557,
                                      -0.122440627709813, 0.828591309516426, 1.71182326176669, 0.947559564600389,
                                      1.31294388623868, 0.884402267347777, 1.2282490604792, 1.59573038657136,
                                      1.68904761348842, 1.68719351293441, 1.77025316832801, 1.15029699949098,
                                      0.878586188626661, 0.783436841740143, 0.977594071070368, 1.00540014027494,
                                      0.708211736223314, 0.748536189926191, 0.529088128561905, 0.723400022376752,
                                      0.183667077896683, -0.0258018684571791, -0.108515678580809,
                                      -0.324958744558766, -0.0640253955200986, -0.0723661997712597,
                                      0.0888265724160236, -0.486821875726552, 0.681612509811535, 0.277467915069619,
                                      0.0547778096220275, 0.25934508019282, -0.0986606193905598, 0.215268207277074,
                                      0.278564884543828, 0.146040009245981, 0.0608579558298222, 0.241986761447615,
                                      0.324373485819542, 0.677686032717945, -0.375330839626439, 0.271834762502206,
                                      -0.308077245591604, -0.11781841220026, 0.0485569491506361,
                                      0.0688050688939703, -0.29032181626865, -0.112871295108143, 0.118885197817812,
                                      -0.801999879462109, -1.01708875210327, -0.0573532825894483,
                                      0.0175740941765744, -0.202339808077771, 0.518098909746123, 0.122328877778079,
                                      -0.29011085635944, -0.195201807274606, 0.174734624334923, 0.119175185412379,
                                      0.151151393488688, 0.0225595772475983, 0.184891734765689, 0.10475258753414,
                                      -0.693855450011581, -0.859187835471612, -1.56388351538142,
                                      -0.720679092107342, 0.224829672408447, 0.190925845367931, 0.0985320823190834,
                                      0.216120875734732, 0.844700121608229, 0.47668386207292, 0.0409018553593238,
                                      0.211541764272086, -0.0212739034914623, 0.255349505672332, 0.198306125945352,
                                      0.208414403240995, -0.702109894013812, -1.14308632029528, -1.14183018435097,
                                      -1.11627574372025, -1.16021684962751, -1.10235874905083, -1.14959655191401,
                                      -1.18035298400527, -0.895379359376014, -0.0301250275774623,
                                      0.344941922093883, -0.0918795614584904, 0.0774875536026635,
                                      0.186627801942814, 0.337954236992364, 0.977822218837029, 0.862573481376429,
                                      0.0116964006620889, -0.247670273136579, -0.360719686996948,
                                      -0.351986977550223, -0.286073963415048, -0.332166996624713,
                                      -0.272109684691539, -0.365824912815857, -0.361690396053647,
                                      -0.632020849170171, 0.31570322267969, 0.451808933905964, 0.452553608128518,
                                      0.42958416045258, 0.39842744962943, 0.380830624744536, 0.410218732048797,
                                      0.428238433710983, 0.392629492683, 0.416015108321735, 0.36440942772672,
                                      0.0587154740724287, 0.299778498674257, 0.0850579002851443, 0.212329994443106,
                                      0.517241683363688, -1.36890534055715, -0.00390335828259591,
                                      0.0547552652867372, -0.217602725930827, -0.671370700586643,
                                      -0.110852138054959, -0.0218392301906525, 0.449475101131786,
                                      0.264015562637255, 0.576899702679538, 0.654963677069149, -0.784401560806516,
                                      -3.80929144745448, -1.51761405378537, -1.16388847005841, 0.341689718731986,
                                      0.0929411306146857, 0.0481649198212289, 0.112808092187549, 0.443850843375906,
                                      0.117833881867716, 0.0258628821424309, -4.03363979657602, -1.43881503888942,
                                      -1.93498691842226, -0.230882727412086, -0.0790429327479149,
                                      0.489165083781835, 0.204736963410514, -0.0210216699372153, 0.127161247584997,
                                      0.0898973569151186, 0.322781046270646, 0.534270780520949, -0.433664928686688,
                                      -3.62346269562913, -2.7192426010093, -0.988215550828907, -0.306742222571127,
                                      -0.240762608286568, -0.345656936557119, -0.317878155945069,
                                      -0.0384022493416026, -3.74533704260741, -2.70895429686943,
                                      -0.648107833051636, -1.49107425336692, 0.186216147639803, 0.141746111104048,
                                      -0.178571917247293, 0.093105065440269, 0.0620595734429692,
                                      0.0562753600484299, 0.12383731643874, 0.0330863080545445, -2.29336829540754,
                                      -3.19401693863226, -1.72054004556526, -0.518609139564788, -0.693551640923558,
                                      -0.329506818917689, -1.00540600431594, -0.106924676164036,
                                      -0.489133848918225, -0.378981318152363, -0.488278468445457,
                                      -0.485436699416531, -0.370573168538028, -0.385042164058993,
                                      -0.377767600503206, -0.202374154941363, -0.38940732469416,
                                      -0.336647798033452, -0.170574827917139, 0.289273875700483, 0.284874074563155,
                                      0.322023250651818, -0.0388609707227527, 0.334641392497855, 0.252090941696029,
                                      0.132361931523611, 0.268597767778866, 0.154096781158931, 0.134925152087512,
                                      0.129609421436963, 0.31027834445521, -0.531556247295935, 0.711545143107982,
                                      0.0576983497537448, 0.267851891467343, 0.246597241168048, -0.29944903074466,
                                      -0.783387091207501, -0.708779596578131, 0.451179485771623,
                                      0.0305219126036222, 0.277026538904915, 0.288360607779466, 0.3233014974119,
                                      0.222809226120457, 0.150044683175843, -0.124410113550103, 0.174717796497234,
                                      0.141030150105946, 0.26996954280495, 0.213729764059779, 0.153999054119148,
                                      0.256772299982766, 0.144342206577692, -0.0519268291717515, 0.373936259011421,
                                      0.23355118360029, 0.576271375126188, 0.656101567537349, 0.0285557902699171,
                                      0.126108961202619, 0.225403869234996, 0.189399131156323, 0.17095603744665,
                                      0.157392307634835, 0.161682498998493, 0.316532038737746, 0.222987791906295,
                                      0.207322634386498, 0.250559780547958, 0.225891485338305, 0.181995648163454,
                                      0.308915855130196, 0.237237224237407, 0.191215848464657, 0.294413465612905,
                                      0.302261498233641, 0.27093167284069, 0.35224362911838, 0.27321318333659,
                                      0.103455546604821, 0.307242576923224, 0.148269776075308, 0.18671353831759,
                                      0.0957622285996461, 0.293464327699747, 0.339077058913284, 0.344944820155122,
                                      0.246865067543663, 0.217810619967417, 0.353613530652505, 0.277879932091826,
                                      0.330954336001641, 0.215212311849682, 0.15760944173786, 0.191009303120631,
                                      0.338585701114851, -0.0739178883618892, 0.311318432141293, 0.129099802377694,
                                      0.195549375898208, 0.137372274475502, 0.214230381073812, 0.273528506264199,
                                      0.358723928378486, 0.0539312502822459, 0.152151263776556, 0.150680883144462,
                                      0.233249684446291, 0.154629384014949, 0.299759015237362, 0.184653650095657,
                                      0.188512054225004, 0.497873276513041, 0.139722435340085, 0.186395675945068,
                                      0.0290615592341671, 0.253630069062606, 0.304924362513425, 0.177949440292151,
                                      0.492902666873161, 0.263573224630151, 0.377289234192167, 0.227153768129461,
                                      0.271055840070684, 0.0679980291725194, 0.238814870987297,
                                      -0.0470645803151242, 0.289091878515062, 0.200617394153036, 0.247861568492525,
                                      0.212097928756952, 0.170733946463262, 0.108128830204093, 0.202651480392922,
                                      0.257965954844851, -0.15034306543388, 0.0296324450659213, 0.329173504457206,
                                      0.0831815515688317, 0.231561762294478, 0.44661068063175, 0.15725931277543,
                                      0.19791845542699, 0.301828319260297, 0.254668043327097, 0.377282672143102,
                                      0.113054434442778, 0.328759516184668, 0.305800406718264, 0.259170636216414,
                                      0.269372819411691, 0.221564668830725, 0.406814191378715, 0.163914255353563,
                                      0.353655763329895, 0.247187437641999, 0.337053542000326, 0.346649875223605,
                                      0.419526762237805, 0.32289437966378, 0.188453260184515, 0.436679063174458,
                                      0.440440484018043, 0.429452015299558, 0.263370258003864, 0.276158079959733,
                                      0.25558893387671, 0.450790849367575, 0.0130428741543741, 0.337276548230343,
                                      0.695967195446331, 0.381507703559022, 0.152254480454616, 0.39188670158721,
                                      0.341323384339741, 0.394834597028977, 0.20172425274009, 0.425393129033405,
                                      0.410082714998972, 0.40708758961234, 0.594717309542407, -0.678754106155723,
                                      0.370461917654395, 0.304604918580588, 0.292397043627633, 0.367314114866793,
                                      0.368707790747737, 0.310296758043798, 0.339064869504784, 0.274343053560483,
                                      0.344539286871237, 0.38237230710077, 0.752401068308638, 0.49997329281467,
                                      0.415216319094501, 0.369435051825089, 0.464060488834404, 0.360928300647353,
                                      0.28689495628358, 0.395715389453322, 0.365552591415319, 0.344122630862529,
                                      0.370011902654393, 0.281247645213418, 0.278856066767352, 0.221077026241349,
                                      0.298457677432157, 0.420151332321843, 0.288530936374848, 0.365953349541255,
                                      0.323848166812967, 0.329772477854523, 0.096134345834061, 0.369571124358011,
                                      0.243071104172665, 0.271984885233964, 0.24821017019531, 0.218182408043726,
                                      0.109762180129185, 0.207271404923618, 0.350743166582209, 0.262620346738104,
                                      0.297557180223522, 0.286123711164901, 0.280839017689815, 0.188668271840937,
                                      0.221659517409962, 0.149782345998966, 0.230625768268592, 0.299259398243007,
                                      0.207185390472252, -2.25096236960862, -4.62869499767484, -4.00115417296084,
                                      -1.02127433050424, 0.262343903402212, 0.201412082073741, 0.345231886997556,
                                      0.459562536818306, 0.157903561833515, -3.30686219871077, -4.35673541585775,
                                      -2.99360005510675, -0.430230380862979, -0.684047368330875,
                                      -0.673894050365056, -0.685780921712696, -0.795849638736798,
                                      -0.481025145518898, -0.573588661214433, -0.765852295033728,
                                      -0.652633166004235, 2.79218694800278, 3.57218684462227, 3.8335413447517,
                                      4.14630866214099, 4.40923656053472, 4.57188860074432, 4.6205270573716,
                                      4.57789616650239, 4.57815988452429, 4.34161157526256, 4.59926366783838,
                                      4.65542033218755, 4.72593227165249, 2.31679764645727, -0.503052541921226,
                                      -0.6065296492847, -0.227259396510197, -0.411310686327968, 0.38790741279968,
                                      -0.706003165178022, -0.507446479417835, -0.655643266792306,
                                      -0.625114088615468, -0.776437316772407, -0.571868242347532,
                                      -0.687281973100239, -0.726028745739141, -0.783733804444756, -0.8243318148853,
                                      -0.86888985225703, -0.726148588087402, -0.335099310489132, -0.70025722034026,
                                      -0.756270935995639, -0.70680900636058, -0.625871569195683, 3.70271557220978,
                                      4.40074579478543, 4.46071294576964, 4.85221977448574, 5.27531181764005,
                                      1.19671278369764, -0.0304584458978907, -0.346958261441469,
                                      -0.672383690096593, -0.738326542143003, -0.704621360683732,
                                      -0.736787172028426, -0.846970975700894, -0.700451096238715,
                                      -0.694331668831245, -0.0712923170866515, 5.22506703374781, 5.17615834453688,
                                      3.76299254956755, 3.4585268875478, 5.30417079483334, 5.23207827543992,
                                      3.79002859870023, 3.8894975601649, 2.05284851367528, 0.261464340764949,
                                      -0.645480146247819, -0.678670813580007, -0.561119820288207,
                                      -0.692345929411516, -0.705637080898007, -0.669100369341575,
                                      0.967191419249801, 1.0568562620953, 0.751722751097425, 0.503184761905939,
                                      -0.546656449367317, -0.576754745878325, -0.64061992039893,
                                      -0.562538172288085, -0.383230474812647, -0.301078494279324,
                                      -0.268798471066408, -0.210222986204481, -0.49841443346977,
                                      -0.698292338135333, -0.727578903825338, -0.656041370226725,
                                      -0.662860817228204, -0.683545401612846, -0.655185977188765,
                                      -0.660846559072171, -0.612054786445834, -0.73158162448901,
                                      -0.766372239130226, -0.713739069597571, -0.648714640815774,
                                      -0.786692415539506, -0.630809944731867, -0.681171699351392,
                                      -1.18760300685746, -0.570688839544957, -0.773977749502817,
                                      -0.847986932905459, -0.809396305732398, -0.854846490833757,
                                      -0.844193293270407, -0.671220920681041, -0.836393322566143,
                                      -0.896652738202642, -0.80761878133394, -0.658032928550956,
                                      -0.706152402528367, -0.840807140432741, -0.754274569060942,
                                      -0.852272759148819, -0.394738379221543, -0.71643030646535,
                                      -0.686001044771177, -0.776775805600954, -0.920331725432917,
                                      -0.663523972414397, 2.45217049527552, 3.68581366939516, -0.559022260967988,
                                      -0.79412701015847, -0.830708768576354, 0.0418515785147546,
                                      -0.843848975583883, -0.886926420685165, -0.875014092972472,
                                      -0.81014287986216, -0.629241443361705, -0.794946895917221,
                                      -0.839137227492143, -0.75610858888792, -0.64899260313176, 3.83068820391547,
                                      5.02493337955107, 4.70489439319446, 0.989008744701957, 0.0246734657674413,
                                      -0.551643532549807, -0.653668330102011, -0.902057612668152,
                                      -0.58959185519061, -0.702113044706333, -0.518602020801427, -0.82395815369339,
                                      -0.584576752697848, -0.726651516018482, -0.725147028894556,
                                      -0.381227060531039, 5.47266835939471, 4.77689116038463, 4.69496794251745,
                                      4.58528213492334, 4.35884161980143, 4.66471931296282, 4.66957760121428,
                                      4.64400005767023, 3.20602883179695, -0.580682476423094, 0.0623650142404737,
                                      -0.751940343287803, -0.424107070150505, -0.792644831580743,
                                      -0.790162972943833, -0.681968060579424, -0.621094469129634,
                                      -0.806370131924447, -0.853502077647411, -0.753558318039755,
                                      -0.862540084717704, -0.791898116049133, -0.768355611286778,
                                      -0.817975389726508, -0.813953774946695, -0.834346402567589,
                                      -0.794847630889103, -0.856563230564498, -0.808481605896035,
                                      -0.805666934486935, -0.805860799269363, -0.820965604950837,
                                      -0.815790360608095, -0.824060373168827, -0.815208453445204,
                                      -0.819694061665195, -0.817378453718589, -0.830430626793502,
                                      -0.82485433093975, -0.816085436927078, -0.857517383733919,
                                      -0.838773410655004, -0.758921236708018, -0.545434590919563,
                                      -0.784273245741131, -0.829332778907106, -0.771788580961625,
                                      -0.788150847261231, -0.860807064155306, -0.867070112326178,
                                      -0.767255704847976, -0.848657053599057, -0.76152385615077,
                                      -0.790250901196136, 2.08304917715747, 2.83487240463894, 0.342735994177456,
                                      0.455247156690283, -0.553581643122605, -0.817907778805262,
                                      -0.869353372780826, -0.687895945108459, -0.817699651590589,
                                      -0.845496497949104, 0.349889064101934, 2.71799581472401, 0.339377983369751,
                                      0.449139607431959, -0.128097775732934, -0.755616780838549,
                                      -0.784222659213745, -0.823939824297538, -0.815612940800385,
                                      -0.837209893588785, -0.844071427651769, -0.782419277308204,
                                      -0.758119285650896, 1.20063057326842, 2.73833575143224, 3.40404961014505,
                                      0.291033275409492, -0.667000019520785, -0.840234481648521,
                                      -0.890763583505348, -0.868538520051184, 0.00160065339866347,
                                      2.90809250324013, 3.78878440875685, -0.442058236222606, -0.516638455908546,
                                      -0.688728743339002, -0.762625894777111, -0.913838937352414,
                                      -0.783648323400871, -0.806429433836961, -0.86836528641876,
                                      -0.811905484434986, -0.833302049229339, 2.98722138162134, 2.57285698812642,
                                      1.95929411132404, -0.419875466861069, -0.835307167708961, -0.793008072010048,
                                      -0.763807905093395, -0.816562262017975, -0.857372378129973,
                                      -0.854993190092349, -0.87419778195194, -0.8394126932185, -0.829614368511907,
                                      -0.886014063603141, -0.864161560821681, -0.7943803690439, -0.813301963561627,
                                      -0.881738699392025, -0.94316023891605, -0.821590899737424, -0.81663014049073,
                                      -0.839135538311898, -0.897263601987782, -0.832854300335459,
                                      -0.846181065080486, -0.8495061380224, -0.837708809222247, -0.72880350624301,
                                      -0.563081317358893, -0.322859871138952, 2.81090386677605, 3.25566003867242,
                                      3.44054324513007, 0.539019235757027, -0.711832604924477, -0.740245586337782,
                                      2.98039753889427, 3.50552084331012, 3.78310117317763, 0.570491961038611,
                                      -0.29318914812865, -0.77351495161453, -0.679707610922074, 2.22682244024822,
                                      2.03437019581322, 2.05365294909274, 0.649319521192783, -0.643584674739072,
                                      -0.817066784197632, -0.747608168466512, -0.164021668056963, 2.55146432736986,
                                      2.88222339901364, 0.165676839384965, 0.0270631416400237, -0.800657561748634,
                                      -0.806279380192502, 0.917350193539496, 2.73750654368009, 2.45171600261823,
                                      0.211680466509186, -0.701399794726109, -0.88384011872269, -0.863790816513039,
                                      -0.864325351242489, -0.856282412495446, -0.897189374023572,
                                      -0.858528813986279, -0.828708763585532, -0.825698255961398,
                                      -0.797573885905762, -0.829236483225152, -0.778209474668811,
                                      -0.859236303184788, -0.828200181616457, -0.81787120000808,
                                      -0.810597488885037, -0.880377276698383, -0.826571128596977,
                                      -0.784000415831102, -0.803743789836148, -0.888070508840024,
                                      -0.797088639912869, -0.778687164539435, -0.976551268331894,
                                      -0.761977612020015, -0.810203281855317, -0.749675754210419,
                                      -0.813531328373303, -0.78219007097918, -0.782874576958405,
                                      -0.770439519056331, -0.775757557592011, -0.781610281658337,
                                      -0.786972193019662, -0.79395809689796, -0.762653692036969,
                                      -0.942040035347784, -0.882934617064659, -0.884923652222756,
                                      -0.703315622036762, -0.735693564369358, -0.855986295925819,
                                      -0.747484981104899, -0.833777970542054, -0.814127811973829,
                                      -0.896576254735715, -0.866793884902982, -0.815790044851196,
                                      -0.86060804094053, -0.828017495038264, -0.907046119236594, -0.85809284841784,
                                      -0.757321064068869, -0.893974631338629, -0.755357788013987,
                                      -0.846582518476628, -0.820909910081507, -0.876425621878466,
                                      -0.785541063051186, -0.790883220170287, -0.830090652676406,
                                      -0.833795882350286, -0.830849084439304, -0.866589511562191,
                                      -0.874250617690243, -0.83661686482372, -0.905281741829271,
                                      -0.880700053617939, -0.85182067437609, -0.811060787090038,
                                      -0.812727490905822, -0.816088470297784, -0.893530510663837,
                                      -0.842107253832981, -0.838860846557576, -0.806402615613953,
                                      -0.879070583237883, -0.0197638563606905, -0.202234954326732,
                                      -0.0992041828136723, -0.379997219431201, -0.445060181631689,
                                      -0.841782598493511, -0.430031355729756, -0.808743969748892,
                                      -1.00892270441415, -0.226918913306233, -1.12389323452177, -0.366999959960016,
                                      -0.847736007427629, -0.748882789543255, -0.761300121141892,
                                      -0.940670728350843, -0.308528876602602, -1.01040399091384,
                                      -0.564204958546161, -0.85378044533715, -0.707915335563198,
                                      -0.830037928624586, -0.736874789005435, -0.594533120358377,
                                      -0.963119199785795, -0.400195112285989, -1.17872121870183,
                                      -0.494457430438909, -0.820082430933374, -0.898526080019373,
                                      -0.762997784418571, -0.704131725519594, -0.975577710438335,
                                      -0.664483426762683, -1.13533486089379, -0.481319981247417, -1.13748616033503,
                                      -0.421948748623188, -1.06930160745353, -0.639569237919024, -1.06417155777282,
                                      -0.57201786910759, -1.09204959098652, 0.145696590185401, 1.7602417235283,
                                      -0.546419910910731, -0.597735888195101, -0.474337791217907,
                                      -0.688182550170832, -0.480545038635783, -0.476612157569241,
                                      -0.737043400551832, -0.503533701567038, -0.422626435419227,
                                      -0.777638071748636, -0.609690882088296, -0.374680606449645,
                                      -0.575735640781157, -0.499478276958018, -0.893538893821482,
                                      -0.487039073886217, -0.284176578287233, -0.359102658560576,
                                      -0.612246579670333, -0.813357807365895, -0.621059222410755,
                                      -0.466712152263056, -0.628740885709152, -0.55968872722555,
                                      -0.487801024844975, -0.813556507242915, -0.57047898968563,
                                      -0.810186264188386, -0.516888202850584, -0.00744959537247898,
                                      1.20269241122865, -0.0403519280722421, 0.617605570165963, 0.598396197101155,
                                      0.0292794486529645, 0.335585376269778, 0.747049335724644, 0.0190708790797602,
                                      1.08964988904622, 0.574001220220222, -0.689288440088746, -0.854638597423423,
                                      -0.855803698671865, -0.854184607660519, -0.856090829126179,
                                      -0.863926252183971, -0.848738240170251, -0.764637470451557,
                                      -0.247659071810944, 1.193268812469, 2.79234840448651, 2.08626759877089,
                                      2.83567731381083, 0.156090296954978, -0.780137758625938, -0.820526849918659,
                                      -0.726679185908659, -0.0255739056935656, 2.01241267408553, 3.35580190477866,
                                      2.62788392539305, 0.126416734573957, 1.10531528577923, 1.08283331406196,
                                      1.09666412809286, 0.610553670006246, -1.23434551797178, 0.342110926059115,
                                      0.846649127689113, 1.1629223881543, 2.31703288862001, 4.36370426970111,
                                      4.41127922682894, 4.42760248437571, 4.26890207717635, 4.31044590046761,
                                      4.28549638139694, 4.26172286697168, 4.30337876638686, 4.37507658234447,
                                      4.34734776997771, 4.28032510948283, 4.2151593416365, 3.68294869190956,
                                      0.548958998490746, 0.756462664604421, 1.43830824169324, -0.0709384009405906,
                                      -2.19397483202942, 0.173228998126724, 0.326339784959086, -0.141938644098066,
                                      0.173862790644726, 0.376232381293091, 0.452829854746757, 0.181143987129792,
                                      -0.660644629260848, -0.195368139642916, 0.347260083064308, 0.17695957849894,
                                      -0.428059595294076, -1.81124042174416, -0.297308386737174, 0.162933097864247,
                                      0.0538365351287326, 0.00791256285448649, 2.97339165102423, 4.29357274051439,
                                      4.31976769140668, 4.23525879908637, 4.0538659826816, 2.38749313807258,
                                      -2.07190329677066, 0.716706276088267, 0.979835967065676, 0.484157833781279,
                                      0.0353769121001492, -0.894174227930605, -0.485749339068173,
                                      0.504124324915069, 0.935883630603463, 0.647895751678395, 3.05494186133706,
                                      4.15090045945363, 3.10647760932943, 1.56578880049227, 4.01593264153567,
                                      4.09952025150289, 4.282459319438, 4.36840335817525, 2.92589841095014,
                                      -1.94470948246073, 0.507629699977869, 0.725140376562151, 1.13907125261807,
                                      0.833062319464184, 0.78607612475499, 0.811957488424102, -3.22909374954244,
                                      -3.65677738916262, -3.08413001529066, -2.44643149151247, -0.670471974860272,
                                      0.17240215173709, 0.819313299918382, 0.0187119576567222, -0.579696135261664,
                                      -0.748393317269044, -0.951359831732521, -0.963368703260451,
                                      -0.886036233076049, 0.0905991362996073, -0.455427978230595,
                                      -0.371151887098863, -0.331023832486697, 0.846487752648402, 0.918540307277081,
                                      0.924599266444175, 0.830916204905976, 1.00139038369166, 1.31080879354181,
                                      1.19860199400753, 1.31212783953434, 1.12553410344985, 1.32653659600127,
                                      1.15799282850233, 0.59105963752921, 1.4543494751387, 0.878864864652075,
                                      0.397133602037895, 0.548335594915103, 0.584772887848074, 0.534277392039454,
                                      0.7892307272495, 0.439769429853479, 0.801677084012841, 0.603791182266357,
                                      0.101371147038248, -0.604127107404056, 0.236264218683952, 0.308790498084163,
                                      -0.643330746729306, 1.3307128306466, 1.28766416034333, 1.21142592994564,
                                      0.660206775673901, -0.45998986029055, 1.14934876543828, 2.10051420861146,
                                      4.35529927494423, 0.452994109540984, 0.596130175664205, 0.698135420384898,
                                      -1.94313738392397, -0.0309397755365326, 0.0418645079048954,
                                      0.0290406252293488, -0.0902635117147715, 1.18928373591418, 0.646155639538085,
                                      0.553573249539743, 0.859963142539879, 1.20560264108433, 2.03154450257716,
                                      4.19042245124453, 4.2104634862097, 2.45331692775933, -2.03971621836922,
                                      -1.20779942856488, 0.955215339311288, 0.587900675658901, -0.662520798467752,
                                      -0.684325773426147, 1.14408917015681, 0.734460182093081, -1.49278126052802,
                                      -0.109473964417549, 1.02506237138207, 0.926828024198743, 3.09801409717048,
                                      4.22982354561976, 4.26017259778631, 4.24891128218473, 4.32101353255586,
                                      4.22145388765643, 4.20990179050826, 4.22917030648607, 4.07632138343452,
                                      0.874348926924147, -2.53743593544122, 0.594987179052606, 1.73202441170604,
                                      1.08198873832894, 0.457861980235934, 0.0470673123239682, -0.194633946221711,
                                      0.851845733628099, 0.771154974006003, 1.00537964724199, 0.617701062314939,
                                      0.851048387989758, 0.804600757534987, 0.782255797603105, 0.677048759246666,
                                      0.682271699856899, 0.641193251978678, 0.0215334195769696, -0.316757834780168,
                                      -0.346047116242021, -0.346403413459305, -0.342689700389177,
                                      -0.317910739535313, -0.246647000735865, -0.254862949410075,
                                      -0.233075654404163, -0.261355703000754, -0.145098933677281,
                                      0.180173287659458, 0.323811271688512, 0.366572256234637, 0.339629298618835,
                                      0.405586350679586, -1.12226246578989, 0.11871854584168, 0.287934490106504,
                                      0.915458381126668, 0.704184988114419, 0.38592958494351, -0.0742217939873703,
                                      0.294322973915867, 0.271762700817843, -0.274298159229093,
                                      -0.0532271146888442, 1.4155488354768, 2.47318251920696, -2.06624753502827,
                                      -2.57675802237987, -0.285892925931376, 0.392605379792445, -0.228117909734022,
                                      0.616359840589748, -0.632935495182625, -0.338928905307314,
                                      -0.255382845419272, 0.948467781349271, -2.30214274986169, -2.73687086730357,
                                      -1.20942715998399, -0.899110523693211, -0.0966091743595747,
                                      0.281135564060616, -0.649004346227861, -0.0643858800750117,
                                      -0.204361026189986, -0.245614911854638, -0.67035791552933, 0.152975241632738,
                                      2.02182997839298, -3.58632911972538, -2.32633076450037, -0.845835660352729,
                                      -0.292890191236944, -0.317194726994625, -0.415401395999554,
                                      -0.665191223310591, 1.56491496532645, -3.72772826940304, -1.57306472895197,
                                      -0.298862274678218, -0.394768299021459, -0.431339769465373,
                                      -0.42547068853771, -0.82698767381965, -0.620871625557378,
                                      -0.0870168576891843, 0.30141891018378, -0.366445421817346, 0.907469809877452,
                                      -2.30050905987281, -3.28275707716915, -0.711840544672519, -0.706736216903081,
                                      -0.80958826945799, -0.738003511542642, -0.313095733769556,
                                      -0.420259812532567, -0.178581333667606, -0.689331319076652,
                                      -0.662279837210246, 0.207263270353935, -0.333983278711203,
                                      -0.519207079541127, -0.891841858487893, -0.807518660991918,
                                      -0.419581119303295, -0.346710435684479, -0.401209755067848,
                                      -0.544382995253715, -0.509661126103031, 0.0463391505674817,
                                      -0.339887633611178, -0.259883000092688, -0.517511703710131,
                                      -0.338120266779259, -0.986978820741407, -1.4891317228443, -1.37719703801236,
                                      -4.47889812089055, -4.35426944791817, -4.13423516785008, -2.88793480832455,
                                      -0.64075448626542, -0.496434067673363, -3.2036265644942, -4.33930524205826,
                                      -4.28999973721278, -3.12772042289262, -1.13009426206817, -0.653953341221125,
                                      -0.794811250800904, -2.99083777441159, -4.31021183219165, -3.81596659818663,
                                      -3.10468484522866, -0.682268146854356, -0.809718310882127,
                                      -0.459775435130416, -0.669595568423266, -4.07642859402847, -4.42574937945935,
                                      -2.55981105749957, -1.66581782929292, -0.418192163034612, -0.404393757909235,
                                      -2.27287270287381, -4.1085196858868, -3.6884255810881, -2.52004567057754,
                                      -0.172754439641171, 0.0789354235368768, 0.0715903594837961,
                                      -0.0201252644334909, 0.0529639732785834, -0.227884092094629,
                                      0.104030108966886, -0.326626991854874, -0.384993762884572,
                                      -0.357716902605253, -0.645268693845123, -0.284483886608026,
                                      -0.316871517666363, -0.335090136233702, -0.534043298700197,
                                      -0.244997075384125, -0.373934650842063, -0.446215708105143,
                                      -0.284663171800333, -0.0660256578783773, -0.102709440698794,
                                      -0.115496040135366, -0.263159844476303, -0.214624915937014,
                                      -0.546110536904952, -0.28921662709895, -0.268272930306046,
                                      -0.642278484572613, -0.575991094795139, -0.868481323873884,
                                      -0.487892997653578, -0.603463283226903, -0.478330346554328,
                                      -0.630451851251552, -0.436569608311532, -0.369048760476548,
                                      -0.304736286895092, -0.368169239338088, -0.510631053531873,
                                      -0.58732127037423, -0.621014949241769, -0.275495173052033,
                                      -0.389859946232375, -0.193673745045057, -0.384967418490033,
                                      -0.203999863126659, -0.310068818366351, -0.160794688469111,
                                      -0.406047194653104, -0.303498656298616, -0.221486586242078,
                                      -0.0232341576578489, -0.25045699637888, -0.171850079909452,
                                      -0.254629322743897, -0.0764352103127591, 0.101314099899135,
                                      -0.152874189366297, -0.0759467422917676, -0.414288364743647,
                                      0.16949149770323, -0.383411852056512, -0.198212345516331, 0.0309585573403262,
                                      0.0412437158900433, 0.241029634608469, -0.245256245958963,
                                      0.0702229762214825, -0.0535285051092191, 0.185494421208443,
                                      -0.109940242732604, -0.100065402022843, -0.0236527793426563,
                                      0.0665463349315782, -0.51046626336409, -0.548192224317036, -0.63642568761099,
                                      -0.161498948632849, 0.0763367454861491, 0.180200831479085,
                                      -0.208065340085203, -0.015614185848734, 0.147275688131244,
                                      -0.077508762410593, -0.076747507185238, 0.262507563015008, 0.100586144966101,
                                      0.234812027996549, 0.0932170141553873, -0.063973033964808,
                                      0.0145054739287455, 0.0707110117220981, -0.0542547183926635,
                                      0.255197372920314, 0.146573997634413, -0.0800691022135157, 0.180886481616593,
                                      0.0175397671349322, -0.119537183584815, 0.247283598293496,
                                      -0.175963410266944, -0.270749647358673, -0.24583694868431, -0.01049289344827,
                                      -0.267423912685593, 0.0337619254023945, 0.195749715944916,
                                      0.0329608304364949, 0.138303988549333, 0.0396204992224197,
                                      -0.146243618454872, 0.0116403807597607, -0.115771074854678,
                                      -0.137722352092099, 0.0980665417151601, -0.0785793900632966,
                                      -0.363800402220653, 0.18043643377535, -0.0232973920457242,
                                      -0.165508555443437, -2.17990020152615, -4.0610753523593, -1.52984118786741,
                                      -1.65143240990446, -0.459257162808773, -1.35724250701024, -1.35721662709394,
                                      -1.341373652452, -1.50749286301959, -0.164447318485777, -1.67477725917557,
                                      -1.409736834275, -0.298717408257745, -1.45299424491336, -1.28325868674835,
                                      -0.238890501100946, -1.63128927590621, -0.34093494854982, -2.10332423465476,
                                      -1.72099008966414, -0.364940180474872, -1.16984401740384, -0.88906468213543,
                                      -1.38276027384171, -1.7514863361267, -0.179127348482522, -1.28214639883623,
                                      -1.47359344418036, 0.310494492849329, -0.168019466143031, -0.397105166752456,
                                      -0.524210407752515, -0.365046997989088, -0.425768611760723,
                                      -0.184249561259581, -0.39695795986177, -0.335710972835788,
                                      -0.325756566199538, -0.345900914075534, -0.403447615189441,
                                      -0.375242322916666, -0.337348376410264, -0.363043069992332,
                                      0.0122359905432781, 0.0446204199204995, -0.0917559307391826,
                                      -0.123044767426357, -0.122440856986448, -0.121794802553776,
                                      -0.659909552161687, -1.44049643776898, -1.83407067776631, 1.20746176476948,
                                      -0.290907168478088, -3.64747807849534, -2.08578861824739, -0.185583380207892,
                                      -0.344182167729517, -0.290864961622354, -1.76934524890099,
                                      -0.0168295859579196, 2.13737258988972, -3.16066424509887, -2.61539749881365,
                                      -0.00469619167550034, 0.00128766781421147, 0.00667321673205436,
                                      -0.0169178517834317, 0.0824291864730571, 0.00356919163844134,
                                      1.25373932259363, 0.108840595227944, 1.095387322952, -0.00554406970374951,
                                      0.0228477571917755, -0.00211415367016147, -0.0385847789219072,
                                      0.0208101831427342, 0.011186058658244, -0.00378518371788906,
                                      -0.0321683085351386, 0.103515884111305, -0.00654244811519382,
                                      0.0634703802757818, 0.0193645157742372, -0.0692274378696482,
                                      0.129730160784144, -0.0105083625347693, 0.106542498327886,
                                      -0.207920116528944, 0.0992479148096694, 0.0426548512807565,
                                      -0.0369065132080719, 0.103740171894233, 0.09841566657606,
                                      0.00316172021916772, 0.0191788004527228, -0.117563541154747,
                                      -0.0946609840054823, 0.0736810383128669, 0.00403196696012434,
                                      -0.0512494908054309, 0.00238835682289467, 0.0184076739948212,
                                      -0.0528482739043817, 0.00477347696470689, 0.0100596195731145,
                                      -0.0539835914681962, -1.0567251152556, 0.0330538430633881,
                                      0.00425043179279703, -0.0115797274079824, -0.0888511702675663,
                                      0.0929316420635848, -0.0538445883706043, 0.00915494228533135,
                                      0.127655698245489, 0.0811124036373832, 0.12967970189335, 0.0264510488213259,
                                      -0.0510625085437403, -0.114965902949705, -0.0267463941513786,
                                      0.17076263053999, -0.518492569725311, -0.00372078437905288,
                                      0.0590111405709831, 0.0249158019226791, -0.0262764815677881,
                                      0.0526892069613471, 0.0456877053125942, -0.0718662376746427,
                                      0.118900416960442, -0.216495722145969, -0.108328553651276,
                                      -0.0575153281430237, -0.0412626533400821, -0.037788559300815,
                                      0.0438145582067462, 0.0339470667745848, -0.0523962163582984,
                                      -0.0797155086580601, -0.0150978302748441, -0.0477665071819453,
                                      -0.143751817736015, -0.0739387294012608, -0.122430053197246,
                                      0.146989624559916, -0.12736342939303, 0.00429213323717473,
                                      -0.0541133840908971, -0.0285195825070689, -0.0266954391845462,
                                      0.0298982413355093, -0.0440522203966323, -0.00159969829337299,
                                      -0.0212660419293727, -0.0513916280893037, -0.0139140967575076,
                                      -0.0277241918882641, -0.107371108809652, 0.011056611964826,
                                      0.104870521293001, -0.0174232458999902, 0.122547028047546,
                                      0.0452004231117656, -0.0858059091644123, 0.0228100065099248,
                                      -0.212836350538205, 0.252379260008527, -0.0127222580141895,
                                      -0.202754689608303, -0.23011295481321, 0.018023907604558,
                                      -0.0386991342878016, -0.0262763227179423, -0.124405945574647,
                                      -0.0328660878575037, -0.0127030658908117, 0.184834083135296,
                                      -0.0498964756057715, -0.00768029456404153, 0.0702622403125182,
                                      -0.268664417472098, -0.147021007154315, -0.0798992311583875,
                                      0.0225312379029619, -0.0832019865987717, 0.0246113158984337,
                                      -0.0318544785027488, -0.127910311985356, -0.0129398879499818,
                                      0.116799119221259, 0.027820348556048, 0.240649271993094, -0.0830710207837634,
                                      -0.233625485196776, -0.0306556715671132, -0.0329740354302142,
                                      -0.0931835196471474, 0.0517684259855104, -0.0287362304029891,
                                      -0.0560656321369033, 0.00506993715333578, 0.0489570118099754,
                                      0.454823987589342, -0.0299088443024267, 0.155215585540879,
                                      -0.171278489637532, -0.457440864275054, -0.325247191759708,
                                      0.130939799252196, -0.0632587179319898, -0.0863907939702598,
                                      0.0839571545150702, -0.0881719804926449, 0.0719543846299146,
                                      -0.0799455978829231, -0.0893366743924769, 0.00332588489108562,
                                      0.058806280193812, 0.190605262591331, 0.0181596507705384,
                                      0.00783696788641144, 0.0311427258577956, -0.0130526409864431,
                                      0.031468049937733, 0.00039500085360797, 0.00180264579111314,
                                      0.0330669222226802, -0.086751246712869, -0.222832280430409,
                                      0.204149640580154, 0.0161958528109219, -0.0829570870325289,
                                      -0.0874169772890056, 0.0884917708655656, -0.0845055938146632,
                                      -0.0489126057917313, 0.14313258679504, 0.00860382379134248,
                                      -0.00185094407119575, 0.062014375093341, 0.047296134932401,
                                      -0.00251976860030234, 0.0501555650080515, -0.0036924140962476,
                                      -0.0562327713957071, -0.0252585313331191, -0.0164086629662693,
                                      -0.00584691214861442, -0.0048516295637855, -0.00591695456808102,
                                      -0.0101237661226252, -0.0103492928045324, -0.00605378529580476,
                                      -0.00244536575064292, -0.000142130813676945, -0.0193930474619065,
                                      -0.0378682279327071, -0.0645833685020583, -0.0248919108870013,
                                      -0.0217913153470762, 0.000591343325181453, -0.17465477889265,
                                      -0.028527314765154, -0.00134170782779706, -0.0109499700459941,
                                      0.129595750817878, -0.0669904870733536, 0.0479687272086243,
                                      -0.0153918804620851, -0.0377831376918261, -0.030398609863139,
                                      -0.0364609697506193, -1.20219493748389, -0.130812987097783,
                                      -0.137959050760192, 0.477187837708605, -0.201129841537215,
                                      -0.0239790801420331, 0.0989514718556141, -0.0408487991917, -0.10412035480029,
                                      -0.180028351057573, -0.0615530169614495, 0.603429300160503,
                                      -0.154806397795589, -0.158989507183337, 0.0208163587954888,
                                      -0.13071951014003, -0.133105386798778, 0.209942985306489, 0.0402463377549726,
                                      0.230483675049058, 0.0749310307182438, -0.109350594113109,
                                      -0.103092505852513, 0.751882610039965, 0.035882507610752,
                                      -0.0764169889646939, 0.382577187148892, -0.0980748517326456,
                                      0.00812749456872363, -0.00832833738483118, 0.033910608203312,
                                      0.125742619444242, -0.782908108768316, -0.257614769766409,
                                      -0.243547170365555, 0.0599128659272183, 0.337373396813666, 0.104273866090501,
                                      0.0383335060905322, -0.0181618592780126, -0.152205341762013,
                                      -0.0102082709644355, -0.05382026761151, -0.0148020299142189,
                                      1.26913916166247, -0.0351580012279379, -0.420590957237835,
                                      -0.159442360783677, -0.0763609311146484, -0.112709350704796,
                                      -0.14922667866499, 0.0126505170498468, -0.164331508502695, 0.120557815034759,
                                      0.0118708440823913, -0.030628044451584, 0.0689194025814745,
                                      -0.0180760341338765, -0.00180059315086134, 0.00721376285405778,
                                      -0.00803255736689461, -0.0221356524117757, 0.0893413298980379,
                                      -0.0873041808326798, 0.00290454814025398, -0.0530956809332547,
                                      -0.113684823466575, -0.00753053629791671, -0.0264323085954517,
                                      -0.0222856558046146, 0.0101191893048853, -0.000420189886643748,
                                      0.0514008141868453, 0.0580415494889356, 1.37146934168012, 0.0132090752937076,
                                      -0.140359177788888, -0.0373909728842062, 0.00563812691844515,
                                      0.0253990626222358, 1.09896977908198, -0.0742485048120725, 0.118661524101085,
                                      0.320028285868529, -0.142266535472935, 0.0694511281270333, 0.04714152706736,
                                      -0.608195557556929, -0.231198795435021, -0.0772218341887531,
                                      -0.118033102296459, 0.0768909833586854, -0.140954846819494,
                                      0.0879576441450847, 0.115077230524674, -1.11706229586722,
                                      -0.0973776686431627, -0.0446846494461954, 0.0204464556157276,
                                      -0.032261786007391, 0.0386093315066807, -1.78257857232733,
                                      0.0145090015900771, -0.0167586093955219, 0.00630467017453508,
                                      0.176360380116317, -0.011531742240873, -0.0167629276518214,
                                      0.00328791492931068, 0.028206561584447, -0.00566584666148435,
                                      0.048713730477892, -0.0544823864155956, 0.044718344876314,
                                      0.0916914521490125, -0.117353380613579, 0.13489625159545, 0.0692262197244654,
                                      0.133445296578176, -0.0630276290540789, 0.314344651609984, 0.190968880903848,
                                      -0.0754473999913076, 0.00625556262356163, 0.261993953575276,
                                      0.0331054986823897, -0.0235238029703414, 0.0445490684537177,
                                      -0.220756594508066, -0.0914673839865867, 0.0742322690814849,
                                      0.0859106105561033, -0.0303653014221855, -0.0799094907743612,
                                      -0.100516077893999, 0.0720679225635189, 0.122134540463138,
                                      0.0702170278992577, -0.116687525751355, -0.127405702047622,
                                      -0.0145853989096198, 0.0885820680381864, -0.0202990102324865,
                                      -0.166049528392144, -0.322398987707349, -0.0515694261480802,
                                      0.16992190039466, 0.00431446491533489, 0.0764815492050107, 0.124920856375019,
                                      0.0820657038741285, -0.0693824432043527, -0.00497968553921931,
                                      0.10215122902316, 0.232096835273008, 0.107945341282806, 0.10810039798416,
                                      0.0872445601538561, -0.291325479315577, -0.158690874515776,
                                      -0.0699327010382767, -0.170308494858233, -0.11800677594548,
                                      0.012770786634431, 0.213523411839609, 0.204601865465144, 0.132903254065602,
                                      0.383800448138422, 0.329807326686785, -0.0290718556750219,
                                      -0.249080371746602, -0.136574169485094, -0.0553396230124434,
                                      -0.165243025658194, -0.122147150863038, -0.0160356140876256,
                                      0.207966143962349, 0.0508547805428962, 0.15653086849972, -0.019178979074708,
                                      0.184005211666113, -0.422218955375278, 2.53937206332176, -2.97397738227714,
                                      3.22720516722854, -3.08454203424722, 2.73250446339877, -2.79967132779576,
                                      2.77456975952871, -3.02660688530897, 1.41072480511466, 0.458115448806227,
                                      -2.30794703946467, 2.417975607923, -2.20763655681055, 2.15111749564373,
                                      -1.75657339220852, 0.472227712233192, 1.22669558150365, -2.16473507660733,
                                      2.65338844610085, -2.69289587889448, 2.53333302905102, -2.49778821625093,
                                      1.55744596610466, -1.70122481279584, 0.970311551355116, -0.144250536359325,
                                      -0.779102231224432, -0.726499312073361, 1.89283233265036, -2.4914556585234,
                                      2.05873124358175, -1.85089455345315, 0.781957077334541, -0.341884227599138,
                                      0.238096966684516, 0.0396644687951598, -1.00159866472838, -0.328450432074215,
                                      0.422977912823867, 1.11676267576357, -1.00782768840711, 1.19887939748773,
                                      -0.368816882770152, -0.0609422809758999, -0.252435260621808,
                                      0.0126043693561993, 0.213137181224628, 0.143026509829714, 0.0456304571242834,
                                      -0.0201123636227353, -0.110972879596961, -0.0237525544233331,
                                      -0.0119030851661001, -0.0364510393116627, -0.0431392487940498,
                                      0.447282543696735, -0.249406112753694, -0.0978246601425276,
                                      -0.00529558850164914, -0.0944663884620256, 0.0682134816366443,
                                      0.101588252530814, -0.0485216361151849, 0.0177561012416449, 0.11483614454739,
                                      -0.0289572424079517, -0.107541440238072, -0.0935418544135485,
                                      0.0407536344930309, -0.022568226461934, 0.0866126968261057,
                                      -0.128111706287835, -0.0327350602820539, 0.978132223085322, 0.57990994589238,
                                      -0.366323907726083, 0.438300810127382, 0.632221094847577, -0.683019514171684,
                                      0.681156396056792, 0.602967449174667, -0.610481224028017, 0.524963145629771,
                                      0.361913208882883, -0.642640035913841, -0.625556739975952,
                                      -0.00612030703501407, -0.000301782063270363, 0.00568178174114064,
                                      -0.0189933893379184, -0.000644536477393686, -0.00820877561829858,
                                      -0.0627353681706234, 0.24810929608811, 0.311937510020735,
                                      -0.0643976018400055, 0.0373587202668931, 0.210492737045105,
                                      -0.0574290328969537, 0.0631025442014335, 0.00658799521584409,
                                      0.043019247887602, 0.829917336881641, 0.666879047697812,
                                      -0.00552378111584726, -0.0106552627660915, -0.522052337219359,
                                      3.23189105852409, -1.27489593319087, 4.42460583640915, -81.4183364176233,
                                      -140.995511970318, 162.400395662834, -36.5372076286684, -14.0224673617325,
                                      225.389924835833, -23.4919025177148, 1.62493269935446, 4.29735059002976,
                                      12.954774322154, 3.28438732176531, 0.467455562889558, 2.24704198577222,
                                      7.76284000904164, -7.74290616860365, 13.162069714239, -2.13961942799061,
                                      -5.81808375299933, -127.406159345028, -110.524684621829, 37.6588667570294,
                                      -33.9405792824443, -84.8472575218535, -338.919422785753, -4.73013391998053,
                                      36.0077241333033, -78.1828732563467, 28.0193076965498, -20.3928172229222,
                                      18.5319460414168, 79.3690594036893, 1.62188455862513, 5.61265790604411,
                                      30.804835731019, -6.73431906600841, 44.0837243614103, -61.2265620912541,
                                      89.5667874935512, -4.09580981059608, -37.5501293442054, 68.5456884953401,
                                      195.940693974708, 0.936469609372614, 9.05780880801878, 30.9311187443575,
                                      54.7397471963965, -266.205384255357, -277.43493131536, 37.3318593989507,
                                      -23.8566061037217, -44.8232345578476, 72.8755068843206, -114.89397831031,
                                      5.00402318103373, 74.1300456763222, 8.46275885090681, -40.616233032369,
                                      98.5965990577327, 20.2526985739352, -39.4024109518363, -20.6339737097692,
                                      2.71333923307669, 31.2009994234821, -40.3592084932033, 5.94253929312143,
                                      -109.978552067866, -287.472291338993, -25.3621478752211, 48.331795043463,
                                      -12.9328692927559, -36.012449298072, -9.64582418577973, 1.93832059674877,
                                      -346.017423971242, -13.2497472628159, 17.6701185224497, -15.7320680318353,
                                      170.10333631616, -20.8144176461631, 13.5502527599897, -29.0428170577565,
                                      -35.5495075697925, -0.630237034123224, -9.61986044229172, -14.86257513198,
                                      89.4126834550943, -1.06170079194672, -17.1110713875666, -53.4286038104167,
                                      -14.690927842011, 51.0004821690184, 46.3188101376012, 5.2839691296634,
                                      20.5426305827754, 31.5317991990839, 8.45950721752734, -1.03810018146001,
                                      31.2898968049167, -35.8834476443283, 4.62142079745043, -4.66692015654759,
                                      -0.221250798892938, 7.92632939599508, -1.24089932558181, -22.7576162814627,
                                      5.77356582506251, 4.87871422772348, -9.31276976045674, 2.93336869898787,
                                      -0.0691589629766029, 13.1024034200792, -6.48823782001049, 0.977128610054288,
                                      -10.3329849293153, 21.4046370310963, 46.368134590876, 65.7381687351833,
                                      191.483961690032, -10.9121882105692, -3.86507082934169, 16.5648417209656,
                                      -7.03488435678953, 37.9474745633054, 196.719842714243, 38.1245089440558,
                                      -215.071145753208, -29.1022427344707, 9.84385469514801, -288.565281747736,
                                      56.1252139219169, -5.90652921882187, -1.55197225509455, -36.3491210414279,
                                      -34.1934331402072, 30.4151177050918, 55.0564955613713, 4.19536760688827,
                                      -4.10014195698595, 268.52489717471, 9.20698321815538, -5.04267510265211,
                                      -211.21525492272, -215.039533544797, 31.8479381781353, 45.985196172707,
                                      13.6847986680706, -29.8760116820191, 89.0580401441611, 107.618635356324,
                                      -15.5362211652181, -106.707091425825, 120.781138000191, 14.0971016537696,
                                      -32.3915642348561, 218.531101878981, 4.88598202148973, 4.08485780834781,
                                      1.75936671392529, 24.1438090724155, 11.553903626657, 6.75942263803481,
                                      9.74156917908078, -7.42316124074235, -124.261564560162, -139.644723932195,
                                      88.6293111644804, 19.4648839435789, -43.4796235941689, -63.0283109777725,
                                      51.1455285908736, -8.08097144663779, -13.7282301266576, -2.62993630517152,
                                      2.5194427625382, -21.8971507461661, 25.2539240940665, -13.7986936587077,
                                      6.14524192770955, -11.6710813064936, 4.07395232061346, 20.1064933419772,
                                      -24.5400294952431, -5.66901474951484, 4.65833426374466, 3.38273853507543,
                                      0.843681193593234, 3.41191886017482, 2.07704326186108, -0.975706159832399,
                                      4.51000487031479, -0.963150011142523, 2.24714608535975, 4.0738156951895,
                                      22.2991380346717, 20.4747335902043, 6.16144887625183, -0.580777498962727,
                                      -107.544588153778, 70.9316171095814, -20.682392307765, 4.45030261457932,
                                      -20.2576690036507, 27.524778911202, -11.1046815370584, -19.1921048501685,
                                      -30.7770059930825, -24.6578116091557, 11.2883473027939, 11.6705801783824,
                                      -4.4425263010431, -76.1308168101238, -16.7974007025478, 0.0996225185356966,
                                      -2.57018675804006, -16.2055280421556, 113.047157047792, -27.9797806222317,
                                      0.859917837653168, -21.5360742610442, -60.1863330956346, -102.233150984745,
                                      126.738314831107, 88.2155820835323, -6.29239702840246, 14.7832266718512,
                                      35.6959928488117, -23.577922186209, 37.3458651517344, -23.1854463303513,
                                      -16.4636687626259, -2.17285846194569, -83.1222557175684, -6.18791125978981,
                                      -146.434807736419, 212.867016801783, 16.4094491082837, 30.8870809961928,
                                      -4.07946138831202, 25.4000972955722, -29.2869653698779, 29.9476377620994,
                                      -171.059842188173, 69.90812361634, 20.1286701633399, -21.2544659736274,
                                      -22.8191263552117, -2.85976061966905, 1.79898422911454, 8.20970744521753,
                                      -22.4463174141423, 10.3780417169733, 4.49469603916709, -12.7575704581116,
                                      -191.144056957115, 199.008721442449, 93.5875178647413, 4.80296417729993,
                                      -18.8554142037792, 22.3538753657542, 18.1927960188712, -38.7642536429046,
                                      39.1444704175755, -8.39514300131471, -12.6453677240342, 3.3417314161646,
                                      -3.813019346771, -9.50255362143916, -13.920419407842, -12.4330595731589,
                                      13.5594683181934, 9.96398730730196, 36.2079450454885, -8.28383359599753,
                                      -1.80799127711482, -2.43610401103641, -0.000125389837787689,
                                      7.65938492606675, 2.16479243089527, -10.5636722149732, -4.56875150636215,
                                      7.65244433122579, 33.3227043817935, -291.838587810861, 43.2923226133914,
                                      -92.0994777895193, -104.984129243767, 50.8163420905245, 30.1069186036788,
                                      -196.176868472156, -61.3719751354227, -44.1940370370882, 216.442268338665,
                                      147.682329193464, 25.9849048247281, 8.45522533776102, -173.989347030037,
                                      28.5957067336258, -73.4549641351014, -258.721687244243, 66.1883283627082,
                                      -1.96439207815425, 61.2376932763164, -4.61477416599296, -52.4704266919113,
                                      1.57857717159567, 371.669581548715, 201.632597390638, 42.1052747235143,
                                      -2.69710325533718, -125.976327729057, 39.7581406266974, -44.7800640937551,
                                      -313.650255344077, 10.1948791876124, -12.2611672572841, 6.06577702190002,
                                      -3.01396462161781, 8.84086432119348, 25.0576734767259, -22.7542916962165,
                                      -17.7213080054404, 49.037614418617, 0.926819013520245, -0.630959452926533,
                                      13.6735015523683, 16.6273056114963, 29.3736671772995, 17.6233848875908,
                                      -9.003405363461, -10.8754183377437, 17.3200282820784, -1.19872018357921,
                                      -4.23920174037575, 28.8045173617818, 24.6550623095591, 7.88550487015549,
                                      28.0239750390951, 6.96095333210528, 26.6622537154768, 29.9475091679199,
                                      -0.316489937381677, 20.5478227031346, -0.963705008591505, 4.05510846310271,
                                      -31.8842606302338, -28.776386919519, 2.77574405370768, 10.3480967164472,
                                      -15.6148563846653, 22.8505763059901, 2.13764131504196, 28.685558475887,
                                      16.5026217691329, 6.45057730677886, 36.4179598552131, 17.4913079050402,
                                      12.1460929217563, -16.819660107846, 27.6937858331339, -13.7560697829066,
                                      -22.2464779672117, -12.0629954927959, 43.8111956211686, 16.9941615658099,
                                      -15.9668346015851, -0.114303256994017, 26.0506073403905, 11.3081760959503,
                                      29.8808759703742, -28.3477817315971, -24.2929037284022, 18.5475266309961,
                                      -26.8253363874421, 21.2408311380117, 18.4029330224072, 56.4793603395363,
                                      16.9674277895014, 17.2626782575709, -8.62149175145507, -64.4656090970139,
                                      -2.55450370829792, 9.32032578735329, -0.333037279679927, -23.6104670418278,
                                      13.1434837941418, 9.91870804456317, -1.16549914734027, -1.22237840794491,
                                      21.0623574951447, 21.0045424459027, 38.6140864270308, 8.80985715258194,
                                      -30.8468194744304, 11.1997950497161, -19.0880935896739, -22.5931957606131,
                                      24.8629759343299, -37.0923633956781, 0.757349731649487, 39.792286807818,
                                      -10.9484839642116, 14.9411015225219, 51.4371077841318, -23.7964765242218,
                                      -0.735904971089957, -1.23513477514322, 19.8492861356758, 20.4224200635402,
                                      5.81349457727887, -11.2028531255845, -13.3639312540625, 25.9604263491222,
                                      24.1878878046421, -86.384658607161, 65.246914387188, 8.84230940698765,
                                      -8.78467148905807, 30.0021622784402, -17.7330210097687, -1.38105987922185,
                                      28.5594259796073, -9.66661353119688, -21.2761094787152, 30.7234862545798,
                                      3.33913661539761, 4.73091306178892, -6.2291322794375, -9.891154506131,
                                      23.6315218219127, -13.9842177434172, -36.1002458975946, 41.6504185536869,
                                      14.2233615066783, 1.81482979796654, -34.441442322258, 127.431663903942,
                                      -84.5727809612395, 51.2108178875443, 71.4320999904564, -38.8193985049936,
                                      -97.1788102353294, 72.3681921293734, 156.871146872108, -171.174770632735,
                                      -25.8150125487115, 65.8721810618012, -38.9208901276611, -53.1055666403237,
                                      10.5932967874409, 81.9242357490396, 164.277085042107, -146.63357746097,
                                      -62.1595343453776, -14.8043693350838, 45.8511827076132, 53.1526244407807,
                                      -61.6438747417725, 63.4501032667845, 155.300393101637, -89.8500921089984,
                                      -11.4556242667808, 89.6506620935946, -13.1615542305514, -36.8095368277748,
                                      -7.08018469917015, -2.8533744418579, -2.69405901103353, 0.401071566260542,
                                      12.7544791701166, -1.21780165947225, -26.7094359393032, 1.25385021585533,
                                      21.0681838066899, -7.64292768812981, -0.562522039221447, -10.7916866869802,
                                      1.78989541520707, 3.23095832526213, -4.38526925593246, 2.97578152079657,
                                      0.726749845953323, 7.87634422117883, 2.68663489623908, 122.999702833752,
                                      109.289125948896, 0.707357317193799, -8.45653208063437, 0.692247645627246,
                                      59.8028657546078, -6.39184784919656, 60.063034253137, -3.85384538444905,
                                      75.2168045549228, 214.780747474444, 46.2151194363692, -254.223801042052,
                                      79.4349376732287, -0.238238700481191, -0.497467088986941, -0.905386593961383,
                                      300.728629189029, -38.4009608883736, 41.0247553284282, -16.9332122123466,
                                      5.41887186997088, 9.35322878797979, 4.77026285359388, -2.09238098705474,
                                      -1.04616879567037, -7.88624162936739, 0.72144113060647, -1.76965989920131,
                                      -2.72206965611129, -3.1004362745841, -0.150826542288863, 0.170001088625446,
                                      -4.37289522354254, -0.406398894681548, -30.6900636046474, -8.77919785170355,
                                      31.0594300829366, 142.845640645167, -160.341944763232, 16.7307259092607,
                                      150.33770214875, -8.43706439008948, 190.000339316729, 102.045863134043,
                                      22.4687275798811, -28.6265545941978, -243.23838882139, 0.856646711609349,
                                      267.728676415282, -9.48425022833763, -17.6504081885776, -358.308173933935,
                                      -122.124094207687, 17.8456498708561, -4.36635819225726, 137.730101652373,
                                      -99.385369928247, 56.7547120253673, -0.601498105899773, -0.146319428296666,
                                      -1.43376423808516, 9.88223132700034, -48.8253368402122, -17.7216942373474,
                                      431.946532673453, -38.8415607252616, 240.023157974026, -324.7232597185,
                                      281.520920738386, 8.64980662095024, -93.7360826063476, 5.59943297620927,
                                      1.47965698803548, 31.6788040735859, 0.184010506586396, 127.798950096708,
                                      -90.8209217279747, -5.77487967949564, 3.41168873163395, -16.4093200171866,
                                      2.73625899781939, -74.0877292578325, -45.9838803670149, 306.601209648033,
                                      -293.842078511126, 9.72998256641737, 89.8502274205804, 40.0140022353861,
                                      9.64188679121095, 50.0601856460166, -23.2584357796006, 79.637028384092,
                                      15.6182242448291, -68.3891445825251, 62.1496707138549, 6.31672644195928,
                                      -59.7874141968784, 14.1028146569357, 2.26919497381437, -2.36857858758145,
                                      4.48037706651897, -280.673355906247, 6.45760523350155, 3.67558632258764,
                                      85.3228119606656, -5.66526543930134, 2.65399944814194, 23.3770955230259,
                                      -25.3958645844827, 28.2110611155442, 14.0739714492143, 37.1964762415085,
                                      -7.72726683979758, 43.2288574845937, -43.1263889153313, -32.3425969576622,
                                      -91.9503202580431, -22.5042842364961, -55.5722547751213, -6.9963743542143,
                                      78.5565245812174, 113.76922318078, 23.0947634924571, -23.1331930425033,
                                      -18.5219987023071, 4.55452042768706, -5.87801045212807, 3.52777391410523,
                                      17.8863492323145, -19.3154021385066, 6.43681695813568, 22.6059542815125,
                                      0.80469280322517, -164.642962438199, -11.8351042464729, -10.2518803967915,
                                      111.239496948411, -168.779024674785, -11.3519376162014, 25.3697602347032,
                                      -10.0956845325784, -22.5891870862711, 210.513802612274, -128.780944263734,
                                      44.5468889951451, 53.4514239398504, -45.0513907039843, -31.9406212960475,
                                      -158.449073165058, 1.58526298535185, 29.7656984606615, -230.020890877824,
                                      -19.8232172421785, 4.10880685505289, 175.604450815468, -0.417854071311405,
                                      36.0822706277371, -34.356810852885, -23.0418330887897, -61.5024007207375,
                                      308.80038714156, 47.097971996569, -228.222748761297, -293.090602900651,
                                      9.32664615599003, 42.3901289570959, -146.375364745087, -136.503614938823,
                                      -11.4781942524909, 7.73643794936575, -14.2433702645739, -1.1196321623227,
                                      -0.46439257559416, -2.06051696462772, -2.76004005054587, -0.187954785068961,
                                      12.4937752366885, 3.71259977189144, -20.8095535044665, -27.3596682208171,
                                      -4.74873948785408, 102.220912731829, 153.866359244419, 55.4256021854429,
                                      48.8584036529198, -146.328343322925, -2.87201081903373, 270.169418238415,
                                      -39.2990246435808, -6.23232021427172, -32.3156249235946, -8.18370126060404,
                                      9.40601080156384, 1.8528457048126, -76.8118085906558, -41.5619510964513,
                                      -110.617843081599, 118.821036558407, -13.8683123513292, 1.14201562478348,
                                      -4.21103424837728, 0.184015452918745, -2.846090053374, 0.847989701525499,
                                      -0.611103232663018, -3.90742957420355, 1.32719442816845, -4.52953777912435,
                                      -52.4358201306419, -44.8505611533495, 28.7154986073306, -37.9182304836443,
                                      -6.89038535610906, 63.7523810913452, -88.8120451226628, 62.2897093062044,
                                      30.8065269428556, 56.3166869610977, -90.1862268347221, 78.5791767477588,
                                      -22.4023045983377, 12.2527909183731, -6.43173435207773, -2.14147889034147,
                                      12.8684815758322, 53.9238496098882, 265.081737301008, -90.622005707296,
                                      -13.4552311925316, 0.26829553715638, -62.4053699762994, -9.79972152341834,
                                      11.2386536914442, -4.30247008068168, -3.45432146735994, -52.8270637158877,
                                      240.680920787242, -70.9939977542299, -30.5629394144636, 29.3046909954245,
                                      43.5647758958433, 64.4303376956093, -91.5208653257683, 20.1637906022334,
                                      4.75354306651476, -0.999157965495082, -23.8230285521744, -14.4483185606291,
                                      18.2224522638868, 155.808127359796, -29.5332901197916, -54.9236674713144,
                                      25.8818480584751, -22.6677026089779, 15.7502701440556, -7.90690787048899,
                                      -136.954481248047, 133.037356445824, -53.6030560487224, 284.814397925209,
                                      -102.983177130383, -128.973774282075, -107.943970271734, -39.9972285946614,
                                      13.0766623492247, -33.90449073094, 11.6743501381152, -2.34406842402446,
                                      -146.122188914762, 188.359715169831, 90.2540565108138, 65.561256168165,
                                      -9.38440941352717, -74.165332437726, 135.3829000725, -37.5909833414778,
                                      -101.288353803544, 226.419557622666, -15.7881265918424, 8.07664924716235,
                                      3.29664121943739, -1.11007731843114, -2.57348489970666, -0.235196490104719,
                                      12.9873314383296, 91.2525116322176, 13.4916523502554, 52.8586660325009,
                                      -14.0603873717082, -9.28865668449897, 43.7238731772303, -50.4278090303614,
                                      -48.3683380523395, 4.50548931439457, 33.4855228415826, -67.1419632979001,
                                      -7.04337628620934, 0.644686417274297, -95.7627735636485, 11.9499659161993,
                                      -6.87215426920175, -2.19986380231812, 4.54849166257992, -3.60571202757385,
                                      -26.5238601759826, -132.003642294939, -125.020727793305, -65.2282649285164,
                                      10.600625682563, 12.2544389713383, -2.30221162581276, 35.6843814191677,
                                      -14.1821985019628, 9.53939157329035, -33.9944381525011, 30.3449029571973,
                                      -11.7571958840486, 6.35739240780182, -11.0729743654875, -3.15137972518482,
                                      23.061231027667, 13.42270112607, 8.84103409051827, -0.744127494738971,
                                      -3.2230914245214, -32.9142430924682, 49.6922012733599, 22.1217508170266,
                                      -16.6299946005216, -14.9129962704984, -11.0958800526318, -4.54602113508164,
                                      2.10883349938659, -0.721323332954315, -113.988027720986, 83.8847463586603,
                                      79.8447565495469, -82.9674248761328, 22.117868733845, 28.6181638088919,
                                      -20.484360611682, -3.38669756814767, 39.2498957826772, 10.9009280754086,
                                      -13.3209578397594, -1.46530124732156, -2.54822999323097, 26.6527486318943,
                                      12.4451872012888, -19.235471647086, -6.09563499569263, 0.532519196437029,
                                      6.73761826682553, 14.4194416648432, 43.4415834640314, -37.0760641259552,
                                      7.92809123488913, 1.89689522311163, -27.9808947916106, -16.326114331183,
                                      16.0824681616786, 16.4685056469564, -14.7807224540177, -37.2942607831554,
                                      0.668230922215521, 2.6235791133986, -8.28145504953566, 1.37471306415086,
                                      18.2724061180909, 34.9561516434353, -1.87451395409242, -14.9967033112315,
                                      11.8622817299828, -3.66312991681001, -4.86539284505983, -52.7799861899857,
                                      13.5109524952655, 13.7083077280609, -54.018688188023, -13.9516222114064,
                                      50.4051516932085, -12.6966614213954, -25.4035023480258, 10.3580209172591,
                                      -79.3692085354172, 16.7123480867348, -13.8783594583009, 18.9648269862694,
                                      26.4580354426689, -1.33493771332458, 10.8113365387814, -19.4716187174838,
                                      35.5169161649034, 60.7780486260409, 49.6978491933257, 4.77260979202349,
                                      2.46811398781926, -76.5415969098212, 16.3513762361059, 68.5933417598356,
                                      -61.6495619786538, -12.128730928362, 54.1600396920138, -11.2036947894521,
                                      -3.05471258726525, -44.7014802136862, 29.9164252961382, 23.1521748117424,
                                      -8.02425108590313, 18.3224922504545, 28.2219488266425, 28.4668277207403,
                                      -13.953178238239, -8.05577099071831, 40.0298386014132, -20.9644707400692,
                                      3.6352288394754, -21.0921627788239, -25.6425107099693, 0.741867917837574,
                                      -42.7262630475335, -0.400637050476699, 6.69473584412129, -56.0959163704025,
                                      -5.54600232214005, 22.8161622868905, -39.1628800495487, 22.094709316516,
                                      2.49706299025065, 30.1546364128001, -17.3920119993971, -7.4399209255231,
                                      44.0197030565549, -9.84954881674113, 6.30864065955697, -44.4759548900572,
                                      29.9985968972277, 6.01164132543176, -51.8689175487201, 33.4589020943337,
                                      -42.7643287555113, -0.00799289875850778, -7.41451247645824,
                                      -14.0975639360521, 63.995437799006, -7.49477445575338, -52.4191071850099,
                                      60.199027942002, -22.7808159919052, 25.2451128152002, -38.8445415532063,
                                      51.4243864387513, -9.24058291064069, 7.5384114250419, 11.9447107473163,
                                      -1.33553906855303, -20.3877520027416, 3.00716883050667, 11.1730203256938,
                                      -13.6874065306781, -11.9214445297988, -14.9957792544279, 71.7162003540892,
                                      -6.82173219398267, -12.4564755325085, -0.0179878996263485, 21.0057705125703,
                                      -14.1903540874627, -2.07949112452163, -10.2503305853798, -1.108106591577,
                                      -2.00213179784531, -18.9891809781382, -1.5491374076821, 14.6241633170314,
                                      -7.04309154374759, -5.82757835710722, 7.28654164467401, -13.7239930889337,
                                      -54.4553382591819, -12.8898231841658, -1.37219029344093, 6.45649229107112,
                                      -7.14355077098143, 2.07562938220178, 29.1388646067125, -9.53829211646474,
                                      9.36976883600586, -19.1161205086375, 10.3940419605552, 14.8975272453675,
                                      -23.0049117009981, 0.769333259240517, -0.405483736370879, 2.1669910611672,
                                      -1.16760340115861, 3.40410969867501, -1.41339793751453, -4.99838229259447,
                                      -82.7528648288823, -112.785388424538, 5.99488931719537, 42.4672422503691,
                                      99.3598721270471, -33.1639674914507, 68.7360825196157, -28.9707856696361,
                                      -213.17333375609, -54.5531070977357, -368.320592805117, -6.83512441468002,
                                      187.494050896181, 127.037245798603, 1.87227415530644, -0.353852148205457,
                                      0.184685455471609, -0.956232011720516, -142.600872699714, 0.333318829014491,
                                      3.59658878999684, 6.04107261668509, -52.3843491478172, 2.81138263646351,
                                      2.09412785299796, 2.13632773643865, -1.41291032767183, 1.44656528131016,
                                      0.814332498489863, -0.0598409144030365, 1.79676220190256, 4.05424997648087,
                                      1.35253852044953, 1.13062516952977, 4.07411785607055, 1.80239088558286,
                                      16.4920213868776, -99.368136891383, -9.13492359269983, -41.4451990168298,
                                      -21.049728441576, -17.7193680257034, 11.2268693628618, 5.42758975608667,
                                      15.3064846007022, -15.8928412695707, 50.6227459117088, 3.34961008580836,
                                      34.7912807474769, -0.789863069948706, 0.0818512634676593, -0.182383769561244,
                                      -12.6173282348977, -63.5796847072125, -14.6943780900519, -1.29834629589547,
                                      2.32657615173174, 11.3461837096345, 13.7054309263669, 0.357153076221202,
                                      4.2574960965087, -70.4531323160075, -3.45886852316487, -7.75403034553936,
                                      -28.3357938551584, -106.724984149085, 20.3386251281992, 15.669005571279,
                                      -25.5260889262195, 3.78455132825824, 7.09912169476916, 3.60793987513488,
                                      2.55593036070506, 7.2823329968716, -20.0911837803763, -2.5357564459843,
                                      -34.038808421098, 116.020174168714, 6.39431986777434, -3.52304893082566,
                                      23.8498126948564, 24.37105049659, -2.57334203861286, 39.4477216068527,
                                      34.7347370607311, 24.293723558048, 10.9247581495851, -23.3919466612786,
                                      -12.5272740431242, 2.91661726877488, 65.2089899491658, -0.821632708231853,
                                      10.0597389136459, -9.33282373017072, -2.54636109963274, 54.3024262776542,
                                      3.17262696226562, -4.6237586169574, 8.49812880598145, -0.05133585481912,
                                      -21.498338258214, -16.6114545631868, -39.3569496821455, -13.4585737930859,
                                      -37.7289330041558, -7.20005089579665, 19.4343667924167, -22.9818079702975,
                                      -0.190524960228068, -8.87656106505343, 14.9076891555654, -7.09687443147854,
                                      3.76309865670141, -1.4151357732511, 12.5656852497696, -13.8985354011909,
                                      20.4742781797165, 11.2514472078981, 30.571032754582, 0.608250434764396,
                                      1.42194152492169, 14.6668197586883, -60.9875288368154, 15.660279910113,
                                      -0.73607597025379, -9.93554773469983, 1.89761527605095, 7.30611812999656,
                                      3.84120306148987, 0.434677882322569, 27.5115274631096, -10.8900257697311,
                                      43.5656672966437, 17.5580106437977, 10.4456291630379, 2.08628236699075,
                                      1.87090145528396, -27.4231551947934, -43.4142517149425, 0.211765365858176,
                                      71.2955645497591, -29.2102030543208, -8.47252515340285, 49.5569294305183,
                                      -2.98608021722175, -34.9610180939595, -66.6079199342494, -15.8221530315409,
                                      -12.4571277269559, -6.64272693404906, -9.93018368275267, -30.5212717729689,
                                      14.1230460634061, 1.00263887320635, 5.73201142776254, 5.42127119762915,
                                      -8.83489864292762, 64.6134949352934, 1.46541654568019, -11.390022840464,
                                      17.6915339038657, -92.4226231676457, 20.5308364152349, -100.975130638714,
                                      -58.4546367913373, 3.25409477655164, -28.4728257691939, 16.1381149108551,
                                      4.56351489594264, 3.59649197156019, 13.75003604055, -36.7930495164307,
                                      0.173833017202192, -1.03113696259637, 0.0813847333004408, 4.27798071647667,
                                      2.71962965636132, 0.835563577220945, 1.08291262460266, -32.4978120561404,
                                      16.9982698272109, 49.2521504030098, -119.038363642698, -7.45888126127056,
                                      8.29136231298747, -10.5242217424556, -15.9963823333659, -30.1175965399711,
                                      30.3779999561322, 19.9105572165935, -6.64659400362014, 2.9779216455644,
                                      22.1587693458864, -8.93434078199854, 0.644494683492375, 10.123768360063,
                                      20.7357750395614, 11.5603698609045, 14.9561185930612, 3.34894970017068,
                                      0.381448465174862, 1.50034171395921, 1.08127116267515, 0.0275426415290449,
                                      0.246815128949651, 1.0367246967313, 0.560793928489236, 1.27479868598392,
                                      2.43003654457524, 5.78944212276983, -28.104645574705, -5.44807185013354,
                                      -18.1371108961599, 10.9252353463995, -38.0292486808954, 20.6224514638955,
                                      7.39589219928727, -78.7145175245525, 9.53286420263192, 66.2820700985361,
                                      24.5969967379111, 0.702613955786854, -1.28487711426317, 3.48537476561787,
                                      11.7208596284868, 280.556238168915, -20.3724415433287, 193.128660829665,
                                      -135.39232489665, -10.7116392945671, -7.40735356977067, 1.82184909785004,
                                      -20.8347407462135, 17.5971373409783, 3.16933407629506, 185.143026468592,
                                      -187.659495761322, 221.586120583058, -10.479887902598, -79.0139904188398,
                                      0.665785922359214, -26.4713938339306, -8.52906748840123, -18.2648580130336,
                                      -0.0504905774841734, -10.5202736786044, -48.3736347923978, 30.540128515679,
                                      230.696977399826, -73.0657431091665, 9.92013594193697, -31.4056510962925,
                                      27.710026253259, -10.8115724965317, -0.665454587662371, -0.609022657522665,
                                      68.0042225355259, -210.6262891121, -48.0270628825628, -84.6880421882461,
                                      12.7471587020036, 32.8917741719179, 169.131375044238, 16.6621857291714,
                                      -45.6797099348096, -2.15307732742762, -19.4761179142658, -4.65436463993938,
                                      36.8295843920226, 14.7554709402157, 215.973712359045, -5.41936134102366,
                                      86.6353668414372, 19.7254609183893, -65.6274667403523, -30.5030797156205,
                                      -5.74038169980922, -42.2659804078226, -24.2455861135735, -13.2585853348787,
                                      44.5030562481445, -1.90128770982752, -0.228655594899092, 1.60943000548764,
                                      8.13681100939992, 2.48956014717296, 3.5146113621116, -44.5966595221295,
                                      2.01445947764998, -0.52174221710916, -11.0432151279538, -8.04282713514555,
                                      -2.19494570648323, 5.53944902712748, -10.362424313552, 0.588053807256582,
                                      7.36573157985579, -4.70258364474534, -5.4127213823754, 9.93267857326495,
                                      1.8984380746375, -93.5785182741237, -24.5305659956148, -0.911406911012767,
                                      -5.87930275552172, -1.39350750780115, 11.9039893518258, 38.5395488765794,
                                      -50.1738695896063, -65.7654673691691, 3.7062476584254, -5.50081049083255,
                                      21.5241554128257, -5.5512111591269, 77.7620736154869, 28.7841623133112,
                                      -13.4436766243919, -3.01330952433881, -18.2580532211293, -11.2604231682299,
                                      -5.8663087101939, -5.5002572768141, -51.7661898528411, 7.36628082925137,
                                      -5.15102667609034, -3.59497550135023, -6.12856271167552, -17.319621967118,
                                      164.270649566562, 26.0693310527191, 36.3693288073801, 0.968370825247572,
                                      2.55826513842381, 0.995845583786666, 3.4750068681636, -19.6478101604843,
                                      6.08809411227915, 1.92131972110715, -10.2351076437702, 9.38394702503724,
                                      25.8778486448949, -21.7413818687093, 3.00441378311945, -14.8845458041922,
                                      23.7312189315176, 46.6940266717563, 1.36760080382038, 21.7820152988064,
                                      -22.6442773184721, -68.5903489487932, -39.0568926316333, -43.4640565081245,
                                      -59.1897850082444, -36.9837105007825, 8.64875508777492, -8.4207890386134,
                                      -16.3740217247, -62.7537354404979, -64.1395677727354, -67.4381055486844,
                                      -47.1227592239432, -44.6854672446062, -33.7209852287036, -29.7769715459916,
                                      -54.2705245176015, -37.9895659890433, -57.8615128157371, -28.4600600813211,
                                      -3.39771391253994, -3.83204353326852, 30.4972153786242, 36.2964768471627,
                                      16.058298391953, 23.7029586315322, 27.7895678767564, 40.6931065304684,
                                      14.8744895962322, 50.0816752231041, 56.0618677343959, 57.7170930283822,
                                      68.0253253857645, 49.3757609841563, 50.490338887052, -3.24995423918694,
                                      -0.92758929564786, -48.1417000271819, -43.8268023738593, -43.6019029136653,
                                      5.00500431073114, 12.1972796073327, -16.1024470131684, 2.10172871386043,
                                      6.81312863369006, 27.387888047357, -47.3505549936866, -42.8762484054437,
                                      -35.3077028459475, -33.6336880923445, -34.7430818293602, -17.8909257592977,
                                      32.9034331792481, 31.6601975745042, 52.8237254638932, 41.3768667676603,
                                      39.2964668512747, -51.5242768741758, -44.3530318589027, -12.6292444478363,
                                      23.3361764118114, -17.4173466133755, -17.8505049836374, 17.186221725551,
                                      15.212864371735, -13.7256034724823, -27.6682869344292, 11.8756448445322,
                                      -6.94621228843047, -18.2546064435702, -5.8309522095329, 19.7807874937931,
                                      3.04683734592775, -3.5709212370227, 16.5499973714933, 11.0698921059359,
                                      -18.9018303989039, 2.84921837958753, 22.0601173668627, 2.32662995039818,
                                      -26.1523476587633, -10.3351530539698, 1.66499873113524, -15.9227312205884,
                                      -16.9152869546294, -2.96513137293157, 10.7398050997757, 1.22671078594364,
                                      -24.8324494765963, 9.7580356974292, 12.3424633336829, -15.1958658004335,
                                      -7.22362330942329, 17.8904034237013, 1.3789315727477, -4.48725477632128,
                                      -16.910229586812, 18.7444235759125, 18.4979400674149, -27.0257585131197,
                                      8.06748022810896, 4.66379881296606, 23.7597331761553, -35.0643162980056,
                                      21.383438446988, 1.08751215231342, -9.84680500637657, -3.58686929364077,
                                      16.7121151538265, 4.17514488624202, -7.08926137578004, -1.15917988351028,
                                      5.26249488224645, 0.0242673598727461, -83.1331958984928, -40.4281235021278,
                                      9.81407865514437, -6.78179966037101, -10.1311113851449, -2.38862507282511,
                                      0.264633559164338, 11.3486947060838, -5.513862886448, -1.76918708703671,
                                      19.347334088417, 11.1420265806684, 2.98134133674062, -3.04947120390216,
                                      19.923994321144, -19.5575993310796, -4.25595547812902, 13.3915970507114,
                                      -7.69365572871471, 3.63427900376875, -4.87314398765234, 6.81874700842014,
                                      11.4518970557824, -4.74041843101317, -1.40286780334984, 4.18239874861854,
                                      3.05261918148668, 0.55621173636136, 12.229907603575, 4.78748282761038,
                                      8.10754182615719, 2.69906625496185, 5.45831613620886, 0.0423554803542541,
                                      -0.364796619533276, 0.27635481387595, 2.06063510828623, 6.46843209688888,
                                      -5.32529029340593, 256.326760132131, 7.39284953881078, -45.8902190171147,
                                      65.1752062542441, -44.8418073954667, -40.2213653348157, 18.3372052553697,
                                      -9.42326267052416, -72.2041938797515, 203.285689872308, -64.2404526840863,
                                      60.4014241253228, 109.621805022889, -160.46831529207, -159.85302760543,
                                      -156.003374399404, 171.255920359935, 117.813464804126, -5.44238770633319,
                                      -49.030205078781, -100.132315993106, 227.73626549421, -78.5099644698826,
                                      -131.341038925942, -134.992986909307, -134.760342630028, -147.084153673628,
                                      -158.477627950697, -149.610044408503, -150.317411498223, -120.381911460647,
                                      -123.207568144025, -147.152690394264, -139.574290526607, 109.958463081461,
                                      35.3478122534167, -9.90655580606784, 18.6622062967152, 235.349403052939,
                                      273.239354272223, 102.832301129356, 57.5201982576717, 161.182011154404,
                                      23.2779554126129, -66.141790198711, 5.73735721461815, 103.795172547279,
                                      16.1424001885953, 127.673565721745, -106.075358733116, -84.7804735668444,
                                      251.317823876448, 98.3068203678256, -44.9070588008528, -144.779903723034,
                                      -0.0331342430514678, 41.8826537655451, 257.951024398706, -152.743626962513,
                                      -135.168589194438, -66.7997206914608, -100.567508864683, 199.234864752808,
                                      241.560534483159, 328.324626176801, 160.918954575839, 232.176810672838,
                                      182.379593120052, 183.441106068504, 2.73420352989912, 91.4607337584391,
                                      -133.535079229356, -41.4347889761084, 90.2268420170593, -121.446738794246,
                                      34.5916742305228, 221.663742922919, -110.190043148351, -100.015940490456,
                                      -63.1778444432403, -88.8205198406348, 194.399822182573, 288.453375485713,
                                      199.93638046553, 133.46063097918, -124.007350051832, -59.6906683591448,
                                      -111.902904038832, -73.7925588475516, 178.704604451964, -101.761960638455,
                                      -3.86908721978734, -75.5415551541356, 128.84792451975, 290.756584612857,
                                      111.674602875415, 61.5389257736779, -92.576833185908, -139.132666422154,
                                      -90.3548642858382, -112.181085675234, 152.599195824861, -102.395948921634,
                                      -90.5542558236405, -62.3496630405728, -117.453487549094, 168.588954281953,
                                      6.37621708706166, -79.5589660392299, -72.7220274611862, -123.543096306497,
                                      -64.7753013488063, -126.455208253149, -58.4025638643319, -35.0528812910211,
                                      -88.9227606587558, -61.134107599147, 44.1870872121747, -95.522104192881,
                                      -150.415303764213, -57.7853087878133, 276.099944757066, -76.3535281547849,
                                      -117.179120590438, -106.140868206208, -128.436841662822, -83.9793554099101,
                                      -131.083072890161, -60.0696662881839, -18.5913940735464, -70.1714421528737,
                                      -5.75722184779215, 109.809501162265, 124.076557066047, -74.9238128894936,
                                      -146.829105463086, 108.66368671967, 77.24137363774, -108.881898028802,
                                      229.341872145886, 3.22122628092237, 130.561519609014, 62.4799322239432,
                                      143.821738504322, 158.340501698231, 88.7459696283794, -100.268244648909,
                                      -79.5314946746008, 42.974613231725, -73.4937257192407, 90.1863667455428,
                                      59.398964061318, -121.739298296533, -108.169019557747, 226.488732341065,
                                      -122.935421768906, 1.70309435770747, 196.428644435256, 108.945623530523,
                                      120.408653295315, 272.873858355383, 28.8654436903785, 146.238269121569,
                                      104.639881624493, 72.5612718791622, 134.530210573568, 52.7800651260167,
                                      27.2551977751423, -136.495450504616, -42.430581156273, 104.873336618358,
                                      -153.240932184921, -157.770121773969, -152.963713765123, -130.521077217728,
                                      -134.33576862076, -128.346755193997, -158.127706361586, 45.9501292469847,
                                      49.6855884682871, 128.54589900235, 174.420058150637, 46.3812655014549,
                                      -59.5795869032376, -35.4164200395377, 25.1519610888067, 13.0203590060214,
                                      101.997984918767, -64.6945692602369, -133.419051279152, -106.094836888465,
                                      -82.8201344354625, -82.7102383590492, -119.948820917442, -60.5170633049578,
                                      -67.8688550304162, -12.102101885115, -54.3544904529482, -150.030002779685,
                                      -161.709401394045, -160.923667523584, -159.032400606211, -162.37006107498,
                                      -161.53128769552, -158.867969198632, -158.644569750132, -161.324820828805,
                                      -154.487103177779, -42.2946064497789, -95.7281163477286, -99.8444296091646,
                                      -77.3965494161435, -111.299059154823, 3.26495684808215, -41.9772508069639,
                                      -77.805502882963, -22.462352180719, -54.879651983407, 61.3522612207517,
                                      -33.9030001526074, -25.814414745994, -87.6003128081315, -106.951438482578,
                                      -121.169861543953, 316.295824651664, 30.2094615749104, 298.910008782208,
                                      134.96160563855, -27.7409960828997, -120.390712520755, -52.960598859824,
                                      69.4694690094603, -75.9443002097055, -121.061364817755, 49.6614789884117,
                                      218.816747906577, 238.701912051165, 111.674541737276, 71.2595288289406,
                                      -15.9021681796932, -17.4730158706628, 28.8530470633895, -12.3897933178798,
                                      -106.805139701582, -85.2283571754308, -2.53487119461795, -85.1720655914299,
                                      218.36750780144, 165.299405348143, 188.081677277291, 138.300782712391,
                                      -15.7097144559677, -104.834717653345, -135.746665790009, -107.145926721153,
                                      -29.2326588369252, 344.40356605958, 180.71875938966, 160.872080232968,
                                      160.201633936794, 65.2773923448322, 108.510992239469, 3.42236519072452,
                                      -66.9878275607872, -52.9690342255785, -65.5759442537506, -55.7839023809747,
                                      -121.723144921607, 365.817878885723, 273.65591355907, 194.675672391444,
                                      21.8572460258898, -95.5918896630373, -19.8405379698923, 72.6328543326567,
                                      -55.3308489788534, -30.5118484053969, 108.999996570261, -73.0010396569962,
                                      -54.7659334412347, -57.398128724826, -136.126745161683, -143.72485911393,
                                      -81.1731410347989, -111.669623146235, -39.8870909215436, -82.7813599678653,
                                      -70.4514047288594, -138.871600383174, -119.617179448495, 13.2600335451578,
                                      -119.458728442095, -78.22873665852, -101.898155583052, -88.9528242692842,
                                      -52.1943375377069, -79.11848784384, -34.994457183669, 192.292799881505,
                                      -29.1637973604694, 266.76089244404, 254.06418447713, -42.0564231852875,
                                      -90.0068564080297, 150.475467083044, 129.74598392383, 249.852943161647,
                                      287.53594904428, 43.2725494783318, -109.344937835653, -77.9973540352091,
                                      148.841766986228, -26.9401158430541, 84.4726594440214, 181.581791437209,
                                      -46.6802467048534, -50.364706205594, -56.8126133865037, 1.34744804376906,
                                      60.297929446457, 2.02654140352031, 232.540296751883, 64.4179794945041,
                                      -45.3061987253124, -134.125366069696, 126.082998797261, 57.0137799727039,
                                      248.220781641841, 219.073425267029, -14.6244347873648, -137.333150694148,
                                      -152.985280701767, -156.87531174113, -144.819154127394, -47.2227367013225,
                                      -61.0317246988994, 49.755394177388, 26.5584493893425, 12.0432882762185,
                                      10.0071329919277, -65.6261300078115, -22.3223112066913, 30.0534978486118,
                                      7.24139819643035, -35.4742011527149, -56.6926132694741, -38.4885652979363,
                                      18.9484778117056, -23.9571986098883, -72.5422770849127, -22.0881966743671,
                                      -6.54147267381693, -29.8543552386663, -58.8253954088472, -22.4321106925726,
                                      -38.4324700467432, -4.51396495228949, -79.220040623634, -66.2672859220602,
                                      -50.6007752287919, -33.1862559013601, -79.0390985243078, -76.3606042001841,
                                      -31.1089601397081, -18.7423832749986, -57.4777847847738, -14.0638836279779,
                                      -21.257179136454, -20.0871620151052, -17.9612727652142, -71.2322142906128,
                                      -4.13402331148734, -23.1780875096738, -31.3977601160169, -59.5958291080706,
                                      -30.8622464492713, -30.8019767842593, 20.0942317275717, -42.9439666280637,
                                      -31.1892784772763, -11.1454084173277, 13.3515008059428, -56.7049607227999,
                                      7.49396903526881, 237.904113568696, -32.4656014024289, -10.6835412531484,
                                      -58.7946155993608, -42.5559232989265, -32.1988457102152, -7.76051146897089,
                                      -66.6841619459283, -26.8186097778392, -44.2974706434581, -15.554348995529,
                                      -55.9478310225337, -76.7361450897246, -46.0360999982822, -48.0811929037249,
                                      -23.9827240916144, -48.0766218810529, -15.7808442040063, -23.4735720398735,
                                      -18.4114548543295, -70.1292269991954, -31.3383116303394, 68.6164265469216,
                                      76.8203289100286, 28.0313364930537, 70.97153380565, 28.9734510857257,
                                      56.8082268628578, 27.5983404319569, 51.1477087497723, 9.6722578365429,
                                      -10.6076706322537, 64.6938891913512, 25.5760558221391, 32.4676899322166,
                                      6.41674547335041, 23.6221858913608, 32.5212672346145, 34.4575729952617,
                                      29.4719013867581, 8.95972561815492, 32.4905802804082, 38.7233186532781,
                                      30.0568705407094, 38.4145465763219, 106.680858733784, 64.849870662034,
                                      54.6189977433716, 14.5262704758928, 21.6158518922754, 15.7635651877318,
                                      25.2880458599984, 19.6511696895374, 31.97257877047, 22.9912032727357,
                                      47.0402250290989, 64.5687516007683, 44.1943365055077, 5.86399275910165,
                                      63.730128533177, 32.3190316296689, 23.1622364844804, 70.3045894762476,
                                      44.1303737796186, 46.7976720728719, 103.987522185233, -33.7819717048256,
                                      -40.7224214670958, -49.6870549307333, -23.3534232402425, -103.69767175927,
                                      -16.9132611666036, -11.4820517583422, -72.8168755950007, -36.2762730303882,
                                      0.231355067388135, -65.301240360541, -9.48541047927171, 3.24225290090151,
                                      -21.4622857925058, -51.7743943707811, -57.101774089947, -25.3731933651642,
                                      -8.83570757916427, 4.40000526132327, -78.146043841864, -89.0545424804249,
                                      -29.4462944098067, -13.8803228121647, -63.7899905396186, -33.9690713754923,
                                      -7.06595012308151, -54.7139988097701, -77.0289327285964, -71.6988640526616,
                                      -5.18394072433792, -29.2099989201087, -23.8634893871437, -24.8669161867462,
                                      -4.11832077680678, 28.2240359498358, -18.2127236002404, 16.2827281080733,
                                      -41.8497790266175, 3.84764957902138, 12.7828048743905, -17.9870719909991,
                                      -49.5772401908047, -154.121181066931, -153.75789165971, -156.268328598939,
                                      -156.391265501838, -152.360845029065, -151.021243721688, -35.745350388812,
                                      101.914858967714, 382.767110332941, -98.2369421873778, 183.800967668233,
                                      244.117763130306, 145.721488419039, -1.02957101260965, 76.2197549325666,
                                      46.5390882955329, 143.982487843744, 375.209674569512, -44.0746229025161,
                                      274.994585095326, 191.219019160008, 0.1640625, 0.156982421875,
                                      0.243408203125, 0.279541015625, 0.369140625, 0.355224609375, 0.50732421875,
                                      0.167236328125, 1.025146484375, -0.08251953125, -0.107666015625,
                                      -0.11962890625, -0.099609375, -0.092529296875, -0.12353515625,
                                      -0.10107421875, -0.099853515625, -0.037353515625, -0.06787109375,
                                      -0.083984375, -0.02294921875, 0.196044921875, 0.392578125, 0.55908203125,
                                      0.650146484375, 0.36962890625, 0.400390625, 0.5712890625, 0.53564453125,
                                      0.486083984375, 0.509033203125, 0.409912109375, 0.438232421875,
                                      0.400634765625, 0.28076171875, 0.371337890625, 0.28466796875, 0.244873046875,
                                      0.423095703125, 0.44384765625, 0.294677734375, 0.29248046875, 0.32275390625,
                                      0.513427734375, 0.627685546875, 0.022705078125, -0.063232421875,
                                      0.01904296875, -0.085205078125, 0.320068359375, 0.367919921875,
                                      0.49755859375, 0.549560546875, 0.309814453125, 0.44287109375, 0.41748046875,
                                      0.33740234375, 0.415283203125, 0.214111328125, 0.270263671875,
                                      0.069091796875, -0.081298828125, -0.22412109375, 0.03125, -0.146484375,
                                      -0.060302734375, -0.12060546875, -0.152099609375, 0.244384765625,
                                      0.478271484375, 0.517333984375, 0.3271484375, 0.2666015625, 0.373291015625,
                                      0.247314453125, 0.346435546875, 0.48486328125, 0.13232421875, 0.48486328125,
                                      0.540771484375, 0.49609375, 0.565185546875, 0.384521484375, 0.53125,
                                      0.502197265625, 0.5, 0.5283203125, 0.581298828125, 0.541015625,
                                      0.308837890625, 0.32275390625, 0.399169921875, 0.34326171875, 0.33251953125,
                                      0.62353515625, 0.252685546875, 0.337646484375, 0.169921875, 0.212890625,
                                      0.077392578125, 0.081298828125, 0.211181640625, 0.152587890625,
                                      0.17431640625, 1.26806640625, 1.057373046875, 0.290283203125, 0.399658203125,
                                      0.5556640625, 0.126953125, 0.3134765625, 0.467529296875, 0.17626953125,
                                      0.14892578125, 0.1669921875, 0.320068359375, 0.343994140625, 0.19140625,
                                      0.29736328125, 0.241455078125, 0.14990234375, 0.272216796875, 0.091796875,
                                      0.143310546875, 0.314697265625, 0.117919921875, 0.201416015625,
                                      -0.05322265625, 0.29296875, 0.398681640625, 0.487060546875, 0.418701171875,
                                      0.384521484375, 0.049560546875, 0.15283203125, 0.39453125, 0.135986328125,
                                      0.3623046875, 0.19140625, 0.262451171875, 0.12255859375, 0.984130859375,
                                      -0.0498046875, 0.100830078125, 0.11181640625, 0.43701171875, 0.455078125,
                                      0.263427734375, 0.399658203125, 0.592041015625, 0.5419921875, 0.325927734375,
                                      0.25732421875, 0.3095703125, 0.276123046875, 0.137451171875, 0.132568359375,
                                      1.99993896484375, -0.165283203125, -0.1787109375, -0.1591796875,
                                      -0.13330078125, -0.14404296875, -0.11572265625, -0.184326171875,
                                      0.10400390625, 0.21240234375, 0.330810546875, 0.313232421875, 0.226318359375,
                                      0.261474609375, 0.318359375, 0.60205078125, 0.579345703125, 0.2587890625,
                                      0.173828125, 0.038330078125, 0.0361328125, 0.057861328125, 0.07666015625,
                                      0.1123046875, 0.106201171875, 0.171142578125, 0.125244140625, 0.2880859375,
                                      0.17529296875, 0.177734375, 0.1708984375, 0.1611328125, 0.155517578125,
                                      0.166015625, 0.164794921875, 0.16064453125, 0.169677734375, 0.1611328125,
                                      0.204833984375, 0.29833984375, 0.26123046875, 0.267578125, 0.261474609375,
                                      0.1337890625, 0.218505859375, 0.24072265625, 0.157470703125, 0.14501953125,
                                      0.1376953125, 0.337646484375, 0.328857421875, 0.229736328125, 0.467529296875,
                                      0.326416015625, 1.99993896484375, -0.26416015625, 0.209228515625,
                                      0.477783203125, 0.33447265625, 0.12060546875, 0.3486328125, 0.1904296875,
                                      0.33984375, 0.14404296875, 0.130859375, 1.559814453125, 0.26123046875,
                                      -0.124267578125, 0.70849609375, 0.33447265625, 0.312744140625, 0.4404296875,
                                      0.30712890625, 0.180419921875, 0.3212890625, 0.31640625, 0.26318359375,
                                      0.233642578125, -0.25439453125, -0.08642578125, 0.30078125, 0.356201171875,
                                      0.07763671875, 0.005859375, 0.10498046875, 0.217529296875, 1.99993896484375,
                                      -0.235595703125, 0.294921875, 0.287841796875, 0.363525390625, 0.66015625,
                                      0.25146484375, 0.275390625, 0.23583984375, 0.17529296875, 0.160888671875,
                                      0.12158203125, 0.171630859375, -0.306396484375, 0.186279296875,
                                      0.320556640625, -0.03173828125, 0.538330078125, 0.2373046875, 0.20849609375,
                                      0.171875, 0.19287109375, 0.149169921875, 0.13818359375, 0.006591796875,
                                      0.0361328125, 0.00537109375, 0.179931640625, -0.004638671875, 0.0361328125,
                                      0.276611328125, 0.347900390625, 0.215087890625, 0.26953125, 0.33251953125,
                                      0.21337890625, 0.156494140625, 0.169189453125, 0.15576171875, 0.128662109375,
                                      0.14990234375, 0.10546875, 1.74853515625, 0.14892578125, 0.695068359375,
                                      0.303955078125, 0.31689453125, 0.18359375, 0.777587890625, 0.31591796875,
                                      0.536376953125, 1.0146484375, 0.4296875, 0.19580078125, 0.18798828125,
                                      1.54736328125, 0.2919921875, 0.203369140625, 0.323486328125, 0.325927734375,
                                      0.357177734375, 0.32861328125, 0.14794921875, 1.99993896484375,
                                      0.286376953125, 0.295166015625, 0.363037109375, 0.301513671875,
                                      0.169189453125, 1.492431640625, 0.765625, 0.392333984375, 0.234130859375,
                                      0.293212890625, 0.146728515625, 0.1279296875, 0.114990234375, 0.13525390625,
                                      0.289306640625, 0.24072265625, 0.22900390625, 0.270751953125, 0.281005859375,
                                      0.284423828125, 0.245361328125, 0.272216796875, 0.334228515625,
                                      0.436279296875, 0.332275390625, 0.339599609375, 0.373291015625, 0.345703125,
                                      0.35498046875, 0.34033203125, 0.399169921875, 0.35986328125, 0.366455078125,
                                      0.357666015625, 0.406982421875, 0.3564453125, 0.359619140625, 0.28369140625,
                                      0.332763671875, 0.37353515625, 0.35107421875, 0.33544921875, 0.2978515625,
                                      0.3125, 0.370361328125, 0.23974609375, 0.33544921875, 0.268798828125,
                                      0.337158203125, 0.3076171875, 0.287109375, 0.322265625, 0.334716796875,
                                      0.30908203125, 0.323486328125, 0.292236328125, 0.323486328125,
                                      0.345458984375, 0.338134765625, 0.41162109375, 0.316650390625,
                                      0.476806640625, 0.35009765625, 0.264404296875, 0.310546875, 0.2939453125,
                                      0.321533203125, 0.299072265625, 0.36962890625, 0.27294921875, 0.372802734375,
                                      0.27294921875, 0.25146484375, 0.324951171875, 0.306396484375, 0.275390625,
                                      0.321533203125, 0.299560546875, 0.280517578125, 0.35595703125, 0.291015625,
                                      0.236572265625, 0.40966796875, 0.327392578125, 0.261474609375, 0.26953125,
                                      1.057373046875, 0.752197265625, 0.884521484375, 0.98046875, 0.539306640625,
                                      0.521728515625, 0.85986328125, 0.525146484375, 0.62451171875, 0.461181640625,
                                      0.4072265625, 0.492919921875, 0.48291015625, 0.33154296875, 0.365478515625,
                                      0.53515625, 0.438720703125, 0.6884765625, 0.51025390625, 0.759521484375,
                                      0.443359375, 0.54638671875, 0.467041015625, 0.38818359375, 0.572265625,
                                      0.505859375, 0.5146484375, 0.45263671875, 0.365966796875, 0.44189453125,
                                      0.4267578125, 0.28173828125, 0.517822265625, 0.63037109375, 0.424560546875,
                                      0.325927734375, 0.459716796875, 0.549560546875, 0.501220703125,
                                      0.441650390625, 0.74755859375, 0.41455078125, 0.41015625, 0.432373046875,
                                      0.117919921875, 0.202392578125, 0.259033203125, 0.218505859375,
                                      0.216552734375, 0.213623046875, 0.226806640625, 0.25927734375, 0.21240234375,
                                      0.234375, 0.24951171875, 0.6748046875, 0.460205078125, 0.281494140625,
                                      0.20166015625, 0.264892578125, 0.23193359375, 0.1884765625, 0.2646484375,
                                      0.224853515625, 0.197021484375, 0.23876953125, 0.194580078125,
                                      0.266357421875, 0.156982421875, 0.249755859375, 0.319091796875,
                                      0.177001953125, 0.211181640625, 0.664306640625, 0.660888671875, 0.59765625,
                                      0.54931640625, 0.800048828125, 0.672119140625, 0.60546875, 0.5439453125,
                                      0.443359375, 0.484130859375, 1.311279296875, 0.625244140625, 0.649658203125,
                                      0.139404296875, 0.151611328125, 0.13916015625, 0.13525390625, 0.126953125,
                                      0.169189453125, 0.330322265625, 0.381591796875, 0.301025390625,
                                      -0.808837890625, -0.52587890625, 0.505126953125, 0.411376953125,
                                      0.374267578125, 0.49267578125, 0.380859375, 0.436767578125, -0.119140625,
                                      -0.8193359375, -0.26708984375, 0.38720703125, -0.95849609375, -0.9609375,
                                      -0.9609375, -0.930419921875, -0.80419921875, -0.865234375, -0.312255859375,
                                      -0.821044921875, 1.99993896484375, 0.085693359375, 0.12060546875,
                                      0.201904296875, 0.259033203125, 0.2646484375, 0.241455078125, 0.2314453125,
                                      0.257568359375, 0.36572265625, 0.35498046875, 0.35009765625, 0.445556640625,
                                      0.4072265625, -0.726806640625, -0.763671875, -0.71630859375, -0.40869140625,
                                      0.3056640625, -0.772705078125, -0.77685546875, -0.834716796875,
                                      -0.727294921875, -0.789306640625, -0.665283203125, -0.7998046875,
                                      -0.849609375, -0.814453125, -0.906494140625, -0.923583984375,
                                      -0.722900390625, -0.68212890625, -0.86962890625, -0.92041015625,
                                      -0.888427734375, -0.768798828125, 1.99993896484375, 0.204833984375,
                                      0.28662109375, 0.478271484375, 0.4736328125, 0.45654296875, -0.573486328125,
                                      -0.509765625, -0.749267578125, -0.841796875, -0.78173828125, -0.8544921875,
                                      -0.852294921875, -0.87548828125, -0.891845703125, -0.187255859375,
                                      1.99993896484375, 0.492431640625, 0.3388671875, 0.6103515625, 0.45556640625,
                                      0.449951171875, 0.16650390625, 0.268310546875, 0.51025390625, -0.31201171875,
                                      -0.5673828125, -0.6787109375, -0.86376953125, -0.871826171875,
                                      -0.843017578125, -0.852783203125, -0.3837890625, -0.533935546875,
                                      -0.461181640625, -0.63671875, -0.740234375, -0.828857421875, -0.8623046875,
                                      -0.5751953125, -0.78955078125, -0.835205078125, -0.834716796875,
                                      -0.802734375, -0.771728515625, -0.9404296875, -0.92626953125, -0.919921875,
                                      -0.94677734375, -0.888916015625, -0.902587890625, -0.78955078125,
                                      -0.76416015625, -0.924560546875, -0.898681640625, -0.881103515625,
                                      -0.829345703125, -0.888671875, -0.857177734375, -0.849609375, -0.8837890625,
                                      -0.611572265625, -0.926025390625, -0.736083984375, -0.48681640625,
                                      -0.896484375, -0.96826171875, -0.622314453125, -0.913818359375,
                                      -0.969482421875, -0.9501953125, -0.396484375, -0.840576171875,
                                      -0.859130859375, -0.776611328125, -0.825927734375, -0.68115234375,
                                      -0.8447265625, -0.92236328125, -0.75244140625, -0.85986328125, -0.869140625,
                                      0.81591796875, 0.30712890625, -0.615234375, -0.709716796875, -0.708740234375,
                                      -0.43798828125, -0.7587890625, -0.97412109375, -0.88671875, -0.850830078125,
                                      -0.87548828125, -0.871826171875, -0.88427734375, -0.74853515625,
                                      -0.85302734375, 1.99993896484375, 0.3486328125, 0.45947265625,
                                      0.132080078125, -0.369140625, -0.651123046875, -0.6904296875, -0.693359375,
                                      -0.71826171875, -0.79248046875, -0.71435546875, -0.80615234375,
                                      -0.80810546875, -0.811767578125, -0.832275390625, -0.523193359375,
                                      1.805908203125, 0.304443359375, 0.26220703125, 0.227294921875,
                                      0.207763671875, 0.35205078125, 0.298583984375, 0.2421875, 0.34228515625,
                                      -0.437255859375, -0.441650390625, -0.704833984375, -0.781982421875,
                                      -0.7744140625, -0.744384765625, -0.73486328125, -0.803955078125,
                                      -0.829345703125, -0.813232421875, -0.888427734375, -0.95556640625,
                                      -0.943359375, -0.8623046875, -0.880615234375, -0.897705078125,
                                      -0.922119140625, -0.844970703125, -0.90869140625, -0.986083984375,
                                      -0.98779296875, -0.98974609375, -0.994140625, -0.989013671875,
                                      -0.990966796875, -0.9921875, -0.992431640625, -0.994873046875,
                                      -0.981689453125, -0.849365234375, -0.938232421875, -0.8857421875,
                                      -0.912353515625, -0.8798828125, -0.79833984375, -0.956298828125,
                                      -0.916015625, -0.873291015625, -0.8984375, -0.914306640625, -0.726318359375,
                                      -0.84521484375, -0.874267578125, -0.83447265625, -0.7734375, 0.6669921875,
                                      0.3408203125, -0.460205078125, -0.333251953125, -0.560791015625,
                                      -0.92041015625, -0.779541015625, -0.7275390625, -0.829345703125,
                                      -0.79638671875, 0.307861328125, 0.87841796875, -0.4697265625,
                                      -0.291259765625, -0.5068359375, -0.819580078125, -0.904541015625,
                                      -0.79150390625, -0.637939453125, -0.764404296875, -0.904296875,
                                      -0.62451171875, -0.89111328125, 1.99993896484375, 0.53125, 0.489990234375,
                                      -0.17333984375, -0.53125, -0.889404296875, -0.969970703125, -0.92236328125,
                                      0.214111328125, 0.621826171875, 0.41650390625, -0.533203125, -0.76904296875,
                                      -0.101318359375, -0.65234375, -0.85693359375, -0.805419921875, -0.7880859375,
                                      -0.953857421875, -0.85888671875, -0.949951171875, 1.99993896484375,
                                      0.49951171875, 0.730712890625, -0.493408203125, -0.90087890625,
                                      -0.618896484375, -0.818115234375, -0.828369140625, -0.75146484375,
                                      -0.78759765625, -0.79248046875, -0.87646484375, -0.858642578125,
                                      -0.930908203125, -0.957275390625, -0.949462890625, -0.951416015625,
                                      -0.984619140625, -0.749755859375, -0.872802734375, -0.97265625,
                                      -0.9287109375, -0.8720703125, -0.9833984375, -0.952392578125,
                                      -0.987548828125, -0.981689453125, -0.9326171875, -0.533203125,
                                      -0.525634765625, 1.043212890625, 0.112548828125, 0.376953125, -0.4140625,
                                      -0.638916015625, -0.873779296875, 1.99993896484375, 0.203125, 0.343017578125,
                                      -0.398193359375, -0.244873046875, -0.849365234375, -0.816650390625,
                                      0.748046875, -0.105224609375, 0.049072265625, -0.294921875, -0.80322265625,
                                      -0.788330078125, -0.834228515625, -0.234375, 0.724609375, 0.081787109375,
                                      -0.38037109375, -0.419189453125, -0.6640625, -0.939208984375, 0.440185546875,
                                      0.142333984375, 0.127685546875, -0.42529296875, -0.699462890625,
                                      -0.957275390625, -0.9951171875, -0.9970703125, -0.86181640625, -0.8896484375,
                                      -0.9169921875, -0.82958984375, -0.763916015625, -0.763427734375,
                                      -0.815673828125, -0.720703125, -0.6328125, -0.84228515625, -0.743896484375,
                                      -0.747314453125, -0.4560546875, -0.8447265625, -0.769775390625,
                                      -0.566650390625, -0.6640625, -0.78759765625, -0.731201171875, -0.6201171875,
                                      -0.657470703125, -0.83251953125, -0.804931640625, -0.8212890625,
                                      -0.580078125, -0.772216796875, -0.81640625, -0.876708984375, -0.601806640625,
                                      -0.76513671875, -0.75634765625, -0.778564453125, -0.652099609375,
                                      -0.711181640625, -0.854736328125, -0.73193359375, -0.599853515625,
                                      -0.67578125, -0.71826171875, -0.807861328125, -0.682861328125, -0.6953125,
                                      -0.6513671875, -0.759033203125, -0.790771484375, -0.660888671875,
                                      -0.57568359375, -0.85693359375, -0.716796875, -0.732666015625,
                                      -0.542236328125, -0.90771484375, -0.705322265625, -0.70263671875,
                                      -0.66943359375, -0.80908203125, -0.75830078125, -0.811279296875,
                                      -0.613525390625, -0.623046875, -0.80859375, -0.811279296875, -0.696044921875,
                                      -0.652587890625, -0.74560546875, -0.812744140625, -0.85791015625,
                                      -0.719482421875, -0.73291015625, -0.79541015625, -0.7255859375,
                                      -0.708251953125, -0.40625, 0.495849609375, 0.49609375, 0.36865234375,
                                      0.171630859375, 0.58935546875, 0.194091796875, 0.06396484375, 0.07763671875,
                                      -0.10107421875, -0.07373046875, -0.00390625, 0.05029296875, -0.09619140625,
                                      -0.01513671875, -0.19677734375, 0.02392578125, 0.047119140625,
                                      -0.061767578125, 0.13037109375, 0.008544921875, -0.1171875, -0.15087890625,
                                      -0.005126953125, -0.131591796875, -0.3310546875, 0.028564453125,
                                      -0.05712890625, -0.12646484375, -0.08935546875, -0.1708984375,
                                      -0.341552734375, -0.21337890625, -0.56494140625, -0.25732421875,
                                      -0.29248046875, -0.27294921875, -0.039794921875, -0.211181640625,
                                      -0.327880859375, -0.207275390625, -0.237060546875, -0.451904296875,
                                      -0.381591796875, -0.36572265625, -0.26806640625, -0.65185546875,
                                      -0.70556640625, -0.5966796875, -0.81103515625, -0.66357421875,
                                      -0.58447265625, -0.8447265625, -0.75732421875, -0.698486328125,
                                      -0.81982421875, -0.453857421875, -0.531982421875, -0.729736328125,
                                      -0.775390625, -0.888427734375, -0.749267578125, -0.7431640625,
                                      -0.75341796875, -0.77685546875, -0.905029296875, -0.6796875, -0.69677734375,
                                      -0.712890625, -0.7421875, -0.715576171875, -0.85400390625, -0.677490234375,
                                      -0.845947265625, 0.520751953125, 0.439453125, 0.55859375, 0.805908203125,
                                      0.3291015625, 0.705322265625, 0.466796875, 0.652587890625, 0.643310546875,
                                      0.538818359375, 0.75048828125, 0.805419921875, 0.023193359375,
                                      -0.979736328125, -0.99365234375, -0.996337890625, -0.99462890625,
                                      -0.98681640625, -0.991943359375, -0.784912109375, -0.218017578125,
                                      0.14794921875, -0.046630859375, -0.0234375, 0.572021484375, -0.287353515625,
                                      -0.772705078125, -0.62158203125, -0.757568359375, -0.29541015625,
                                      0.268798828125, 0.00634765625, 0.40576171875, 0.15869140625, 0.177001953125,
                                      0.1728515625, 0.182373046875, 0.1572265625, 0.07958984375, 0.359130859375,
                                      0.3056640625, 0.3193359375, 1.7529296875, 0.98974609375, 1.0126953125,
                                      0.980712890625, 0.94677734375, 0.91943359375, 0.896484375, 0.91796875,
                                      0.94189453125, 0.965576171875, 0.973388671875, 0.928466796875,
                                      0.910400390625, 1.01123046875, 0.390625, 0.247314453125, 0.342041015625,
                                      0.751708984375, 0.4794921875, 0.249755859375, 0.095703125, 0.087158203125,
                                      0.15576171875, 0.098388671875, 0.17138671875, 0.06640625, -0.050537109375,
                                      0.13525390625, 0.05126953125, 0.000244140625, 0.097900390625, -0.24365234375,
                                      0.033935546875, 0.001953125, -0.038818359375, 0.11767578125,
                                      1.99993896484375, 0.92333984375, 0.9296875, 1.002685546875, 0.956787109375,
                                      1.199462890625, 0.5625, 0.367431640625, 0.615234375, 0.205322265625,
                                      0.009033203125, -0.071533203125, -0.0625, 0.198974609375, 0.224853515625,
                                      0.17041015625, 0.942138671875, 0.88427734375, 0.84375, 0.967041015625,
                                      1.005859375, 0.93017578125, 0.924560546875, 0.979248046875, 1.031005859375,
                                      0.3291015625, 0.283203125, 0.220947265625, 0.2568359375, 0.17333984375,
                                      0.126708984375, 0.185302734375, 0.019775390625, -0.8310546875,
                                      -0.615478515625, -0.5625, -0.0263671875, 0.397216796875, 0.19677734375,
                                      0.097900390625, -0.1083984375, -0.229248046875, -0.24658203125,
                                      -0.26318359375, -0.072509765625, 0.019775390625, -0.1064453125,
                                      -0.07763671875, -0.025390625, 0.30908203125, 0.27685546875, 0.20849609375,
                                      0.21923828125, 0.196044921875, 0.4404296875, 0.4873046875, 0.353515625,
                                      0.3623046875, 0.37060546875, 0.272705078125, 1.99993896484375,
                                      1.061767578125, 0.309326171875, 0.693359375, 0.22998046875, 0.306396484375,
                                      0.07080078125, 0.726318359375, 0.134765625, 0.185302734375, 0.10693359375,
                                      0.126708984375, 0.04638671875, 0.0625, 0.142333984375, 0.068603515625,
                                      0.857666015625, 0.398193359375, 0.220947265625, 0.323486328125, -0.025390625,
                                      0.25341796875, 1.99993896484375, 1.00830078125, 0.615234375, 0.19482421875,
                                      0.578369140625, 0.401123046875, 0.040283203125, -0.031494140625,
                                      0.02490234375, 0.234130859375, 0.457763671875, 0.28857421875, 0.2099609375,
                                      0.17822265625, 0.28564453125, 1.078857421875, 0.96875, 1.43408203125,
                                      1.03466796875, 0.36474609375, -0.04150390625, 0.3310546875, 0.25634765625,
                                      0.03759765625, -0.112548828125, 0.667724609375, 0.36279296875,
                                      -0.082763671875, 0.1328125, 0.200927734375, 0.16455078125, 1.99993896484375,
                                      0.899169921875, 0.8876953125, 0.887939453125, 0.959228515625, 0.924072265625,
                                      0.91748046875, 0.88916015625, 1.024658203125, 0.484375, -0.2919921875,
                                      0.3935546875, 0.416748046875, 0.28076171875, 0.18359375, 0.0498046875,
                                      0.07666015625, 0.209716796875, 0.168212890625, 0.175048828125,
                                      0.117431640625, 0.162109375, 0.35107421875, 0.16357421875, 0.152587890625,
                                      0.13525390625, 0.16259765625, 0.01953125, -0.111572265625, -0.140625,
                                      -0.15185546875, -0.14599609375, -0.129638671875, -0.115478515625,
                                      -0.12548828125, -0.12255859375, -0.103271484375, -0.050048828125,
                                      0.08837890625, 0.034423828125, 0.072021484375, 0.143310546875,
                                      0.059814453125, -0.015869140625, 0.05029296875, 0.07568359375, 0.18408203125,
                                      0.205078125, 0.11669921875, 0.06298828125, 0.30419921875, 0.087646484375,
                                      -0.062255859375, -0.0634765625, 1.99993896484375, 0.787353515625,
                                      0.205078125, -0.068603515625, -0.05126953125, 0.053466796875,
                                      -0.002685546875, 0.446533203125, -0.148193359375, -0.1123046875,
                                      -0.01611328125, 0.47802734375, -0.0673828125, -0.275634765625, -0.078125,
                                      -0.180908203125, -0.030029296875, 0.155517578125, -0.175048828125,
                                      -0.0224609375, -0.033935546875, -0.002197265625, -0.168212890625,
                                      1.982177734375, 0.939697265625, -0.452392578125, -0.216064453125,
                                      -0.130859375, -0.107421875, -0.09423828125, -0.123046875, -0.11474609375,
                                      1.99993896484375, -0.523193359375, -0.048095703125, -0.05517578125,
                                      -0.061279296875, 0.170654296875, -0.025390625, -0.218505859375,
                                      -0.089599609375, 0.018310546875, 0.159912109375, -0.12353515625,
                                      1.48291015625, 0.22412109375, -0.427001953125, -0.081787109375,
                                      -0.190185546875, -0.175048828125, -0.170166015625, -0.0341796875,
                                      -0.046142578125, -0.012451171875, -0.11669921875, -0.104736328125,
                                      0.176513671875, -0.083740234375, -0.146728515625, -0.220947265625,
                                      -0.1669921875, -0.095458984375, -0.10498046875, 0.036376953125,
                                      -0.166748046875, -0.061767578125, 0.205322265625, -0.09228515625,
                                      -0.124755859375, -0.166748046875, -0.13037109375, -0.158203125,
                                      -0.1748046875, -0.3251953125, -0.0625, -0.79345703125, -0.6669921875,
                                      -0.164794921875, -0.079833984375, -0.08154296875, -0.109375, -0.77587890625,
                                      -0.7587890625, -0.072021484375, -0.169189453125, -0.1787109375,
                                      -0.163818359375, -0.087158203125, -0.8896484375, -0.490478515625,
                                      -0.159423828125, -0.129638671875, -0.08203125, -0.011962890625,
                                      -0.055419921875, -0.10107421875, -0.860107421875, 0.059326171875,
                                      -0.264892578125, -0.07275390625, -0.136962890625, 0.06005859375,
                                      -0.742919921875, -0.445556640625, 0.030517578125, -0.006103515625,
                                      -0.016845703125, -0.03955078125, -0.068115234375, -0.04443359375,
                                      -0.05224609375, 0.017578125, -0.0693359375, -0.039794921875, -0.035400390625,
                                      -0.118408203125, -0.035400390625, -0.00927734375, -0.02294921875,
                                      -0.0947265625, -0.013427734375, -0.03125, -0.085693359375, -0.0546875,
                                      -0.02001953125, 0.031005859375, 0.0830078125, -0.06298828125,
                                      -0.022216796875, -0.1083984375, -0.024169921875, -0.0849609375,
                                      -0.12158203125, -0.116455078125, -0.13134765625, -0.08349609375,
                                      -0.148193359375, -0.057861328125, -0.0859375, -0.01171875, -0.081787109375,
                                      -0.026611328125, -0.024658203125, -0.0556640625, -0.07666015625,
                                      -0.157958984375, -0.0400390625, 0.034423828125, 0.004638671875,
                                      -0.05224609375, 0.052490234375, 0.04443359375, -0.029052734375, 0.2294921875,
                                      -0.029052734375, 0.110595703125, -0.048583984375, 0.0146484375,
                                      -0.00537109375, -0.026611328125, 0.0205078125, 0.078369140625, 0.0361328125,
                                      0.097900390625, -0.032958984375, 0.011962890625, 0.221923828125,
                                      -0.002197265625, 0.0673828125, 0.03662109375, 0.034423828125,
                                      -0.047607421875, 0.057861328125, 0.017578125, 0.017822265625, -0.06298828125,
                                      -0.015625, 0.10400390625, 0.0361328125, -0.125, -0.1123046875,
                                      -0.100830078125, 0.224853515625, 0.27197265625, 0.239990234375,
                                      0.095458984375, 0.16015625, 0.338134765625, 0.18408203125, 0.06884765625,
                                      0.20361328125, 0.077392578125, 0.466796875, 0.17626953125, 0.064208984375,
                                      0.06298828125, 0.257568359375, 0.113525390625, 0.504150390625, 0.1435546875,
                                      0.110595703125, 0.5224609375, 0.160888671875, 0.08154296875, 0.287353515625,
                                      0.29296875, 0.120361328125, 0.0888671875, 0.187744140625, 0.140869140625,
                                      0.14013671875, 0.209716796875, 0.167724609375, 0.166015625, 0.0908203125,
                                      0.10791015625, 0.17138671875, 0.174072265625, 0.065673828125, 0.128662109375,
                                      0.168212890625, 0.073974609375, 0.413330078125, 0.093994140625, 0.03515625,
                                      -0.086181640625, -0.875, -0.310546875, -0.200439453125, 0.043212890625,
                                      -0.26171875, 0.464599609375, -0.00146484375, -0.250244140625, 0.30224609375,
                                      -0.037841796875, -0.224609375, 0.0068359375, -0.14990234375, 0.0078125,
                                      0.111083984375, -0.17529296875, 0.068359375, -0.15625, 0.001220703125,
                                      0.072509765625, -0.204833984375, 0.042236328125, -0.021484375, -0.3134765625,
                                      0.05322265625, -0.005615234375, -0.203857421875, 0.08447265625,
                                      0.005615234375, -0.04296875, 0.18505859375, -0.044189453125, 0.163330078125,
                                      0.046142578125, 0.010498046875, 0.21044921875, 0.1005859375, -0.03271484375,
                                      0.126953125, -0.002685546875, 0.032470703125, 0.0556640625, -0.05517578125,
                                      -0.049072265625, -0.068359375, -0.074462890625, -0.0908203125,
                                      -0.076416015625, -0.071044921875, -0.103271484375, 0.536376953125,
                                      0.26904296875, 0.24462890625, -0.47314453125, -0.145751953125,
                                      0.018798828125, 0.016845703125, 0.03466796875, -0.0732421875, 0.572509765625,
                                      0.61376953125, 0.082763671875, -0.3896484375, 1.00827234703779,
                                      1.00473596743703, 1.03071218974258, 1.08564625409041, 1.15922498735004,
                                      1.12005322380986, 2.08033768015025, 1.13749005344779, 3.3275393840523,
                                      1.00248246218022, 1.03152025900735, 1.00538876088623, 0.965252160495493,
                                      0.949442016968496, 0.929168556336903, 0.940669234240097, 0.975684908408871,
                                      0.990820897224565, 1.00205169252392, 0.983463682947999, 0.961915487685334,
                                      1.11505280468014, 1.17516328407579, 1.33561312211877, 1.10594226414672,
                                      1.29610348928332, 1.72237807469364, 1.21763160938114, 1.16425140179432,
                                      1.25005717146784, 1.30810132227145, 1.19842823825724, 1.26454431996296,
                                      1.20509714269238, 1.21063713532423, 1.29714418398208, 1.15441504065648,
                                      1.23392995223464, 1.23900049061646, 1.20169815071613, 1.16932331751877,
                                      1.04259407085575, 1.04339208119616, 1.28550590234245, 3.46403113816845,
                                      0.95508539552008, 0.99098784844217, 1.01306422254251, 1.06750881222699,
                                      1.33356908363097, 1.65958310275308, 1.3548010685009, 1.41193803562926,
                                      1.20817516650989, 1.16339435219708, 1.13327553592358, 1.18295288361488,
                                      1.11666425709441, 1.09472160780617, 1.20941329520853, 2.90907659856999,
                                      0.984585633524757, 1.02236378380317, 1.28655940854413, 1.09035116435995,
                                      1.05818007716204, 0.994870201059964, 1.09450717330471, 1.25143979122286,
                                      1.68138109922675, 1.40957946816656, 1.22285952112313, 1.05441602815897,
                                      1.153174718183, 1.06830776724573, 1.11524319352974, 1.21879592222114,
                                      1.13708031531064, 1.20815750465141, 1.17206373602229, 1.15501497655299,
                                      1.17548658157407, 1.16907596734352, 1.38968776880446, 1.10108691290666,
                                      1.07172948778232, 1.09308042466986, 1.09387307155833, 1.24735765126618,
                                      1.05102811274691, 1.10549856624226, 1.04077618047776, 1.07652827901944,
                                      1.12742953750621, 1.23306760481349, 1.0489959014307, 1.09060617665658,
                                      1.04828733571234, 1.11539889590205, 1.07511007610351, 1.09435280128679,
                                      1.20628116349993, 1.10070781151443, 1.1392283053397, 3.3942775829077,
                                      1.53791816886361, 1.0651610653827, 1.73059739437659, 1.26750657830422,
                                      1.16241363901917, 1.14821503229632, 1.06461132868756, 1.39566432109949,
                                      1.26096933506671, 1.06487393241978, 1.17927576926249, 1.29713878476708,
                                      1.20385982317464, 1.29916001288615, 1.40340665951133, 1.32715263674804,
                                      1.17536678169588, 1.02218347262339, 1.17086066534285, 1.27478840324997,
                                      1.09824884783404, 2.83719198536552, 1.04256514265682, 1.31242163742102,
                                      1.55516884411473, 1.48189325167752, 1.69991766926608, 1.23067637311611,
                                      1.07958269397261, 1.12152402376558, 1.29507332631044, 1.07325221188058,
                                      1.22362282665955, 1.17886973451521, 1.18018169030702, 1.05362113449758,
                                      2.40928880439009, 1.00069022200187, 1.47211794538898, 1.43682524144854,
                                      1.76988513094265, 1.34929358490739, 1.20060813514664, 1.45062241349818,
                                      1.33208389401934, 1.27526018255521, 1.39867092294261, 1.12761310576786,
                                      1.35758101813427, 1.19282344890044, 1.07866594316398, 1.02781666361359,
                                      3.0870318639206, 0.968609030526579, 0.941526191636811, 0.9361922679973,
                                      1.00720542543789, 0.952226246319961, 0.956134672004533, 0.929871004090131,
                                      1.07470464641163, 1.34713392225038, 1.71333129851459, 1.21899000885697,
                                      1.20718977791425, 1.15061881064222, 1.20720172653486, 1.30483708694798,
                                      1.23436423188354, 1.39550680877244, 1.18236793014424, 1.11190995202711,
                                      1.07835709105434, 1.06424384903424, 1.16466063934951, 1.08062873588168,
                                      1.14088578051843, 1.06351929746013, 1.15314357608213, 1.17706474638361,
                                      1.0372552858863, 1.03427112092307, 1.03384803302524, 1.03259106338383,
                                      1.03488607659594, 1.03842120350378, 1.03515296791564, 1.03375425584648,
                                      1.03216044377511, 1.0676966813855, 1.11969249493195, 1.09113678059734,
                                      1.11140481855176, 1.15915544286889, 1.05335377322256, 1.28284682809896,
                                      1.08779695884958, 1.08270752082771, 1.15412150178716, 1.10513446188264,
                                      1.12496953499129, 1.308367877654, 1.23083464011397, 1.18999402505513,
                                      1.15478861447355, 1.26701384740938, 2.89008389189607, 1.15955830587577,
                                      1.31091196628661, 2.00423262171035, 1.61658846315352, 1.09147696678963,
                                      1.34890578571419, 1.12232510136583, 1.22258431707483, 1.25875345420932,
                                      1.03568360912763, 3.16505371391674, 1.21774918471646, 1.63709578803142,
                                      1.29674860326847, 1.22914631336409, 1.19006554883854, 1.25705264815948,
                                      1.28660362854311, 1.29252660746091, 1.10828068985838, 1.14092702655483,
                                      1.10699651175021, 2.94187068438857, 1.28467198207784, 1.50895677146776,
                                      1.90768645836378, 1.23868020066008, 1.07489330959951, 1.06939794630606,
                                      1.10203177013572, 1.0539199296588, 2.85734114790363, 1.43913288254983,
                                      1.46490734725225, 1.20736813087772, 1.44382804725704, 1.47313605468253,
                                      1.20587680755856, 1.19552655266886, 1.31856547687305, 1.09336783680805,
                                      1.07789272072855, 1.0512606013123, 3.09436098481211, 1.29493828442333,
                                      1.51678742733198, 1.38085948291222, 1.14572804863161, 1.31702962994971,
                                      1.16950619657358, 1.23832067192214, 1.26234965892815, 1.34685359769664,
                                      1.28655153263839, 1.22134321596921, 1.11616880482532, 1.18732074589262,
                                      1.10842693813165, 1.06522183440618, 1.08483317367143, 1.08088008638811,
                                      1.51104181002592, 1.15342299789948, 1.04957057027918, 1.15696761054793,
                                      1.2675952880826, 1.05168665908233, 1.1049018997127, 1.07290405278368,
                                      1.05100684598187, 1.13326349160095, 1.38432569738907, 1.08810784244759,
                                      2.90080871651219, 1.26206596958679, 1.48363026458014, 1.50221014118055,
                                      1.31728331555618, 1.07331443869702, 2.75888140295999, 1.3534124205685,
                                      1.38288872718842, 1.82705195634521, 1.3647284709632, 1.1552090727675,
                                      1.11881092800692, 2.55053471400544, 1.6136146551887, 1.34500509243395,
                                      1.39356773279469, 1.16607159810888, 1.32802579172637, 1.19220536744814,
                                      1.0511677538147, 3.30573720527091, 1.55192053237592, 1.38067673226621,
                                      1.22166441641822, 1.31055599385712, 1.09555302916148, 3.19802317095087,
                                      1.35322177884457, 1.3844309027, 1.44203585801797, 1.38997094707178,
                                      1.1023140906677, 1.03868603877826, 1.04408470200993, 1.09270165474411,
                                      1.19606926592105, 1.10160172845778, 1.15380792218228, 1.25886969829674,
                                      1.25866322151028, 1.21922385956191, 1.2827925584105, 1.43165240731797,
                                      1.18998393224239, 1.36616595659792, 1.36208796264167, 1.38793399913726,
                                      1.27675045595447, 1.32394028157859, 1.48254605101133, 1.35189729193351,
                                      1.3881094822538, 1.32334002031099, 1.49287282973944, 1.35439055377107,
                                      1.36628034741202, 1.25767168003114, 1.55092009107822, 1.27515258437028,
                                      1.25158769389308, 1.37713777643215, 1.38346502565791, 1.21244563049645,
                                      1.284146384616, 1.25394815155085, 1.36805623383137, 1.81559019728378,
                                      1.29785195286776, 1.48515658631941, 1.30834888046082, 1.32193176021415,
                                      1.33985591693837, 1.29086794564026, 1.27437959237087, 1.18251592831036,
                                      1.62482499133893, 1.42904921911939, 1.43789138856104, 1.37847035464212,
                                      1.34182970209514, 1.69719412316126, 1.35179958579352, 1.25816141347782,
                                      1.37554955337549, 1.32945969778538, 1.19560612082843, 1.42339097417007,
                                      1.2281361586668, 1.39568640044703, 1.23128136338309, 1.4503059524819,
                                      1.46681579090016, 1.39762166269919, 1.31553147679002, 1.1860621179537,
                                      1.26829682417083, 1.3735644259082, 1.22553915109278, 1.2414413706455,
                                      1.23105825199445, 1.1031115847196, 1.27442156910056, 1.3674344520969,
                                      1.38446915523767, 1.43506588306114, 1.41910706954753, 2.0108871074252,
                                      2.28594479771542, 2.13702761354756, 2.18799499633276, 2.23916806799368,
                                      2.07198410207217, 2.06699497078891, 2.17758909005985, 2.04904362769757,
                                      2.10510647121647, 1.93067229003388, 2.06231597599611, 2.05154661882795,
                                      1.98022679161955, 1.96261524906825, 2.01953433422097, 1.79673794762765,
                                      1.9230430434067, 2.13158430291068, 2.03234000670718, 2.15847736532252,
                                      2.0500108125448, 2.00486741243553, 2.04396002445677, 2.02215342483968,
                                      1.8339331866546, 2.04792166295676, 2.04689720374724, 2.05064413429735,
                                      1.99899323277094, 2.05106380877564, 2.06231198754379, 1.78799081546229,
                                      1.69657536244431, 1.91616285654732, 2.04665906509682, 2.02058010054974,
                                      2.05238859129827, 2.07663939012899, 2.02002845708707, 2.00391102394294,
                                      2.14281590967017, 1.81283030295545, 2.04291979291618, 1.47567960794009,
                                      1.28337804336611, 1.41338348946704, 1.44552928226702, 1.24757221012934,
                                      1.28652473103325, 1.29391921682854, 1.5192906010174, 1.41970363865554,
                                      1.19822298639236, 1.34449913522956, 1.38299197309237, 1.62438103918734,
                                      1.41257748686668, 1.43528016395946, 1.12520740504522, 1.34835817052853,
                                      1.30812137105687, 1.38698480767041, 1.36062200942826, 1.20988137735617,
                                      1.30336502572444, 1.3196741174719, 1.47060814458329, 1.3703956456987,
                                      1.20623965678944, 1.40347875286011, 1.37664616186575, 1.13383892518107,
                                      1.17864418630897, 2.11510725687292, 2.22373264576153, 2.09306598991348,
                                      2.09182333194874, 2.15457821043939, 2.1686305367715, 2.22979706172248,
                                      2.08140169398475, 2.06345198075492, 2.11688468147003, 2.43985480490167,
                                      2.10219547425027, 2.22444970794609, 1.06965377473616, 1.0367684243899,
                                      1.03690826132728, 1.04608870626862, 1.0403362583829, 1.04281206418475,
                                      1.27139638194886, 1.84098508385786, 1.94363804645049, 1.15187546865088,
                                      1.29645205115802, 1.39131016943034, 1.29899245061582, 1.26773690991046,
                                      1.38094166641315, 1.23706004095886, 2.07083085966314, 2.03286646566809,
                                      1.18658419480985, 1.41515106115983, 1.69360272538558, 0.236641221374046,
                                      -0.427480916030534, 0.259541984732824, 21.7251908396947, 33.4961832061069,
                                      65.2977099236641, 28.6946564885496, 42.1603053435115, 236.038167938931,
                                      47.1145038167939, 24.9389312977099, 17.5572519083969, 17.6259541984733,
                                      6.09160305343511, -0.763358778625954, 8.45038167938931, 6.0, 10.412213740458,
                                      24.1984732824427, 6.41984732824428, 37.8549618320611, -12.6717557251908,
                                      85.1297709923664, 43.5114503816794, 18.4427480916031, 250.129770992366,
                                      137.198473282443, 79.1297709923664, 94.557251908397, 54.1526717557252,
                                      93.5267175572519, 33.3664122137405, 49.9160305343511, 48.7480916030534,
                                      72.5267175572519, 120.045801526718, 24.7328244274809, 20.7786259541985,
                                      66.5267175572519, 54.4198473282443, 86.2137404580153, 3.69465648854962,
                                      3.48854961832061, 107.045801526718, 250.129770992366, 5.98473282442748,
                                      4.86259541984733, 28.7022900763359, 36.0152671755725, 9.45801526717557,
                                      250.129770992366, 120.770992366412, 69.7862595419847, 24.5190839694656,
                                      53.9389312977099, 44.7557251908397, 47.0916030534351, 119.656488549618,
                                      26.5648854961832, 127.572519083969, 250.129770992366, 10.175572519084,
                                      -1.12977099236641, 103.465648854962, 13.7175572519084, 13.3969465648855,
                                      27.8778625954199, 59.9923664122137, 55.4809160305344, 102.473282442748,
                                      91.0076335877863, 60.824427480916, 5.16793893129771, 8.0763358778626,
                                      13.1450381679389, 38.1145038167939, 33.7786259541985, 20.2442748091603,
                                      71.1603053435115, 36.5954198473282, 142.625954198473, 175.603053435115,
                                      128.595419847328, 117.916030534351, 16.793893129771, 9.35114503816794,
                                      22.0839694656489, 15.6946564885496, 56.4351145038168, 21.9160305343511,
                                      14.2213740458015, 11.5954198473282, 58.9923664122137, 183.801526717557,
                                      141.450381679389, 36.1984732824428, 39.587786259542, 49.9541984732824,
                                      35.175572519084, 23.1450381679389, 25.824427480916, 36.8702290076336,
                                      13.7251908396947, 10.8931297709924, 49.0458015267176, 31.0, 2.3587786259542,
                                      19.5725190839695, 18.9465648854962, 33.2595419847328, 3.06870229007634,
                                      65.6641221374046, 23.9770992366412, 40.2671755725191, 9.16030534351145,
                                      2.9236641221374, 52.5267175572519, 21.6106870229008, 20.4351145038168,
                                      71.9389312977099, 83.2824427480916, 41.030534351145, 15.5954198473282,
                                      21.8167938931298, 63.4045801526718, 58.6106870229008, 205.145038167939,
                                      9.79389312977099, 75.4961832061069, 57.587786259542, 217.916030534351,
                                      100.389312977099, 29.1297709923664, 6.84732824427481, 21.8473282442748,
                                      58.3282442748092, 38.0687022900763, 11.2290076335878, 28.4580152671756,
                                      14.0458015267176, 36.9465648854962, 187.702290076336, 12.7862595419847,
                                      21.1297709923664, 38.5954198473282, 44.824427480916, 83.2900763358779,
                                      47.2519083969466, 43.030534351145, 23.1145038167939, 45.6564885496183,
                                      108.030534351145, 33.2213740458015, 19.3435114503817, 69.6870229007634,
                                      20.4274809160305, 110.381679389313, 181.290076335878, 10.9541984732824,
                                      3.96946564885496, -0.580152671755725, 21.1526717557252, 12.1603053435115,
                                      6.14503816793893, 9.43511450381679, -1.46564885496183, 56.7557251908397,
                                      86.8549618320611, 38.9236641221374, 36.6946564885496, 24.0534351145038,
                                      16.8167938931298, 68.6259541984733, 29.2824427480916, 21.0152671755725,
                                      30.0992366412214, 6.77862595419847, 22.7328244274809, 13.8625954198473,
                                      34.0916030534351, 15.9465648854962, 24.0229007633588, 15.3893129770992,
                                      15.3129770992366, 3.25954198473282, -0.442748091603053, 0.633587786259542,
                                      0.778625954198473, 0.129770992366412, 2.12977099236641, 6.58015267175573,
                                      2.46564885496183, 1.3587786259542, 1.41984732824427, 12.4580152671756,
                                      31.7022900763359, 14.7862595419847, 12.6030534351145, 5.06106870229008,
                                      7.50381679389313, 22.0152671755725, 32.4961832061069, 20.4045801526718,
                                      23.0763358778626, 22.7786259541985, 15.7251908396947, 22.5038167938931,
                                      68.4885496183206, 10.4198473282443, 20.4198473282443, 39.6488549618321,
                                      250.129770992366, 23.2290076335878, 32.3740458015267, 100.541984732824,
                                      45.8702290076336, 21.6030534351145, 17.7480916030534, 76.4580152671756,
                                      29.8702290076336, 15.412213740458, 83.6335877862595, 5.70992366412214,
                                      46.5343511450382, 81.0763358778626, 66.9541984732825, 31.0534351145038,
                                      25.9160305343511, 29.4885496183206, 26.618320610687, 33.2748091603053,
                                      32.5267175572519, 51.4656488549618, 21.175572519084, 250.129770992366,
                                      3.87786259541985, 83.7404580152672, 113.503816793893, 35.1374045801527,
                                      35.3129770992366, -0.923664122137405, 33.6793893129771, 50.4732824427481,
                                      250.129770992366, 131.740458015267, 151.358778625954, 33.4351145038168,
                                      58.4198473282443, 66.3587786259542, 11.6793893129771, 15.9236641221374,
                                      40.3587786259542, 32.6412213740458, 37.2595419847328, 2.19083969465649,
                                      158.030534351145, -0.740458015267176, 127.946564885496, 52.9541984732824,
                                      15.6793893129771, 37.1145038167939, 42.6793893129771, 21.5801526717557,
                                      19.3358778625954, 48.5038167938931, 14.8931297709924, 29.8549618320611,
                                      66.7862595419847, 10.1984732824427, 2.04580152671756, 16.381679389313,
                                      20.0229007633588, 11.763358778626, 14.7480916030534, 53.8625954198473,
                                      8.11450381679389, 11.3129770992366, 61.9083969465649, 6.86259541984733,
                                      7.56488549618321, 6.09923664122137, 4.74809160305344, 5.0381679389313,
                                      49.5954198473282, -0.358778625954198, 50.9160305343511, 25.1450381679389,
                                      160.007633587786, 147.503816793893, 68.1450381679389, 11.7709923664122,
                                      229.664122137405, 52.5954198473282, 43.5038167938931, 168.687022900763,
                                      151.946564885496, 25.1068702290076, 21.3587786259542, 250.129770992366,
                                      66.8625954198473, 110.458015267176, 110.412213740458, 79.7862595419847,
                                      54.4656488549618, 34.4198473282443, -15.4961832061069, 250.129770992366,
                                      61.9312977099237, 212.656488549618, 137.923664122137, 48.1603053435115,
                                      0.984732824427481, 58.5496183206107, 34.0229007633588, 139.763358778626,
                                      141.847328244275, 46.6030534351145, 4.42748091603053, 4.74809160305344,
                                      -0.213740458015267, 6.41221374045802, 26.7404580152672, 17.1068702290076,
                                      27.381679389313, 37.6412213740458, 42.3282442748092, 35.6564885496183,
                                      47.412213740458, 38.618320610687, 32.9923664122137, 52.7099236641221,
                                      48.3664122137405, 20.1526717557252, 32.9312977099237, 36.0458015267176,
                                      50.4427480916031, 26.9389312977099, 41.1068702290076, 40.5725190839695,
                                      48.8015267175573, 15.030534351145, 37.9465648854962, 29.5496183206107,
                                      39.5648854961832, 15.8931297709924, 12.1068702290076, 22.5419847328244,
                                      29.3358778625954, 27.7557251908397, 43.2213740458015, 44.1908396946565,
                                      26.1603053435115, 43.5038167938931, 31.7480916030534, 29.5343511450382,
                                      70.3206106870229, 59.2824427480916, 30.8091603053435, 88.7862595419847,
                                      38.7557251908397, 35.3282442748092, 33.9236641221374, 29.8091603053435,
                                      25.824427480916, 39.1832061068702, 55.2595419847328, 25.4809160305344,
                                      31.8396946564886, 39.8396946564886, 41.1450381679389, 23.4656488549618,
                                      25.4045801526718, 42.6030534351145, 57.030534351145, 25.9618320610687,
                                      32.7786259541985, 34.9847328244275, 36.8931297709924, 40.6564885496183,
                                      21.9847328244275, 32.7099236641221, 31.2900763358779, 28.7557251908397,
                                      21.7786259541985, 17.206106870229, 32.6106870229008, 25.8015267175573,
                                      42.2290076335878, 29.5343511450382, 28.0687022900763, 27.5954198473282,
                                      35.6030534351145, 18.1526717557252, 85.3129770992367, 85.0229007633588,
                                      27.4503816793893, 60.381679389313, 24.6412213740458, 102.274809160305,
                                      44.4274809160305, 37.3282442748092, 57.8473282442748, 45.6335877862595,
                                      21.9312977099237, 74.7099236641221, 67.9007633587786, 28.763358778626,
                                      40.7099236641221, 38.8320610687023, 25.8015267175573, 84.824427480916,
                                      50.2748091603053, 16.7786259541985, 88.4274809160305, 40.4580152671756,
                                      41.236641221374, 89.5038167938931, 67.7251908396947, 23.824427480916,
                                      58.0534351145038, 34.5801526717557, 30.4809160305344, 45.9312977099237,
                                      45.4809160305344, 36.0229007633588, 56.1145038167939, 51.5190839694657,
                                      46.587786259542, 78.3358778625954, 38.3893129770992, 22.5419847328244,
                                      73.2977099236641, 46.2824427480916, 46.6564885496183, 75.3435114503817,
                                      31.3358778625954, 17.2900763358779, 102.81679389313, 90.4427480916031,
                                      77.2290076335878, 56.9770992366412, 37.4809160305344, 70.8778625954198,
                                      41.6717557251908, 43.4427480916031, 60.4732824427481, 42.8091603053435, 63.0,
                                      69.4045801526718, 50.5648854961832, 75.3358778625954, 12.5190839694657,
                                      48.236641221374, 76.5954198473282, 31.0229007633588, 71.4732824427481,
                                      15.5725190839695, 29.3282442748092, 72.4885496183206, 41.7480916030534,
                                      64.0839694656489, 81.3740458015267, 48.8778625954199, 76.3664122137405,
                                      83.3435114503817, 36.0687022900763, 76.6106870229008, 51.2290076335878,
                                      63.3053435114504, 53.1068702290076, 60.8396946564886, 128.229007633588,
                                      76.7786259541985, 66.3664122137405, 81.0458015267176, 85.8854961832061,
                                      70.2977099236641, 79.0, 56.3129770992366, 4.82442748091603, 5.20610687022901,
                                      0.396946564885496, 1.0763358778626, 3.41221374045802, 1.56488549618321,
                                      35.2748091603053, 37.4503816793893, 183.870229007634, 27.4274809160305,
                                      -0.66412213740458, 110.114503816794, 88.2595419847328, 36.4809160305344,
                                      57.6793893129771, 41.8625954198473, 28.9389312977099, 143.053435114504,
                                      9.47328244274809, 90.030534351145, 98.1145038167939, 1.99236641221374,
                                      1.3206106870229, 24.5114503816794, 132.763358778626, 25.0534351145038,
                                      31.3587786259542, 50.0839694656489, 14.9541984732824, 53.6259541984733,
                                      5.9618320610687, 5.33587786259542, 5.12213740458015, 9.03053435114504,
                                      5.04580152671756, 2.25190839694656, 3.19847328244275, 9.49618320610687,
                                      21.2748091603053, 9.90076335877863, 3.06106870229008, 1.42748091603053,
                                      11.7404580152672, 24.6641221374046, 32.412213740458, 104.450381679389,
                                      39.6870229007634, 78.0534351145038, 74.3511450381679, 66.1908396946565,
                                      112.740458015267, 47.8702290076336, 32.9389312977099, 15.618320610687,
                                      -19.7862595419847, 57.9847328244275, 98.8320610687023, 12.1450381679389,
                                      3.51145038167939, 129.916030534351, 70.2290076335878, 29.8931297709924,
                                      44.6717557251908, 83.9160305343512, 12.4961832061069, 93.8167938931298,
                                      3.20610687022901, 3.25190839694656, 12.7022900763359, 9.80916030534351,
                                      10.2137404580153, 57.0229007633588, 136.206106870229, 59.6259541984733,
                                      130.778625954198, -34.7099236641221, 157.648854961832, 42.3511450381679,
                                      23.6946564885496, 7.90076335877863, 28.6946564885496, 94.3587786259542,
                                      29.412213740458, 74.4427480916031, 64.1679389312977, 7.48091603053435,
                                      16.9236641221374, 25.9923664122137, 17.1374045801527, 13.8473282442748,
                                      120.480916030534, 139.374045801527, 8.85496183206107, 9.08396946564885, 43.0,
                                      21.0152671755725, 50.763358778626, 114.625954198473, 6.7175572519084,
                                      49.7099236641221, 14.9236641221374, 80.8931297709924, 127.267175572519,
                                      113.099236641221, 8.03053435114504, 19.969465648855, 8.25190839694657,
                                      16.8320610687023, 11.6717557251908, -21.4198473282443, 14.3282442748092,
                                      16.1908396946565, 52.8396946564886, 0.236641221374046, 113.526717557252,
                                      130.732824427481, 21.9770992366412, 23.8931297709924, 13.9007633587786,
                                      37.1526717557252, 9.25190839694657, 56.1297709923664, 66.2519083969466,
                                      15.9389312977099, 0.0687022900763359, 69.8091603053435, 14.763358778626,
                                      7.93893129770992, 52.1221374045802, 176.450381679389, 68.2213740458015,
                                      19.2671755725191, 48.7251908396947, 17.7099236641221, 19.587786259542,
                                      15.3740458015267, 84.763358778626, 13.4503816793893, 24.4198473282443,
                                      152.496183206107, 131.69465648855, 16.5267175572519, 35.1297709923664,
                                      5.87022900763359, 180.557251908397, 13.1068702290076, 8.64885496183206,
                                      54.1297709923664, 12.1832061068702, 49.1908396946565, 164.778625954198,
                                      5.92366412213741, 41.8702290076336, 102.81679389313, 20.8473282442748,
                                      13.7786259541985, 9.48854961832061, 14.4885496183206, 146.618320610687,
                                      18.7480916030534, 23.9236641221374, 11.8778625954198, 250.129770992366,
                                      13.0839694656489, 40.9847328244275, 16.6106870229008, 53.9770992366412,
                                      66.0152671755725, 124.198473282443, 83.2671755725191, 72.3969465648855,
                                      8.31297709923664, 51.8473282442748, 171.137404580153, 44.4045801526718,
                                      36.824427480916, 4.11450381679389, 22.2671755725191, 92.3053435114504,
                                      2.80152671755725, 3.29770992366412, 2.50381679389313, 7.36641221374046,
                                      11.0916030534351, 16.9618320610687, 5.61068702290076, 7.0381679389313,
                                      20.0229007633588, 19.8396946564886, 133.160305343511, 75.5648854961832,
                                      55.4961832061069, 37.5648854961832, 23.6793893129771, 110.664122137405,
                                      141.290076335878, 9.03053435114504, 10.793893129771, 5.3206106870229,
                                      31.7175572519084, 28.7786259541985, 13.7557251908397, 11.8091603053435,
                                      9.97709923664122, 43.8625954198473, 69.3206106870229, -0.16793893129771,
                                      3.44274809160305, 0.351145038167939, 2.53435114503817, 1.78625954198473,
                                      2.33587786259542, 0.946564885496183, 1.64885496183206, 1.52671755725191,
                                      3.52671755725191, 24.1068702290076, -1.46564885496183, 35.1908396946565,
                                      43.7328244274809, 24.0992366412214, 41.0839694656489, 4.47328244274809,
                                      34.9083969465649, 46.7862595419847, 50.0152671755725, 0.221374045801527,
                                      71.793893129771, 18.3587786259542, 17.7175572519084, 10.8854961832061,
                                      15.0763358778626, 250.129770992366, 49.1068702290076, 126.137404580153,
                                      43.1450381679389, 31.6335877862595, 14.7022900763359, 10.0992366412214,
                                      40.7022900763359, 24.8015267175573, 9.51908396946565, 4.72519083969466,
                                      98.0839694656489, 119.618320610687, 58.5190839694657, 58.8854961832061,
                                      50.8015267175573, 57.5801526717557, 53.3435114503817, 13.7328244274809,
                                      15.3664122137405, 25.175572519084, 30.0381679389313, 16.9923664122137,
                                      250.129770992366, 63.4580152671756, 76.5954198473282, 36.381679389313,
                                      38.6030534351145, 22.0916030534351, 2.54198473282443, 21.2519083969466,
                                      9.09923664122137, 132.572519083969, 131.824427480916, 72.6488549618321,
                                      106.152671755725, 16.1374045801527, 54.1679389312977, 20.8702290076336,
                                      18.4503816793893, 27.7175572519084, 26.206106870229, 30.0992366412214,
                                      5.87022900763359, 250.129770992366, 112.427480916031, 94.206106870229,
                                      68.6259541984733, 16.412213740458, 42.1145038167939, 73.1450381679389,
                                      26.8015267175573, -3.22137404580153, 116.763358778626, 25.030534351145,
                                      26.6717557251908, 8.68702290076336, 8.40458015267176, 3.34351145038168,
                                      21.3358778625954, 21.2900763358779, 63.381679389313, 21.3358778625954,
                                      47.4809160305344, 1.45038167938931, 37.9083969465649, 62.412213740458,
                                      1.13740458015267, 3.70229007633588, 71.381679389313, 65.4580152671756,
                                      11.6412213740458, 12.3435114503817, 6.37404580152672, 112.625954198473,
                                      70.557251908397, 173.839694656489, 52.1679389312977, 21.6259541984733,
                                      8.04580152671756, 70.0763358778626, 97.7175572519084, 152.923664122137,
                                      17.8473282442748, 30.3053435114504, 15.6793893129771, 8.57251908396947,
                                      242.114503816794, 36.6564885496183, 44.2748091603053, 23.9083969465649,
                                      20.2290076335878, 19.2748091603053, 19.8167938931298, 4.74809160305344,
                                      74.2137404580153, 29.8167938931298, 20.8778625954199, 55.6793893129771,
                                      18.8702290076336, 6.43511450381679, 45.6717557251908, 65.0763358778626,
                                      107.427480916031, 13.2748091603053, 39.4732824427481, 7.58015267175573,
                                      6.70992366412214, 3.34351145038168, 3.41221374045802, 10.6335877862595,
                                      56.3206106870229, 90.5267175572519, 75.6564885496183, 64.8473282442748,
                                      36.4503816793893, 27.6488549618321, 35.6412213740458, 82.1297709923664,
                                      35.8091603053435, 28.5343511450382, 37.236641221374, 24.2977099236641,
                                      43.5343511450382, 35.2137404580153, 10.7709923664122, 26.9007633587786,
                                      24.8167938931298, 40.6335877862595, 26.5725190839695, 33.381679389313,
                                      38.9618320610687, 27.5572519083969, 35.5114503816794, 31.7175572519084,
                                      13.7251908396947, 14.5114503816794, 23.6030534351145, 22.0916030534351,
                                      20.236641221374, 28.9312977099237, 30.0534351145038, 28.5419847328244,
                                      30.8320610687023, 22.8396946564886, 29.2137404580153, 21.030534351145,
                                      38.2595419847328, 34.206106870229, 21.5572519083969, 28.6335877862595,
                                      27.2290076335878, 24.0610687022901, 39.9389312977099, 20.0152671755725,
                                      22.5419847328244, 48.0458015267176, 53.1526717557252, 32.8625954198473,
                                      40.2519083969466, 126.480916030534, 21.3969465648855, 37.6564885496183,
                                      28.3893129770992, 38.1450381679389, 22.7786259541985, 30.6870229007634,
                                      10.9923664122137, 54.7480916030534, 35.7022900763359, 33.0076335877863,
                                      17.2290076335878, 32.9847328244275, 13.8931297709924, 19.9312977099237,
                                      42.206106870229, 30.1679389312977, 41.7022900763359, 34.381679389313,
                                      41.1374045801527, 28.175572519084, 32.557251908397, 57.0839694656489,
                                      62.9236641221374, 49.7022900763359, 70.793893129771, 55.6717557251908,
                                      49.1908396946565, 49.5190839694657, 40.4045801526718, 62.8854961832061,
                                      46.4961832061069, 78.6259541984733, 66.3893129770992, 83.4732824427481,
                                      47.1603053435115, 54.6259541984733, 80.3664122137405, 56.6106870229008,
                                      52.7557251908397, 44.5114503816794, 84.3282442748092, 40.6488549618321,
                                      60.7786259541985, 59.9770992366412, 54.8473282442748, 65.9465648854962,
                                      59.3358778625954, 58.1221374045802, 64.1526717557252, 49.8091603053435,
                                      64.1450381679389, 56.4045801526718, 61.3358778625954, 43.5190839694657,
                                      96.7175572519084, 55.9465648854962, 64.1068702290076, 42.8625954198473,
                                      73.5496183206107, 66.9389312977099, 62.7862595419847, 53.5954198473282,
                                      74.3206106870229, 67.6946564885496, 80.8854961832061, 32.0916030534351,
                                      37.4427480916031, 9.93893129770992, 9.0763358778626, 17.6030534351145,
                                      19.2671755725191, 18.0152671755725, 11.175572519084, 7.06870229007634,
                                      12.6030534351145, 7.06870229007634, 9.24427480916031, 52.4656488549618,
                                      7.11450381679389, 6.77862595419847, 18.4351145038168, 10.7175572519084,
                                      14.0458015267176, 11.8854961832061, 8.77099236641221, 9.25954198473282,
                                      7.25190839694657, 12.2213740458015, 14.0687022900763, 10.3435114503817,
                                      21.8167938931298, 15.0076335877863, 9.96946564885496, 4.49618320610687,
                                      53.7175572519084, 30.9007633587786, 15.3587786259542, 23.3358778625954,
                                      21.0076335877863, 22.175572519084, 30.6717557251908, 33.3053435114504,
                                      17.0839694656489, 28.3511450381679, 32.2977099236641, 21.5267175572519,
                                      7.66412213740458, 3.68702290076336, 3.22137404580153, 3.10687022900763,
                                      2.65648854961832, 5.41221374045802, 12.7328244274809, 23.8854961832061,
                                      9.22137404580153, 39.6412213740458, 31.2137404580153, 125.923664122137,
                                      133.717557251908, 76.6793893129771, 38.8854961832061, 98.0763358778626,
                                      20.3893129770992, 15.2900763358779, 23.5038167938931, 59.2748091603053,
                                      125.832061068702, 114.412213740458, 1.25190839694657, 0.610687022900763,
                                      1.02290076335878, 5.09923664122137, 15.4580152671756, 13.3587786259542,
                                      25.1908396946565, 7.97709923664122, 19.969465648855, 5.97709923664122,
                                      4.70229007633588, 8.22900763358779, 9.80916030534351, 5.32824427480916,
                                      3.08396946564885, 5.93129770992367, 3.00763358778626, 5.03053435114504,
                                      5.97709923664122, 11.2290076335878, 9.11450381679389, 8.12977099236641,
                                      26.4961832061069, 20.7099236641221, 19.0152671755725, 50.4961832061069,
                                      34.9236641221374, 20.6488549618321, 18.1603053435115, 55.824427480916,
                                      17.6106870229008, 25.1526717557252, 39.3358778625954, 15.5954198473282,
                                      37.9541984732824, 19.4198473282443, 19.5725190839695, 5.27480916030534,
                                      45.7175572519084, 18.3511450381679, 15.236641221374, 3.89312977099237,
                                      4.49618320610687, 11.0229007633588, 106.870229007634, 3.45801526717557,
                                      29.9083969465649, 30.7022900763359, 9.74045801526718, 12.9770992366412,
                                      44.3587786259542, 23.7175572519084, 32.763358778626, 37.1145038167939,
                                      25.0610687022901, 27.4351145038168, 31.0534351145038, 20.5114503816794,
                                      6.80152671755725, 4.64885496183206, 153.36641221374, 10.0839694656489,
                                      4.33587786259542, 49.1603053435115, 22.4427480916031, 18.2213740458015,
                                      17.8320610687023, 40.0687022900763, 30.4809160305344, 58.1374045801527,
                                      46.8167938931298, 39.6870229007634, 7.6412213740458, 11.9465648854962,
                                      12.5190839694657, 18.0076335877863, 43.6946564885496, 7.53435114503817,
                                      14.8396946564886, 8.86259541984733, 35.8473282442748, 44.4503816793893,
                                      28.9160305343511, 30.6106870229008, 12.1832061068702, 5.34351145038168,
                                      11.5954198473282, 7.58778625954199, 7.69465648854962, 14.0152671755725,
                                      26.0076335877863, 5.78625954198473, 17.9236641221374, 63.8931297709924,
                                      45.1374045801527, 11.6793893129771, 24.6641221374046, 16.8702290076336,
                                      19.2137404580153, 7.38931297709924, 16.9312977099237, 26.9160305343511,
                                      23.0458015267176, 22.9007633587786, 49.6412213740458, 16.6488549618321,
                                      6.52671755725191, 16.8320610687023, 16.1145038167939, 13.824427480916,
                                      4.54198473282443, 8.38931297709924, 5.76335877862595, 8.76335877862596,
                                      6.32824427480916, 12.7709923664122, 19.8396946564886, 21.1297709923664,
                                      28.4809160305344, 30.2824427480916, 22.8015267175573, 10.1145038167939,
                                      1.80916030534351, 15.9923664122137, 17.0458015267176, 13.9847328244275,
                                      51.3206106870229, 7.38931297709924, 25.2671755725191, 24.8702290076336,
                                      40.3511450381679, 22.2900763358779, 51.3664122137405, 15.7022900763359,
                                      8.16030534351145, 23.3435114503817, 6.38167938931298, 12.5267175572519,
                                      11.9160305343511, 8.03053435114504, 4.09923664122137, 39.5801526717557,
                                      18.3435114503817, 39.0610687022901, 5.30534351145038, 14.8778625954198,
                                      37.4580152671756, 9.24427480916031, 34.3358778625954, 23.3511450381679,
                                      33.9312977099237, 16.1679389312977, 7.19083969465649, 12.412213740458,
                                      16.587786259542, 2.90076335877863, 21.9312977099237, 70.2213740458015,
                                      2.12213740458015, 2.33587786259542, 1.38931297709924, 9.78625954198473,
                                      14.6030534351145, 9.82442748091603, 1.75572519083969, 0.916030534351145,
                                      18.2595419847328, 31.2595419847328, 11.0152671755725, -6.19847328244275,
                                      7.59541984732825, 9.02290076335878, 9.0763358778626, 36.8167938931298,
                                      24.0992366412214, 16.0839694656489, 12.7480916030534, 11.8931297709924,
                                      15.206106870229, 10.8931297709924, 10.3511450381679, 10.587786259542,
                                      21.9465648854962, 60.0916030534351, 4.02290076335878, 1.21374045801527,
                                      1.06106870229008, 1.58778625954198, 1.45801526717557, 1.05343511450382,
                                      1.13740458015267, 1.56488549618321, 2.19083969465649, 1.22137404580153,
                                      10.4274809160305, 17.2595419847328, 8.6030534351145, 26.9236641221374,
                                      17.206106870229, 13.2519083969466, 54.618320610687, 2.3969465648855,
                                      12.4427480916031, 39.5954198473282, 16.0152671755725, 24.175572519084,
                                      19.3664122137405, 24.5190839694656, 29.8625954198473, 7.40458015267176,
                                      15.3053435114504, 250.129770992366, 8.03053435114504, 80.5648854961832,
                                      35.4580152671756, 60.1450381679389, 16.6412213740458, 34.5496183206107,
                                      9.80152671755725, 14.6946564885496, 9.36641221374046, 128.458015267176,
                                      250.129770992366, 96.9923664122137, 38.8473282442748, 51.7022900763359,
                                      62.9618320610687, 20.6793893129771, 20.5190839694656, 57.2519083969466,
                                      7.29007633587786, 15.7099236641221, 50.8854961832061, 25.793893129771,
                                      250.129770992366, 58.3358778625954, 80.557251908397, 30.1832061068702,
                                      40.9923664122137, 18.9007633587786, 4.48091603053435, 16.0610687022901,
                                      143.30534351145, 250.129770992366, 68.3282442748092, 6.22137404580153,
                                      49.3511450381679, 22.4045801526718, 103.81679389313, 57.2671755725191,
                                      37.3893129770992, 32.030534351145, 13.9923664122137, 10.0152671755725,
                                      25.0763358778626, 250.129770992366, 73.4809160305344, 32.7786259541985,
                                      40.7709923664122, 8.6412213740458, 26.3129770992366, 59.0229007633588,
                                      11.0763358778626, 27.5954198473282, 25.0534351145038, 32.2671755725191,
                                      25.7251908396947, 10.0763358778626, 5.75572519083969, 2.82442748091603,
                                      31.8702290076336, 4.49618320610687, 4.25190839694657, 10.2213740458015,
                                      17.4274809160305, 3.08396946564885, 9.44274809160305, 26.2748091603053,
                                      6.75572519083969, 5.30534351145038, 5.55725190839695, 4.70992366412214,
                                      5.86259541984733, 3.55725190839695, 2.06106870229008, 126.175572519084,
                                      7.6793893129771, 34.6717557251908, 42.6870229007634, 29.4580152671756,
                                      7.48854961832061, 29.8396946564886, 33.175572519084, 92.3740458015267,
                                      54.030534351145, 30.9770992366412, 14.206106870229, 5.15267175572519,
                                      69.3969465648855, 15.9083969465649, 40.0, 21.9389312977099, 11.763358778626,
                                      15.5114503816794, 28.4580152671756, 0.633587786259542, 23.1603053435115,
                                      17.6335877862595, 48.0610687022901, 33.1145038167939, 13.2442748091603,
                                      8.58778625954199, 19.7251908396947, 35.5190839694657, 64.3511450381679, 17.0,
                                      23.824427480916, 7.09160305343511, 2.00763358778626, 1.45038167938931,
                                      2.0763358778626, 9.08396946564885, 9.77099236641221, 18.8320610687023,
                                      33.0534351145038, 22.5572519083969, 21.5190839694656, 23.1908396946565,
                                      20.1526717557252, 27.6946564885496, 37.0076335877863, 24.587786259542,
                                      24.7328244274809, 36.8702290076336, 42.5267175572519, 34.7786259541985,
                                      40.3893129770992, 28.5343511450382, 49.4045801526718, 44.0534351145038,
                                      28.0916030534351, 29.4503816793893, 37.6641221374046, 31.6717557251908,
                                      36.0534351145038, 28.381679389313, 22.4503816793893, 28.1145038167939,
                                      24.1068702290076, 17.2213740458015, 17.7480916030534, 33.6030534351145,
                                      38.0458015267176, 33.7099236641221, 34.381679389313, 16.1374045801527,
                                      22.5343511450382, 21.4503816793893, 22.3511450381679, 45.2519083969466,
                                      26.9007633587786, 28.618320610687, 19.7786259541985, 41.9847328244275,
                                      26.0076335877863, 38.6106870229008, 35.3969465648855, 45.3053435114504,
                                      31.5190839694656, 29.1450381679389, 15.9236641221374, 12.1450381679389,
                                      18.3587786259542, 34.6412213740458, 25.1068702290076, 37.1908396946565,
                                      28.6259541984733, 26.236641221374, 36.6106870229008, 24.5343511450382,
                                      16.8549618320611, 27.3206106870229, 19.0839694656489, 28.5648854961832,
                                      17.4580152671756, 22.0, 28.618320610687, 25.236641221374, 26.2595419847328,
                                      20.3969465648855, 18.5801526717557, 18.8473282442748, 40.0687022900763,
                                      37.6335877862595, 61.2213740458015, 36.4274809160305, 45.5419847328244,
                                      39.7709923664122, 36.4274809160305, 43.7251908396947, 41.3435114503817,
                                      35.5419847328244, 52.5954198473282, 42.7786259541985, 31.618320610687,
                                      46.1068702290076, 37.9541984732824, 38.3435114503817, 49.6870229007634,
                                      38.0458015267176, 45.4351145038168, 43.5496183206107, 42.9847328244275,
                                      38.6488549618321, 43.8625954198473, 35.3129770992366, 42.6106870229008,
                                      64.2671755725191, 52.0076335877863, 38.8015267175573, 30.236641221374,
                                      27.0229007633588, 37.3129770992366, 45.9618320610687, 37.9770992366412,
                                      28.9236641221374, 38.175572519084, 44.9312977099237, 35.1984732824428,
                                      52.7404580152672, 24.5801526717557, 50.0992366412214, 55.4198473282443,
                                      40.4045801526718, 38.4351145038168, 25.7328244274809, 35.5954198473282,
                                      49.6641221374046, 13.6717557251908, 8.73282442748092, 5.51145038167939,
                                      7.41984732824428, 11.618320610687, 8.53435114503817, 6.20610687022901,
                                      7.86259541984733, 9.51145038167939, 6.38167938931298, 12.9847328244275,
                                      53.7175572519084, 7.31297709923664, 2.80152671755725, 11.236641221374,
                                      9.40458015267176, 12.1068702290076, 9.30534351145038, 10.0152671755725,
                                      11.969465648855, 9.15267175572519, 17.7557251908397, 10.1450381679389,
                                      7.81679389312977, 19.7099236641221, 13.7175572519084, 15.8015267175573,
                                      9.00763358778626, 18.0534351145038, 20.9312977099237, 16.6335877862595, 24.0,
                                      20.236641221374, 31.5725190839695, 15.8854961832061, 19.7328244274809,
                                      26.2442748091603, 25.1526717557252, 24.1679389312977, 29.5190839694656,
                                      9.75572519083969, 1.87786259541985, 2.87786259541985, 2.33587786259542,
                                      2.26717557251908, 2.82442748091603, 2.19083969465649, 29.7404580152672,
                                      13.7175572519084, 169.900763358779, 26.030534351145, 93.6259541984733,
                                      78.0458015267176, 39.0381679389313, 44.5190839694657, 15.1908396946565,
                                      36.0763358778626, 26.2442748091603, 159.854961832061, 12.3206106870229,
                                      95.3053435114504, 53.5267175572519, 4.22778801274176, 5.16900436659065,
                                      25.6290567900808, 140.065801962635, 110.096410594969, 68.6748668763276,
                                      66.0421288854041, 44.073934286246, 259.587407565429, 47.5220618951014,
                                      27.3863523551565, 17.5910941678261, 21.95963765978, 10.8975608441718,
                                      4.70266802288008, 11.5285423963733, 11.2455780923059, 24.546971582668,
                                      24.9281994400635, 13.3100485258465, 39.5488963572531, 138.586386936578,
                                      88.2911006536644, 69.067307256381, 111.427033787809, 258.2437349517,
                                      257.618835087933, 116.747266300188, 99.1591071156795, 173.284074377764,
                                      98.8087560381052, 49.2741480907164, 96.6944443605123, 92.2317401926091,
                                      78.8752104091346, 155.716075358116, 34.1559560538846, 52.7133943172545,
                                      158.083108201153, 147.507909474757, 89.8101922774318, 47.0950769563782,
                                      89.2501292544519, 117.463356474317, 270.221100608862, 13.5577719795716,
                                      30.5521293664178, 64.8149283975932, 36.246776378812, 139.799330282762,
                                      260.337309715949, 163.469770740772, 108.513954158847, 151.710694741335,
                                      146.315410093449, 170.391933978257, 84.9374456610678, 123.692488029368,
                                      28.2126581861752, 130.883102039163, 287.849243517791, 35.4325901663251,
                                      75.4158422925226, 151.826698273032, 38.3327639558461, 68.2315548198262,
                                      42.1858817837814, 65.4097678999752, 149.6346061397, 188.439327386972,
                                      167.128610120949, 124.873840186562, 18.4718613420965, 44.5566535890788,
                                      34.7425814665515, 59.3718498040288, 255.280848322603, 30.2785025444381,
                                      88.0107441751449, 51.1587876015528, 142.641626624411, 204.799740019447,
                                      178.207210131975, 122.412138765998, 51.5617008752106, 16.4073381716012,
                                      36.2514277783157, 23.8023774630187, 134.060374323058, 31.0824181514239,
                                      36.8620935187039, 60.783949194605, 61.4939056902183, 225.285704950318,
                                      197.829894485803, 43.9286978359362, 54.5485896032368, 53.7575685156723,
                                      49.4467197077309, 31.4343646681958, 83.4282052271451, 201.202198810666,
                                      45.1049608227545, 42.6794282250059, 109.7844052683, 45.1259869654901,
                                      13.3038237144036, 54.0765088294268, 180.686190524952, 70.3848664642447,
                                      21.75980810757, 85.6253570248404, 24.3636582747994, 42.5601628416019,
                                      20.2819787516561, 96.7833141930087, 58.4052885325515, 47.5421642958461,
                                      169.092129970393, 156.001090908944, 116.028086052916, 57.7953419909718,
                                      16.7348267314832, 187.065578866273, 109.537656908416, 60.6242336852489,
                                      211.246117063117, 105.29394784908, 115.170546424459, 179.685586104556,
                                      225.317422050421, 146.236765944032, 105.260791175293, 27.2254345719022,
                                      50.4331693860846, 75.8234334095854, 48.2307021443183, 151.053732348868,
                                      106.485349209069, 34.8531704173626, 38.919465347536, 354.047392923751,
                                      21.7246356138114, 66.1434700262969, 108.066362095848, 138.76345725347,
                                      134.101761519929, 138.190786141884, 121.736378371497, 165.998184617166,
                                      149.314148865975, 120.913787665219, 179.384946836585, 109.034156009656,
                                      92.8614109982417, 20.8488054351272, 114.721070741271, 316.805704369167,
                                      11.1646408178591, 6.19584515411936, 12.3835452891365, 23.9813810516066,
                                      18.9412756886386, 19.463640069977, 10.9782617312232, 100.58149937455,
                                      128.078025647681, 145.750437674001, 136.801584647791, 91.5810536559296,
                                      81.8626174863514, 50.7897910865966, 75.1874048891915, 116.746250573202,
                                      142.184234379312, 42.6627699506683, 21.9677693778839, 30.0081093481988,
                                      36.1378996150912, 37.3217598451562, 20.6311250094099, 52.0000610731488,
                                      33.6695578170281, 75.1190112996234, 71.4723024539172, 8.19279232290602,
                                      4.87604644301086, 4.66941641775709, 4.54568455377924, 3.62923114277972,
                                      6.8456557992266, 5.29278540065251, 5.04767204418752, 4.78481052368505,
                                      17.9834448533745, 91.9075888859002, 27.7703870024541, 36.0471610621395,
                                      45.2086532613068, 25.053668865873, 93.8302939373497, 54.9513163180249,
                                      35.6032211968865, 55.2066457152991, 50.545080847025, 108.846892734345,
                                      79.2425463683768, 71.5446110721351, 59.7633153893323, 35.0461216005443,
                                      40.6992591485301, 433.237471844346, 84.6385814526129, 162.843471685584,
                                      119.203105897809, 82.537989586358, 27.2739502880392, 62.7472228353149,
                                      107.267997489262, 56.2807224985408, 18.1993474097151, 154.743907608355,
                                      367.088665819209, 146.086342252824, 116.361547227149, 84.5894936556599,
                                      84.4901310831721, 68.3444122470249, 91.9648187242845, 108.698041129756,
                                      36.7212373772698, 34.5269285224427, 146.477148317796, 67.9905880466804,
                                      365.551691340655, 109.273374420328, 112.905038138624, 128.292491390277,
                                      65.6615707550213, 38.9529381554791, 15.7791060467554, 34.1405401474115,
                                      162.767581506324, 359.033446863151, 144.301402896579, 170.946594371309,
                                      115.821421902248, 125.684924747719, 105.94464313338, 75.9025337347359,
                                      45.0555620441554, 43.4610897793216, 56.0812653835536, 61.2277323419583,
                                      28.3535116804594, 354.109860098607, 154.890074198988, 147.309906899785,
                                      86.0999012528248, 45.80045545821, 66.7878060476078, 89.0174103936109,
                                      52.6953089279111, 60.8564661243582, 117.339288369541, 40.4168081954713,
                                      44.4618541774326, 66.8871087831446, 19.4606983676543, 14.1046518529366,
                                      46.3112325518802, 23.1063507256177, 63.4050222117767, 148.759570335258,
                                      66.5535133678166, 20.036553915368, 39.0021014585956, 110.110808939557,
                                      25.6332661187643, 54.6507961127968, 71.6569902974797, 66.0487686070564,
                                      57.8196531422804, 50.202166312586, 100.372282089956, 284.164884331426,
                                      88.3559559669832, 182.456447312181, 179.43049275602, 72.0713385423551,
                                      65.7056407164314, 232.034020244663, 131.067531460535, 167.433933475663,
                                      176.324758414821, 155.383430173091, 26.4934614976396, 58.1018407887044,
                                      354.965169400711, 75.4116801833153, 120.486387055338, 155.652109966002,
                                      82.1471591727543, 60.8036183708881, 36.3365305605629, 89.3272230433966,
                                      260.983776754265, 84.9867860448081, 218.691700448365, 139.040271413582,
                                      57.0397550124918, 22.5924861413008, 250.182471605302, 107.436484841784,
                                      154.227626486038, 148.730339102666, 55.9114663483863, 14.252717934952,
                                      6.78519900006947, 6.44956406580247, 50.1938698182647, 61.486851820515,
                                      62.0673151603279, 90.6482648433986, 85.7505516040688, 66.3965608472979,
                                      85.2192081826189, 54.3508582418027, 59.5293091812238, 90.4893165800655,
                                      69.5492974259924, 77.0059586145183, 44.2611653102134, 57.1559241201151,
                                      88.2616778053257, 70.8191245799469, 43.2571691360161, 53.9702219841838,
                                      79.6717707188227, 60.2945927222634, 43.7003546356319, 64.1177852030817,
                                      75.7144557307015, 83.2844641199967, 38.19726659513, 52.7103277286612,
                                      58.3840353935487, 59.9139112985304, 45.9718899018768, 59.4520621623484,
                                      59.5785090559682, 113.120711281292, 77.4320178119938, 71.1059183802091,
                                      64.2651173088581, 110.737342865663, 63.7112773173576, 48.5180540967472,
                                      98.2207976836119, 79.2615122978396, 58.7920798521526, 43.4788688781742,
                                      46.7585068497988, 71.146097197504, 90.9922437752132, 74.1799379198366,
                                      68.9350653700537, 75.4428485029915, 68.0832920337852, 46.9096913466836,
                                      136.665015199873, 152.123729770013, 70.5708548678966, 68.9915690954944,
                                      40.8726855404114, 51.4046215286507, 84.9125478615297, 76.855263975362,
                                      47.9607464828613, 59.7407574665292, 39.7501505806539, 54.7896678004769,
                                      58.0485325481592, 42.0668274413547, 48.5496573287087, 53.3308836213615,
                                      64.9849130604215, 80.8089313550738, 56.5007795714871, 62.2555074523241,
                                      60.7928828499643, 43.6152237810306, 55.8429525328888, 91.6671455172171,
                                      113.347597567173, 76.4631340048123, 109.85787763188, 65.4895662060761,
                                      105.848942923505, 80.0613281768931, 87.9331651656019, 79.5900899940853,
                                      63.2099313848267, 86.0738716478798, 89.1011587516149, 85.6119938777099,
                                      76.2650392688079, 78.4320268332282, 84.2272950054672, 71.2717428234893,
                                      91.248828153334, 63.6843515922481, 84.7255966245409, 97.3257850310469,
                                      73.7635562736474, 89.2142538474853, 111.525615096619, 114.861392313032,
                                      88.2613609016716, 82.4424519361375, 75.2974262947922, 66.5696904561738,
                                      80.3936856260363, 72.4721061177464, 71.9396902784001, 67.8584929937498,
                                      115.90298282467, 75.204266635474, 93.4681320447907, 69.2235938790868,
                                      76.4709625375241, 92.8680059166229, 71.5663002404691, 115.824949952612,
                                      87.6706178247032, 78.2307361215625, 118.298789911069, 108.3533115593,
                                      97.4359529748312, 77.8378234804557, 65.2016782201561, 41.653808244551,
                                      72.1982267118093, 72.7193025547302, 43.6024513771594, 60.7756355314344,
                                      64.9961866801984, 63.3467407859866, 94.8963318200578, 72.1786032844881,
                                      75.6629811723548, 63.7719472430599, 48.3906616912772, 77.6519051101002,
                                      75.3563899988893, 71.9153324802386, 40.8882881338972, 44.4717847877955,
                                      73.2841781760162, 68.1833137906255, 66.3897983581059, 82.0981592016773,
                                      74.7523502940399, 78.029278256643, 85.2242297556933, 44.8271248954384,
                                      78.7278469695955, 113.60230299997, 64.2344191421468, 77.6348165064481,
                                      112.413252040453, 132.083911010433, 77.949461348758, 167.162920152816,
                                      85.2274805604966, 89.6596267474526, 103.149978244161, 82.5297113436873,
                                      160.780702869604, 10.5213411199143, 6.02462750622617, 8.84629083222218,
                                      8.51575559484908, 9.28122167038852, 18.7365335360056, 80.809435044811,
                                      228.228236690472, 234.191467967524, 40.4356160088339, 159.638846742253,
                                      172.799227723391, 116.911872009122, 72.5716988721268, 99.1580193543833,
                                      99.4924225652059, 208.28348923383, 269.729156240551, 135.090887874475,
                                      143.583694054368, 115.741142226792, 0.136962890625, 0.139404296875,
                                      0.142822265625, -0.072998046875, -0.02734375, 0.094482421875, -0.02783203125,
                                      0.090576171875, -2.0, -0.147216796875, -0.192626953125, -0.209228515625,
                                      -0.171875, -0.208984375, -0.16845703125, -0.160888671875, -0.180908203125,
                                      -0.17236328125, -0.1904296875, -0.16259765625, -0.15576171875,
                                      -0.091064453125, 0.088134765625, 0.113037109375, 0.04345703125,
                                      0.07958984375, -0.24853515625, -0.08984375, 0.169921875, -0.09228515625,
                                      -0.130615234375, -0.01904296875, -0.037841796875, 0.0302734375,
                                      -0.102294921875, -0.079345703125, 0.070068359375, 0.033203125,
                                      -0.01123046875, -0.09521484375, 0.0107421875, 0.200439453125, 0.146484375,
                                      0.044921875, -2.0, -0.21875, -0.2666015625, -0.2080078125, -0.268798828125,
                                      -0.169189453125, -0.04443359375, -0.060302734375, -0.19921875, 0.078125,
                                      0.013671875, 0.019287109375, -0.048583984375, -0.05126953125, 0.1318359375,
                                      -0.0693359375, -1.5048828125, -0.220703125, -0.90380859375, -1.221435546875,
                                      -0.298828125, -0.25146484375, -0.3994140625, -0.40869140625, -0.30908203125,
                                      -0.23583984375, -0.073974609375, -0.0146484375, 0.18115234375,
                                      0.013427734375, 0.0576171875, 0.046875, -0.103271484375, -0.1484375,
                                      -0.065185546875, 0.35498046875, 0.093505859375, 0.206298828125,
                                      0.09033203125, 0.1064453125, 0.321044921875, 0.38623046875, 0.368896484375,
                                      0.392578125, 0.05029296875, 0.229736328125, 0.1630859375, 0.220703125,
                                      0.215087890625, 0.086669921875, 0.1298828125, 0.1142578125, 0.143798828125,
                                      0.080078125, -0.17529296875, -0.01220703125, -0.237548828125,
                                      -0.301025390625, -0.0673828125, -0.099365234375, -2.0, -0.546875,
                                      0.07763671875, -0.404052734375, -0.448486328125, -0.1123046875,
                                      0.01318359375, -0.124755859375, -0.00927734375, 0.0087890625, 0.0107421875,
                                      0.014892578125, 0.059326171875, -0.126953125, -0.198974609375, -0.2119140625,
                                      -0.071044921875, -0.135009765625, 0.06005859375, -0.160400390625,
                                      -0.154541015625, 0.060546875, -2.0, -0.25146484375, -0.05712890625,
                                      -0.28955078125, -0.18701171875, -0.097412109375, -0.143798828125,
                                      -0.090087890625, -0.107666015625, -0.10107421875, 0.00927734375,
                                      -0.122802734375, -0.0869140625, 0.027587890625, 0.05078125, -1.2548828125,
                                      -0.220703125, -0.6796875, -0.35205078125, -0.083251953125, -0.208251953125,
                                      -0.123291015625, -0.345947265625, -0.07275390625, -0.07861328125,
                                      -0.145751953125, -0.13671875, -0.145263671875, -0.12158203125, 0.06494140625,
                                      0.08056640625, -1.952392578125, -0.234130859375, -0.20361328125,
                                      -0.2021484375, -0.299072265625, -0.234375, -0.28173828125, -0.214599609375,
                                      -0.291015625, -0.216552734375, -0.101318359375, -0.12744140625,
                                      -0.055908203125, -0.008056640625, -0.051025390625, -0.147216796875,
                                      -0.24609375, -0.196533203125, -0.163818359375, -0.065185546875,
                                      -0.06298828125, -0.080322265625, -0.106689453125, -0.144287109375,
                                      -0.13134765625, -0.168212890625, -0.22998046875, -0.106201171875,
                                      0.149169921875, 0.15478515625, 0.1376953125, 0.14013671875, 0.126953125,
                                      0.140380859375, 0.1513671875, 0.133056640625, 0.145751953125, 0.13134765625,
                                      -0.025634765625, -0.055908203125, -0.19970703125, -0.02490234375,
                                      0.058349609375, -0.4697265625, -0.298828125, -0.059326171875, -0.18212890625,
                                      -0.24853515625, -0.085205078125, -0.109130859375, -0.058349609375,
                                      0.005126953125, 0.00390625, 0.094970703125, -2.0, -1.017822265625,
                                      -0.8076171875, -0.57177734375, -0.137451171875, 0.0302734375,
                                      -0.176513671875, -0.009033203125, -0.017822265625, -0.006103515625,
                                      -0.46142578125, -2.0, -0.6181640625, -0.635009765625, -0.35693359375,
                                      -0.219482421875, -0.00244140625, -0.07275390625, -0.18408203125,
                                      -0.00732421875, -0.0498046875, -0.0419921875, 0.091552734375, -2.0,
                                      -1.0732421875, -0.924560546875, -0.456787109375, -0.307861328125,
                                      -0.057861328125, -0.044677734375, -0.12646484375, -0.04443359375, -2.0,
                                      -0.82958984375, -0.342041015625, -0.637939453125, -0.280517578125,
                                      -0.443359375, -0.21875, -0.153076171875, -0.12548828125, -0.070068359375,
                                      0.01220703125, 0.0185546875, -2.0, -0.99609375, -0.88037109375,
                                      -0.403564453125, -0.20458984375, -0.32470703125, -0.324951171875,
                                      -0.369140625, -0.304931640625, -0.273681640625, -0.238525390625,
                                      -0.24267578125, -0.044189453125, -0.1005859375, -0.060546875,
                                      -0.117431640625, -0.049072265625, -0.064453125, -0.173828125,
                                      -0.058349609375, 0.102783203125, -0.033935546875, -0.71728515625,
                                      0.05126953125, 0.080810546875, 0.05078125, 0.09375, 0.0712890625,
                                      0.050537109375, 0.050048828125, -0.331787109375, -0.2822265625,
                                      -0.43310546875, -0.282470703125, -0.076171875, 0.08740234375, -1.54931640625,
                                      -0.59814453125, -0.6328125, -0.125244140625, -0.157470703125,
                                      -0.120361328125, 0.092041015625, -0.156982421875, -0.0107421875, -0.0625,
                                      -0.112548828125, -0.13720703125, -0.092529296875, -0.08544921875,
                                      0.029541015625, -0.2861328125, 0.00537109375, -0.110107421875,
                                      -0.17431640625, -0.09521484375, 0.068359375, -0.1298828125, -0.099609375,
                                      -0.4931640625, -0.083251953125, -0.0927734375, 0.079833984375, 0.06689453125,
                                      0.084228515625, 0.06591796875, -0.09521484375, 0.052001953125, -0.0703125,
                                      -0.041748046875, -0.060302734375, -0.080078125, -0.0126953125,
                                      -0.04345703125, -0.119384765625, -0.22802734375, -0.068115234375,
                                      -0.045654296875, -0.113525390625, -0.15771484375, -0.13623046875,
                                      -0.127197265625, -0.12451171875, -0.19970703125, -0.14990234375,
                                      -0.1552734375, -0.02685546875, -0.1123046875, -0.147216796875, -0.13671875,
                                      -0.092041015625, -0.0986328125, -0.120361328125, -0.069091796875,
                                      -0.1552734375, -0.1484375, -0.105224609375, -0.171875, -0.20068359375,
                                      -0.116943359375, -0.142822265625, -0.092041015625, -0.06591796875,
                                      -0.10546875, -0.077880859375, -0.08203125, -0.297119140625, -0.08544921875,
                                      -0.110595703125, -0.142578125, -0.146728515625, -0.172607421875,
                                      -0.220458984375, -0.19580078125, -0.08984375, -0.10302734375, -0.1142578125,
                                      -0.078857421875, -0.0517578125, -0.105224609375, -0.064697265625,
                                      -0.09130859375, -0.12451171875, -0.10888671875, -0.06494140625,
                                      -0.139404296875, -0.061279296875, -0.07763671875, -0.026123046875,
                                      -0.052490234375, -0.066650390625, -0.0576171875, -0.150634765625,
                                      -0.08544921875, -0.078125, -0.097900390625, -0.12255859375, -0.072998046875,
                                      -0.192626953125, -0.375244140625, -0.17431640625, -0.16845703125,
                                      -0.207763671875, -0.279541015625, -0.25244140625, -0.153076171875,
                                      -0.142578125, -0.18896484375, -0.2353515625, -0.195068359375,
                                      -0.126708984375, -0.191162109375, -0.189208984375, -0.139404296875,
                                      -0.276611328125, -0.132568359375, -0.23291015625, -0.091064453125,
                                      -0.08740234375, -0.074462890625, -0.353759765625, -0.16552734375,
                                      -0.3193359375, -0.1591796875, -0.078369140625, -0.170166015625,
                                      -0.161865234375, -0.166015625, -0.0595703125, -0.25341796875,
                                      -0.060791015625, -0.001953125, -0.162353515625, -0.11328125, -0.12353515625,
                                      -0.10595703125, 0.01318359375, -0.188720703125, -0.263427734375,
                                      -0.183349609375, -0.154052734375, -0.037841796875, -0.25244140625,
                                      0.093017578125, -0.011962890625, 0.07861328125, 0.11669921875,
                                      0.083740234375, 0.033935546875, 0.07763671875, 0.041015625, 0.062744140625,
                                      0.088623046875, 0.053955078125, 0.033203125, 0.078369140625, 0.099609375,
                                      0.114990234375, 0.077392578125, 0.03369140625, 0.0595703125, 0.07861328125,
                                      0.0986328125, 0.08984375, 0.07958984375, 0.067138671875, 0.065673828125,
                                      0.00830078125, -0.04345703125, 0.044677734375, 0.1064453125, -0.1513671875,
                                      -0.1591796875, -0.189453125, -0.14990234375, -0.204833984375, -0.20263671875,
                                      -0.146484375, -0.18359375, -0.1103515625, -0.16357421875, -0.18701171875,
                                      -0.1376953125, -0.1220703125, 0.11083984375, 0.111083984375, 0.088623046875,
                                      0.091064453125, 0.05859375, 0.083740234375, -0.0654296875, -0.178955078125,
                                      -1.600830078125, -1.1142578125, -1.189208984375, -0.6923828125,
                                      -0.19970703125, -0.1279296875, -0.08642578125, -0.1064453125,
                                      -0.267822265625, -1.919921875, -1.044189453125, -0.966064453125,
                                      -0.977294921875, -0.984375, -0.9775390625, -1.000732421875, -1.08251953125,
                                      -1.053955078125, -1.007568359375, -2.0, -1.11279296875, -2.0, -0.1494140625,
                                      -0.037109375, 0.04052734375, 0.109619140625, 0.150634765625, 0.18505859375,
                                      0.15771484375, 0.142333984375, -0.048095703125, 0.104248046875,
                                      0.137939453125, 0.162109375, -0.972900390625, -1.125732421875,
                                      -1.281494140625, -1.0439453125, -1.19677734375, -1.303955078125,
                                      -1.210205078125, -1.093994140625, -1.108154296875, -1.29736328125,
                                      -1.17431640625, -1.19287109375, -1.171875, -1.154541015625, -1.199462890625,
                                      -1.14111328125, -1.218994140625, -1.227294921875, -1.0703125, -1.0673828125,
                                      -1.013427734375, -1.0029296875, -1.22216796875, -2.0, -0.220458984375,
                                      0.05078125, 0.008544921875, 0.210693359375, -1.243408203125, -1.33544921875,
                                      -1.3359375, -1.296875, -1.171875, -1.157958984375, -1.092041015625,
                                      -1.14501953125, -1.058349609375, -1.066162109375, -1.187255859375, -2.0,
                                      0.259765625, -0.241943359375, -0.395263671875, 0.2841796875, 0.2353515625,
                                      -0.19140625, -0.044921875, -1.24462890625, -1.331298828125, -1.359619140625,
                                      -1.1650390625, -1.0126953125, -1.13525390625, -1.056396484375, -1.0859375,
                                      -1.00390625, -0.6552734375, -0.7802734375, -0.782470703125, -1.09033203125,
                                      -1.11767578125, -1.103515625, -1.362060546875, -0.96630859375,
                                      -0.91552734375, -0.9248046875, -0.9541015625, -1.130859375, -1.02001953125,
                                      -1.067626953125, -0.99462890625, -1.02001953125, -1.069580078125,
                                      -1.056396484375, -1.02099609375, -1.060791015625, -1.034423828125,
                                      -1.08447265625, -1.02392578125, -1.073486328125, -1.202392578125,
                                      -1.058837890625, -1.09228515625, -1.87646484375, -1.099365234375,
                                      -1.01416015625, -1.27685546875, -1.176513671875, -1.081298828125,
                                      -1.10400390625, -1.051025390625, -1.1865234375, -1.255859375, -1.0634765625,
                                      -1.14404296875, -1.20361328125, -1.1953125, -1.295654296875, -1.2421875,
                                      -1.295166015625, -1.127197265625, -0.995849609375, -1.158203125,
                                      -1.230224609375, -1.06689453125, -1.32861328125, -0.529296875,
                                      -1.195068359375, -1.548095703125, -1.392822265625, -1.318603515625,
                                      -1.20751953125, -1.0791015625, -1.117919921875, -1.291259765625,
                                      -1.060302734375, -1.223388671875, -1.1591796875, -1.165283203125,
                                      -1.03515625, -1.765380859375, 0.220703125, 0.049560546875, -1.36376953125,
                                      -1.49853515625, -1.1611328125, -1.1904296875, -1.428466796875,
                                      -1.26708984375, -1.20703125, -1.372802734375, -1.11669921875, -1.2646484375,
                                      -1.11328125, -1.057861328125, -1.013671875, -0.33349609375, 0.231689453125,
                                      0.202392578125, 0.177001953125, 0.057373046875, 0.0712890625, 0.1552734375,
                                      0.169677734375, -0.738037109375, -1.291748046875, -1.217041015625,
                                      -1.145263671875, -1.162353515625, -1.1123046875, -1.1884765625,
                                      -1.200927734375, -1.160400390625, -1.39013671875, -1.18017578125,
                                      -1.10205078125, -1.07666015625, -1.061767578125, -1.16064453125,
                                      -1.064208984375, -1.140380859375, -1.056884765625, -1.15185546875,
                                      -1.150146484375, -1.014404296875, -1.010009765625, -1.01123046875,
                                      -1.009033203125, -1.01171875, -1.0166015625, -1.01220703125, -1.01416015625,
                                      -1.011474609375, -1.057373046875, -1.10107421875, -1.05126953125,
                                      -1.09423828125, -1.1416015625, -1.0419921875, -1.10107421875,
                                      -1.033935546875, -1.0693359375, -1.14306640625, -1.091064453125,
                                      -1.1103515625, -1.295166015625, -1.18505859375, -1.178955078125,
                                      -1.119384765625, -1.22216796875, -0.991455078125, -0.52490234375,
                                      -1.148193359375, -1.985595703125, -1.60498046875, -1.089599609375,
                                      -1.337890625, -1.109619140625, -1.1767578125, -1.2412109375, -1.023193359375,
                                      -2.0, -1.067138671875, -1.515380859375, -1.2666015625, -1.15869140625,
                                      -1.154541015625, -1.25634765625, -1.2548828125, -1.28466796875,
                                      -1.0791015625, -1.134765625, -1.07958984375, -0.994873046875, -0.7041015625,
                                      -0.62109375, -1.695068359375, -1.186279296875, -1.061279296875,
                                      -1.05419921875, -1.09033203125, -1.022705078125, -0.997314453125,
                                      -0.580810546875, -1.281494140625, -1.132568359375, -1.406494140625,
                                      -1.291259765625, -1.178955078125, -1.14453125, -1.274169921875,
                                      -1.0888671875, -1.07080078125, -1.03271484375, -1.83740234375,
                                      -0.65869140625, -1.314697265625, -1.35302734375, -1.11767578125,
                                      -1.26806640625, -1.1123046875, -1.22314453125, -1.23388671875,
                                      -1.316650390625, -1.2587890625, -1.18408203125, -1.10693359375,
                                      -1.156494140625, -1.080078125, -1.032470703125, -1.0458984375,
                                      -1.044189453125, -1.479248046875, -1.126708984375, -1.018798828125,
                                      -1.1513671875, -1.19921875, -1.028076171875, -1.093505859375,
                                      -1.050537109375, -1.028076171875, -1.0322265625, -1.372314453125,
                                      -1.031494140625, -1.1650390625, -0.362548828125, -0.7138671875, -1.2890625,
                                      -1.28125, -1.046142578125, -0.86328125, -0.439697265625, -0.37158203125,
                                      -1.256103515625, -1.314453125, -1.109375, -1.04736328125, -1.8193359375,
                                      -0.6162109375, -0.731201171875, -1.29638671875, -1.124755859375,
                                      -1.238525390625, -1.158203125, -1.023193359375, -1.711181640625,
                                      -0.452880859375, -1.375, -1.179443359375, -1.23388671875, -1.078125, -2.0,
                                      -0.41552734375, -0.758544921875, -1.2158203125, -1.3828125, -1.095458984375,
                                      -1.030517578125, -1.036865234375, -1.087890625, -1.166748046875,
                                      -1.098388671875, -1.131591796875, -1.24072265625, -1.228515625,
                                      -1.162841796875, -1.26806640625, -1.42724609375, -1.16162109375,
                                      -1.282958984375, -1.327392578125, -1.382080078125, -1.242919921875,
                                      -1.29833984375, -1.4755859375, -1.3369140625, -1.37158203125, -1.306640625,
                                      -1.4609375, -1.322021484375, -1.339111328125, -1.218994140625,
                                      -1.51123046875, -1.217041015625, -1.16015625, -1.3623046875, -1.32763671875,
                                      -1.181884765625, -1.177490234375, -1.225830078125, -1.302734375,
                                      -1.801025390625, -1.272216796875, -1.468994140625, -1.256103515625,
                                      -1.2763671875, -1.317138671875, -1.278076171875, -1.230224609375,
                                      -1.156982421875, -1.614501953125, -1.425537109375, -1.43359375,
                                      -1.345947265625, -1.322998046875, -1.66357421875, -1.32275390625,
                                      -1.1904296875, -1.365234375, -1.318603515625, -1.1669921875, -1.421630859375,
                                      -1.218994140625, -1.387451171875, -1.223876953125, -1.43505859375,
                                      -1.459716796875, -1.38037109375, -1.303466796875, -1.167724609375,
                                      -1.25439453125, -1.35302734375, -1.204345703125, -1.21923828125, -1.23046875,
                                      -1.0947265625, -1.260986328125, -1.356201171875, -1.3759765625,
                                      -1.385009765625, -1.396240234375, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0,
                                      -2.0, -2.0, -2.0, -1.87353515625, -2.0, -2.0, -1.978515625, -1.95166015625,
                                      -2.0, -1.7392578125, -1.918701171875, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0,
                                      -2.0, -1.765625, -2.0, -2.0, -2.0, -1.960205078125, -2.0, -1.99169921875,
                                      -1.779296875, -1.667236328125, -1.88037109375, -2.0, -2.0, -2.0, -2.0, -2.0,
                                      -2.0, -2.0, -1.781005859375, -2.0, -1.1796875, -0.601806640625,
                                      -1.058837890625, -1.384033203125, -1.07421875, -1.165283203125, -1.083984375,
                                      -1.263427734375, -1.196533203125, -1.110107421875, -1.058837890625,
                                      -1.19580078125, -1.4462890625, -1.162109375, -1.274658203125,
                                      -1.072021484375, -1.169921875, -1.11865234375, -1.12158203125,
                                      -1.157470703125, -1.11474609375, -1.144287109375, -1.11572265625,
                                      -1.24072265625, -1.073486328125, -1.1123046875, -1.06884765625,
                                      -1.146728515625, -1.122802734375, -1.146484375, -2.0, -2.0, -2.0, -2.0, -2.0,
                                      -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.058837890625, -1.02734375,
                                      -1.027587890625, -1.035888671875, -1.0322265625, -1.02734375, -1.26904296875,
                                      -1.7119140625, -1.2568359375, -0.3720703125, -0.696533203125,
                                      -1.142333984375, -1.0615234375, -1.263916015625, -1.34912109375,
                                      -1.2177734375, -1.93408203125, -1.167724609375, -0.564697265625,
                                      -0.695556640625, -1.45703125, 0.148193359375, 0.13134765625, 0.14404296875,
                                      -0.0283203125, -0.6640625, -0.546875, -0.12548828125, 0.052001953125,
                                      -0.55908203125, 0.770263671875, 0.7880859375, 0.7744140625, 0.727294921875,
                                      0.832275390625, 0.8515625, 0.732421875, 0.824462890625, 0.6767578125,
                                      0.806640625, 0.83203125, 0.6611328125, 0.37353515625, -0.2099609375,
                                      -0.18603515625, 0.136474609375, -1.0322265625, -1.4873046875,
                                      -0.209228515625, -0.117431640625, -0.43310546875, -0.218994140625,
                                      -0.144775390625, -0.285888671875, -0.301513671875, -0.633056640625,
                                      -0.412353515625, -0.105712890625, -0.125244140625, -0.49951171875,
                                      -0.728271484375, -0.47119140625, -0.0625, -0.2900390625, -0.5234375,
                                      -0.364013671875, 0.83154296875, 0.81884765625, 0.7490234375, 0.633056640625,
                                      -0.409912109375, -1.146728515625, -0.38916015625, 0.00439453125,
                                      -0.103271484375, -0.294921875, -0.45556640625, -0.385498046875,
                                      -0.230224609375, 0.0322265625, -0.17822265625, -1.482666015625,
                                      0.814208984375, 0.27197265625, -0.39013671875, 0.705810546875, 0.73876953125,
                                      0.75341796875, 0.81494140625, -0.482666015625, -1.0859375, -0.228271484375,
                                      -0.103271484375, 0.11767578125, -0.065185546875, 0.00537109375,
                                      -0.106201171875, -1.098876953125, -0.980224609375, -0.995849609375,
                                      -0.710693359375, -0.659423828125, -0.4208984375, -0.174072265625,
                                      -0.427490234375, -0.27099609375, -0.292724609375, -0.37158203125,
                                      -0.361328125, -0.453857421875, -0.13037109375, -0.26904296875,
                                      -0.212646484375, -0.203125, -0.13330078125, -0.125244140625, 0.060546875,
                                      0.0126953125, 0.00048828125, 0.094482421875, 0.114013671875, 0.00390625,
                                      -0.061767578125, 0.1015625, 0.111083984375, -2.0, -0.233154296875,
                                      0.068603515625, -1.189697265625, -0.415771484375, -0.47265625, -0.0361328125,
                                      -0.03173828125, -0.734375, -0.01611328125, 0.016845703125, -0.27197265625,
                                      -0.504638671875, -0.229248046875, -0.25146484375, -0.635009765625,
                                      -0.366455078125, -0.067626953125, 0.15380859375, -0.209716796875,
                                      -0.39111328125, 0.023193359375, -0.464599609375, 0.658203125, -0.6328125,
                                      -0.221923828125, -0.769287109375, -1.232177734375, -0.2529296875,
                                      -0.18896484375, -0.190185546875, -0.27099609375, 0.038330078125,
                                      -0.130859375, -0.102294921875, -0.0009765625, 0.101806640625, -0.81689453125,
                                      0.73876953125, 0.684814453125, -0.443115234375, -1.25732421875,
                                      -0.754150390625, -0.1025390625, -0.3720703125, -0.481201171875,
                                      -0.423095703125, -0.2841796875, -0.076171875, -0.668701171875,
                                      -0.518798828125, 0.07568359375, 0.07373046875, -1.367431640625,
                                      0.83642578125, 0.849853515625, 0.815185546875, 0.81494140625, 0.821044921875,
                                      0.77978515625, 0.7939453125, 0.430908203125, -0.41455078125, -1.2060546875,
                                      -0.326171875, 0.055908203125, 0.0478515625, -0.185791015625, -0.213134765625,
                                      -0.2705078125, -0.07666015625, 0.005859375, 0.06982421875, -0.025146484375,
                                      0.00537109375, -0.0595703125, -0.01904296875, 0.00439453125, -0.00146484375,
                                      -0.108154296875, -0.14453125, -0.181640625, -0.17919921875, -0.184326171875,
                                      -0.176025390625, -0.170166015625, -0.152099609375, -0.163818359375,
                                      -0.152099609375, -0.15625, -0.14306640625, -0.10791015625, -0.06396484375,
                                      -0.08203125, -0.098388671875, -0.080810546875, -0.58203125, -0.31982421875,
                                      -0.11328125, -0.0478515625, -0.04345703125, -0.159423828125, -0.194580078125,
                                      -0.2119140625, -0.102783203125, -0.320556640625, -0.14453125,
                                      -0.551025390625, 0.176513671875, -1.05126953125, -1.070556640625,
                                      -0.368408203125, -0.02978515625, -0.24365234375, -0.206298828125,
                                      -0.304443359375, -0.228515625, -0.27685546875, -1.42041015625,
                                      -0.96240234375, -1.1611328125, -0.800048828125, -0.490478515625,
                                      -0.187255859375, -0.227294921875, -0.309326171875, -0.25146484375,
                                      -0.211181640625, -0.40087890625, -0.3125, -0.519287109375, -0.413818359375,
                                      -1.316162109375, -1.158447265625, -0.396728515625, -0.252197265625,
                                      -0.206298828125, -0.296142578125, -0.2763671875, -0.37109375,
                                      -1.273193359375, -0.974609375, -0.259765625, -0.544189453125, -0.3759765625,
                                      -0.303466796875, -0.333251953125, -0.381103515625, -0.233154296875,
                                      -0.146728515625, -0.225341796875, -0.626708984375, -1.08740234375,
                                      -1.28759765625, -0.49658203125, -0.347900390625, -0.509521484375,
                                      -0.394287109375, -0.330078125, -0.29345703125, -0.30810546875,
                                      -0.362548828125, -0.313720703125, -0.1435546875, -0.249267578125,
                                      -0.278076171875, -0.38818359375, -0.36376953125, -0.278564453125,
                                      -0.354736328125, -0.364013671875, -0.2421875, -0.294677734375,
                                      -0.337158203125, -0.194580078125, -0.17724609375, -0.243408203125,
                                      -0.1923828125, -0.571533203125, -0.60595703125, -0.493896484375, -2.0,
                                      -1.258544921875, -1.36865234375, -1.388427734375, -0.371826171875,
                                      -0.3349609375, -2.0, -1.24267578125, -1.35400390625, -1.38623046875,
                                      -0.697509765625, -0.331787109375, -0.41845703125, -2.0, -1.485107421875,
                                      -1.27880859375, -1.348876953125, -0.42822265625, -0.418701171875,
                                      -0.366943359375, -0.323974609375, -2.0, -1.47216796875, -1.21875,
                                      -0.982421875, -0.32275390625, -0.2177734375, -2.0, -1.268798828125,
                                      -1.293212890625, -1.220703125, -0.39208984375, -0.15087890625,
                                      -0.105224609375, -0.164306640625, -0.092041015625, -0.275146484375,
                                      -0.203857421875, -0.2587890625, -0.29833984375, -0.27978515625,
                                      -0.4228515625, -0.337890625, -0.34716796875, -0.31640625, -0.325439453125,
                                      -0.29541015625, -0.3232421875, -0.358642578125, -0.28662109375,
                                      -0.22314453125, -0.33642578125, -0.320068359375, -0.224365234375,
                                      -0.275146484375, -0.330078125, -0.337646484375, -0.199951171875,
                                      -0.33935546875, -0.32861328125, -0.404052734375, -0.315673828125,
                                      -0.306640625, -0.359375, -0.523193359375, -0.4072265625, -0.2841796875,
                                      -0.322265625, -0.32861328125, -0.352783203125, -0.423583984375,
                                      -0.35302734375, -0.267333984375, -0.433349609375, -0.203857421875,
                                      -0.274658203125, -0.2724609375, -0.362060546875, -0.20068359375,
                                      -0.314453125, -0.271728515625, -0.253173828125, -0.15478515625, -0.244140625,
                                      -0.27001953125, -0.330810546875, -0.3203125, -0.129150390625,
                                      -0.357177734375, -0.270263671875, -0.32568359375, -0.207275390625,
                                      -0.29150390625, -0.24951171875, -0.259765625, -0.2109375, -0.09912109375,
                                      -0.227294921875, -0.23828125, -0.221923828125, -0.093017578125,
                                      -0.14990234375, -0.27001953125, -0.195068359375, -0.2158203125,
                                      -0.29736328125, -0.322998046875, -0.37841796875, -0.34912109375,
                                      -0.29931640625, -0.280029296875, -0.286376953125, -0.267578125,
                                      -0.245849609375, -0.240966796875, -0.29248046875, -0.17919921875,
                                      -0.215087890625, -0.248779296875, -0.231201171875, -0.243896484375,
                                      -0.210205078125, -0.2412109375, -0.386474609375, -0.26171875,
                                      -0.263916015625, -0.228759765625, -0.2333984375, -0.29248046875,
                                      -0.22412109375, -0.248291015625, -0.421142578125, -0.554931640625,
                                      -0.33203125, -0.2802734375, -0.37255859375, -0.21240234375, -0.28125,
                                      -0.401611328125, -0.18994140625, -0.364501953125, -0.28173828125,
                                      -0.344482421875, -0.21826171875, -0.389892578125, -0.227783203125,
                                      -0.259033203125, -0.3994140625, -0.291259765625, -0.236572265625,
                                      -0.408935546875, -1.279052734375, -1.14404296875, -0.93701171875,
                                      -0.80419921875, -0.720947265625, -0.753662109375, -0.803466796875,
                                      -0.839111328125, -0.887939453125, -0.64111328125, -0.806884765625,
                                      -0.74951171875, -0.308837890625, -0.75732421875, -0.874755859375,
                                      -0.62939453125, -0.767333984375, -0.683349609375, -0.875244140625,
                                      -1.084716796875, -0.437255859375, -0.644287109375, -0.787109375,
                                      -1.068115234375, -0.869873046875, -0.669189453125, -0.9169921875,
                                      -0.7568359375, -0.232666015625, -0.29345703125, -0.3916015625, -0.712890625,
                                      -0.615234375, -0.487060546875, -0.260986328125, -0.51904296875,
                                      -0.778076171875, -0.45947265625, -0.34716796875, -0.682861328125,
                                      -0.483154296875, -0.436279296875, -0.725341796875, -0.115966796875,
                                      -0.0966796875, -0.123291015625, -0.153076171875, -0.1357421875,
                                      -0.154052734375, -0.496826171875, -0.80615234375, -1.36669921875,
                                      0.034423828125, -1.135009765625, -1.126708984375, -0.97119140625,
                                      -0.225341796875, -0.341064453125, -0.352783203125, -1.3388671875,
                                      -1.258544921875, 0.104736328125, -1.222900390625, -1.37404580152672,
                                      0.984804483605613, 0.983370465181009, 0.983573860046131, 0.946893489140952,
                                      0.914633222246091, 0.917833001671195, 0.336538619785481, 0.859952871062572,
                                      0.368884608382422, 0.776239463808361, 0.806029339838477, 0.796342297688653,
                                      0.747570473783991, 0.874865180937028, 0.890177564159894, 0.767147185903779,
                                      0.861580083070584, 0.742162965067843, 0.841915255053556, 0.853533013557319,
                                      0.711883125627209, 0.683232457189424, 0.751114771790616, 0.932612521973769,
                                      0.884366987751498, 0.587871006229566, 0.370432696573115, 0.787329744675081,
                                      0.862982961636757, 0.849343690217927, 0.768772932811544, 0.88944135615387,
                                      0.680279265868075, 0.814322737907355, 0.940252459692483, 0.859403609753264,
                                      0.913843699294254, 0.932628499706427, 0.752142191315903, 0.943701324723415,
                                      0.96131346876248, 0.966948526221567, 0.945983175908385, 0.791629430664809,
                                      0.338930679221708, 0.87010215121206, 0.854064276226163, 0.820743774395099,
                                      0.749924894387014, 0.664893673592423, 0.62057521455195, 0.732107069390248,
                                      0.752028067681344, 0.866519051751248, 0.789804128979371, 0.912520616115586,
                                      0.909616531334745, 0.89795894316759, 0.920997234073176, 0.267648172607398,
                                      0.573911260161846, 0.880206219070574, 0.840796670791775, 0.777520126192676,
                                      0.810655517650732, 0.80611526339284, 0.799472129534008, 0.847030901195438,
                                      0.678122769189497, 0.551324779083577, 0.643589233026956, 0.712415563030625,
                                      0.91529632501972, 0.912303862982745, 0.866055067073565, 0.866796938263116,
                                      0.813480116159799, 1.01906969869786, 0.940292966417269, 0.967851837708729,
                                      0.823476629033269, 0.897721079584408, 0.905642009546411, 0.633328686454524,
                                      0.899966191610479, 0.988540253606389, 0.971001193606648, 0.957925464384529,
                                      0.863465233439202, 0.969752285434659, 0.981484210254133, 0.977502111447291,
                                      0.983506652272334, 0.903690109853856, 0.939204446788087, 0.804429837056777,
                                      0.800921804606966, 0.9411346550288, 0.920226643934095, 0.90510361610443,
                                      0.856183698273008, 0.91433796440627, 0.879065266105458, 0.867753677435105,
                                      0.899811067883554, 0.97024685058633, 0.958957865983771, 0.926629462564453,
                                      0.508054059602706, 0.897235364656936, 0.974860516376613, 0.942788340227699,
                                      0.923748795246583, 0.978317301557633, 0.957571009799105, 0.517668690945036,
                                      0.896325689495525, 0.877544891589375, 0.780152923864162, 0.879097947302019,
                                      0.738957050373637, 0.863480143700339, 0.937506643907529, 0.79029576076822,
                                      0.909481273091327, 0.881124993514636, 0.284689743753378, 0.747820747798672,
                                      0.777220328036092, 0.710673985533336, 0.822287226232371, 0.59796139774632,
                                      0.812105596624395, 0.975744163962916, 0.905731611959627, 0.85781100747234,
                                      0.924697595858984, 0.881592642002762, 0.890998260181668, 0.749324056883859,
                                      0.898141696166831, 0.240945523297894, 0.810862321253789, 0.873054862785921,
                                      0.814371447961633, 0.530715336742655, 0.800076318617187, 0.738266435101129,
                                      0.724097099086712, 0.76238194474139, 0.908846784997091, 0.82225087349678,
                                      0.814660793696377, 0.928562284780902, 0.838052160524308, 0.842216687944051,
                                      0.535131579199384, 0.342354978223184, 0.889068514999664, 0.903041190248232,
                                      0.858594574723963, 0.835543102739191, 0.854267548597149, 0.866799620064871,
                                      0.846806957706776, 0.705482007955043, 0.50114744369173, 0.681207557546028,
                                      0.708472285011959, 0.809936183075593, 0.782953845757518, 0.773144830373861,
                                      0.784925588251783, 0.865774773816346, 0.851382630276628, 0.819357397732773,
                                      0.902153820840218, 0.95942351625515, 0.960550897255535, 0.867127699981001,
                                      0.893244421618275, 0.904988627810184, 0.93331205834823, 0.856562816253793,
                                      0.935125140419195, 1.01342449955067, 1.01033204828779, 1.01536266166595,
                                      1.0162386287602, 1.00552042463018, 1.01414987102467, 1.01471836998273,
                                      1.01120241163998, 1.01138918895997, 0.998187419155453, 0.854335574375487,
                                      0.957296653848026, 0.918965062771372, 0.935590293051983, 0.89117083637976,
                                      0.881050613519057, 0.96275731028533, 0.926694073912119, 0.89536326665565,
                                      0.912175668271819, 0.919482764146324, 0.756551302638302, 0.872038188141872,
                                      0.885552466954975, 0.889150938921627, 0.792536613087919, 0.383113975926368,
                                      0.752180624892001, 0.855241893492379, 0.81718564589456, 0.687675887063447,
                                      0.926067517280568, 0.794632167652418, 0.831870372424019, 0.943706914410854,
                                      0.81660853166031, 0.333477505658771, 0.323997605980164, 0.916631658203464,
                                      0.838918415676952, 0.638651809610497, 0.911637374005171, 0.914343701007495,
                                      0.811378328315656, 0.704025793562135, 0.7796440785153, 0.928170770100981,
                                      0.771111738773913, 0.95520386911484, 0.312442583531511, 0.852895900161055,
                                      0.840992199987466, 0.724002635524231, 0.569254691083273, 0.925116513620911,
                                      0.986119102765028, 0.946746778462028, 0.37410717218596, 0.671998921875997,
                                      0.828280092507349, 0.651534486529764, 0.833949211147586, 0.408376702451354,
                                      0.748582215145629, 0.887080181923126, 0.857437898316699, 0.858778382322327,
                                      0.959483620772672, 0.875028541644398, 0.969863190490668, 0.266043613658722,
                                      0.917710483013036, 0.886858118338805, 0.584421153974347, 0.937359767598277,
                                      0.75892616319675, 0.861575171232526, 0.85920179528967, 0.81187818648467,
                                      0.824116198772345, 0.844668912298484, 0.897729246194424, 0.874931673379459,
                                      0.945110488618751, 0.976538146668997, 1.00178425278544, 0.984803787575418,
                                      1.00260235014251, 0.806410232412893, 0.90990663961287, 1.00530700331096,
                                      0.973281169357946, 0.872075404443858, 1.00032336252089, 0.968176795209309,
                                      1.01027694535754, 1.00416632819797, 0.991779904485155, 0.701785868066114,
                                      0.699370213305581, 0.229423220396627, 0.826376346329109, 0.79948748771653,
                                      0.64431453876282, 0.74044060161962, 0.94135718000112, 0.210409138485033,
                                      0.816950602867258, 0.83129864887342, 0.688247014730335, 0.744427803274246,
                                      0.88742863811445, 0.858929709464062, 0.173673410893632, 0.914850987145965,
                                      0.773417809746304, 0.795149524530598, 0.871026040768617, 0.885049871127918,
                                      0.929432361117864, 0.242643112347719, 0.130438055208173, 0.862666640561981,
                                      0.756617675811343, 0.67490599295538, 0.708546484871282, 0.961769952968174,
                                      0.167204426416641, 0.806560299917278, 0.809622216487715, 0.748735514263459,
                                      0.752922760650793, 0.961232799033848, 1.00390307355445, 1.00599071052653,
                                      0.868310424687984, 0.911742306023349, 0.930975174514491, 0.84463514594081,
                                      0.776831299833222, 0.788552487667389, 0.842171888613697, 0.763799866392289,
                                      0.722796204907324, 0.859123123710822, 0.805800510758451, 0.770091941781528,
                                      0.593347764545131, 0.891386108438349, 0.819565387374852, 0.623648660794725,
                                      0.711208627164563, 0.837521665207489, 0.780934727951204, 0.757852219002803,
                                      0.756034535065778, 0.871678109309678, 0.82813989877794, 0.864831456042732,
                                      0.714540052261707, 0.921335288645615, 0.837869781732849, 0.938275525077996,
                                      0.736799620379641, 0.845607443808414, 0.800478584963329, 0.819555205512328,
                                      0.720158606451098, 0.761008924700102, 0.910773923134042, 0.842768557502347,
                                      0.675129217565846, 0.722627092752691, 0.762783499685678, 0.846453714629621,
                                      0.754096644577435, 0.753525435174706, 0.690032672177657, 0.835257494278838,
                                      0.865077777408767, 0.708193250903957, 0.644741879350302, 0.914429289430989,
                                      0.765072759646294, 0.768176362545141, 0.632866468600733, 0.925106430374524,
                                      0.711477305288262, 0.798799397667862, 0.701309828084533, 0.820767267508569,
                                      0.789925735701131, 0.888265395211717, 0.702730576832014, 0.705965408442243,
                                      0.825176821168559, 0.827871535756902, 0.737217939717818, 0.706398106127065,
                                      0.797316821415622, 0.855457671629359, 0.928485798948497, 0.783106847919544,
                                      0.7487673962185, 0.842643612866434, 0.816229404113678, 0.809210758409153,
                                      0.545357588076278, 0.188897014736095, 0.201773475415668, 0.154942573861296,
                                      0.199766416044612, 0.209956821135157, 0.246555198493635, 0.117629172564289,
                                      0.168684211905264, 0.149630321500474, 0.171221096077748, 0.173512719141619,
                                      0.20107585504282, 0.187776679306028, 0.192261633492478, 0.266707179349852,
                                      0.120146957057943, 0.0997080594569291, 0.193156588788762, 0.191195157484158,
                                      0.238963666444536, 0.149150741018257, 0.28006239332126, 0.233446188156045,
                                      0.227192364920012, 0.388970188695074, 0.195072178809213, 0.206724983980577,
                                      0.185406440995808, 0.205908990487142, 0.253910240728855, 0.410250843584215,
                                      0.285156772560791, 0.577380809704328, 0.343837813518463, 0.407797872346769,
                                      0.308240277621978, 0.136966807263518, 0.237630236203266, 0.43589891652644,
                                      0.28378908770378, 0.339390507268246, 0.488996912059122, 0.434422225580457,
                                      0.807287884775143, 0.943827100471728, 0.870153081733688, 0.763657084368138,
                                      0.664085881438672, 0.985063766022687, 0.699777944289644, 0.719771198767452,
                                      0.925442307632416, 0.767317943787659, 0.803182360905093, 0.90275788815099,
                                      0.483499412394248, 0.584538582738236, 0.751099654528219, 0.809787403629022,
                                      0.922366027666388, 0.75929865669193, 0.848230404808218, 0.776669617293924,
                                      0.873120537366228, 0.953048515377827, 0.694423836296324, 0.777921873796141,
                                      0.926944886631934, 0.753805914808728, 0.783665774868039, 0.92155919889038,
                                      0.687303775180776, 0.869914192813275, 0.0778150130635931, 0.149333059256734,
                                      0.147387151383816, 0.171035798592062, 0.129428844580637, 0.0974170043021411,
                                      0.145467253287019, 0.14765684743286, 0.0776202106319048, 0.112126482678395,
                                      0.0982325524521637, 0.129394761571139, 0.19920528590761, 0.992405604622731,
                                      1.00313865319583, 1.00566398426499, 1.0069597170026, 0.997916226131419,
                                      1.0034856546039, 0.870124380165453, 0.566175163746922, 0.632095778746557,
                                      0.850064070474719, 0.90255173437142, 0.811405030941429, 0.821230783107051,
                                      0.813364705870815, 0.773139163961472, 0.849598255303939, 0.529871161023789,
                                      0.587810777076577, 0.936825000632454, 0.840451078610327, 0.783896389116627,
                                      -3.97709923664122, -4.80916030534351, -6.30534351145038, -42.6488549618321,
                                      -83.5343511450382, -23.7328244274809, -52.6717557251908, -10.9770992366412,
                                      -219.152671755725, -45.8854961832061, -26.9618320610687, -11.6564885496183,
                                      -14.7786259541985, -9.19083969465649, -4.12977099236641, -10.2671755725191,
                                      -4.65648854961832, -23.9007633587786, -15.7022900763359, -11.3740458015267,
                                      -5.94656488549618, -136.12213740458, -74.3435114503817, -28.4656488549618,
                                      -42.5267175572519, -119.0, -250.137404580153, -94.763358778626,
                                      -49.5343511450382, -123.87786259542, -40.793893129771, -24.7862595419847,
                                      -50.7709923664122, -55.8015267175573, -78.6259541984733, -64.1450381679389,
                                      -32.5038167938931, -7.22137404580153, -78.4580152671756, -128.954198473282,
                                      -16.9389312977099, -13.618320610687, -32.0687022900763, -61.0610687022901,
                                      -196.389312977099, -5.92366412213741, -19.2595419847328, -52.7251908396947,
                                      -31.7328244274809, -138.412213740458, -146.458015267176, -38.0458015267176,
                                      -63.7022900763359, -52.6717557251908, -46.8931297709924, -84.6259541984733,
                                      -35.1984732824428, -85.3969465648855, -10.4427480916031, -7.41221374045802,
                                      -250.137404580153, -14.9389312977099, -41.175572519084, -48.8396946564886,
                                      -22.0763358778626, -67.9541984732825, -32.4809160305344, -39.6259541984733,
                                      -146.022900763359, -187.587786259542, -41.9389312977099, -14.824427480916,
                                      -16.5419847328244, -24.6793893129771, -31.7251908396947, -50.8167938931298,
                                      -250.137404580153, -23.030534351145, -34.1068702290076, -37.1679389312977,
                                      -104.63358778626, -126.793893129771, -124.007633587786, -121.687022900763,
                                      -44.7175572519084, -12.0076335877863, -33.3206106870229, -19.0610687022901,
                                      -13.206106870229, -29.206106870229, -27.4351145038168, -39.6030534351145,
                                      -25.8473282442748, -162.916030534351, -45.4503816793893, -31.6335877862595,
                                      -52.4045801526718, -11.9923664122137, -44.2900763358779, -27.5954198473282,
                                      -64.2213740458015, -124.610687022901, -43.6870229007634, -22.4656488549618,
                                      -44.3511450381679, -19.2900763358779, -6.72519083969466, -22.0610687022901,
                                      -41.7251908396947, -23.0839694656489, -15.3740458015267, -19.5496183206107,
                                      -20.793893129771, -33.763358778626, -16.412213740458, -45.5801526717557,
                                      -32.6412213740458, -24.2442748091603, -72.8320610687023, -82.8015267175573,
                                      -84.3282442748092, -56.3740458015267, -8.01526717557252, -69.7175572519084,
                                      -35.7557251908397, -0.0687022900763359, -194.671755725191, -103.709923664122,
                                      -108.977099236641, -48.9312977099237, -86.1145038167939, -139.793893129771,
                                      -28.7786259541985, -11.3893129770992, -47.7328244274809, -19.3053435114504,
                                      -45.9923664122137, -44.4503816793893, -30.2748091603053, -14.0,
                                      -4.42748091603053, -250.137404580153, -12.3053435114504, -37.236641221374,
                                      -103.374045801527, -137.221374045802, -56.2137404580153, -61.2824427480916,
                                      -67.9007633587786, -53.175572519084, -14.2137404580153, -79.5954198473282,
                                      -83.0916030534351, -71.0916030534351, -6.45038167938931, -10.4427480916031,
                                      -3.78625954198473, -44.1297709923664, -5.87786259541985, -5.89312977099237,
                                      -12.2519083969466, -22.7328244274809, -15.4198473282443, -11.2519083969466,
                                      -5.32824427480916, -98.824427480916, -126.0, -137.954198473282,
                                      -18.7022900763359, -55.0992366412214, -56.206106870229, -35.206106870229,
                                      -18.8549618320611, -35.7480916030534, -30.8625954198473, -36.2900763358779,
                                      -12.3053435114504, -29.1679389312977, -16.5038167938931, -35.587786259542,
                                      -15.0610687022901, -21.5038167938931, -14.8702290076336, -29.2824427480916,
                                      -33.4198473282443, -6.22137404580153, -3.43511450381679, -2.34351145038168,
                                      -3.9618320610687, -3.19083969465649, -2.82442748091603, -5.18320610687023,
                                      -4.01526717557252, -4.74045801526718, -2.51145038167939, -88.618320610687,
                                      -22.0992366412214, -17.1450381679389, -24.4351145038168, -18.4274809160305,
                                      -69.3969465648855, -2.65648854961832, -10.2671755725191, -31.4045801526718,
                                      -23.5725190839695, -32.2824427480916, -30.3358778625954, -46.206106870229,
                                      -58.1068702290076, -22.7786259541985, -16.0839694656489, -52.2519083969466,
                                      -74.9923664122137, -122.213740458015, -36.4198473282443, -40.030534351145,
                                      -15.7404580152672, -36.1526717557252, -99.824427480916, -50.2748091603053,
                                      -16.8320610687023, -61.4274809160305, -250.137404580153, -117.595419847328,
                                      -108.725190839695, -50.0839694656489, -51.1297709923664, -24.9923664122137,
                                      -32.0, -58.9389312977099, -18.4656488549618, -21.175572519084,
                                      -109.854961832061, -43.4732824427481, -164.725190839695, -96.7022900763359,
                                      -69.7175572519084, -31.4045801526718, -42.5725190839695, -11.9618320610687,
                                      -11.763358778626, -24.6106870229008, -17.030534351145, -91.5114503816794,
                                      -70.3587786259542, -52.2442748091603, -19.6717557251908, -44.5801526717557,
                                      -38.1374045801527, -25.2290076335878, -18.0534351145038, -28.7557251908397,
                                      -26.5114503816794, -54.557251908397, -14.8854961832061, -137.832061068702,
                                      -94.5419847328244, -28.9923664122137, -18.2137404580153, -17.6793893129771,
                                      -49.2748091603053, -20.4809160305344, -15.9618320610687, -45.0534351145038,
                                      -30.969465648855, -29.3740458015267, -29.3740458015267, -52.0229007633588,
                                      -15.2290076335878, -13.793893129771, -18.2137404580153, -18.9770992366412,
                                      -9.50381679389313, -59.0763358778626, -18.2671755725191, -10.2442748091603,
                                      -19.8091603053435, -43.9007633587786, -11.3664122137405, -5.23664122137405,
                                      -10.0916030534351, -9.92366412213741, -34.236641221374, -39.6946564885496,
                                      -100.24427480916, -250.137404580153, -50.1908396946565, -128.488549618321,
                                      -176.007633587786, -29.7175572519084, -65.6030534351145, -103.435114503817,
                                      -82.6412213740458, -92.0229007633588, -167.106870229008, -27.4580152671756,
                                      -13.4809160305344, -56.3969465648855, -137.450381679389, -75.4045801526718,
                                      -63.6259541984733, -155.114503816794, -16.2671755725191, -53.9083969465649,
                                      -25.969465648855, -86.6564885496183, -177.923664122137, -83.4045801526718,
                                      -159.412213740458, -25.3053435114504, -29.4885496183206, -22.0229007633588,
                                      -250.137404580153, -93.5267175572519, -63.9465648854962, -148.190839694657,
                                      -32.3053435114504, -13.3969465648855, -4.73282442748092, -5.49618320610687,
                                      -41.5267175572519, -20.7709923664122, -26.0381679389313, -31.9541984732824,
                                      -39.2595419847328, -53.5801526717557, -82.2137404580153, -52.4274809160305,
                                      -26.5190839694656, -67.7480916030534, -64.5419847328244, -58.030534351145,
                                      -33.5038167938931, -41.6412213740458, -74.1450381679389, -52.9160305343511,
                                      -20.8931297709924, -43.8015267175573, -57.3664122137405, -36.0458015267176,
                                      -15.2748091603053, -48.6793893129771, -52.8778625954199, -75.0687022900763,
                                      -7.9618320610687, -29.206106870229, -50.0534351145038, -51.3206106870229,
                                      -37.0076335877863, -59.236641221374, -40.5725190839695, -108.534351145038,
                                      -60.3740458015267, -39.0916030534351, -52.4503816793893, -110.137404580153,
                                      -58.8854961832061, -17.8702290076336, -49.3587786259542, -67.9160305343512,
                                      -53.0076335877863, -25.9160305343511, -28.6412213740458, -55.3053435114504,
                                      -79.4503816793893, -16.8015267175573, -22.9618320610687, -58.1526717557252,
                                      -50.5343511450382, -16.9389312977099, -25.5038167938931, -42.0916030534351,
                                      -67.9389312977099, -66.3053435114504, -34.2824427480916, -27.2519083969466,
                                      -82.412213740458, -67.6259541984733, -23.793893129771, -23.5954198473282,
                                      -33.5801526717557, -45.5725190839695, -55.8473282442748, -25.9770992366412,
                                      -40.1450381679389, -51.5954198473282, -42.5419847328244, -24.4656488549618,
                                      -17.5496183206107, -60.3282442748092, -52.6335877862595, -27.618320610687,
                                      -36.2824427480916, -72.9083969465649, -73.8320610687023, -38.7328244274809,
                                      -109.732824427481, -41.7786259541985, -104.870229007634, -34.2824427480916,
                                      -67.4809160305344, -31.1450381679389, -26.9923664122137, -64.6412213740458,
                                      -54.2671755725191, -28.175572519084, -39.8320610687023, -31.4656488549618,
                                      -36.9465648854962, -44.8015267175573, -28.3282442748092, -36.030534351145,
                                      -42.0992366412214, -76.5648854961832, -57.3053435114504, -65.1450381679389,
                                      -104.36641221374, -99.763358778626, -29.7022900763359, -63.5801526717557,
                                      -27.6106870229008, -48.7175572519084, -34.3740458015267, -26.7786259541985,
                                      -44.9236641221374, -40.4198473282443, -43.0229007633588, -55.3740458015267,
                                      -51.0534351145038, -30.9541984732824, -60.0763358778626, -62.7786259541985,
                                      -37.969465648855, -96.9236641221374, -49.1374045801527, -64.1068702290076,
                                      -114.022900763359, -25.206106870229, -7.49618320610687, -37.2824427480916,
                                      -65.0, -30.8702290076336, -45.0687022900763, -71.6946564885496,
                                      -31.8396946564886, -18.9312977099237, -64.2519083969466, -41.9007633587786,
                                      -34.4732824427481, -54.2595419847328, -56.3435114503817, -63.2900763358779,
                                      -38.824427480916, -41.5114503816794, -73.5419847328244, -32.2595419847328,
                                      -40.3129770992366, -42.4045801526718, -54.5648854961832, -67.7404580152672,
                                      -40.5954198473282, -25.8625954198473, -71.4885496183206, -33.2671755725191,
                                      -28.8854961832061, -44.6793893129771, -42.0, -110.893129770992,
                                      -62.7328244274809, -73.6259541984733, -96.3740458015267, -75.2595419847328,
                                      -53.9465648854962, -145.145038167939, -39.9770992366412, -77.824427480916,
                                      -82.5114503816794, -59.1068702290076, -150.259541984733, -9.94656488549618,
                                      -4.3206106870229, -7.94656488549618, -8.40458015267176, -8.09923664122137,
                                      -13.6717557251908, -72.9618320610687, -163.862595419847, -178.885496183206,
                                      -26.5419847328244, -133.725190839695, -125.885496183206, -102.190839694656,
                                      -57.969465648855, -40.236641221374, -38.1297709923664, -194.778625954198,
                                      -11.6030534351145, -113.923664122137, -115.045801526718, -87.0992366412214,
                                      -2.3969465648855, -1.58015267175573, -1.70229007633588, -76.1297709923664,
                                      -89.7099236641221, -36.5114503816794, -63.030534351145, -10.8473282442748,
                                      -63.8167938931298, -8.86259541984733, -3.02290076335878, -4.98473282442748,
                                      -8.6793893129771, -3.55725190839695, -2.24427480916031, -4.87022900763359,
                                      -6.35114503816794, -9.33587786259542, -15.6641221374046, -5.74809160305344,
                                      -15.8320610687023, -38.1679389312977, -24.763358778626, -20.3282442748092,
                                      -49.8091603053435, -102.923664122137, -56.5038167938931, -49.4809160305344,
                                      -43.587786259542, -15.5190839694657, -36.6564885496183, -26.1145038167939,
                                      -96.5038167938931, -84.0992366412214, -39.4809160305344, -15.2824427480916,
                                      -26.5038167938931, -48.6412213740458, -136.885496183206, -88.1450381679389,
                                      -32.2824427480916, -3.64885496183206, -78.9618320610687, -93.4580152671756,
                                      -83.1679389312977, -13.4961832061069, -12.4885496183206, -13.6870229007634,
                                      -18.0229007633588, -35.2213740458015, -34.7709923664122, 54.4198473282443,
                                      -103.748091603053, -148.679389312977, -141.549618320611, -63.8167938931298,
                                      -82.557251908397, -91.1603053435115, -3.01526717557252, -5.79389312977099,
                                      -19.5496183206107, -7.99236641221374, 9.09923664122137, -141.977099236641,
                                      -19.4427480916031, -14.1603053435115, -29.2137404580153, -15.7557251908397,
                                      -55.0916030534351, -88.618320610687, -132.916030534351, -114.984732824427,
                                      -6.97709923664122, 3.48091603053435, -5.78625954198473, -7.31297709923664,
                                      -26.9465648854962, -21.7099236641221, -15.1984732824427, -42.3282442748092,
                                      -46.6641221374046, -145.129770992366, -119.832061068702, -47.4427480916031,
                                      -11.0229007633588, -11.4885496183206, -28.0534351145038, -22.8625954198473,
                                      -127.435114503817, -6.81679389312977, -10.6412213740458, -16.2671755725191,
                                      -25.587786259542, -87.8167938931298, -31.7709923664122, -27.0458015267176,
                                      -17.8396946564886, -6.52671755725191, -34.030534351145, -14.3893129770992,
                                      -51.824427480916, -157.93893129771, -35.1450381679389, -38.4274809160305,
                                      -100.870229007634, -38.5190839694657, -9.44274809160305, -30.1832061068702,
                                      -170.977099236641, -21.8549618320611, -20.9923664122137, -15.1221374045802,
                                      -4.6030534351145, -17.8625954198473, -12.2290076335878, -24.8625954198473,
                                      -48.1984732824428, -36.9923664122137, -31.9007633587786, -92.3282442748092,
                                      -86.2595419847328, -27.4580152671756, -4.81679389312977, -16.6870229007634,
                                      -108.572519083969, -17.2519083969466, -69.0916030534351, -21.0229007633588,
                                      -21.2442748091603, -7.83969465648855, -138.908396946565, -21.8854961832061,
                                      -49.2137404580153, -26.9465648854962, -36.381679389313, -73.6412213740458,
                                      -13.9389312977099, -57.6412213740458, -103.290076335878, -34.0763358778626,
                                      -2.27480916030534, -8.91603053435115, -12.9847328244275, -53.2519083969466,
                                      -32.2977099236641, -46.7480916030534, -128.625954198473, 34.6335877862595,
                                      -120.580152671756, -164.274809160305, -144.603053435115, -79.9389312977099,
                                      -17.5572519083969, -103.12213740458, -69.557251908397, -9.51908396946565,
                                      -0.381679389312977, -250.137404580153, -3.25190839694656, -2.53435114503817,
                                      -2.36641221374046, -10.7709923664122, -9.24427480916031, -16.4274809160305,
                                      -0.977099236641221, -19.8931297709924, -43.824427480916, -67.5419847328244,
                                      -14.3969465648855, 4.64885496183206, -2.35114503816794, -36.2824427480916,
                                      -75.0229007633588, -34.9618320610687, -19.236641221374, -31.793893129771,
                                      -18.1297709923664, -16.8091603053435, -32.7557251908397, -23.030534351145,
                                      -17.8015267175573, -51.3358778625954, -29.9465648854962, -51.4961832061069,
                                      -1.6793893129771, -5.81679389312977, -2.02290076335878, -4.6412213740458,
                                      -4.30534351145038, -3.25954198473282, -0.824427480916031, -2.09160305343511,
                                      -4.90839694656489, -1.49618320610687, -11.6259541984733, -41.8625954198473,
                                      -24.969465648855, -19.2595419847328, -34.2824427480916, -14.9160305343511,
                                      -10.7251908396947, -39.8702290076336, -25.9847328244275, -14.1374045801527,
                                      -30.030534351145, -103.832061068702, -19.6946564885496, -33.5267175572519,
                                      -24.7175572519084, -33.9007633587786, -8.46564885496183, -192.595419847328,
                                      -15.5801526717557, -14.0534351145038, -60.1603053435115, -39.4656488549618,
                                      -6.04580152671756, -48.2824427480916, -29.6412213740458, -30.4045801526718,
                                      -9.37404580152672, -60.587786259542, -122.007633587786, -5.65648854961832,
                                      -80.4961832061069, -59.0916030534351, -45.4885496183206, -2.29007633587786,
                                      -89.4351145038168, -91.0992366412214, -9.66412213740458, -22.0763358778626,
                                      -56.5419847328244, -13.1679389312977, -191.503816793893, -91.6564885496183,
                                      -42.0458015267176, -79.2290076335878, -38.9236641221374, -15.4351145038168,
                                      -13.9847328244275, -24.4809160305344, -73.0, -241.656488549618,
                                      -49.4580152671756, -79.4503816793893, -50.6870229007634, -124.343511450382,
                                      -75.6412213740458, -61.1603053435115, -28.3129770992366, -13.824427480916,
                                      -55.4351145038168, -27.4503816793893, -4.68702290076336, -250.137404580153,
                                      -33.0458015267176, -46.5801526717557, -42.0687022900763, -38.2137404580153,
                                      -41.9007633587786, -87.6106870229008, -42.9618320610687, -43.8091603053435,
                                      -28.9160305343511, -23.7099236641221, -13.5801526717557, -8.83206106870229,
                                      -8.0, -3.79389312977099, -21.236641221374, -6.93129770992367,
                                      -39.5496183206107, -1.94656488549618, -38.8396946564886, -20.0229007633588,
                                      -16.4809160305344, -106.328244274809, -24.8854961832061, -54.6259541984733,
                                      -7.01526717557252, -51.5648854961832, -57.6335877862595, -8.34351145038168,
                                      -8.88549618320611, -176.152671755725, -85.1374045801527, -136.343511450382,
                                      -23.4427480916031, -16.412213740458, -5.61068702290076, -21.0763358778626,
                                      -129.908396946565, -143.832061068702, -46.6488549618321, -43.381679389313,
                                      -12.4885496183206, -10.2977099236641, -22.9083969465649, -41.3511450381679,
                                      -49.1603053435115, -47.1679389312977, -15.618320610687, -16.6793893129771,
                                      -18.8091603053435, -21.3129770992366, -21.2595419847328, -19.1526717557252,
                                      -32.2290076335878, -43.7022900763359, -32.4198473282443, -6.00763358778626,
                                      -92.8091603053435, -102.595419847328, -138.564885496183, -29.5190839694656,
                                      -34.5954198473282, -11.0076335877863, -6.04580152671756, -5.38931297709924,
                                      -25.236641221374, -59.5648854961832, -35.8778625954199, -75.8167938931298,
                                      -83.5267175572519, -47.4198473282443, -25.3893129770992, -19.9236641221374,
                                      -57.5114503816794, -59.0916030534351, -30.3129770992366, -24.5572519083969,
                                      -26.8320610687023, -39.9770992366412, -30.5648854961832, -31.3129770992366,
                                      -24.3053435114504, -32.2213740458015, -24.8091603053435, -20.1679389312977,
                                      -38.3740458015267, -36.3969465648855, -40.4351145038168, -32.1679389312977,
                                      -17.0381679389313, -20.7480916030534, -20.412213740458, -22.9007633587786,
                                      -12.0687022900763, -16.1679389312977, -58.0916030534351, -16.1145038167939,
                                      -16.7786259541985, -60.412213740458, -48.3129770992366, -22.5114503816794,
                                      -19.6641221374046, -17.206106870229, -35.4351145038168, -15.0839694656489,
                                      -36.5496183206107, -23.4809160305344, -40.3664122137405, -12.2290076335878,
                                      -31.587786259542, -48.4045801526718, -59.0381679389313, -28.1679389312977,
                                      -35.4427480916031, -28.1221374045802, -135.732824427481, -152.091603053435,
                                      -24.5572519083969, -25.5419847328244, -16.1526717557252, -31.1908396946565,
                                      -24.9923664122137, -27.7251908396947, -31.3282442748092, -41.5190839694657,
                                      -20.8167938931298, -28.381679389313, -18.7404580152672, -21.2900763358779,
                                      -46.5954198473282, -25.8931297709924, -60.1832061068702, -72.7557251908397,
                                      -48.5954198473282, -27.9770992366412, -27.5190839694656, -16.4580152671756,
                                      -48.2595419847328, -65.763358778626, -70.6030534351145, -65.7557251908397,
                                      -72.6564885496183, -62.030534351145, -55.5496183206107, -65.2824427480916,
                                      -58.9618320610687, -35.9465648854962, -49.0763358778626, -84.4198473282443,
                                      -47.3206106870229, -54.1221374045802, -74.7251908396947, -66.1068702290076,
                                      -49.9389312977099, -63.6335877862595, -65.6106870229008, -61.618320610687,
                                      -56.2137404580153, -44.2900763358779, -64.1984732824428, -68.2900763358779,
                                      -85.3435114503817, -77.6946564885496, -79.2748091603053, -44.236641221374,
                                      -66.175572519084, -49.9312977099237, -58.4580152671756, -50.1679389312977,
                                      -69.7709923664122, -58.2748091603053, -93.3053435114504, -73.5801526717557,
                                      -55.7709923664122, -66.2977099236641, -63.8320610687023, -31.2824427480916,
                                      -44.3740458015267, -80.4274809160305, -78.0763358778626, -70.5267175572519,
                                      -40.3587786259542, -47.2442748091603, -16.030534351145, -8.75572519083969,
                                      -16.3664122137405, -7.65648854961832, -8.12977099236641, -13.1603053435115,
                                      -10.1526717557252, -3.76335877862595, -10.0763358778626, -12.1908396946565,
                                      -73.2748091603053, -12.969465648855, -13.7786259541985, -13.824427480916,
                                      -14.9770992366412, -10.4961832061069, -16.4351145038168, -15.8549618320611,
                                      -6.38931297709924, -12.969465648855, -9.93893129770992, -9.3206106870229,
                                      -12.030534351145, -10.0687022900763, -14.2213740458015, -8.45801526717557,
                                      -5.83969465648855, -12.3893129770992, -44.8854961832061, -20.1374045801527,
                                      -30.2671755725191, -23.5496183206107, -55.6564885496183, -13.5267175572519,
                                      -30.3893129770992, -78.9389312977099, -20.5496183206107, -27.412213740458,
                                      -62.5038167938931, -16.5954198473282, -50.0381679389313, -2.61832061068702,
                                      -2.27480916030534, -2.80152671755725, -3.03053435114504, -6.61068702290076,
                                      -3.70229007633588, -33.030534351145, -148.068702290076, -118.549618320611,
                                      -15.0534351145038, -35.8549618320611, -54.5267175572519, -82.763358778626,
                                      -24.6335877862595, -82.0458015267176, -97.4580152671756, -111.893129770992,
                                      -175.977099236641, -8.41984732824428, -50.4656488549618, -66.1145038167939,
                                      -0.450381679389313, -1.04580152671756, -7.18320610687023, -17.030534351145,
                                      -64.7099236641221, -13.1603053435115, -17.5496183206107, -7.29007633587786,
                                      -93.7862595419847, -7.43511450381679, -7.79389312977099, -5.05343511450382,
                                      -6.89312977099237, -3.62595419847328, -1.29007633587786, -3.76335877862595,
                                      -3.00763358778626, -9.05343511450382, -5.29770992366412, -7.83969465648855,
                                      -0.106870229007634, -16.7480916030534, -16.3511450381679, -66.8625954198473,
                                      -18.618320610687, -26.7251908396947, -20.1908396946565, -21.1450381679389,
                                      -29.6946564885496, -76.381679389313, -44.3358778625954, -25.587786259542,
                                      -21.1679389312977, -6.32824427480916, -3.82442748091603, -9.23664122137405,
                                      -9.16793893129771, -3.9236641221374, -11.4274809160305, -53.8167938931298,
                                      -12.6259541984733, -6.0763358778626, -15.0610687022901, -43.0381679389313,
                                      -27.1832061068702, -4.06870229007634, -10.4198473282443, -35.7175572519084,
                                      -9.87022900763359, -23.9923664122137, -31.0076335877863, -62.9160305343511,
                                      -17.793893129771, -36.8549618320611, -17.8854961832061, -18.0076335877863,
                                      -19.5572519083969, -18.7328244274809, -2.25954198473282, -6.01526717557252,
                                      -63.7404580152672, -12.9312977099237, -34.6412213740458, -36.9083969465649,
                                      -30.0534351145038, -9.6030534351145, -26.6717557251908, -15.9541984732824,
                                      -29.9312977099237, -25.1221374045802, -43.9847328244275, -7.46564885496183,
                                      -8.86259541984733, -19.5496183206107, -4.22900763358779, -44.9312977099237,
                                      -39.2213740458015, -15.6641221374046, -50.7709923664122, -16.5038167938931,
                                      -29.5496183206107, -35.1603053435115, -44.9465648854962, -45.3511450381679,
                                      -16.1297709923664, -6.34351145038168, -19.1450381679389, -11.3893129770992,
                                      -20.9083969465649, -10.5496183206107, -14.0458015267176, -4.77862595419847,
                                      -3.0381679389313, -52.0687022900763, -9.12213740458015, -6.15267175572519,
                                      -16.2442748091603, -5.41221374045802, -16.7404580152672, -8.11450381679389,
                                      -14.5801526717557, -16.7786259541985, -24.1068702290076, -6.06106870229008,
                                      -36.7709923664122, -12.3206106870229, -3.12213740458015, -11.7557251908397,
                                      -37.7328244274809, -11.4656488549618, -10.1145038167939, -25.412213740458,
                                      -4.3206106870229, -10.2900763358779, -9.80152671755725, -17.1068702290076,
                                      -11.6030534351145, -14.4885496183206, -18.4274809160305, -31.0076335877863,
                                      -18.1145038167939, -9.64885496183206, -3.34351145038168, -26.5954198473282,
                                      -22.2595419847328, -0.931297709923664, -13.7328244274809, -22.8625954198473,
                                      -35.6335877862595, -56.9312977099237, -24.3587786259542, -18.763358778626,
                                      -28.2442748091603, -5.66412213740458, -11.1679389312977, -29.1145038167939,
                                      -8.92366412213741, -18.7251908396947, -7.52671755725191, -6.97709923664122,
                                      -1.66412213740458, -18.0839694656489, -10.9923664122137, -39.206106870229,
                                      -45.8931297709924, -13.9618320610687, -21.1603053435115, -54.7251908396947,
                                      -28.8396946564886, -65.3740458015267, -22.0916030534351, -11.2748091603053,
                                      -21.0458015267176, -37.1679389312977, -17.8396946564886, -4.06870229007634,
                                      -1.19083969465649, -12.7175572519084, -0.885496183206107, -0.946564885496183,
                                      -1.90839694656489, -6.10687022900763, -9.59541984732825, -7.26717557251908,
                                      -1.10687022900763, -22.4351145038168, -22.9770992366412, -39.3206106870229,
                                      -60.0763358778626, -44.2671755725191, -27.381679389313, -29.1832061068702,
                                      -13.381679389313, -32.1832061068702, -33.8931297709924, -5.25954198473283,
                                      -1.9618320610687, -8.87022900763359, -16.1679389312977, -15.6030534351145,
                                      -10.3740458015267, -15.4580152671756, -7.61068702290076, -33.4274809160305,
                                      -21.6106870229008, -3.25954198473282, -0.587786259541985,
                                      -0.0458015267175573, -0.152671755725191, -0.572519083969466,
                                      -0.137404580152672, -0.145038167938931, -0.0687022900763359,
                                      -0.549618320610687, 0.259541984732824, -12.5725190839695, -17.9160305343511,
                                      -5.16030534351145, -11.7480916030534, -10.3893129770992, -24.8473282442748,
                                      -44.0839694656489, -4.75572519083969, -45.2824427480916, -12.0763358778626,
                                      -32.6259541984733, -49.2671755725191, -22.5190839694656, -12.4809160305344,
                                      -13.1908396946565, -6.0381679389313, -19.3435114503817, -52.0610687022901,
                                      -98.7404580152672, -67.9312977099237, -25.763358778626, -3.45801526717557,
                                      -18.0, -29.3740458015267, -8.6412213740458, -11.0152671755725,
                                      6.91603053435115, -187.725190839695, -68.2977099236641, -48.2290076335878,
                                      -76.7862595419847, -32.8931297709924, -30.0152671755725, -19.1984732824427,
                                      -33.2900763358779, -10.2519083969466, -19.9389312977099, -87.1145038167939,
                                      -51.3358778625954, -26.793893129771, -43.9541984732824, -8.9618320610687,
                                      -71.2748091603053, -37.3511450381679, -4.52671755725191, -1.61068702290076,
                                      -10.1450381679389, -0.381679389312977, -58.2671755725191, -15.7175572519084,
                                      -48.7022900763359, -63.5648854961832, -91.5190839694657, -41.824427480916,
                                      -44.4961832061069, -23.6259541984733, -25.0839694656489, -21.2519083969466,
                                      -9.95419847328244, -1.51145038167939, -171.587786259542, -80.7328244274809,
                                      -35.206106870229, -56.1145038167939, -36.1908396946565, -48.3358778625954,
                                      -12.4885496183206, -36.0229007633588, -19.6870229007634, -26.2442748091603,
                                      -20.969465648855, -30.1450381679389, -7.75572519083969, -9.09923664122137,
                                      -0.709923664122137, -39.3587786259542, -1.72519083969466, -3.50381679389313,
                                      -147.206106870229, -23.5038167938931, -1.47328244274809, -11.6564885496183,
                                      -20.2442748091603, -1.9618320610687, -2.01526717557252, -4.80152671755725,
                                      -1.84732824427481, -4.90839694656489, -6.06870229007634, -15.8396946564886,
                                      -74.1374045801527, -10.9465648854962, -49.1603053435115, -31.8320610687023,
                                      -24.2137404580153, -2.93893129770992, -43.8091603053435, -23.9923664122137,
                                      -64.1221374045802, -41.9083969465649, -41.2824427480916, -18.6946564885496,
                                      -9.44274809160305, -16.3511450381679, -18.824427480916, -13.3587786259542,
                                      -17.9465648854962, -23.4351145038168, -29.9618320610687, -12.587786259542,
                                      -11.763358778626, -12.9389312977099, -16.1374045801527, -24.175572519084,
                                      -36.6717557251908, -25.2900763358779, -3.32824427480916, -18.3587786259542,
                                      -45.9312977099237, -40.1374045801527, -20.0687022900763, -37.2824427480916,
                                      -4.45038167938931, -1.41221374045802, -0.351145038167939, -12.5725190839695,
                                      -16.4427480916031, -4.30534351145038, -8.13740458015267, -13.2595419847328,
                                      -28.1145038167939, -31.7862595419847, -18.9312977099237, -19.2748091603053,
                                      -28.9389312977099, -27.7786259541985, -46.8473282442748, -25.7786259541985,
                                      -19.8625954198473, -44.0992366412214, -30.6564885496183, -29.4809160305344,
                                      -33.4427480916031, -49.1984732824428, -29.587786259542, -28.1374045801527,
                                      -19.6717557251908, -16.2824427480916, -46.0839694656489, -28.1145038167939,
                                      -34.3664122137405, -33.0687022900763, -29.3053435114504, -19.3053435114504,
                                      -19.4427480916031, -30.9618320610687, -26.5572519083969, -34.4198473282443,
                                      -32.8167938931298, -16.3511450381679, -24.7557251908397, -19.3969465648855,
                                      -36.4427480916031, -35.4198473282443, -26.9847328244275, -16.5648854961832,
                                      -27.5343511450382, -23.3129770992366, -26.7480916030534, -25.6870229007634,
                                      -17.8473282442748, -36.2290076335878, -46.3282442748092, -23.5114503816794,
                                      -22.5190839694656, -18.5114503816794, -25.5114503816794, -28.7099236641221,
                                      -28.8702290076336, -30.1984732824427, -36.5114503816794, -21.6946564885496,
                                      -36.4274809160305, -27.8549618320611, -21.9541984732824, -24.0458015267176,
                                      -27.6259541984733, -19.0763358778626, -18.7557251908397, -24.206106870229,
                                      -23.6412213740458, -15.1450381679389, -13.6793893129771, -25.5801526717557,
                                      -21.2290076335878, -24.0916030534351, -23.9847328244275, -27.7786259541985,
                                      -57.3358778625954, -53.824427480916, -66.6335877862595, -48.8320610687023,
                                      -42.3664122137405, -37.3129770992366, -35.2748091603053, -50.3664122137405,
                                      -30.7099236641221, -29.0534351145038, -33.0992366412214, -34.763358778626,
                                      -42.8091603053435, -23.6793893129771, -36.8091603053435, -38.824427480916,
                                      -37.3206106870229, -49.1603053435115, -54.2900763358779, -39.1832061068702,
                                      -45.6259541984733, -45.2824427480916, -55.5038167938931, -35.8625954198473,
                                      -41.1679389312977, -49.3129770992366, -47.0534351145038, -32.3282442748092,
                                      -26.3511450381679, -49.4732824427481, -29.3587786259542, -41.3358778625954,
                                      -33.2824427480916, -61.6564885496183, -36.3206106870229, -36.9465648854962,
                                      -32.0839694656489, -53.3740458015267, -33.4885496183206, -35.5267175572519,
                                      -41.4580152671756, -36.2977099236641, -34.0610687022901, -28.3511450381679,
                                      -28.3358778625954, -27.587786259542, -8.75572519083969, -8.44274809160305,
                                      -4.33587786259542, -9.38167938931298, -5.15267175572519, -11.1374045801527,
                                      -4.79389312977099, -4.55725190839695, -6.12213740458015, -54.1221374045802,
                                      -11.412213740458, -9.0763358778626, -5.69465648854962, -14.5419847328244,
                                      -5.58778625954199, -7.36641221374046, -8.58015267175573, -6.37404580152672,
                                      -3.87786259541985, -13.6488549618321, -8.15267175572519, -6.00763358778626,
                                      -5.2824427480916, -5.24427480916031, -16.236641221374, -3.56488549618321,
                                      -4.80152671755725, -34.969465648855, -15.5114503816794, -16.5572519083969,
                                      -19.2442748091603, -8.81679389312977, -32.3282442748092, -19.0687022900763,
                                      -25.3969465648855, -12.6946564885496, -29.3282442748092, -18.5801526717557,
                                      -16.6717557251908, -27.7251908396947, -2.82442748091603, -0.961832061068702,
                                      -1.10687022900763, -2.91603053435115, -1.41221374045802, -1.41984732824427,
                                      -37.5725190839695, -57.557251908397, -76.9923664122137, -25.618320610687,
                                      -79.7862595419847, -40.9618320610687, -23.1526717557252, -46.5267175572519,
                                      -29.6946564885496, -32.4809160305344, -58.9541984732824, -53.9236641221374,
                                      -76.206106870229, -57.793893129771, -75.2137404580153, 0.888190163645606,
                                      0.477022894655511, 0.693186936473068, 9.63188792300084, 11.5738709959803,
                                      3.4856336051392, 2.72606738527864, 4.95745549790352, 7.69812035811064,
                                      2.48124484021873, 1.13342713540229, 2.15403832969809, 1.68335756013155,
                                      1.08160154223258, 1.14072980809482, 1.10499973566811, 0.827637234889895,
                                      4.00519009478398, 3.55436766733508, 0.461942451004802, 0.761333190392558,
                                      23.1625507775424, 14.3580095650154, 12.4203282302727, 13.6446063028866,
                                      26.3115741381731, 37.0449654876303, 30.3684894073462, 16.5923963340527,
                                      28.1778835958953, 8.81727304471125, 6.50899338841002, 3.90595104764778,
                                      26.3752963104297, 2.86655197309236, 7.28167854905009, 2.47475453316431,
                                      1.67161629311527, 8.92333761540341, 23.1934288892274, 4.10999767409515,
                                      1.20523690723607, 3.64790463206621, 9.17753151281572, 4.84211416391281,
                                      1.46455509442764, 1.29930301855001, 5.45179809251797, 1.57372295131959,
                                      13.9320610477895, 11.2746825361002, 56.3599841884847, 33.1181083704019,
                                      20.0126540587544, 35.1743292228855, 14.6925923007419, 6.83095645469157,
                                      9.60525284586356, 1.74977418273829, 2.2335648279828, 2.51313199229541,
                                      4.87538911590911, 10.3132115850188, 23.6430797850804, 5.02748694315363,
                                      5.07910158258284, 9.19967367290259, 3.55252282055115, 7.17312773034287,
                                      17.6080397193606, 19.365908215872, 4.4727483202472, 0.699171636033001,
                                      6.58893606234403, 2.75589435102805, 1.61168161470493, 9.22954430529726,
                                      4.93200075341994, 17.2884585462236, 6.88623665218396, 22.9178460459387,
                                      19.3649077036278, 8.91767082057037, 9.06634379591728, 3.44651205018127,
                                      1.65215601141174, 5.7895164956014, 1.62137835855725, 23.8974912488782,
                                      3.27992980321444, 7.25134991823025, 3.24228946886945, 1.9028622030606,
                                      7.5197852968543, 8.5998693620714, 3.85548329932312, 5.37024392756056,
                                      1.08831512521996, 3.30425911729102, 1.69226500675785, 8.14876468119537,
                                      5.20168377843926, 2.23924502581734, 8.61368444391355, 14.654263905593,
                                      4.12707507495559, 1.08627858555403, 5.59631018816512, 15.8544399315764,
                                      3.57268219109619, 4.27494547219965, 2.61133423520753, 2.50713102194342,
                                      2.45878561727738, 1.68320177943395, 3.39991293416597, 3.36835034478583,
                                      13.1939161842106, 5.21101825595654, 6.98805660707179, 12.205820428302,
                                      2.06847693203652, 1.26639357826246, 3.98808025288992, 12.7167003917603,
                                      1.94520233707451, 11.4846383238243, 2.8137719365517, 36.7288241906299,
                                      7.62754340851681, 10.0652437213171, 14.7035543597252, 13.2355185873281,
                                      5.64896843294327, 3.45077009230403, 21.0633152201278, 1.9744596131452,
                                      4.36978730392248, 4.14351960711815, 1.57275993115941, 1.16724022407176,
                                      2.58423724750453, 3.35459489608601, 9.4480290329601, 11.6072586794985,
                                      11.3602072687711, 26.995356212436, 35.9080212695205, 10.5055586164523,
                                      11.5210516381543, 8.64787459998249, 9.23886794180459, 11.8436485983208,
                                      9.13015713140889, 6.52821731797718, 1.6962890149146, 2.28769596390387,
                                      2.06794160826475, 1.16921050372799, 0.183523897407195, 1.05840183282058,
                                      0.896288535625921, 0.595860083874398, 2.51546119327237, 0.955449561503199,
                                      1.72654138854694, 6.53949020287615, 22.7095041336577, 18.4676636461615,
                                      18.9205137347276, 4.73397548179602, 6.38617030338784, 13.9151846585429,
                                      8.87640861591396, 19.2076147176887, 6.23802389673518, 0.671972553486468,
                                      2.58449654664638, 6.2708192957677, 5.38495379786706, 2.08834091185235,
                                      3.60652892282335, 9.7492213324933, 6.57291360715356, 5.44071887056892,
                                      0.778775618006525, 0.567458550841094, 0.852094428236692, 1.17284306910664,
                                      0.278495211403889, 0.574601809665919, 0.344358582248549, 1.09618426928014,
                                      1.14506361294883, 0.692387873456488, 3.61527555703588, 5.01947724346677,
                                      3.50800527979402, 5.61764640826982, 6.74375529909411, 12.7485601230356,
                                      2.33502953212888, 12.3425389203663, 13.5297887368716, 10.7880583903813,
                                      10.6737813277072, 9.37336820287658, 14.9912022837477, 2.36423194316446,
                                      2.33044571182853, 3.09578456261981, 20.7812156599442, 5.82805305704706,
                                      14.8975582368218, 22.1222704256375, 6.6944649933008, 2.4103771150913,
                                      7.11818451459035, 9.98438557184984, 6.55601211075304, 4.91571640928978,
                                      8.7384277117907, 23.3858210476079, 13.8270511460292, 28.3111658907915,
                                      19.4774862974317, 6.57502323938478, 10.558101890463, 7.26833414974008,
                                      11.6078761572377, 4.55667008190202, 4.01863992095438, 5.29264777836179,
                                      3.01125445622775, 10.7548176820404, 27.5688721773054, 30.683086223509,
                                      28.2280425813184, 8.58346625864095, 2.24044175969759, 1.91937301593876,
                                      1.29532785415598, 5.78052143527145, 30.3879170013265, 23.9319332250619,
                                      12.6580377324243, 21.344688126221, 14.7932077453316, 21.9503269261483,
                                      8.49092105377691, 5.07707622449539, 9.40442215094795, 3.70412325749231,
                                      4.97538372532862, 1.22805899569908, 39.0503829030077, 28.2173525495862,
                                      20.2976869568583, 20.9424333272116, 3.6975810140098, 7.27264612236273,
                                      7.21944521793091, 3.52649449081402, 7.95230083882637, 16.4462721350648,
                                      2.82386006266541, 9.77064944531833, 2.72668721079341, 2.8885439697829,
                                      1.04348660044011, 1.49707871627072, 3.77598797577145, 3.64905457948339,
                                      2.20668789728435, 4.71165544022003, 2.30267526694514, 3.05271938176409,
                                      13.7135997043871, 2.5868515954897, 0.438914702640154, 2.88962304345318,
                                      0.861107952112862, 2.87998787623137, 1.06772037770417, 1.33958047402756,
                                      2.17305331578522, 2.59036328258888, 21.351004483622, 14.8160958025826,
                                      11.702431991491, 4.41023886704585, 12.733687066903, 8.64920867336247,
                                      5.55658602671148, 14.1533490454472, 18.2604387178003, 4.72007023164562,
                                      5.50392266787878, 17.181218749633, 5.97313489074642, 8.86496210780915,
                                      12.6063357740895, 6.29624317872237, 4.07410523065529, 1.80173695241147,
                                      15.7749693854148, 10.6937982884657, 5.18489219222965, 18.2820092699093,
                                      13.5461279237281, 7.37386009596581, 1.91688190081325, 12.8689747924189,
                                      4.47964792126727, 39.6116513235149, 8.94097798617255, 8.46326689553586,
                                      1.79207313880896, 1.00407073094911, 1.02744802188649, 1.46001222103314,
                                      7.28692630077208, 3.60500173744072, 5.33417395721576, 10.175171647613,
                                      20.5681268236781, 25.2365315431503, 6.1856040963066, 9.18694282227078,
                                      15.7617691378202, 10.5246941021996, 6.65583195865297, 9.0642964854842,
                                      11.4405581270383, 14.900591289208, 3.53110991801074, 6.26633405476193,
                                      12.3099336995925, 12.3535320729053, 13.5460676995008, 10.303304859138,
                                      16.2302319775433, 10.530093729801, 11.3987487262818, 3.1206345885761,
                                      2.26585151662476, 11.6179319030983, 12.0094654120703, 5.65696240971357,
                                      4.67917138329182, 11.7514292735585, 9.09427217407118, 10.1649268857834,
                                      15.9546678041928, 13.5139278360063, 10.9856876445014, 11.6481346009995,
                                      8.32715619007216, 10.425468941836, 8.44672959415543, 18.1479683683878,
                                      10.3072178108946, 17.6483679507016, 13.4960040228511, 16.2110699199473,
                                      3.53509299256141, 8.37923531156054, 13.5332704304105, 10.8937342359451,
                                      12.3029518635033, 10.4100140946237, 11.8899691044616, 6.31464297689469,
                                      14.867899170117, 9.44927480599983, 6.01041188170202, 12.6536843457432,
                                      12.9792027931375, 7.7122889577161, 9.31196966144028, 10.4165333208255,
                                      6.39882735746238, 8.44134342220676, 5.93711092806379, 15.0748767290796,
                                      9.62020156090159, 5.12084601643261, 5.97066132770647, 7.61256264835768,
                                      7.09571036119575, 10.7696127139949, 7.87238699581196, 11.9882282061851,
                                      19.6131932587427, 20.4988398636481, 16.0520629701837, 11.9908429992395,
                                      22.458582200513, 28.5389599241602, 10.3681405882453, 20.8752178851578,
                                      16.9293427377875, 13.7060085536873, 17.5367437476713, 11.7105531445008,
                                      11.028738066583, 17.6549323908384, 21.9310200506917, 22.3293925456681,
                                      16.7613720509601, 19.961423375321, 12.42595691839, 9.24281542429269,
                                      9.03678166591706, 14.406040452361, 16.2526720963343, 18.3481885311961,
                                      12.4201499468146, 24.7631175805499, 8.13357972639046, 17.1810305153156,
                                      22.0412097943814, 11.9952912114058, 19.0447887873441, 16.5777309746484,
                                      14.8031879827861, 8.2820768906426, 20.5860794578186, 18.0288876574165,
                                      11.529093329807, 18.1143429735221, 10.7861433898113, 13.1044959103549,
                                      11.0291818817442, 7.68082332231957, 18.6845935905359, 17.5578410123538,
                                      6.3425192819593, 6.78100243741959, 5.88508528184146, 7.07557824283127,
                                      1.72324757135472, 6.06711295142042, 4.25421703328404, 2.50824640751067,
                                      8.21505281947465, 4.32949616371715, 1.07752627583917, 2.90294213073445,
                                      8.06270946753083, 4.96579994020026, 5.78436594123444, 3.43768351772922,
                                      2.09926439953889, 11.2264645461486, 8.22103382315022, 7.30716593283938,
                                      2.62781755298534, 3.21859617575413, 8.43744916051907, 2.66239362986311,
                                      5.23546711961869, 1.1645663024511, 7.60492711208755, 2.521465448443,
                                      5.53395629020491, 14.5963081431433, 7.65110204532735, 3.93085300975521,
                                      9.25745557131109, 10.5291141977791, 11.7175870899985, 9.77271573542407,
                                      6.54359230963138, 5.87858625464996, 8.87574883108524, 6.99748970613878,
                                      3.64513208783521, 4.24310764334932, 1.01787531607837, 2.68816119192809,
                                      1.14386708561895, 1.42144701807583, 1.62754822770158, 1.67621142768624,
                                      5.47949650970629, 12.9355283867662, 52.234287395109, 1.80606557224051,
                                      2.05046750572078, 8.24718990023328, 31.9561113719713, 12.1526597683013,
                                      14.5189555383986, 4.41449134273086, 4.87140740487862, 15.1767666601304,
                                      2.18520700494233, 20.4424715930882, 13.5559924798413, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 19.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 18.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0,
                                      20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 18.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      17.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 16.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 18.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 16.0, 12.0, 10.0, 8.0, 14.0, 15.0, 20.0, 17.0, 17.0, 10.0,
                                      12.0, 17.0, 19.0, 13.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      17.0, 18.0, 16.0, 15.0, 18.0, 16.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 19.0, 19.0, 19.0, 15.0, 11.0, 16.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 18.0, 19.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 18.0, 10.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 17.0, 17.0, 15.0, 15.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 16.0, 20.0, 20.0, 20.0,
                                      15.0, 18.0, 17.0, 20.0, 14.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 17.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 19.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 9.0, 2.0, 9.0, 12.0, 6.0, 5.0, 8.0, 5.0,
                                      4.0, 3.0, 2.0, 4.0, 8.0, 8.0, 4.0, 6.0, 5.0, 0.0, 1.0, 2.0, 6.0, 1.0, 2.0,
                                      1.0, 1.0, 1.0, 3.0, 3.0, 0.0, 1.0, 0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 0.0, 3.0,
                                      14.0, 4.0, 4.0, 5.0, 6.0, 7.0, 6.0, 6.0, 3.0, 3.0, 4.0, 0.0, 0.0, 1.0, 0.0,
                                      7.0, 8.0, 3.0, 3.0, 5.0, 6.0, 4.0, 5.0, 6.0, 6.0, 10.0, 4.0, 4.0, 0.0, 0.0,
                                      1.0, 2.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 3.0, 1.0, 0.0, 0.0, 0.0, 1.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2.0, 1.0,
                                      1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 2.0, 4.0,
                                      4.0, 5.0, 1.0, 0.0, 3.0, 0.0, 2.0, 13.0, 3.0, 3.0, 4.0, 2.0, 8.0, 1.0, 0.0,
                                      0.0, 2.0, 0.0, 1.0, 1.0, 2.0, 1.0, 12.0, 6.0, 2.0, 4.0, 8.0, 5.0, 4.0, 3.0,
                                      3.0, 0.0, 6.0, 1.0, 0.0, 2.0, 1.0, 6.0, 7.0, 3.0, 0.0, 6.0, 6.0, 5.0, 5.0,
                                      3.0, 5.0, 4.0, 6.0, 2.0, 7.0, 1.0, 2.0, 3.0, 2.0, 4.0, 3.0, 0.0, 0.0, 0.0,
                                      2.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 2.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 3.0, 2.0, 1.0,
                                      1.0, 1.0, 10.0, 2.0, 1.0, 2.0, 8.0, 0.0, 2.0, 3.0, 0.0, 1.0, 8.0, 3.0, 0.0,
                                      2.0, 3.0, 0.0, 0.0, 2.0, 2.0, 4.0, 0.0, 1.0, 0.0, 8.0, 2.0, 1.0, 2.0, 4.0,
                                      0.0, 0.0, 0.0, 8.0, 3.0, 2.0, 4.0, 1.0, 2.0, 3.0, 2.0, 1.0, 1.0, 0.0, 1.0,
                                      0.0, 9.0, 0.0, 1.0, 7.0, 0.0, 2.0, 1.0, 2.0, 2.0, 7.0, 1.0, 1.0, 1.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
                                      3.0, 5.0, 2.0, 1.0, 7.0, 4.0, 0.0, 7.0, 3.0, 1.0, 7.0, 6.0, 1.0, 1.0, 9.0,
                                      0.0, 2.0, 3.0, 2.0, 1.0, 0.0, 7.0, 3.0, 2.0, 5.0, 6.0, 6.0, 0.0, 7.0, 2.0,
                                      5.0, 3.0, 6.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 2.0, 1.0, 2.0, 1.0, 6.0, 7.0,
                                      2.0, 3.0, 5.0, 5.0, 1.0, 2.0, 7.0, 4.0, 3.0, 4.0, 3.0, 5.0, 2.0, 4.0, 1.0,
                                      2.0, 0.0, 3.0, 0.0, 3.0, 2.0, 1.0, 3.0, 3.0, 3.0, 0.0, 5.0, 3.0, 6.0, 4.0,
                                      2.0, 2.0, 7.0, 2.0, 2.0, 2.0, 4.0, 4.0, 0.0, 4.0, 4.0, 6.0, 0.0, 5.0, 3.0,
                                      5.0, 2.0, 3.0, 1.0, 5.0, 4.0, 2.0, 1.0, 2.0, 5.0, 4.0, 3.0, 0.0, 3.0, 5.0,
                                      3.0, 1.0, 4.0, 5.0, 10.0, 10.0, 12.0, 9.0, 10.0, 10.0, 10.0, 10.0, 11.0,
                                      11.0, 9.0, 10.0, 10.0, 9.0, 11.0, 9.0, 11.0, 10.0, 11.0, 8.0, 11.0, 10.0,
                                      8.0, 11.0, 9.0, 11.0, 8.0, 10.0, 9.0, 10.0, 9.0, 9.0, 8.0, 9.0, 8.0, 11.0,
                                      9.0, 11.0, 9.0, 10.0, 7.0, 11.0, 4.0, 3.0, 0.0, 1.0, 5.0, 5.0, 0.0, 5.0, 3.0,
                                      0.0, 5.0, 4.0, 0.0, 7.0, 6.0, 4.0, 5.0, 0.0, 4.0, 2.0, 5.0, 2.0, 0.0, 3.0,
                                      2.0, 0.0, 6.0, 3.0, 0.0, 4.0, 1.0, 6.0, 11.0, 14.0, 11.0, 13.0, 12.0, 9.0,
                                      12.0, 13.0, 12.0, 14.0, 13.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 9.0,
                                      5.0, 1.0, 0.0, 2.0, 2.0, 4.0, 3.0, 3.0, 7.0, 5.0, 0.0, 2.0, 3.0, 20.0, 20.0,
                                      20.0, 13.0, 13.0, 4.0, 12.0, 7.0, 5.0, 13.0, 12.0, 10.0, 10.0, 14.0, 20.0,
                                      18.0, 18.0, 12.0, 11.0, 14.0, 14.0, 20.0, 14.0, 11.0, 13.0, 12.0, 9.0, 11.0,
                                      12.0, 12.0, 12.0, 13.0, 10.0, 9.0, 12.0, 10.0, 11.0, 9.0, 11.0, 6.0, 11.0,
                                      19.0, 18.0, 3.0, 5.0, 18.0, 16.0, 12.0, 10.0, 19.0, 13.0, 13.0, 10.0, 10.0,
                                      6.0, 14.0, 9.0, 8.0, 8.0, 4.0, 12.0, 12.0, 20.0, 8.0, 13.0, 14.0, 16.0, 9.0,
                                      14.0, 11.0, 14.0, 5.0, 16.0, 16.0, 8.0, 13.0, 12.0, 15.0, 8.0, 9.0, 7.0,
                                      11.0, 14.0, 12.0, 15.0, 17.0, 11.0, 13.0, 1.0, 15.0, 11.0, 11.0, 12.0, 11.0,
                                      12.0, 12.0, 12.0, 12.0, 11.0, 17.0, 8.0, 9.0, 11.0, 11.0, 12.0, 14.0, 19.0,
                                      15.0, 17.0, 12.0, 18.0, 15.0, 14.0, 12.0, 18.0, 18.0, 6.0, 10.0, 13.0, 13.0,
                                      7.0, 11.0, 15.0, 14.0, 7.0, 2.0, 4.0, 15.0, 15.0, 11.0, 10.0, 10.0, 10.0,
                                      12.0, 14.0, 8.0, 14.0, 16.0, 6.0, 9.0, 6.0, 6.0, 15.0, 13.0, 19.0, 9.0, 6.0,
                                      15.0, 12.0, 15.0, 4.0, 7.0, 17.0, 15.0, 3.0, 12.0, 4.0, 12.0, 18.0, 18.0,
                                      20.0, 8.0, 12.0, 16.0, 16.0, 20.0, 16.0, 7.0, 3.0, 12.0, 14.0, 16.0, 7.0,
                                      13.0, 15.0, 7.0, 16.0, 17.0, 9.0, 13.0, 13.0, 12.0, 15.0, 12.0, 18.0, 20.0,
                                      20.0, 20.0, 20.0, 18.0, 19.0, 19.0, 19.0, 19.0, 13.0, 8.0, 11.0, 9.0, 15.0,
                                      15.0, 10.0, 4.0, 15.0, 10.0, 16.0, 10.0, 12.0, 7.0, 16.0, 15.0, 15.0, 7.0,
                                      19.0, 15.0, 6.0, 7.0, 17.0, 12.0, 9.0, 12.0, 12.0, 16.0, 19.0, 17.0, 8.0,
                                      7.0, 6.0, 12.0, 11.0, 10.0, 12.0, 12.0, 12.0, 12.0, 14.0, 18.0, 12.0, 6.0,
                                      8.0, 11.0, 20.0, 15.0, 16.0, 17.0, 9.0, 5.0, 12.0, 12.0, 8.0, 15.0, 12.0,
                                      7.0, 9.0, 13.0, 19.0, 14.0, 20.0, 4.0, 8.0, 10.0, 9.0, 12.0, 7.0, 12.0, 10.0,
                                      13.0, 8.0, 14.0, 14.0, 19.0, 12.0, 9.0, 9.0, 17.0, 8.0, 16.0, 15.0, 10.0,
                                      13.0, 16.0, 14.0, 14.0, 18.0, 12.0, 20.0, 15.0, 12.0, 5.0, 8.0, 8.0, 15.0,
                                      16.0, 10.0, 12.0, 9.0, 6.0, 9.0, 17.0, 12.0, 11.0, 6.0, 9.0, 8.0, 14.0, 8.0,
                                      20.0, 9.0, 9.0, 8.0, 3.0, 11.0, 19.0, 18.0, 12.0, 3.0, 9.0, 7.0, 18.0, 18.0,
                                      20.0, 18.0, 7.0, 10.0, 16.0, 13.0, 10.0, 10.0, 9.0, 12.0, 10.0, 10.0, 11.0,
                                      13.0, 11.0, 8.0, 11.0, 7.0, 12.0, 11.0, 10.0, 12.0, 9.0, 9.0, 10.0, 12.0,
                                      11.0, 9.0, 10.0, 10.0, 9.0, 6.0, 13.0, 9.0, 9.0, 11.0, 11.0, 10.0, 8.0, 10.0,
                                      9.0, 11.0, 10.0, 9.0, 11.0, 10.0, 10.0, 7.0, 9.0, 14.0, 9.0, 10.0, 9.0, 10.0,
                                      12.0, 9.0, 11.0, 10.0, 14.0, 6.0, 10.0, 8.0, 10.0, 9.0, 12.0, 12.0, 10.0,
                                      11.0, 10.0, 9.0, 13.0, 12.0, 9.0, 7.0, 12.0, 12.0, 12.0, 9.0, 11.0, 14.0,
                                      7.0, 13.0, 8.0, 9.0, 16.0, 8.0, 10.0, 11.0, 8.0, 9.0, 11.0, 13.0, 9.0, 14.0,
                                      10.0, 10.0, 11.0, 13.0, 12.0, 11.0, 14.0, 9.0, 14.0, 12.0, 9.0, 13.0, 10.0,
                                      12.0, 14.0, 7.0, 13.0, 10.0, 9.0, 10.0, 12.0, 11.0, 16.0, 18.0, 11.0, 5.0,
                                      11.0, 15.0, 6.0, 8.0, 17.0, 11.0, 8.0, 12.0, 12.0, 12.0, 13.0, 7.0, 18.0,
                                      10.0, 9.0, 11.0, 8.0, 19.0, 12.0, 7.0, 19.0, 11.0, 8.0, 15.0, 11.0, 11.0,
                                      12.0, 11.0, 13.0, 12.0, 11.0, 13.0, 12.0, 11.0, 11.0, 11.0, 12.0, 10.0, 10.0,
                                      10.0, 18.0, 18.0, 20.0, 19.0, 17.0, 19.0, 15.0, 10.0, 3.0, 13.0, 20.0, 6.0,
                                      5.0, 9.0, 11.0, 17.0, 13.0, 5.0, 18.0, 12.0, 5.0, 16.0, 18.0, 15.0, 8.0,
                                      16.0, 9.0, 11.0, 4.0, 13.0, 10.0, 12.0, 12.0, 14.0, 11.0, 16.0, 18.0, 16.0,
                                      13.0, 13.0, 15.0, 18.0, 18.0, 11.0, 7.0, 11.0, 13.0, 5.0, 9.0, 5.0, 2.0, 7.0,
                                      12.0, 18.0, 20.0, 9.0, 4.0, 11.0, 16.0, 14.0, 13.0, 8.0, 15.0, 10.0, 9.0,
                                      9.0, 12.0, 16.0, 7.0, 12.0, 18.0, 13.0, 0.0, 15.0, 10.0, 20.0, 10.0, 11.0,
                                      10.0, 8.0, 5.0, 9.0, 7.0, 0.0, 14.0, 16.0, 9.0, 11.0, 10.0, 13.0, 9.0, 8.0,
                                      17.0, 7.0, 0.0, 9.0, 11.0, 10.0, 18.0, 6.0, 12.0, 11.0, 7.0, 11.0, 13.0, 7.0,
                                      13.0, 13.0, 15.0, 20.0, 12.0, 7.0, 10.0, 20.0, 9.0, 11.0, 16.0, 7.0, 8.0,
                                      8.0, 17.0, 7.0, 9.0, 16.0, 20.0, 15.0, 13.0, 15.0, 8.0, 7.0, 7.0, 11.0, 10.0,
                                      11.0, 14.0, 13.0, 4.0, 14.0, 10.0, 8.0, 10.0, 16.0, 11.0, 17.0, 7.0, 19.0,
                                      9.0, 5.0, 14.0, 13.0, 3.0, 17.0, 6.0, 5.0, 12.0, 16.0, 19.0, 12.0, 4.0, 19.0,
                                      11.0, 7.0, 4.0, 8.0, 8.0, 13.0, 11.0, 11.0, 0.0, 10.0, 13.0, 18.0, 9.0, 5.0,
                                      15.0, 12.0, 15.0, 6.0, 7.0, 18.0, 16.0, 17.0, 15.0, 15.0, 11.0, 13.0, 18.0,
                                      14.0, 12.0, 4.0, 0.0, 2.0, 10.0, 19.0, 6.0, 6.0, 18.0, 12.0, 19.0, 11.0,
                                      10.0, 9.0, 15.0, 18.0, 16.0, 1.0, 20.0, 15.0, 20.0, 17.0, 19.0, 16.0, 19.0,
                                      19.0, 16.0, 18.0, 17.0, 20.0, 7.0, 10.0, 14.0, 10.0, 19.0, 4.0, 4.0, 10.0,
                                      20.0, 5.0, 12.0, 11.0, 12.0, 16.0, 13.0, 5.0, 3.0, 13.0, 12.0, 9.0, 19.0,
                                      11.0, 7.0, 11.0, 16.0, 12.0, 2.0, 11.0, 14.0, 8.0, 2.0, 11.0, 16.0, 9.0,
                                      10.0, 12.0, 17.0, 15.0, 5.0, 5.0, 12.0, 15.0, 9.0, 17.0, 11.0, 16.0, 8.0,
                                      6.0, 9.0, 5.0, 14.0, 9.0, 14.0, 13.0, 7.0, 13.0, 8.0, 13.0, 12.0, 4.0, 6.0,
                                      8.0, 12.0, 15.0, 8.0, 13.0, 20.0, 3.0, 16.0, 11.0, 12.0, 10.0, 16.0, 12.0,
                                      12.0, 11.0, 4.0, 7.0, 19.0, 10.0, 11.0, 19.0, 18.0, 8.0, 15.0, 11.0, 17.0,
                                      16.0, 13.0, 12.0, 11.0, 13.0, 9.0, 16.0, 15.0, 10.0, 10.0, 14.0, 13.0, 9.0,
                                      15.0, 16.0, 9.0, 10.0, 12.0, 7.0, 12.0, 11.0, 17.0, 14.0, 6.0, 12.0, 12.0,
                                      8.0, 16.0, 17.0, 9.0, 9.0, 15.0, 8.0, 12.0, 16.0, 16.0, 18.0, 15.0, 11.0,
                                      5.0, 14.0, 10.0, 7.0, 15.0, 14.0, 11.0, 8.0, 14.0, 10.0, 13.0, 9.0, 11.0,
                                      12.0, 8.0, 12.0, 10.0, 12.0, 8.0, 11.0, 9.0, 9.0, 13.0, 13.0, 9.0, 12.0,
                                      14.0, 7.0, 11.0, 11.0, 11.0, 6.0, 10.0, 9.0, 9.0, 11.0, 9.0, 14.0, 14.0,
                                      11.0, 8.0, 9.0, 11.0, 13.0, 4.0, 8.0, 11.0, 13.0, 7.0, 9.0, 14.0, 11.0, 13.0,
                                      7.0, 9.0, 16.0, 12.0, 12.0, 12.0, 12.0, 8.0, 11.0, 10.0, 7.0, 13.0, 11.0,
                                      7.0, 8.0, 14.0, 13.0, 10.0, 9.0, 14.0, 10.0, 9.0, 7.0, 9.0, 11.0, 7.0, 7.0,
                                      12.0, 10.0, 8.0, 11.0, 10.0, 9.0, 8.0, 12.0, 12.0, 9.0, 13.0, 10.0, 9.0,
                                      11.0, 7.0, 11.0, 7.0, 9.0, 10.0, 12.0, 9.0, 8.0, 13.0, 11.0, 10.0, 12.0, 8.0,
                                      13.0, 7.0, 11.0, 12.0, 12.0, 10.0, 14.0, 12.0, 7.0, 11.0, 12.0, 14.0, 10.0,
                                      14.0, 12.0, 9.0, 11.0, 16.0, 11.0, 5.0, 7.0, 16.0, 15.0, 10.0, 10.0, 8.0,
                                      12.0, 12.0, 15.0, 16.0, 12.0, 9.0, 12.0, 13.0, 9.0, 14.0, 12.0, 14.0, 10.0,
                                      10.0, 11.0, 9.0, 10.0, 11.0, 9.0, 12.0, 13.0, 7.0, 12.0, 10.0, 13.0, 13.0,
                                      15.0, 14.0, 10.0, 15.0, 19.0, 16.0, 10.0, 7.0, 7.0, 8.0, 10.0, 10.0, 13.0,
                                      18.0, 15.0, 8.0, 5.0, 6.0, 12.0, 20.0, 18.0, 17.0, 13.0, 9.0, 5.0, 10.0,
                                      12.0, 12.0, 10.0, 13.0, 8.0, 13.0, 17.0, 15.0, 13.0, 13.0, 6.0, 14.0, 11.0,
                                      16.0, 15.0, 15.0, 9.0, 7.0, 11.0, 9.0, 11.0, 5.0, 13.0, 9.0, 10.0, 12.0, 7.0,
                                      6.0, 9.0, 15.0, 9.0, 15.0, 15.0, 14.0, 17.0, 5.0, 4.0, 17.0, 9.0, 12.0, 11.0,
                                      15.0, 14.0, 12.0, 8.0, 9.0, 12.0, 8.0, 10.0, 17.0, 11.0, 7.0, 9.0, 9.0, 18.0,
                                      6.0, 12.0, 9.0, 11.0, 12.0, 15.0, 10.0, 6.0, 8.0, 12.0, 18.0, 12.0, 11.0,
                                      6.0, 16.0, 17.0, 13.0, 8.0, 9.0, 10.0, 11.0, 15.0, 11.0, 10.0, 12.0, 14.0,
                                      11.0, 11.0, 13.0, 8.0, 10.0, 8.0, 12.0, 6.0, 12.0, 10.0, 14.0, 11.0, 17.0,
                                      5.0, 6.0, 9.0, 11.0, 12.0, 7.0, 15.0, 7.0, 16.0, 14.0, 7.0, 9.0, 15.0, 16.0,
                                      6.0, 10.0, 6.0, 14.0, 9.0, 11.0, 15.0, 11.0, 15.0, 4.0, 3.0, 17.0, 16.0, 7.0,
                                      11.0, 10.0, 8.0, 11.0, 14.0, 10.0, 13.0, 11.0, 7.0, 11.0, 9.0, 4.0, 7.0,
                                      10.0, 19.0, 16.0, 6.0, 18.0, 11.0, 11.0, 9.0, 11.0, 16.0, 11.0, 14.0, 13.0,
                                      4.0, 10.0, 15.0, 13.0, 15.0, 14.0, 12.0, 12.0, 17.0, 19.0, 12.0, 11.0, 15.0,
                                      20.0, 13.0, 15.0, 13.0, 9.0, 10.0, 6.0, 13.0, 12.0, 8.0, 11.0, 10.0, 6.0,
                                      5.0, 13.0, 16.0, 17.0, 19.0, 9.0, 16.0, 18.0, 19.0, 14.0, 16.0, 14.0, 4.0,
                                      9.0, 15.0, 10.0, 14.0, 9.0, 7.0, 15.0, 6.0, 14.0, 5.0, 9.0, 14.0, 12.0, 11.0,
                                      14.0, 3.0, 2.0, 17.0, 10.0, 17.0, 12.0, 13.0, 9.0, 13.0, 9.0, 10.0, 0.0,
                                      15.0, 8.0, 15.0, 18.0, 11.0, 9.0, 8.0, 12.0, 10.0, 9.0, 9.0, 7.0, 1.0, 17.0,
                                      3.0, 14.0, 16.0, 8.0, 15.0, 11.0, 1.0, 13.0, 8.0, 18.0, 6.0, 15.0, 10.0,
                                      14.0, 11.0, 8.0, 8.0, 10.0, 3.0, 6.0, 8.0, 8.0, 6.0, 13.0, 11.0, 5.0, 17.0,
                                      10.0, 8.0, 6.0, 12.0, 8.0, 13.0, 7.0, 12.0, 12.0, 6.0, 11.0, 8.0, 14.0, 13.0,
                                      9.0, 11.0, 10.0, 13.0, 13.0, 11.0, 14.0, 16.0, 15.0, 12.0, 11.0, 8.0, 11.0,
                                      11.0, 16.0, 12.0, 9.0, 8.0, 8.0, 9.0, 11.0, 16.0, 12.0, 6.0, 7.0, 12.0, 9.0,
                                      8.0, 20.0, 11.0, 11.0, 9.0, 6.0, 9.0, 19.0, 15.0, 14.0, 8.0, 9.0, 12.0, 10.0,
                                      11.0, 14.0, 12.0, 10.0, 6.0, 10.0, 10.0, 7.0, 10.0, 9.0, 9.0, 12.0, 7.0,
                                      12.0, 12.0, 10.0, 11.0, 7.0, 11.0, 10.0, 9.0, 11.0, 11.0, 12.0, 13.0, 10.0,
                                      11.0, 12.0, 9.0, 11.0, 9.0, 10.0, 13.0, 9.0, 10.0, 8.0, 10.0, 11.0, 9.0,
                                      12.0, 11.0, 10.0, 8.0, 13.0, 11.0, 10.0, 10.0, 11.0, 9.0, 12.0, 12.0, 11.0,
                                      11.0, 13.0, 7.0, 7.0, 9.0, 12.0, 11.0, 9.0, 8.0, 10.0, 14.0, 9.0, 13.0, 11.0,
                                      9.0, 9.0, 8.0, 11.0, 10.0, 11.0, 8.0, 11.0, 10.0, 11.0, 10.0, 7.0, 12.0,
                                      11.0, 11.0, 10.0, 9.0, 13.0, 12.0, 8.0, 12.0, 14.0, 9.0, 11.0, 15.0, 10.0,
                                      9.0, 12.0, 10.0, 10.0, 11.0, 13.0, 10.0, 9.0, 12.0, 11.0, 10.0, 10.0, 7.0,
                                      9.0, 12.0, 10.0, 10.0, 13.0, 11.0, 10.0, 10.0, 11.0, 11.0, 8.0, 11.0, 11.0,
                                      8.0, 10.0, 7.0, 11.0, 14.0, 8.0, 12.0, 12.0, 10.0, 6.0, 12.0, 12.0, 11.0,
                                      8.0, 8.0, 18.0, 10.0, 11.0, 8.0, 8.0, 18.0, 10.0, 8.0, 12.0, 14.0, 7.0, 12.0,
                                      17.0, 10.0, 9.0, 10.0, 10.0, 10.0, 11.0, 9.0, 15.0, 10.0, 11.0, 9.0, 10.0,
                                      11.0, 11.0, 10.0, 12.0, 8.0, 8.0, 14.0, 11.0, 14.0, 10.0, 17.0, 11.0, 8.0,
                                      15.0, 3.0, 11.0, 7.0, 11.0, 9.0, 16.0, 11.0, 13.0, 11.0, 8.0, 1.0, 1.0, 2.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                                      0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 1.0, 3.0, 1.0, 0.0, 5.0, 2.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 5.0, 1.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 2.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 3.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 4.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.0,
                                      0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 1.0, 1.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 2.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 3.0, 0.0, 0.0, 1.0, 0.0, 0.0, 3.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0,
                                      2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 2.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 0.0, 1.0, 7.0, 8.0, 7.0, 8.0, 6.0, 8.0, 7.0, 7.0, 7.0, 5.0,
                                      8.0, 6.0, 6.0, 6.0, 6.0, 7.0, 6.0, 7.0, 7.0, 7.0, 6.0, 6.0, 6.0, 6.0, 6.0,
                                      6.0, 8.0, 6.0, 5.0, 7.0, 5.0, 3.0, 3.0, 3.0, 6.0, 5.0, 7.0, 5.0, 6.0, 4.0,
                                      5.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
                                      1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0, 3.0, 8.0, 5.0, 8.0, 6.0, 7.0, 6.0, 7.0, 5.0, 8.0, 5.0, 6.0,
                                      3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                      0.0, 0.0, 1.0, 2.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 6.0, 7.0, 16.0, 8.0, 13.0,
                                      15.0, 7.0, 8.0, 10.0, 8.0, 6.0, 0.0, 1.0, 2.0, 8.0, 9.0, 4.0, 6.0, 0.0, 6.0,
                                      9.0, 7.0, 8.0, 11.0, 9.0, 8.0, 8.0, 8.0, 7.0, 10.0, 10.0, 8.0, 8.0, 8.0,
                                      10.0, 9.0, 14.0, 9.0, 1.0, 1.0, 17.0, 15.0, 2.0, 4.0, 8.0, 9.0, 1.0, 7.0,
                                      7.0, 10.0, 10.0, 14.0, 6.0, 11.0, 12.0, 11.0, 15.0, 8.0, 8.0, 0.0, 11.0, 7.0,
                                      6.0, 4.0, 11.0, 6.0, 9.0, 6.0, 15.0, 4.0, 4.0, 10.0, 7.0, 8.0, 5.0, 12.0,
                                      10.0, 13.0, 9.0, 6.0, 8.0, 5.0, 3.0, 9.0, 7.0, 19.0, 5.0, 7.0, 8.0, 8.0, 9.0,
                                      8.0, 8.0, 8.0, 8.0, 8.0, 3.0, 12.0, 10.0, 7.0, 9.0, 7.0, 6.0, 1.0, 5.0, 3.0,
                                      8.0, 2.0, 5.0, 6.0, 8.0, 2.0, 2.0, 14.0, 9.0, 7.0, 6.0, 13.0, 9.0, 3.0, 6.0,
                                      13.0, 17.0, 16.0, 4.0, 5.0, 9.0, 10.0, 10.0, 10.0, 7.0, 6.0, 12.0, 6.0, 4.0,
                                      14.0, 8.0, 14.0, 14.0, 3.0, 7.0, 1.0, 11.0, 14.0, 5.0, 8.0, 5.0, 16.0, 13.0,
                                      3.0, 5.0, 16.0, 8.0, 16.0, 8.0, 2.0, 2.0, 0.0, 12.0, 8.0, 4.0, 3.0, 0.0, 4.0,
                                      13.0, 17.0, 8.0, 6.0, 4.0, 13.0, 7.0, 5.0, 12.0, 4.0, 3.0, 10.0, 7.0, 7.0,
                                      6.0, 5.0, 7.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 6.0, 12.0,
                                      9.0, 10.0, 5.0, 5.0, 10.0, 15.0, 5.0, 10.0, 3.0, 9.0, 8.0, 13.0, 3.0, 5.0,
                                      5.0, 13.0, 1.0, 4.0, 14.0, 13.0, 3.0, 8.0, 11.0, 8.0, 8.0, 4.0, 1.0, 3.0,
                                      12.0, 13.0, 13.0, 8.0, 9.0, 9.0, 8.0, 8.0, 7.0, 8.0, 5.0, 2.0, 8.0, 14.0,
                                      12.0, 9.0, 0.0, 5.0, 4.0, 3.0, 11.0, 15.0, 8.0, 8.0, 12.0, 5.0, 8.0, 13.0,
                                      9.0, 7.0, 1.0, 5.0, 0.0, 16.0, 12.0, 10.0, 11.0, 8.0, 13.0, 8.0, 9.0, 6.0,
                                      12.0, 6.0, 6.0, 1.0, 4.0, 10.0, 9.0, 3.0, 12.0, 1.0, 5.0, 10.0, 7.0, 4.0,
                                      5.0, 5.0, 2.0, 8.0, 0.0, 3.0, 8.0, 15.0, 12.0, 12.0, 5.0, 4.0, 10.0, 8.0,
                                      11.0, 14.0, 9.0, 3.0, 8.0, 9.0, 14.0, 11.0, 12.0, 5.0, 12.0, 0.0, 10.0, 11.0,
                                      12.0, 17.0, 9.0, 0.0, 2.0, 8.0, 17.0, 11.0, 13.0, 2.0, 2.0, 0.0, 2.0, 13.0,
                                      9.0, 4.0, 7.0, 10.0, 10.0, 11.0, 8.0, 10.0, 10.0, 9.0, 7.0, 9.0, 12.0, 8.0,
                                      13.0, 8.0, 9.0, 9.0, 8.0, 10.0, 11.0, 10.0, 7.0, 9.0, 11.0, 10.0, 10.0, 11.0,
                                      13.0, 7.0, 10.0, 11.0, 9.0, 9.0, 10.0, 11.0, 10.0, 11.0, 9.0, 10.0, 11.0,
                                      9.0, 10.0, 9.0, 13.0, 11.0, 6.0, 11.0, 9.0, 11.0, 9.0, 8.0, 11.0, 8.0, 9.0,
                                      6.0, 13.0, 10.0, 12.0, 10.0, 10.0, 8.0, 8.0, 10.0, 9.0, 9.0, 11.0, 7.0, 8.0,
                                      11.0, 13.0, 8.0, 8.0, 8.0, 11.0, 9.0, 6.0, 13.0, 7.0, 12.0, 11.0, 4.0, 12.0,
                                      10.0, 9.0, 11.0, 10.0, 9.0, 7.0, 11.0, 6.0, 10.0, 10.0, 9.0, 7.0, 8.0, 8.0,
                                      6.0, 11.0, 6.0, 8.0, 11.0, 7.0, 8.0, 8.0, 6.0, 13.0, 7.0, 10.0, 11.0, 9.0,
                                      8.0, 9.0, 4.0, 2.0, 9.0, 14.0, 8.0, 5.0, 13.0, 12.0, 3.0, 9.0, 12.0, 8.0,
                                      7.0, 8.0, 7.0, 13.0, 2.0, 10.0, 11.0, 9.0, 12.0, 1.0, 8.0, 13.0, 1.0, 9.0,
                                      12.0, 5.0, 9.0, 9.0, 8.0, 9.0, 7.0, 8.0, 9.0, 7.0, 8.0, 9.0, 9.0, 9.0, 8.0,
                                      10.0, 10.0, 9.0, 2.0, 2.0, 0.0, 0.0, 3.0, 1.0, 5.0, 10.0, 17.0, 5.0, 0.0,
                                      14.0, 15.0, 10.0, 9.0, 2.0, 7.0, 15.0, 2.0, 8.0, 15.0, 1.0, 0.0, 4.0, 12.0,
                                      4.0, 11.0, 9.0, 16.0, 7.0, 10.0, 6.0, 8.0, 4.0, 8.0, 1.0, 1.0, 4.0, 7.0, 6.0,
                                      3.0, 0.0, 2.0, 9.0, 13.0, 9.0, 7.0, 15.0, 11.0, 14.0, 18.0, 13.0, 8.0, 2.0,
                                      0.0, 11.0, 15.0, 8.0, 4.0, 6.0, 7.0, 11.0, 4.0, 10.0, 11.0, 11.0, 4.0, 2.0,
                                      12.0, 7.0, 2.0, 7.0, 20.0, 5.0, 10.0, 0.0, 10.0, 9.0, 10.0, 10.0, 14.0, 10.0,
                                      11.0, 20.0, 6.0, 3.0, 10.0, 9.0, 9.0, 7.0, 10.0, 12.0, 3.0, 13.0, 20.0, 10.0,
                                      9.0, 10.0, 1.0, 14.0, 7.0, 9.0, 13.0, 9.0, 7.0, 12.0, 6.0, 6.0, 5.0, 0.0,
                                      8.0, 13.0, 10.0, 0.0, 11.0, 9.0, 4.0, 11.0, 10.0, 10.0, 3.0, 13.0, 11.0, 4.0,
                                      0.0, 5.0, 7.0, 4.0, 12.0, 13.0, 13.0, 8.0, 9.0, 9.0, 6.0, 5.0, 16.0, 5.0,
                                      10.0, 12.0, 9.0, 4.0, 8.0, 1.0, 13.0, 1.0, 11.0, 15.0, 4.0, 7.0, 15.0, 3.0,
                                      14.0, 15.0, 8.0, 3.0, 1.0, 6.0, 15.0, 1.0, 8.0, 13.0, 15.0, 12.0, 12.0, 7.0,
                                      9.0, 9.0, 20.0, 9.0, 7.0, 2.0, 10.0, 15.0, 5.0, 8.0, 3.0, 14.0, 13.0, 2.0,
                                      1.0, 1.0, 4.0, 3.0, 8.0, 2.0, 2.0, 5.0, 7.0, 16.0, 20.0, 18.0, 10.0, 1.0,
                                      14.0, 13.0, 2.0, 8.0, 1.0, 8.0, 10.0, 8.0, 5.0, 2.0, 4.0, 19.0, 0.0, 2.0,
                                      0.0, 1.0, 1.0, 3.0, 0.0, 1.0, 1.0, 2.0, 3.0, 0.0, 13.0, 10.0, 5.0, 10.0, 1.0,
                                      16.0, 16.0, 9.0, 0.0, 15.0, 7.0, 8.0, 6.0, 3.0, 7.0, 14.0, 17.0, 7.0, 8.0,
                                      10.0, 1.0, 9.0, 11.0, 9.0, 4.0, 8.0, 18.0, 9.0, 6.0, 11.0, 16.0, 9.0, 4.0,
                                      10.0, 10.0, 8.0, 3.0, 4.0, 15.0, 15.0, 8.0, 5.0, 9.0, 3.0, 9.0, 3.0, 12.0,
                                      14.0, 11.0, 15.0, 6.0, 10.0, 6.0, 7.0, 11.0, 7.0, 10.0, 4.0, 8.0, 16.0, 14.0,
                                      12.0, 7.0, 4.0, 12.0, 7.0, 0.0, 17.0, 4.0, 9.0, 7.0, 9.0, 4.0, 7.0, 5.0, 9.0,
                                      14.0, 12.0, 0.0, 10.0, 9.0, 0.0, 2.0, 11.0, 5.0, 8.0, 2.0, 4.0, 7.0, 7.0,
                                      9.0, 7.0, 10.0, 3.0, 5.0, 10.0, 10.0, 6.0, 7.0, 10.0, 4.0, 3.0, 10.0, 10.0,
                                      8.0, 13.0, 8.0, 9.0, 3.0, 5.0, 14.0, 8.0, 8.0, 12.0, 4.0, 3.0, 11.0, 10.0,
                                      5.0, 12.0, 7.0, 4.0, 2.0, 2.0, 5.0, 9.0, 15.0, 6.0, 10.0, 13.0, 5.0, 6.0,
                                      9.0, 12.0, 6.0, 10.0, 7.0, 11.0, 8.0, 7.0, 12.0, 8.0, 10.0, 8.0, 12.0, 9.0,
                                      11.0, 10.0, 7.0, 6.0, 10.0, 7.0, 5.0, 13.0, 9.0, 8.0, 9.0, 14.0, 10.0, 11.0,
                                      10.0, 9.0, 11.0, 6.0, 5.0, 9.0, 12.0, 11.0, 9.0, 7.0, 16.0, 12.0, 9.0, 7.0,
                                      13.0, 10.0, 5.0, 9.0, 7.0, 13.0, 11.0, 4.0, 8.0, 8.0, 8.0, 8.0, 12.0, 8.0,
                                      8.0, 13.0, 7.0, 9.0, 12.0, 12.0, 6.0, 7.0, 10.0, 11.0, 6.0, 10.0, 11.0, 13.0,
                                      10.0, 9.0, 13.0, 13.0, 8.0, 10.0, 12.0, 9.0, 10.0, 11.0, 12.0, 8.0, 8.0,
                                      11.0, 7.0, 10.0, 11.0, 9.0, 13.0, 9.0, 13.0, 11.0, 10.0, 8.0, 11.0, 12.0,
                                      7.0, 9.0, 9.0, 8.0, 11.0, 7.0, 12.0, 9.0, 8.0, 8.0, 10.0, 6.0, 8.0, 12.0,
                                      8.0, 7.0, 6.0, 10.0, 6.0, 8.0, 11.0, 9.0, 4.0, 8.0, 15.0, 12.0, 4.0, 5.0,
                                      10.0, 10.0, 11.0, 6.0, 8.0, 5.0, 3.0, 7.0, 11.0, 8.0, 6.0, 8.0, 5.0, 8.0,
                                      6.0, 9.0, 10.0, 9.0, 10.0, 10.0, 9.0, 11.0, 8.0, 7.0, 11.0, 7.0, 8.0, 5.0,
                                      4.0, 2.0, 6.0, 7.0, 5.0, 1.0, 3.0, 10.0, 13.0, 13.0, 12.0, 10.0, 10.0, 6.0,
                                      2.0, 5.0, 10.0, 15.0, 14.0, 0.0, 0.0, 0.0, 2.0, 7.0, 11.0, 14.0, 9.0, 8.0,
                                      7.0, 9.0, 7.0, 9.0, 7.0, 2.0, 4.0, 4.0, 7.0, 12.0, 4.0, 8.0, 4.0, 5.0, 5.0,
                                      9.0, 13.0, 9.0, 10.0, 8.0, 14.0, 6.0, 11.0, 10.0, 7.0, 13.0, 12.0, 10.0, 4.0,
                                      10.0, 5.0, 5.0, 5.0, 2.0, 11.0, 16.0, 3.0, 5.0, 8.0, 8.0, 4.0, 5.0, 8.0,
                                      12.0, 11.0, 8.0, 12.0, 10.0, 3.0, 7.0, 11.0, 11.0, 10.0, 2.0, 13.0, 8.0,
                                      11.0, 9.0, 7.0, 5.0, 10.0, 13.0, 11.0, 6.0, 1.0, 7.0, 7.0, 14.0, 3.0, 3.0,
                                      5.0, 11.0, 11.0, 10.0, 8.0, 4.0, 9.0, 10.0, 8.0, 6.0, 8.0, 9.0, 6.0, 11.0,
                                      10.0, 11.0, 7.0, 13.0, 5.0, 9.0, 4.0, 9.0, 3.0, 15.0, 13.0, 11.0, 8.0, 5.0,
                                      12.0, 5.0, 13.0, 4.0, 5.0, 12.0, 11.0, 4.0, 3.0, 13.0, 10.0, 14.0, 6.0, 8.0,
                                      7.0, 2.0, 8.0, 4.0, 11.0, 17.0, 3.0, 4.0, 13.0, 9.0, 9.0, 12.0, 8.0, 6.0,
                                      9.0, 7.0, 9.0, 13.0, 6.0, 11.0, 16.0, 13.0, 10.0, 1.0, 4.0, 13.0, 2.0, 9.0,
                                      9.0, 11.0, 9.0, 4.0, 9.0, 6.0, 6.0, 13.0, 9.0, 2.0, 3.0, 0.0, 5.0, 8.0, 7.0,
                                      1.0, 0.0, 8.0, 9.0, 4.0, 0.0, 7.0, 5.0, 7.0, 10.0, 10.0, 14.0, 5.0, 5.0,
                                      10.0, 9.0, 8.0, 12.0, 15.0, 6.0, 4.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0,
                                      0.0, 5.0, 10.0, 2.0, 10.0, 5.0, 9.0, 13.0, 3.0, 14.0, 6.0, 15.0, 11.0, 6.0,
                                      8.0, 8.0, 6.0, 16.0, 18.0, 3.0, 10.0, 3.0, 6.0, 6.0, 11.0, 7.0, 11.0, 10.0,
                                      20.0, 5.0, 12.0, 5.0, 2.0, 9.0, 11.0, 12.0, 7.0, 10.0, 10.0, 10.0, 12.0,
                                      19.0, 3.0, 17.0, 6.0, 4.0, 11.0, 4.0, 7.0, 19.0, 7.0, 12.0, 2.0, 14.0, 5.0,
                                      10.0, 6.0, 9.0, 11.0, 12.0, 8.0, 15.0, 14.0, 12.0, 11.0, 13.0, 7.0, 8.0,
                                      15.0, 3.0, 10.0, 12.0, 13.0, 8.0, 10.0, 5.0, 7.0, 7.0, 6.0, 9.0, 9.0, 11.0,
                                      4.0, 5.0, 11.0, 7.0, 9.0, 6.0, 3.0, 8.0, 6.0, 3.0, 5.0, 7.0, 9.0, 12.0, 8.0,
                                      7.0, 4.0, 8.0, 10.0, 12.0, 12.0, 11.0, 7.0, 4.0, 6.0, 13.0, 13.0, 8.0, 11.0,
                                      11.0, 0.0, 7.0, 7.0, 11.0, 13.0, 10.0, 1.0, 5.0, 6.0, 12.0, 10.0, 5.0, 10.0,
                                      4.0, 0.0, 4.0, 10.0, 13.0, 10.0, 9.0, 13.0, 10.0, 11.0, 11.0, 6.0, 12.0, 8.0,
                                      8.0, 10.0, 9.0, 12.0, 9.0, 10.0, 11.0, 9.0, 9.0, 8.0, 7.0, 8.0, 9.0, 8.0,
                                      11.0, 9.0, 11.0, 10.0, 7.0, 11.0, 10.0, 12.0, 10.0, 8.0, 10.0, 7.0, 9.0,
                                      10.0, 12.0, 7.0, 9.0, 9.0, 10.0, 9.0, 11.0, 8.0, 8.0, 9.0, 8.0, 7.0, 13.0,
                                      13.0, 10.0, 7.0, 9.0, 11.0, 12.0, 10.0, 6.0, 11.0, 7.0, 8.0, 11.0, 11.0,
                                      11.0, 8.0, 10.0, 9.0, 11.0, 9.0, 10.0, 9.0, 10.0, 12.0, 8.0, 9.0, 9.0, 10.0,
                                      11.0, 6.0, 8.0, 12.0, 8.0, 6.0, 10.0, 9.0, 5.0, 9.0, 11.0, 8.0, 8.0, 10.0,
                                      9.0, 7.0, 9.0, 11.0, 8.0, 9.0, 10.0, 10.0, 12.0, 9.0, 8.0, 10.0, 10.0, 7.0,
                                      8.0, 10.0, 10.0, 9.0, 9.0, 12.0, 9.0, 8.0, 11.0, 8.0, 13.0, 8.0, 5.0, 10.0,
                                      8.0, 7.0, 7.0, 11.0, 7.0, 7.0, 9.0, 11.0, 10.0, 2.0, 7.0, 9.0, 12.0, 12.0,
                                      2.0, 7.0, 12.0, 8.0, 5.0, 11.0, 7.0, 3.0, 8.0, 9.0, 10.0, 10.0, 10.0, 9.0,
                                      11.0, 5.0, 10.0, 9.0, 10.0, 10.0, 9.0, 8.0, 8.0, 2.0, 8.0, 2.0, 1.0, 5.0,
                                      3.0, 10.0, 3.0, 9.0, 11.0, 5.0, 17.0, 9.0, 13.0, 9.0, 11.0, 4.0, 9.0, 6.0,
                                      9.0, 11.0, 16.0, 17.0, 15.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 18.0,
                                      20.0, 20.0, 18.0, 17.0, 15.0, 16.0, 20.0, 20.0, 18.0, 19.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 18.0, 20.0, 20.0, 20.0, 19.0, 18.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 17.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 19.0, 16.0, 18.0, 18.0, 18.0, 20.0, 16.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 17.0, 11.0, 17.0, 18.0, 13.0, 14.0, 17.0, 18.0,
                                      13.0, 18.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 19.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 17.0, 20.0, 19.0, 20.0, 19.0,
                                      19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 18.0, 19.0, 19.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 19.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      19.0, 20.0, 18.0, 19.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
                                      20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 2.8016357421875,
                                      2.8349609375, 2.9415283203125, 1.2449951171875, 2.4495849609375,
                                      3.75244140625, 3.179931640625, 2.6378173828125, -1.2674560546875,
                                      -2.341064453125, -2.8563232421875, -3.0106201171875, -2.538818359375,
                                      -2.7750244140625, -2.858642578125, -2.69287109375, -2.464111328125,
                                      -2.321044921875, -2.278076171875, -2.116943359375, -2.166015625,
                                      0.54443359375, 4.2099609375, 5.759521484375, 6.776611328125, 4.4705810546875,
                                      0.9996337890625, 3.41259765625, 6.9710693359375, 4.3671875, 5.7532958984375,
                                      2.8536376953125, 5.638916015625, 4.6341552734375, 2.5194091796875,
                                      3.3433837890625, 2.7972412109375, 2.5140380859375, 3.50048828125,
                                      3.2525634765625, 3.736328125, 4.2091064453125, 4.6661376953125,
                                      4.2042236328125, -1.8895263671875, -3.2916259765625, -3.2884521484375,
                                      -2.0400390625, -3.270263671875, 0.583984375, 2.2633056640625, 4.8544921875,
                                      3.06884765625, 3.554443359375, 4.238525390625, 3.170166015625,
                                      1.662353515625, 4.087158203125, 3.5675048828125, 3.2918701171875,
                                      -5.6749267578125, -2.99658203125, -12.661376953125, -14.2322998046875,
                                      -4.3345947265625, -3.2742919921875, -4.5093994140625, -4.2813720703125,
                                      -0.8829345703125, 3.6094970703125, 4.1083984375, 3.4658203125, 4.2802734375,
                                      3.9630126953125, 2.7957763671875, 3.215087890625, 3.7781982421875,
                                      0.7281494140625, 4.7396240234375, 8.43017578125, 5.2305908203125,
                                      6.9324951171875, 4.97119140625, 6.5113525390625, 8.0322265625, 8.353515625,
                                      8.3681640625, 8.67529296875, 5.999267578125, 4.8934326171875,
                                      4.4813232421875, 5.3515625, 5.4898681640625, 4.2138671875, 4.363037109375,
                                      3.4483642578125, 4.264892578125, 1.9371337890625, 1.0804443359375,
                                      0.716064453125, -0.2034912109375, 0.9300537109375, 0.90234375,
                                      1.503662109375, -0.9605712890625, 4.2362060546875, 2.2808837890625,
                                      1.4576416015625, 2.4351806640625, 0.7747802734375, 2.11376953125,
                                      2.2935791015625, 1.84619140625, 1.4527587890625, 2.21630859375,
                                      2.5516357421875, 4.095458984375, -0.436279296875, 2.44970703125,
                                      -0.1168212890625, 0.6993408203125, 1.2813720703125, 1.48388671875,
                                      0.054443359375, 0.6861572265625, 1.71240234375, -2.16064453125,
                                      -3.1593017578125, 0.9473876953125, 1.4566650390625, 0.3602294921875,
                                      3.2618408203125, 1.583740234375, -0.059326171875, 0.33203125,
                                      1.8865966796875, 1.69580078125, 1.853759765625, 1.2528076171875,
                                      1.89892578125, 1.64794921875, -1.592041015625, -2.4295654296875,
                                      -5.557861328125, -1.9273681640625, 1.940185546875, 2.032958984375,
                                      1.608642578125, 2.136962890625, 4.6278076171875, 3.05126953125,
                                      1.384033203125, 2.1588134765625, 1.1883544921875, 2.2371826171875,
                                      2.0501708984375, 2.086669921875, -1.63836669921875, -3.6728515625,
                                      -3.6658935546875, -3.548583984375, -3.76708984375, -3.486083984375,
                                      -3.6558837890625, -3.8214111328125, -2.6729736328125, 1.06884765625,
                                      2.778564453125, 0.7535400390625, 1.432861328125, 2.0206298828125,
                                      2.5933837890625, 5.26611328125, 4.671142578125, 1.2896728515625,
                                      0.1243896484375, -0.3507080078125, -0.323974609375, 0.006103515625,
                                      -0.2044677734375, 0.0211181640625, -0.410400390625, -0.33349609375,
                                      -1.39990234375, 2.5400390625, 3.109130859375, 3.1202392578125, 3.017578125,
                                      2.889892578125, 2.8095703125, 2.9422607421875, 3.018310546875,
                                      2.8602294921875, 2.9598388671875, 2.7392578125, 1.4715576171875,
                                      2.4246826171875, 1.5235595703125, 2.062744140625, 3.4541015625,
                                      -4.6627197265625, 1.182861328125, 1.4493408203125, 0.1685791015625,
                                      -1.6815185546875, 0.8209228515625, 1.0723876953125, 3.0234375,
                                      2.325439453125, 3.681396484375, 3.98974609375, -2.09783935546875,
                                      -15.16259765625, -5.2244873046875, -3.859619140625, 2.689453125,
                                      1.5465087890625, 1.33544921875, 1.6500244140625, 3.05126953125,
                                      1.727783203125, 1.49462890625, -15.230224609375, -4.7950439453125,
                                      -7.193359375, 0.01171875, 0.7476806640625, 3.2349853515625, 2.0894775390625,
                                      1.01171875, 1.689697265625, 1.572509765625, 2.600341796875, 3.4757080078125,
                                      -0.6951904296875, -14.22216796875, -10.49755859375, -3.1468505859375,
                                      -0.096923828125, 0.1470947265625, -0.28564453125, -0.1817626953125,
                                      0.9608154296875, -14.3919067382813, -10.3232421875, -1.6158447265625,
                                      -5.1915283203125, 1.9537353515625, 1.96044921875, 0.4091796875,
                                      1.4908447265625, 1.43798828125, 1.402099609375, 1.7291259765625,
                                      1.334228515625, -8.41357421875, -12.4339599609375, -6.3505859375,
                                      -0.9630126953125, -1.718505859375, -0.2762451171875, -3.1905517578125,
                                      0.7701416015625, -0.98095703125, -0.44091796875, -0.9046630859375,
                                      -0.8414306640625, -0.3935546875, -0.4903564453125, -0.4052734375,
                                      0.3370361328125, -0.473388671875, -0.23291015625, 0.44775390625,
                                      2.5228271484375, 2.37548828125, 2.565185546875, 0.957275390625,
                                      2.577880859375, 2.258056640625, 1.72802734375, 2.342529296875,
                                      1.8519287109375, 1.757568359375, 1.7733154296875, 2.525390625,
                                      -1.197021484375, 4.3280029296875, 1.4644775390625, 2.423583984375,
                                      2.2376708984375, -0.046875, -2.23095703125, -1.86865234375, 2.634765625,
                                      1.2530517578125, 2.3944091796875, 2.413330078125, 2.679443359375,
                                      2.1290283203125, 1.8935546875, 0.725341796875, 1.959228515625,
                                      1.785400390625, 2.241455078125, 2.123046875, 2.17047119140625,
                                      2.2723388671875, 1.822998046875, 0.93505859375, 2.7681884765625,
                                      2.2054443359375, 3.6392822265625, 3.9007568359375, 1.2158203125,
                                      1.824462890625, 2.29296875, 2.0006103515625, 1.926513671875, 1.8634033203125,
                                      1.86962890625, 2.477294921875, 2.166015625, 2.0982666015625, 2.2235107421875,
                                      2.2078857421875, 1.97021484375, 2.4652099609375, 2.2005615234375,
                                      2.0438232421875, 2.4813232421875, 2.5968017578125, 2.37890625,
                                      2.684814453125, 2.3077392578125, 1.4776611328125, 2.4373779296875,
                                      1.7171630859375, 1.9833984375, 1.588134765625, 2.4381103515625,
                                      2.669677734375, 2.642822265625, 2.2171630859375, 2.0234375, 2.5579833984375,
                                      2.208984375, 2.5059814453125, 2.0030517578125, 1.7899169921875,
                                      1.8768310546875, 2.6124267578125, 0.8114013671875, 2.3970947265625,
                                      1.728271484375, 1.9901123046875, 1.8094482421875, 2.1512451171875,
                                      2.3602294921875, 2.7252197265625, 1.4544677734375, 1.9022216796875,
                                      1.8494873046875, 2.2388916015625, 1.9263916015625, 2.632568359375,
                                      2.1190185546875, 2.099365234375, 3.51171875, 1.7208251953125, 2.000244140625,
                                      1.2794189453125, 2.155029296875, 2.394775390625, 1.9810791015625,
                                      3.3450927734375, 2.2923583984375, 2.791015625, 2.220458984375,
                                      2.3695068359375, 1.38427734375, 2.0958251953125, 0.9063720703125,
                                      2.3507080078125, 1.9844970703125, 2.1787109375, 2.1363525390625,
                                      2.023681640625, 1.77783203125, 2.194580078125, 2.33349609375, 0.4833984375,
                                      1.240966796875, 2.6185302734375, 1.4315185546875, 2.147705078125, 3.234375,
                                      1.858642578125, 2.1199951171875, 2.5916748046875, 2.1806640625, 2.7802734375,
                                      1.758544921875, 2.6209716796875, 2.554931640625, 2.396728515625,
                                      2.3028564453125, 2.1920166015625, 2.7177734375, 1.927978515625,
                                      2.7037353515625, 2.3067626953125, 2.64892578125, 2.61865234375,
                                      3.012451171875, 2.4405517578125, 1.9310302734375, 2.751220703125,
                                      3.348876953125, 2.9365234375, 2.3026123046875, 2.3480224609375,
                                      2.3123779296875, 3.1278076171875, 1.2188720703125, 2.536865234375,
                                      4.27490234375, 2.7923583984375, 1.7125244140625, 2.958984375, 2.844970703125,
                                      2.89306640625, 2.0667724609375, 2.8115234375, 3.0540771484375, 2.89794921875,
                                      3.649169921875, -1.72265625, 2.8175048828125, 2.531494140625,
                                      2.4342041015625, 2.726806640625, 2.7862548828125, 2.5233154296875,
                                      2.5894775390625, 2.322509765625, 2.675537109375, 2.84912109375,
                                      4.3170166015625, 3.2476806640625, 2.950439453125, 2.78955078125,
                                      3.145263671875, 2.70849609375, 2.431396484375, 2.8880615234375,
                                      2.7415771484375, 2.6636962890625, 2.822998046875, 2.368896484375,
                                      2.4007568359375, 2.1292724609375, 2.49462890625, 2.916015625, 2.403564453125,
                                      2.748291015625, 2.548828125, 2.6190185546875, 1.5718994140625,
                                      2.7125244140625, 2.14697265625, 2.2225341796875, 2.4443359375,
                                      2.040283203125, 1.5762939453125, 2.2547607421875, 2.6754150390625,
                                      2.115234375, 2.603515625, 2.4117431640625, 2.3970947265625, 2.00146484375,
                                      2.1263427734375, 1.826904296875, 2.1856689453125, 2.483154296875,
                                      2.1053466796875, -8.598876953125, -18.60498046875, -15.89697265625,
                                      -3.34912109375, 2.2642822265625, 2.0035400390625, 2.776611328125,
                                      3.2786865234375, 1.9215087890625, -12.65576171875, -17.2930908203125,
                                      -11.5511474609375, -0.233154296875, -18.4552001953125, -18.4217529296875,
                                      -18.478271484375, -18.9742431640625, -17.617919921875, -17.998291015625,
                                      -19.0003662109375, -18.33349609375, -3.60882568359375, -0.3662109375,
                                      0.739501953125, 2.0806884765625, 3.234375, 3.89697265625, 4.087158203125,
                                      3.9263916015625, 3.89306640625, 2.86376953125, 4.0723876953125, 4.2568359375,
                                      4.4482421875, -5.7701416015625, -17.6942138671875, -18.05615234375,
                                      -16.5462646484375, -17.1864013671875, -13.935302734375, -18.6248779296875,
                                      -17.6944580078125, -18.3330078125, -18.0433349609375, -18.8680419921875,
                                      -17.95654296875, -18.43212890625, -18.7086181640625, -18.908935546875,
                                      -19.0006103515625, -19.272705078125, -18.6328125, -16.9971923828125,
                                      -18.57666015625, -18.794677734375, -18.6163330078125, -18.1627197265625,
                                      0.22296142578125, 3.3079833984375, 3.467529296875, 5.191650390625,
                                      6.8968505859375, -10.5911865234375, -15.6883544921875, -17.17138671875,
                                      -18.406982421875, -18.771240234375, -18.577392578125, -18.7177734375,
                                      -19.220458984375, -18.50146484375, -18.5177001953125, -16.06787109375,
                                      6.51702880859375, 6.502197265625, 0.3961181640625, -0.8779296875,
                                      7.034912109375, 6.725341796875, 0.509033203125, 0.9649658203125,
                                      -6.80029296875, -14.342041015625, -18.3792724609375, -18.3775634765625,
                                      -17.9571533203125, -18.437255859375, -18.5791015625, -18.434326171875,
                                      -11.43017578125, -11.053955078125, -12.3565673828125, -13.4044189453125,
                                      -17.984130859375, -17.9840087890625, -18.27587890625, -17.9598388671875,
                                      -17.1829833984375, -16.8341064453125, -16.7034912109375, -16.450439453125,
                                      -17.7574462890625, -18.5152587890625, -18.6446533203125, -18.3271484375,
                                      -18.371337890625, -18.4210205078125, -18.3704833984375, -18.3389892578125,
                                      -18.13525390625, -18.6324462890625, -18.8006591796875, -18.601318359375,
                                      -18.3692626953125, -18.9232177734375, -18.2978515625, -18.4989013671875,
                                      -20.73681640625, -17.9036865234375, -18.8408203125, -19.1820068359375,
                                      -18.9630126953125, -19.23583984375, -19.1201171875, -18.5653076171875,
                                      -19.1131591796875, -19.35888671875, -18.9998779296875, -18.3116455078125,
                                      -18.595458984375, -19.1204833984375, -18.7689208984375, -19.15625,
                                      -17.1728515625, -18.6241455078125, -18.4869384765625, -18.8447265625,
                                      -19.480224609375, -18.3826904296875, -4.9547119140625, 0.410400390625,
                                      -18.135009765625, -19.0279541015625, -19.1119384765625, -15.283447265625,
                                      -19.1641845703125, -19.31640625, -19.2789306640625, -19.0155029296875,
                                      -18.2130126953125, -18.903564453125, -19.0758056640625, -18.8531494140625,
                                      -18.354736328125, 0.71026611328125, 5.8262939453125, 4.48583984375,
                                      -11.297607421875, -15.409912109375, -17.9825439453125, -18.3045654296875,
                                      -19.4942626953125, -18.0189208984375, -18.553466796875, -17.8614501953125,
                                      -19.0439453125, -18.0748291015625, -18.669189453125, -18.6258544921875,
                                      -17.2958984375, 8.1160888671875, 4.747802734375, 4.4034423828125,
                                      3.9482421875, 2.9620361328125, 4.3482666015625, 4.3160400390625,
                                      4.2037353515625, -1.8240966796875, -17.98046875, -15.393798828125,
                                      -18.8260498046875, -17.333984375, -18.9049072265625, -18.988525390625,
                                      -18.5321044921875, -18.23046875, -18.9345703125, -19.142822265625,
                                      -18.7957763671875, -19.21826171875, -18.9111328125, -18.8336181640625,
                                      -19.041748046875, -18.9954833984375, -19.111572265625, -18.94677734375,
                                      -19.175537109375, -18.99072265625, -18.97998046875, -18.9786376953125,
                                      -19.0439453125, -19.0220947265625, -19.0550537109375, -19.0177001953125,
                                      -19.0419921875, -19.026611328125, -19.08544921875, -19.052978515625,
                                      -19.0125732421875, -19.2423095703125, -19.1290283203125, -18.7574462890625,
                                      -17.8507080078125, -18.9010009765625, -19.1204833984375, -18.788818359375,
                                      -18.978515625, -19.2098388671875, -19.20263671875, -18.8223876953125,
                                      -19.22265625, -18.7977294921875, -18.8360595703125, -6.672119140625,
                                      -3.5517578125, -14.1981201171875, -13.6689453125, -17.8638916015625,
                                      -19.0684814453125, -19.2806396484375, -18.4254150390625, -18.992919921875,
                                      -19.1871337890625, -14.3414306640625, -3.115234375, -14.19482421875,
                                      -13.6873779296875, -16.0894775390625, -18.7091064453125, -18.94873046875,
                                      -19.0767822265625, -18.96142578125, -19.0615234375, -19.129638671875,
                                      -18.793212890625, -18.757568359375, -10.5695190429688, -3.988525390625,
                                      -0.8607177734375, -14.5235595703125, -18.4755859375, -19.170166015625,
                                      -19.3480224609375, -19.2674560546875, -15.8955078125, -3.1248779296875,
                                      0.893310546875, -17.4566650390625, -17.766357421875, -18.848388671875,
                                      -18.6876220703125, -19.42626953125, -18.80908203125, -19.042724609375,
                                      -19.277099609375, -18.99267578125, -19.1153564453125, -2.80047607421875,
                                      -5.078857421875, -7.29443359375, -17.2581787109375, -19.11328125,
                                      -18.982421875, -18.843017578125, -19.0318603515625, -19.061767578125,
                                      -19.193115234375, -19.1553955078125, -19.1165771484375, -19.0606689453125,
                                      -19.3431396484375, -19.225830078125, -18.9222412109375, -18.994873046875,
                                      -19.2945556640625, -19.6068115234375, -19.0791015625, -19.0185546875,
                                      -19.0565185546875, -19.352783203125, -19.0950927734375, -19.1234130859375,
                                      -19.16064453125, -19.110595703125, -18.6634521484375, -18.0347900390625,
                                      -17.01123046875, -3.693359375, -1.882568359375, -0.764404296875,
                                      -13.27978515625, -18.7257080078125, -18.73486328125, -2.70440673828125,
                                      -0.604736328125, 0.7117919921875, -13.2186279296875, -16.8292236328125,
                                      -18.797119140625, -18.501708984375, -5.9117431640625, -7.044677734375,
                                      -6.77001953125, -12.8228759765625, -18.3741455078125, -18.873046875,
                                      -18.7401123046875, -16.4427490234375, -4.9158935546875, -3.2203369140625,
                                      -15.00341796875, -15.416015625, -19.0487060546875, -18.9686279296875,
                                      -11.6888427734375, -3.9661865234375, -5.0511474609375, -14.5999755859375,
                                      -18.58056640625, -19.2825927734375, -19.227294921875, -19.2281494140625,
                                      -19.1712646484375, -19.390869140625, -19.20751953125, -19.065185546875,
                                      -18.998779296875, -18.9259033203125, -19.0234375, -18.7403564453125,
                                      -19.22216796875, -18.9451904296875, -18.9447021484375, -18.96337890625,
                                      -19.2020263671875, -18.9593505859375, -18.879150390625, -19.11279296875,
                                      -19.3341064453125, -18.9013671875, -18.9268798828125, -19.61328125,
                                      -18.7431640625, -18.89892578125, -18.73388671875, -19.1060791015625,
                                      -18.9627685546875, -18.8817138671875, -18.8798828125, -18.907470703125,
                                      -18.9049072265625, -18.947998046875, -18.9761962890625, -18.7994384765625,
                                      -19.6834716796875, -19.1763916015625, -19.19287109375, -18.418701171875,
                                      -18.64990234375, -19.0152587890625, -18.85498046875, -18.96533203125,
                                      -18.93701171875, -19.072265625, -19.1875, -19.05615234375, -19.2552490234375,
                                      -19.084716796875, -19.419921875, -19.18603515625, -18.769775390625,
                                      -19.2027587890625, -18.8699951171875, -19.197265625, -19.166748046875,
                                      -19.307861328125, -19.0450439453125, -18.8250732421875, -18.934814453125,
                                      -18.8798828125, -19.146240234375, -19.1109619140625, -19.247802734375,
                                      -19.188720703125, -19.2967529296875, -19.200927734375, -19.2281494140625,
                                      -19.08056640625, -19.0494384765625, -18.9666748046875, -19.282470703125,
                                      -19.14208984375, -19.052734375, -19.0523681640625, -18.8160400390625,
                                      -15.521484375, -16.564208984375, -15.844970703125, -17.206298828125,
                                      -17.5458984375, -19.2332763671875, -17.531982421875, -18.99267578125,
                                      -19.439453125, -17.2562255859375, -19.8175048828125, -17.3609619140625,
                                      -19.1138916015625, -18.5882568359375, -18.6123046875, -19.1202392578125,
                                      -17.3084716796875, -19.727294921875, -18.382568359375, -19.3148193359375,
                                      -18.53515625, -18.8736572265625, -18.475341796875, -18.289794921875,
                                      -19.285400390625, -17.7908935546875, -20.197021484375, -17.6236572265625,
                                      -19.0115966796875, -19.4246826171875, -18.91796875, -18.640625,
                                      -19.4287109375, -18.6590576171875, -20.18896484375, -18.023681640625,
                                      -20.26708984375, -17.8106689453125, -19.5250244140625, -18.6214599609375,
                                      -19.831787109375, -18.2100830078125, -20.1375732421875, -15.003173828125,
                                      -7.9525146484375, -17.912109375, -18.1336669921875, -17.5960693359375,
                                      -18.4886474609375, -17.518798828125, -17.5447998046875, -18.6888427734375,
                                      -17.6334228515625, -17.3665771484375, -18.9693603515625, -18.095458984375,
                                      -17.0657958984375, -17.9273681640625, -17.6810302734375, -19.310791015625,
                                      -17.55615234375, -16.7955322265625, -17.09521484375, -18.0625,
                                      -18.9991455078125, -18.1497802734375, -17.3634033203125, -18.3057861328125,
                                      -17.8800048828125, -17.6536865234375, -19.056884765625, -17.9786376953125,
                                      -19.0364990234375, -18.2158203125, -15.387939453125, -10.554443359375,
                                      -15.786376953125, -12.5517578125, -12.6153564453125, -16.4725341796875,
                                      -13.9193115234375, -12.112060546875, -16.021240234375, -10.6417236328125,
                                      -12.74658203125, -18.93603515625, -19.2080078125, -19.1837158203125,
                                      -19.1907958984375, -19.197021484375, -19.2174072265625, -19.15966796875,
                                      -18.8544921875, -16.807861328125, -10.75732421875, -3.752685546875,
                                      -6.834716796875, -3.3857421875, -14.825439453125, -18.830322265625,
                                      -19.010498046875, -18.703857421875, -15.705810546875, -7.0440673828125,
                                      -1.083740234375, -4.210205078125, -15.6219482421875, 3.0804443359375,
                                      2.98681640625, 3.044921875, 0.9564208984375, -6.7628173828125,
                                      -0.234619140625, 2.0943603515625, 3.2413330078125, 7.9376220703125,
                                      16.963134765625, 17.1265869140625, 17.166015625, 16.4793701171875,
                                      16.68212890625, 16.6138916015625, 16.4827880859375, 16.6966552734375,
                                      16.945556640625, 16.860107421875, 16.58056640625, 16.33251953125,
                                      14.16650390625, 0.75830078125, 1.67724609375, 4.5562744140625,
                                      -2.04736328125, -11.5858154296875, -0.921875, -0.2034912109375,
                                      -2.18408203125, -0.938232421875, -0.05908203125, 0.35107421875,
                                      -0.785888671875, -4.3333740234375, -2.4661865234375, -0.1700439453125,
                                      -0.8741455078125, -3.404541015625, -9.4947509765625, -2.9442138671875,
                                      -0.9261474609375, -1.2955322265625, -1.6402587890625, 10.7868041992188,
                                      16.6256103515625, 16.74853515625, 16.3525390625, 15.5341796875,
                                      8.948486328125, -10.1827392578125, 1.419677734375, 2.517578125, 0.4091796875,
                                      -1.5394287109375, -5.3416748046875, -3.6448974609375, 0.5794677734375,
                                      2.3507080078125, 1.205810546875, 11.34912109375, 16.040771484375,
                                      11.7154541015625, 4.7666015625, 15.4542236328125, 15.862060546875,
                                      16.6290283203125, 16.9398193359375, 11.2496337890625, -9.676025390625,
                                      0.6434326171875, 1.4500732421875, 3.2294921875, 1.89697265625,
                                      1.6812744140625, 1.84130859375, -15.4400634765625, -17.1585693359375,
                                      -14.7994384765625, -12.000244140625, -4.390380859375, -0.8787841796875,
                                      1.765625, -1.5987548828125, -4.1092529296875, -4.811767578125,
                                      -5.701416015625, -5.7156982421875, -5.503662109375, -1.203369140625,
                                      -3.5855712890625, -3.203125, -3.0760498046875, 2.103271484375,
                                      2.367919921875, 2.3026123046875, 1.9359130859375, 2.67578125,
                                      3.9764404296875, 3.5296630859375, 3.9251708984375, 3.0985107421875,
                                      3.9654541015625, 3.312255859375, 1.12841796875, 4.691162109375,
                                      2.160400390625, 0.0850830078125, 0.64697265625, 0.829833984375, 0.65625,
                                      1.45068359375, 0.2510986328125, 1.7913818359375, 0.9637451171875,
                                      -1.10791015625, -4.298095703125, -0.6583251953125, -0.1239013671875,
                                      -4.4334716796875, 3.92724609375, 3.7841796875, 3.549560546875,
                                      1.2760009765625, -3.6800537109375, 3.2926025390625, 7.12677001953125,
                                      16.8939208984375, 0.2041015625, 0.8607177734375, 1.06689453125,
                                      -10.21142578125, -1.8302001953125, -1.464599609375, -1.5130615234375,
                                      -2.136474609375, 3.5201416015625, 1.1756591796875, 0.7520751953125,
                                      2.0794677734375, 3.4779052734375, 6.7877197265625, 16.2239990234375,
                                      16.3194580078125, 9.22216796875, -9.9617919921875, -6.9090576171875,
                                      2.5389404296875, 0.9722900390625, -4.3909912109375, -4.5697021484375,
                                      3.2891845703125, 1.5498046875, -8.0267333984375, -2.2099609375,
                                      2.7178955078125, 2.338623046875, 11.3477172851563, 16.3759765625,
                                      16.50439453125, 16.43701171875, 16.7332763671875, 16.340576171875,
                                      16.27099609375, 16.3609619140625, 15.868408203125, 2.1422119140625,
                                      -12.79833984375, 0.8076171875, 5.659423828125, 2.94482421875,
                                      0.3634033203125, -1.419921875, -2.484619140625, 1.9873046875,
                                      1.6458740234375, 2.65185546875, 1.0238037109375, 1.97705078125,
                                      1.8135986328125, 1.668212890625, 1.2640380859375, 1.307373046875, 1.1796875,
                                      -1.5491943359375, -2.9627685546875, -3.09375, -3.09228515625, -3.07177734375,
                                      -2.9700927734375, -2.6689453125, -2.703857421875, -2.6151123046875,
                                      -2.73388671875, -2.2366943359375, -0.85595703125, -0.2294921875,
                                      -0.0267333984375, -0.144287109375, 0.1549072265625, -6.383056640625,
                                      -1.147705078125, -0.3677978515625, 2.2100830078125, 1.4300537109375,
                                      0.06591796875, -1.925537109375, -0.44970703125, -0.49853515625,
                                      -2.7099609375, -1.858642578125, 4.10955810546875, 9.0185546875, -10.34765625,
                                      -12.814453125, -2.92919921875, 0.06982421875, -2.61669921875,
                                      1.1607666015625, -4.3106689453125, -3.095703125, -2.6600341796875,
                                      3.007568359375, -11.3499755859375, -13.4063720703125, -6.6387939453125,
                                      -5.5101318359375, -2.0157470703125, -0.3575439453125, -4.42431640625,
                                      -1.94091796875, -2.5587158203125, -2.6875, -4.486083984375, -0.91162109375,
                                      7.291748046875, -16.9610595703125, -11.702880859375, -5.2635498046875,
                                      -2.8692626953125, -2.945556640625, -3.3582763671875, -4.43359375,
                                      5.60809326171875, -17.622314453125, -8.3697509765625, -2.8740234375,
                                      -3.2493896484375, -3.425048828125, -3.33203125, -5.1295166015625,
                                      -4.3448486328125, -2.0184326171875, -0.2796630859375, -3.16748046875,
                                      2.0772705078125, -11.474365234375, -15.77587890625, -4.609130859375,
                                      -4.650634765625, -5.0848388671875, -4.8441162109375, -2.9892578125,
                                      -3.4130859375, -2.38671875, -4.5526123046875, -4.476318359375,
                                      -0.629638671875, -3.0389404296875, -3.8206787109375, -5.4407958984375,
                                      -5.080810546875, -3.449951171875, -3.0538330078125, -3.2696533203125,
                                      -3.9417724609375, -3.8460693359375, -1.392822265625, -3.0675048828125,
                                      -2.7083740234375, -3.844970703125, -3.060546875, -5.7618408203125,
                                      -7.923583984375, -7.4666748046875, -20.7633056640625, -19.998291015625,
                                      -19.2930908203125, -13.828125, -4.2938232421875, -3.66943359375,
                                      -15.1298828125, -19.979736328125, -20.0006103515625, -14.6990966796875,
                                      -6.3232421875, -4.408447265625, -5.0179443359375, -14.199462890625,
                                      -20.018310546875, -17.9874267578125, -14.72412109375, -4.5135498046875,
                                      -5.0550537109375, -3.59521484375, -4.552734375, -19.251708984375,
                                      -20.4769287109375, -12.2813720703125, -8.6688232421875, -3.3985595703125,
                                      -3.3309326171875, -10.998779296875, -19.0325927734375, -17.470947265625,
                                      -12.30810546875, -2.2744140625, -1.2879638671875, -1.3052978515625,
                                      -1.6968994140625, -1.3822021484375, -2.6044921875, -1.1947021484375,
                                      -3.0400390625, -3.264404296875, -3.0953369140625, -4.348876953125,
                                      -2.8154296875, -2.9644775390625, -3.0115966796875, -3.918212890625,
                                      -2.6658935546875, -3.2022705078125, -3.53955078125, -2.8104248046875,
                                      -1.9168701171875, -2.09716796875, -2.1717529296875, -2.7606201171875,
                                      -2.46435546875, -3.9039306640625, -2.818603515625, -2.746826171875, -4.40625,
                                      -4.1209716796875, -5.3226318359375, -3.6588134765625, -4.2281494140625,
                                      -3.7203369140625, -4.350341796875, -3.5765380859375, -3.1920166015625,
                                      -2.8828125, -3.2147216796875, -3.7957763671875, -4.0679931640625,
                                      -4.2276611328125, -2.8138427734375, -3.3306884765625, -2.4163818359375,
                                      -3.2376708984375, -2.5050048828125, -3.01220703125, -2.295654296875,
                                      -3.3050537109375, -2.9197998046875, -2.545654296875, -1.7001953125,
                                      -2.6239013671875, -2.3038330078125, -2.6396484375, -1.9091796875,
                                      -1.118896484375, -2.2493896484375, -1.867919921875, -3.4056396484375,
                                      -0.840087890625, -3.180419921875, -2.4219970703125, -1.397705078125,
                                      -1.4456787109375, -0.5712890625, -2.667724609375, -1.354248046875,
                                      -1.8717041015625, -0.7886962890625, -2.0574951171875, -2.0128173828125,
                                      -1.7042236328125, -1.2666015625, -3.7938232421875, -3.976318359375,
                                      -4.3822021484375, -2.28271484375, -1.2236328125, -0.886962890625,
                                      -2.43212890625, -1.5777587890625, -0.994384765625, -1.9090576171875,
                                      -1.8651123046875, -0.5511474609375, -1.150634765625, -0.6845703125,
                                      -1.1488037109375, -1.90869140625, -1.51806640625, -1.420654296875,
                                      -1.867431640625, -0.4661865234375, -1.081787109375, -1.9761962890625,
                                      -0.735107421875, -1.531005859375, -2.162109375, -0.6163330078125,
                                      -2.336181640625, -2.7225341796875, -2.5457763671875, -1.7220458984375,
                                      -2.77685546875, -1.452880859375, -0.7010498046875, -1.5040283203125,
                                      -1.0955810546875, -1.55712890625, -2.207275390625, -1.5498046875,
                                      -2.0467529296875, -2.2303466796875, -1.1314697265625, -2.12744140625,
                                      -3.178466796875, -1.005126953125, -1.7081298828125, -2.4193115234375,
                                      -10.75732421875, -18.8846435546875, -8.041015625, -8.7052001953125,
                                      -3.3221435546875, -7.3878173828125, -7.65478515625, -7.188232421875,
                                      -7.9429931640625, -2.3216552734375, -8.7855224609375, -7.5924072265625,
                                      -2.803955078125, -7.71728515625, -7.1778564453125, -2.414306640625,
                                      -8.5372314453125, -3.018798828125, -10.63330078125, -9.1732177734375,
                                      -3.08056640625, -6.6549072265625, -5.6483154296875, -7.39697265625,
                                      -8.927978515625, -2.3486328125, -6.979248046875, -7.7996826171875,
                                      -0.25390625, -2.415771484375, -3.3800048828125, -3.872802734375,
                                      -3.0936279296875, -3.310791015625, -2.447998046875, -3.3143310546875,
                                      -3.0318603515625, -3.049072265625, -3.04345703125, -3.31494140625,
                                      -3.2225341796875, -3.0531005859375, -3.275390625, -1.5791015625,
                                      -1.4263916015625, -2.0025634765625, -2.1458740234375, -2.13525390625,
                                      -2.12255859375, -4.348876953125, -7.575439453125, -9.728515625,
                                      3.446044921875, -2.49951171875, -17.2176513671875, -10.67333984375,
                                      -2.469482421875, -3.193603515625, -2.7340087890625, -8.8143310546875,
                                      -1.773681640625, 7.585205078125, -15.1685791015625, -12.2990834476384,
                                      18.9199442103683, 18.8774759118692, 18.9599800888117, 19.1614525971704,
                                      19.9213076841304, 19.1333689746247, 19.6172847726477, 18.8536857175246,
                                      19.9629074410505, 17.1610781909421, 17.4006837977027, 17.5767986501595,
                                      17.0068711363965, 17.3745224009748, 17.3493019739738, 17.1646130593271,
                                      17.3324374434511, 17.49180298372, 17.5609699161325, 17.2766120263161,
                                      17.1026180798292, 17.9385187960473, 18.5605096646597, 19.3247498621203,
                                      18.8000360741192, 19.6234109009494, 20.5417981702243, 19.3179275673085,
                                      19.1411704291558, 19.2991674832217, 19.2511192346088, 19.3400068732396,
                                      19.0496952007313, 19.2352947233357, 19.6892759786038, 19.6141853281388,
                                      19.2522617046832, 19.4795905544686, 19.6598995372443, 20.1794718689192,
                                      19.501173775014, 19.2869264968745, 19.2650600968147, 19.1383339779418,
                                      18.6252369521764, 17.3276376103701, 17.4506221755907, 17.4708015286498,
                                      17.3846033249119, 18.8905232688714, 20.8591938829733, 18.5299470760746,
                                      19.3162312790685, 19.222022181098, 19.3880682940033, 19.8975433544317,
                                      19.8402372627648, 19.1633546875385, 19.0225811786388, 16.5901140702861,
                                      21.5220286539723, 17.6015399794243, 18.1140328795043, 19.4081624352713,
                                      17.5687381504898, 17.5972385875227, 17.3710664213965, 17.5800599301362,
                                      18.2164164436061, 19.9685331757855, 19.3436427056315, 18.924227170005,
                                      18.7574128626766, 19.1303707965781, 18.8991631788797, 18.8938574991126,
                                      20.9714638407913, 20.488821438639, 20.2626689331153, 19.9004690023616,
                                      19.652814049851, 19.8015935027704, 19.1691954467506, 19.4382408811362,
                                      19.4428361625512, 19.4056951108844, 19.5672315525943, 19.4838429376761,
                                      19.762419786879, 19.207430181195, 19.5523482688264, 19.388877208315,
                                      19.4469859764961, 19.2232101975212, 19.2246850695815, 18.828536274833,
                                      18.7839750128188, 18.9463150223188, 19.3679092725162, 19.0029812370754,
                                      18.9071224484044, 19.3933808679902, 18.8016131705932, 18.902329488248,
                                      27.0119941437553, 20.9675406415533, 19.1414898806427, 20.8488860357721,
                                      19.5852906667195, 19.4924467973144, 19.2910218440802, 19.1374436026497,
                                      19.4637566474405, 19.5370230637958, 19.1670562764801, 18.749740280427,
                                      19.7688366131382, 19.2562853467336, 19.1815978732785, 20.1349520992153,
                                      18.9039491491611, 19.1803468920258, 18.88803674785, 19.1934514894163,
                                      20.0517748306971, 18.7777205889332, 17.2545534935967, 17.8914051584471,
                                      19.1263932872218, 19.3650972699332, 19.9836947436702, 20.1693965381264,
                                      19.6890908508144, 19.4106350374998, 19.3954356301951, 19.4952838340345,
                                      18.8092956038037, 19.2672341259027, 19.2727323684449, 19.0894581343732,
                                      18.7890749957847, 18.6692467349045, 17.448947760616, 18.4685978157342,
                                      18.7763244265341, 20.1157901675699, 20.0474548107028, 18.8179651344303,
                                      20.0117682422902, 19.6875128774258, 19.6737102204546, 19.0590276147055,
                                      19.4276000863251, 20.1733820317254, 19.550340161473, 18.9436799720831,
                                      17.5820851813111, 20.8586165750996, 17.4448536853544, 17.47315279668,
                                      17.2755564768952, 17.4384205289676, 17.3079947147445, 17.2778779046991,
                                      17.3245555732987, 17.7288856607849, 19.0781599563884, 20.9026752099818,
                                      19.53567201691, 18.4397651024268, 19.3084706149216, 19.3799664738202,
                                      19.6811775565013, 19.680812601031, 19.3379851824569, 19.2999462659142,
                                      18.9962924058693, 19.2683104250444, 19.043610575054, 19.0556378644662,
                                      19.1720878344094, 19.1009058524923, 19.2381285063897, 19.1701269792512,
                                      19.4825221161762, 19.4742701528578, 19.48325807559, 19.4653479067513,
                                      19.5063190374498, 19.4574705635382, 19.465728839166, 19.4455523153414,
                                      19.4332978677069, 19.4502002287834, 19.4152355313881, 19.1883372402023,
                                      19.3161596432337, 19.4240200529382, 19.3264368807037, 19.1120303790106,
                                      20.0335547573487, 19.3411946075591, 19.2799940261417, 19.034222330064,
                                      19.29030176444, 19.323555206185, 19.5137034762553, 19.3626974594296,
                                      19.4078104284063, 19.5032668290644, 19.3570692468117, 19.4666563166184,
                                      18.7979874896906, 20.5327194190374, 21.7049375287826, 18.6505338310437,
                                      19.1436946978547, 19.8139394311675, 19.0132587584219, 19.8887921587876,
                                      19.5309367763985, 15.7884689590661, 21.4005254352038, 20.1046756010273,
                                      21.5737092099998, 18.9654780011724, 19.8478437960997, 19.4300805538651,
                                      19.4119509983799, 19.7976327596716, 19.2833577934683, 19.4832542178225,
                                      19.4562158191195, 19.6428856938825, 19.1107742209855, 19.5044799740173,
                                      21.0041898623032, 20.9502001384837, 19.6178250714443, 19.412538880752,
                                      19.5854592692971, 19.5936377071467, 16.8920702168359, 22.2571229725136,
                                      21.0812863625808, 20.176247504193, 19.5732380675017, 19.7225991790613,
                                      20.4458371971888, 19.9613032779895, 19.7745465373736, 19.6920521630774,
                                      19.5165843472945, 19.1674492612256, 19.434762333501, 18.8829016181565,
                                      20.3612829647017, 22.3221303854477, 18.6778190494461, 19.7844558773005,
                                      20.2665136972126, 19.8801320527535, 19.6710437837853, 19.6275250284557,
                                      19.6123470962001, 19.8931071352001, 19.8070734356683, 19.2058956383524,
                                      19.6067525766794, 19.6221929395173, 19.7592053359656, 19.6951427741632,
                                      19.6284376814036, 19.9642162108386, 19.6816618935889, 19.574999228966,
                                      19.6796798900366, 20.0812861598789, 19.5242252288339, 19.4506270470316,
                                      19.6359709044296, 19.5005897622325, 19.7681569428376, 20.0193744897484,
                                      18.7527359434404, 22.9801010737179, 20.4219397743782, 21.3021773095615,
                                      20.5224774527081, 19.5498705539239, 19.2571984941272, 21.1045184698922,
                                      20.8862214112442, 21.3164803837712, 21.563768616285, 18.6906917656577,
                                      19.5134519621356, 19.359651562606, 19.1435933578244, 21.4770671522906,
                                      20.0923238327566, 21.233945524688, 19.2671156605357, 19.8212081718115,
                                      19.4079015751786, 17.2481462016633, 22.1794776940018, 21.1245241944206,
                                      21.0509789281402, 18.5473538037785, 19.6922158234211, 19.3950905993649,
                                      19.8939223987082, 20.5340348687509, 19.8528351489278, 20.8322085940479,
                                      19.0639929582163, 19.4397040609415, 19.3711738588661, 19.3973422350848,
                                      19.3144970434304, 19.8604803225238, 19.4157276626252, 19.5055379269702,
                                      19.5679321556105, 19.4164570241348, 19.7750373965007, 19.2348622862826,
                                      19.7881561418616, 19.512643402074, 19.7927105802982, 19.5155832382955,
                                      19.8362889845573, 19.7505037424928, 19.5126930002175, 19.5180877214567,
                                      19.8423726225368, 19.4250094929767, 19.5708703229314, 20.1398686149334,
                                      19.5499162943771, 19.5179230348184, 19.3200751381708, 20.0200371332348,
                                      19.7529331145796, 20.0407926504221, 19.5816379300934, 19.7757738142937,
                                      19.5550653827227, 19.7301136270186, 19.7308170315547, 19.482080499232,
                                      20.1431926729674, 19.8354929953077, 19.8450437348239, 19.2874963027378,
                                      19.4016709582285, 19.5270042384938, 19.5228162018066, 19.5192469639918,
                                      19.4924220190367, 19.675546152551, 19.7391906870127, 19.5894716658959,
                                      20.0179135814016, 19.7509921733471, 19.9886167790461, 19.6249581860647,
                                      19.6336703824921, 19.6253480992317, 19.3723229604738, 19.5690166455121,
                                      19.4991320504522, 19.8555390419295, 19.469237398031, 19.6805097982161,
                                      19.2445807543389, 19.7391310410946, 19.6305063760052, 19.4733201547382,
                                      19.5682896043458, 19.4508082593516, 19.5953189702471, 19.5654171781529,
                                      19.5908425716932, 19.3832980224155, 19.5129670548163, 19.3743128583958,
                                      19.6436503691566, 19.5490647002814, 19.7398269682016, 19.6578092696127,
                                      19.6199778734829, 19.9302144915948, 20.8226549568285, 18.6848077443886,
                                      20.0618896977773, 20.2766139589275, 21.0435806915201, 19.1377427732756,
                                      20.2821826851274, 20.1663769821308, 18.0378964174268, 20.8574799636093,
                                      18.483387008336, 19.8638028040929, 19.27223803119, 19.4079182290241,
                                      20.0209718851324, 18.2511155622424, 20.5732766205431, 19.416453141773,
                                      20.3494976092133, 19.2323943577388, 19.5884574747402, 19.5064827464923,
                                      19.3221463618377, 20.2214468070032, 19.0758666471197, 21.0235933189061,
                                      18.5257236235591, 19.6254493143057, 20.1429708979167, 19.5471491199331,
                                      19.0597137872406, 19.9406989976975, 19.7421876362954, 20.7868986785387,
                                      18.6136144858181, 21.0639285387633, 18.4390318334685, 20.181371033201,
                                      19.4918869592157, 20.7581983845733, 18.8122487271748, 20.8186585788983,
                                      19.8841417311143, 20.6706097005632, 20.0553553245381, 20.476668368318,
                                      18.5269809052488, 20.2552623872599, 20.0820716684014, 19.8576663046562,
                                      20.6376520455939, 18.4913163909872, 20.0481385339223, 20.8362152751576,
                                      19.1265297026419, 19.2879592416488, 19.7798250175663, 18.3876831118173,
                                      21.5524763978611, 18.412859890335, 20.2750849751415, 20.1013390196322,
                                      18.8344340538878, 20.4344603067008, 19.7391165936205, 19.7597103349274,
                                      20.7232921868606, 18.5145932267005, 19.753696841431, 20.9906715722648,
                                      18.2118540889323, 19.4561237540155, 20.1067887006356, 19.6292249146565,
                                      15.6459886783587, 20.3344928706651, 16.4344242656761, 17.6557096287313,
                                      20.0779868130254, 18.0657563999103, 16.2941894780507, 19.7932849727535,
                                      15.932128403282, 17.8917882070259, 20.1564049195632, 19.4266260990997,
                                      19.3874334637163, 19.400993634356, 19.4365977798179, 19.4256161422831,
                                      19.4058828142287, 19.8538688272135, 19.3091163915601, 22.1270518773153,
                                      19.3801324604751, 20.0252248527881, 21.2415956703741, 19.6350401093199,
                                      19.3973680995643, 19.8095707304731, 19.4079812466767, 19.9843464114869,
                                      20.7053108619248, 19.1067727981103, 21.3080155977163, 21.0445543530017,
                                      -31.1679389312977, -43.8625954198473, -38.3587786259542, -162.526717557252,
                                      -392.167938931298, 497.679389312977, -74.5954198473281, 191.496183206107,
                                      387.698473282442, -15.5267175572519, -19.885496183206, 27.6297709923664,
                                      -18.9541984732825, -18.3549618320611, -43.8854961832061, -62.0152671755726,
                                      -21.3358778625954, -54.2137404580153, 18.8053435114503, -33.2862595419847,
                                      31.114503816794, -1132.19083969466, -312.267175572519, 51.9809160305343,
                                      -122.687022900763, -40.9198473282451, 4.52290076335775, -160.732824427481,
                                      -147.916030534351, -337.69465648855, 67.8435114503818, -65.7862595419849,
                                      0.961832061068844, 144.62213740458, -105.729007633588, 13.9007633587793,
                                      -15.0954198473283, 45.5381679389314, -99.6717557251909, 40.5190839694658,
                                      254.408396946565, -52.7595419847328, -188.729007633588, 493.736641221375,
                                      702.732824427481, -33.9007633587786, -45.6793893129771, -56.8167938931298,
                                      72.469465648855, -1491.20610687023, -656.507633587788, -32.0725190839697,
                                      -29.2442748091603, -57.5076335877864, 222.282442748092, -322.332061068702,
                                      88.7442748091603, 207.351145038168, 68.7595419847328, 523.568702290076,
                                      216.072519083968, -1.59923664122134, -348.030534351145, 301.870229007634,
                                      -64.4885496183206, -113.141221374046, -161.610687022901, 68.8664122137404,
                                      -1266.72900763359, -692.316793893129, -136.828244274809, 280.454198473283,
                                      -110.454198473282, -152.675572519084, -23.1526717557253, -101.125954198473,
                                      -828.282442748093, -107.935114503817, 168.263358778626, 24.6106870229008,
                                      473.885496183205, 160.167938931298, -71.8396946564886, -103.748091603053,
                                      -179.248091603053, -42.1717557251909, -31.1755725190839, -25.793893129771,
                                      600.854961832061, -106.374045801527, -41.9885496183206, -116.729007633588,
                                      90.7022900763358, -70.3396946564882, 186.877862595419, 23.3435114503816,
                                      -84.6335877862598, 24.5229007633588, -28.1679389312977, -58.7480916030534,
                                      -42.3893129770991, -51.3664122137406, -20.8816793893129, -13.1908396946565,
                                      -8.1335877862594, -46.06106870229, -42.5190839694657, -85.1755725190841,
                                      -219.56106870229, 1.89312977099222, -83.1221374045801, -5.5114503816794,
                                      -29.0801526717557, -95.5916030534351, -61.293893129771, -325.919847328244,
                                      230.12213740458, -53.1259541984734, -140.519083969466, 51.6793893129768,
                                      414.213740458015, -73.3587786259542, -45.2595419847328, -323.900763358779,
                                      130.458015267176, 207.954198473282, 1129.52671755725, -645.530534351145,
                                      -670.931297709924, -80.3358778625955, -45.2633587786262, -276.164122137405,
                                      25.3778625954199, -19.087786259542, -61.679389312977, 295.198473282443,
                                      -146.549618320611, -228.049618320611, 139.458015267176, 23.2709923664122,
                                      239.778625954198, 726.095419847328, -48.9198473282443, -68.7251908396947,
                                      -1422.96564885496, -443.656488549619, 385.103053435114, -177.137404580153,
                                      -176.931297709924, -239.431297709924, 297.732824427481, 463.110687022901,
                                      -439.503816793893, -324.019083969466, 456.80534351145, 15.9312977099236,
                                      538.641221374046, 603.358778625954, -40.4809160305344, -30.8969465648855,
                                      -70.6641221374046, 72.0534351145039, -17.0839694656489, -46.9122137404581,
                                      -7.78625954198472, -914.996183206107, -600.927480916031, -32.8396946564881,
                                      318.81679389313, -86.793893129771, -123.782442748092, -243.19465648855,
                                      189.206106870229, -65.1908396946564, -131.832061068702, 38.6297709923666,
                                      -29.0114503816794, -114.862595419847, 44.2137404580152, -57.4236641221374,
                                      -31.9503816793893, -8.11832061068695, -64.4274809160306, -65.8396946564886,
                                      -146.213740458015, -59.9770992366412, -27.0916030534351, -24.0267175572519,
                                      -33.6259541984733, -16.2137404580153, -21.4809160305344, -39.6374045801527,
                                      -35.7480916030534, -26.0954198473282, 7.35877862595422, -13.0954198473283,
                                      -9.91984732824439, 7.49618320610696, -60.7595419847328, -68.4541984732824,
                                      -234.568702290076, 224.706106870229, -47.706106870229, -0.91221374045798,
                                      -151.866412213741, -99.0763358778626, -51.6984732824428, 204.496183206107,
                                      -179.221374045801, -87.3816793893131, 25.4236641221373, 626.950381679388,
                                      -482.278625954199, -856.461832061068, 628.114503816793, 108.591603053435,
                                      -59.6946564885497, -101.213740458015, -47.9847328244273, -72.6603053435115,
                                      -9.11450381679403, 64.5534351145039, -726.87786259542, -644.263358778626,
                                      214.80534351145, 357.63358778626, 40.1145038167939, -47.9656488549618,
                                      2.31297709923656, -56.6717557251909, 22.5458015267175, -0.847328244274745,
                                      -98.0076335877862, -60.3053435114503, 508.30534351145, -914.026717557252,
                                      25.6412213740459, 691.973282442748, 35.3664122137405, 80.2900763358778,
                                      -91.4961832061069, -57.2557251908397, -39.973282442748, -638.263358778626,
                                      263.129770992366, 512.580152671755, 3.65267175572535, -31.9389312977098,
                                      68.4809160305343, -104.759541984733, -19.4847328244275, 110.309160305344,
                                      21.381679389313, -157.148854961832, -88.4465648854962, -159.687022900763,
                                      -1212.21374045802, 1178.00763358778, 224.484732824427, -21.1908396946565,
                                      -79.5229007633587, -13.8702290076337, 64.6030534351144, -116.778625954198,
                                      30.1183206106869, -154.625954198473, 114.515267175572, -88.9427480916029,
                                      -27.820610687023, -92.0954198473283, -40.469465648855, 9.50763358778621,
                                      58.8893129770993, -138.629770992367, 90.9160305343512, -44.1488549618321,
                                      -15.1603053435115, 91.675572519084, -31.7442748091604, -10.0954198473282,
                                      -27.2862595419847, -45.9656488549618, -244.629770992366, 27.6030534351145,
                                      -596.870229007633, -1047.50763358779, -7.72519083969485, 306.774809160305,
                                      208.351145038168, 139.958015267176, -255.396946564886, -654.106870229008,
                                      -113.709923664122, -141.885496183206, 242.12213740458, 439.248091603054,
                                      36.6145038167939, -274.251908396947, -443.461832061069, -14.3015267175571,
                                      656.083969465649, -261.164122137405, 272.217557251908, -174.763358778626,
                                      127.30534351145, -741.129770992367, -82.0725190839692, -144.648854961832,
                                      245.332061068702, 626.725190839694, 106.022900763359, -129.717557251908,
                                      -1045.09923664122, -303.034351145038, 887.893129770992, -48.2786259542004,
                                      153.129770992366, -72.5725190839695, -24.7557251908397, -45.2290076335878,
                                      -57.7748091603053, 71.7671755725191, -50.6221374045801, -156.209923664122,
                                      -52.0610687022901, -43.9770992366412, -115.236641221374, 94.8015267175571,
                                      19.5152671755726, -115.358778625954, -12.4427480916029, 12.736641221374,
                                      -46.4236641221373, -43.3854961832059, -39.3702290076334, -8.11450381679389,
                                      42.2480916030534, 22.3664122137405, -134.81679389313, 40.6908396946567,
                                      -24.1984732824429, -46.5267175572518, 30.43893129771, -66.4007633587785,
                                      24.496183206107, -35.469465648855, -21.416030534351, -86.6564885496182,
                                      -22.4541984732824, 1.73664122137414, 100.503816793893, -212.809160305344,
                                      90.6488549618321, 13.3854961832061, -128.038167938931, -85.8778625954201,
                                      -9.65267175572508, 135.381679389313, 20.7977099236641, -86.5038167938932,
                                      -73.4274809160306, 49.5229007633588, 24.4961832061067, -135.965648854962,
                                      -156.698473282443, 92.7595419847327, 101.690839694657, -125.419847328244,
                                      -265.351145038168, 89.7099236641222, 31.1984732824427, 42.9045801526719,
                                      -111.167938931298, -70.0305343511453, 10.5572519083969, -5.8931297709924,
                                      -128.496183206107, -137.374045801527, 172.351145038168, -22.2519083969464,
                                      64.6946564885495, -31.4312977099238, -79.8664122137404, 9.78244274809165,
                                      -74.0763358778627, -88.8816793893128, -101.667938931297, 5.63740458015281,
                                      47.2251908396946, -221.370229007634, -221.118320610687, 98.3206106870231,
                                      -0.351145038167887, -14.4465648854961, -15.9083969465649, -92.9961832061069,
                                      63.8587786259542, -68.4160305343512, -101.041984732825, 104.793893129771,
                                      -229.450381679389, 92.4999999999999, 63.6335877862595, -242.106870229008,
                                      23.6641221374046, 79.2709923664121, -129.015267175572, 98.1450381679389,
                                      -36.2022900763359, -20.6297709923665, 78.236641221374, 82.6641221374045,
                                      -127.282442748092, 12.2213740458017, -10.4809160305344, -60.6030534351144,
                                      -161.904580152671, -8.68702290076325, -22.8816793893129, -92.5038167938932,
                                      89.5877862595418, -197.87786259542, 92.2404580152673, 71.3664122137406,
                                      -95.0916030534351, 6.04961832061069, -18.442748091603, -63.0190839694656,
                                      126.950381679389, -44.5076335877861, -191.393129770992, 63.4541984732823,
                                      24.2786259541986, -165.038167938931, 146.652671755725, -242.068702290076,
                                      -773.221374045801, 113.778625954199, 375.568702290076, 90.3702290076336,
                                      -415.286259541985, 122.232824427481, 329.236641221374, -530.125954198473,
                                      -16.7175572519085, 397.450381679389, -321.652671755725, -44.2099236641221,
                                      43.4083969465649, -232.603053435114, 335.37786259542, -478.13358778626,
                                      -22.4694656488552, 296.278625954198, -271.656488549618, 396.095419847328,
                                      -361.408396946565, -39.8358778625953, 318.652671755725, -590.442748091603,
                                      23.5190839694658, 379.919847328244, -419.263358778626, 59.9847328244274,
                                      154.473282442748, -16.4274809160305, 35.5687022900764, -162.003816793893,
                                      -8.95038167938918, -79.5190839694657, -158.790076335878, 71.1030534351146,
                                      -71.3778625954201, -238.114503816794, 60.1832061068704, -81.8282442748094,
                                      -121.645038167939, 99.232824427481, -120.69465648855, -31.0,
                                      -43.4580152671756, -50.2022900763359, -40.1641221374046, -26.6335877862595,
                                      -57.206106870229, -224.824427480916, -504.92748091603, 946.164122137406,
                                      -46.6641221374046, -961.992366412214, 591.835877862596, 489.309160305344,
                                      -98.4885496183207, 87.9007633587787, -149.431297709924, -766.706106870229,
                                      1030.08778625954, -318.996183206107, -305.160305343512, 331.068702290076,
                                      -7.32824427480917, -0.736641221374053, 22.5381679389313, 593.629770992367,
                                      -551.935114503817, 133.950381679389, -72.7175572519084, 98.4580152671755,
                                      -57.7709923664122, 21.6297709923664, 6.85496183206105, -2.22900763358779,
                                      -26.8015267175573, 7.22519083969466, -5.53435114503816, -17.8435114503817,
                                      -9.80534351145038, -6.85114503816796, 1.97328244274807, -11.6335877862595,
                                      -28.1068702290076, -216.69465648855, 44.0648854961833, 162.916030534351,
                                      244.145038167939, -432.980916030534, 375.374045801527, 215.42748091603,
                                      315.603053435115, 803.312977099236, 252.954198473282, 14.6374045801527,
                                      -532.587786259542, -1035.8358778626, 260.732824427481, 966.641221374046,
                                      -27.6297709923664, -329.980916030534, -820.209923664121, -464.809160305344,
                                      66.7290076335879, 39.973282442748, 243.988549618321, -226.259541984733,
                                      188.645038167939, 1.67938931297707, -37.0114503816794, 3.28244274809164,
                                      -12.618320610687, -275.083969465649, 31.4198473282443, 1934.03435114503,
                                      -787.385496183206, 166.679389312977, -1359.15267175573, 564.797709923665,
                                      -193.660305343511, -234.893129770993, 37.0343511450382, 107.404580152672,
                                      130.969465648855, 61.6870229007634, 734.129770992367, -887.416030534352,
                                      -58.3435114503818, 13.5152671755725, -22.706106870229, 15.1870229007634,
                                      -267.461832061069, 399.851145038167, 553.01145038168, -1083.47709923664,
                                      63.6946564885496, 381.0, 106.309160305344, 181.091603053435,
                                      184.156488549618, -174.961832061069, 294.629770992367, -121.209923664122,
                                      -40.7290076335877, 353.198473282443, 7.58015267175558, -239.503816793893,
                                      56.9312977099237, -30.1297709923664, -39.175572519084, -59.2137404580153,
                                      -1170.90458015267, 12.0725190839695, 74.4465648854962, 177.919847328244,
                                      -76.2480916030534, 25.7442748091605, 112.332061068702, -169.93893129771,
                                      78.2251908396947, 38.8664122137404, 118.511450381679, -24.1412213740458,
                                      44.5648854961833, -80.5610687022901, -147.725190839695, -366.026717557252,
                                      -300.557251908397, -146.13358778626, -23.9351145038168, 221.63358778626,
                                      973.278625954199, 92.5152671755726, -20.3435114503816, 6.26717557251903,
                                      30.06106870229, -50.1488549618321, -20.1145038167939, 247.202290076336,
                                      -303.022900763359, -6.81679389312981, 345.774809160305, 99.8702290076337,
                                      -527.767175572519, -8.05725190839698, -29.1793893129771, 1011.19847328244,
                                      -901.767175572519, -5.07251908396955, 62.5190839694657, -55.0038167938932,
                                      44.2786259541984, 764.580152671756, -977.595419847329, 207.69465648855,
                                      749.209923664123, -68.0763358778627, -232.290076335878, -575.587786259542,
                                      -19.0687022900763, 822.438931297711, -854.095419847329, -47.057251908397,
                                      63.5038167938931, 676.614503816794, 33.1297709923664, -6.65267175572527,
                                      -135.461832061069, 106.477099236641, -102.160305343511, 1783.25572519084,
                                      -135.564885496183, -664.194656488549, -996.740458015267, 55.1068702290075,
                                      950.713740458015, -482.164122137404, -412.717557251909, -39.3854961832061,
                                      115.942748091603, 20.6183206106867, -10.2900763358779, -7.61068702290077,
                                      -3.49236641221375, -9.59541984732823, -9.36641221374049, 6.71755725190847,
                                      13.3091603053435, -133.81679389313, -129.625954198473, -222.675572519084,
                                      1181.5, 675.087786259542, 297.148854961832, 59.4503816793893,
                                      -628.423664122138, 378.667938931298, 822.576335877863, -312.664122137405,
                                      -12.5343511450382, -133.889312977099, -52.6106870229008, 34.6832061068703,
                                      21.030534351145, -338.824427480916, -294.25572519084, -356.041984732824,
                                      412.221374045801, -55.7748091603053, 5.44656488549618, -18.8435114503817,
                                      -11.8091603053435, -14.9847328244275, 7.90839694656488, -3.71755725190839,
                                      -13.9847328244275, 6.18702290076337, -27.6564885496184, -199.026717557252,
                                      -233.595419847328, 90.1221374045803, 16.587786259542, -39.0954198473283,
                                      181.984732824428, -333.229007633588, 218.614503816794, 348.828244274809,
                                      102.179389312977, -852.007633587786, 305.07251908397, -126.488549618321,
                                      -26.4427480916031, -57.9809160305342, -22.3358778625954, -257.293893129771,
                                      291.396946564886, 1041.13358778626, -249.87786259542, 3.57251908396953,
                                      64.8854961832061, -304.98854961832, -28.9885496183207, -2.95038167938937,
                                      9.59923664122138, -54.2748091603054, -228.064885496183, 929.690839694657,
                                      -111.599236641221, -219.301526717557, 84.3473282442747, 500.599236641221,
                                      -201.103053435115, -284.75572519084, 55.0496183206107, 33.0343511450383,
                                      -34.2099236641222, -84.4160305343512, -360.595419847328, 341.919847328244,
                                      603.404580152672, -195.484732824428, -188.458015267176, 64.3664122137405,
                                      -71.6106870229007, 0.862595419847303, -168.362595419847, -53.5267175572517,
                                      555.736641221374, 73.0992366412215, 822.152671755725, -610.469465648855,
                                      -198.087786259542, -323.519083969466, -100.980916030534, 109.610687022901,
                                      -166.942748091603, 80.0381679389313, -1.11450381679389, -811.561068702291,
                                      760.324427480917, 416.13358778626, 165.160305343511, -36.8167938931299,
                                      -233.839694656489, 171.778625954198, -132.973282442748, -410.080152671756,
                                      972.778625954199, -94.4923664122136, 51.026717557252, -7.06488549618322,
                                      7.12213740458017, -5.41984732824428, -53.0954198473281, 30.3931297709924,
                                      103.908396946565, 130.538167938931, 138.167938931298, -82.4122137404581,
                                      8.61068702290065, -39.5687022900762, -168.923664122137, -368.992366412214,
                                      214.309160305344, -17.9083969465648, -202.992366412214, -36.4312977099236,
                                      -37.6297709923664, -276.198473282443, 16.1679389312978, -27.4427480916045,
                                      -24.6908396946565, 64.93893129771, -33.4465648854962, -12.5954198473283,
                                      -53.2251908396949, 213.564885496184, -189.618320610687, -66.8969465648855,
                                      13.1526717557252, -34.5954198473282, 99.4541984732826, 6.4503816793894,
                                      60.3206106870228, -73.4923664122138, 122.370229007634, 0.47709923664118,
                                      -3.56488549618325, -112.091603053435, -18.6679389312976, 87.8893129770992,
                                      -27.9351145038169, 59.8511450381679, -4.42748091603056, -34.8587786259542,
                                      -118.68320610687, 141.473282442748, 18.6488549618331, -105.492366412214,
                                      73.3549618320612, -24.6717557251909, -14.412213740458, -1.53435114503817,
                                      -40.9236641221374, -359.167938931298, 131.351145038168, 400.618320610687,
                                      -166.702290076336, 32.9580152671754, 185.770992366412, -49.973282442748,
                                      -241.213740458015, 157.145038167939, 79.5916030534352, -67.2442748091603,
                                      52.3778625954197, -71.2786259541985, 97.4656488549618, 12.5114503816793,
                                      -72.9312977099237, 5.27480916030537, -65.2022900763359, 51.8206106870227,
                                      -122.0, 152.057251908397, -103.748091603054, 28.8244274809159,
                                      77.8244274809161, -21.1832061068703, -110.374045801526, 7.6106870229007,
                                      45.8282442748093, -37.1564885496183, -40.2557251908396, 44.9618320610687,
                                      -23.1564885496185, -141.259541984733, 100.053435114504, 38.5305343511451,
                                      66.5114503816793, 3.33587786259538, -27.4160305343512, 82.8053435114503,
                                      -34.4351145038167, -57.2938931297711, -157.80534351145, 135.954198473282,
                                      62.5725190839693, -127.614503816794, -189.923664122138, 214.225190839695,
                                      131.801526717557, -32.5419847328245, -343.480916030534, 422.171755725189,
                                      -11.0992366412214, -72.5801526717558, 31.3816793893128, -35.8320610687024,
                                      60.9007633587787, 85.232824427481, -126.076335877863, -78.3091603053435,
                                      145.015267175573, -15.7175572519084, -19.3702290076335, 158.458015267175,
                                      -196.648854961832, -21.9694656488548, 197.179389312977, -194.68320610687,
                                      -120.290076335878, 181.461832061069, 104.06106870229, -36.034351145038,
                                      -155.797709923664, 58.2748091603053, 204.900763358779, -198.625954198473,
                                      19.5648854961832, 36.8358778625955, 104.648854961832, -65.0725190839695,
                                      -229.839694656488, 204.347328244275, 2.75572519083966, -156.312977099237,
                                      85.4809160305346, 119.68320610687, -70.5992366412215, -46.2595419847328,
                                      76.6641221374046, 49.0190839694655, -143.057251908397, -98.1717557251906,
                                      187.187022900763, -254.545801526718, -13.6450381679391, 27.973282442748,
                                      -28.5610687022901, -47.0038167938931, -166.07251908397, 210.297709923664,
                                      26.7786259541984, 4.39694656488575, -99.8969465648854, 198.576335877863,
                                      131.24427480916, -247.347328244275, 208.687022900763, -121.919847328244,
                                      -67.408396946565, -57.9694656488548, -91.4351145038168, 348.587786259542,
                                      52.732824427481, -336.866412213741, 15.0725190839695, -67.6183206106871,
                                      101.515267175573, -59.1526717557253, 138.515267175573, 8.39312977099237,
                                      -9.52290076335882, 9.35114503816793, 51.6335877862596, -35.9465648854963,
                                      2.3702290076336, 28.2595419847328, 6.32061068702289, -48.9961832061069,
                                      -237.442748091603, 217.083969465649, 8.45801526717552, -53.969465648855,
                                      -30.4809160305344, 37.5076335877863, 0.87404580152684, 27.4198473282443,
                                      -22.9732824427481, -5.82442748091608, -13.6068702290076, -56.4045801526718,
                                      5.44656488549624, 8.63358778625955, 0.843511450381736, -15.1793893129771,
                                      8.13740458015267, -38.6832061068702, -173.572519083969, -61.0687022900765,
                                      11.8740458015268, 21.7595419847328, -78.416030534351, 56.7862595419848,
                                      -30.5801526717558, -79.2938931297709, 41.3740458015267, -15.6984732824428,
                                      -11.6526717557251, 52.3206106870229, -87.5801526717558, 9.16793893129773,
                                      3.36259541984733, 7.83969465648854, -12.1374045801527, -5.55725190839695,
                                      25.087786259542, -174.87786259542, -685.381679389313, -769.622137404581,
                                      32.6068702290077, 589.240458015267, 756.187022900763, 177.545801526718,
                                      148.954198473282, 257.442748091603, -666.568702290076, -646.603053435114,
                                      -1229.21374045802, 159.087786259542, 661.858778625954, 601.251908396946,
                                      10.5610687022901, -0.748091603053437, 0.522900763358781, -138.167938931298,
                                      -428.083969465649, 61.030534351145, 59.5152671755725, 10.0916030534351,
                                      -155.343511450382, 10.7862595419847, 2.07633587786262, 6.8854961832061,
                                      17.3893129770992, 14.4045801526718, 6.00763358778626, 10.5038167938931,
                                      13.6793893129771, 3.2175572519084, 23.4885496183206, -3.82824427480913,
                                      32.0343511450382, -89.1984732824427, -30.8625954198473, -270.916030534351,
                                      37.0496183206108, 265.450381679389, 65.1259541984732, -9.66030534351132,
                                      1.45419847328245, 94.8664122137405, -85.1259541984733, 47.0190839694656,
                                      140.591603053435, 45.4389312977099, 173.267175572519, 66.4083969465649,
                                      24.4885496183206, 0.622137404580152, 123.347328244275, -337.259541984733,
                                      6.01908396946554, 19.4312977099237, -37.3206106870229, 1.98473282442749,
                                      252.919847328245, 2.81297709923663, 63.3587786259541, -55.7290076335878,
                                      -19.3435114503817, -140.045801526717, -108.954198473283, -256.835877862595,
                                      133.942748091603, -5.77480916030545, 8.62595419847327, 79.9618320610687,
                                      52.7595419847327, -51.820610687023, 28.3435114503817, 24.175572519084,
                                      156.23282442748, 11.8664122137404, -260.240458015267, 354.25572519084,
                                      -3.4770992366412, 53.145038167939, -25.5114503816794, 46.8625954198474,
                                      -122.038167938931, 219.954198473282, 170.095419847328, 130.049618320611,
                                      -4.42748091603055, -112.656488549618, 22.7328244274809, -54.9923664122137,
                                      222.362595419847, -52.9160305343512, -177.862595419847, -62.7709923664122,
                                      69.0267175572519, 105.645038167939, -13.2404580152672, -51.4732824427482,
                                      -60.1641221374046, 4.587786259542, -4.90839694656488, -18.5534351145038,
                                      -117.534351145038, 12.4809160305344, 28.2480916030534, 5.38167938931299,
                                      71.8167938931298, -24.8740458015266, 112.442748091603, 6.55725190839696,
                                      77.7099236641222, 9.22137404580153, 28.3206106870229, 1.95801526717558,
                                      15.6412213740458, -29.145038167939, 128.709923664122, 84.912213740458,
                                      62.4427480916031, 11.7557251908397, 10.4465648854962, 17.3549618320611,
                                      -231.870229007634, 80.1106870229008, -62.9580152671756, -14.7366412213741,
                                      32.5267175572519, 40.1412213740458, -27.8053435114504, -30.3129770992367,
                                      91.4351145038168, 38.1374045801528, 82.1564885496183, -52.618320610687,
                                      41.526717557252, 14.7786259541985, 3.61068702290076, -83.6908396946565,
                                      -114.568702290076, 41.7786259541985, 292.0, -129.332061068702,
                                      -97.3969465648853, -43.8206106870228, 21.2175572519083, -14.8320610687022,
                                      87.0687022900765, 5.56106870229008, -36.2671755725191, -14.5801526717556,
                                      -26.4732824427481, -49.1870229007634, 58.3854961832061, 10.5992366412214,
                                      23.6106870229008, 162.606870229008, 57.614503816794, 43.4656488549619,
                                      -345.618320610687, -66.8893129770993, 146.175572519084, -485.404580152672,
                                      20.5343511450381, -159.240458015267, 27.0954198473283, 37.7786259541986,
                                      -109.961832061069, -76.8435114503817, -88.6183206106871, 3.30534351145039,
                                      78.851145038168, 174.011450381679, 8.89694656488549, 10.912213740458,
                                      6.96183206106869, 21.4198473282443, 8.67557251908398, 16.9465648854962,
                                      9.85496183206107, -174.404580152672, 0.477099236641252, 36.2290076335879,
                                      -421.988549618321, -303.729007633588, -55.5229007633588, -168.148854961832,
                                      -41.5610687022901, 108.931297709924, -45.3740458015265, 84.6259541984733,
                                      27.7251908396946, 0.797709923664162, 39.2061068702291, 1.71374045801535,
                                      4.91221374045801, 32.4770992366412, 171.412213740458, -12.7022900763359,
                                      -110.049618320611, 2.40458015267176, 7.09541984732825, 17.1564885496183,
                                      9.31297709923664, 7.73282442748092, 6.08396946564885, 10.9618320610687,
                                      11.3587786259542, 10.293893129771, 32.5801526717557, 60.9770992366411,
                                      -86.9312977099237, 98.6908396946566, -49.2366412213741, 7.37022900763366,
                                      315.973282442748, -273.641221374046, 70.3282442748092, -102.198473282443,
                                      101.935114503817, 27.8549618320612, -91.7633587786259, -9.12977099236629,
                                      48.3167938931297, -46.6183206106871, 78.6183206106871, 1296.10687022901,
                                      -489.797709923664, -48.2748091603053, -559.129770992367, 42.118320610687,
                                      8.59923664122137, 69.7442748091603, -89.2290076335878, 36.9770992366412,
                                      27.7595419847328, 927.442748091602, -613.549618320611, 243.538167938931,
                                      -225.980916030534, -454.30534351145, -0.0152671755726033, 30.6946564885496,
                                      -1.04961832061073, 14.5496183206107, 10.2404580152671, -4.61450381679385,
                                      -38.9465648854962, -12.496183206107, 995.374045801526, -308.641221374046,
                                      640.51145038168, -342.942748091603, -163.125954198473, 49.7175572519084,
                                      5.90076335877864, 19.2709923664123, 506.675572519084, -15.5877862595423,
                                      415.011450381679, -442.870229007634, 317.996183206107, -288.324427480916,
                                      244.984732824428, -174.160305343511, -5.30534351145024, 21.5229007633587,
                                      -5.87786259541985, -4.13740458015269, 161.824427480916, 567.36641221374,
                                      146.278625954199, 51.8473282442749, 92.9045801526717, -105.141221374046,
                                      -136.339694656488, 277.862595419847, -249.977099236641, 44.3587786259541,
                                      10.0305343511451, 82.8740458015268, -36.06106870229, 13.175572519084,
                                      -0.843511450381667, 21.3091603053435, -2.25190839694669, 14.8969465648855,
                                      27.8320610687023, -151.610687022901, 31.5229007633588, 9.42748091603053,
                                      -36.1221374045802, 33.4160305343511, 16.0992366412214, 30.3893129770993,
                                      -2.2824427480916, 11.1870229007633, 12.2328244274809, -12.2671755725191,
                                      -22.5458015267176, -58.3167938931297, -5.15267175572521, -88.4885496183207,
                                      48.385496183206, -33.0305343511452, 25.3396946564885, -98.8129770992365,
                                      -51.2862595419846, 305.419847328244, 113.408396946565, 16.675572519084,
                                      -27.7290076335877, -3.91221374045801, 6.04580152671764, 23.4503816793893,
                                      228.37786259542, 27.3969465648855, -69.1374045801527, -23.824427480916,
                                      64.7786259541985, -68.9160305343512, 12.0687022900764, -23.1221374045802,
                                      133.591603053435, 44.8511450381679, -11.9007633587788, -13.3167938931298,
                                      -12.7251908396947, -124.118320610687, 305.63358778626, -21.0534351145038,
                                      -40.3893129770991, 17.912213740458, 10.5343511450382, 11.1564885496183,
                                      -3.92748091603055, -33.1793893129772, 42.2786259541985, 30.7137404580152,
                                      35.8358778625955, -16.0801526717558, -36.6183206106871, 47.2977099236642,
                                      34.7099236641222, -48.7709923664121, 61.56106870229, -48.2442748091604,
                                      -38.9465648854961, 70.3549618320611, -44.6412213740456, 50.9809160305343,
                                      40.6564885496181, -28.9580152671757, -19.7137404580154, 29.7290076335879,
                                      -32.645038167939, -22.0725190839694, 1.02671755725197, -124.610687022901,
                                      33.003816793893, -49.2786259541985, 11.0687022900763, 32.2213740458016,
                                      55.8625954198471, -1.01145038167946, -70.4618320610687, 56.8167938931299,
                                      -29.1946564885495, 8.39312977099228, 33.2213740458016, -65.8396946564886,
                                      54.6832061068701, -2.64122137404591, 6.88167938931279, 40.6984732824428,
                                      117.797709923664, -29.6870229007635, 12.9580152671756, 43.522900763359,
                                      52.5496183206108, 39.9618320610687, 42.4351145038169, -31.8931297709924,
                                      50.6221374045802, -36.8473282442748, -27.2557251908397, -119.393129770992,
                                      -16.9312977099235, 36.1030534351144, 44.7977099236641, -39.5,
                                      -14.5305343511451, -4.66030534351153, 39.3854961832062, 7.86641221374055,
                                      -122.015267175573, 34.0725190839695, 1.22519083969468, -5.86641221374043,
                                      -16.1068702290077, 5.25572519083993, 124.385496183206, 29.4007633587787,
                                      41.7061068702291, 5.63740458015264, 0.923664122137394, -68.1106870229008,
                                      54.8358778625952, -76.7022900763359, -38.3664122137404, 75.2671755725192,
                                      46.7633587786258, -90.9427480916032, -37.5190839694655, 85.6946564885497,
                                      21.3320610687022, -71.3358778625955, 4.08396946564861, 71.5419847328244,
                                      -5.54961832061095, -33.7290076335878, 58.4007633587785, -2.85114503816785,
                                      -87.8587786259542, 17.851145038168, 63.057251908397, -68.0534351145038,
                                      -35.6030534351144, 58.2099236641221, -6.81297709923655, -70.083969465649,
                                      -33.6412213740455, 141.557251908397, -20.7671755725191, -68.68320610687,
                                      62.8816793893129, 26.0801526717558, 53.912213740458, 28.4122137404582,
                                      -55.8396946564885, 30.8473282442747, 27.5763358778627, -70.7633587786258,
                                      54.152671755725, 37.8282442748091, -35.2977099236641, -32.8167938931297,
                                      91.381679389313, -9.58396946564895, 10.0610687022901, -25.6412213740459,
                                      94.8091603053436, 59.2022900763359, 25.5648854961831, 6.13740458015267,
                                      -25.3320610687023, 30.9465648854962, 9.91603053435112, 20.3091603053435,
                                      -2.5229007633588, 45.5763358778626, 10.4160305343511, 2.43893129770993,
                                      -212.713740458015, 232.446564885496, 18.0267175572519, -30.5381679389313,
                                      13.8396946564885, 35.1526717557252, 33.4961832061068, 21.8511450381679,
                                      -34.6984732824428, 38.1564885496183, 23.7251908396946, 22.320610687023,
                                      7.43129770992364, 25.1145038167939, 39.0877862595419, -64.118320610687,
                                      41.7671755725192, 25.0114503816794, -1.20229007633601, -3.3816793893129,
                                      33.8587786259541, 2.24427480916026, 68.767175572519, 5.86259541984723,
                                      5.87022900763363, -2.2709923664123, 43.5992366412214, -8.6679389312977,
                                      15.5763358778627, 17.7480916030535, -16.7748091603054, 3.92366412213742,
                                      19.7709923664122, 15.5114503816794, 9.27099236641221, 12.1106870229008,
                                      9.08778625954198, -9.56870229007621, -275.687022900763, 500.175572519085,
                                      30.030534351145, -144.858778625954, 467.965648854962, -24.1870229007635,
                                      80.8778625954199, -20.2251908396947, 9.35496183206104, -241.492366412214,
                                      473.038167938932, -280.320610687023, 261.893129770992, 24.2099236641222,
                                      43.6564963029203, 46.2861690667736, 62.6089210667731, 1477.39770022625,
                                      1249.04046810364, 690.290580207564, 520.93971376067, 307.744232458748,
                                      1699.1074063969, 389.425535726241, 166.368752618625, 147.225820042138,
                                      151.096815366445, 101.77024810479, 53.5210401509341, 90.9456498413243,
                                      84.594672811569, 219.422807320243, 196.662484098029, 98.0616500271451,
                                      140.858054603656, 1190.21342730106, 894.324958783218, 682.522751119727,
                                      806.244720033209, 1789.68829901941, 1931.32354777965, 1173.27840880788,
                                      982.804085099886, 1402.88414558361, 829.63612690173, 435.753100868339,
                                      778.118732391248, 1153.04624927094, 805.045881250165, 1252.69405435508,
                                      279.237882119327, 386.700746208112, 1818.00283637267, 1109.97600143793,
                                      524.638049686407, 107.361448606152, 717.42382958333, 883.68404542842,
                                      1794.51195514947, 74.5323573821491, 156.968929143036, 440.162365752339,
                                      296.345109507168, 1569.61842667648, 1754.8661500064, 2089.11520957783,
                                      1432.61967604573, 1734.39750167248, 1476.55866997911, 1513.97618817559,
                                      745.831814399275, 1106.95391078039, 164.197509330353, 576.720771805012,
                                      1108.01638870568, 210.480634889202, 885.501103369974, 1662.99287230375,
                                      253.108662766194, 307.73360755971, 455.149197233148, 351.384991273893,
                                      1555.7422415088, 2005.62344425116, 1571.89806441297, 1258.42802767221,
                                      201.730039581409, 469.162012501464, 245.017903632913, 432.049204686169,
                                      1482.37094410976, 295.393277508427, 715.406207899911, 420.192668296756,
                                      1294.89869679561, 1988.36116097367, 1174.86334102066, 990.599558352443,
                                      329.588491118203, 138.873175303234, 343.977634265335, 247.966583876836,
                                      1389.88467712155, 287.387422304279, 340.702038005338, 470.012822098604,
                                      245.99332188868, 1432.76910275195, 780.15535452325, 390.072742957793,
                                      431.971906869104, 186.899376809331, 449.511816729471, 187.395598432215,
                                      495.55359837335, 580.614128175256, 351.617427993279, 465.023027574362,
                                      928.044072417713, 312.102554559155, 85.3723354277397, 485.551693309358,
                                      1902.81447872754, 400.814869655689, 231.914000972603, 267.401553363744,
                                      183.313080150031, 380.323083990833, 169.031821992274, 492.675895897401,
                                      651.589425224108, 417.827988416557, 729.680635522418, 1149.04941587924,
                                      1249.37308130323, 394.47579816273, 98.8721434223333, 1189.45305639445,
                                      1050.11653399567, 250.308240111737, 1684.66090333872, 764.15591339007,
                                      1280.68642921561, 991.209001344375, 1344.79275633522, 1432.30601538382,
                                      1134.2441258532, 304.434366874449, 398.622615931974, 921.966269753682,
                                      406.824263425754, 1169.40367435085, 979.353950117394, 205.675611659999,
                                      280.886800924568, 1673.22433631819, 201.052114506021, 732.074472976966,
                                      1559.45470739065, 1198.37623627204, 1242.55522994041, 1917.41738223139,
                                      849.659128362778, 1316.06500494563, 1163.7479338604, 1034.35210958222,
                                      1355.01169580829, 956.707497326469, 844.433436584581, 148.917469775485,
                                      571.840275927231, 1145.45324000414, 76.1120208526484, 55.4938241498732,
                                      76.9203173582858, 170.141371160322, 159.753265689538, 177.542590524695,
                                      54.0591879221186, 953.978227648453, 955.538191215659, 1256.97807278971,
                                      1506.72885089163, 909.915756070183, 462.62745759783, 587.72998442111,
                                      823.729626382038, 816.849024080464, 1147.02158558024, 447.188323686336,
                                      154.753605244183, 271.170378313629, 381.85085813225, 373.230816967644,
                                      214.489019480617, 471.171008547099, 443.234022226526, 674.540090951393,
                                      489.309936571639, 87.1330786450366, 38.6964448539275, 42.7155637332477,
                                      50.9244171534339, 35.4371526203367, 40.2082921430905, 50.942628860669,
                                      52.085412156833, 39.3470069580736, 71.0642518914843, 537.502520742158,
                                      322.589296749743, 303.430372286288, 398.994964464669, 250.24547180384,
                                      742.924599524357, 541.508856376345, 396.259591121566, 643.994611236625,
                                      493.116866146701, 1022.01178653503, 589.64735818812, 626.134595013185,
                                      349.997855963359, 276.09536251016, 212.681897409759, 2108.34855539806,
                                      880.891223560207, 1993.77528728897, 1325.64998860521, 612.077489556479,
                                      212.775918404124, 517.197660879077, 1006.29136015215, 397.508561268609,
                                      212.030681728702, 986.909454627588, 1571.09110041201, 1716.55059782655,
                                      1225.80275838001, 1020.79915570694, 644.670505020774, 665.357452470816,
                                      857.995344477786, 645.45592113361, 273.218428532861, 359.18010087554,
                                      730.908433777623, 361.287481235039, 1795.45687425963, 1453.25601148593,
                                      1537.93410628024, 1300.58283139244, 658.495704931014, 273.534663019355,
                                      146.590016297088, 282.532572572249, 627.347917567388, 2162.00612644971,
                                      1519.71854198386, 1411.57689910904, 1407.4827649551, 1018.46738974483,
                                      1174.11338076141, 736.670661968277, 439.351786561729, 501.407326962606,
                                      446.094137432926, 480.464853646386, 210.239388235558, 2257.70081927242,
                                      1888.59482324537, 1547.90403847645, 817.163984325502, 312.666471639801,
                                      645.080017075997, 1036.56888166337, 486.763076908453, 598.167824631707,
                                      1183.54172199456, 426.805738253344, 499.400923332475, 471.261261608658,
                                      147.760825403201, 114.992821460565, 383.440044522217, 248.647513305826,
                                      566.315861885324, 399.997552290746, 439.724961043748, 130.299243400771,
                                      225.924821506256, 791.92269009107, 218.710664907002, 395.345237226243,
                                      309.61642227226, 342.480578502261, 497.629004415016, 397.18678528086,
                                      607.474715307183, 1532.48072471399, 599.55077412123, 1878.31776058125,
                                      1823.66293471132, 532.262981939142, 354.967507057784, 1345.56452246637,
                                      1293.64158647666, 1827.06410983508, 1940.13453341578, 875.769088245233,
                                      256.436307336326, 391.812676168353, 1343.57897748938, 611.289816378927,
                                      1106.76305191364, 1518.60248999647, 511.759791959728, 511.436474255948,
                                      490.073402078919, 758.658154030849, 980.27233252742, 735.776808447981,
                                      1766.6739120321, 1013.95321957103, 538.072289792871, 153.744316301752,
                                      1269.2390710172, 974.690385419169, 1777.39076087342, 1703.79877183241,
                                      659.06926975489, 139.935812489294, 75.2298872989013, 60.5606770251882,
                                      108.689662550899, 519.940489945327, 466.952782593284, 923.407039520733,
                                      848.283568366295, 792.591076937416, 774.722751221597, 447.638986636969,
                                      647.099074273116, 848.599161315582, 758.390572355455, 567.604204162631,
                                      488.563091940319, 569.770107842229, 818.979687532732, 616.227372793396,
                                      416.894098717682, 626.738360984882, 686.012261611423, 595.108505684503,
                                      488.992855207704, 636.86418880593, 567.395457869122, 688.84727560982,
                                      390.382119545855, 439.956570467743, 519.972325178503, 587.603220423457,
                                      392.674501105739, 407.749588824068, 596.143918201811, 640.721969541745,
                                      459.453567478121, 676.641286845136, 636.010100523609, 677.505259616547,
                                      663.308021300762, 416.209414200081, 706.180935441294, 615.593891413426,
                                      586.077380541661, 472.027808061964, 594.452332296712, 582.432167027345,
                                      803.142320015768, 540.029831000295, 597.045652361098, 688.422185536129,
                                      794.394059675501, 479.494800198302, 786.434579673312, 1719.57944591337,
                                      591.446812580661, 683.143806923409, 476.223966721354, 558.063025007498,
                                      597.162392812717, 714.32587355677, 451.823431561963, 622.22926856322,
                                      540.918664502007, 671.996776070147, 491.86524831562, 409.554936332504,
                                      543.259544360432, 530.6388736766, 614.970000371839, 504.387617640887,
                                      656.122577945929, 630.556014380176, 667.962383987029, 435.643509372992,
                                      612.641048298365, 1030.69002016005, 1063.95570395102, 840.964911977213,
                                      1052.35794394397, 855.4030123, 967.136879725403, 844.378328079844,
                                      947.527589304871, 785.324009847686, 684.738169173247, 1007.52060478065,
                                      842.276850443353, 887.989896679385, 758.594490560088, 845.891768910717,
                                      873.910782276269, 880.011691027565, 863.629298708915, 777.571509984279,
                                      876.151317997017, 896.977746638014, 852.883334821545, 904.755901890663,
                                      1195.46196025078, 987.376431819232, 961.467730310508, 795.612786454109,
                                      829.829862800713, 803.914188095636, 839.202105875616, 811.679725951747,
                                      865.457533573315, 844.538265547145, 936.075374343578, 1011.48886181072,
                                      903.380124509249, 756.1180524445, 999.789514860939, 886.717774184976,
                                      839.26202844058, 1032.15329119535, 903.671890072196, 912.21507396834,
                                      1190.19667015178, 588.330840079842, 530.032726327992, 536.920387260234,
                                      633.849297873986, 289.815425098749, 656.325408129342, 680.631552621818,
                                      422.691546439744, 571.857819711334, 740.646064897021, 451.046780918596,
                                      681.550366906359, 747.084195907654, 651.595784232051, 522.466696110343,
                                      493.29918153576, 627.026884184397, 691.345873592339, 757.205884444731,
                                      394.035928311227, 352.594179428769, 597.5609470446, 665.453913559779,
                                      453.730455116463, 584.282910045024, 694.36203198276, 500.147659949161,
                                      375.612876927357, 410.621532900922, 720.260290532832, 608.298857354178,
                                      614.118371069359, 617.979214443424, 725.132938912928, 853.184954120941,
                                      646.354292090316, 826.413614470155, 563.798030114366, 764.24751857348,
                                      764.009059359403, 662.301904701626, 525.088877793487, 73.2343993264733,
                                      73.4389494947073, 62.6013034706029, 60.6715775178128, 78.8804778003464,
                                      87.2855570659675, 597.749577358535, 1226.83097122663, 2369.93148786272,
                                      300.307604209516, 1545.46841798609, 1763.22992957064, 1329.91500189702,
                                      727.967472460446, 1069.77299638745, 929.830993039887, 1367.64292578804,
                                      2294.79475298504, 583.410655114018, 1895.80107179767, 1551.48831934456 };

    double dist[500];
    int c_k;
    double s;
    signed char b_Y[3];
    int exitg4;
    int exitg3;
    short outsize_idx_0;
    boolean_T guard1 = false;
    int exitg2;
    boolean_T exitg1;
    for (k = 0; k <= 499; k += 2) {
        if (a[k] <= a[k + 1]) {
            idx[k] = k + 1;
            idx[k + 1] = k + 2;
        } else {
            idx[k] = k + 2;
            idx[k + 1] = k + 1;
        }
    }

    i = 2;
    while (i < 500) {
        nb = i << 1;
        nbins = 1;
        for (low_ip1 = 1 + i; low_ip1 < 501; low_ip1 = qEnd + i) {
            high_i = nbins;
            mid_i = low_ip1;
            qEnd = nbins + nb;
            if (qEnd > 501) {
                qEnd = 501;
            }

            k = 0;
            kEnd = qEnd - nbins;
            while (k + 1 <= kEnd) {
                if (a[idx[high_i - 1] - 1] <= a[idx[mid_i - 1] - 1]) {
                    iwork[k] = idx[high_i - 1];
                    high_i++;
                    if (high_i == low_ip1) {
                        while (mid_i < qEnd) {
                            k++;
                            iwork[k] = idx[mid_i - 1];
                            mid_i++;
                        }
                    }
                } else {
                    iwork[k] = idx[mid_i - 1];
                    mid_i++;
                    if (mid_i == qEnd) {
                        while (high_i < low_ip1) {
                            k++;
                            iwork[k] = idx[high_i - 1];
                            high_i++;
                        }
                    }
                }

                k++;
            }

            for (k = 0; k + 1 <= kEnd; k++) {
                idx[(nbins + k) - 1] = iwork[k];
            }

            nbins = qEnd;
        }

        i = nb;
    }

    emxInit_real_T1(&Uc, 1);
    low_ip1 = Uc->size[0];
    Uc->size[0] = 500;
    emxEnsureCapacity((emxArray__common *)Uc, low_ip1, (int)sizeof(double));
    for (k = 0; k < 500; k++) {
        Uc->data[k] = a[idx[k] - 1];
    }

    nb = -1;
    k = 0;
    while (k + 1 <= 500) {
        nbins = (int)Uc->data[k];
        do {
            exitg5 = 0;
            k++;
            if (k + 1 > 500) {
                exitg5 = 1;
            } else {
                eok = (std::abs((double)nbins - Uc->data[k]) < eps((double)nbins / 2.0));
                if (!eok) {
                    exitg5 = 1;
                }
            }
        } while (exitg5 == 0);

        nb++;
        Uc->data[nb] = nbins;
    }

    emxInit_real_T1(&nn, 1);
    low_ip1 = Uc->size[0];
    if (1 > nb + 1) {
        i2 = -1;
    } else {
        i2 = nb;
    }

    Uc->size[0] = i2 + 1;
    emxEnsureCapacity((emxArray__common *)Uc, low_ip1, (int)sizeof(double));
    emxInit_real_T(&edges, 2);
    emxInit_real_T(&b_testSamples, 2);
    emxInit_real_T(&r8, 2);
    for (i = 0; i < 5; i++) {
        nbins = testSamples->size[1];
        low_ip1 = b_testSamples->size[0] * b_testSamples->size[1];
        b_testSamples->size[0] = 1;
        b_testSamples->size[1] = nbins;
        emxEnsureCapacity((emxArray__common *)b_testSamples, low_ip1, (int)sizeof
                (double));
        for (low_ip1 = 0; low_ip1 < nbins; low_ip1++) {
            b_testSamples->data[b_testSamples->size[0] * low_ip1] = testSamples->
                    data[i + testSamples->size[0] * low_ip1];
        }

        low_ip1 = r8->size[0] * r8->size[1];
        r8->size[0] = 500;
        r8->size[1] = b_testSamples->size[1];
        emxEnsureCapacity((emxArray__common *)r8, low_ip1, (int)sizeof(double));
        for (low_ip1 = 0; low_ip1 < 500; low_ip1++) {
            nbins = b_testSamples->size[1];
            for (nb = 0; nb < nbins; nb++) {
                r8->data[low_ip1 + r8->size[0] * nb] = b_testSamples->data
                [b_testSamples->size[0] * nb];
            }
        }

        for (low_ip1 = 0; low_ip1 < 64; low_ip1++) {
            for (nb = 0; nb < 500; nb++) {
                x[nb + 500 * low_ip1] = tX[nb + 500 * low_ip1] - r8->data[nb + 500 *
                                                                               low_ip1];
            }
        }

        for (b_k = 1; b_k < 32001; b_k++) {
            c_k = b_k;
            y[c_k - 1] = x[c_k - 1] * x[c_k - 1];
        }

        for (nbins = 0; nbins < 500; nbins++) {
            s = y[nbins];
            for (k = 0; k < 63; k++) {
                s += y[nbins + (k + 1) * 500];
            }

            dist[nbins] = s;
        }

        sort(dist, idx);
        for (nbins = 0; nbins < 3; nbins++) {
            b_Y[nbins] = a[idx[nbins] - 1];
        }

        nbins = Uc->size[0];
        low_ip1 = edges->size[0] * edges->size[1];
        edges->size[0] = 1;
        edges->size[1] = (short)(nbins + 1);
        emxEnsureCapacity((emxArray__common *)edges, low_ip1, (int)sizeof(double));
        k = 0;
        do {
            exitg4 = 0;
            nbins = Uc->size[0];
            if (k <= nbins - 2) {
                edges->data[1 + k] = Uc->data[k] + (Uc->data[1 + k] - Uc->data[k]) / 2.0;
                k++;
            } else {
                exitg4 = 1;
            }
        } while (exitg4 == 0);

        edges->data[0] = rtMinusInf;
        edges->data[edges->size[1] - 1] = rtInf;
        k = 1;
        do {
            exitg3 = 0;
            nbins = Uc->size[0];
            if (k - 1 <= nbins - 2) {
                edges->data[k] += eps(edges->data[k]);
                k++;
            } else {
                exitg3 = 1;
            }
        } while (exitg3 == 0);

        outsize_idx_0 = (short)edges->size[1];
        low_ip1 = nn->size[0];
        nn->size[0] = outsize_idx_0;
        emxEnsureCapacity((emxArray__common *)nn, low_ip1, (int)sizeof(double));
        nbins = outsize_idx_0;
        for (low_ip1 = 0; low_ip1 < nbins; low_ip1++) {
            nn->data[low_ip1] = 0.0;
        }

        nbins = edges->size[1];
        guard1 = false;
        if (nbins > 1) {
            nb = 1;
            do {
                exitg2 = 0;
                if (nb + 1 <= nbins) {
                    if (!(edges->data[nb] >= edges->data[nb - 1])) {
                        eok = false;
                        exitg2 = 1;
                    } else {
                        nb++;
                    }
                } else {
                    guard1 = true;
                    exitg2 = 1;
                }
            } while (exitg2 == 0);
        } else {
            guard1 = true;
        }

        if (guard1) {
            eok = true;
        }

        if (!eok) {
            low_ip1 = nn->size[0];
            nn->size[0] = outsize_idx_0;
            emxEnsureCapacity((emxArray__common *)nn, low_ip1, (int)sizeof(double));
            nbins = outsize_idx_0;
            for (low_ip1 = 0; low_ip1 < nbins; low_ip1++) {
                nn->data[low_ip1] = rtNaN;
            }
        } else {
            nbins = 0;
            for (k = 0; k < 3; k++) {
                nb = 0;
                if ((b_Y[nbins] >= edges->data[0]) && (b_Y[nbins] < edges->data
                [edges->size[1] - 1])) {
                    nb = 1;
                    low_ip1 = 2;
                    high_i = edges->size[1];
                    while (high_i > low_ip1) {
                        mid_i = (nb >> 1) + (high_i >> 1);
                        if (((nb & 1) == 1) && ((high_i & 1) == 1)) {
                            mid_i++;
                        }

                        if (b_Y[nbins] >= edges->data[mid_i - 1]) {
                            nb = mid_i;
                            low_ip1 = mid_i + 1;
                        } else {
                            high_i = mid_i;
                        }
                    }
                }

                if (b_Y[nbins] == edges->data[edges->size[1] - 1]) {
                    nb = edges->size[1];
                }

                if (nb > 0) {
                    nn->data[nb - 1]++;
                }

                nbins++;
            }
        }

        low_ip1 = edges->size[0] * edges->size[1];
        edges->size[0] = 1;
        edges->size[1] = nn->size[0] - 1;
        emxEnsureCapacity((emxArray__common *)edges, low_ip1, (int)sizeof(double));
        for (k = 0; k <= nn->size[0] - 2; k++) {
            edges->data[k] = nn->data[k];
        }

        if (nn->size[0] - 1 > 0) {
            edges->data[edges->size[1] - 1] += nn->data[nn->size[0] - 1];
        }

        nbins = 1;
        nb = edges->size[1];
        s = edges->data[0];
        low_ip1 = 0;
        if (edges->size[1] > 1) {
            if (rtIsNaN(edges->data[0])) {
                high_i = 2;
                exitg1 = false;
                while ((!exitg1) && (high_i <= nb)) {
                    nbins = high_i;
                    if (!rtIsNaN(edges->data[high_i - 1])) {
                        s = edges->data[high_i - 1];
                        low_ip1 = high_i - 1;
                        exitg1 = true;
                    } else {
                        high_i++;
                    }
                }
            }

            if (nbins < edges->size[1]) {
                while (nbins + 1 <= nb) {
                    if (edges->data[nbins] > s) {
                        s = edges->data[nbins];
                        low_ip1 = nbins;
                    }

                    nbins++;
                }
            }
        }

        Y[i] = Uc->data[low_ip1];
    }

    emxFree_real_T(&r8);
    emxFree_real_T(&b_testSamples);
    emxFree_real_T(&nn);
    emxFree_real_T(&edges);
    emxFree_real_T(&Uc);
}

//
// Arguments    : const double x[100]
//                int DOF
//                emxArray_real_T *coeffOut
//                emxArray_real_T *scoreOut
//                emxArray_real_T *latentOut
//                double tsquared[5]
//                emxArray_real_T *explained
// Return Type  : void
//
static void localSVD(const double x[100], int DOF, emxArray_real_T *coeffOut,
                     emxArray_real_T *scoreOut, emxArray_real_T *latentOut,
                     double tsquared[5], emxArray_real_T *explained)
{
    double A[100];
    double s[6];
    int ixstart;
    double e[20];
    double work[5];
    double score[25];
    double Vf[400];
    int q;
    int m;
    int iter;
    boolean_T apply_transform;
    double ztest0;
    int qs;
    int kase;
    double snorm;
    double rt;
    double ztest;
    double coeff[100];
    int exitg3;
    emxArray_real_T *standScores;
    boolean_T exitg2;
    double f;
    double varargin_1[5];
    double mtmp;
    boolean_T exitg1;
    double sqds;
    memcpy(&A[0], &x[0], 100U * sizeof(double));
    for (ixstart = 0; ixstart < 6; ixstart++) {
        s[ixstart] = 0.0;
    }

    memset(&e[0], 0, 20U * sizeof(double));
    for (ixstart = 0; ixstart < 5; ixstart++) {
        work[ixstart] = 0.0;
    }

    memset(&score[0], 0, 25U * sizeof(double));
    memset(&Vf[0], 0, 400U * sizeof(double));
    for (q = 0; q < 5; q++) {
        iter = q + 5 * q;
        apply_transform = false;
        if (q + 1 <= 4) {
            ztest0 = xnrm2(5 - q, A, iter + 1);
            if (ztest0 > 0.0) {
                apply_transform = true;
                if (A[iter] < 0.0) {
                    s[q] = -ztest0;
                } else {
                    s[q] = ztest0;
                }

                if (std::abs(s[q]) >= 1.0020841800044864E-292) {
                    ztest0 = 1.0 / s[q];
                    ixstart = (iter - q) + 5;
                    for (kase = iter; kase + 1 <= ixstart; kase++) {
                        A[kase] *= ztest0;
                    }
                } else {
                    ixstart = (iter - q) + 5;
                    for (kase = iter; kase + 1 <= ixstart; kase++) {
                        A[kase] /= s[q];
                    }
                }

                A[iter]++;
                s[q] = -s[q];
            } else {
                s[q] = 0.0;
            }
        }

        for (qs = q + 1; qs + 1 < 21; qs++) {
            kase = q + 5 * qs;
            if (apply_transform) {
                xaxpy(5 - q, -(xdotc(5 - q, A, iter + 1, A, kase + 1) / A[q + 5 * q]),
                      iter + 1, A, kase + 1);
            }

            e[qs] = A[kase];
        }

        if (q + 1 <= 4) {
            for (ixstart = q; ixstart + 1 < 6; ixstart++) {
                score[ixstart + 5 * q] = A[ixstart + 5 * q];
            }
        }

        ztest0 = b_xnrm2(19 - q, e, q + 2);
        if (ztest0 == 0.0) {
            e[q] = 0.0;
        } else {
            if (e[q + 1] < 0.0) {
                e[q] = -ztest0;
            } else {
                e[q] = ztest0;
            }

            ztest0 = e[q];
            if (std::abs(e[q]) >= 1.0020841800044864E-292) {
                ztest0 = 1.0 / e[q];
                for (kase = q + 1; kase + 1 < 21; kase++) {
                    e[kase] *= ztest0;
                }
            } else {
                for (kase = q + 1; kase + 1 < 21; kase++) {
                    e[kase] /= ztest0;
                }
            }

            e[q + 1]++;
            e[q] = -e[q];
            if (q + 2 <= 5) {
                for (ixstart = q + 1; ixstart + 1 < 6; ixstart++) {
                    work[ixstart] = 0.0;
                }

                for (qs = q + 1; qs + 1 < 21; qs++) {
                    b_xaxpy(4 - q, e[qs], A, (q + 5 * qs) + 2, work, q + 2);
                }

                for (qs = q + 1; qs + 1 < 21; qs++) {
                    c_xaxpy(4 - q, -e[qs] / e[q + 1], work, q + 2, A, (q + 5 * qs) + 2);
                }
            }
        }

        for (ixstart = q + 1; ixstart + 1 < 21; ixstart++) {
            Vf[ixstart + 20 * q] = e[ixstart];
        }
    }

    m = 4;
    s[4] = A[24];
    s[5] = 0.0;
    e[5] = 0.0;
    for (ixstart = 0; ixstart < 5; ixstart++) {
        score[20 + ixstart] = 0.0;
    }

    score[24] = 1.0;
    for (q = 3; q >= 0; q += -1) {
        iter = q + 5 * q;
        if (s[q] != 0.0) {
            for (qs = q + 1; qs + 1 < 6; qs++) {
                kase = (q + 5 * qs) + 1;
                d_xaxpy(5 - q, -(b_xdotc(5 - q, score, iter + 1, score, kase) /
                                 score[iter]), iter + 1, score, kase);
            }

            for (ixstart = q; ixstart + 1 < 6; ixstart++) {
                score[ixstart + 5 * q] = -score[ixstart + 5 * q];
            }

            score[iter]++;
            for (ixstart = 1; ixstart <= q; ixstart++) {
                score[(ixstart + 5 * q) - 1] = 0.0;
            }
        } else {
            for (ixstart = 0; ixstart < 5; ixstart++) {
                score[ixstart + 5 * q] = 0.0;
            }

            score[iter] = 1.0;
        }
    }

    for (q = 19; q >= 0; q += -1) {
        if ((q + 1 <= 5) && (e[q] != 0.0)) {
            kase = (q + 20 * q) + 2;
            for (qs = q + 1; qs + 1 < 21; qs++) {
                ixstart = (q + 20 * qs) + 2;
                e_xaxpy(19 - q, -(c_xdotc(19 - q, Vf, kase, Vf, ixstart) / Vf[kase - 1]),
                        kase, Vf, ixstart);
            }
        }

        memset(&Vf[q * 20], 0, 20U * sizeof(double));
        Vf[q + 20 * q] = 1.0;
    }

    for (q = 0; q < 6; q++) {
        ztest0 = e[q];
        if (s[q] != 0.0) {
            rt = std::abs(s[q]);
            ztest = s[q] / rt;
            s[q] = rt;
            if (q + 1 < 6) {
                ztest0 = e[q] / ztest;
            }

            if (q + 1 <= 5) {
                xscal(ztest, score, 1 + 5 * q);
            }
        }

        if ((q + 1 < 6) && (ztest0 != 0.0)) {
            rt = std::abs(ztest0);
            ztest = rt / ztest0;
            ztest0 = rt;
            s[q + 1] *= ztest;
            b_xscal(ztest, Vf, 1 + 20 * (q + 1));
        }

        e[q] = ztest0;
    }

    iter = 0;
    snorm = 0.0;
    for (ixstart = 0; ixstart < 6; ixstart++) {
        ztest0 = std::abs(s[ixstart]);
        ztest = std::abs(e[ixstart]);
        if ((ztest0 >= ztest) || rtIsNaN(ztest)) {
        } else {
            ztest0 = ztest;
        }

        if ((snorm >= ztest0) || rtIsNaN(ztest0)) {
        } else {
            snorm = ztest0;
        }
    }

    while ((m + 2 > 0) && (!(iter >= 75))) {
        ixstart = m;
        do {
            exitg3 = 0;
            q = ixstart + 1;
            if (ixstart + 1 == 0) {
                exitg3 = 1;
            } else {
                ztest0 = std::abs(e[ixstart]);
                if ((ztest0 <= 2.2204460492503131E-16 * (std::abs(s[ixstart]) + std::abs
                        (s[ixstart + 1]))) || (ztest0 <= 1.0020841800044864E-292) ||
                    ((iter > 20) && (ztest0 <= 2.2204460492503131E-16 * snorm))) {
                    e[ixstart] = 0.0;
                    exitg3 = 1;
                } else {
                    ixstart--;
                }
            }
        } while (exitg3 == 0);

        if (ixstart + 1 == m + 1) {
            kase = 4;
        } else {
            qs = m + 2;
            kase = m + 2;
            exitg2 = false;
            while ((!exitg2) && (kase >= ixstart + 1)) {
                qs = kase;
                if (kase == ixstart + 1) {
                    exitg2 = true;
                } else {
                    ztest0 = 0.0;
                    if (kase < m + 2) {
                        ztest0 = std::abs(e[kase - 1]);
                    }

                    if (kase > ixstart + 2) {
                        ztest0 += std::abs(e[kase - 2]);
                    }

                    ztest = std::abs(s[kase - 1]);
                    if ((ztest <= 2.2204460492503131E-16 * ztest0) || (ztest <=
                                                                       1.0020841800044864E-292)) {
                        s[kase - 1] = 0.0;
                        exitg2 = true;
                    } else {
                        kase--;
                    }
                }
            }

            if (qs == ixstart + 1) {
                kase = 3;
            } else if (qs == m + 2) {
                kase = 1;
            } else {
                kase = 2;
                q = qs;
            }
        }

        switch (kase) {
            case 1:
                f = e[m];
                e[m] = 0.0;
                for (kase = m; kase + 1 >= q + 1; kase--) {
                    xrotg(&s[kase], &f, &ztest0, &ztest);
                    if (kase + 1 > q + 1) {
                        f = -ztest * e[kase - 1];
                        e[kase - 1] *= ztest0;
                    }

                    xrot(Vf, 1 + 20 * kase, 1 + 20 * (m + 1), ztest0, ztest);
                }
                break;

            case 2:
                f = e[q - 1];
                e[q - 1] = 0.0;
                for (kase = q; kase + 1 <= m + 2; kase++) {
                    xrotg(&s[kase], &f, &ztest0, &ztest);
                    f = -ztest * e[kase];
                    e[kase] *= ztest0;
                    b_xrot(score, 1 + 5 * kase, 1 + 5 * (q - 1), ztest0, ztest);
                }
                break;

            case 3:
                varargin_1[0] = std::abs(s[m + 1]);
                varargin_1[1] = std::abs(s[m]);
                varargin_1[2] = std::abs(e[m]);
                varargin_1[3] = std::abs(s[q]);
                varargin_1[4] = std::abs(e[q]);
                ixstart = 1;
                mtmp = varargin_1[0];
                if (rtIsNaN(varargin_1[0])) {
                    kase = 2;
                    exitg1 = false;
                    while ((!exitg1) && (kase < 6)) {
                        ixstart = kase;
                        if (!rtIsNaN(varargin_1[kase - 1])) {
                            mtmp = varargin_1[kase - 1];
                            exitg1 = true;
                        } else {
                            kase++;
                        }
                    }
                }

                if (ixstart < 5) {
                    while (ixstart + 1 < 6) {
                        if (varargin_1[ixstart] > mtmp) {
                            mtmp = varargin_1[ixstart];
                        }

                        ixstart++;
                    }
                }

                f = s[m + 1] / mtmp;
                ztest0 = s[m] / mtmp;
                ztest = e[m] / mtmp;
                sqds = s[q] / mtmp;
                rt = ((ztest0 + f) * (ztest0 - f) + ztest * ztest) / 2.0;
                ztest0 = f * ztest;
                ztest0 *= ztest0;
                if ((rt != 0.0) || (ztest0 != 0.0)) {
                    ztest = std::sqrt(rt * rt + ztest0);
                    if (rt < 0.0) {
                        ztest = -ztest;
                    }

                    ztest = ztest0 / (rt + ztest);
                } else {
                    ztest = 0.0;
                }

                f = (sqds + f) * (sqds - f) + ztest;
                rt = sqds * (e[q] / mtmp);
                for (kase = q + 1; kase <= m + 1; kase++) {
                    xrotg(&f, &rt, &ztest0, &ztest);
                    if (kase > q + 1) {
                        e[kase - 2] = f;
                    }

                    f = ztest0 * s[kase - 1] + ztest * e[kase - 1];
                    e[kase - 1] = ztest0 * e[kase - 1] - ztest * s[kase - 1];
                    rt = ztest * s[kase];
                    s[kase] *= ztest0;
                    xrot(Vf, 1 + 20 * (kase - 1), 1 + 20 * kase, ztest0, ztest);
                    s[kase - 1] = f;
                    xrotg(&s[kase - 1], &rt, &ztest0, &ztest);
                    f = ztest0 * e[kase - 1] + ztest * s[kase];
                    s[kase] = -ztest * e[kase - 1] + ztest0 * s[kase];
                    rt = ztest * e[kase];
                    e[kase] *= ztest0;
                    if (kase < 5) {
                        b_xrot(score, 1 + 5 * (kase - 1), 1 + 5 * kase, ztest0, ztest);
                    }
                }

                e[m] = f;
                iter++;
                break;

            default:
                if (s[q] < 0.0) {
                    s[q] = -s[q];
                    b_xscal(-1.0, Vf, 1 + 20 * q);
                }

                kase = q + 1;
                while ((q + 1 < 6) && (s[q] < s[kase])) {
                    rt = s[q];
                    s[q] = s[kase];
                    s[kase] = rt;
                    xswap(Vf, 1 + 20 * q, 1 + 20 * (q + 1));
                    if (q + 1 < 5) {
                        b_xswap(score, 1 + 5 * q, 1 + 5 * (q + 1));
                    }

                    q = kase;
                    kase++;
                }

                iter = 0;
                m--;
                break;
        }
    }

    for (kase = 0; kase < 5; kase++) {
        work[kase] = s[kase];
        memcpy(&coeff[kase * 20], &Vf[kase * 20], 20U * sizeof(double));
        for (ixstart = 0; ixstart < 5; ixstart++) {
            score[ixstart + 5 * kase] *= work[kase];
        }

        work[kase] = work[kase] * work[kase] / (double)DOF;
    }

    if (DOF > 1) {
        if (DOF >= 20) {
            kase = DOF;
        } else {
            kase = 20;
        }

        ztest0 = (double)kase * eps(work[0]);
        q = 0;
        for (kase = 0; kase < 5; kase++) {
            if (work[kase] > ztest0) {
                q++;
            }
        }
    } else {
        q = 0;
    }

    emxInit_real_T(&standScores, 2);
    ixstart = standScores->size[0] * standScores->size[1];
    standScores->size[0] = 5;
    standScores->size[1] = q;
    emxEnsureCapacity((emxArray__common *)standScores, ixstart, (int)sizeof(double));
    for (ixstart = 0; ixstart < 5; ixstart++) {
        tsquared[ixstart] = 0.0;
    }

    for (kase = 0; kase + 1 <= q; kase++) {
        ztest0 = std::sqrt(work[kase]);
        for (ixstart = 0; ixstart < 5; ixstart++) {
            standScores->data[ixstart + standScores->size[0] * kase] = score[ixstart +
                                                                             5 * kase] / ztest0;
            tsquared[ixstart] += standScores->data[ixstart + standScores->size[0] *
                                                             kase] * standScores->data[ixstart + standScores->size[0] * kase];
        }
    }

    emxFree_real_T(&standScores);
    if (DOF < 20) {
        if (DOF <= 5) {
            qs = DOF;
        } else {
            qs = 5;
        }

        ixstart = scoreOut->size[0] * scoreOut->size[1];
        scoreOut->size[0] = 5;
        scoreOut->size[1] = qs;
        emxEnsureCapacity((emxArray__common *)scoreOut, ixstart, (int)sizeof(double));
        kase = 5 * qs;
        for (ixstart = 0; ixstart < kase; ixstart++) {
            scoreOut->data[ixstart] = 0.0;
        }

        for (kase = 0; kase + 1 <= qs; kase++) {
            for (ixstart = 0; ixstart < 5; ixstart++) {
                scoreOut->data[ixstart + scoreOut->size[0] * kase] = score[ixstart + 5 *
                                                                                     kase];
            }
        }

        ixstart = latentOut->size[0];
        latentOut->size[0] = qs;
        emxEnsureCapacity((emxArray__common *)latentOut, ixstart, (int)sizeof(double));
        for (ixstart = 0; ixstart < qs; ixstart++) {
            latentOut->data[ixstart] = 0.0;
        }

        for (kase = 0; kase + 1 <= qs; kase++) {
            latentOut->data[kase] = work[kase];
        }

        ixstart = coeffOut->size[0] * coeffOut->size[1];
        coeffOut->size[0] = 20;
        coeffOut->size[1] = qs;
        emxEnsureCapacity((emxArray__common *)coeffOut, ixstart, (int)sizeof(double));
        kase = 20 * qs;
        for (ixstart = 0; ixstart < kase; ixstart++) {
            coeffOut->data[ixstart] = 0.0;
        }

        for (kase = 0; kase + 1 <= qs; kase++) {
            for (ixstart = 0; ixstart < 20; ixstart++) {
                coeffOut->data[ixstart + coeffOut->size[0] * kase] = coeff[ixstart + 20 *
                                                                                     kase];
            }
        }
    } else {
        ixstart = scoreOut->size[0] * scoreOut->size[1];
        scoreOut->size[0] = 5;
        scoreOut->size[1] = 5;
        emxEnsureCapacity((emxArray__common *)scoreOut, ixstart, (int)sizeof(double));
        for (ixstart = 0; ixstart < 25; ixstart++) {
            scoreOut->data[ixstart] = score[ixstart];
        }

        ixstart = latentOut->size[0];
        latentOut->size[0] = 5;
        emxEnsureCapacity((emxArray__common *)latentOut, ixstart, (int)sizeof(double));
        for (ixstart = 0; ixstart < 5; ixstart++) {
            latentOut->data[ixstart] = work[ixstart];
        }

        ixstart = coeffOut->size[0] * coeffOut->size[1];
        coeffOut->size[0] = 20;
        coeffOut->size[1] = 5;
        emxEnsureCapacity((emxArray__common *)coeffOut, ixstart, (int)sizeof(double));
        for (ixstart = 0; ixstart < 100; ixstart++) {
            coeffOut->data[ixstart] = coeff[ixstart];
        }
    }

    ztest0 = sum(latentOut);
    ixstart = explained->size[0];
    explained->size[0] = latentOut->size[0];
    emxEnsureCapacity((emxArray__common *)explained, ixstart, (int)sizeof(double));
    kase = latentOut->size[0];
    for (ixstart = 0; ixstart < kase; ixstart++) {
        explained->data[ixstart] = 100.0 * latentOut->data[ixstart] / ztest0;
    }
}

//
// Arguments    : int idx[500]
//                double x[500]
//                int offset
//                int np
//                int nq
//                int iwork[500]
//                double xwork[500]
// Return Type  : void
//
static void merge(int idx[500], double x[500], int offset, int np, int nq, int
iwork[500], double xwork[500])
{
    int n;
    int qend;
    int p;
    int iout;
    int exitg1;
    if ((np == 0) || (nq == 0)) {
    } else {
        n = np + nq;
        for (qend = 0; qend + 1 <= n; qend++) {
            iwork[qend] = idx[offset + qend];
            xwork[qend] = x[offset + qend];
        }

        p = 0;
        n = np;
        qend = np + nq;
        iout = offset - 1;
        do {
            exitg1 = 0;
            iout++;
            if (xwork[p] <= xwork[n]) {
                idx[iout] = iwork[p];
                x[iout] = xwork[p];
                if (p + 1 < np) {
                    p++;
                } else {
                    exitg1 = 1;
                }
            } else {
                idx[iout] = iwork[n];
                x[iout] = xwork[n];
                if (n + 1 < qend) {
                    n++;
                } else {
                    n = iout - p;
                    while (p + 1 <= np) {
                        idx[(n + p) + 1] = iwork[p];
                        x[(n + p) + 1] = xwork[p];
                        p++;
                    }

                    exitg1 = 1;
                }
            }
        } while (exitg1 == 0);
    }
}

//
// Arguments    : int idx[500]
//                double x[500]
//                int offset
//                int n
//                int preSortLevel
//                int iwork[500]
//                double xwork[500]
// Return Type  : void
//
static void merge_block(int idx[500], double x[500], int offset, int n, int
preSortLevel, int iwork[500], double xwork[500])
{
    int nPairs;
    int bLen;
    int tailOffset;
    int nTail;
    nPairs = n >> preSortLevel;
    bLen = 1 << preSortLevel;
    while (nPairs > 1) {
        if ((nPairs & 1) != 0) {
            nPairs--;
            tailOffset = bLen * nPairs;
            nTail = n - tailOffset;
            if (nTail > bLen) {
                merge(idx, x, offset + tailOffset, bLen, nTail - bLen, iwork, xwork);
            }
        }

        tailOffset = bLen << 1;
        nPairs >>= 1;
        for (nTail = 1; nTail <= nPairs; nTail++) {
            merge(idx, x, offset + (nTail - 1) * tailOffset, bLen, bLen, iwork, xwork);
        }

        bLen = tailOffset;
    }

    if (n > bLen) {
        merge(idx, x, offset, bLen, n - bLen, iwork, xwork);
    }
}

//
// Arguments    : int idx[500]
//                double x[500]
//                int offset
// Return Type  : void
//
static void merge_pow2_block(int idx[500], double x[500], int offset)
{
    int iwork[256];
    double xwork[256];
    int b;
    int bLen;
    int bLen2;
    int nPairs;
    int k;
    int blockOffset;
    int q;
    int p;
    int exitg1;
    for (b = 0; b < 6; b++) {
        bLen = 1 << (b + 2);
        bLen2 = bLen << 1;
        nPairs = 256 >> (b + 3);
        for (k = 1; k <= nPairs; k++) {
            blockOffset = (offset + (k - 1) * bLen2) - 1;
            for (q = 1; q <= bLen2; q++) {
                iwork[q - 1] = idx[blockOffset + q];
                xwork[q - 1] = x[blockOffset + q];
            }

            p = 0;
            q = bLen;
            do {
                exitg1 = 0;
                blockOffset++;
                if (xwork[p] <= xwork[q]) {
                    idx[blockOffset] = iwork[p];
                    x[blockOffset] = xwork[p];
                    if (p + 1 < bLen) {
                        p++;
                    } else {
                        exitg1 = 1;
                    }
                } else {
                    idx[blockOffset] = iwork[q];
                    x[blockOffset] = xwork[q];
                    if (q + 1 < bLen2) {
                        q++;
                    } else {
                        q = blockOffset - p;
                        while (p + 1 <= bLen) {
                            idx[(q + p) + 1] = iwork[p];
                            x[(q + p) + 1] = xwork[p];
                            p++;
                        }

                        exitg1 = 1;
                    }
                }
            } while (exitg1 == 0);
        }
    }
}

//
// Arguments    : double x[500]
//                int idx[500]
// Return Type  : void
//
static void sort(double x[500], int idx[500])
{
    double x4[4];
    short idx4[4];
    int m;
    double xwork[500];
    int nNaNs;
    int ib;
    int k;
    signed char perm[4];
    int iwork[500];
    int i2;
    int i3;
    int i4;
    memset(&idx[0], 0, 500U * sizeof(int));
    for (m = 0; m < 4; m++) {
        x4[m] = 0.0;
        idx4[m] = 0;
    }

    memset(&xwork[0], 0, 500U * sizeof(double));
    nNaNs = -499;
    ib = 0;
    for (k = 0; k < 500; k++) {
        if (rtIsNaN(x[k])) {
            idx[-nNaNs] = k + 1;
            xwork[-nNaNs] = x[k];
            nNaNs++;
        } else {
            ib++;
            idx4[ib - 1] = (short)(k + 1);
            x4[ib - 1] = x[k];
            if (ib == 4) {
                ib = (k - nNaNs) - 502;
                if (x4[0] <= x4[1]) {
                    m = 1;
                    i2 = 2;
                } else {
                    m = 2;
                    i2 = 1;
                }

                if (x4[2] <= x4[3]) {
                    i3 = 3;
                    i4 = 4;
                } else {
                    i3 = 4;
                    i4 = 3;
                }

                if (x4[m - 1] <= x4[i3 - 1]) {
                    if (x4[i2 - 1] <= x4[i3 - 1]) {
                        perm[0] = (signed char)m;
                        perm[1] = (signed char)i2;
                        perm[2] = (signed char)i3;
                        perm[3] = (signed char)i4;
                    } else if (x4[i2 - 1] <= x4[i4 - 1]) {
                        perm[0] = (signed char)m;
                        perm[1] = (signed char)i3;
                        perm[2] = (signed char)i2;
                        perm[3] = (signed char)i4;
                    } else {
                        perm[0] = (signed char)m;
                        perm[1] = (signed char)i3;
                        perm[2] = (signed char)i4;
                        perm[3] = (signed char)i2;
                    }
                } else if (x4[m - 1] <= x4[i4 - 1]) {
                    if (x4[i2 - 1] <= x4[i4 - 1]) {
                        perm[0] = (signed char)i3;
                        perm[1] = (signed char)m;
                        perm[2] = (signed char)i2;
                        perm[3] = (signed char)i4;
                    } else {
                        perm[0] = (signed char)i3;
                        perm[1] = (signed char)m;
                        perm[2] = (signed char)i4;
                        perm[3] = (signed char)i2;
                    }
                } else {
                    perm[0] = (signed char)i3;
                    perm[1] = (signed char)i4;
                    perm[2] = (signed char)m;
                    perm[3] = (signed char)i2;
                }

                idx[ib] = idx4[perm[0] - 1];
                idx[ib + 1] = idx4[perm[1] - 1];
                idx[ib + 2] = idx4[perm[2] - 1];
                idx[ib + 3] = idx4[perm[3] - 1];
                x[ib] = x4[perm[0] - 1];
                x[ib + 1] = x4[perm[1] - 1];
                x[ib + 2] = x4[perm[2] - 1];
                x[ib + 3] = x4[perm[3] - 1];
                ib = 0;
            }
        }
    }

    if (ib > 0) {
        for (m = 0; m < 4; m++) {
            perm[m] = 0;
        }

        if (ib == 1) {
            perm[0] = 1;
        } else if (ib == 2) {
            if (x4[0] <= x4[1]) {
                perm[0] = 1;
                perm[1] = 2;
            } else {
                perm[0] = 2;
                perm[1] = 1;
            }
        } else if (x4[0] <= x4[1]) {
            if (x4[1] <= x4[2]) {
                perm[0] = 1;
                perm[1] = 2;
                perm[2] = 3;
            } else if (x4[0] <= x4[2]) {
                perm[0] = 1;
                perm[1] = 3;
                perm[2] = 2;
            } else {
                perm[0] = 3;
                perm[1] = 1;
                perm[2] = 2;
            }
        } else if (x4[0] <= x4[2]) {
            perm[0] = 2;
            perm[1] = 1;
            perm[2] = 3;
        } else if (x4[1] <= x4[2]) {
            perm[0] = 2;
            perm[1] = 3;
            perm[2] = 1;
        } else {
            perm[0] = 3;
            perm[1] = 2;
            perm[2] = 1;
        }

        for (k = 1; k <= ib; k++) {
            idx[(k - nNaNs) - ib] = idx4[perm[k - 1] - 1];
            x[(k - nNaNs) - ib] = x4[perm[k - 1] - 1];
        }
    }

    m = (nNaNs + 499) >> 1;
    for (k = 1; k <= m; k++) {
        ib = idx[k - nNaNs];
        idx[k - nNaNs] = idx[500 - k];
        idx[500 - k] = ib;
        x[k - nNaNs] = xwork[500 - k];
        x[500 - k] = xwork[k - nNaNs];
    }

    if (((nNaNs + 499) & 1) != 0) {
        x[(m - nNaNs) + 1] = xwork[(m - nNaNs) + 1];
    }

    memset(&iwork[0], 0, 500U * sizeof(int));
    m = 2;
    if (1 - nNaNs > 1) {
        ib = (1 - nNaNs) >> 8;
        if (ib > 0) {
            for (m = 1; m <= ib; m++) {
                merge_pow2_block(idx, x, (m - 1) << 8);
            }

            m = ib << 8;
            ib = 1 - (nNaNs + m);
            if (ib > 0) {
                merge_block(idx, x, m, ib, 2, iwork, xwork);
            }

            m = 8;
        }

        merge_block(idx, x, 0, 1 - nNaNs, m, iwork, xwork);
    }
}

//
// Arguments    : const emxArray_real_T *x
// Return Type  : double
//
static double sum(const emxArray_real_T *x)
{
    double y;
    int k;
    if (x->size[0] == 0) {
        y = 0.0;
    } else {
        y = x->data[0];
        for (k = 2; k <= x->size[0]; k++) {
            y += x->data[k - 1];
        }
    }

    return y;
}

//
// Arguments    : int n
//                double a
//                int ix0
//                double y[100]
//                int iy0
// Return Type  : void
//
static void xaxpy(int n, double a, int ix0, double y[100], int iy0)
{
    int ix;
    int iy;
    int k;
    if ((n < 1) || (a == 0.0)) {
    } else {
        ix = ix0 - 1;
        iy = iy0 - 1;
        for (k = 0; k < n; k++) {
            y[iy] += a * y[ix];
            ix++;
            iy++;
        }
    }
}

//
// Arguments    : int n
//                const double x[100]
//                int ix0
//                const double y[100]
//                int iy0
// Return Type  : double
//
static double xdotc(int n, const double x[100], int ix0, const double y[100],
                    int iy0)
{
    double d;
    int ix;
    int iy;
    int k;
    d = 0.0;
    if (n < 1) {
    } else {
        ix = ix0;
        iy = iy0;
        for (k = 1; k <= n; k++) {
            d += x[ix - 1] * y[iy - 1];
            ix++;
            iy++;
        }
    }

    return d;
}

//
// Arguments    : int n
//                const double x[100]
//                int ix0
// Return Type  : double
//
static double xnrm2(int n, const double x[100], int ix0)
{
    double y;
    double scale;
    int kend;
    int k;
    double absxk;
    double t;
    y = 0.0;
    if (n < 1) {
    } else if (n == 1) {
        y = std::abs(x[ix0 - 1]);
    } else {
        scale = 2.2250738585072014E-308;
        kend = (ix0 + n) - 1;
        for (k = ix0; k <= kend; k++) {
            absxk = std::abs(x[k - 1]);
            if (absxk > scale) {
                t = scale / absxk;
                y = 1.0 + y * t * t;
                scale = absxk;
            } else {
                t = absxk / scale;
                y += t * t;
            }
        }

        y = scale * std::sqrt(y);
    }

    return y;
}

//
// Arguments    : double x[400]
//                int ix0
//                int iy0
//                double c
//                double s
// Return Type  : void
//
static void xrot(double x[400], int ix0, int iy0, double c, double s)
{
    int ix;
    int iy;
    int k;
    double temp;
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < 20; k++) {
        temp = c * x[ix] + s * x[iy];
        x[iy] = c * x[iy] - s * x[ix];
        x[ix] = temp;
        iy++;
        ix++;
    }
}

//
// Arguments    : double *a
//                double *b
//                double *c
//                double *s
// Return Type  : void
//
static void xrotg(double *a, double *b, double *c, double *s)
{
    double roe;
    double absa;
    double absb;
    double scale;
    double ads;
    double bds;
    roe = *b;
    absa = std::abs(*a);
    absb = std::abs(*b);
    if (absa > absb) {
        roe = *a;
    }

    scale = absa + absb;
    if (scale == 0.0) {
        *s = 0.0;
        *c = 1.0;
        scale = 0.0;
        *b = 0.0;
    } else {
        ads = absa / scale;
        bds = absb / scale;
        scale *= std::sqrt(ads * ads + bds * bds);
        if (roe < 0.0) {
            scale = -scale;
        }

        *c = *a / scale;
        *s = *b / scale;
        if (absa > absb) {
            *b = *s;
        } else if (*c != 0.0) {
            *b = 1.0 / *c;
        } else {
            *b = 1.0;
        }
    }

    *a = scale;
}

//
// Arguments    : double a
//                double x[25]
//                int ix0
// Return Type  : void
//
static void xscal(double a, double x[25], int ix0)
{
    int k;
    for (k = ix0; k <= ix0 + 4; k++) {
        x[k - 1] *= a;
    }
}

//
// Arguments    : double x[400]
//                int ix0
//                int iy0
// Return Type  : void
//
static void xswap(double x[400], int ix0, int iy0)
{
    int ix;
    int iy;
    int k;
    double temp;
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < 20; k++) {
        temp = x[ix];
        x[ix] = x[iy];
        x[iy] = temp;
        ix++;
        iy++;
    }
}

//
// alpha=[];
// %%% DOWNSAMPLE AS NEEDED.
// %%%
// Arguments    : double X[600]
//                double Y[5]
// Return Type  : void
//
void activityTracker(double X[600], double Y[5])
{
    int i0;
    int i1;
    double X3[100];
    double X4[100];
    int k;
    emxArray_real_T *b_X;
    double x[3];
    emxArray_real_T *c_X;
    emxArray_real_T *d_X;
    emxArray_real_T *b_X3;
    double y;
    emxArray_real_T *e_X;
    int b_k;
    emxArray_real_T *f_X;
    emxArray_real_T *g_X;
    emxArray_real_T *b_X4;
    double XAccSample[100];
    double YAccSample[100];
    double ZAccSample[100];
    double AccResSample[100];
    double XGyroSample[100];
    double YGyroSample[100];
    double ZGyroSample[100];
    double GyroResSample[100];
    int p;
    int j;
    double dv0[5];
    double dv1[5];
    double dv2[5];
    double dv3[5];
    double dv4[5];
    double dv5[5];
    double dv6[5];
    double dv7[5];
    double meanSampleFeatures[40];
    double stdvSampleFeatures[40];
    emxArray_real_T *pcaSampleFeatures;
    emxArray_real_T *r0;
    emxArray_real_T *r1;
    emxArray_real_T *r2;
    emxArray_real_T *r3;
    emxArray_real_T *r4;
    emxArray_real_T *r5;
    emxArray_real_T *r6;
    emxArray_real_T *r7;
    double maxSampleFeatures[40];
    double minSampleFeatures[40];
    boolean_T b_XAccSample[100];
    double b_Y[5];
    double c_Y[5];
    double d_Y[5];
    double e_Y[5];
    double f_Y[5];
    double g_Y[5];
    double h_Y[5];
    double i_Y[5];
    double j_Y[5];
    double k_Y[5];
    double l_Y[5];
    double m_Y[5];
    double n_Y[5];
    double o_Y[5];
    double p_Y[5];
    double integrationSampleFeatures[40];
    emxArray_real_T *b_meanSampleFeatures;

    //  X=zeros(s,6);
    // %%Divide to make into G instead of m/s^2
    for (i0 = 0; i0 < 3; i0++) {
        for (i1 = 0; i1 < 100; i1++) {
            X[i1 + 100 * i0] /= 9.806;
            X[i1 + 100 * (3 + i0)] *= 57.2958;
        }
    }

    // resultant
    for (k = 0; k < 100; k++) {
        for (i0 = 0; i0 < 3; i0++) {
            x[i0] = X[k + 100 * i0] * X[k + 100 * i0];
        }

        y = x[0];
        for (b_k = 0; b_k < 2; b_k++) {
            y += x[b_k + 1];
        }

        X3[k] = std::sqrt(y);
        for (i0 = 0; i0 < 3; i0++) {
            x[i0] = X[k + 100 * (3 + i0)] * X[k + 100 * (3 + i0)];
        }

        y = x[0];
        for (b_k = 0; b_k < 2; b_k++) {
            y += x[b_k + 1];
        }

        X4[k] = std::sqrt(y);
    }

    emxInit_real_T1(&b_X, 1);
    emxInit_real_T1(&c_X, 1);
    emxInit_real_T1(&d_X, 1);
    emxInit_real_T1(&b_X3, 1);
    emxInit_real_T1(&e_X, 1);
    emxInit_real_T1(&f_X, 1);
    emxInit_real_T1(&g_X, 1);
    emxInit_real_T1(&b_X4, 1);
    for (k = 0; k < 5; k++) {
        p = 1 + 20 * k;
        j = 20 * (1 + k);
        if (p > j) {
            i0 = 0;
            i1 = 0;
        } else {
            i0 = p - 1;
            i1 = j;
        }

        b_k = b_X->size[0];
        b_X->size[0] = i1 - i0;
        emxEnsureCapacity((emxArray__common *)b_X, b_k, (int)sizeof(double));
        b_k = i1 - i0;
        for (i1 = 0; i1 < b_k; i1++) {
            b_X->data[i1] = X[i0 + i1];
        }

        for (i0 = 0; i0 < 20; i0++) {
            XAccSample[k + 5 * i0] = b_X->data[i0];
        }

        if (p > j) {
            i0 = 0;
            i1 = 0;
        } else {
            i0 = p - 1;
            i1 = j;
        }

        b_k = c_X->size[0];
        c_X->size[0] = i1 - i0;
        emxEnsureCapacity((emxArray__common *)c_X, b_k, (int)sizeof(double));
        b_k = i1 - i0;
        for (i1 = 0; i1 < b_k; i1++) {
            c_X->data[i1] = X[100 + (i0 + i1)];
        }

        for (i0 = 0; i0 < 20; i0++) {
            YAccSample[k + 5 * i0] = c_X->data[i0];
        }

        if (p > j) {
            i0 = 0;
            i1 = 0;
        } else {
            i0 = p - 1;
            i1 = j;
        }

        b_k = d_X->size[0];
        d_X->size[0] = i1 - i0;
        emxEnsureCapacity((emxArray__common *)d_X, b_k, (int)sizeof(double));
        b_k = i1 - i0;
        for (i1 = 0; i1 < b_k; i1++) {
            d_X->data[i1] = X[200 + (i0 + i1)];
        }

        for (i0 = 0; i0 < 20; i0++) {
            ZAccSample[k + 5 * i0] = d_X->data[i0];
        }

        if (p > j) {
            i0 = 0;
            i1 = 0;
        } else {
            i0 = p - 1;
            i1 = j;
        }

        b_k = b_X3->size[0];
        b_X3->size[0] = i1 - i0;
        emxEnsureCapacity((emxArray__common *)b_X3, b_k, (int)sizeof(double));
        b_k = i1 - i0;
        for (i1 = 0; i1 < b_k; i1++) {
            b_X3->data[i1] = X3[i0 + i1];
        }

        for (i0 = 0; i0 < 20; i0++) {
            AccResSample[k + 5 * i0] = b_X3->data[i0];
        }

        if (p > j) {
            i0 = 0;
            i1 = 0;
        } else {
            i0 = p - 1;
            i1 = j;
        }

        b_k = e_X->size[0];
        e_X->size[0] = i1 - i0;
        emxEnsureCapacity((emxArray__common *)e_X, b_k, (int)sizeof(double));
        b_k = i1 - i0;
        for (i1 = 0; i1 < b_k; i1++) {
            e_X->data[i1] = X[300 + (i0 + i1)];
        }

        for (i0 = 0; i0 < 20; i0++) {
            XGyroSample[k + 5 * i0] = e_X->data[i0];
        }

        if (p > j) {
            i0 = 0;
            i1 = 0;
        } else {
            i0 = p - 1;
            i1 = j;
        }

        b_k = f_X->size[0];
        f_X->size[0] = i1 - i0;
        emxEnsureCapacity((emxArray__common *)f_X, b_k, (int)sizeof(double));
        b_k = i1 - i0;
        for (i1 = 0; i1 < b_k; i1++) {
            f_X->data[i1] = X[400 + (i0 + i1)];
        }

        for (i0 = 0; i0 < 20; i0++) {
            YGyroSample[k + 5 * i0] = f_X->data[i0];
        }

        if (p > j) {
            i0 = 0;
            i1 = 0;
        } else {
            i0 = p - 1;
            i1 = j;
        }

        b_k = g_X->size[0];
        g_X->size[0] = i1 - i0;
        emxEnsureCapacity((emxArray__common *)g_X, b_k, (int)sizeof(double));
        b_k = i1 - i0;
        for (i1 = 0; i1 < b_k; i1++) {
            g_X->data[i1] = X[500 + (i0 + i1)];
        }

        for (i0 = 0; i0 < 20; i0++) {
            ZGyroSample[k + 5 * i0] = g_X->data[i0];
        }

        if (p > j) {
            i0 = 0;
            i1 = 0;
        } else {
            i0 = p - 1;
            i1 = j;
        }

        b_k = b_X4->size[0];
        b_X4->size[0] = i1 - i0;
        emxEnsureCapacity((emxArray__common *)b_X4, b_k, (int)sizeof(double));
        b_k = i1 - i0;
        for (i1 = 0; i1 < b_k; i1++) {
            b_X4->data[i1] = X4[i0 + i1];
        }

        for (i0 = 0; i0 < 20; i0++) {
            GyroResSample[k + 5 * i0] = b_X4->data[i0];
        }
    }

    emxFree_real_T(&b_X4);
    emxFree_real_T(&g_X);
    emxFree_real_T(&f_X);
    emxFree_real_T(&e_X);
    emxFree_real_T(&b_X3);
    emxFree_real_T(&d_X);
    emxFree_real_T(&c_X);
    emxFree_real_T(&b_X);
    Fmean(XAccSample, dv0);
    Fmean(YAccSample, dv1);
    Fmean(ZAccSample, dv2);
    Fmean(AccResSample, dv3);
    Fmean(XGyroSample, dv4);
    Fmean(YGyroSample, dv5);
    Fmean(ZGyroSample, dv6);
    Fmean(GyroResSample, dv7);
    for (i0 = 0; i0 < 5; i0++) {
        meanSampleFeatures[i0] = dv0[i0];
        meanSampleFeatures[5 + i0] = dv1[i0];
        meanSampleFeatures[10 + i0] = dv2[i0];
        meanSampleFeatures[15 + i0] = dv3[i0];
        meanSampleFeatures[20 + i0] = dv4[i0];
        meanSampleFeatures[25 + i0] = dv5[i0];
        meanSampleFeatures[30 + i0] = dv6[i0];
        meanSampleFeatures[35 + i0] = dv7[i0];
    }

    Fstddev(XAccSample, dv0);
    Fstddev(YAccSample, dv1);
    Fstddev(ZAccSample, dv2);
    Fstddev(AccResSample, dv3);
    Fstddev(XGyroSample, dv4);
    Fstddev(YGyroSample, dv5);
    Fstddev(ZGyroSample, dv6);
    Fstddev(GyroResSample, dv7);
    for (i0 = 0; i0 < 5; i0++) {
        stdvSampleFeatures[i0] = dv0[i0];
        stdvSampleFeatures[5 + i0] = dv1[i0];
        stdvSampleFeatures[10 + i0] = dv2[i0];
        stdvSampleFeatures[15 + i0] = dv3[i0];
        stdvSampleFeatures[20 + i0] = dv4[i0];
        stdvSampleFeatures[25 + i0] = dv5[i0];
        stdvSampleFeatures[30 + i0] = dv6[i0];
        stdvSampleFeatures[35 + i0] = dv7[i0];
    }

    emxInit_real_T(&pcaSampleFeatures, 2);
    emxInit_real_T(&r0, 2);
    emxInit_real_T(&r1, 2);
    emxInit_real_T(&r2, 2);
    emxInit_real_T(&r3, 2);
    emxInit_real_T(&r4, 2);
    emxInit_real_T(&r5, 2);
    emxInit_real_T(&r6, 2);
    emxInit_real_T(&r7, 2);
    Fpca(XAccSample, r0);
    Fpca(YAccSample, r1);
    Fpca(ZAccSample, r2);
    Fpca(AccResSample, r3);
    Fpca(XGyroSample, r4);
    Fpca(YGyroSample, r5);
    Fpca(ZGyroSample, r6);
    Fpca(GyroResSample, r7);
    i0 = pcaSampleFeatures->size[0] * pcaSampleFeatures->size[1];
    pcaSampleFeatures->size[0] = 5;
    pcaSampleFeatures->size[1] = ((((((r0->size[1] + r1->size[1]) + r2->size[1]) +
                                     r3->size[1]) + r4->size[1]) + r5->size[1]) + r6->size[1]) + r7->size[1];
    emxEnsureCapacity((emxArray__common *)pcaSampleFeatures, i0, (int)sizeof
            (double));
    b_k = r0->size[1];
    for (i0 = 0; i0 < b_k; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            pcaSampleFeatures->data[i1 + pcaSampleFeatures->size[0] * i0] = r0->
                    data[i1 + r0->size[0] * i0];
        }
    }

    b_k = r1->size[1];
    for (i0 = 0; i0 < b_k; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            pcaSampleFeatures->data[i1 + pcaSampleFeatures->size[0] * (i0 + r0->size[1])]
                    = r1->data[i1 + r1->size[0] * i0];
        }
    }

    b_k = r2->size[1];
    for (i0 = 0; i0 < b_k; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            pcaSampleFeatures->data[i1 + pcaSampleFeatures->size[0] * ((i0 + r0->size
            [1]) + r1->size[1])] = r2->data[i1 + r2->size[0] * i0];
        }
    }

    b_k = r3->size[1];
    for (i0 = 0; i0 < b_k; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            pcaSampleFeatures->data[i1 + pcaSampleFeatures->size[0] * (((i0 + r0->
                    size[1]) + r1->size[1]) + r2->size[1])] = r3->data[i1 + r3->size[0] * i0];
        }
    }

    b_k = r4->size[1];
    for (i0 = 0; i0 < b_k; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            pcaSampleFeatures->data[i1 + pcaSampleFeatures->size[0] * ((((i0 +
                                                                          r0->size[1]) + r1->size[1]) + r2->size[1]) + r3->size[1])] = r4->data[i1
                                                                                                                                                + r4->size[0] * i0];
        }
    }

    b_k = r5->size[1];
    for (i0 = 0; i0 < b_k; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            pcaSampleFeatures->data[i1 + pcaSampleFeatures->size[0] * (((((i0 +
                                                                           r0->size[1]) + r1->size[1]) + r2->size[1]) + r3->size[1]) + r4->size[1])]
                    = r5->data[i1 + r5->size[0] * i0];
        }
    }

    b_k = r6->size[1];
    for (i0 = 0; i0 < b_k; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            pcaSampleFeatures->data[i1 + pcaSampleFeatures->size[0] * ((((((i0 +
                                                                            r0->size[1]) + r1->size[1]) + r2->size[1]) + r3->size[1]) + r4->size[1])
                                                                       + r5->size[1])] = r6->data[i1 + r6->size[0] * i0];
        }
    }

    b_k = r7->size[1];
    for (i0 = 0; i0 < b_k; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            pcaSampleFeatures->data[i1 + pcaSampleFeatures->size[0] * (((((((i0 +
                                                                             r0->size[1]) + r1->size[1]) + r2->size[1]) + r3->size[1]) + r4->size[1])
                                                                        + r5->size[1]) + r6->size[1])] = r7->data[i1 + r7->size[0] * i0];
        }
    }

    emxFree_real_T(&r7);
    emxFree_real_T(&r6);
    emxFree_real_T(&r5);
    emxFree_real_T(&r4);
    emxFree_real_T(&r3);
    emxFree_real_T(&r2);
    emxFree_real_T(&r1);
    emxFree_real_T(&r0);
    Fmax(XAccSample, dv0);
    Fmax(YAccSample, dv1);
    Fmax(ZAccSample, dv2);
    Fmax(AccResSample, dv3);
    Fmax(XGyroSample, dv4);
    Fmax(YGyroSample, dv5);
    Fmax(ZGyroSample, dv6);
    Fmax(GyroResSample, dv7);
    for (i0 = 0; i0 < 5; i0++) {
        maxSampleFeatures[i0] = dv0[i0];
        maxSampleFeatures[5 + i0] = dv1[i0];
        maxSampleFeatures[10 + i0] = dv2[i0];
        maxSampleFeatures[15 + i0] = dv3[i0];
        maxSampleFeatures[20 + i0] = dv4[i0];
        maxSampleFeatures[25 + i0] = dv5[i0];
        maxSampleFeatures[30 + i0] = dv6[i0];
        maxSampleFeatures[35 + i0] = dv7[i0];
    }

    Fmin(XAccSample, dv0);
    Fmin(YAccSample, dv1);
    Fmin(ZAccSample, dv2);
    Fmin(AccResSample, dv3);
    Fmin(XGyroSample, dv4);
    Fmin(YGyroSample, dv5);
    Fmin(ZGyroSample, dv6);
    Fmin(GyroResSample, dv7);
    for (i0 = 0; i0 < 5; i0++) {
        minSampleFeatures[i0] = dv0[i0];
        minSampleFeatures[5 + i0] = dv1[i0];
        minSampleFeatures[10 + i0] = dv2[i0];
        minSampleFeatures[15 + i0] = dv3[i0];
        minSampleFeatures[20 + i0] = dv4[i0];
        minSampleFeatures[25 + i0] = dv5[i0];
        minSampleFeatures[30 + i0] = dv6[i0];
        minSampleFeatures[35 + i0] = dv7[i0];
    }

    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (XAccSample[i0] < 0.9);
    }

    b_sum(b_XAccSample, Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (YAccSample[i0] < 0.9);
    }

    b_sum(b_XAccSample, b_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (ZAccSample[i0] < 0.9);
    }

    b_sum(b_XAccSample, c_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (AccResSample[i0] < 0.9);
    }

    b_sum(b_XAccSample, d_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (XGyroSample[i0] < 0.9);
    }

    b_sum(b_XAccSample, e_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (YGyroSample[i0] < 0.9);
    }

    b_sum(b_XAccSample, f_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (ZGyroSample[i0] < 0.9);
    }

    b_sum(b_XAccSample, g_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (GyroResSample[i0] < 0.9);
    }

    b_sum(b_XAccSample, h_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (XAccSample[i0] > 1.5);
    }

    b_sum(b_XAccSample, i_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (YAccSample[i0] > 1.5);
    }

    b_sum(b_XAccSample, j_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (ZAccSample[i0] > 1.5);
    }

    b_sum(b_XAccSample, k_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (AccResSample[i0] > 1.5);
    }

    b_sum(b_XAccSample, l_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (XGyroSample[i0] > 1.5);
    }

    b_sum(b_XAccSample, m_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (YGyroSample[i0] > 1.5);
    }

    b_sum(b_XAccSample, n_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (ZGyroSample[i0] > 1.5);
    }

    b_sum(b_XAccSample, o_Y);
    for (i0 = 0; i0 < 100; i0++) {
        b_XAccSample[i0] = (GyroResSample[i0] > 1.5);
    }

    b_sum(b_XAccSample, p_Y);
    Fintegrate(XAccSample, dv0);
    Fintegrate(YAccSample, dv1);
    Fintegrate(ZAccSample, dv2);
    Fintegrate(AccResSample, dv3);
    Fintegrate(XGyroSample, dv4);
    Fintegrate(YGyroSample, dv5);
    Fintegrate(ZGyroSample, dv6);
    Fintegrate(GyroResSample, dv7);
    for (i0 = 0; i0 < 5; i0++) {
        integrationSampleFeatures[i0] = dv0[i0];
        integrationSampleFeatures[5 + i0] = dv1[i0];
        integrationSampleFeatures[10 + i0] = dv2[i0];
        integrationSampleFeatures[15 + i0] = dv3[i0];
        integrationSampleFeatures[20 + i0] = dv4[i0];
        integrationSampleFeatures[25 + i0] = dv5[i0];
        integrationSampleFeatures[30 + i0] = dv6[i0];
        integrationSampleFeatures[35 + i0] = dv7[i0];
    }

    emxInit_real_T(&b_meanSampleFeatures, 2);

    // Apply some threshold? else ? 0
    i0 = b_meanSampleFeatures->size[0] * b_meanSampleFeatures->size[1];
    b_meanSampleFeatures->size[0] = 5;
    b_meanSampleFeatures->size[1] = 56 + pcaSampleFeatures->size[1];
    emxEnsureCapacity((emxArray__common *)b_meanSampleFeatures, i0, (int)sizeof
            (double));
    for (i0 = 0; i0 < 8; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            b_meanSampleFeatures->data[i1 + b_meanSampleFeatures->size[0] * i0] =
                    meanSampleFeatures[i1 + 5 * i0];
        }
    }

    for (i0 = 0; i0 < 8; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            b_meanSampleFeatures->data[i1 + b_meanSampleFeatures->size[0] * (i0 + 8)] =
                    stdvSampleFeatures[i1 + 5 * i0];
        }
    }

    b_k = pcaSampleFeatures->size[1];
    for (i0 = 0; i0 < b_k; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            b_meanSampleFeatures->data[i1 + b_meanSampleFeatures->size[0] * (i0 + 16)]
                    = pcaSampleFeatures->data[i1 + pcaSampleFeatures->size[0] * i0];
        }
    }

    for (i0 = 0; i0 < 8; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            b_meanSampleFeatures->data[i1 + b_meanSampleFeatures->size[0] * ((i0 +
                                                                              pcaSampleFeatures->size[1]) + 16)] = maxSampleFeatures[i1 + 5 * i0];
        }
    }

    for (i0 = 0; i0 < 8; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            b_meanSampleFeatures->data[i1 + b_meanSampleFeatures->size[0] * ((i0 +
                                                                              pcaSampleFeatures->size[1]) + 24)] = minSampleFeatures[i1 + 5 * i0];
        }
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (32 +
                                                                         pcaSampleFeatures->size[1])] = Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (33 +
                                                                         pcaSampleFeatures->size[1])] = b_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (34 +
                                                                         pcaSampleFeatures->size[1])] = c_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (35 +
                                                                         pcaSampleFeatures->size[1])] = d_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (36 +
                                                                         pcaSampleFeatures->size[1])] = e_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (37 +
                                                                         pcaSampleFeatures->size[1])] = f_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (38 +
                                                                         pcaSampleFeatures->size[1])] = g_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (39 +
                                                                         pcaSampleFeatures->size[1])] = h_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (40 +
                                                                         pcaSampleFeatures->size[1])] = i_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (41 +
                                                                         pcaSampleFeatures->size[1])] = j_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (42 +
                                                                         pcaSampleFeatures->size[1])] = k_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (43 +
                                                                         pcaSampleFeatures->size[1])] = l_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (44 +
                                                                         pcaSampleFeatures->size[1])] = m_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (45 +
                                                                         pcaSampleFeatures->size[1])] = n_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (46 +
                                                                         pcaSampleFeatures->size[1])] = o_Y[i0];
    }

    for (i0 = 0; i0 < 5; i0++) {
        b_meanSampleFeatures->data[i0 + b_meanSampleFeatures->size[0] * (47 +
                                                                         pcaSampleFeatures->size[1])] = p_Y[i0];
    }

    for (i0 = 0; i0 < 8; i0++) {
        for (i1 = 0; i1 < 5; i1++) {
            b_meanSampleFeatures->data[i1 + b_meanSampleFeatures->size[0] * ((i0 +
                                                                              pcaSampleFeatures->size[1]) + 48)] = integrationSampleFeatures[i1 + 5 *
                                                                                                                                                  i0];
        }
    }

    emxFree_real_T(&pcaSampleFeatures);
    knnclassify(b_meanSampleFeatures, Y);

    // - function activityTracker -%
    emxFree_real_T(&b_meanSampleFeatures);
}

//
// Arguments    : void
// Return Type  : void
//
void activityTracker_initialize()
{
    rt_InitInfAndNaN(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void activityTracker_terminate()
{
    //No code.
}

//
// File trailer for activityTracker.cpp
//
// [EOF]
//


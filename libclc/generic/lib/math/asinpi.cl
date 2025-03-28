//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <clc/clc.h>
#include <clc/clcmacro.h>
#include <clc/math/math.h>

_CLC_OVERLOAD _CLC_DEF float asinpi(float x) {
    // Computes arcsin(x).
    // The argument is first reduced by noting that arcsin(x)
    // is invalid for abs(x) > 1 and arcsin(-x) = -arcsin(x).
    // For denormal and small arguments arcsin(x) = x to machine
    // accuracy. Remaining argument ranges are handled as follows.
    // For abs(x) <= 0.5 use
    // arcsin(x) = x + x^3*R(x^2)
    // where R(x^2) is a rational minimax approximation to
    // (arcsin(x) - x)/x^3.
    // For abs(x) > 0.5 exploit the identity:
    // arcsin(x) = pi/2 - 2*arcsin(sqrt(1-x)/2)
    // together with the above rational approximation, and
    // reconstruct the terms carefully.


    const float pi = 3.1415926535897933e+00f;
    const float piby2_tail = 7.5497894159e-08F;   /* 0x33a22168 */
    const float hpiby2_head = 7.8539812565e-01F;  /* 0x3f490fda */

    uint ux = as_uint(x);
    uint aux = ux & EXSIGNBIT_SP32;
    uint xs = ux ^ aux;
    float shalf = as_float(xs | as_uint(0.5f));

    int xexp = (int)(aux >> EXPSHIFTBITS_SP32) - EXPBIAS_SP32;

    float y = as_float(aux);

    // abs(x) >= 0.5
    int transform = xexp >= -1;

    float y2 = y * y;
    float rt = 0.5f * (1.0f - y);
    float r = transform ? rt : y2;

    // Use a rational approximation for [0.0, 0.5]
    float a = mad(r,
                  mad(r,
                      mad(r, -0.00396137437848476485201154797087F, -0.0133819288943925804214011424456F),
                      -0.0565298683201845211985026327361F),
                  0.184161606965100694821398249421F);
    float b = mad(r, -0.836411276854206731913362287293F, 1.10496961524520294485512696706F);
    float u = r * MATH_DIVIDE(a, b);

    float s = MATH_SQRT(r);
    float s1 = as_float(as_uint(s) & 0xffff0000);
    float c = MATH_DIVIDE(mad(-s1, s1, r), s + s1);
    float p = mad(2.0f*s, u, -mad(c, -2.0f, piby2_tail));
    float q = mad(s1, -2.0f, hpiby2_head);
    float vt = hpiby2_head - (p - q);
    float v = mad(y, u, y);
    v = transform ? vt : v;
    v = MATH_DIVIDE(v, pi);
    float xbypi = MATH_DIVIDE(x, pi);

    float ret = as_float(xs | as_uint(v));
    ret = aux > 0x3f800000U ? as_float(QNANBITPATT_SP32) : ret;
    ret = aux == 0x3f800000U ? shalf : ret;
    ret = xexp < -14 ? xbypi : ret;

    return ret;
}

_CLC_UNARY_VECTORIZE(_CLC_OVERLOAD _CLC_DEF, float, asinpi, float)

#ifdef cl_khr_fp64
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

_CLC_OVERLOAD _CLC_DEF double asinpi(double x) {
    // Computes arcsin(x).
    // The argument is first reduced by noting that arcsin(x)
    // is invalid for abs(x) > 1 and arcsin(-x) = -arcsin(x).
    // For denormal and small arguments arcsin(x) = x to machine
    // accuracy. Remaining argument ranges are handled as follows.
    // For abs(x) <= 0.5 use
    // arcsin(x) = x + x^3*R(x^2)
    // where R(x^2) is a rational minimax approximation to
    // (arcsin(x) - x)/x^3.
    // For abs(x) > 0.5 exploit the identity:
    // arcsin(x) = pi/2 - 2*arcsin(sqrt(1-x)/2)
    // together with the above rational approximation, and
    // reconstruct the terms carefully.

    const double pi = 0x1.921fb54442d18p+1;
    const double piby2_tail = 6.1232339957367660e-17;	/* 0x3c91a62633145c07 */
    const double hpiby2_head = 7.8539816339744831e-01;	/* 0x3fe921fb54442d18 */

    double y = fabs(x);
    int xneg = as_int2(x).hi < 0;
    int xexp = (as_int2(y).hi >> 20) - EXPBIAS_DP64;

    // abs(x) >= 0.5
    int transform = xexp >= -1;

    double rt = 0.5 * (1.0 - y);
    double y2 = y * y;
    double r = transform ? rt : y2;

    // Use a rational approximation for [0.0, 0.5]
    double un = fma(r,
                    fma(r,
                        fma(r,
                            fma(r,
                                fma(r, 0.0000482901920344786991880522822991,
                                       0.00109242697235074662306043804220),
                                -0.0549989809235685841612020091328),
                            0.275558175256937652532686256258),
                        -0.445017216867635649900123110649),
                    0.227485835556935010735943483075);

    double ud = fma(r,
                    fma(r,
                        fma(r,
                            fma(r, 0.105869422087204370341222318533,
                                   -0.943639137032492685763471240072),
                            2.76568859157270989520376345954),
                        -3.28431505720958658909889444194),
                    1.36491501334161032038194214209);

    double u = r * MATH_DIVIDE(un, ud);


    // Reconstruct asin carefully in transformed region
    double s = sqrt(r);
    double sh = as_double(as_ulong(s) & 0xffffffff00000000UL);
    double c = MATH_DIVIDE(fma(-sh, sh, r), s + sh);
    double p = fma(2.0*s, u, -fma(-2.0, c, piby2_tail));
    double q = fma(-2.0, sh, hpiby2_head);
    double vt = hpiby2_head - (p - q);
    double v = fma(y, u, y);
    v = transform ? vt : v;

    v = xexp < -28 ? y : v;
    v = MATH_DIVIDE(v, pi);
    v = xexp >= 0 ? as_double(QNANBITPATT_DP64) : v;
    v = y == 1.0 ? 0.5 : v;
    return xneg ? -v : v;
}

_CLC_UNARY_VECTORIZE(_CLC_OVERLOAD _CLC_DEF, double, asinpi, double)

#endif

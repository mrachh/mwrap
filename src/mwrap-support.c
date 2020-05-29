/* Code generated by mwrap 0.33.5 */
/*
  Copyright statement for mwrap:

  mwrap -- MEX file generation for MATLAB and Octave
  Copyright (c) 2007-2008 David Bindel

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  You may distribute a work that contains part or all of the source code
  generated by mwrap under the terms of your choice.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <mex.h>

#if MX_HAS_INTERLEAVED_COMPLEX
#include <matrix.h>
#endif

#ifndef ulong
#  define ulong unsigned long
#endif
#ifndef uint
#  define uint  unsigned int
#endif
#ifndef ushort
#  define ushort unsigned short
#endif
#ifndef uchar
#  define uchar unsigned char
#endif

#ifndef longlong
#  define longlong long long
#endif
#ifndef ulonglong
#  define ulonglong unsigned long long
#endif

#ifndef float32_t
#  define float32_t float
#endif
#ifndef float64_t
#  define float64_t double
#endif

/*
 * Support for 32-bit and 64-bit MEX files
 */
#ifndef mwSize
#  define mwSize size_t
#endif
#ifndef mwIndex
#  define mwIndex size_t
#endif
#ifndef mwSignedIndex
#  define mwSignedIndex ptrdiff_t
#endif


/*
 * Records for call profile.
 */
int* mexprofrecord_= NULL;


/*
 * Support routines for copying data into and out of the MEX stubs, R2018a
 */

#if MX_HAS_INTERLEAVED_COMPLEX

void* mxWrapGetP(const mxArray* a, const char* fmt, const char** e)
{
    void* p = NULL;
#ifdef R2008OO
    mxArray* ap;
#endif
    if (mxGetClassID(a) == mxDOUBLE_CLASS && mxIsComplex(a) )
    {
        if( mxGetM(a)*mxGetN(a) == 1 && (*mxGetComplexDoubles(a)).real == 0 )
        return NULL;
    }
    if (mxGetClassID(a) == mxDOUBLE_CLASS && !mxIsComplex(a) )
    {
        if( mxGetM(a)*mxGetN(a) == 1 && *mxGetDoubles(a) == 0)
        return NULL;
    }
    if (mxIsChar(a)) {
        char pbuf[128];
        mxGetString(a, pbuf, sizeof(pbuf));
        sscanf(pbuf, fmt, &p);
    } 
#ifdef R2008OO
    else if (ap = mxGetProperty(a, 0, "mwptr")) {
        return mxWrapGetP(ap, fmt, e);
    }
#endif
    if (p == 0)
        *e = "Invalid pointer";
    return p;
}

mxArray* mxWrapCreateP(void* p, const char* fmt)
{
    if (p == 0) {
        mxArray* z = mxCreateDoubleMatrix(1,1, mxREAL);
        *mxGetDoubles(z) = 0;
        return z;
    } else {
        char pbuf[128];
        sprintf(pbuf, fmt, p);
        return mxCreateString(pbuf);
    }
}

mxArray* mxWrapStrncpy(const char* s)
{
    if (s) {
        return mxCreateString(s);
    } else {
        mxArray* z = mxCreateDoubleMatrix(1,1, mxREAL);
        *mxGetDoubles(z) = 0;
        return z;
    }
}

char* mxWrapGetString(const mxArray* a, const char** e)
{
    char* s;
    mwSize slen;
    if (!a || (!mxIsChar(a) && mxGetM(a)*mxGetN(a) > 0)) {
        *e = "Invalid string argument";
        return NULL;
    }
    slen = mxGetM(a)*mxGetN(a) + 1;
    s = (char*) mxMalloc(slen);
    if (mxGetM(a)*mxGetN(a) == 0)
        *s = 0;
    else
        mxGetString(a, s, slen);
    return s;
}


double mxWrapGetScalar(const mxArray* a, const char** e)
{
    if (!a || mxGetClassID(a) != mxDOUBLE_CLASS || mxGetM(a)*mxGetN(a) != 1) {
        *e = "Invalid scalar argument";
        return 0;
    }
    if( mxIsComplex(a) )
      return (double) (*mxGetComplexDoubles(a)).real;
    else
      return (double) (*mxGetDoubles(a));
}

#define mxWrapGetArrayDef(func, T) \
T* func(const mxArray* a, const char** e)     \
{ \
    T* array; \
    mwSize arraylen; \
    mwIndex i; \
    T* p; \
    double* q; \
    mxComplexDouble* z; \
    if (!a || mxGetClassID(a) != mxDOUBLE_CLASS) { \
        *e = "Invalid array argument"; \
        return 0; \
    } \
    arraylen = mxGetM(a)*mxGetN(a); \
    array = (T*) mxMalloc(mxGetM(a)*mxGetN(a) * sizeof(T)); \
    p = array; \
    if( mxIsComplex(a) ) \
      { \
	z = mxGetComplexDoubles(a);	   \
	for (i = 0; i < arraylen; ++i)		\
	  *p++ = (T) (*z++).real;			\
      } \
    else \
      {				   \
	q = mxGetDoubles(a);	   \
	for (i = 0; i < arraylen; ++i)		\
	  *p++ = (T) (*q++);			\
      } \
    return array; \
}


#define mxWrapCopyDef(func, T) \
void func(mxArray* a, const T* q, mwSize n) \
{ \
    mwIndex i; \
    double* p;	\
    mxComplexDouble* z; \
    if( mxIsComplex(a) ) \
      { \
	z = mxGetComplexDoubles(a);	   \
	for (i = 0; i < n; ++i)		\
	  (*z++).real = (double) *q++;	\
	  (*z++).imag = 0;	\
      } \
    else \
      {				   \
	p = mxGetDoubles(a);	   \
	for (i = 0; i < n; ++i)		\
	  *p++ = (double) *q++;		\
      } \
}


#define mxWrapReturnDef(func, T) \
mxArray* func(const T* q, mwSize m, mwSize n) \
{ \
    mwIndex i; \
    double* p; \
    if (!q) { \
        return mxCreateDoubleMatrix(0,0, mxREAL); \
    } else { \
        mxArray* a = mxCreateDoubleMatrix(m,n, mxREAL); \
        p = mxGetDoubles(a); \
        for (i = 0; i < m*n; ++i) \
	  *p++ = (double) *q++;	  \
        return a; \
    } \
}


#define mxWrapGetScalarZDef(func, T, ZT, setz)	\
void func(T* z, const mxArray* a) \
{ \
    if( mxIsComplex(a) ) \
      { \
  setz(z, (ZT) (*mxGetComplexDoubles(a)).real, (ZT) (*mxGetComplexDoubles(a)).imag); \
      } \
    else \
      {				   \
  setz(z, (ZT) (*mxGetComplexDoubles(a)).real, (ZT) 0);	\
      } \
}


#define mxWrapGetArrayZDef(func, T, ZT, setz)      \
T* func(const mxArray* a, const char** e)     \
{ \
    T* array; \
    mwSize arraylen; \
    mwIndex i; \
    T* p; \
    double* q; \
    mxComplexDouble* z; \
    if (!a || mxGetClassID(a) != mxDOUBLE_CLASS) { \
        *e = "Invalid array argument"; \
        return 0; \
    } \
    arraylen = mxGetM(a)*mxGetN(a); \
    array = (T*) mxMalloc(mxGetM(a)*mxGetN(a) * sizeof(T)); \
    p = array; \
    if( mxIsComplex(a) ) \
      { \
	z = mxGetComplexDoubles(a);	   \
	for (i = 0; i < arraylen; ++i) {	\
	  setz(p, (ZT) (*z).real, (ZT) (*z).imag);	\
  	  ++p; ++z; }					\
      } \
    else \
      {				   \
	q = mxGetDoubles(a);	   \
	for (i = 0; i < arraylen; ++i)	{	\
	  setz(p, (ZT) (*q), (ZT) 0 );		\
          ++p; ++q; }			\
      }						\
    return array; \
}


#define mxWrapCopyZDef(func, T, freal, fimag)	    \
void func(mxArray* a, const T* q, mwSize n) \
{ \
    mwIndex i; \
    double* p;	\
    mxComplexDouble* z; \
    if( mxIsComplex(a) ) \
      { \
	z = mxGetComplexDoubles(a);	   \
	for (i = 0; i < n; ++i)	{		\
          (*z).real = freal(*q);			\
	  (*z).imag = fimag(*q);			\
	  ++z; ++q; 	}			\
      } \
    else \
      {				   \
	p = mxGetDoubles(a);	   \
	for (i = 0; i < n; ++i)		\
	  *p++ = freal(*q++);		\
      } \
}


#define mxWrapReturnZDef(func, T, freal, fimag)	      \
mxArray* func(const T* q, mwSize m, mwSize n) \
{ \
    mwIndex i; \
    mxComplexDouble* p; \
    if (!q) { \
        return mxCreateDoubleMatrix(0,0, mxCOMPLEX); \
    } else { \
        mxArray* a = mxCreateDoubleMatrix(m,n, mxCOMPLEX); \
        p = mxGetComplexDoubles(a); \
        for (i = 0; i < m*n; ++i) {	  \
          (*p).real = freal(*q);			\
	  (*p).imag = fimag(*q);			\
	  ++p; ++q; 	}			\
        return a; \
    } \
}

#else

/*
 * Support routines for copying data into and out of the MEX stubs, -R2017b
 */

void* mxWrapGetP(const mxArray* a, const char* fmt, const char** e)
{
    void* p = 0;
#ifdef R2008OO
    mxArray* ap;
#endif
    if (mxGetClassID(a) == mxDOUBLE_CLASS && 
        mxGetM(a)*mxGetN(a) == 1 && *mxGetPr(a) == 0)
        return p;
    if (mxIsChar(a)) {
        char pbuf[128];
        mxGetString(a, pbuf, sizeof(pbuf));
        sscanf(pbuf, fmt, &p);
    } 
#ifdef R2008OO
    else if (ap = mxGetProperty(a, 0, "mwptr")) {
        return mxWrapGetP(ap, fmt, e);
    }
#endif
    if (p == 0)
        *e = "Invalid pointer";
    return p;
}

mxArray* mxWrapCreateP(void* p, const char* fmt)
{
    if (p == 0) {
        mxArray* z = mxCreateDoubleMatrix(1,1, mxREAL);
        *mxGetPr(z) = 0;
        return z;
    } else {
        char pbuf[128];
        sprintf(pbuf, fmt, p);
        return mxCreateString(pbuf);
    }
}

mxArray* mxWrapStrncpy(const char* s)
{
    if (s) {
        return mxCreateString(s);
    } else {
        mxArray* z = mxCreateDoubleMatrix(1,1, mxREAL);
        *mxGetPr(z) = 0;
        return z;
    }
}

double mxWrapGetScalar(const mxArray* a, const char** e)
{
    if (!a || mxGetClassID(a) != mxDOUBLE_CLASS || mxGetM(a)*mxGetN(a) != 1) {
        *e = "Invalid scalar argument";
        return 0;
    }
    return *mxGetPr(a);
}

char* mxWrapGetString(const mxArray* a, const char** e)
{
    char* s;
    mwSize slen;
    if (!a || (!mxIsChar(a) && mxGetM(a)*mxGetN(a) > 0)) {
        *e = "Invalid string argument";
        return NULL;
    }
    slen = mxGetM(a)*mxGetN(a) + 1;
    s = (char*) mxMalloc(slen);
    if (mxGetM(a)*mxGetN(a) == 0)
        *s = 0;
    else
        mxGetString(a, s, slen);
    return s;
}


#define mxWrapGetArrayDef(func, T) \
T* func(const mxArray* a, const char** e)     \
{ \
    T* array; \
    mwSize arraylen; \
    mwIndex i; \
    T* p; \
    double* q; \
    if (!a || mxGetClassID(a) != mxDOUBLE_CLASS) { \
        *e = "Invalid array argument"; \
        return 0; \
    } \
    arraylen = mxGetM(a)*mxGetN(a); \
    array = (T*) mxMalloc(mxGetM(a)*mxGetN(a) * sizeof(T)); \
    p = array; \
    q = mxGetPr(a); \
    for (i = 0; i < arraylen; ++i) \
        *p++ = (T) (*q++); \
    return array; \
}


#define mxWrapCopyDef(func, T) \
void func(mxArray* a, const T* q, mwSize n) \
{ \
    mwIndex i; \
    double* p = mxGetPr(a); \
    for (i = 0; i < n; ++i) \
        *p++ = *q++; \
}


#define mxWrapReturnDef(func, T) \
mxArray* func(const T* q, mwSize m, mwSize n) \
{ \
    mwIndex i; \
    double* p; \
    if (!q) { \
        return mxCreateDoubleMatrix(0,0, mxREAL); \
    } else { \
        mxArray* a = mxCreateDoubleMatrix(m,n, mxREAL); \
        p = mxGetPr(a); \
        for (i = 0; i < m*n; ++i) \
            *p++ = *q++; \
        return a; \
    } \
}


#define mxWrapGetScalarZDef(func, T, ZT, setz) \
void func(T* z, const mxArray* a) \
{ \
    double* pr = mxGetPr(a); \
    double* pi = mxGetPi(a); \
    setz(z, (ZT) *pr, (pi ? (ZT) *pi : (ZT) 0)); \
}


#define mxWrapGetArrayZDef(func, T, ZT, setz) \
T* func(const mxArray* a, const char** e) \
{ \
    T* array; \
    mwSize arraylen; \
    mwIndex i; \
    T* p; \
    double* qr; \
    double* qi; \
    if (!a || mxGetClassID(a) != mxDOUBLE_CLASS) { \
        *e = "Invalid array argument"; \
        return 0; \
    } \
    arraylen = mxGetM(a)*mxGetN(a); \
    array = (T*) mxMalloc(mxGetM(a)*mxGetN(a) * sizeof(T)); \
    p = array; \
    qr = mxGetPr(a); \
    qi = mxGetPi(a); \
    for (i = 0; i < arraylen; ++i) { \
        ZT val_qr = *qr++; \
        ZT val_qi = (qi ? (ZT) *qi++ : (ZT) 0); \
        setz(p, val_qr, val_qi); \
        ++p; \
    } \
    return array; \
}


#define mxWrapCopyZDef(func, T, real, imag) \
void func(mxArray* a, const T* q, mwSize n) \
{ \
    mwIndex i; \
    double* pr = mxGetPr(a); \
    double* pi = mxGetPi(a); \
    for (i = 0; i < n; ++i) { \
        *pr++ = real(*q); \
        *pi++ = imag(*q); \
        ++q; \
    } \
}


#define mxWrapReturnZDef(func, T, real, imag) \
mxArray* func(const T* q, mwSize m, mwSize n) \
{ \
    mwIndex i; \
    double* pr; \
    double* pi; \
    if (!q) { \
        return mxCreateDoubleMatrix(0,0, mxCOMPLEX); \
    } else { \
        mxArray* a = mxCreateDoubleMatrix(m,n, mxCOMPLEX); \
        pr = mxGetPr(a); \
        pi = mxGetPi(a); \
        for (i = 0; i < m*n; ++i) { \
            *pr++ = real(*q); \
            *pi++ = imag(*q); \
            ++q; \
        } \
        return a; \
    } \
}

#endif

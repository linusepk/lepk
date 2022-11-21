/* Version: 1.0 */

/*
 * MIT License
 * 
 * Copyright (c) 2022 Linus Erik Pontus Kåreblom
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LEPK_TYPE_H
#define LEPK_TYPE_H

/* Signed integers. */
typedef signed char        I8;
typedef signed short       I16;
typedef signed int         I32;
typedef signed long long   I64;

/* Unsigned integers. */
typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;

/* Floating point. */
typedef float  F32;
typedef double F64;

/* Boolean. */
typedef U8  B8;
typedef U32 B32;

#ifndef true
#define true 1
#endif /* true */

#ifndef false
#define false 0
#endif /* false */

/* Set bit at N position to V within B. */
#define BOOL_SET(B, N, V) ((V) ? (B) = (B) | 1 << (N) : ((B) = (B) ^ 1 << (N)))
#define BOOL_GET(B, N) ((B) >> (N) & 1)

#endif // LEPK_TYPE_H

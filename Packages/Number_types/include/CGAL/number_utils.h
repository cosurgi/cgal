// ======================================================================
//
// Copyright (c) 1999 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
// 
// release       : 
// release_date  : 
// 
// file          : number_utils.h
// package       : Number_types
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Stefan Schirra
//
// coordinator   : MPI, Saarbruecken
// ======================================================================

#ifndef CGAL_NUMBER_UTILS_H
#define CGAL_NUMBER_UTILS_H

#include <CGAL/config.h>
#include <CGAL/enum.h>
#include <CGAL/kernel_basic.h>
#include <algorithm>

CGAL_BEGIN_NAMESPACE

template <class NT>
inline
bool
is_zero(const NT& x)
{ return x == 0; }

template <class NT>
inline
bool
is_one(const NT& x)
{ return x == 1; }

template <class NT>
inline
bool
is_negative(const NT& x)
{ return x < 0; }

template <class NT>
inline
bool
is_positive(const NT& x)
{ return 0 < x; }

template <class NT>
CGAL_KERNEL_INLINE
Sign
sign(const NT& x)
{ return (x < 0) ? NEGATIVE : (0 < x) ? POSITIVE : ZERO; }

template <class NT>
CGAL_KERNEL_INLINE
NT
abs(const NT& x)
{
  if (x < 0)
    return -x;
  return x;
}

template <class NT1, class NT2>
CGAL_KERNEL_INLINE
Comparison_result
compare(const NT1& n1, const NT2& n2)
{ return (n1 < n2) ? SMALLER : (n2 < n1) ? LARGER : EQUAL; }

template <class NT>
inline
NT
square( const NT& n)
{ return n*n; }

template <class NT>
inline
double
to_double( const NT& n)
{ return CGAL::to_double(n); }

template <class NT>
inline
NT
div( const NT& n1, const NT& n2)
{ return CGAL::div(n1, n2); }

template <class NT>
inline
NT
sqrt( const NT& n)
{ return CGAL::sqrt(n); }

template <class NT>
inline
bool
is_valid( const NT& n)
{ return CGAL::is_valid(n); }

template <class NT>
inline
bool
is_finite( const NT& n)
{ return CGAL::is_finite(n); }

template <class NT>
inline
NT
gcd( const NT& n1, const NT& n2)
{
  CGAL_precondition(!CGAL_NTS is_zero(n2));
  NT x = CGAL_NTS abs(n1);
  NT y = CGAL_NTS abs(n2);
  do {
    x %= y;
    if (CGAL_NTS is_zero(x)) return y;
    y %= x;
  } while (CGAL_NTS is_positive(y));
  return x;
}

// for min and max see <CGAL/number_type_basic.h>

CGAL_END_NAMESPACE

#endif // CGAL_NUMBER_UTILS_H

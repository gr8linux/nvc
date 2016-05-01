//
//  Copyright (C) 2012-2013  Nick Gasson
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _ARRAY_H
#define _ARRAY_H

#include <assert.h>

#define ARRAY_BASE_SZ 8

//
// Generic array template used in type and tree code
//

#define DEFINE_ARRAY(what)                                     \
   what##_t *what##_array_alloc(what##_array_t *a)             \
   {                                                           \
      if (unlikely(a->count == 0)) {                           \
         assert(a->items == NULL);                             \
         a->items = xmalloc(sizeof(what##_t) * ARRAY_BASE_SZ); \
      }                                                        \
      else if (((a->count & (a->count - 1)) == 0)              \
               && (a->count >= ARRAY_BASE_SZ)) {               \
         const int sz = next_power_of_2(a->count + 1);         \
         a->items = xrealloc(a->items, sizeof(what##_t) * sz); \
      }                                                        \
                                                               \
      return &(a->items[a->count++]);                          \
   }                                                           \
                                                               \
   void what##_array_add(what##_array_t *a, what##_t t)        \
   {                                                           \
      *what##_array_alloc(a) = t;                              \
   }                                                           \
                                                               \
   void what##_array_resize(what##_array_t *a,                 \
                                   size_t n, what##_t fill)    \
   {                                                           \
      if (n > 0) {                                             \
         const int sz = (n <= ARRAY_BASE_SZ)                   \
            ? ARRAY_BASE_SZ : next_power_of_2(n);              \
         a->items = xrealloc(a->items, sz * sizeof(what##_t)); \
         for (unsigned i = a->count; i < n; i++)               \
            a->items[i] = fill;                                \
      }                                                        \
      a->count = n;                                            \
   }                                                           \

#define DECLARE_ARRAY(what)                                    \
   typedef struct {                                            \
      uint32_t  count;                                         \
      what##_t *items;                                         \
   } what##_array_t;                                           \
                                                               \
   void what##_array_add(what##_array_t *a, what##_t t);       \
   what##_t *what##_array_alloc(what##_array_t *a);            \
                                                               \
   __attribute__ ((unused))                                    \
   static inline what##_t *what##_array_nth_ptr(               \
      what##_array_t *a, unsigned n)                           \
   {                                                           \
      assert(n < a->count);                                    \
      return &(a->items[n]);                                   \
   }                                                           \
                                                               \
   __attribute__ ((unused))                                    \
   static inline what##_t what##_array_nth(what##_array_t *a,  \
                                           unsigned n)         \
   {                                                           \
      assert(n < a->count);                                    \
      return a->items[n];                                      \
   }                                                           \
                                                               \
   void what##_array_resize(what##_array_t *a,                 \
                            size_t n, what##_t fill);

#define DECLARE_AND_DEFINE_ARRAY(what) \
   DECLARE_ARRAY(what)                 \
   DEFINE_ARRAY(what)

#endif  // _ARRAY_H

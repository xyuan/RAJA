/*!
 ******************************************************************************
 *
 * \file
 *
 * \brief   Header file providing RAJA Vtable for workgroup.
 *
 ******************************************************************************
 */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJA_PATTERN_WORKGROUP_Vtable_HPP
#define RAJA_PATTERN_WORKGROUP_Vtable_HPP


#include "RAJA/config.hpp"

#include <utility>


namespace RAJA
{

namespace detail
{

template < typename T, typename ... CallArgs >
void Vtable_move_construct(void* dest, void* src)
{
  T* dest_as_T = static_cast<T*>(dest);
  T* src_as_T = static_cast<T*>(src);
  new(dest_as_T) T(std::move(*src_as_T));
}

template < typename T, typename ... CallArgs >
void Vtable_call(const void* obj, CallArgs... args)
{
  const T* obj_as_T = static_cast<const T*>(obj);
  (*obj_as_T)(std::forward<CallArgs>(args)...);
}

template < typename T, typename ... CallArgs >
void Vtable_destroy(void* obj)
{
  T* obj_as_T = static_cast<T*>(obj);
  (*obj_as_T).~T();
}

/*!
 * A vtable abstraction
 *
 * Provides function pointers for basic functions.
 */
template < typename ... CallArgs >
struct Vtable {
  using move_sig = void(*)(void* /*dest*/, void* /*src*/);
  using call_sig = void(*)(const void* /*obj*/, CallArgs... /*args*/);
  using destroy_sig = void(*)(void* /*obj*/);

  move_sig move_construct;
  call_sig call;
  destroy_sig destroy;
  size_t size;
};

template < typename ... CallArgs >
using Vtable_move_sig = typename Vtable<CallArgs...>::move_sig;
template < typename ... CallArgs >
using Vtable_call_sig = typename Vtable<CallArgs...>::call_sig;
template < typename ... CallArgs >
using Vtable_destroy_sig = typename Vtable<CallArgs...>::destroy_sig;

/*!
 * Populate and return a Vtable object appropriate for the given policy
 */
// template < typename T, typename ... CallArgs >
// inline Vtable<CallArgs...> get_Vtable(work_policy const&);

}  // namespace detail

}  // namespace RAJA

#endif  // closing endif for header file include guard
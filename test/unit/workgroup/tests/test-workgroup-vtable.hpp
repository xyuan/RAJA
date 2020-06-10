//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Header file containing tests for RAJA workgroup constructors.
///

#ifndef __TEST_WORKGROUP_VTABLE__
#define __TEST_WORKGROUP_VTABLE__

#include "gtest/gtest.h"

#include "RAJA/RAJA.hpp"
#include "RAJA_gtest.hpp"
#include "RAJA/internal/MemUtils_CPU.hpp"
#include "camp/resource.hpp"

#include "RAJA_unit_forone.hpp"
#include "../test-workgroup-utils.hpp"

template <typename T>
class WorkGroupBasicVtableUnitTest : public ::testing::Test
{
};

TYPED_TEST_SUITE_P(WorkGroupBasicVtableUnitTest);

template  < typename ForOnePol,
            typename ... CallArgs >
typename  std::enable_if<
            std::is_base_of<RunOnHost, ForOnePol>::value
          >::type
call_dispatcher( void* obj, void(*call_function)(void*, CallArgs...),
                 CallArgs... callArgs )
{
  forone<ForOnePol>( [=] () {
    call_function(obj, callArgs...);
  });
}

#if defined(RAJA_ENABLE_CUDA) || defined(RAJA_ENABLE_HIP)
template  < typename ForOnePol,
            typename ... CallArgs >
typename  std::enable_if<
            std::is_base_of<RunOnDevice, ForOnePol>::value
          >::type
call_dispatcher( void* obj, void(*call_function)(void*, CallArgs...),
                 CallArgs... callArgs )
{
  forone<ForOnePol>( [=] __device__ () {
    call_function(obj, callArgs...);
  });
}
#endif

template < typename IndexType,
           typename ... Args >
struct VtableTestCallable
{
  VtableTestCallable(IndexType* _ptr, IndexType _val)
    : ptr(_ptr)
    , val(_val)
  { }

  RAJA_HOST_DEVICE void operator()(IndexType i, Args... args)
  {
    RAJA_UNUSED_VAR(args...);
    ptr[i] = val;
  }

private:
  IndexType* ptr;
  IndexType  val;
};

template < typename ExecPolicy,
           typename IndexType,
           typename WORKING_RES,
           typename ForOnePol,
           typename ... Args >
void testWorkGroupVtable(RAJA::xargs<Args...>)
{
  using TestCallable = VtableTestCallable<IndexType, Args...>;

  camp::resources::Resource work_res{WORKING_RES()};
  camp::resources::Resource host_res{camp::resources::Host()};

  IndexType* testVal = host_res.allocate<IndexType>(3);
  IndexType* initVal = work_res.allocate<IndexType>(3);
  TestCallable* new_obj = work_res.allocate<TestCallable>(1);

  initVal[0] = (IndexType)5;
  initVal[1] = (IndexType)7;
  initVal[2] = (IndexType)5;

  testVal[0] = (IndexType)5;
  testVal[1] = (IndexType)5;
  testVal[2] = (IndexType)5;


  RAJA::detail::Vtable<IndexType, Args...> vtable =
      RAJA::detail::get_Vtable<TestCallable, IndexType, Args...>(ExecPolicy{});

  TestCallable obj(testVal, initVal[1]);

  vtable.move_construct(new_obj, &obj);

  // move a value onto device and fiddle
  call_dispatcher<ForOnePol, IndexType, Args...>(
      new_obj, vtable.call, (IndexType)1, Args{}...);

  vtable.destroy(new_obj);


  ASSERT_EQ(testVal[0], initVal[0]);
  ASSERT_EQ(testVal[1], initVal[1]);
  ASSERT_EQ(testVal[2], initVal[2]);

  work_res.deallocate( new_obj );
  work_res.deallocate( initVal );
  host_res.deallocate( testVal );
}

TYPED_TEST_P(WorkGroupBasicVtableUnitTest, BasicWorkGroupVtable)
{
  using ExecPolicy = typename camp::at<TypeParam, camp::num<0>>::type;
  using IndexType = typename camp::at<TypeParam, camp::num<1>>::type;
  using Args = typename camp::at<TypeParam, camp::num<2>>::type;
  using ResourceType = typename camp::at<TypeParam, camp::num<3>>::type;
  using ForOneType = typename camp::at<TypeParam, camp::num<4>>::type;

  testWorkGroupVtable< ExecPolicy, IndexType, ResourceType, ForOneType >(
      Args{});
}


REGISTER_TYPED_TEST_SUITE_P(WorkGroupBasicVtableUnitTest,
                            BasicWorkGroupVtable);
#endif  //__TEST_WORKGROUP_VTABLE__

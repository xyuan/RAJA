//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "test-forall-segment.hpp"

#if defined(RAJA_ENABLE_TBB)

// Cartesian product of types for TBB tests
using TBBForallSegmentTypes =
  Test< camp::cartesian_product<StrongIdxTypeList,
                                HostResourceList, 
                                TBBForallExecPols> >::Types;

INSTANTIATE_TYPED_TEST_SUITE_P(TBB,
                               ForallSegmentTest,
                               TBBForallSegmentTypes);
#endif

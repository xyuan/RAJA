//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Source file containing tests for RAJA reducer reset.
///

#include "tests/test-reducer-reset.hpp"

#include "test-reducer-utils.hpp"

using OpenMPTargetReducerResetTypes = Test< camp::cartesian_product<
                                                        OpenMPTargetReducerPolicyList,
                                                        DataTypeList,
                                                        OpenMPTargetResourceList
                                                      >
                             >::Types;


INSTANTIATE_TYPED_TEST_CASE_P(OpenMPTargetResetTest,
                              ReducerResetUnitTest,
                              OpenMPTargetReducerResetTypes);


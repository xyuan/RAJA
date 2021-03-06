//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "test-scan.hpp"

#if defined(RAJA_ENABLE_OPENMP)

using OpenMPInclusiveScanTypes = 
  Test< camp::cartesian_product< OpenMPForallExecPols,
                                 HostResourceList,
                                 ScanOpTypes >>::Types;

INSTANTIATE_TYPED_TEST_SUITE_P(OpenMP, 
                               ScanFunctionalTest, 
                               OpenMPInclusiveScanTypes);

#endif

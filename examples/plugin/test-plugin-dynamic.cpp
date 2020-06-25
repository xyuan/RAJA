//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "RAJA/RAJA.hpp"
#include <cstdlib>

int main(int RAJA_UNUSED_ARG(argc), char **RAJA_UNUSED_ARG(argv[]))
{
  // Loading .so files as plugins in current directory.
  RAJA::util::init_plugins("./");
  
  double *a = new double[10];
  for (int i = 0; i < 4; i++)
  {
    RAJA::forall<RAJA::seq_exec>(RAJA::RangeSegment(0, 10), [=](int i) {
      a[i] = 0;
    });
  }
}
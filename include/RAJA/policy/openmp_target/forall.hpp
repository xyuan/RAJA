//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJA_target_forall_openmp_HPP
#define RAJA_target_forall_openmp_HPP

#include "RAJA/config.hpp"

#if defined(RAJA_ENABLE_TARGET_OPENMP)

#include <omp.h>

#include "RAJA/util/types.hpp"

#include "RAJA/policy/openmp/policy.hpp"

namespace RAJA
{

namespace policy
{

namespace omp
{

///
/// OpenMP target parallel for policy implementation
///

template <size_t ThreadsPerTeam, typename Iterable, typename Func>
RAJA_INLINE void forall_impl(const omp_target_parallel_for_exec<ThreadsPerTeam>& exec,
                             Iterable&& iter,
                             Func&& loop_body)
{
  RAJA::resources::Resource res{RAJA::resources::Omp()};
  forall_impl(res, exec, iter, loop_body);
}
template <size_t ThreadsPerTeam, typename Iterable, typename Func>
RAJA_INLINE RAJA::resources::EventProxy forall_impl(RAJA::resources::Resource &res,
                                                    const omp_target_parallel_for_exec<ThreadsPerTeam>&,
                                                    Iterable&& iter,
                                                    Func&& loop_body)
{
  char *omp_ptr;

  RAJA::resources::Omp omp_res = RAJA::resources::raja_get<RAJA::resources::Omp>(res);
  omp_res.get_ptr_dev(omp_ptr);

  using Body = typename std::remove_reference<decltype(loop_body)>::type;
  Body body = loop_body;

  RAJA_EXTRACT_BED_IT(iter);

  // Reset if exceed CUDA threads per block limit.
  int tperteam = ThreadsPerTeam;
  if ( tperteam > omp::MAXNUMTHREADS )
  {
    tperteam = omp::MAXNUMTHREADS;
  }

  // calculate number of teams based on user defined threads per team
  // datasize is distance between begin() and end() of iterable
  auto numteams = RAJA_DIVIDE_CEILING_INT( distance_it, tperteam );
  if ( numteams > tperteam )
  {
    // Omp target reducers will write team # results, into Threads-sized array.
    // Need to insure NumTeams <= Threads to prevent array out of bounds access.
    numteams = tperteam;
  }

// thread_limit(tperteam) unused due to XL seg fault (when tperteam != distance)
  auto i = distance_it;
#pragma omp target teams distribute parallel for num_teams(numteams) \
    schedule(static, 1) firstprivate(body,begin_it)
  for (i = 0; i < distance_it; ++i) {
    Body ib = body;
    ib(begin_it[i]);
  }

  return RAJA::resources::EventProxy(&res);
}

template <typename Iterable, typename Func>
RAJA_INLINE void forall_impl(const omp_target_parallel_for_exec_nt& exec,
                             Iterable&& iter,
                             Func&& loop_body)
{
  RAJA::resources::Resource res{RAJA::resources::Omp()};
  forall_impl(res, exec, iter, loop_body);
}
template <typename Iterable, typename Func>
RAJA_INLINE RAJA::resources::EventProxy forall_impl(RAJA::resources::Resource &res,
                                                    const omp_target_parallel_for_exec_nt&,
                                                    Iterable&& iter,
                                                    Func&& loop_body)
{
  char *omp_ptr;

  RAJA::resources::Omp omp_res = RAJA::resources::raja_get<RAJA::resources::Omp>(res);
  omp_res.get_ptr_dev(omp_ptr);

  using Body = typename std::remove_reference<decltype(loop_body)>::type;
  Body body = loop_body;

  RAJA_EXTRACT_BED_IT(iter);

#pragma omp target teams distribute parallel for schedule(static, 1) \
    firstprivate(body,begin_it) \
    depend(inout : omp_ptr)
  for (decltype(distance_it) i = 0; i < distance_it; ++i) {
    Body ib = body;
    ib(begin_it[i]);
  }

  return RAJA::resources::EventProxy(&res);
}

}  // namespace omp

}  // namespace policy

}  // namespace RAJA

#endif  // closing endif for if defined(RAJA_TARGET_RAJA_ENABLE_OPENMP)

#endif  // closing endif for header file include guard

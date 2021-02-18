/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2019-     GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

/**
 * @file PoroelasticPhaseFieldSolver.hpp
 *
 */

#ifndef GEOSX_PHYSICSSOLVERS_COUPLEDSOLVERS_PoroelasticPhaseFieldSOLVER_HPP_
#define GEOSX_PHYSICSSOLVERS_COUPLEDSOLVERS_PoroelasticPhaseFieldSOLVER_HPP_

#include "common/EnumStrings.hpp"
#include "physicsSolvers/SolverBase.hpp"

namespace geosx
{

class PoroelasticPhaseFieldSolver : public SolverBase
{
public:
  PoroelasticPhaseFieldSolver( const string & name,
                            Group * const parent );
  ~PoroelasticPhaseFieldSolver() override;

  /**
   * @brief name of the node manager in the object catalog
   * @return string that contains the catalog name to generate a new NodeManager object through the object catalog.
   */
  static string catalogName()
  {
    return "PoroelasticPhaseField";
  }

  virtual void registerDataOnMesh( dataRepository::Group * const MeshBodies ) override final;

  virtual void
  implicitStepSetup( real64 const & time_n,
                     real64 const & dt,
                     DomainPartition & domain ) override final;

  virtual void
  implicitStepComplete( real64 const & time_n,
                        real64 const & dt,
                        DomainPartition & domain ) override final;

  virtual void
  resetStateToBeginningOfStep( DomainPartition & domain ) override;

  virtual real64
  solverStep( real64 const & time_n,
              real64 const & dt,
              int const cycleNumber,
              DomainPartition & domain ) override;

  real64 splitOperatorStep( real64 const & time_n,
                            real64 const & dt,
                            integer const cycleNumber,
                            DomainPartition & domain );

  void mapDamageToQuadrature( DomainPartition & domain );

  enum class CouplingTypeOption : integer
  {
    FixedStress,
    TightlyCoupled
  };

  struct viewKeyStruct : SolverBase::viewKeyStruct
  {
    constexpr static auto couplingTypeOptionString = "couplingTypeOption";

    constexpr static auto totalMeanStressString = "totalMeanStress";
    constexpr static auto oldTotalMeanStressString = "oldTotalMeanStress";

    constexpr static auto poroelasticSolverNameString = "poroelasticSolverName";
    constexpr static auto damageSolverNameString = "damageSolverName";
    constexpr static auto subcyclingOptionString = "subcycling";

  } PoroelasticPhaseFieldSolverViewKeys;

protected:
  virtual void postProcessInput() override final;

  virtual void initializePostInitialConditionsPreSubGroups( dataRepository::Group * const problemManager ) override final;

private:

  string m_poroelasticSolverName;
  string m_damageSolverName;
  CouplingTypeOption m_couplingTypeOption;
  integer m_subcyclingOption;

};

ENUM_STRINGS( PoroelasticPhaseFieldSolver::CouplingTypeOption, "FixedStress", "TightlyCoupled" )

} /* namespace geosx */

#endif /* GEOSX_PHYSICSSOLVERS_COUPLEDSOLVERS_PoroelasticPhaseFieldSOLVER_HPP_ */

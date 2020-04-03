/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2019, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-746361
 *
 * All rights reserved. See COPYRIGHT for details.
 *
 * This file is part of the GEOSX Simulation Framework.
 *
 * GEOSX is a free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (as published by the
 * Free Software Foundation) version 2.1 dated February 1999.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * @file SinglePhaseReservoir.hpp
 *
 */

#ifndef GEOSX_PHYSICSSOLVERS_MULTIPHYSICS_SINGLEPHASERESERVOIR_HPP_
#define GEOSX_PHYSICSSOLVERS_MULTIPHYSICS_SINGLEPHASERESERVOIR_HPP_

#include "physicsSolvers/multiphysics/ReservoirSolverBase.hpp"

namespace geosx
{

class SinglePhaseReservoir : public ReservoirSolverBase
{
public:

  /**
   * @brief main constructor for ManagedGroup Objects
   * @param name the name of this instantiation of ManagedGroup in the repository
   * @param parent the parent group of this instantiation of ManagedGroup
   */
  SinglePhaseReservoir( const std::string & name,
                        Group * const parent );

  /**
   * @brief default destructor
   */
  virtual ~SinglePhaseReservoir() override;

  /// deleted copy constructor
  SinglePhaseReservoir( SinglePhaseReservoir const & ) = delete;

  /// default move constructor
  SinglePhaseReservoir( SinglePhaseReservoir && ) = default;

  /// deleted assignment operator
  SinglePhaseReservoir & operator=( SinglePhaseReservoir const & ) = delete;

  /// deleted move operator
  SinglePhaseReservoir & operator=( SinglePhaseReservoir && ) = delete;

  /**
   * @brief name of the node manager in the object catalog
   * @return string that contains the catalog name to generate a new NodeManager object through the object catalog.
   */
  static string CatalogName() { return "SinglePhaseReservoir"; }

  /**
   * @defgroup Solver Interface Functions
   *
   * These functions provide the primary interface that is required for derived classes
   */
  /**@{*/

  virtual void SetupDofs( DomainPartition const * const domain,
                          DofManager & dofManager ) const override;

  /**@}*/

  virtual void AddCouplingSparsityPattern( DomainPartition * const domain,
                                           DofManager & dofManager,
                                           ParallelMatrix & matrix ) override;

  virtual void AssembleCouplingTerms( real64 const time_n,
                                      real64 const dt,
                                      DomainPartition * const domain,
                                      DofManager const * const dofManager,
                                      ParallelMatrix * const matrix,
                                      ParallelVector * const rhs ) override;

};

} /* namespace geosx */

#endif /* GEOSX_PHYSICSSOLVERS_MULTIPHYSICS_SINGLEPHASERESERVOIR_HPP_ */
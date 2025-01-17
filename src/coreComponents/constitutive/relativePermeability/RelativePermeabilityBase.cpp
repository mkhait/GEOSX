/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 TotalEnergies
 * Copyright (c) 2019-     GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

/**
 * @file RelativePermeabilityBase.cpp
 */

#include "RelativePermeabilityBase.hpp"

namespace geosx
{

using namespace dataRepository;

namespace constitutive
{

namespace
{

integer toPhaseType( string const & lookup, string const & groupName )
{
  static unordered_map< string, integer > const phaseDict =
  {
    { "gas", RelativePermeabilityBase::PhaseType::GAS },
    { "oil", RelativePermeabilityBase::PhaseType::OIL },
    { "water", RelativePermeabilityBase::PhaseType::WATER }
  };
  return findOption( phaseDict, lookup, RelativePermeabilityBase::viewKeyStruct::phaseNamesString(), groupName );
}

} // namespace


RelativePermeabilityBase::RelativePermeabilityBase( string const & name, Group * const parent )
  : ConstitutiveBase( name, parent )
{
  registerWrapper( viewKeyStruct::phaseNamesString(), &m_phaseNames ).
    setInputFlag( InputFlags::REQUIRED ).
    setDescription( "List of fluid phases" );

  registerWrapper( viewKeyStruct::phaseTypesString(), &m_phaseTypes ).
    setSizedFromParent( 0 );

  registerWrapper( viewKeyStruct::phaseOrderString(), &m_phaseOrder ).
    setSizedFromParent( 0 );

  registerWrapper( viewKeyStruct::phaseRelPermString(), &m_phaseRelPerm ).setPlotLevel( PlotLevel::LEVEL_0 );
  registerWrapper( viewKeyStruct::dPhaseRelPerm_dPhaseVolFractionString(), &m_dPhaseRelPerm_dPhaseVolFrac );
}

void RelativePermeabilityBase::postProcessInput()
{
  ConstitutiveBase::postProcessInput();

  integer const numPhases = numFluidPhases();
  GEOSX_THROW_IF( numPhases< 2 || numPhases > MAX_NUM_PHASES,
                  getFullName() << ": number of fluid phases must be between 2 and " << MAX_NUM_PHASES << ", got " << numPhases,
                  InputError );

  m_phaseTypes.resize( numPhases );
  m_phaseOrder.resizeDefault( MAX_NUM_PHASES, -1 );

  for( integer ip = 0; ip < numPhases; ++ip )
  {
    m_phaseTypes[ip] = toPhaseType( m_phaseNames[ip], getFullName() );
    m_phaseOrder[m_phaseTypes[ip]] = ip;
  }

  // call to correctly set member array tertiary sizes on the 'main' material object
  resizeFields( 0, 0 );

  // set labels on array wrappers for plottable fields
  setLabels();
}

void RelativePermeabilityBase::resizeFields( localIndex const size, localIndex const numPts )
{
  integer const numPhases = numFluidPhases();

  m_phaseRelPerm.resize( size, numPts, numPhases );
  m_dPhaseRelPerm_dPhaseVolFrac.resize( size, numPts, numPhases, numPhases );
}

void RelativePermeabilityBase::setLabels()
{
  getWrapper< array3d< real64, relperm::LAYOUT_RELPERM > >( viewKeyStruct::phaseRelPermString() ).
    setDimLabels( 2, m_phaseNames );
}

void RelativePermeabilityBase::allocateConstitutiveData( dataRepository::Group & parent,
                                                         localIndex const numConstitutivePointsPerParentIndex )
{
  ConstitutiveBase::allocateConstitutiveData( parent, numConstitutivePointsPerParentIndex );
  resizeFields( parent.size(), numConstitutivePointsPerParentIndex );
}

} // namespace constitutive

} // namespace geosx

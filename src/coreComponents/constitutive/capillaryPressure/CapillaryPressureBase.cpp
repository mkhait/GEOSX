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
 * @file CapillaryPressureBase.cpp
 */

#include "CapillaryPressureBase.hpp"

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
    { "gas", CapillaryPressureBase::PhaseType::GAS },
    { "oil", CapillaryPressureBase::PhaseType::OIL },
    { "water", CapillaryPressureBase::PhaseType::WATER }
  };
  return findOption( phaseDict, lookup, CapillaryPressureBase::viewKeyStruct::phaseNamesString(), groupName );
}

} // namespace

CapillaryPressureBase::CapillaryPressureBase( string const & name,
                                              Group * const parent )
  : ConstitutiveBase( name, parent )
{
  registerWrapper( viewKeyStruct::phaseNamesString(), &m_phaseNames ).
    setSizedFromParent( 0 ).
    setInputFlag( InputFlags::REQUIRED ).
    setDescription( "List of fluid phases" );

  registerWrapper( viewKeyStruct::phaseTypesString(), &m_phaseTypes ).
    setSizedFromParent( 0 );

  registerWrapper( viewKeyStruct::phaseOrderString(), &m_phaseOrder ).
    setSizedFromParent( 0 );

  registerWrapper( viewKeyStruct::phaseCapPressureString(), &m_phaseCapPressure ).
    setPlotLevel( PlotLevel::LEVEL_0 );

  registerWrapper( viewKeyStruct::dPhaseCapPressure_dPhaseVolFractionString(), &m_dPhaseCapPressure_dPhaseVolFrac );
}

void CapillaryPressureBase::postProcessInput()
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
    m_phaseOrder[m_phaseTypes[ip]] = LvArray::integerConversion< integer >( ip );
  }

  GEOSX_THROW_IF( m_phaseOrder[CapillaryPressureBase::REFERENCE_PHASE] < 0,
                  getFullName() << ": reference oil phase has not been defined and should be included in model", InputError );

  // call to correctly set member array tertiary sizes on the 'main' material object
  resizeFields( 0, 0 );

  // set labels on array wrappers for plottable fields
  setLabels();
}

void CapillaryPressureBase::resizeFields( localIndex const size,
                                          localIndex const numPts )
{
  integer const NP = numFluidPhases();

  m_phaseCapPressure.resize( size, numPts, NP );
  m_dPhaseCapPressure_dPhaseVolFrac.resize( size, numPts, NP, NP );
}

void CapillaryPressureBase::setLabels()
{
  getWrapper< array3d< real64, cappres::LAYOUT_CAPPRES > >( viewKeyStruct::phaseCapPressureString() ).
    setDimLabels( 2, m_phaseNames );
}

void CapillaryPressureBase::allocateConstitutiveData( dataRepository::Group & parent,
                                                      localIndex const numConstitutivePointsPerParentIndex )
{
  resizeFields( parent.size(), numConstitutivePointsPerParentIndex );
  ConstitutiveBase::allocateConstitutiveData( parent, numConstitutivePointsPerParentIndex );
}

} // namespace constitutive

} // namespace geosx

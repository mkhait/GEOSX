/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2019 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2019 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2019 Total, S.A
 * Copyright (c) 2019-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */


/**
 * @file Damage.cpp
 */

#include "Damage.hpp"

#include "LinearElasticAnisotropic.hpp"
#include "LinearElasticIsotropic.hpp"
#include "LinearElasticTransverseIsotropic.hpp"

namespace geosx
{

using namespace dataRepository;
namespace constitutive
{

template< typename BASE >
Damage< BASE >::Damage( string const & name, Group * const parent ):
  BASE( name, parent ),
  m_damage(),
  m_strainEnergyDensity()
{

  this->registerWrapper( viewKeyStruct::damageString, &m_damage )->
    setApplyDefaultValue( 0.0 )->
    setPlotLevel( PlotLevel::LEVEL_0 )->
    setDescription( "Material Damage Variable" );

  this->registerWrapper( viewKeyStruct::strainEnergyDensityString, &m_strainEnergyDensity )->
    setApplyDefaultValue( 0.0 )->
    setPlotLevel( PlotLevel::LEVEL_0 )->
    setDescription( "Stress Deviator" );

}

template< typename BASE >
Damage< BASE >::~Damage()
{}

template< typename BASE >
void Damage< BASE >::PostProcessInput()
{}

template< typename BASE >
void Damage< BASE >::DeliverClone( string const & name,
                                        Group * const parent,
                                        std::unique_ptr< ConstitutiveBase > & clone ) const
{
  if( !clone )
  {
    clone = std::make_unique< Damage< BASE > >( name, parent );
  }
  BASE::DeliverClone( name, parent, clone );
  Damage< BASE > * const newConstitutiveRelation = dynamic_cast< Damage< BASE > * >(clone.get());

  newConstitutiveRelation->m_damage = m_damage;
  newConstitutiveRelation->m_strainEnergyDensity = m_strainEnergyDensity;
}

template< typename BASE >
void Damage< BASE >::AllocateConstitutiveData( dataRepository::Group * const parent,
                                                    localIndex const numConstitutivePointsPerParentIndex )
{
  BASE::AllocateConstitutiveData( parent, numConstitutivePointsPerParentIndex );

  m_damage.resize( parent->size(), numConstitutivePointsPerParentIndex );
  m_strainEnergyDensity.resize( parent->size(), numConstitutivePointsPerParentIndex );

}

typedef Damage< LinearElasticIsotropic > DamageLinearElasticIsotropic;

REGISTER_CATALOG_ENTRY( ConstitutiveBase, DamageLinearElasticIsotropic, string const &, Group * const )

}
} /* namespace geosx */
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
 * @file MultiscaleMeshLevel.hpp
 */

#ifndef GEOSX_LINEARALGEBRA_MULTISCALE_MULTISCALEMESHLEVEL_HPP
#define GEOSX_LINEARALGEBRA_MULTISCALE_MULTISCALEMESHLEVEL_HPP

#include "common/DataTypes.hpp"
#include "linearAlgebra/multiscale/MeshObjectManager.hpp"
#include "linearAlgebra/utilities/LinearSolverParameters.hpp"

namespace geosx
{

class DomainPartition;
class MeshLevel;

namespace multiscale
{

/**
 * @brief
 */
class MeshLevel
{
public:

  explicit MeshLevel( string const & name );

  MeshObjectManager & cellManager() { return m_cellManager; }
  MeshObjectManager const & cellManager() const { return m_cellManager; }

  MeshObjectManager & nodeManager() { return m_nodeManager; }
  MeshObjectManager const & nodeManager() const { return m_nodeManager; }

  void buildFineMesh( geosx::MeshLevel & mesh,
                      std::vector< string > const & regions );

  void buildCoarseMesh( multiscale::MeshLevel & fineMesh,
                        LinearSolverParameters::Multiscale::Coarsening const & coarse_params,
                        array1d< string > const & boundaryNodeSets );

  void writeCellData( std::vector< string > const & fieldNames ) const;

  void writeNodeData( std::vector< string > const & fieldNames ) const;

  string const & name() const { return m_name; }

  DomainPartition * domain() const { return m_domain; }
  geosx::MeshLevel * sourceMesh() const { return m_sourceMesh; }
  multiscale::MeshLevel * fineMesh() const { return m_fineMesh; }

private:

  void writeCellDataFine( std::vector< string > const & fieldNames ) const;
  void writeCellDataCoarse( std::vector< string > const & fieldNames ) const;
  void writeNodeDataFine( std::vector< string > const & fieldNames ) const;
  void writeNodeDataCoarse( std::vector< string > const & fieldNames ) const;

  string m_name; ///< Unique name prefix

  conduit::Node m_rootNode;     ///< not used per se, but needed to satisfy Group constructor
  dataRepository::Group m_root; ///< needed for ObjectManagerBase constructor

  MeshObjectManager m_cellManager; ///< Cell manager
  MeshObjectManager m_nodeManager; ///< Node manager

  DomainPartition * m_domain{};         ///< Pointer to domain required to access communicators
  multiscale::MeshLevel * m_fineMesh{}; ///< Pointer to parent fine mesh (saved on each coarse level)
  geosx::MeshLevel * m_sourceMesh{};    ///< Pointer to GEOSX mesh level (saved on the finest level mesh)
  std::vector< string > m_regions;      ///< List of regions in the source GEOSX mesh
};

} // namespace multiscale
} // namespace geosx

#endif //GEOSX_LINEARALGEBRA_MULTISCALE_MULTISCALEMESHLEVEL_HPP

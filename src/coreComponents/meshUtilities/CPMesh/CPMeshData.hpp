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
 * @file CPMeshData.hpp
 */


#ifndef GEOSX_MESHUTILITIES_CPMESH_CPMESHDATA_HPP_
#define GEOSX_MESHUTILITIES_CPMESH_CPMESHDATA_HPP_

#include "common/DataTypes.hpp"
#include "dataRepository/ObjectCatalog.hpp"

namespace geosx
{

namespace CPMesh
{

/**
 * @struct CPMeshDimensions
 * @brief Struct storing global and local mesh dimensions
 */
struct CPMeshDimensions
{
  CPMeshDimensions()
    :
    m_nX( 0 ),
    m_nY( 0 ),
    m_nZ( 0 ),
    m_iMin( 0 ),
    m_jMin( 0 ),
    m_iMax( 0 ),
    m_jMax( 0 ),
    m_iMinOverlap( 0 ),
    m_jMinOverlap( 0 ),
    m_iMaxOverlap( 0 ),
    m_jMaxOverlap( 0 )
  {}

  /**
   * @brief Const getter for the number of cells in the X-direction
   * @return the number of cells in the X-direction
   */
  localIndex nX() const { return m_nX; }

  /**
   * @brief Const getter for the number of cells in the Y-direction
   * @return the number of cells in the Y-direction
   */
  localIndex nY() const { return m_nY; }

  /**
   * @brief Const getter for the number of cells in the Z-direction
   * @return the number of cells in the Z-direction
   */
  localIndex nZ() const { return m_nZ; }


  /**
   * @brief Const getter for the number of cells in the X-direction in the MPI partition (including overlap)
   * @return the number of cells in the X-direction in the MPI partition (including overlap)
   */
  localIndex nXLocal() const { return (m_iMax+m_iMaxOverlap)-(m_iMin-m_iMinOverlap)+1; }

  /**
   * @brief Const getter for the number of cells in the Y-direction in the MPI partition (including overlap)
   * @return the number of cells in the Y-direction in the MPI partition (including overlap)
   */
  localIndex nYLocal() const { return (m_jMax+m_jMaxOverlap)-(m_jMin-m_jMinOverlap)+1; }

  /**
   * @brief Const getter for the number of cells in the Z-direction in the MPI partition (including overlap)
   * @return the number of cells in the Z-direction in the MPI partition (including overlap)
   */
  localIndex nZLocal() const { return m_nZ; }


  /**
   * @brief Const getter for the first index of the MPI partition in the X-direction
   * @return the first index of the MPI partition in the X-direction
   */
  localIndex iMinLocal() const { return m_iMin-m_iMinOverlap; }

  /**
   * @brief Const getter for the first index of the MPI partition in the Y-direction
   * @return the first index of the MPI partition in the Y-direction
   */
  localIndex jMinLocal() const { return m_jMin-m_jMinOverlap; }

  /**
   * @brief Const getter for the last index of the MPI partition in the X-direction
   * @return the last index of the MPI partition in the X-direction
   */
  localIndex iMaxLocal() const { return m_iMax+m_iMaxOverlap; }

  /**
   * @brief Const getter for the last index of the MPI partition in the Y-direction
   * @return the last index of the MPI partition in the Y-direction
   */
  localIndex jMaxLocal() const { return m_jMax+m_jMaxOverlap; }


  /**
   * @brief Const getter for the size of the overlap in the X-minus direction
   * @return the size of the overlap in the X-minus direction
   */
  localIndex iMinOverlap() const { return m_iMinOverlap; }

  /**
   * @brief Const getter for the size of the overlap in the Y-minus direction
   * @return the size of the overlap in the Y-minus direction
   */
  localIndex jMinOverlap() const { return m_jMinOverlap; }

  /**
   * @brief Const getter for the size of the overlap in the X-plus direction
   * @return the size of the overlap in the X-plus direction
   */
  localIndex iMaxOverlap() const { return m_iMaxOverlap; }

  /**
   * @brief Const getter for the size of the overlap in the Y-plus direction
   * @return the size of the overlap in the Y-plus direction
   */
  localIndex jMaxOverlap() const { return m_jMaxOverlap; }

  /**
   * @brief Define the domain dimensions
   * @param[in] nX the number of cells in the X direction
   * @param[in] nY the number of cells in the Y direction
   * @param[in] nZ the number of cells in the Z direction
   */
  void defineDomainDimensions( localIndex const nX, localIndex const nY, localIndex const nZ )
  {
    m_nX = nX;
    m_nY = nY;
    m_nZ = nZ;
  }

  /**
   * @brief Define the partition boundaries
   * @param[in] iMin first index in the X direction
   * @param[in] jMin first index in the Y direction
   * @param[in] iMax last index in the X direction
   * @param[in] jMax last index in the Y direction
   */
  void definePartitionBoundaries( localIndex const iMin, localIndex const jMin,
                                  localIndex const iMax, localIndex const jMax )
  {
    m_iMin = iMin;
    m_jMin = jMin;
    m_iMax = iMax;
    m_jMax = jMax;
  }

  /**
   * @brief Define the partition overlaps
   * @param[in] iMinOverlap overlap size in the X-minus direction
   * @param[in] jMinOverlap overlap size in the Y-minus direction
   * @param[in] iMaxOverlap overlap size in the X-plus direction
   * @param[in] jMaxOverlap overlap size in the Y-plus direction
   */
  void definePartitionOverlaps( localIndex const iMinOverlap, localIndex const jMinOverlap,
                                localIndex const iMaxOverlap, localIndex const jMaxOverlap )
  {
    m_iMinOverlap = iMinOverlap;
    m_jMinOverlap = jMinOverlap;
    m_iMaxOverlap = iMaxOverlap;
    m_jMaxOverlap = jMaxOverlap;
  }

private:

  /// global information

  /// total number of cells in the x direction
  localIndex m_nX;

  /// total number of cells in the y direction
  localIndex m_nY;

  /// total number of cells in the z direction
  localIndex m_nZ;



  /// local information: definition of the MPI partition

  /// beginning of the MPI partition in the x direction
  localIndex m_iMin;

  /// beginning of the MPI partition in the y direction
  localIndex m_jMin;

  /// end of the MPI partition in the x direction
  localIndex m_iMax;

  /// end of the MPI partition in the y direction
  localIndex m_jMax;

  /// overlap in the x-min direction
  localIndex m_iMinOverlap;

  /// overlap in the y-min direction
  localIndex m_jMinOverlap;

  /// overlap in the x-max direction
  localIndex m_iMaxOverlap;

  /// overlap in the y-max direction
  localIndex m_jMaxOverlap;

};

/**
 * @struct CPMeshVertices
 * @brief Struct storing vertex information
 */
struct CPMeshVertices
{

  // vertices

  /// vertices obtained by filtering out duplicates in m_cPVertices
  array2d< real64 > m_vertices;

  /// map from vertex local index to vertex global index
  array1d< globalIndex > m_vertexToGlobalVertex;

  // corner-point vertices

  /// original (duplicated) CPG vertices
  array2d< real64 > m_cPVertices;

  /// true if the corner-point vertex belongs to a pillar inside this partition, false otherwise
  array1d< bool > m_cPVertexIsInsidePartition;

  /// map from cPVertex local index to unique (filtered) vertex
  array1d< localIndex > m_cPVertexToVertex;

  /// map from cPVertex local index to cPVertex global index
  array1d< globalIndex > m_cPVertexToGlobalCPVertex;


};

/**
 * @struct CPMeshFaces
 * @brief Struct storing faces information
 */
struct CPMeshFaces
{
  // TODO
};

/**
 * @struct CPMeshCells
 * @brief Struct storing cell information
 */
struct CPMeshCells
{

  /// map from active cell (inside partition) local index to active cell local index
  array1d< localIndex > m_activeCellInsidePartitionToActiveCell;

  /// map from active cell local index to cell local index
  array1d< localIndex > m_activeCellToCell;

  /// map from local index to original CPG nodes
  array1d< localIndex > m_cellToCPVertices;

  /// map from active cell local index to cell global index
  array1d< globalIndex > m_activeCellToGlobalCell;

};


} // end namespace CPMesh

} // end namespace geosx

#endif //GEOSX_MESHUTILITIES_CPMESH_CPMESHDATA_HPP_

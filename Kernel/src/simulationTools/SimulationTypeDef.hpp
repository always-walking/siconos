/* Siconos-Kernel, Copyright INRIA 2005-2012.
 * Siconos is a program dedicated to modeling, simulation and control
 * of non smooth dynamical systems.
 * Siconos is a free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * Siconos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Siconos; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Contact: Vincent ACARY, siconos-team@lists.gforge.inria.fr
*/

/*! \file SimulationTypeDef.hpp
 * \brief Typedef for simulation-related objects
 */

#ifndef SimulationTypedef_H
#define SimulationTypedef_H

#include <vector>
#include <map>
#include <set>

#include "SiconosPointers.hpp"

#include "Interaction.hpp"

/** double precision machine */
/*  eq dlmach('e'),  DBL_EPSILON,  fabs(a-b) <  */
const double MACHINE_PREC = std::numeric_limits<double>::epsilon();



// ================== Objects to handle DS ==================

/** Map of SP::SimpleMatrix; key = the number of the related DS*/
typedef std::map<unsigned int, SP::SimpleMatrix> MapOfDSMatrices;

/** Iterator through a map of matrices */
typedef MapOfDSMatrices::iterator MatIterator;

/** Const iterator through a map of matrices */
typedef MapOfDSMatrices::const_iterator ConstMatIterator;

// ================== Objects to handle Interactions ==================

/** Map of SiconosMatrices with a Interactions as a key - Used for diagonal interactionBlock-terms in assembled matrices of LCP etc ...*/
typedef std::map< SP::Interaction, SP::SiconosMatrix>  MapOfInteractionMatrices;

/** Iterator through a MapOfInteractionMatrices */
typedef MapOfInteractionMatrices::iterator InteractionMatrixColumnIterator ;

/** Const iterator through a MapOfInteractionMatrices */
typedef MapOfInteractionMatrices::const_iterator ConstInteractionMatrixColumnIterator;

/** Map of MapOfDSInteractionMatrices with a DynamicalSystem as a key - Used for interactionBlock-terms indexed by a DynamicalSystem and an Interaction in assembled matrices of LCP etc ..*/
typedef std::map< SP::DynamicalSystem , MapOfInteractionMatrices >  MapOfDSMapOfInteractionMatrices;

/** Iterator through a MapOfDSMapOfInteractionMatrices */
typedef MapOfDSMapOfInteractionMatrices::iterator DSInteractionMatrixRowIterator ;

/** Const iterator through a MapOfDSMapOfInteractionMatrices */
typedef MapOfDSMapOfInteractionMatrices::const_iterator ConstDSInteractionMatrixRowIterator ;




/** Map of MapOfInteractionMapOfDSMatrices with a DynamicalSystem as a key - Used for interactionBlock-terms indexed by a DynamicalSystem and an Interaction in assembled matrices of LCP etc ..*/
typedef std::map< SP::Interaction , MapOfDSMatrices >  MapOfInteractionMapOfDSMatrices;

/** Iterator through a MapOfInteractionMapOfDSMatrices */
typedef MapOfInteractionMapOfDSMatrices::iterator InteractionDSMatrixRowIterator ;

/** Const iterator through a MapOfInteractionMapOfDSMatrices */
typedef MapOfInteractionMapOfDSMatrices::const_iterator ConstInteractionDSMatrixRowIterator ;

/** list of indices */
typedef std::vector<unsigned int> IndexInt;
TYPEDEF_SPTR(IndexInt)
TYPEDEF_SPTR(VectorOfBlockVectors)
TYPEDEF_SPTR(VectorOfVectors)
TYPEDEF_SPTR(VectorOfMatrices)
TYPEDEF_SPTR(VectorOfSMatrices)

/** \struct InteractionProperties mandatory properties for an Interaction  */
struct InteractionProperties
{
  SP::SiconosMatrix block;    // diagonal block
  SP::DynamicalSystem source;
  unsigned int source_pos;
  SP::DynamicalSystem target;
  unsigned int target_pos;
  SP::OneStepIntegrator osi;
  bool forControl;            /**< true if the relation is used to control the DS */
  SP::VectorOfBlockVectors DSlink;     /**< pointer links to DS variables needed for computation, mostly x (or q), z, r (or p) */
  SP::VectorOfVectors workVectors;     /**< set of SiconosVector, mostly to have continuous memory vectors (not the case with BlockVector in DSlink) */
  SP::VectorOfSMatrices workMatrices;   /**< To store jacobians */


  ACCEPT_SERIALIZATION(InteractionProperties);
};

/** \struct SystemProperties mandatory properties for a DynamicalSystems */
struct SystemProperties
{
  SP::SiconosMatrix upper_block;   /**< i,j block i<j */
  SP::SiconosMatrix lower_block;   /**< i,j block i>j */
  SP::VectorOfVectors workVectors;     /**< Used for instance in Newton iteration */
  SP::VectorOfMatrices workMatrices;   /**< Mostly for Lagrangian system */
//  SP::SiconosMemory _xMemory       /**< old value of x, TBD */

  ACCEPT_SERIALIZATION(SystemProperties);
};

struct GraphProperties
{
  bool symmetric;

  ACCEPT_SERIALIZATION(GraphProperties);
};





// ================== Objects to handle OSI ==================


/** Vector of OneStepIntegrator */
typedef std::set<SP::OneStepIntegrator> OSISet;

/** Iterator through vector of OSI*/
typedef OSISet::iterator OSIIterator;
/** A map that links DynamicalSystems and their OneStepIntegrator. */
typedef std::map<SP::DynamicalSystem, SP::OneStepIntegrator> DSOSIMap;

/** Iterator through a DSOSIMap. */
typedef DSOSIMap::iterator DSOSIIterator;

/** Const Iterator through a DSOSIMap. */
typedef DSOSIMap::const_iterator DSOSIConstIterator;

// ================== Objects to handle OSNS ==================

/** Map of OSNS */
//typedef std::map<std::string, SP::OneStepNSProblem > OneStepNSProblems;
typedef std::vector<SP::OneStepNSProblem> OneStepNSProblems;

/** Iterator through OneStepNSProblems */
typedef OneStepNSProblems::iterator OSNSIterator;

/** Const iterator through OneStepNSProblems */
typedef OneStepNSProblems::const_iterator ConstOSNSIterator;

// ================== Misc ==================

/** default tolerance value, used to update index sets */
const double DEFAULT_TOLERANCE = 10 * MACHINE_PREC;

enum SICONOS_OSNSP
{
  SICONOS_OSNSP_DEFAULT = 0
};
enum SICONOS_OSNSP_ED
{
  SICONOS_OSNSP_ED_SMOOTH_ACC,
  SICONOS_OSNSP_ED_IMPACT,
  SICONOS_OSNSP_ED_SMOOTH_POS
};
enum SICONOS_OSNSP_TS
{
  SICONOS_OSNSP_TS_VELOCITY = 0,
  SICONOS_OSNSP_TS_POS = 1
};
const int SICONOS_NB_OSNSP_TS = 1;
const int SICONOS_NB_OSNSP_TSP = 2;

/** Event constants */
#define TD_EVENT 1
#define NS_EVENT 2

TYPEDEF_SPTR(OSISet)
TYPEDEF_SPTR(OneStepNSProblems)

#endif

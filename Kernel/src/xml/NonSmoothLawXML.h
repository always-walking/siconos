/* Siconos-Kernel version 1.2.0, Copyright INRIA 2005-2006.
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
 * Contact: Vincent ACARY vincent.acary@inrialpes.fr
 */
/** \class NSLawXML
 *   \brief This class manages NSLaw data part
 *  \author SICONOS Development Team - copyright INRIA
 *   \version 1.2.0.
 *   \date 05/13/2004
 *
 *
 *
 * NSLawXML allows to manage data of a NSLaw DOM tree.
 */


#ifndef __NSLawXML__
#define __NSLawXML__

#include "SiconosDOMTreeTools.h"
//#include "NonSmoothLaw.h"

class NonSmoothLaw;

//const string NSLAW_TYPE = "type";
class NonSmoothLawXML
{
protected:

  /** rootNode */
  xmlNodePtr rootNode;

  /** size */
  xmlNodePtr sizeNode;

public:

  /** \fn NonSmoothLawXML()
   *   \brief default constructor
   */
  NonSmoothLawXML();

  /** \fn NonSmoothLawXML(xmlNodePtr);
   *   \brief constructor from DOM tree data
   */
  NonSmoothLawXML(xmlNodePtr);

  /** \fn ~NonSmoothLawXML()
   *   \brief destructor
   */
  virtual ~NonSmoothLawXML();

  /** \fn const string getType() const
   *   \brief get the type of non smooth law
   *   \return a string
   */
  inline const std::string getType() const
  {
    std::string type((char*) rootNode->name);
    return type;
  }

  /** \fn xmlNodePtr getNode()
   *   \brief Return node named "NonSmoothLaw"
   *   \return an xmlNodePtr
   */
  inline xmlNodePtr getNode() const
  {
    return rootNode;
  }

  /** \fn const bool hasSize() const
  *  \brief return true if size node is present
  *  \return a bool
  */
  const bool hasSize() const
  {
    return (sizeNode != NULL);
  };

  /** \fn const unsigned int getSize() const
   *   \brief Return the size of the InteractionXML
   *   \return an unsigned int
   */
  inline const unsigned int getSize() const
  {
    if (!hasSize())
      XMLException::selfThrow("NonSmoothLawXML::getSize() : sizeNode == NULL");
    return SiconosDOMTreeTools::getContentValue<int>(sizeNode);
  }

  /** \fn void setSize(const unsigned int& i)
   *  \brief to save the size of the Interaction
   *  \return an unsigned int
   */
  void setSize(const unsigned int);

  /** \fn void updateNonSmoothLawXML( xmlNodePtr node, NonSmoothLaw* nsl )
   *   \brief makes the operations to create the NonSmoothLaw of the Interaction
   *   \param xmlNodePtr : the root node of the NonSmoothLawXML
   *   \param Relation* : the NonSmoothLaw of this NonSmoothLawXML
   */
  void updateNonSmoothLawXML(xmlNodePtr node, NonSmoothLaw* nsl);
};

#endif

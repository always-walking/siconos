/* Siconos-Kernel version 3.0.0, Copyright INRIA 2005-2008.
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

/*! \file InteractionsSet.h
Set of SP::Interaction
*/
#ifndef InteractionsSET_H
#define InteractionsSET_H

#include "SiconosSet.h"
#include "Interaction.h"

/** A set of pointers to interactions, sorted in a growing order according to their address */
typedef SiconosSet<Interaction, double*> InteractionsSet;
/** Iterator through a set of Interactions */
typedef std::set<SP::Interaction, Cmp<Interaction, double*> >::iterator InteractionsIterator;
/** const Iterator through a set of Interactions */
typedef std::set<SP::Interaction, Cmp<Interaction, double*> >::const_iterator ConstInteractionsIterator;
/** return type value for insert function - bool = false if insertion failed. */
typedef std::pair<InteractionsIterator, bool> CheckInsertInteraction;

TYPEDEF_SPTR(InteractionsSet);

#endif

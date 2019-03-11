/* Siconos is a program dedicated to modeling, simulation and control
 * of non smooth dynamical systems.
 *
 * Copyright 2018 INRIA.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#include <math.h>
#include "projectionOnRollingCone.h"


unsigned projectionOnRollingCone(double* r, double  mu, double mur)
{
  double normT = hypot(r[1], r[2]);
  double normMT = hypot(r[3], r[4]);


  if ((mu * normT <= - r[0]) && ((mur * normMT <= - r[0])))
  {
    r[0] = 0.0;
    r[1] = 0.0;
    r[2] = 0.0;
    r[3] = 0.0;
    r[4] = 0.0;
    return PROJRCONE_DUAL;
  }
  else if ((normT <= mu * r[0]) && (normMT <= mur * r[0]))
  {
    return PROJRCONE_INSIDE;
  }
  else
  {
    if ((normT > mu * r[0]) && (normMT <= mur * r[0]))
    {
      double mu2 = mu * mu;
      r[0] = (mu * normT + r[0]) / (mu2 + 1.0);
      r[1] = mu * r[0] * r[1] / normT;
      r[2] = mu * r[0] * r[2] / normT;
      //r[3] = r[3] ;
      //r[4] = r[4] ;
      return PROJRCONE_BOUNDARY_FRICTION;
    }
    else if ((normT > mu * r[0]) && (normMT > mur * r[0]))
    {
      double mu2 = mu * mu;
      double mur2 = mur*mur;
      r[0] = (mu * normT + mur * normMT + r[0]) / (mur2+ mu2 + 1.0);
      r[1] = mu * r[0] * r[1] / normT;
      r[2] = mu * r[0] * r[2] / normT;
      r[3] = mur * r[0] * r[3] / normMT;
      r[4] = mur * r[0] * r[4] / normMT;
      return PROJRCONE_BOUNDARY_FRICTION_ROLLING;
    }

    else if ((normT <= mu * r[0]) && (normMT > mur * r[0]))
    {
      double mur2 = mur*mur;
      r[0] = (mur * normMT + r[0]) / (mur2 + 1.0);
      //r[1] = r[1] ;
      //r[2] = r[2] ;
      r[3] = mur * r[0] * r[3] / normMT;
      r[4] = mur * r[0] * r[4] / normMT;
      return PROJRCONE_BOUNDARY_ROLLING;
    }
    else
      return 18;
  }
}
unsigned projectionOnDualRollingCone(double* u, double  mu, double mur)
{
  return 0;

}

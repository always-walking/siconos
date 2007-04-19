/* Siconos-sample version 2.0.1, Copyright INRIA 2005-2006.
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
#include "Robot.h"
#include <stdio.h>
#include <math.h>

const unsigned int n0 = 7; // Real problem dimension

extern "C" void mass(unsigned int sizeOfq, const double *q, double *mass, unsigned int sizeZ, double* z)
{
  unsigned int n = sizeOfq;
  unsigned int n1 = n0 * n0;

  unsigned int i, j;

  double massTmp[n1];
  // mass set to zero
  for (i = 0; i < n1; i++)
    massTmp[i] = 0.0;

  double qTmp[n0];
  for (i = 1; i < n0; i++)
    qTmp[i] = 0.0;

  // compute mass matrix
  Inertia(massTmp, qTmp);

  // Motor Inertia effect
  massTmp[0]  += 0.75;
  massTmp[8]  += 0.75;
  massTmp[16] += 0.2125;
  massTmp[24] += 0.2125;
  massTmp[32] += 0.00575;
  massTmp[40] += 0.00575;
  massTmp[48] += 0.00575;

  // compute reduced mass matrix
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      mass[j + n * i] = massTmp[n0 * 2 * i + n0 + 2 * j + 1];
    }
  }
}

extern "C" void NNL(unsigned int sizeOfq, const double *q, const double *velocity, double *NNL, unsigned int sizeZ, double* z)
{
  unsigned int n = sizeOfq;

  unsigned int i;

  double NNLTmp[n0];
  // NNL set to zero
  for (i = 0; i < n0; i++)
    NNLTmp[i] = 0.0;

  double qTmp[n0], vTmp[n0];
  for (i = 1; i < n0; i++)
  {
    qTmp[i] = 0.0;
    vTmp[i] = 0.0;
  }
  for (i = 0; i < n; i++)
  {
    qTmp[2 * i + 1] = q[i];
    vTmp[2 * i + 1] = velocity[i];
  }

  // compute mass matrix
  NLEffects(NNLTmp, qTmp, vTmp);

  NNLTmp[0] += 10 * vTmp[0];
  NNLTmp[1] += 10 * vTmp[1];
  NNLTmp[2] += 5 * vTmp[2];
  NNLTmp[3] += 5 * vTmp[3];
  NNLTmp[4] += 2 * vTmp[4];
  NNLTmp[5] += 2 * vTmp[5];
  NNLTmp[6] += 2 * vTmp[6];

  // compute reduced NNL vector
  for (i = 0; i < n; i++)
    NNL[i] = NNLTmp[2 * i + 1];

}

extern "C" void jacobianQNNL(unsigned int sizeOfq, const double *q, const double *velocity, double *jacob, unsigned int sizeZ, double* z)
{
  unsigned int n = sizeOfq;
  unsigned int n1 = n0 * n0;

  unsigned int i, j;

  double jacoTmp[n1];
  // set to zero
  for (i = 0; i < n1; i++)
    jacoTmp[i] = 0.0;

  double qTmp[n0], vTmp[n0];

  for (i = 0; i < n0; i++)
  {
    qTmp[i] = 0.0;
    vTmp[i] = 0.0;
  }
  for (i = 0; i < n; i++)
  {
    qTmp[2 * i + 1] = q[i];
    vTmp[2 * i + 1] = velocity[i];
  }

  // compute jacobian matrix
  JacobianQNLEffects(jacoTmp, qTmp, vTmp);

  // compute reduced jacobian matrix
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      jacob[j + n * i] = jacoTmp[n0 * 2 * i + n0 + 2 * j + 1];
    }
  }
}

extern "C" void jacobianVNNL(unsigned int sizeOfq, const double *q, const  double *velocity, double *jacob, unsigned int sizeZ, double* z)
{
  unsigned int n = sizeOfq;
  unsigned int n1 = n0 * n0;

  unsigned int i, j;

  double jacoTmp[n1];
  // set to zero
  for (i = 0; i < n1; i++)
    jacoTmp[i] = 0.0;

  double qTmp[n0], vTmp[n0];
  for (i = 0; i < n0; i++)
  {
    qTmp[i] = 0.0;
    vTmp[i] = 0.0;
  }
  for (i = 0; i < n; i++)
  {
    qTmp[2 * i + 1] = q[i];
    vTmp[2 * i + 1] = velocity[i];
  }

  // compute jacobian matrix
  JacobianVNLEffects(jacoTmp, qTmp, vTmp);

  // compute reduced jacobian matrix
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      jacob[j + n * i] = jacoTmp[n0 * 2 * i + n0 + 2 * j + 1];
    }
  }
}

extern "C" void h0(unsigned int sizeOfq, const double* q, unsigned int sizeOfY, double* y, unsigned int sizeZ, double* z)
{
  y[0] = 0.45 * cos(q[0]);
}

extern "C" void G0(unsigned int sizeOfq, const double* q, unsigned int sizeOfY, double* G, unsigned int sizeZ, double* z)
{
  G[0] = -0.45 * sin(q[0]);
  G[1] = 0.0;
}


extern "C" void h1(unsigned int sizeOfq, const double* q, unsigned int sizeOfY, double* y, unsigned int sizeZ, double* z)
{
  y[0] = 0.45 * cos(q[0]) + 0.48 * cos(q[1] + q[0]);
}

extern "C" void G1(unsigned int sizeOfq, const double* q, unsigned int sizeOfY, double* G, unsigned int sizeZ, double* z)
{
  G[0] = -0.45 * sin(q[0]) - 0.48 * sin(q[1] + q[0]);
  G[1] = -0.48 * sin(q[1] + q[0]);
}

extern "C" void h2(unsigned int sizeOfq, const double* q, unsigned int sizeOfY, double* y, unsigned int sizeZ, double* z)
{
  //  printf("BJBJBJBJ%f\n",y[0]);
  //printf("BJBJBJBsddssdJ%f\n",y[1]);
  y[0] = 0.45 * cos(q[0]);
  y[1] = 0.45 * cos(q[0]) + 0.48 * cos(q[1] + q[0]);
  //printf("AAAAAAAAABJBJBJBJ%f\n",y[0]);
  //  printf("AAAAAABJBJBJBsddssdJ%f\n",y[1]);
}

extern "C" void G2(unsigned int sizeOfq, const double* q, unsigned int sizeOfY, double* G, unsigned int sizeZ, double* z)
{
  G[0] = -0.45 * sin(q[0]);
  G[1] = -0.45 * sin(q[0]) - 0.48 * sin(q[1] + q[0]);
  G[2] = 0.0;
  G[3] = -0.48 * sin(q[1] + q[0]);
  G[4] = 0.0;
  G[5] = 0.0;
}


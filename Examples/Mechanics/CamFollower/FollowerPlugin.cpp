/* Siconos-Samples version 2.0.1, Copyright INRIA 2005-2006.
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
#include <stdio.h>
#include <math.h>
#include "CamState.h"

const double g = 0; // gravity
const double m = 1.221;

extern "C" void FollowerMass(unsigned int sizeOfq, const double *q, double* mass, unsigned int sizeZ, double* z)
{
  double m = 1.221;
  // initialisation of the Mass matrix
  for (unsigned int i = 0; i < sizeOfq * sizeOfq; i++)
    mass[i] = 0.0;

  mass[0] = m;
}

extern "C" void FollowerQNLInertia(unsigned int sizeOfq, const double *q, const double *velocity, double *Q, unsigned int sizeZ, double* z)
{
  unsigned int i;

  for (i = 0; i < sizeOfq; i++)
    Q[i] = 0.0;
}

extern "C" void FollowerFInt(double time, unsigned int sizeOfq, const double *q, const double *velocity, double *fInt, unsigned int sizeZ, double* z)
{
  for (unsigned int i = 0; i < sizeOfq; i++)
    fInt[i] = 0.0;
}

extern "C" double FextFunction(double time, double* z)
{
  double rpm = *z;
  double res = -0.0;
  double w, beta, hc, hcp, hcpp, mass, kspring, cfriction, phio;
  double PI = 3.14159265;
  double Rb = 0.40;
  double k1 = 0.45;
  double k2 = 0.40393320723821;
  double rho1 = 0.85;
  double rho3 = 0.15;
  double rho2 = 0.55393320723821;
  double dBetadt, tmp1;
  double beta1 = 0;
  double beta2 = 1.050328174371336;
  double beta3 = 1.22173047639603;


  // char str [80],comando[80];
  //   float f;
  //   FILE * pFile;

  //   pFile = fopen ("parameters.txt","r");
  //   rewind (pFile);
  //   fscanf (pFile, "%f", &f);
  //   fclose (pFile);
  // printf ("%f\n",f);
  //rpm=(double)(f);

  phio = PI / 2;
  //  rpm=358;

  //  printf ("%f\n",rpm);
  //  rpm=150;
  w = 2 * PI * rpm / 60;
  mass = 1.221;
  kspring = 1430.8;
  cfriction = 0;

  beta = w * time;
  beta = fmod(beta + phio, 2 * PI);

  //hc=sin(beta)-0.9;
  //hp=w*cos(beta);
  //hpp=-w*w*sin(beta);

  if (beta > PI)
  {
    beta = 2 * PI - beta;
    w = -w;
  }
  dBetadt = w;
  if (beta <= (PI / 2 - beta1))
  {
    hc = Rb;
    hcp = 0;
    hcpp = 0;
  }
  else if (beta <= (PI / 2 + beta2))
  {
    hc = -k1 * sin(beta) + rho1 * sqrt(1 - pow((cos(beta)) * (k1 / rho1), 2)) ;
    hcp = (-k1 * cos(beta) + (pow(k1, 2) * sin(beta) * cos(beta)) / (rho1 * sqrt(1 - pow((cos(beta)) * (k1 / rho1), 2)))) * dBetadt;
    tmp1 = pow(cos(beta - beta1), 2) - pow(sin(beta - beta1), 2) - (pow(k1 * sin(beta - beta1) * cos(beta - beta1), 2)) / (pow(rho1, 2) * (1 - pow((cos(beta - beta1)) * (k1 / rho1), 2)));
    hcpp = (k1 * sin(beta - beta1) + (pow(k1, 2) / (rho1 * sqrt(1 - pow((cos(beta - beta1)) * (k1 / rho1), 2)))) * tmp1) * (pow(dBetadt, 2));
  }
  else if (beta <= (PI / 2 + beta3))
  {
    hc = -k2 * cos(beta + PI / 9) + rho3 * sqrt(1 - pow((sin(beta + PI / 9)) * (k2 / rho3), 2)) ;
    hcp = (k2 * sin(beta + PI / 9) - (pow(k2, 2) * sin(beta + PI / 9) * cos(beta + PI / 9)) / (rho3 * sqrt(1 - pow((sin(beta + PI / 9)) * (k2 / rho3), 2)))) * dBetadt;
    tmp1 = pow(cos(beta - beta3), 2) - pow(sin(beta - beta3), 2) - (pow(k2 * sin(beta - beta3) * cos(beta - beta3), 2)) / (pow(rho3, 2) * (1 - pow((cos(beta - beta3)) * (k2 / rho3), 2)));
    hcpp = (-k2 * sin(beta - beta3) + (pow(k2, 2) / (rho3 * sqrt(1 - pow((cos(beta - beta3)) * (k2 / rho3), 2)))) * tmp1) * pow(dBetadt, 2);
  }
  else
  {
    hc = rho2;
    hcp = 0;
    hcpp = 0;
  }

  hc = hc - 0.3;

  res = -(mass * hcpp + cfriction * hcp + kspring * hc);
  //res=0;
  return res;
}

extern "C" void FollowerFExtR(double time, unsigned int sizeOfq, double *fExt, unsigned int sizeZ, double* z)
{
  //double rpm = z[0];

  for (unsigned int i = 0; i < sizeOfq; i++)
    fExt[i] = 0.0;

  fExt[0] = -m * g;
}
extern "C" void FollowerFExt(double time, unsigned int sizeOfq, double *fExt, unsigned int sizeZ, double* z)
{
  double rpm = z[0];
  //  double rpm = 358;

  for (unsigned int i = 0; i < sizeOfq; i++)
    fExt[i] = 0.0;

  fExt[0] = -m * g + FextFunction(time, &rpm);
}

extern "C" void groundFExt(double time, unsigned int sizeOfq, double *fExt, unsigned int sizeZ, double* z)
{
  for (unsigned int i = 0; i < sizeOfq; i++)
    fExt[i] = 0.0;
}

extern "C" void ballJacobianQFInt(double time, unsigned int sizeOfq, const double *q, const double *velocity, double *jacob, unsigned int sizeZ, double* z)
{
  for (unsigned int i = 0; i < sizeOfq * sizeOfq; i++)
    jacob[i] = 0.0;
}

extern "C" void ballJacobianVelocityFInt(double time, unsigned int sizeOfq, const double *q, const double *velocity, double *jacob, unsigned int sizeZ, double* z)
{
  for (unsigned int i = 0; i < sizeOfq * sizeOfq; i++)
    jacob[i] = 0.0;
}

extern "C" void ballJacobianQQNLInertia(unsigned int sizeOfq, const double *q, const double *velocity, double *jacob, unsigned int sizeZ, double* z)
{
  for (unsigned int i = 0; i <  sizeOfq * sizeOfq; i++)
  {
    jacob[i] = 0.0;
  }
}

extern "C" void ballJacobianVelocityQNLInertia(unsigned int sizeOfq, const double *q, const double *velocity, double *jacob, unsigned int sizeZ, double* z)
{
  printf("Call of the function 'ballJacobianVelocityQ' of the basic plugin.\nYou have to implement this function.\n");
  for (unsigned int i = 0; i <  sizeOfq * sizeOfq; i++)
  {
    jacob[i] = 0.0;
  }
}



extern "C" void FollowerComputeH0(unsigned int sizeDS, const double* q, unsigned int sizeY, double* y, unsigned int sizeZ, double* z)
{

  y[0] = q[0];
}

extern "C" void FollowerComputeG0(unsigned int sizeDS, const double* q, unsigned int sizeY, double* G0, unsigned int sizeZ, double* z)
{
  G0[0] = 1;
}

extern "C" void FollowerComputeH1(unsigned int sizeDS, const double* q, double time, unsigned int sizeY, double* y, unsigned int sizeZ, double* z)
{
  double CamEqForce, CamPosition, CamVelocity, CamAcceleration, rpm = z[0];

  CamEqForce = CamState(time, rpm, CamPosition, CamVelocity, CamAcceleration);
  y[0] = q[0] - CamPosition;
  y[0] = q[0];
}


extern "C" void FollowerComputeG10(unsigned int sizeDS, const double* q, double time, unsigned int sizeY, double* G0, unsigned int sizeZ, double* z)
{
  G0[0] = 1;
}


extern "C" void FollowerComputeG11(unsigned int sizeDS, const double* q, double time, unsigned int sizeY, double* G1, unsigned int sizeZ, double* z)
{

  double CamEqForce, CamPosition, CamVelocity, CamAcceleration, rpm = z[0];

  CamEqForce = CamState(time, rpm, CamPosition, CamVelocity, CamAcceleration);
  G1[0] = -CamVelocity;
  G1[0] = 0;
}



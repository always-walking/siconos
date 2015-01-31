/* Siconos-sample , Copyright INRIA 2005-2012.
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

/* !\file SMCElectroPneumatic.cpp
  \brief Simulation of an Electropneumatic setup controlled with a Twisting algorithm
  O. Huber
  */

#include "SiconosKernel.hpp"
#include "SiconosControl.hpp"
using namespace std;

// main program
int main(int argc, char* argv[])
{

  // User-defined parameters
  unsigned ndof = 4;          // Number of degrees of freedom of your system
  double t0 = 0.0;                // Starting time
  double T = 100.0;                   // Total simulation time
  double h = 1.0e-3;              // Time step for simulation
  double hControl = 1.0e-1;       // Time step for control
  double y0 = 0.0;
  double v0 = 0.0;
  double pP0  = 501570.871800124;
  double pN0  = 501570.871800124;

  SP::SiconosVector param(new SiconosVector(3));
  (*param)(0) = 1e-2;
  (*param)(1) = 2./3.;
  (*param)(2) = 10.;
  // ================= Creation of the model =======================
  // Steps:
  // - create a Dynamical System
  // - add a Simulation to the model

  // -------------------------
  // --- Dynamical systems ---
  // -------------------------

  // First System:

  // Matrix declaration
  // For the DynamycalSystem
  SP::SiconosVector x0(new SiconosVector(ndof));
  (*x0)(0) = pP0;
  (*x0)(1) = pN0;
  (*x0)(2) = v0;
  (*x0)(3) = y0;

  // Dynamical Systems
  SP::FirstOrderNonLinearDS plant(new FirstOrderNonLinearDS(x0, "electro_pneumatic_nantes:computef", "electro_pneumatic_nantes:computeJacfx"));
  plant->setzPtr(param);

  // -------------
  // --- Model process ---
  // -------------
  SP::ControlLsodarSimulation simLsodar(new ControlLsodarSimulation(t0, T, h));

  simLsodar->addDynamicalSystem(plant, "plant");
  // ------------------
  // --- Simulation ---
  // ------------------
  // TimeDiscretisation
  // Control stuff
  // For the Sensor
  SP::SimpleMatrix sensorC(new SimpleMatrix(4, 4));
  sensorC->eye();
  SP::LinearSensor sensor(new LinearSensor(plant, sensorC));
  // add the sliding mode controller
  SP::LinearSMC twisting(new LinearSMC(sensor));

  twisting->seth("electro_pneumatic_nantes:computeh");
  twisting->setg("electro_pneumatic_nantes:computeg");
  twisting->setJachx("electro_pneumatic_nantes:computeJachx");
  twisting->setJachlambda("electro_pneumatic_nantes:computeJachlambda");
  twisting->setJacgx("electro_pneumatic_nantes:computeJacgx");
  twisting->setJacglambda("electro_pneumatic_nantes:computeJacglambda");

  twisting->noUeq(true);
  twisting->setSizeu(2);
  simLsodar->addSensor(sensor, hControl);
  simLsodar->addActuator(twisting, hControl);

  // =========================== End of model definition ===========================

  // ================================= Computation =================================

  // --- Simulation initialization ---

  cout << "====> Simulation initialisation ..." << endl << endl;
  // initialise the process and the ControlManager
  simLsodar->initialize();
//  (std11::static_pointer_cast<LsodarOSI>(simLsodar->integrator()))->setJT(1);
//  (std11::static_pointer_cast<LsodarOSI>(simLsodar->integrator()))->setMaxOrder(0, 5);
  simLsodar->run();
  SimpleMatrix& data = *simLsodar->data();
  ioMatrix::write("result.dat", "ascii", data, "noDim");
  std::cout << std::endl << simLsodar->dataLegend() << std::endl;

#if 0
  // Comparison with a reference file
  SimpleMatrix dataPlotRef(dataPlot);
  dataPlotRef.zero();
  ioMatrix::read("SMCExampleImplicit.ref", "ascii", dataPlotRef);
  std::cout << (dataPlot - dataPlotRef).normInf() << std::endl;
  if ((dataPlot - dataPlotRef).normInf() > 1e-12)
  {
    std::cout << "Warning. The results is rather different from the reference file." << std::endl;
    std::cout << (dataPlot - dataPlotRef).normInf() << std::endl;
    return 1;
  }
#else
  return 0;
#endif
  return 0;
}

// Copyright
//   2012-2015 ETH Zurich
// 
// License
//   This file is a part of WindDrivenRain.
// 
//   WindDrivenRain is free software: you can redistribute it and/or modify it
//   under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
// 
//   WindDrivenRain is distributed in the hope that it will be useful, but
//   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//   more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with WindDrivenRain. If not, see <http://www.gnu.org/licenses/>.
// 
// Application
//   wind-driven-rain
// 
// Description
//   Solves for wind-driven rain with an Eulerian multiphase model
//   Written by Aytac Kubilay, March 2012, ETH Zurich/Empa
//
//   Adaptation for OpenFOAM(R) 5.x API & code reorganization
//   by Alexey Matveichev
// 
//   Latest Update: 9.12.2017

#include "OStringStream.H"
#include "Time.H"
#include "fvMesh.H"
#include "volFields.H"
#include "uniformDimensionedFields.H"


#include "fvmDdt.H"
#include "fvmDiv.H"
#include "fvmLaplacian.H"
#include "fvmSup.H"

#include "fvcFlux.H"
#include "fvcDiv.H"
#include "fvcSnGrad.H"
#include "fvcAverage.H"

#include "simpleControl.H"

#include "update_values.hpp"
#include "field_factory.hpp"
#include "ethz.ipp"

int main(int argc, char *argv[])
{
  using namespace Foam;

  o2m::FieldFactory ff{o2m::setup_case(argc, argv)};

  auto& run_time = ff.time();
  auto& mesh = ff.mesh();

  #include "create_fields.inc"

  uniformDimensionedVectorField g(
    IOobject("g", run_time.constant(), mesh, IOobject::MUST_READ));

  simpleControl simple(mesh);

  #include "create_rain_fields.inc"
  #include "create_td_fields.inc"

  if (solveTD) {
    Info << nl << "Solving for the turbulent dispersion of raindrops" << endl;
  } else {
    Info << nl << "Turbulent dispersion of raindrops is neglected" << endl;
  }

  ethz::get_parameters(temp, rhoa, mua, rhop);

  Info << nl << "Temperature: " << temp.value() << " K" << endl;
  Info << "Air density: " << rhoa.value() << " kg/m3" << endl;
  Info << "Air dynamic viscosity: " << mua.value() << " kg/m-s" << endl;
  Info << "Water density: " << rhop.value() << " kg/m3" << endl;

  while (simple.loop()) {
    Info << nl << "Time = " << run_time.timeName() << nl;

    while (simple.correctNonOrthogonal()) {
      forAll(phases, phase_no) {
        // phi is used by the inletOutlet boundary condition and Courant
        // number calculation
        const surfaceScalarField phi(
          IOobject("phi", run_time.timeName(), mesh),
          phirain[phase_no]);

        #include "alpha_equations.inc"

        const dimensionedScalar dp("dp", dimLength, phases[phase_no].dp);
        const volScalarField magUrel(mag(U - Urain[phase_no]));

        Re = (magUrel*dp*rhoa)/mua;
        CdRe = ethz::get_CdRe(Re);
        CdRe.correctBoundaryConditions();

        if (solveTD) {
          tfl = 0.2*(k/epsilon);
          tp = (4*rhop*dp*dp)/(3*mua*CdRe);
          Ctrain[phase_no] = sqrt(tfl/(tfl+tp));
        }

        nutrain = nut*sqr(Ctrain[phase_no]);

        #include "velocity_equations.inc"
      }
    }

    if (run_time.write()) {
      /*
      forAll(phases, i) {
        Urain[i].write();
        alpharain[i].write();
        //Ctrain[phase_no].write();
      }
      */
    }

    Info << "ExecutionTime = " << run_time.elapsedCpuTime() << " s"
      << " ClockTime = " << run_time.elapsedClockTime() << " s" << nl << endl;
  }

  Info << "Writing final output" << nl << endl;
  forAll(phases, i) {
    Urain[i].write();
    alpharain[i].write();
    //Ctrain[phase_no].write();
  }

  #include "calculate_catch_ratio.inc"

  Info << "ExecutionTime = " << run_time.elapsedCpuTime() << " s"
    << "  ClockTime = " << run_time.elapsedClockTime() << " s" << nl << endl;

  Info << "End" << nl << endl;

  return 0;
}


// This file is a part of WindDrivenRain.
//
// WindDrivenRain is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// WindDrivenRain is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// WindDrivenRain. If not, see <http://www.gnu.org/licenses/>.

PtrList<volScalarField> Ctrain(phases.size());

forAll(phases, i) {
  const word name = ethz::phase_field_name("Ct", i + 1);
  Ctrain.set(i, &ff.make<scalar>(name, 0.0));
}

auto& k = ff.read<scalar>("k");
auto& epsilon = ff.read<scalar>("epsilon");
auto& nut = ff.read<scalar>("nut");

// Perform scaling only at t = 0
if (run_time.timeName() == "0") {
  k = k * pow(scalingFactor, 2);
  k.correctBoundaryConditions();
  epsilon = epsilon * pow(scalingFactor, 3);
  epsilon.correctBoundaryConditions();
  nut = nut * scalingFactor;
  nut.correctBoundaryConditions();
}

auto& nutrain =
    ff.make<scalar>("nutr", dimensionedScalar("nutr", dimViscosity, 0.0));
auto& tp = ff.make<scalar>("tp", dimensionedScalar("tp", dimTime, 0.0));
auto& tfl = ff.make<scalar>("tfl", dimensionedScalar("tfl", dimTime, 0.0));


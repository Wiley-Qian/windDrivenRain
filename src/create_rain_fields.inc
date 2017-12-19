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

PtrList<volVectorField> Urain(phases.size());
PtrList<surfaceScalarField> phirain(phases.size());
PtrList<volScalarField> alpharain(phases.size());
PtrList<volScalarField> scr(phases.size());

{
  word name;
  forAll(phases, i) {
    name = ethz::phase_field_name("U", i + 1);
    Urain.set(i, &ff.read<vector>(name, {.w = Foam::IOobject::AUTO_WRITE}));

    name = ethz::phase_field_name("phi", i + 1);
    phirain.set(i, fvc::flux(Urain[i]));

    name = ethz::phase_field_name("alpha", i + 1);
    alpharain.set(i, &ff.read<scalar>(name, {.w = Foam::IOobject::AUTO_WRITE}));
  }
}

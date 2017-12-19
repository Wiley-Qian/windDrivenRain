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

Info << nl << "Calculating specific catch ratio fields\n" << endl;

forAll(phases, i) {
  word name = ethz::phase_field_name("scr", i + 1);

  const surfaceScalarField normalvel(
      mag((mesh.Sf()/mesh.magSf()) & fvc::interpolate(Urain[i])));
  const surfaceScalarField surfaceScr(
    (normalvel*fvc::interpolate(alpharain[i]))*((3600*1E3)/(Rh*phases[i].fhd)));

  const surfaceScalarField::Boundary& patchSurfaceScr =
      surfaceScr.boundaryField();

  auto& scrtemp = ff.make<scalar>(name, 0.0);
  volScalarField::Boundary& scrtempb = scrtemp.boundaryFieldRef();

  forAll(scrtempb, patchi) { scrtempb[patchi] = patchSurfaceScr[patchi]; }

  scr.set(i, scrtemp);
  scr[i].write();
}

Info << nl << "Calculating catch ratio field\n" << endl;

auto& gcr = ff.make<scalar>("gcr", 0.0);

forAll(phases, i) { gcr += scr[i]*phases[i].fhd; }
gcr.write();

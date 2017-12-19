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

Info << "Reading transportProperties" << nl << endl;

auto& transport = ff.time().constant("transport");

const dimensionedScalar Rh("Rh", dimVelocity, transport);
const dimensionedScalar temp("temp", dimTemperature, transport);
dimensionedScalar rhoa("rhoa", dimDensity, 0.0);
dimensionedScalar rhop("rhop", dimDensity, 0.0);
dimensionedScalar mua("mua", dimMass / dimLength / dimTime, 0.0);

const scalar scalingFactor = transport.lookupOrDefault("scalingFactor", 1.0);

const bool solveTD = transport.lookupOrDefault("solveTD", false);

List<ethz::Phase> phases = ethz::read_phases(transport);

Info << "Reading field U" << nl << endl;
auto& U = ff.read<vector>("U");

// Perform scaling only at t=0
if (run_time.timeName() == "0") {
  Info << "Scaling velocity field" << nl << endl;
  U = U * scalingFactor;
  U.correctBoundaryConditions();
}

auto& Re = ff.make<scalar>("Re", 500, {.r = IOobject::READ_IF_PRESENT});
auto& CdRe = ff.make<scalar>("CdRe", 250, {.r = IOobject::READ_IF_PRESENT});


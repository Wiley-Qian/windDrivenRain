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

{
  auto& alphar = alpharain[phase_no];
  auto& phir = phirain[phase_no];

  fvScalarMatrix alphaEqn(
      fvm::ddt(alphar)
    + fvm::div(phir, alphar, "div(phir,alphar)")
    // added for bounded div scheme
    + fvm::Sp(fvc::div(phir), alphar));

  alphaEqn.relax();
  alphaEqn.solve();
}

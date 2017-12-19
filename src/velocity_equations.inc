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
  auto& Ur = Urain[phase_no];
  auto& alphar = alpharain[phase_no];
  auto& Ctr = Ctrain[phase_no];
  auto& phir = phirain[phase_no];

  volTensorField Rca(-nutrain*(T(fvc::grad(Ur))));
  Rca = Rca + (2.0/3.0)*sqr(Ctr)*I*k - (2.0/3.0)*I*tr(Rca);
  Rca.correctBoundaryConditions();

  const scalar eps = gMax(alphar)*0.001;

  const surfaceScalarField phiRa(
      -fvc::interpolate(nutrain)*mesh.magSf()
       *fvc::snGrad(alphar)/fvc::interpolate(alphar + eps));

  Foam::fvVectorMatrix UrainEqn(
      fvm::ddt(Ur)
    + fvm::div(phir, Ur, "div(phir,Ur)")

    - fvm::laplacian(nutrain, Ur, "laplacian(nutr,Ur)")
    + fvc::div(Rca)

    + fvm::div(phiRa, Ur, "div(phir,Ur)")
    + (fvc::grad(alphar)/(fvc::average(alphar) + eps) & Rca)
    ==
      g
    + ((3*mua*CdRe)/(4*rhop*dp*dp))*U
    - fvm::Sp(((3*mua*CdRe)/(4*rhop*dp*dp)), Ur));

  UrainEqn.relax();
  UrainEqn.solve();

  phir = Foam::fvc::flux(Ur);
}

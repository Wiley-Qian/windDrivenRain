// Copyright (C) 2017 Alexey Matveichev
//
// This file is a part of FEMME: Field opEration and Manipuliation Made Easy
//
// FEMME is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// FEMME is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FEMME.  If not, see <http://www.gnu.org/licenses/>.

#ifndef FIELD_FACTORY_HPP_
#define FIELD_FACTORY_HPP_

#include "timex.hpp"

#include "HashPtrTable.H"
#include "argList.H"
#include "fvMesh.H"
#include "fvPatchFieldsFwd.H"
#include "volFieldsFwd.H"

namespace Foam {
class word;
template <typename T>
class dimensioned;
}  // namespace Foam

namespace o2m {
struct IOoptions {
  Foam::IOobject::readOption r = Foam::IOobject::NO_READ;
  Foam::IOobject::writeOption w = Foam::IOobject::NO_WRITE;
  bool reg = true;
};

class FieldFactory {
  TimeX time_;
  Foam::fvMesh mesh_;

  Foam::HashPtrTable<Foam::volScalarField, Foam::word> scalar_fields_;
  Foam::HashPtrTable<Foam::volVectorField, Foam::word> vector_fields_;
  Foam::HashPtrTable<Foam::volTensorField, Foam::word> tensor_fields_;

  template <typename T>
  Foam::HashPtrTable<Foam::GeometricField<T, Foam::fvPatchField, Foam::volMesh>,
                     Foam::word>&
  find_target();

 public:
  FieldFactory(const Foam::argList&);

  virtual ~FieldFactory();

  Foam::fvMesh& mesh() { return mesh_; }
  const Foam::fvMesh& mesh() const { return mesh_; }

  TimeX& time() { return time_; }
  const TimeX& time() const { return time_; }

  template <typename T>
  Foam::GeometricField<T, Foam::fvPatchField, Foam::volMesh>& make(
      const Foam::word&, const Foam::dimensioned<T>&,
      const IOoptions& = {Foam::IOobject::NO_READ, Foam::IOobject::NO_WRITE,
                          true},
      const Foam::word& = "calculated");

  template <typename T>
  Foam::GeometricField<T, Foam::fvPatchField, Foam::volMesh>& read(
      const Foam::word&, const IOoptions& = {.r = Foam::IOobject::MUST_READ});
};

inline Foam::argList setup_case(int& argc, char**& argv) {
  Foam::argList::noBanner();

  return Foam::argList{argc, argv};
}
}  // namespace o2m

#include "field_factory.ipp"

#endif  // FIELD_FACTORY_HPP_

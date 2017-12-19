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

#include "field_factory.hpp"

namespace o2m {
FieldFactory::FieldFactory(const Foam::argList& args)
    : time_(args),
      mesh_(Foam::IOobject(Foam::fvMesh::defaultRegion, time_.timeName(), time_,
                           Foam::IOobject::MUST_READ)),
      scalar_fields_(),
      vector_fields_(),
      tensor_fields_() {}

FieldFactory::~FieldFactory() {}

template <>
Foam::HashPtrTable<
    Foam::GeometricField<Foam::scalar, Foam::fvPatchField, Foam::volMesh>,
    Foam::word>&
FieldFactory::find_target() {
  return scalar_fields_;
}

template <>
Foam::HashPtrTable<
    Foam::GeometricField<Foam::vector, Foam::fvPatchField, Foam::volMesh>,
    Foam::word>&
FieldFactory::find_target() {
  return vector_fields_;
}

template <>
Foam::HashPtrTable<
    Foam::GeometricField<Foam::tensor, Foam::fvPatchField, Foam::volMesh>,
    Foam::word>&
FieldFactory::find_target() {
  return tensor_fields_;
}

}  // namespace o2m

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

#ifndef FIELD_FACTORY_IPP_
#define FIELD_FACTORY_IPP_

#include "field_factory.hpp"

#include "volFields.H"

namespace o2m {
template <typename T>
Foam::GeometricField<T, Foam::fvPatchField, Foam::volMesh>& FieldFactory::read(
    const Foam::word& name, const IOoptions& opts) {
  typedef Foam::GeometricField<T, Foam::fvPatchField, Foam::volMesh> R;
  Foam::HashPtrTable<R, Foam::word>& tgt = find_target<T>();
  if (not tgt.found(name)) {
    tgt.set(name, new R(Foam::IOobject(name, time_.timeName(), mesh_, opts.r,
                                       opts.w, opts.reg),
                        mesh_));
  }

  return *tgt[name];
}

template <typename T>
Foam::GeometricField<T, Foam::fvPatchField, Foam::volMesh>& FieldFactory::make(
    const Foam::word& name, const Foam::dimensioned<T>& iv,
    const IOoptions& opts, const Foam::word& bc) {
  typedef Foam::GeometricField<T, Foam::fvPatchField, Foam::volMesh> R;
  Foam::HashPtrTable<R, Foam::word>& tgt = find_target<T>();
  tgt.set(name, new R(Foam::IOobject(name, time_.timeName(), mesh_, opts.r,
                                     opts.w, opts.reg),
                      mesh_, iv, bc));
  return *tgt[name];
}
}  // namespace o2m

#endif  // FIELD_FACTORY_IPP_

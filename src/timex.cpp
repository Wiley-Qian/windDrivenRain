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

#include "timex.hpp"

namespace o2m {

const Foam::word TimeX::constant_folder_name = "constant";
const Foam::word TimeX::system_folder_name = "system";

const auto read_if_present = Foam::IOobject::READ_IF_PRESENT;
const auto no_write = Foam::IOobject::NO_WRITE;

inline bool is_dictionary(Foam::IOobject& io) {
  return io.typeHeaderOk<Foam::dictionary>();
}

TimeX::TimeX(const Foam::argList& args)
    : Foam::Time{Foam::Time::controlDictName, args}, dicts_{} {
  // Not Found dictionary
  dicts_["not-found"] = new Foam::IOdictionary(
      Foam::IOobject("not-found", "constant", *this), Foam::dictionary::null);
}

TimeX::~TimeX() {}

Foam::dictionary& TimeX::constant(const Foam::word& name) {
  if (dicts_.found(name)) {
    dicts_[name]->regIOobject::read();
    return *dicts_[name];
  }
  // Checking name
  {
    Foam::IOobject io(name, constant(), *this, read_if_present, no_write,
                      false);
    if (is_dictionary(io)) {
      dicts_.set(name, new Foam::IOdictionary(io));
      return *dicts_[name];
    }
  }
  // Checking name + Properties variant
  {
    Foam::IOobject io(name + "Properties", constant(), *this, read_if_present,
                      no_write, false);
    if (is_dictionary(io)) {
      dicts_.set(name, new Foam::IOdictionary(io));
      return *dicts_[name];
    }
  }
  return *dicts_["not-found"];
}

Foam::dictionary& TimeX::system(const Foam::word& name) {
  if (dicts_.found(name)) {
    dicts_[name]->regIOobject::read();
    return *dicts_[name];
  }
  // Checking name
  {
    Foam::IOobject io(name, system(), *this, Foam::IOobject::READ_IF_PRESENT,
                      Foam::IOobject::NO_WRITE, false);
    if (is_dictionary(io)) {
      dicts_.set(name, new Foam::IOdictionary(io));
      return *dicts_[name];
    }
  }
  // Checking name + Dict variant
  {
    Foam::IOobject io(name + "Dict", system(), *this,
                      Foam::IOobject::READ_IF_PRESENT, Foam::IOobject::NO_WRITE,
                      false);
    if (is_dictionary(io)) {
      dicts_.set(name, new Foam::IOdictionary(io));
      return *dicts_[name];
    }
  }
  return *dicts_["not-found"];
}

}  // namespace o2m

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

#ifndef TIMEX_HPP_
#define TIMEX_HPP_

#include "Time.H"
#include "IOdictionary.H"
#include "HashPtrTable.H"

namespace o2m {
class TimeX : public Foam::Time {
  // Cache of dictionaries
  Foam::HashPtrTable<Foam::IOdictionary, Foam::word> dicts_;

 public:
  TimeX(const Foam::argList&);
  virtual ~TimeX();

  // Constants instead of hidden methods
  static const Foam::word constant_folder_name;
  static const Foam::word system_folder_name;

  // Hiding methods with no-arguments signatures
  auto constant() const { return TimePaths::constant(); }
  auto system() const { return TimePaths::system(); }

  // Simplified dictionary access
  Foam::dictionary& constant(const Foam::word&);
  Foam::dictionary& system(const Foam::word&);
};
}  // namespace o2m

#endif  // TIMEX_HPP_

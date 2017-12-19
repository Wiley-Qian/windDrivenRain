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

#ifndef ETHZ_IPP_
#define ETHZ_IPP_

namespace ethz {
inline Foam::word phase_field_name(Foam::word base, Foam::label idx) {
  Foam::OStringStream ss;
  ss << base << idx;
  return ss.str();
}

struct Phase {
  Foam::scalar dp = 0.0;
  Foam::scalar fhd = 0.0;
};

inline Foam::List<Phase> read_phases(Foam::dictionary& dict) {
  Foam::List<Phase> res;
  const Foam::List<Foam::dictionary> t(dict.lookup("phases"));
  forAll(t, i) {
    const auto& d = t[i];
    const auto dp = Foam::readScalar(d.lookup("dp"));
    const auto fhd = Foam::readScalar(d.lookup("fhd"));
    res.append({dp, fhd});
  }
  return res;
}

}  // namespace ethz

#endif  // ETHZ_IPP_

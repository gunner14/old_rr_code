// ordering.h

/*    Copyright 2009 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef BSON_ORDERING_H__
#define BSON_ORDERING_H__

#include "base/logging.h"
#include "bson/builder.h"

namespace bson {

/** A precomputation of a BSON key pattern.
The constructor is private to make conversion more explicit so we notice where we call make().
Over time we should push this up higher and higher.
*/
class Ordering { 
  const unsigned bits;
  const unsigned nkeys;
  Ordering(unsigned b,unsigned n) : bits(b),nkeys(n) { }
public:
  /** so, for key pattern { a : 1, b : -1 }
  get(0) == 1
  get(1) == -1
  */
  int get(int i) const { 
    return ((1 << i) & bits) ? -1 : 1;
  }

  // for woCompare...
  unsigned descending(unsigned mask) const { return bits & mask; }

  operator std::string () const {
    StringBuilder buf(32);
    for ( unsigned i=0; i<nkeys; i++)
      buf.append( get(i) > 0 ? "+" : "-" );
    return buf.str();
  }

  static Ordering make(const BSONObj& obj);
};

}
#endif


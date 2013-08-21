/* NOTE: Standalone bson header for when not using MongoDB.  
   See also: bsondemo.

   MongoDB includes db/jsobj.h instead. This file, however, pulls in much less code / dependencies.
*/

/** @file bson.h 
    BSON classes
*/

/*
 *    Copyright 2009 10gen Inc.
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

/**
   BSONObj and its helpers

   "BSON" stands for "binary JSON" -- ie a binary way to represent objects that would be
   represented in JSON (plus a few extensions useful for databases & other languages).

   http://www.bsonspec.org/
*/

#ifndef BSON_BSON_H__
#define BSON_BSON_H__

#include "bson/bsontypes.h"

class BSONElement;
class BSONObj;

#include "bson/bsonmisc.h"
#include "bson/ordering.h"
#include "bson/bsonelement.h"
#include "bson/bsonobj.h"
#include "bson/bsonobjiterator.h"
#include "bson/bsonobjbuilder.h"



#if 0
class BSONArrayBuilder;
class BSONObjBuilder;
class BSONObjBuilderValueStream;
class BSONObjIterator;
class Ordering;
class Record;
struct BSONArray; // empty subclass of BSONObj useful for overloading
struct BSONElementCmpWithoutField;

extern BSONObj maxKey;
extern BSONObj minKey;


#include "bson/oid.h"
#include "bson/bsonmisc.h"
#include "bson/bsonobjbuilder.h"
#include "bson/bsonobjiterator.h"
#include "bson/bsoninlines.h"
#endif

#endif

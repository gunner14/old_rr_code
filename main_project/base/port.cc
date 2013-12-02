// Copyright 2009 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "base/port.h"

#ifdef WIN32


#elif defined(__linux__) || defined(__unix__)

#include <ctype.h>
#include <algorithm>

char* strlwr(char* str) {
  char* p = str;
  while (*p != '\0') {
    *p = static_cast<char>(tolower(*p));
    ++p;
  }
  return str;
}

#endif

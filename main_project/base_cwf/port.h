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


// Some non-standard but very common methods have different names on different
// platform. Or sometimes, not all platform provides an implementation for a
// non-standard but widely used functions.
// According to the requirements of this application, this file solves this
// problem. It unifies different names on different platform, and provides
// custom implementation for missing functions.

#ifndef COMMON_PORT_H__
#define COMMON_PORT_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string.h>
#include "base/basictypes.h"

#ifdef WIN32

// Convert integer to a string base 10.
inline char* itoa(int val, char* dest) {
  return itoa(val, dest, 10);
}

// Absolute value for int64.
inline int64 llabs(int64 val) {
  return _abs64(val);
}

#elif defined(__linux__) || defined(__unix__)

// Convert string to lower case.
char* strlwr(char* str);

// case-insensitive version of strncmp method.
inline int strnicmp(const char* str1, const char* str2, int count) {
  return strncasecmp(str1, str2, count);
}

// case-insensitive version of strcmp method.
inline int stricmp(const char* str1, const char* str2) {
  return strcasecmp(str1, str2);
}

// Convert integer to a string base 10.
inline char* itoa(int val, char* dest) {
  sprintf(dest, "%d", val);
  return dest;
}

// Sleep for milliseconds.
inline void Sleep(int milliseconds) {
  timespec req, rem;
  req.tv_sec = milliseconds / 1000;
  req.tv_nsec = (milliseconds % 1000) * 1000000;
  nanosleep(&req, &rem);
}

#endif

#endif  // COMMON_PORT_H__

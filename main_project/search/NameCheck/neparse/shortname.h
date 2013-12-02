#include "abutility.h"
#include "abstring.h"
#include "name_util.h"
#include "AB_UTILITY_NAME.h"

#ifndef _SHORTNAME_H
#define _SHORTNAME_H

#define MAX_SIMI_VALUE 100
#define MIN_SIMI_VALUE 0

int short_name_simi(const char *szDictDir,
                    const ABU_NAME_S *pNameUnitLeft,
                    const ABU_NAME_S *pNameUnitRight);

#endif

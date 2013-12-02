#include "base/mkdirs.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>

namespace xce {
#ifdef POSIX
int mkdirs(const char *pathname, mode_t mode) {
  int retval = 0;
  #define FILENAME_MAX 300
  char path[FILENAME_MAX];

  strncpy(path, pathname, FILENAME_MAX - 1);
	path[FILENAME_MAX - 1] = 0;

  size_t path_len = strlen(path);
  while (path_len && '/' == path[path_len - 1])
    path[path_len - 1] = 0;

  while (0 != (retval = mkdir(path, mode))) {
    char subpath[FILENAME_MAX] = "", *delim;
    if (NULL == (delim = strrchr(path, '/')))
      return retval;
    strncat(subpath, path, delim - path);     // Appends NUL
    if (0 != mkdirs(subpath, mode))
      break;
  }
  return retval;
}

#else

int mkdirs(const char *pathname, int mode) {
  return 0;
}

#endif

}

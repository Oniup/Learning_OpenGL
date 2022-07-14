#include "app/utils.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>

namespace utils
{

  const char* GetPathDirectory(const char* path)
  {
    int length = strlen(path);
    if (length < 1)
      return nullptr;

    // finding the length of the directory string
    int i = length - 1;
    for (; i > -1; i--)
    {
      if (path[i] == '/' || path[i] == '\\')
        break;
    }  

    if (i < 1)
      return nullptr;

    char* directory = new char[i];
    memcpy(directory, path, i);
    directory[i] = '\0';

    return (const char*)directory;
  }

}
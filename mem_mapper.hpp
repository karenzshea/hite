#include <iostream>
#include "constants.hpp"
extern "C" {
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
};

namespace hite
{
    // memory maps file on construction
    // unmaps on destruction
    // prohibits copying
    struct MemMap {
        char * map = nullptr;
        struct stat file_stat = {0};
        int fd;

        // ctors
        MemMap() = default;
        MemMap(const std::string &filepath);
        // dtor
        ~MemMap();
        // move constructors
        MemMap(MemMap&&);
        MemMap& operator=(MemMap&&);
        // disallow copying
        MemMap(const MemMap&) = delete;
        MemMap& operator=(const MemMap&) = delete;
    };
}

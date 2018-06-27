#include "mem_mapper.hpp"

namespace hite
{
MemMap::MemMap(const std::string &filepath) {
    fd = open(filepath.c_str(), O_RDWR, (mode_t)0222);
    if (fd == -1)
    {
        throw std::runtime_error("Couldn't open hgt file!");
    }
    if (fstat(fd, &file_stat) == -1)
    {
        throw std::runtime_error("Couldn't get size of hgt file!");
    }
    if (file_stat.st_size == 0 || file_stat.st_size != HGT_FILE_SIZE)
    {
        throw std::runtime_error("Hgt file is empty or the wrong size!");
    }
    map = static_cast<char *>(mmap(0, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    // for debug
    std::cout << "map pointer: " << (unsigned long)map << std::endl;
    if (map == MAP_FAILED)
    {
        throw std::runtime_error("Error mmapping file!");
    }
}
MemMap::~MemMap() {
    if (map)
    {
        // for debug
        std::cout << "map ptr to be destructed " << (unsigned long)map << std::endl;
        if (munmap(map, file_stat.st_size) == -1)
        {
            std::cerr << "munmap failed with " << ::strerror(errno) << std::endl;
        }
    }
    ::close(fd);
}

// move functions
MemMap::MemMap(MemMap&& other) : map{other.map} {
    // how does self-assignment get handled here?
    other.map = nullptr;
}
MemMap& MemMap::operator=(MemMap&& other) {
    // is there a better way to avoid self assignment?
    if (this != &other)
    {
        fd = other.fd;
        other.fd = -1;

        map = other.map;
        other.map = nullptr;

        file_stat = std::move(other.file_stat);
    }
    return *this;
}
}

#include "ge/util/filemap.hpp"

#include "ge/util/paths.hpp"

#include "ge/error.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

namespace fs = std::filesystem;

namespace ge {

ReadOnlyFileMap::ReadOnlyFileMap(const fs::path& filePath)
{
    map(filePath);
}

ReadOnlyFileMap::~ReadOnlyFileMap()
{
    unmap();
}

void ReadOnlyFileMap::map(const fs::path& filePath)
{
    if (!fs::is_regular_file(filePath)) {
        throw Exception{} << "ReadOnlyFileMap: no such file: " << filePath;
    }

    unmap();

    int fd = open(filePath.c_str(), O_RDONLY);
    GE_ASSERT(fd != -1);

    struct stat sb;
    int result = fstat(fd, &sb);
    GE_ASSERT(result != -1);
    _length = sb.st_size;

    _addr = mmap(nullptr, _length, PROT_READ, MAP_PRIVATE, fd, 0);
    GE_ASSERT(_addr != MAP_FAILED);

    close(fd);
}

void ReadOnlyFileMap::unmap()
{
    if (_addr) {
        munmap(_addr, _length);
    }
}

std::span<const std::byte> ReadOnlyFileMap::content() const
{
    return {static_cast<const std::byte*>(_addr), _length};
}

} // namespace ge

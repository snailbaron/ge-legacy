#include "ge/util/filemap.hpp"
#include "ge/error.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace ge {

ReadOnlyFileMap::ReadOnlyFileMap(const std::filesystem::path& filePath)
{
    map(filePath);
}

ReadOnlyFileMap::~ReadOnlyFileMap()
{
    unmap();
}

void map(const std::filesystem::path& filePath)
{
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

void unmap()
{
    if (_addr) {
        munmap(_addr, _length);
    }
}

} // namespace ge

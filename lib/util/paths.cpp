#include "ge/util/paths.hpp"

#include <iostream>

namespace fs = std::filesystem;

namespace ge {

fs::path currentExecutablePath()
{
#ifdef __linux__
    return fs::read_symlink("/proc/self/exe");
#endif
}

fs::path currentExecutableDirectory()
{
    return currentExecutablePath().parent_path();
}

} // namespace ge

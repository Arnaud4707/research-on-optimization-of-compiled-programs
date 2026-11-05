#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <string_view>

void process_file(const std::filesystem::path& path, const std::string& pattern)
{
    int fd = open(path.c_str(), O_RDONLY);

    if (fd == -1)
        return;

    struct stat st;

    if (fstat(fd, &st) == -1)
    {
        close(fd);
        return;
    }

    if (st.st_size == 0)
    {
        close(fd);
        return;
    }

    std::size_t size = static_cast<std::size_t>(st.st_size);

    void* mapping = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);

    close(fd);

    if (mapping == MAP_FAILED)
        return;

    const char* content = static_cast<const char*>(mapping);

    if (std::memchr(content, '\0', size) != nullptr)
    {
        if (std::search(content, content + size,
                        pattern.begin(), pattern.end()) != content + size)
        {
            std::cout << "custom_grep: " << path
                      << ": binary file matches\n";
        }

        munmap(mapping, size);
        return;
    }

    const char* end = content + size;
    const char* line_start = content;
    int count_line = 0;

    while (line_start < end)
    {
        const char* line_end = static_cast<const char*>(
            std::memchr(line_start, '\n', end - line_start));

        if (!line_end)
            line_end = end;

        ++count_line;

        std::string_view line(line_start, line_end - line_start);

        std::size_t found = line.find(pattern);

        if (found != std::string_view::npos)
        {
            std::cout << "\033[38;2;118;38;113m" << path << "\033[0m"
                      << "\033[0;32m:" << count_line << ":\033[0m"
                      << line.substr(0, found)
                      << "\033[0;31m" << line.substr(found, pattern.length()) << "\033[0m"
                      << line.substr(found + pattern.length())
                      << std::endl;
        }

        line_start = line_end + (line_end < end);
    }

    munmap(mapping, size);
}

void rdir(const std::filesystem::path& path, const std::string& pattern)
{
    for (const auto& dir_entry : std::filesystem::directory_iterator{path})
    {
        if (dir_entry.is_directory())
        {
            if (dir_entry.path().filename() == ".git")
                continue;

            rdir(dir_entry.path(), pattern);
        }
        else if (dir_entry.is_regular_file())
        {
            process_file(dir_entry.path(), pattern);
        }
    }
}

int main(int ar, char** av)
{
    std::string path;
    std::string opt;
    std::string pattern;

    int n_opt = 0;
    if (ar < 2)
        return (0);
    if (av[1][0] == '-' && std::strlen(av[1]) >= 2)
    {
        n_opt++;
        opt = av[1];
    }
    if (ar < 3 + n_opt)
        path = ".";
    else
        path = av[2 + n_opt];
    
    pattern = av[1 + n_opt];
    
    try {
        if (n_opt > 0 && opt[1] == 'r')
            rdir(path, pattern);
        else
            std::cout << "Only -r option.\n";
    } catch (std::exception& e) {
        std::cout << "fatal error: " << e.what() << std::endl;
    }
    
    return (0);
}
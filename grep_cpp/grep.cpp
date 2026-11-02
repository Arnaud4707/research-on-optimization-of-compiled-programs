#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <string.h>

void find_pattern(std::string path, std::string pattern)
{
    std::ifstream file;
    file.open(path);
    std::string line;
    int count_line = 0;
    while (std::getline(file, line)){
        count_line++;
        std::size_t found = line.find(pattern);
        if (found!=std::string::npos)
            std::cout << "\033[38;2;118;38;113m" << path << "\033[0m"
        << "\033[0;32m:" << count_line << ":\033[0m"
        << line.substr(0, found)
        << "\033[0;31m" << line.substr(found, pattern.length()) << "\033[0m"
        << line.substr(found + pattern.length()) << std::endl;
    }
    file.close();
}

bool binary_file_contains_pattern(const std::filesystem::path& path, const std::string& pattern)
{
    std::ifstream file(path, std::ios::binary);

    if (!file || pattern.empty())
        return false;

    constexpr std::size_t BLOCK_SIZE = 8192;

    char buffer[BLOCK_SIZE];
    std::string overlap;

    while (file.read(buffer, BLOCK_SIZE) || file.gcount() > 0)
    {
        std::string block = overlap;
        block.append(buffer, file.gcount());

        if (block.find(pattern) != std::string::npos)
            return true;

        // Conserver les derniers caractères pour le prochain bloc.
        const std::size_t keep = std::min(
            pattern.size() - 1,
            block.size()
        );

        overlap = block.substr(block.size() - keep);
    }

    return false;
}

bool is_binary_file(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);

    if (!file)
        return false;

    constexpr long int BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];
    
    file.read(buffer, BUFFER_SIZE);
    const auto count = file.gcount();

    for (std::streamsize i = 0; i < count && i < BUFFER_SIZE; ++i)
    {
        if (buffer[i] == '\0')
            return true;
    }
    
    return false;
}

void rdir(std::string path, std::string pattern)
{
    const std::filesystem::path sandbox{path};

    for (auto const& dir_entry : std::filesystem::directory_iterator{sandbox}) {
        if (dir_entry.is_directory()){
            if (dir_entry.path().filename() == ".git")
                continue;
            rdir(dir_entry.path(), pattern);
        }
        else if (!dir_entry.is_regular_file()){
            continue;
        }
        else if (is_binary_file(dir_entry.path())){
            if (binary_file_contains_pattern(dir_entry.path(), pattern)){
                std::cout << "custom_grep: " << dir_entry.path() << ": binary file matches" << std::endl;
                    continue;
            }
        }
        find_pattern(dir_entry.path(), pattern);
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
    if (av[1][0] == '-' && strlen(av[1]) >= 2)
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
        {
            const std::filesystem::path sandbox{path};
            for (auto const& dir_entry : std::filesystem::directory_iterator{sandbox})
            {
                if (is_binary_file(dir_entry.path()))
                    continue;
                find_pattern(dir_entry.path(), pattern);
            }
        };
    } catch (std::exception& e) {
        std::cout << "fatal error: " << e.what() << std::endl;
    }
    
    return (0);
}
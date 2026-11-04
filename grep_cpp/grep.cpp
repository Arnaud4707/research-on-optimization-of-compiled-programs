#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void process_file(const std::filesystem::path& path, const std::string& pattern)
{
    std::ifstream file(path, std::ios::binary);

    if (!file)
        return;

    constexpr std::size_t BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];

    std::string content;

    while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0)
        content.append(buffer, file.gcount());

    if (content.find('\0') != std::string::npos)
    {
        if (content.find(pattern) != std::string::npos)
            std::cout << "custom_grep: " << path << ": binary file matches\n";
        return;
    }

    std::istringstream stream(content);
    std::string line;
    int count_line = 0;

    while (std::getline(stream, line))
    {
        ++count_line;

        std::size_t found = line.find(pattern);

        if (found != std::string::npos)
        {
            std::cout << "\033[38;2;118;38;113m" << path << "\033[0m"
                << "\033[0;32m:" << count_line << ":\033[0m"
                << line.substr(0, found)
                << "\033[0;31m" << line.substr(found, pattern.length()) << "\033[0m"
                << line.substr(found + pattern.length())
                << std::endl;
        }
    }
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
            std::cout << "Only -r option." << std::endl;
    } catch (std::exception& e) {
        std::cout << "fatal error: " << e.what() << std::endl;
    }
    
    return (0);
}
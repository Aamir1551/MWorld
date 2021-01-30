#include <fstream>
#include <ostream>

#include <util_functions.h>

int util_functions::read_from_file(std::string file_name, std::string &contents)
{
    std::ifstream file;
    file.open(file_name);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            contents.append(line + "\n");
        }
        return 2;
    }
    else
    {
        return 0;
    };
};

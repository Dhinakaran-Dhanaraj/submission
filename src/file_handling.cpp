#include "file_handling.h"

#include <fstream>
#include <iostream>

bool write_to_file(const std::string &filepath, const std::vector<uint8_t> &contents)
{
    std::ofstream os{filepath, std::ios::binary};

    if (!os)
    {
        std::cerr << "Error opening " << filepath << std::endl;
        return true;
    }

    if (!os.write(reinterpret_cast<const char *>(contents.data()), contents.size()))
    {
        std::cerr << "Error writing to file " << filepath << std::endl;
        return true;
    }

    os.close();

    return false;
}

bool read_from_file(const std::string &filepath, std::vector<uint8_t> &contents)
{
    std::ifstream is{filepath, std::ios::binary};

    if (!is)
    {
        std::cerr << "Error opening " << filepath << std::endl;
        return true;
    }

    is.seekg(0, std::ios::end);
    std::streamsize file_sz = is.tellg();
    is.seekg(0, std::ios::beg);
    contents.resize(file_sz);

    if( 0 < contents.size() ) {
        if (!is.read(reinterpret_cast<char *>(contents.data()), file_sz))
        {
            std::cerr << "Error reading from " << filepath << std::endl;
            contents.clear();
            return true;
        }
    }

    is.close();

    return false;
}

std::vector<uint8_t> string_to_vector(const std::string &str)
{
    const uint8_t *data = reinterpret_cast<const uint8_t *>(str.data());
    std::vector<uint8_t> vecData(data, data + str.size());
    return vecData;
}
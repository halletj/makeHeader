#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <tuple>
#include <stdexcept>
#include <filesystem>

std::vector<unsigned char> readBytes(const std::string& filename) {
    std::vector<unsigned char> bytes;

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    // Get the file size
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize <= 0) {
        throw std::runtime_error("Failed to get file size");
    }

    // Allocate space
    bytes.resize(fileSize);

    // Read the file
    file.read(reinterpret_cast<char*>(bytes.data()), fileSize);

    return bytes;
}

void writeHeader(const std::string& name, const std::string& filename, const std::vector<unsigned char>& bytes) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    file << "#pragma once\n";
    file << "#include <cstddef>\n";
    file << "const unsigned char " << name << "_data[] = {\n";
    for (size_t i = 0; i < bytes.size(); ++i) {
        file << "0x" << std::hex << static_cast<int>(bytes[i]);
        if (i != bytes.size() - 1) {
            file << ",";
        }
        if ( (i + 1) % 16 == 0) {
            file << "\n";
        }
    }
    file << "\n};\n";
    file << std::dec;
    file << "const size_t " << name << "_size = " << bytes.size() << ";\n";
}

bool isNewerTimestamp(const std::string& input, const std::string& output) {
    if (!std::filesystem::exists(input) || !std::filesystem::exists(output)) {
        return true;
    }
    const auto inputTime = std::filesystem::last_write_time(input);
    const auto outputTime = std::filesystem::last_write_time(output);
    return inputTime > outputTime;
}

std::tuple<std::string, std::string, std::string> processArgs(int argc, char* argv[]) {
    if (argc != 4) {
        throw std::runtime_error("Usage: makeHeader <name> <input> <output>");
    }

    return {argv[1], argv[2], argv[3]};
}

int main(int argc, char* argv[]) {
    try {

        auto [name, input, output] = processArgs(argc, argv);
        if (!isNewerTimestamp(input, output)) {
            std::cout << "Skipping " << output << " because it is up to date\n";
            return EXIT_SUCCESS;
        }
        auto bytes = readBytes(input);
        writeHeader(name, output, bytes);
        std::cout << "Updated " << bytes.size() << " bytes, " << output << "\n";
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;  
}
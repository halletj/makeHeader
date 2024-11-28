//----------------------------------------------------------------------

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <tuple>
#include <stdexcept>
#include <filesystem>

//----------------------------------------------------------------------
class success_exception : public std::runtime_error {
public:
    success_exception(const std::string& message) : std::runtime_error(message) {}
};

//----------------------------------------------------------------------

std::vector<unsigned char> readBytes(const std::string& filename) {
    std::vector<unsigned char> bytes;

    // Open the file
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    // Get the file size
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Check for errors
    if (fileSize <= 0) {
        throw std::runtime_error("Failed to get file size");
    }

    // Allocate space
    bytes.resize(fileSize);

    // Read the file
    file.read(reinterpret_cast<char*>(bytes.data()), fileSize);

    return bytes;
}

//----------------------------------------------------------------------

void writeHeader(const std::string& name, const std::string& filename, const std::vector<unsigned char>& bytes) {
    // Open the file
    std::ofstream file(filename, std::ios::trunc);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    // Write the header
    file << "#pragma once\n";
    file << "#include <cstddef>\n";
    // Write the data
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
    // Write the size
    file << std::dec;
    file << "const size_t " << name << "_size = " << bytes.size() << ";\n";
}

//----------------------------------------------------------------------

void checkTimestamps(const std::string& input, const std::string& output) {
    // If either file doesn't exist, we need to update
    if (!std::filesystem::exists(input) || !std::filesystem::exists(output)) {
        return;
    }

    // If the input file is newer than the output file, we need to update
    const auto inputTime = std::filesystem::last_write_time(input);
    const auto outputTime = std::filesystem::last_write_time(output);
    
    if (inputTime > outputTime) {
        return;
    }

    // If the output file is newer than the input file, we don't need to update
    throw success_exception("Skipping " + output + " because it is up to date");
}

//----------------------------------------------------------------------

std::tuple<std::string, std::string, std::string> processArgs(int argc, char* argv[]) {
    // Check for the right number of arguments
    if (argc != 4) {
        throw std::runtime_error("Usage: makeHeader <name> <input> <output>");
    }

    // Return the arguments
    return {argv[1], argv[2], argv[3]};
}

//----------------------------------------------------------------------

int main(int argc, char* argv[]) {
    try {
        // Process the arguments
        const auto [name, input, output] = processArgs(argc, argv);
        // Check if we need to update
        checkTimestamps(input, output);
        // Read the bytes
        const auto bytes = readBytes(input);
        // Write the header
        writeHeader(name, output, bytes);        
        throw success_exception("Updated " + std::to_string(bytes.size()) + " bytes, " + output);
    }
    catch(const success_exception& e) {
        std::cout << e.what() << '\n';
        return EXIT_SUCCESS;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;  
}
//
// Created by Syl on 16/08/2024.
//
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cassert>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <fmt/core.h>

constexpr static auto headerStart =
    "// This file is auto generated!\n"
    "#ifndef BINARYDATA_H\n"
    "#define BINARYDATA_H\n"
    "#include <vector>\n"
    "#include <string>\n"
    "namespace binary_data {\n"
    "    struct BinaryResource {\n"
    "        std::string originalFilename;\n"
    "        std::vector<char> data;\n"
    "    };\n";
constexpr static auto headerEnd =
    "}\n"
    "#endif";

constexpr static auto sourceEnd =
    "}\n";
void addFile(const std::filesystem::path& path, std::ofstream& headerStream, std::ofstream& sourceStream) {
    const auto nameWithoutExtension = path.stem().string();
    headerStream << "    extern BinaryResource " << nameWithoutExtension << ";\n";

    sourceStream << "    BinaryResource " << nameWithoutExtension << " {\n";
    sourceStream << "        .originalFilename = " << path.filename() << ",\n";
    // Now - load *all* the data from the file as a char[]..
    std::ifstream inStream{ path, std::ios::in | std::ios::binary };
    inStream.seekg(0, std::ios::end);
    const auto len = inStream.tellg();
    inStream.seekg(0, std::ios::beg);
    std::vector<char> data(len);
    inStream.read(data.data(), len);
    sourceStream << "        .data = {\n";
    sourceStream << "            ";
    for (size_t i = 0; i < data.size(); ++i) {
        sourceStream << "0x" << std::hex << ((int)data[i] & 0xFF);
        if (i != data.size() - 1) {
            sourceStream << ", ";
        }
    }
    sourceStream << "\n"
                    "        }\n";
    sourceStream
        << "    };\n";
}

int main(int argc, char** argv) {
    /*
     * argv[0] - exe name
     * argv[1] - dest path
     * argv[2] - header name
     * argv[3+] - sources
     */
    if (argc < 4) {
        std::cout << "Usage:\n"
                     "./serialise [dest] [header] [sources...]";
        return -1;
    }
    // clang-format off
    const std::filesystem::path destPath{ argv[1] };
    const std::string headerNameStem{ argv[2] };
    // clang-format on
    const std::string headerName{ fmt::format("{}/{}.h", destPath.string(), headerNameStem) };
    size_t currentSourceIndex{ 0 };
    std::ofstream headerStream{ headerName, std::ios::out };
    headerStream << headerStart;
    std::stringstream sourceStartStream;
    sourceStartStream << "#include \"" << headerNameStem << ".h\"\n";
    sourceStartStream << "namespace binary_data {\n";
    const auto sourceStart = sourceStartStream.str();
    for (auto i = 3; i < argc; ++i) {
        std::filesystem::path asPath{ argv[i] };
        std::ofstream currentSourceStream{ fmt::format("{}/{}.cpp", destPath.string(), asPath.stem().string()) };
        currentSourceStream << sourceStart;
        addFile(asPath, headerStream, currentSourceStream);
        currentSourceStream << sourceEnd;
        currentSourceStream.flush();
    }

    headerStream << headerEnd;
    headerStream.flush();
    return 0;
}
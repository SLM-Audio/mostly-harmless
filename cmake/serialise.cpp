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
    "    };\n"
    "    [[nodiscard]] BinaryResource* getNamedResource(const std::string& name);\n";

constexpr static auto headerEnd =
    "}\n"
    "#endif";

constexpr static auto sourceEnd =
    "}\n";

constexpr static auto mainSourceEnd =
    "\n    };\n"
    "    BinaryResource* getNamedResource(const std::string& name) {\n"
    "        const auto it = s_resourceMap.find(name);\n"
    "        if(it == s_resourceMap.end()) return nullptr;\n"
    "        return s_resourceMap[name];\n"
    "    }\n"
    "}";

void addFile(const std::filesystem::path& path, std::ofstream& headerStream, std::ofstream& sourceStream, std::ofstream& mainSourceStream) {
    const auto nameWithoutExtension = path.stem().string();
    // Need the extension too..
    const auto extension = path.extension().string().substr(1);
    headerStream << "    extern BinaryResource " << nameWithoutExtension << "_" << extension << ";\n";
    mainSourceStream << "        { \"" << path.filename().string() << "\", &" << nameWithoutExtension << "_" << extension << " }";
    sourceStream << "    BinaryResource " << nameWithoutExtension << "_" << extension << " {\n";
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
    std::ofstream mainSourceStream{ fmt::format("{}/{}.cpp", destPath.string(), headerNameStem), std::ios::out };
    mainSourceStream << "#include \"" << headerNameStem << ".h\"\n";
    mainSourceStream << "#include <unordered_map>\n";
    mainSourceStream << "namespace binary_data {\n";
    mainSourceStream << "    static std::unordered_map<std::string, BinaryResource*> s_resourceMap = {\n";

    for (auto i = 3; i < argc; ++i) {
        std::filesystem::path asPath{ argv[i] };
        const auto extension = asPath.extension().string().substr(1);
        std::ofstream currentSourceStream{ fmt::format("{}/{}_{}.cpp", destPath.string(), asPath.stem().string(), extension) };
        currentSourceStream << sourceStart;
        addFile(asPath, headerStream, currentSourceStream, mainSourceStream);
        if (i != argc - 1) {
            mainSourceStream << ",\n";
        }
        currentSourceStream << sourceEnd;
        currentSourceStream.flush();
    }

    headerStream << headerEnd;
    headerStream.flush();
    mainSourceStream << mainSourceEnd;
    mainSourceStream.flush();
    return 0;
}
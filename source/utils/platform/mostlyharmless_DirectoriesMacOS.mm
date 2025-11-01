#include <mostly_harmless/utils/mostlyharmless_Directories.h>
#include <Foundation/Foundation.h>

namespace mostly_harmless::utils::directories {
    std::optional<std::filesystem::path> getDirectory(DirectoryType type) {
        const char* homeDirectory = NSHomeDirectory().UTF8String;
        std::filesystem::path homePath{ homeDirectory };
        switch (type) {
            case DirectoryType::Desktop: {
                return homePath / "Desktop";
            }
            case DirectoryType::Documents: {
                return homePath / "Documents";
            }
            case DirectoryType::UserAppData: {
                return homePath / "Library" / "Application Support";
            }
            case DirectoryType::Temp: {
                const char* tempDir = NSTemporaryDirectory().UTF8String;
                return tempDir;
            }
            default: return {};
        }
    }
} // namespace mostly_harmless::utils::directories

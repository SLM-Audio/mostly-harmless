//
// Created by Syl on 08/02/2025.
//
#include <mostly_harmless/utils/mostlyharmless_Directories.h>
#include <mostly_harmless/utils/mostlyharmless_OnScopeExit.h>
#include <mostly_harmless/utils/mostlyharmless_Macros.h>
#include <ShlObj_core.h>
namespace mostly_harmless::utils::directories {
    std::optional<std::filesystem::path> getDirectory(DirectoryType type) {
        PWCHAR pathRes{ nullptr };
        KNOWNFOLDERID id;
        switch(type) {
            case DirectoryType::Desktop: {
                id = FOLDERID_Desktop;
                break;
            }
            case DirectoryType::Documents: {
                id = FOLDERID_Documents;
                break;
            }
            case DirectoryType::UserAppData: [[fallthrough]];
            case DirectoryType::Temp: {
                id = FOLDERID_LocalAppData;
                break;
            }
            default: {
                return {};
            }
        }

        const auto folderRes = SHGetKnownFolderPath(id, KF_FLAG_DEFAULT, NULL, &pathRes);
        OnScopeExit se {
            [pathRes]() -> void {
              if(pathRes) {
                  CoTaskMemFree(pathRes);
              }
            }
        };
        if(folderRes == E_FAIL || folderRes == E_INVALIDARG || !pathRes) {
           return {};
        }
        std::filesystem::path dirPath{ pathRes };
        if(type == DirectoryType::UserAppData) {
            auto res = dirPath.parent_path();
            return res;
        }
        else if(type == DirectoryType::Temp) {
            auto res = dirPath / "Temp";
            return res;
        }
        return dirPath;
    }
}
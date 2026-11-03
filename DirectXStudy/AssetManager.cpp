#include "AssetManager.h"
#include <Windows.h>
#include <objbase.h>
#include <cstdio>
#include <stdexcept>
#include <filesystem>
#include "Datas.h"
#include <fstream>

NLOHMANN_JSON_SERIALIZE_ENUM(AssetType, {
    {AssetType::Unknown,  "Unknown"},
    {AssetType::ModelAssetType,    "Model"},
    {AssetType::TextureAssetType,  "Texture"},
    {AssetType::MaterialAssetType, "Material"},
    //{AssetType::Shader,   "Shader"},
    {AssetType::FolderAssetType,   "Folder"}
    })

AssetManager::AssetManager()
{
    std::filesystem::path assetsFolder = "Assets";
    std::filesystem::path path = std::filesystem::current_path() / assetsFolder;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
    {
        const std::filesystem::path& entryPath = entry.path();
        std::string extension = entryPath.extension().string();
        if (extension == ".meta")
            continue; // Skip .meta files
        std::string fileName = entryPath.string();
        const std::filesystem::path metaPath = fileName+".meta";
        if (std::filesystem::exists(metaPath))
        {
            LoadMetaDataFromFile(metaPath.string());
        }
        else
        {
            std::transform(extension.begin(), extension.end(), extension.begin(),
                [](unsigned char c) {
                    return static_cast<char>(std::tolower(c));
                });
            CreateMetaData(fileName, GetAssetTypeFromExtension(extension));
        }
	}
}

MetaData* AssetManager::GetMetaData(const std::string& guid)
{
    auto it = metaDataMap.find(guid);

    if (it == metaDataMap.end())
        return nullptr;

    return &it->second;
}

std::string AssetManager::GenerateGUID()
{
    GUID guid;

    if (FAILED(CoCreateGuid(&guid)))
        throw std::runtime_error("Failed to generate GUID");

    char buffer[33];

    std::snprintf(
        buffer,
        sizeof(buffer),
        "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
        static_cast<unsigned int>(guid.Data1),
        static_cast<unsigned int>(guid.Data2),
        static_cast<unsigned int>(guid.Data3),
        static_cast<unsigned int>(guid.Data4[0]),
        static_cast<unsigned int>(guid.Data4[1]),
        static_cast<unsigned int>(guid.Data4[2]),
        static_cast<unsigned int>(guid.Data4[3]),
        static_cast<unsigned int>(guid.Data4[4]),
        static_cast<unsigned int>(guid.Data4[5]),
        static_cast<unsigned int>(guid.Data4[6]),
        static_cast<unsigned int>(guid.Data4[7])
    );

    return buffer;
}

void AssetManager::CreateMetaData(const std::string& assetPath, AssetType assetType)
{
    std::ofstream file(assetPath + ".meta");
    nlohmann::json jsonfile ;
    jsonfile = nlohmann::json{
    {"GUID", GenerateGUID()},
    {"Path", assetPath},
    {"AssetType", assetType},
    };
    file << jsonfile.dump(4);
    file.close();

	metaDataMap[assetPath] = { jsonfile["GUID"].get<std::string>(), assetType, assetPath };
}

AssetType AssetManager::GetAssetTypeFromExtension(const std::string& extension)
{
    if(extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".bmp")
        return AssetType::TextureAssetType;
    else if (extension == ".fbx" || extension == ".obj" || extension == ".dae")
        return AssetType::ModelAssetType;
    else if (extension == ".mat")
        return AssetType::MaterialAssetType;
    //else if (extension == ".hlsl" || extension == ".shader")
    //    return AssetType::Shader;
    else if (extension.empty())
		return AssetType::FolderAssetType;
}

void AssetManager::LoadMetaDataFromFile(const std::string& metaFilePath)
{   
    std::ifstream file(metaFilePath);
    nlohmann::json sceneJson;
    file >> sceneJson;

	std::string guid = sceneJson["GUID"].get<std::string>();
    metaDataMap[guid] = { guid, sceneJson["AssetType"].get<AssetType>(), sceneJson["Path"].get<std::string>() };
}

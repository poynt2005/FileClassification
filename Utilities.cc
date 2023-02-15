#include "FileClassificator.h"
#include <filesystem>
#include <iostream>
#include <regex>
namespace Utilities
{
    namespace fs = std::filesystem;

    void GetCurrentPath(std::string &out)
    {
        out = fs::current_path().string();
    }

    void CombinePath(const std::vector<std::string> &vec, std::string &out)
    {
        fs::path tempPath;
        for (auto &&p : vec)
        {
            tempPath /= p;
        }
        out = tempPath.string();
    }

    bool MoveFile(const std::string &src, const std::string &dst)
    {
        fs::path dstPath(dst);
        if (fs::exists(dstPath))
        {
            std::string dstFilename = dstPath.stem().string();
            const std::regex findFilenameOrderNumber("_[0-9]+$");
            if (std::regex_search(dstFilename, findFilenameOrderNumber))
            {
                const std::regex dilmiter("_");

                std::sregex_token_iterator iter(dstFilename.begin(), dstFilename.end(), dilmiter, -1);
                std::sregex_token_iterator iterEnd;
                std::vector<std::string> dstFilenameSplit = {iter, iterEnd};

                int currentNumber = std::stoi(dstFilenameSplit[dstFilenameSplit.size() - 1]);
                std::string newDstFilename = dstFilenameSplit[dstFilenameSplit.size() - 2] + "_" + std::to_string(currentNumber + 1) + dstPath.extension().string();
                dstPath = dstPath.parent_path() / newDstFilename;
            }
            else
            {
                dstPath = dstPath.parent_path() / (dstPath.stem().string() + "_1" + dstPath.extension().string());
            }
        }

        fs::rename(fs::path(src), dstPath);

        return fs::exists(dstPath);
    }

    void ListAllFile(const std::string &directory, std::vector<std::string> &allFiles)
    {
        for (auto &&entry : fs::directory_iterator(fs::path(directory)))
        {
            allFiles.emplace_back(entry.path().filename().string());
        }
    }

    bool Mkdir(const std::string &dirName)
    {
        if (!fs::is_directory(dirName))
        {
            return fs::create_directory(dirName);
        }

        return true;
    }

    void GetFileName(const std::string &inFilePath, std::string &outFilename)
    {
        outFilename = fs::path(inFilePath).filename().string();
    }

    void GetAbsolutePath(const std::string &relPath, std::string &absPath)
    {
        absPath = fs::absolute(relPath).string();
    }
}
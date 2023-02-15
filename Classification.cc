#include "FileClassificator.h"
#include <set>
#include <functional>
#include <iostream>
#include <thread>
#include <mutex>

namespace Classificator
{
    void SubJobSplit(const std::vector<std::string> &totalJob, std::vector<std::vector<std::string>> &outJob)
    {
        int subJobCount = totalJob.size() / std::thread::hardware_concurrency() + 1;

        for (int i = 0; i < totalJob.size(); i += subJobCount)
        {
            int maxSubJobCount = subJobCount;

            if (totalJob.size() - i < maxSubJobCount)
            {
                maxSubJobCount = totalJob.size() - i;
            }

            std::vector<std::string> tmpVec(totalJob.begin() + i, totalJob.begin() + i + maxSubJobCount);
            outJob.emplace_back(tmpVec);
        }
    }

    void ClassificateBase(const std::string &sourceFolder, const std::string &newFolder, std::vector<std::string> &movedFiles, const std::function<void(const ProgressInfo, const double, const std::string &)> &progresser)
    {

        progresser(ProgressInfo::STARTED, 0.0, "");

        std::set<std::string> fileHashSet;

        std::vector<std::string> allFiles;

        std::string sourceFolderAbsPath;
        Utilities::GetAbsolutePath(sourceFolder, sourceFolderAbsPath);

        std::string dstFolderAbsPath;
        Utilities::GetAbsolutePath(newFolder, dstFolderAbsPath);
        progresser(ProgressInfo::FILE_LISTED, 0.0, "");

        Utilities::ListAllFile(sourceFolderAbsPath, allFiles);

        Utilities::Mkdir(dstFolderAbsPath);
        progresser(ProgressInfo::DST_FOLDER_ESTABLISHED, 0.0, "");

        // std::vector<std::thread> threads;

        // std::vector<std::vector<std::string>> jobSplit;
        // SubJobSplit(allFiles, jobSplit);

        // for (int i = 0 ; i<jobSplit.size();i++)
        // {
        //     auto job = std::move(jobSplit[i]);
        //     std::mutex jobMutex;
        //     std::vector<std::string> processingJob;
        //     std::vector<std::string> movedJob;

        //     auto doJob = [&]() -> void
        //     {
        //         std::unique_lock<std::mutex> lock(jobMutex);

        //         for (auto &&entry : job)
        //         {
        //             std::vector<std::string> pathVec = {
        //                 sourceFolderAbsPath,
        //                 entry};

        //             processingJob.emplace_back(entry);

        //             std::string fullPath;
        //             Utilities::CombinePath(pathVec, fullPath);
        //             std::string fileSHA3Hash;
        //             Hasher::GetSHA512StringValue(fullPath, fileSHA3Hash);
        //             auto addSetResult = fileHashSet.insert(fileSHA3Hash);

        //             if (!addSetResult.second)
        //             {
        //                 movedJob.emplace_back(entry);
        //                 std::string dstFilename;
        //                 Utilities::GetFileName(fullPath, dstFilename);
        //                 std::vector<std::string> dstFilepathVec = {
        //                     dstFolderAbsPath,
        //                     dstFilename};
        //                 std::string dstFilePath;
        //                 Utilities::CombinePath(dstFilepathVec, dstFilePath);

        //                 if (Utilities::MoveFile(fullPath, dstFilePath))
        //                 {
        //                     movedFiles.emplace_back(dstFilename);
        //                 }
        //             }
        //         }
        //     };

        //     threads.emplace_back(std::thread());
        // }

        for (int i = 0; i < allFiles.size(); i++)
        {
            auto entry = std::move(allFiles[i]);

            double progressRatio = static_cast<double>(i + 1) / static_cast<double>(allFiles.size());

            std::vector<std::string> pathVec = {
                sourceFolderAbsPath,
                entry};
            progresser(ProgressInfo::CALCULATE_FILE, progressRatio, "Calculate File Hash: " + entry);
            std::string fullPath;
            Utilities::CombinePath(pathVec, fullPath);
            std::string fileSHA3Hash;
            Hasher::GetSHA512StringValue(fullPath, fileSHA3Hash);
            auto addSetResult = fileHashSet.insert(fileSHA3Hash);

            if (!addSetResult.second)
            {
                progresser(ProgressInfo::CALCULATE_FILE, progressRatio, "Duplicated found, moving: " + entry);
                std::string dstFilename;
                Utilities::GetFileName(fullPath, dstFilename);
                std::vector<std::string> dstFilepathVec = {
                    dstFolderAbsPath,
                    dstFilename};
                std::string dstFilePath;
                Utilities::CombinePath(dstFilepathVec, dstFilePath);

                if (Utilities::MoveFile(fullPath, dstFilePath))
                {
                    movedFiles.emplace_back(dstFilename);
                }
            }
        }

        progresser(ProgressInfo::FINISHED, 1.0, "");
    }

    void Classificate(const std::string &sourceFolder, const std::string &newFolder, std::vector<std::string> &movedFiles)
    {
        ClassificateBase(sourceFolder, newFolder, movedFiles, [](const ProgressInfo pi, const double d, const std::string &s) -> void {});
    }

    void Classificate(const std::string &sourceFolder, const std::string &newFolder, std::vector<std::string> &movedFiles, const std::function<void(const ProgressInfo, const double, const std::string &)> &progresser)
    {
        ClassificateBase(sourceFolder, newFolder, movedFiles, progresser);
    }

    void GetProgressInfoText(const ProgressInfo info, std::string &infoStr)
    {
        switch (info)
        {
        case ProgressInfo::CALCULATE_FILE:
            infoStr = "Calculating file";
            break;
        case ProgressInfo::DST_FOLDER_ESTABLISHED:
            infoStr = "Created destiation folder";
            break;
        case ProgressInfo::FILE_LISTED:
            infoStr = "Source file listed";
            break;
        case ProgressInfo::FINISHED:
            infoStr = "Calcuation finished";
            break;
        case ProgressInfo::STARTED:
            infoStr = "Calcuation starting";
            break;
        }
    }
}
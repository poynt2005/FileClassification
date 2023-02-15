#ifndef __FILE_CLASSIFICATOR_H__
#define __FILE_CLASSIFICATOR_H__

#ifdef EXPORTFILERAPI
#define FILERAPI __declspec(dllexport)
#else
#define FILERAPI __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <functional>

namespace Hasher
{

    FILERAPI void GetSHA512StringValue(const std::string &, std::string &);
}

namespace Classificator
{
    enum class ProgressInfo
    {
        STARTED,
        FILE_LISTED,
        DST_FOLDER_ESTABLISHED,
        CALCULATE_FILE,
        FINISHED
    };

    FILERAPI void GetProgressInfoText(const ProgressInfo, std::string &);

    FILERAPI void Classificate(const std::string &, const std::string &, std::vector<std::string> &);
    FILERAPI void Classificate(const std::string &, const std::string &, std::vector<std::string> &, const std::function<void(const ProgressInfo, const double, const std::string &)> &);
}

namespace Utilities
{
    FILERAPI void GetCurrentPath(std::string &);
    FILERAPI void GetAbsolutePath(const std::string &, std::string &);
    FILERAPI void CombinePath(const std::vector<std::string> &, std::string &);
    FILERAPI bool MoveFile(const std::string &, const std::string &);
    FILERAPI void ListAllFile(const std::string &, std::vector<std::string> &);
    FILERAPI bool Mkdir(const std::string &);
    FILERAPI void GetFileName(const std::string &, std::string &);
}

#endif
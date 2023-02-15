#include <iostream>
#include "FileClassificator.h"
#include <thread>
int main()
{
    // std::string str = "Hello World";

    // CryptoPP::SHA3_512 hash;

    // std::string digest;
    // digest.resize(hash.DigestSize());

    // hash.Update(reinterpret_cast<const CryptoPP::byte *>(str.data()), str.length());
    // hash.Final(reinterpret_cast<CryptoPP::byte *>(const_cast<char *>(digest.data())));

    // std::string outString;
    // CryptoPP::StringSource(digest, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(outString)));
    // std::cout << outString << "\n";

    // std::string outString2;
    // CryptoPP::HashFilter f1(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(outString2)));
    // CryptoPP::ChannelSwitch cs;
    // cs.AddDefaultRoute(f1);

    // CryptoPP::FileSource("cryptlib.lib", true, new CryptoPP::Redirector(f1));

    // std::cout << outString2 << "\n";

    // std::vector<std::string> vec = {"erm", "a", "b"};

    // std::filesystem::path pqq;

    // for (auto &&p : vec)
    // {
    //     pqq /= p;
    // }

    // std::cout << pqq << "\n";
    // std::cout << std::filesystem::current_path() << '\n';

    std::cout << std::thread::hardware_concurrency() / 2 << "\n";
    std::vector<std::string> movedFiles;
    Classificator::Classificate("./wallpaper", "./newDir", movedFiles, [](Classificator::ProgressInfo info, const double ratio, const std::string &s) -> void
                                { std::cout << ratio << ", " << s << "\n"; });

    for (auto &&p : movedFiles)
    {
        std::cout << p << '\n';
    }

    std::cout << "Moved: " << movedFiles.size() << " files\n";

    std::cout << "End\n";
    return 0;
}
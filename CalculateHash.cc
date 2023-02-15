#include "FileClassificator.h"
#include "cryptlib.h"
#include "channels.h"
#include "filters.h"
#include "files.h"
#include "sha.h"
#include "sha3.h"
#include "hex.h"

namespace Hasher
{
    void GetSHA512StringValue(const std::string &inFilePath, std::string &outHashString)
    {
        CryptoPP::SHA3_512 hash;
        auto stringPipeline = new CryptoPP::StringSink(outHashString);
        auto hexEncoderPipeline = new CryptoPP::HexEncoder(stringPipeline);
        CryptoPP::HashFilter filter(hash, hexEncoderPipeline);
        CryptoPP::ChannelSwitch cs;
        cs.AddDefaultRoute(filter);
        auto cryptoRedirector = new CryptoPP::Redirector(cs);
        CryptoPP::FileSource(inFilePath.c_str(), true, cryptoRedirector);
    }
}
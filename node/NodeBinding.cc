#include "NodeBinding.h"
#include <string>
#include <vector>
#include <iostream>

Napi::Value NodeClassificate(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2)
    {
        Napi::TypeError::New(env, "Wrong number of arguments, expect at least 2 string arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() || !info[0].IsString())
    {
        Napi::TypeError::New(env, "Wrong type of arguments, position 0 or 1 must be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Function progresser;
    if (info.Length() >= 3)
    {
        if (!info[2].IsFunction())
        {
            Napi::TypeError::New(env, "Wrong type of argument, position 2 must be a function").ThrowAsJavaScriptException();
            return env.Null();
        }
        progresser = info[2].As<Napi::Function>();
    }

    std::vector<std::string> movedFiles;

    if (progresser)
    {
        Classificator::Classificate(info[0].As<Napi::String>().Utf8Value(), info[1].As<Napi::String>().Utf8Value(), movedFiles, [&env, &progresser](const Classificator::ProgressInfo info, const double ratio, const std::string &fileCalculateInfo) -> void
                                    {
                                        std::string progressInfoText;
                                        Classificator::GetProgressInfoText(info, progressInfoText);
                                        progresser.Call(env.Global(), {Napi::String::New(env, progressInfoText),
                                                                       Napi::Number::New(env, ratio),
                                                                       Napi::String::New(env, fileCalculateInfo)});
                                        // lambda end
                                    });
    }
    else
    {
        Classificator::Classificate(info[0].As<Napi::String>().Utf8Value(), info[1].As<Napi::String>().Utf8Value(), movedFiles);
    }
    Napi::Array jsMovedFiles = Napi::Array::New(env, movedFiles.size());

    for (int i = 0; i < movedFiles.size(); i++)
    {
        auto f = std::move(movedFiles[i]);
        jsMovedFiles.Set(i, Napi::String::New(env, f));
    }

    return jsMovedFiles;
}

Napi::Value NodeGetSHA512StringValue(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong number of argument, expect at least one string argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString())
    {
        Napi::TypeError::New(env, "Wrong type of argument, position 0 must be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string hexVal;
    Hasher::GetSHA512StringValue(info[0].As<Napi::String>().Utf8Value(), hexVal);

    return Napi::String::New(env, hexVal);
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "Classificate"),
        Napi::Function::New(env, NodeClassificate));
    exports.Set(
        Napi::String::New(env, "GetSHA512StringValue"),
        Napi::Function::New(env, NodeGetSHA512StringValue));
    return exports;
}

NODE_API_MODULE(FileClassificator, Initialize);

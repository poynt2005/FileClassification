#ifndef __NODE_BINDING_H__
#define __NODE_BINDING_H__

#include <napi.h>
#include "../FileClassificator.h"

Napi::Value NodeClassificate(const Napi::CallbackInfo &);
Napi::Value NodeGetSHA512StringValue(const Napi::CallbackInfo &);
Napi::Object Initialize(Napi::Env, Napi::Object);

#endif
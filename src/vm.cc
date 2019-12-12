#include "vm.h"

NrandomxVM::NrandomxVM(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NrandomxVM>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    randomx_vm *myMachine = info[0].As<Napi::External<randomx_vm>>().Data();
    this->vm = myMachine;
};

Napi::FunctionReference NrandomxVM::constructor;

void NrandomxVM::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "NrandomxVM", {});
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("NrandomxVM", func);
}

Napi::Object NrandomxVM::NewInstance(Napi::Value arg) {
    Napi::Object obj = constructor.New({ arg });
    return obj;
}
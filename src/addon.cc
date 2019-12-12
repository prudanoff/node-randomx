#include "vm.h"

Napi::Object CreateVM(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
   	Napi::String a = info[0].As<Napi::String>();
	std::string key(a.Utf8Value());

	char myKey[key.length() + 1];
	strcpy(myKey, key.c_str()); 

	// TODO: pass flags from the outside
	randomx_flags flags = randomx_get_flags();
	flags = RANDOMX_FLAG_DEFAULT;
	flags |= RANDOMX_FLAG_HARD_AES;
	flags |= RANDOMX_FLAG_JIT;
	flags |= RANDOMX_FLAG_FULL_MEM;

	randomx_cache *myCache = randomx_alloc_cache(flags);
	if (myCache == nullptr) {
		throw Napi::Error::New(env, "C++ error: randomx_alloc_cache() error");
	}

	randomx_init_cache(myCache, &myKey, sizeof myKey);
	randomx_dataset* dataset = randomx_alloc_dataset(flags);
	
	if (dataset == nullptr) {
		throw Napi::Error::New(env, "C++ error: randomx_alloc_dataset() error");
	}
	uint32_t datasetItemCount = randomx_dataset_item_count();
	randomx_init_dataset(dataset, myCache, 0, datasetItemCount);
	randomx_release_cache(myCache);
	myCache = nullptr;
	
	randomx_vm *myMachine = randomx_create_vm(flags, myCache, dataset);
	if (myMachine == nullptr) {
		throw Napi::Error::New(env, "C++ error: randomx_create_vm() error");
	}
    
    Napi::Object obj = NrandomxVM::NewInstance(Napi::External<randomx_vm>::New(info.Env(), myMachine));
	return obj;
}

Napi::String CalcHash(const Napi::CallbackInfo& info) {
   	char hash[RANDOMX_HASH_SIZE];
	NrandomxVM* nvm = Napi::ObjectWrap<NrandomxVM>::Unwrap(info[0].As<Napi::Object>());
   	Napi::String a = info[1].As<Napi::String>();
	std::string input(a.Utf8Value());

	char myInput[input.length() + 1];
	strcpy(myInput, input.c_str()); 

	randomx_calculate_hash(nvm->vm, &myInput, sizeof myInput, hash);

	//randomx_dataset* dataset;
	//randomx_get_dataset_memory(dataset);

	//randomx_destroy_vm(nvm->vm);
	//randomx_release_dataset(dataset);

	std::stringstream ss;
	for (int i = 0; i < RANDOMX_HASH_SIZE; ++i)
	    ss << std::setfill('0') << std::setw(2) << std::hex << (0xff & (unsigned int)hash[i]);

	return Napi::String::New(info.Env(), ss.str());
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
	NrandomxVM::Init(env, exports);
	exports.Set(Napi::String::New(env, "RandomxVM"),
	  Napi::Function::New(env, CreateVM));
	exports.Set(Napi::String::New(env, "hash"),
	  Napi::Function::New(env, CalcHash));
	return exports;
}

NODE_API_MODULE(addon, InitAll)
#include <wasmedge/wasmedge.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>

WasmEdge_Result check_u32_ptr(
    void *,
    const WasmEdge_CallingFrameContext *frame,
    const WasmEdge_Value *params,
    WasmEdge_Value *returns
) {
    int32_t offset = WasmEdge_ValueGetI32(params[0]);
    std::cout << "check_u32_ptr offset=" << offset << "\n";

    if (offset % 4 != 0) {
        returns[0] = WasmEdge_ValueGenI32(-1);
        return WasmEdge_Result_Success;
    }

    if (offset > 65536 - 4) {
        returns[0] = WasmEdge_ValueGenI32(-2);
        return WasmEdge_Result_Success;
    }

    WasmEdge_MemoryInstanceContext *memory = WasmEdge_CallingFrameGetMemoryInstance(frame, 0);
    uint8_t *ptr = WasmEdge_MemoryInstanceGetPointer(memory, offset, 4);

    uint32_t value = 
        static_cast<uint32_t>(ptr[0]) |
        (static_cast<uint32_t>(ptr[1]) << 8) |
        (static_cast<uint32_t>(ptr[2]) << 16) |
        (static_cast<uint32_t>(ptr[3]) << 24);

    returns[0] = WasmEdge_ValueGenI32(static_cast<int32_t>(value));
    return WasmEdge_Result_Success;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " misaligned_pointer_regression.wasm\n";
        return 1;
    }

    const char *wasm_path = argv[1];

    // TODO: create VM
    WasmEdge_ConfigureContext *conf_cxt = WasmEdge_ConfigureCreate();
    WasmEdge_VMContext *vm_cxt = WasmEdge_VMCreate(conf_cxt, nullptr);
    if (vm_cxt == nullptr) {
        std::cerr << "failed to create VM\n";
        return 1;
    }

    // TODO: create env module
    WasmEdge_String env_name = WasmEdge_StringCreateByCString("env");
    WasmEdge_ModuleInstanceContext *env_module = WasmEdge_ModuleInstanceCreate(env_name);
    WasmEdge_StringDelete(env_name);

    // TODO: add check_u32_ptr host function
    WasmEdge_ValType param_types[1] = {WasmEdge_ValTypeGenI32()};
    WasmEdge_ValType return_types[1] = {WasmEdge_ValTypeGenI32()};

    WasmEdge_FunctionTypeContext *func_type = WasmEdge_FunctionTypeCreate(param_types, 1, return_types, 1);
    WasmEdge_FunctionInstanceContext *func = WasmEdge_FunctionInstanceCreate(func_type, check_u32_ptr, nullptr, 0);

    WasmEdge_String func_name = WasmEdge_StringCreateByCString("check_u32_ptr");
    WasmEdge_ModuleInstanceAddFunction(env_module, func_name, func);

    WasmEdge_StringDelete(func_name);
    WasmEdge_FunctionTypeDelete(func_type);

    // TODO: register env module
    WasmEdge_Result res = WasmEdge_VMRegisterModuleFromImport(vm_cxt, env_module);
    if (!WasmEdge_ResultOK(res)) {
        std::cerr << "failed to register env module\n";
        return 1;
    }

    // TODO: load / validate / instantiate wasm
    res = WasmEdge_VMLoadWasmFromFile(vm_cxt, wasm_path);
    if (!WasmEdge_ResultOK(res)) {
        std::cerr << "failed to load wasm\n";
        return 1;
    }

    res = WasmEdge_VMValidate(vm_cxt);
    if (!WasmEdge_ResultOK(res)) {
        std::cerr << "failed to validate wasm\n";
        return 1;
    }

    res = WasmEdge_VMInstantiate(vm_cxt);
    if (!WasmEdge_ResultOK(res)) {
        std::cerr << "failed to instantiate wasm\n";
        return 1;
    }

    // TODO: call init
    WasmEdge_String init = WasmEdge_StringCreateByCString("init");
    res = WasmEdge_VMExecute(vm_cxt, init, nullptr, 0, nullptr, 0);
    if (!WasmEdge_ResultOK(res)) {
        printf("init failed\n");
        return EXIT_FAILURE;
    }
    WasmEdge_StringDelete(init);

    // TODO: call aligned_call
    WasmEdge_Value Returns[1];
    WasmEdge_String aligned_call = WasmEdge_StringCreateByCString("aligned_call");
    res = WasmEdge_VMExecute(vm_cxt, aligned_call, nullptr, 0, Returns, 1);
    if (!WasmEdge_ResultOK(res)) {
        printf("aligned_call failed\n");
        return EXIT_FAILURE;
    }
    int32_t result = WasmEdge_ValueGetI32(Returns[0]);
    if (result >= 0) {
        printf("aligned_call: ok value=0x%x\n", static_cast<uint32_t>(result));
    }
    WasmEdge_StringDelete(aligned_call);

    // TODO: call misaligned_call
    WasmEdge_String misaligned_call = WasmEdge_StringCreateByCString("misaligned_call");
    res = WasmEdge_VMExecute(vm_cxt, misaligned_call, nullptr, 0, Returns, 1);
    if (!WasmEdge_ResultOK(res)) {
        printf("misaligned_call failed\n");
        return EXIT_FAILURE;
    }
    int32_t status = WasmEdge_ValueGetI32(Returns[0]);
    if (status == -1) {
        printf("misaligned_call: error misaligned pointer\n");
    }
    WasmEdge_StringDelete(misaligned_call);

    // TODO: call oob_call
    WasmEdge_String oob_call = WasmEdge_StringCreateByCString("oob_call");
    res = WasmEdge_VMExecute(vm_cxt, oob_call, nullptr, 0, Returns, 1);
    if (!WasmEdge_ResultOK(res)) {
        printf("oob_call failed\n");
        return EXIT_FAILURE;
    }
    status = WasmEdge_ValueGetI32(Returns[0]);
    if (status == -2) {
        printf("oob_call: error out of bounds\n");
    }
    WasmEdge_StringDelete(oob_call);

    // clean
    WasmEdge_VMDelete(vm_cxt);
    WasmEdge_ConfigureDelete(conf_cxt);

    return 0;
}

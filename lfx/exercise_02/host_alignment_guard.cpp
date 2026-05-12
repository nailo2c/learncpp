#include <wasmedge/wasmedge.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

bool read_u32_guest_pointer(
    WasmEdge_MemoryInstanceContext *memory, 
    uint32_t offset,
    uint32_t *out_value
) {
    // 1. check offset 4-byte aligned
    if (offset % 4 != 0) {
        std::cout << "misaligned pointer\n";
        return false;
    }

    // 2. check offset..offset+3 in memory scope
    uint32_t pages = WasmEdge_MemoryInstanceGetPageSize(memory);
    uint64_t memory_size = static_cast<uint64_t>(pages) * 65536;
    uint64_t access_size = 4;
    if (offset > memory_size - access_size) {
        std::cout << "out of bounds\n";
        return false;
    }

    uint8_t *ptr = WasmEdge_MemoryInstanceGetPointer(memory, offset, 4);
    if (ptr == nullptr) {
        std::cout << "out of bounds\n";
        return false;
    }

    *out_value = 
        static_cast<uint32_t>(ptr[0]) |
        (static_cast<uint32_t>(ptr[1]) << 8) |
        (static_cast<uint32_t>(ptr[2]) << 16) |
        (static_cast<uint32_t>(ptr[3]) << 24);

    // e.g. 0x44 | (0x33 << 8) | (0x22 << 16) | (0x11 << 24)
    // result: 0x11223344
    
    return true;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " guest_pointers.wasm\n";
        return 1;
    }

    const char *wasm_path = argv[1];

    // TODO: create WasmEdge VM
    WasmEdge_ConfigureContext* conf_cxt = WasmEdge_ConfigureCreate();
    WasmEdge_VMContext* vm_cxt = WasmEdge_VMCreate(conf_cxt, nullptr);

    // TODO: load/instantiate wasm module
    WasmEdge_Result Res = WasmEdge_VMLoadWasmFromFile(vm_cxt, wasm_path);
    if (!WasmEdge_ResultOK(Res)) {
        printf("WASM file loading failed\n");
        return EXIT_FAILURE;
    }
    Res = WasmEdge_VMValidate(vm_cxt);
    if (!WasmEdge_ResultOK(Res)) {
        printf("WASM validation failed\n");
        return EXIT_FAILURE;
    }
    Res = WasmEdge_VMInstantiate(vm_cxt);
    if (!WasmEdge_ResultOK(Res)) {
        printf("WASM instantiation failed\n");
        return EXIT_FAILURE;
    }

    // TODO: call init
    WasmEdge_String func_name = WasmEdge_StringCreateByCString("init");
    Res = WasmEdge_VMExecute(vm_cxt, func_name, nullptr, 0, nullptr, 0);
    if (!WasmEdge_ResultOK(Res)) {
        printf("init failed\n");
        return EXIT_FAILURE;
    }

    WasmEdge_StringDelete(func_name);
    
    // TODO: call pointer functions
    WasmEdge_Value Returns[1];
    func_name = WasmEdge_StringCreateByCString("ptr_aligned");
    Res = WasmEdge_VMExecute(vm_cxt, func_name, nullptr, 0, Returns, 1);
    if (!WasmEdge_ResultOK(Res)) {
        printf("ptr_aligned failed\n");
        return EXIT_FAILURE;
    }
    int32_t offset = WasmEdge_ValueGetI32(Returns[0]);
    std::cout << "ptr_aligned offset=" << offset << "\n";

    // get memory
    const WasmEdge_ModuleInstanceContext *module = WasmEdge_VMGetActiveModule(vm_cxt);
    if (module == nullptr) {
        std::cout << "failed to get active module\n";
        return EXIT_FAILURE;
    }

    WasmEdge_String memory_name = WasmEdge_StringCreateByCString("memory");

    WasmEdge_MemoryInstanceContext *memory = WasmEdge_ModuleInstanceFindMemory(module, memory_name);
    WasmEdge_StringDelete(memory_name);
    if (memory == nullptr) {
        std::cout << "failed to find exported memory\n";
        return EXIT_FAILURE;
    }

    uint32_t value;
    read_u32_guest_pointer(memory, offset, &value);
    std::cout << "aligned: ok value=0x" << std::hex << value << std::dec << "\n";

    // ptr_misaligned
    func_name = WasmEdge_StringCreateByCString("ptr_misaligned");
    Res = WasmEdge_VMExecute(vm_cxt, func_name, nullptr, 0, Returns, 1);
    if (!WasmEdge_ResultOK(Res)) {
        printf("ptr_misaligned failed\n");
        return EXIT_FAILURE;
    }
    offset = WasmEdge_ValueGetI32(Returns[0]);
    std::cout << "ptr_misaligned offset=" << offset << "\n";

    WasmEdge_StringDelete(func_name);
    read_u32_guest_pointer(memory, offset, &value);

    // ptr_oob
    func_name = WasmEdge_StringCreateByCString("ptr_oob");
    Res = WasmEdge_VMExecute(vm_cxt, func_name, nullptr, 0, Returns, 1);
    if (!WasmEdge_ResultOK(Res)) {
        printf("ptr_oob failed\n");
        return EXIT_FAILURE;
    }
    offset = WasmEdge_ValueGetI32(Returns[0]);
    std::cout << "ptr_oob offset=" << offset << "\n";

    WasmEdge_StringDelete(func_name);
    read_u32_guest_pointer(memory, offset, &value);

    // Clean
    WasmEdge_VMDelete(vm_cxt);
    WasmEdge_ConfigureDelete(conf_cxt);

    return 0;
}

// c++ -std=c++17 host_alignment_guard.cpp -I/usr/local/include -L/usr/local/lib -lwasmedge -o host_alignment_guard
// ./host_alignment_guard guest_pointers.wasm

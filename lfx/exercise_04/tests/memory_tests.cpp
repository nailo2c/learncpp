#include "memory.hpp"

#include <cstdint>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <string>

bool expect_ok(const std::string &name, const std::function<void()> &fn) {
    try {
        fn();
        std::cout << "PASS " << name << "\n";
        return true;
    } catch (const std::runtime_error &err) {
        std::cout << "FAIL " << name << ": " << err.what() << "\n";
        return false;
    }
}

bool expect_throw(const std::string &name, const std::function<void()> &fn) {
    try {
        fn();
        std::cout << "FAIL " << name << ": expected exception\n";
        return false;
    } catch (const std::runtime_error &err) {
        std::cout << "PASS " << name << ": " << err.what() << "\n";
        return true;
    }
}

int main() {

    Memory memory(32);
    uint32_t value = 0x11223344;
    bool ok = true;

    ok &= expect_ok("store_i32", [&]() { memory.store_i32(4, value, 4); });
    ok &= expect_ok("load_i32_4_4", [&]() { memory.load_i32(4, 4); });
    ok &= expect_ok("load_i32_5_1", [&]() { memory.load_i32(5, 1); });
    ok &= expect_throw("load_i32_invalid", [&]() { memory.load_i32(5, 8); });
    ok &= expect_throw("load_i32_oob", [&]() { memory.load_i32(32 - 2, 1); });
    ok &= expect_throw("read_guest_pointer_u32_17", [&]() { memory.read_guest_pointer_u32(17); });
    ok &= expect_ok("read_guest_pointer_u32_16", [&]() { memory.read_guest_pointer_u32(16); });

    return ok ? 0 : 1;
}

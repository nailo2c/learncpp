#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class Memory {
public:
    explicit Memory(std::size_t size);

    uint32_t load_i32(uint32_t address, uint32_t declared_align);
    void store_i32(uint32_t address, uint32_t value, uint32_t declared_align);

    uint64_t load_i64(uint32_t address, uint32_t declared_align);
    void store_i64(uint32_t address, uint64_t value, uint32_t declared_align);

    uint32_t read_guest_pointer_u32(uint32_t address);

private:
    std::vector<uint8_t> data_;
};

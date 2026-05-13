#include "memory.hpp"

#include <cstdint>
#include <iostream>
#include <stdexcept>

Memory::Memory(std::size_t size) : data_(size, 0) {}

void Memory::store_i32(uint32_t address, uint32_t value, uint32_t declared_align) {
    // check if declared_align valid
    if (declared_align != 1 && declared_align != 2 && declared_align != 4) {
        throw std::runtime_error("invalid declared alignment");
    }

    // check if declared_align oob
    if (data_.size() < 4 || address > data_.size() - 4) {
        throw std::runtime_error("out of bounds");
    }

    data_[address + 0] = value & 0xff;
    data_[address + 1] = (value >> 8) & 0xff;
    data_[address + 2] = (value >> 16) & 0xff;
    data_[address + 3] = (value >> 24) & 0xff;
}

uint32_t Memory::load_i32(uint32_t address, uint32_t declared_align) {
    // check if declared_align valid
    if (declared_align != 1 && declared_align != 2 && declared_align != 4) {
        throw std::runtime_error("invalid declared alignment");
    }

    // check if access is in bounds
    if (data_.size() < 4 || address > data_.size() - 4) {
        throw std::runtime_error("out of bounds");
    }

    return static_cast<uint32_t>(data_[address + 0]) |
        (static_cast<uint32_t>(data_[address + 1]) << 8) |
        (static_cast<uint32_t>(data_[address + 2]) << 16) |
        (static_cast<uint32_t>(data_[address + 3]) << 24);
}

void Memory::store_i64(uint32_t address, uint64_t value, uint32_t declared_align) {
    // check if declared_align valid
    if (declared_align != 1 && declared_align != 2 && declared_align != 4 && declared_align != 8) {
        throw std::runtime_error("invalid declared alignment");
    }

    // check if declared_align oob
    if (data_.size() < 8 || address > data_.size() - 8) {
        throw std::runtime_error("out of bounds");
    }

    data_[address + 0] = value & 0xff;
    data_[address + 1] = (value >> 8) & 0xff;
    data_[address + 2] = (value >> 16) & 0xff;
    data_[address + 3] = (value >> 24) & 0xff;
    data_[address + 4] = (value >> 32) & 0xff;
    data_[address + 5] = (value >> 40) & 0xff;
    data_[address + 6] = (value >> 48) & 0xff;
    data_[address + 7] = (value >> 56) & 0xff;
}

uint64_t Memory::load_i64(uint32_t address, uint32_t declared_align) {
    // check if declared_align valid
    if (declared_align != 1 && declared_align != 2 && declared_align != 4 && declared_align != 8) {
        throw std::runtime_error("invalid declared alignment");
    }

    // check if access is in bounds
    if (data_.size() < 8 || address > data_.size() - 8) {
        throw std::runtime_error("out of bounds");
    }

    return static_cast<uint64_t>(data_[address + 0]) |
        (static_cast<uint64_t>(data_[address + 1]) << 8) |
        (static_cast<uint64_t>(data_[address + 2]) << 16) |
        (static_cast<uint64_t>(data_[address + 3]) << 24) |
        (static_cast<uint64_t>(data_[address + 4]) << 32) |
        (static_cast<uint64_t>(data_[address + 5]) << 40) |
        (static_cast<uint64_t>(data_[address + 6]) << 48) |
        (static_cast<uint64_t>(data_[address + 7]) << 56);
}

uint32_t Memory::read_guest_pointer_u32(uint32_t address) {
    if (address % 4 != 0) {
        throw std::runtime_error("misaligned pointer");
    }

    // check if access is in bounds
    if (data_.size() < 4 || address > data_.size() - 4) {
        throw std::runtime_error("out of bounds");
    }

    return static_cast<uint32_t>(data_[address + 0]) |
        (static_cast<uint32_t>(data_[address + 1]) << 8) |
        (static_cast<uint32_t>(data_[address + 2]) << 16) |
        (static_cast<uint32_t>(data_[address + 3]) << 24);
}

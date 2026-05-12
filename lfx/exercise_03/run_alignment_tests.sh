#!/usr/bin/env bash

passed=0
failed=0

pass() {
    echo "PASS $1"
    passed=$((passed + 1))
}

fail() {
    echo "FAIL $1"
    failed=$((failed + 1))
}

wasm_path() {
    local wat="$1"
    echo "${wat%.wat}.wasm"
}

expect_compile_pass() {
    local name="$1"
    local wat="tests/${name}.wat"
    local wasm
    wasm="$(wasm_path "$wat")"

    if wat2wasm "$wat" -o "$wasm" >/tmp/"${name}".wat2wasm.log 2>&1; then
        pass "$name compile"
    else
        cat /tmp/"${name}".wat2wasm.log
        fail "$name compile"
    fi
}

expect_compile_fail() {
    local name="$1"
    local wat="tests/${name}.wat"
    local wasm
    wasm="$(wasm_path "$wat")"

    if wat2wasm "$wat" -o "$wasm" >/tmp/"${name}".wat2wasm.log 2>&1; then
        fail "$name rejected"
    else
        cat /tmp/"${name}".wat2wasm.log
        pass "$name rejected"
    fi
}

expect_run_pass() {
    local name="$1"
    local func="$2"
    local wasm="tests/${name}.wasm"

    if wasmedge --reactor "$wasm" "$func" >/tmp/"${name}".wasmedge.log 2>&1; then
        cat /tmp/"${name}".wasmedge.log
        pass "$name run"
    else
        cat /tmp/"${name}".wasmedge.log
        fail "$name run"
    fi
}

expect_run_fail() {
    local name="$1"
    local func="$2"
    local wasm="tests/${name}.wasm"

    if wasmedge --reactor "$wasm" "$func" >/tmp/"${name}".wasmedge.log 2>&1; then
        cat /tmp/"${name}".wasmedge.log
        fail "$name trapped"
    else
        cat /tmp/"${name}".wasmedge.log
        pass "$name trapped"
    fi
}

build_only() {
    expect_compile_pass valid_i32_align_1
    expect_compile_pass valid_i32_align_2
    expect_compile_pass valid_i32_align_4
    expect_compile_fail invalid_i32_align_8
    expect_compile_pass valid_i64_store_align_8
    expect_compile_fail invalid_i64_store_align_16
    expect_compile_pass runtime_unaligned_address
    expect_compile_pass runtime_oob_address
}

run_all() {
    build_only
    expect_run_pass valid_i32_align_1 valid_i32_align_1
    expect_run_pass valid_i32_align_2 valid_i32_align_2
    expect_run_pass valid_i32_align_4 valid_i32_align_4
    expect_run_pass valid_i64_store_align_8 valid_i64_store_align_8
    expect_run_pass runtime_unaligned_address runtime_unaligned_address
    expect_run_fail runtime_oob_address runtime_oob_address
}

if [[ "${1:-}" == "--build-only" ]]; then
    build_only
elif [[ $# -eq 0 ]]; then
    run_all
else
    echo "usage: ./run_alignment_tests.sh [--build-only]"
    exit 2
fi

echo "${passed} passed, ${failed} failed"

if [[ "$failed" -ne 0 ]]; then
    exit 1
fi

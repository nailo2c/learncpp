#!/usr/bin/env bash
set -e

mode="${1:-}"

if [[ "$mode" != "interpreter" && "$mode" != "aot" ]]; then
    echo "usage ./run_regression.sh interpreter|aot"
    exit 2
fi

wat2wasm misaligned_pointer_regression.wat -o misaligned_pointer_regression.wasm

cmake -S . -B build
cmake --build build

if [[ "$mode" == "interpreter" ]]; then
    echo "mode=interpreter"
    timeout 5s ./build/runner misaligned_pointer_regression.wasm
    exit 0
fi

if [[ "$mode" == "aot" ]]; then
    echo "mode=aot"

    if ! command -v wasmedgec >/dev/null 2>&1; then
        echo "wasmedgec not installed"
        exit 0
    fi

    wasmedgec misaligned_pointer_regression.wasm misaligned_pointer_regression.aot.wasm

    timeout 5s ./build/runner misaligned_pointer_regression.aot.wasm
    exit 0
fi


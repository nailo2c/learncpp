;; wat2wasm misaligned_pointer_regression.wat -o misaligned_pointer_regression.wasm
(module
    (import "env" "check_u32_ptr"
        (func $check_u32_ptr (param i32) (result i32)))
    (memory (export "memory") 1)

    (func (export "init")
        i32.const 16
        i32.const 0x11223344
        i32.store
    )

    (func (export "aligned_call") (result i32)
        i32.const 16
        call $check_u32_ptr  ;; pass 16 as ptr to C++ host
    )

    (func (export "misaligned_call") (result i32)
        i32.const 17
        call $check_u32_ptr  ;; pass 17 as ptr to C++ host
    )

    (func (export "oob_call") (result i32)
        i32.const 65536
        call $check_u32_ptr  ;; pass 65536 as out-of-bounds ptr to C++ host
    )
)

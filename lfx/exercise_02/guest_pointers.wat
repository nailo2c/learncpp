;; wat2wasm guest_pointers.wat -o guest_pointers.wasm
(module
    (memory (export "memory") 1)

    (func (export "init")
        i32.const 16
        i32.const 0x11223344
        i32.store
    )

    (func (export "ptr_aligned") (result i32)
        i32.const 16
    )

    (func (export "ptr_misaligned") (result i32)
        i32.const 17
    )

    (func (export "ptr_oob") (result i32)
        i32.const 65536
    )
)

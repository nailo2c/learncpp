;; wat2wasm runtime_oob_address.wat -o runtime_oob_address.wasm
(module
    (memory (export "memory") 1)  ;; export memory so the host can inspect it

    (func (export "runtime_oob_address") (result i32)
        i32.const 65536
        i32.load align=4
    )
)

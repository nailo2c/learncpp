;; i64.store natural alignment is 8, so declared align=8 is valid.
(module
    (memory (export "memory") 1)

    (func (export "valid_i64_store_align_8")
        i32.const 8  ;; memory addr / offset is i32 in Wasm memory
        i64.const 0x11223344
        i64.store align=8
    )
)

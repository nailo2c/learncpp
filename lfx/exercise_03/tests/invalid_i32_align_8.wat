;; i32.load natural alignment is 4, so declared align=8 is invalid.
(module
    (memory (export "memory") 1)

    (data (i32.const 0) "\01\02\03\04\05\06\07\08")

    (func (export "invalid_i32_align_8") (result i32)
        i32.const 0
        i32.load align=8
    )
)

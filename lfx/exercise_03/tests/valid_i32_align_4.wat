;; i32.load natural alignment is 4, so declared align=4 is valid.
(module
    (memory (export "memory") 1)

    (data (i32.const 0) "\01\02\03\04\05\06\07\08")

    (func (export "valid_i32_align_4") (result i32)
        i32.const 0
        i32.load align=4
    )
)

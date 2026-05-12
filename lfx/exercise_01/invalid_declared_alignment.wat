;; wat2wasm invalid_declared_alignment.wat -o invalid_declared_alignment.wasm
;; invalid_declared_alignment.wat:10:9: error: alignment must not be larger than natural alignment (4)
;;         i32.load align=8
;;         ^^^^^^^^
(module
    ;; 1 page = 65536 bytes
    (memory (export "memory") 1)  ;; export memory so the host can inspect it

    ;; put known bytes starting at memory address 0
    (data (i32.const 0) "\01\02\03\04\05\06\07\08")  ;; \01 = 0x01 = binary 00000001 (\01 is hex byte escape)

    (func (export "load_aligned_8") (result i32)
        i32.const 0
        i32.load align=8
    )
)

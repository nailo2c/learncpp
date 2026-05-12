;; wat2wasm runtime_unaligned_address.wat -o runtime_unaligned_address.wasm
(module
    ;; 1 page = 65536 bytes
    (memory (export "memory") 1)  ;; export memory so the host can inspect it

    ;; put known bytes starting at memory address 0
    (data (i32.const 0) "\01\02\03\04\05\06\07\08")  ;; \01 = 0x01 = binary 00000001 (\01 is hex byte escape)

    (func (export "runtime_unaligned_address") (result i32)
        i32.const 1
        i32.load align=4
    )
)

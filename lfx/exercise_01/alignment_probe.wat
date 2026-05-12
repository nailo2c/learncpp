;; wat2wasm alignment_probe.wat -o alignment_probe.wasm
;; wasmedge --reactor alignment_probe.wasm load_aligned
;; wasmedge --reactor alignment_probe.wasm load_unaligned
;; wasmedge --reactor alignment_probe.wasm load_oob
(module
    ;; 1 page = 65536 bytes
    (memory (export "memory") 1)  ;; export memory so the host can inspect it

    ;; put known bytes starting at memory address 0
    (data (i32.const 0) "\01\02\03\04\05\06\07\08")  ;; \01 = 0x01 = binary 00000001 (\01 is hex byte escape)

    (func (export "load_aligned") (result i32)
        i32.const 0  ;; push 0 to the Wasm stack
        i32.load     ;; treat 0 as the mem addr and read 4 bytes as an i32
    )

    (func (export "load_unaligned") (result i32)
        i32.const 1
        i32.load
    )

    (func (export "load_oob") (result i32)
        i32.const 65534
        i32.load
    )
)

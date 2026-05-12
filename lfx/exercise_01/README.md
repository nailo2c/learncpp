```console
aaron.chen@Aarons-MacBook-Air-7 exercise_01 % wat2wasm alignment_probe.wat -o alignment_probe.wasm
aaron.chen@Aarons-MacBook-Air-7 exercise_01 % wasmedge --reactor alignment_probe.wasm load_aligned
84148994

aaron.chen@Aarons-MacBook-Air-7 exercise_01 % wat2wasm alignment_probe.wat -o alignment_probe.wasm  
aaron.chen@Aarons-MacBook-Air-7 exercise_01 % wasmedge --reactor alignment_probe.wasm load_unaligned
67305985

aaron.chen@Aarons-MacBook-Air-7 exercise_01 % wat2wasm alignment_probe.wat -o alignment_probe.wasm  
aaron.chen@Aarons-MacBook-Air-7 exercise_01 % wasmedge --reactor alignment_probe.wasm load_unaligned
8

aaron.chen@Aarons-MacBook-Air-7 exercise_01 % wat2wasm invalid_declared_alignment.wat -o invalid_declared_alignment.wasm
invalid_declared_alignment.wat:10:9: error: alignment must not be larger than natural alignment (4)
        i32.load align=8
        ^^^^^^^^
```        

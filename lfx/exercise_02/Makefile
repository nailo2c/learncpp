all: guest_pointers.wasm host_alignment_guard

guest_pointers.wasm: guest_pointers.wat
	wat2wasm guest_pointers.wat -o guest_pointers.wasm

host_alignment_guard: host_alignment_guard.cpp
	c++ -std=c++17 host_alignment_guard.cpp -I/usr/local/include -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lwasmedge -o host_alignment_guard

run: all
	./host_alignment_guard guest_pointers.wasm

clean:
	rm -f guest_pointers.wasm host_alignment_guard

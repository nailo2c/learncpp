# WasmEdge LFX 2026 Term 2 Practice Pre-test 2 Exercises

These exercises are not official WasmEdge or LFX pre-tests. They are plausible practice tasks based on the public project issue:

- WasmEdge issue #4820: https://github.com/WasmEdge/WasmEdge/issues/4820
- Related issues mentioned there: #2694, #2733, #2881

Important background distinction:

- Standard WebAssembly integer and floating-point memory accesses may use unaligned effective addresses. The `align=` immediate on `i32.load`, `i64.store`, and similar instructions is a validation/performance hint, not a runtime trap condition by itself.
- WASI-style guest pointers are different. A host API that interprets an `i32` as a pointer to a typed guest object may need to reject a pointer whose address is not aligned for the pointed-to type.
- Out-of-bounds memory access is separate from misalignment. A pointer can be aligned but out of range, in range but misaligned, or both. Good tests name those cases separately.

# Practice Exercise 1: WAT Memory Alignment Probe
Difficulty: Easy

## Task

Create a small WebAssembly text module that demonstrates the difference between:

- aligned `i32.load` access,
- unaligned but in-bounds `i32.load` access,
- out-of-bounds `i32.load` access,
- invalid declared alignment in the instruction encoding.

The module should export one memory and at least three functions:

- `load_aligned`: loads an `i32` from an address divisible by 4.
- `load_unaligned`: loads an `i32` from an address not divisible by 4 but still inside memory.
- `load_oob`: attempts to load an `i32` from an address close enough to the end of memory that the access crosses the memory boundary.

Use a data segment with known bytes so the two successful loads return predictable little-endian values.

Also create a second intentionally invalid WAT file containing an `i32.load align=8`. This file should fail validation or fail during `wat2wasm`.

## Submission Requirements

Submit these files:

- `alignment_probe.wat`
- `invalid_declared_alignment.wat`
- `README.md`

Expected build commands:

```bash
wat2wasm alignment_probe.wat -o alignment_probe.wasm
wat2wasm invalid_declared_alignment.wat -o invalid_declared_alignment.wasm
```

Expected run commands:

```bash
wasmedge --reactor alignment_probe.wasm load_aligned
wasmedge --reactor alignment_probe.wasm load_unaligned
wasmedge --reactor alignment_probe.wasm load_oob
```

Command-output requirements:

- Include terminal output showing `alignment_probe.wat` compiles.
- Include terminal output showing `load_aligned` returns the expected value.
- Include terminal output showing `load_unaligned` also returns a value and does not trap solely because the address is unaligned.
- Include terminal output showing `load_oob` traps with an out-of-bounds memory access.
- Include terminal output showing `invalid_declared_alignment.wat` is rejected.

## Expected Behavior

The aligned load should succeed.

The unaligned in-bounds load should also succeed in standard WebAssembly. It may be slower on some implementations, but the address being unaligned is not itself a Wasm trap.

The out-of-bounds load should trap because the effective address plus access width exceeds the current memory size.

The invalid declared alignment case should fail before execution because `i32.load align=8` declares an alignment larger than the natural alignment of `i32`, which is 4 bytes.

## What the Evaluator Is Likely Checking

- Correct WAT syntax for memory, data segments, exports, and memory instructions.
- Correct understanding of little-endian load results.
- Correct distinction between the `align=` immediate and the actual effective address.
- Correct explanation that ordinary unaligned Wasm loads do not trap.
- Clear README with reproducible commands.
- Evidence that out-of-bounds behavior is tested separately from misalignment.

## Hints

- A WebAssembly page is 65536 bytes.
- `i32.load` reads 4 bytes.
- Put bytes at addresses like `0`, `4`, and `5` so you can compare aligned and unaligned loads.
- Do not use `align=8` in the valid module. Use it only in the intentionally invalid file.
- If your WasmEdge version does not print return values from `--reactor`, add a tiny runner or use another tool only for printing return values, then still run WasmEdge to show trap behavior.

## Practice Value

This exercise practices the WebAssembly memory model, WAT syntax, load/store alignment hints, little-endian memory behavior, and the difference between misalignment and out-of-bounds traps.

# Practice Exercise 2: Host-side Guest Pointer Alignment Guard
Difficulty: Medium

## Task

Write a small C++ program using the WasmEdge C API that loads a Wasm module, reads exported guest memory, and safely interprets guest offsets as `uint32_t` pointers.

Create a WAT module that exports:

- `memory`
- `init`: stores a known 32-bit value in memory
- `ptr_aligned`: returns an aligned offset, such as `16`
- `ptr_misaligned`: returns a misaligned offset, such as `17`
- `ptr_oob`: returns an aligned offset that is too close to the end of memory for a 4-byte read

The C++ host program should:

1. Load and instantiate the Wasm module through the WasmEdge C API.
2. Call `init`.
3. Call each pointer-returning function.
4. Read a `uint32_t` from guest memory only through a helper such as `read_u32_guest_pointer(memory, offset)`.
5. Reject misaligned offsets before dereferencing.
6. Reject out-of-bounds offsets before dereferencing.
7. Print a separate result line for each case.

## Submission Requirements

Submit these files:

- `guest_pointers.wat`
- `host_alignment_guard.cpp`
- `CMakeLists.txt` or `Makefile`
- `README.md`

Expected build commands:

```bash
wat2wasm guest_pointers.wat -o guest_pointers.wasm
cmake -S . -B build
cmake --build build
```

Alternative single-command build is acceptable if documented, for example:

```bash
c++ -std=c++17 host_alignment_guard.cpp -o host_alignment_guard $(pkg-config --cflags --libs wasmedge)
```

Expected run command:

```bash
./build/host_alignment_guard guest_pointers.wasm
```

Command-output requirements:

- Include output showing the aligned pointer succeeds and prints the expected `uint32_t` value.
- Include output showing the misaligned pointer is rejected as `misaligned pointer`.
- Include output showing the out-of-bounds pointer is rejected as `out of bounds`.
- Include output showing nonzero exit status only if module loading or setup fails. Expected pointer failures should be reported as handled test cases, not as a crashed process.

## Expected Behavior

Example output shape:

```text
aligned: ok value=0x11223344
misaligned: error misaligned pointer offset=17 align=4
oob: error out of bounds offset=65534 size=4 memory_size=65536
summary: 3/3 cases passed
```

The aligned case should read exactly the expected value from guest memory.

The misaligned case should not call `reinterpret_cast<uint32_t *>` on an unaligned host pointer. The program should detect `offset % 4 != 0` and report a controlled error.

The out-of-bounds case should verify that `offset + sizeof(uint32_t)` is within the memory length before reading.

## What the Evaluator Is Likely Checking

- Correct WasmEdge C API setup and teardown.
- Correct retrieval of exported memory and exported function results.
- No undefined behavior from misaligned C++ pointer dereference.
- Bounds checks that avoid integer overflow.
- Clean separation between host pointer validation and Wasm memory instruction behavior.
- Useful output that makes each test case easy to inspect.
- Clean C++ code with RAII or clear cleanup paths.

## Hints

- Treat the guest offset as untrusted input, even if it came from your own Wasm module.
- Prefer reading bytes and assembling the `uint32_t` manually, or use `std::memcpy` after checking bounds and alignment.
- If you use `WasmEdge_MemoryInstanceGetPointer`, pass the access length, not just the offset.
- Check `offset > memory_size - size` instead of `offset + size > memory_size` to avoid overflow.
- Keep the WAT module simple. This exercise is mostly about the host-side guard.

## Practice Value

This exercise practices WasmEdge C API usage, guest memory access, safe pointer validation, C++ runtime debugging, and the project-relevant distinction between Wasm memory accesses and host APIs that interpret guest offsets as typed pointers.

# Practice Exercise 3: Binary Validation Tests for Memory Alignment Immediates
Difficulty: Medium

## Task

Build a small binary test suite that validates WebAssembly memory instruction alignment immediates.

Create several WAT files and a small test runner. The runner can be a shell script, C++ program, or CMake test target. It should compile each WAT file to Wasm and then run verification/execution checks with WasmEdge.

Include at least these cases:

- Valid `i32.load align=1`
- Valid `i32.load align=2`
- Valid `i32.load align=4`
- Invalid `i32.load align=8`
- Valid `i64.store align=8`
- Invalid `i64.store align=16`
- Valid unaligned effective address using a valid declared alignment
- Out-of-bounds effective address using a valid declared alignment

The purpose is to test both the binary validation rule for the `align` immediate and the runtime behavior of addresses.

## Submission Requirements

Submit these files:

- `tests/valid_i32_align_1.wat`
- `tests/valid_i32_align_2.wat`
- `tests/valid_i32_align_4.wat`
- `tests/invalid_i32_align_8.wat`
- `tests/valid_i64_store_align_8.wat`
- `tests/invalid_i64_store_align_16.wat`
- `tests/runtime_unaligned_address.wat`
- `tests/runtime_oob_address.wat`
- `run_alignment_tests.sh` or equivalent C++/CMake test runner
- `README.md`

Expected build command:

```bash
./run_alignment_tests.sh --build-only
```

Expected run command:

```bash
./run_alignment_tests.sh
```

Command-output requirements:

- Print `PASS` or `FAIL` for every test case.
- For invalid modules, show that rejection happens during `wat2wasm`, WasmEdge validation, or both.
- For the unaligned effective address module, show that execution succeeds.
- For the out-of-bounds effective address module, show that execution traps.
- End with a summary such as `8 passed, 0 failed`.

## Expected Behavior

Modules with a declared alignment no larger than the natural alignment of the memory access should validate.

Modules with a declared alignment larger than the natural alignment should be rejected.

A valid module that performs an in-bounds unaligned `i32.load` should execute successfully.

A valid module that performs an out-of-bounds load/store should trap during execution.

## What the Evaluator Is Likely Checking

- You can create focused `.wat` files for binary/runtime behavior.
- You understand that `align=1`, `align=2`, and `align=4` are valid for `i32.load`, while `align=8` is not.
- You understand that `align=8` is valid for `i64.store`, while `align=16` is not.
- Your test runner handles expected failures without aborting the whole suite.
- Your output is readable and reproducible.
- Your tests separate validation-time failures from execution-time traps.

## Hints

- WAT syntax uses byte alignment values such as `align=4`, not log2 values.
- In the binary encoding, alignment is stored as an exponent, but WAT hides that detail.
- Keep every test module tiny. One memory, one exported function, one behavior.
- Use stable string matching for output only where necessary. Exit status is usually more reliable than exact error text.
- Add comments in each WAT file explaining why the case should pass or fail.

## Practice Value

This exercise practices Wasm binary/runtime tests, WAT syntax, validation rules, load/store alignment immediates, and test harness design.

# Practice Exercise 4: Mini Runtime Memory Access Checker
Difficulty: Medium

## Task

Implement a small C++ memory-access checker that models the parts of a Wasm runtime needed for load/store testing.

You do not need to implement a full interpreter. Create a `Memory` class backed by `std::vector<uint8_t>` and helper functions for:

- `load_i32(address, declared_align)`
- `store_i32(address, value, declared_align)`
- `load_i64(address, declared_align)`
- `store_i64(address, value, declared_align)`
- `read_guest_pointer_u32(address)`

Rules:

- Reject declared alignments greater than the natural alignment of the instruction.
- For ordinary Wasm integer load/store operations, allow in-bounds unaligned effective addresses.
- Reject out-of-bounds accesses.
- For `read_guest_pointer_u32`, require `address % 4 == 0` and require the access to be in bounds.
- Do not use undefined behavior for unaligned host reads or writes.

Add tests that cover:

- aligned `i32` load/store,
- unaligned but in-bounds `i32` load/store,
- invalid declared alignment,
- aligned out-of-bounds access,
- misaligned guest pointer,
- valid guest pointer.

## Submission Requirements

Submit these files:

- `include/memory.hpp`
- `src/memory.cpp`
- `tests/memory_tests.cpp`
- `CMakeLists.txt`
- `README.md`

Expected build commands:

```bash
cmake -S . -B build
cmake --build build
```

Expected run command:

```bash
ctest --test-dir build --output-on-failure
```

If you do not use CTest, document an equivalent command:

```bash
./build/memory_tests
```

Command-output requirements:

- Include output showing all tests pass.
- Include at least one printed failure message from a deliberately negative test case, such as `invalid declared alignment` or `misaligned pointer`, if your test framework reports checked exceptions.
- Include sanitizer output if you run with sanitizers enabled.

## Expected Behavior

The checker should behave like this:

```text
store_i32(4, value, 4) -> ok
load_i32(4, 4) -> ok
load_i32(5, 1) -> ok if bytes 5..8 are in bounds
load_i32(5, 8) -> validation/configuration error
load_i32(memory_size - 2, 1) -> out of bounds
read_guest_pointer_u32(17) -> misaligned pointer
read_guest_pointer_u32(16) -> ok if bytes 16..19 are in bounds
```

The implementation should never depend on the host CPU allowing unaligned access. Use byte operations or `std::memcpy`.

## What the Evaluator Is Likely Checking

- Clean C++ API design for memory checks.
- Correct natural alignment constants.
- Correct bounds checking, including overflow-safe arithmetic.
- Clear separation between validation errors, runtime out-of-bounds traps, and guest pointer alignment traps.
- Tests that would catch a runtime accidentally treating all unaligned Wasm loads as traps.
- Tests that would catch a host implementation accidentally dereferencing a misaligned pointer.

## Hints

- A declared alignment of zero is not useful in WAT form, but your C++ helper can represent alignment in bytes and reject non-powers of two.
- Consider returning a small `Result<T>` type or throwing a custom exception with an error enum.
- Keep byte order explicit. WebAssembly numeric memory operations are little-endian.
- If using exceptions, make the tests assert the specific error category, not just "throws something".
- Sanitizers are useful here: `-fsanitize=address,undefined` can reveal accidental bad host accesses.

## Practice Value

This exercise practices C++ runtime logic, memory bounds checking, alignment validation, safe byte-level loads/stores, and test-driven debugging of memory behavior.

# Practice Exercise 5: End-to-end Misaligned Guest Pointer Regression Harness
Difficulty: Hard

## Task

Create a small end-to-end regression harness that mimics the kind of bug described in the related WasmEdge issues: guest code corrupts or mutates a pointer value, and a host-facing operation must classify the resulting pointer correctly.

Build a WAT module with:

- an exported memory,
- a mutable global that acts like a simple shadow stack pointer,
- an `init` function that writes a known 32-bit value at an aligned stack address,
- an `aligned_call` function that passes an aligned pointer to an imported host function,
- a `misaligned_call` function that corrupts the pointer by adding 1 before passing it to the imported host function,
- an `oob_call` function that passes an aligned pointer outside the valid memory range,
- optionally, a `both_bad_call` function that passes a pointer that is both misaligned and out of bounds.

Then write a C++ WasmEdge C API host runner that:

1. Registers an imported host function, for example `env.check_u32_ptr`.
2. Instantiates the WAT-compiled Wasm module.
3. Lets the guest call the host import with several pointer values.
4. In the host import, validates the pointer before reading memory.
5. Reports a deterministic classification for every case:
   - `ok`
   - `misaligned pointer`
   - `out of bounds`
   - if both apply, a documented priority such as `misaligned pointer` first
6. Runs the same checks with the interpreter path and, if available, the AOT path through `wasmedgec`.
7. Uses a timeout in the test script so a hang is reported as a failed test.

This should be a small repo-style task, not a full WasmEdge patch. The goal is to build a realistic reproducer and regression test shape.

## Submission Requirements

Submit these files:

- `misaligned_pointer_regression.wat`
- `runner.cpp`
- `run_regression.sh`
- `CMakeLists.txt` or `Makefile`
- `README.md`

Expected build commands:

```bash
wat2wasm misaligned_pointer_regression.wat -o misaligned_pointer_regression.wasm
cmake -S . -B build
cmake --build build
```

Expected run commands:

```bash
./run_regression.sh interpreter
./run_regression.sh aot
```

The AOT command may internally use:

```bash
wasmedgec misaligned_pointer_regression.wasm misaligned_pointer_regression.aot.wasm
./build/runner misaligned_pointer_regression.aot.wasm
```

Command-output requirements:

- Include interpreter output.
- Include AOT output if `wasmedgec` is available, or document that `wasmedgec` is not installed.
- Include timeout behavior, such as a `timeout 5s` wrapper around each run.
- Include output proving the process does not hang.
- Include a short table in `README.md` listing each guest function and expected classification.

## Expected Behavior

Example output shape:

```text
mode=interpreter
aligned_call: ok value=0xdecafbad
misaligned_call: error misaligned pointer offset=1025 align=4
oob_call: error out of bounds offset=65536 size=4 memory_size=65536
both_bad_call: error misaligned pointer offset=65537 align=4
summary: 4/4 cases passed
```

The aligned path should succeed.

The misaligned path should fail quickly and predictably. It should not produce a checksum, continue silently, crash the host, or hang.

The out-of-bounds path should be reported as out of bounds when the pointer is aligned but outside memory.

If you include the case where both misalignment and out-of-bounds apply, the chosen priority should be documented and tested. The important part is deterministic behavior.

## What the Evaluator Is Likely Checking

- Correct construction of a small Wasm reproducer.
- Correct WasmEdge C API host import registration.
- Safe access to exported guest memory from a host function.
- No process hang when a bad pointer is passed.
- Clear classification of aligned, misaligned, out-of-bounds, and both-bad pointers.
- Interpreter and AOT awareness.
- A regression-test mindset similar to real runtime bug reports.
- Good README with commands that can be copied and run.

## Hints

- Keep the guest module tiny. The complexity should be in the host validation and test script, not in Wasm logic.
- Use a mutable global for the shadow stack pointer so the WAT resembles compiler-generated stack-pointer patterns.
- The host import can return an integer status code instead of throwing a trap if that is easier to test. Just make the behavior explicit.
- If you choose to raise a WasmEdge host-function error, make sure your runner still prints a readable classification.
- Use `timeout` in the shell script to turn a hang into a failing test.
- Add a `--case` option to the runner if calling every exported function in one process makes error handling awkward.

## Practice Value

This exercise practices WasmEdge C API imports, guest memory validation, pointer alignment checks, regression-test design, interpreter versus AOT testing, and understanding trap versus out-of-bounds behavior.

# Recommended Preparation Plan

## Day 1: WAT and Memory Basics

- Solve Practice Exercise 1.
- Review WebAssembly memory pages, little-endian numeric loads, `data` segments, and exported reactor functions.
- Write a short note explaining why an unaligned `i32.load` is not the same as a misaligned guest pointer passed to a host API.

## Day 2: WasmEdge C API Guest Memory Access

- Solve Practice Exercise 2.
- Focus on loading a module, invoking exported functions, getting exported memory, and safely reading guest memory from C++.
- Add clear error messages for misalignment and out-of-bounds cases.

## Day 3: Binary and Runtime Test Habits

- Solve Practice Exercise 3.
- Make the test runner robust: expected-invalid cases should pass the suite when they fail validation.
- Keep each WAT test minimal and named after the behavior it checks.

## Day 4: Runtime Logic in C++

- Solve Practice Exercise 4.
- Run the tests with sanitizers if available.
- Make sure no code path relies on undefined behavior or CPU-specific unaligned access support.

## Day 5: End-to-end Regression Practice

- Solve Practice Exercise 5.
- Run both interpreter and AOT paths if available.
- Review your READMEs and command outputs. A real pre-test submission should be easy for an evaluator to build, run, and inspect without guessing.

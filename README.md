# ase-treesitter-adapter

[![Layer](https://img.shields.io/badge/Adapter-Tree--sitter-yellow.svg)]()
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)]()

> Header-only C++ wrapper around tree-sitter grammar entry-points.
> Isolates the `extern "C"` linkage required by tree-sitter so the rest of
> the ASE codebase stays pure C++.

Part of [ASE - Antares Simulation Engine](../..)

## Why this exists

Tree-sitter is a pure C library. Its grammar libraries (`ts-grammar-cpp`,
`ts-grammar-python`, …) export C functions like `tree_sitter_cpp()` that
return a `const TSLanguage*` pointer. Calling these from C++ requires
declaring them with `extern "C"` so the C++ compiler doesn't mangle the
symbol names.

The ASE validator (`core/ase-validator/ecs_validator`) forbids `extern "C"`
project-wide via the `EXTERN_C_FORBIDDEN` rule. The intent is to keep ASE
a pure C++ codebase: no accidental C-linkage creep, every dep that is
truly C-only must be encapsulated.

This adapter is the **single allowed home** for the tree-sitter C linkage.
The path `adapter/ase-treesitter-adapter/` is whitelisted in
`core/ase-validator/ecs_validator/data/third_party_oop.json` for the
`EXTERN_C_FORBIDDEN` rule. Consumers include this header and call
`ase::treesitter::grammar_*()` — pure C++ in the caller's translation unit.

## API

```cpp
#include <ase/treesitter/grammars.hpp>
#include <tree_sitter/api.h>  // for the actual TSLanguage / TSParser API

TSParser* p = ts_parser_new();
ts_parser_set_language(p, ase::treesitter::grammar_cpp());
TSTree* tree = ts_parser_parse_string(p, nullptr, source, source_len);
```

Available grammars:

| Function                              | Tree-sitter symbol         |
|---------------------------------------|----------------------------|
| `ase::treesitter::grammar_cpp()`      | `tree_sitter_cpp()`        |
| `ase::treesitter::grammar_c()`        | `tree_sitter_c()`          |
| `ase::treesitter::grammar_python()`   | `tree_sitter_python()`     |
| `ase::treesitter::grammar_bash()`     | `tree_sitter_bash()`       |
| `ase::treesitter::grammar_json()`     | `tree_sitter_json()`       |
| `ase::treesitter::grammar_typescript()` | `tree_sitter_typescript()` |

## Architecture

- **Header-only** — no `.cpp`, no link-time dependency on tree-sitter
  itself. The adapter just declares the C symbols and inline-wraps them.
- **Forward-declared `TSLanguage`** — consumers that actually parse code
  include `<tree_sitter/api.h>` themselves. The adapter header has no
  third-party include.
- **Symbol resolution** — the consumer (e.g. `ase-client-viewer`) is
  responsible for linking the actual `ts-grammar-*` static libraries. The
  inline wrappers in this adapter become unresolved external references
  during compilation; the consumer's link step provides them.

## Adding a new grammar

1. Add a new grammar library target to the consumer's `CMakeLists.txt`
   (FetchContent or system package).
2. Add a matching `extern "C"` declaration in `grammars.hpp` and a
   matching inline `grammar_xxx()` wrapper.
3. Regenerate `colors.hpp` is not needed — this adapter is colors-free.
4. Update this README's API table.

## Integration

### From root ASE build

`adapter/ase-treesitter-adapter/` is already wired into the root
`CMakeLists.txt`. Consumers just link the alias:

```cmake
target_link_libraries(my-client PRIVATE ase::treesitter)
```

### Standalone subproject

```cmake
if(NOT TARGET ase::treesitter)
    add_subdirectory(${ASE_ROOT}/adapter/ase-treesitter-adapter
                     ${CMAKE_BINARY_DIR}/ase-treesitter-adapter)
endif()
target_link_libraries(my-client PRIVATE ase::treesitter)
```

## Dependencies

- C++20
- A consumer-provided link environment that resolves the `tree_sitter_*`
  symbols (e.g. `ts-grammar-cpp`, `ts-grammar-c`, etc.).

## Versioning

`VERSION` file at the repository root is the SSOT. Format: `MM.mm.pp.BBBBB`.

## License

Proprietary — Antares Simulation Engine

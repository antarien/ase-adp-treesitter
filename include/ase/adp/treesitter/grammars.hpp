#pragma once

/**
 * ASE Tree-sitter Adapter — grammar lookup wrappers.
 *
 * Tree-sitter is a C library. Its grammar entry-points (tree_sitter_cpp(),
 * tree_sitter_python(), …) are C functions returning an opaque language
 * descriptor. Calling them from C++ requires an `extern "C"` block.
 *
 * The ASE validator forbids `extern "C"` in regular project code via the
 * EXTERN_C_FORBIDDEN rule — to keep the rest of the codebase pure C++.
 * This adapter is the SOLE allowed home for the tree-sitter C linkage:
 * it lives under adapter/ase-adp-treesitter/ which is whitelisted in
 * core/ase-validator/ecs_validator/data/third_party_oop.json.
 *
 * Consumers include this header instead of writing their own extern "C"
 * declarations, and call ase::adp::treesitter::grammar_*() — pure C++ in the
 * caller's translation unit. The actual grammar symbols are resolved at
 * link time from the consumer's tree-sitter grammar libraries.
 *
 * TSLanguage is forward-declared so this header has zero build-time
 * dependency on tree-sitter itself; consumers that go on to parse code
 * include <tree_sitter/api.h> separately.
 */

struct TSLanguage;

extern "C" {

const TSLanguage* tree_sitter_cpp();
const TSLanguage* tree_sitter_c();
const TSLanguage* tree_sitter_python();
const TSLanguage* tree_sitter_bash();
const TSLanguage* tree_sitter_json();
const TSLanguage* tree_sitter_typescript();

}  // extern "C"

namespace ase::adp::treesitter {

inline const TSLanguage* grammar_cpp()        { return ::tree_sitter_cpp();        }
inline const TSLanguage* grammar_c()          { return ::tree_sitter_c();          }
inline const TSLanguage* grammar_python()     { return ::tree_sitter_python();     }
inline const TSLanguage* grammar_bash()       { return ::tree_sitter_bash();       }
inline const TSLanguage* grammar_json()       { return ::tree_sitter_json();       }
inline const TSLanguage* grammar_typescript() { return ::tree_sitter_typescript(); }

}  // namespace ase::adp::treesitter

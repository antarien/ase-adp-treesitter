cmake_minimum_required(VERSION 3.22)

#[[
ASE TREESITTER ADAPTER BUILD CONFIGURATION

@file        CMakeLists.txt
@brief       Build configuration for ase-treesitter-adapter
@description Header-only adapter that wraps the C grammar entry-points of
             tree-sitter (tree_sitter_cpp(), tree_sitter_python(), …) behind
             ase::treesitter::grammar_*() inline functions. The extern "C"
             block lives only in this adapter's header; the rest of the
             codebase stays pure C++. Adapter path is whitelisted in
             core/ase-validator/ecs_validator/data/third_party_oop.json
             for the EXTERN_C_FORBIDDEN rule.

@module      ase-treesitter-adapter
@layer       adapter (3rd-party isolation, consumed by L5 clients)
@created     2026-04-14
@modified    2026-04-14
@version     00.00.01.00001 [stub]
]]

# Parse local VERSION (SSOT) if not building from root
if(NOT DEFINED ASE_TREESITTER_ADAPTER_VERSION)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION")
        message(FATAL_ERROR "VERSION file not found: ${CMAKE_CURRENT_SOURCE_DIR}/VERSION")
    endif()
    file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" _VERSION_LINES)
    foreach(_LINE ${_VERSION_LINES})
        if(_LINE MATCHES "^ASE_TREESITTER_ADAPTER=(.*)$")
            set(ASE_TREESITTER_ADAPTER_VERSION "${CMAKE_MATCH_1}")
        endif()
    endforeach()
    if(NOT DEFINED ASE_TREESITTER_ADAPTER_VERSION)
        message(FATAL_ERROR "ASE_TREESITTER_ADAPTER version not found in VERSION file")
    endif()
endif()

project(ase-treesitter-adapter VERSION ${ASE_TREESITTER_ADAPTER_VERSION} LANGUAGES CXX)

message(STATUS "Configuring ${PROJECT_NAME} v${PROJECT_VERSION}")

# ──────────────────────────────────────────────────────────────────────
# Header-only library
# ──────────────────────────────────────────────────────────────────────
# No source files, no link dependencies. The grammar symbols are resolved
# at the consumer's link step against the consumer's own tree-sitter
# grammar libraries (ts-grammar-cpp, ts-grammar-c, …). TSLanguage is
# forward-declared so this header doesn't even need <tree_sitter/api.h>.
add_library(${PROJECT_NAME} INTERFACE)
add_library(ase::treesitter ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
)

install(DIRECTORY include/ase
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

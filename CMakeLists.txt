# File templated from https://github.com/cmu-db/noisepage/

# Resources for learning about _modern_ CMake:
# - https://llvm.org/docs/CMakePrimer.html LLVM's CMake guide. Start here.
# - https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1 Gist: This is essentially Pfeifer's talk.
# - https://www.youtube.com/watch?v=bsXLMQ6WgIk Talk: C++Now 2017 Daniel Pfeifer "Effective CMake".
# - https://www.youtube.com/watch?v=eC9-iRN2b04 Talk: CppCon2017 Mathieu Ropert "Using Modern CMake Patterns ...".
# - https://cliutils.gitlab.io/modern-cmake/    Book: Modern CMake. Some examples are no longer modern.
# - The official cmake.org site is full of outdated anti-patterns. Use for documentation, not for inspiration.

# Also, if you use CMake syntax for multiline comments, a kitten dies.
# If you didn't know that CMake has multiline comments, good.

# Explanations of various CMake quirks here.

#   1. A crash course on CMake's GLOB and GLOB_RECURSE.
#       CMake is not a build system -- CMake will not build your files.
#       CMake is a build system generator -- CMake will generate something (make, ninja, etc.) that builds your files.
#
#       How do you specify what files should get built?
#       - Well, you can either specify every single file manually, which is as horrible as it sounds. "Best practice".
#       - The old and not-recommended way is to use a GLOB, which creates a list of files.
#             Tangent: In CMake, a list is merely a ;-separated string. State of the art 2020 technology right there.
#
#       The problem with GLOB and GLOB_RECURSE is that the files are known to the build system _generator_, rather than
#       the build system. So you run GLOB and give a list of files to CMake. CMake hardcodes all these files when CMake
#       generates your build system. Later, you add a new .h or .cpp file, and your build system doesn't pick it up,
#       because the build system needs to be regenerated! Incidentally, CLion has a File > Reload CMake Project button
#       handy for this reason. To summarize the problem: "if you're hardcoding a list of files into your build system,
#       you're not going to pick up new files".
#
#       Well, specifying every single source file in CMake still sounds like a pain. Instead, the band-aid hack "modern"
#       solution is to specify CONFIGURE_DEPENDS. This means "if the build system supports this feature, if anything in
#       the GLOB changes, rerun CMake". In practice, our build system is Ninja or Make, and both support this.
#
#   2. Footgun warning: target_include_directories.
#       You must specify SYSTEM includes (typically disables warnings and errors from those includes) separately from
#       non-SYSTEM. In other words, always do
#           target_include_directories(target_name PUBLIC foo INTERFACE bar PRIVATE baz)
#           target_include_directories(target_name SYSTEM PUBLIC foo INTERFACE bar PRIVATE baz)
#       and never do
#           target_include_directories(target_name PUBLIC foo SYSTEM PRIVATE blah)
#       because the latter does not do what you think it does.
#
#   3. Footgun warning: FetchProject_ and friends automatically lowercase the project name.
#       If you're looking for ${someProject_SOURCE_DIR}, that capital letter will ruin your day.
#       Try doing string(TOLOWER ${someProject} someProject_LOWER) and using ${${someProject_LOWER}_SOURCE_DIR} instead.
#      
# Organization of this file. (You can Ctrl-F for these!)
#   HEADER Project definition.
#   HEADER Safety checks.
#   HEADER System info.
#   HEADER jucc libraries.
#   HEADER jucc binary.
#   HEADER Tests.
#   HEADER Benchmarks.
#   HEADER Generated file destinations.
#   HEADER Miscellaneous.

#######################################################################################################################
# HEADER Project definition.
#######################################################################################################################

# Ubuntu 20.04 ships with CMake version 3.16.3. But CI on Mac is outdated and picking up 3.15.
cmake_minimum_required(VERSION 3.16)

# CMake has extremely limited support for multi-line string literals. Therefore an explicit CONCAT call is used here.
string(
        CONCAT
        JUCC_DESCRIPTION
        "JuCC"
)

# Set the name of the CMake project to JuCC. This also automatically defines various magic variables,
# such as ${PROJECT_SOURCE_DIR} and ${PROJECT_BINARY_DIR}.
project(
        JuCC
        # The version number of the JuCC project.
        # The individual components can be extracted with ${PROJECT_VERSION_MAJOR}, ${PROJECT_VERSION_MINOR},
        # ${PROJECT_VERSION_PATCH}, and ${PROJECT_VERSION_TWEAK} respectively.
        VERSION 1.0.0.0
        DESCRIPTION "${JUCC_DESCRIPTION}"
        # JuCC is a C++ project.
        LANGUAGES CXX
)

# Create a compile_commands.json file that can be easily parsed by build tools, clang-tidy, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#######################################################################################################################
# HEADER Safety checks.
#######################################################################################################################

# People keep running CMake in the wrong folder, completely nuking their project or creating weird bugs.
# This checks if you're running CMake from a folder that already has CMakeLists.txt.
# Importantly, this catches the common case of running it from the root directory.
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" PATH_TO_CMAKELISTS_TXT)
if (EXISTS "${PATH_TO_CMAKELISTS_TXT}")
    message(FATAL_ERROR "Run CMake from a build subdirectory! \"mkdir build ; cd build ; cmake ..\" \
    Some junk files were created in this folder (CMakeCache.txt, CMakeFiles); you should delete those.")
endif ()

#######################################################################################################################
# HEADER System info.
#######################################################################################################################

# Print a welcome message with the project's version number.
message(STATUS
        "Welcome to JuCC!\n\
        Version: ${PROJECT_VERSION}\n")

# CMAKE_MODULE_PATH is the search path for the include() and find_package() CMake commands.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR}")

function(print_sys_info QUERY_TARGET)
    cmake_host_system_information(RESULT SYS_INFO QUERY ${QUERY_TARGET})
    message(STATUS "  System ${QUERY_TARGET}: ${SYS_INFO}")
endfunction()
print_sys_info("NUMBER_OF_LOGICAL_CORES;NUMBER_OF_PHYSICAL_CORES")
print_sys_info("HOSTNAME;FQDN")
print_sys_info("AVAILABLE_VIRTUAL_MEMORY;TOTAL_VIRTUAL_MEMORY")
print_sys_info("AVAILABLE_PHYSICAL_MEMORY;TOTAL_PHYSICAL_MEMORY")
print_sys_info("IS_64BIT;HAS_IA64")
print_sys_info("HAS_FPU;HAS_MMX;HAS_MMX_PLUS")
print_sys_info("HAS_SSE;HAS_SSE2;HAS_SSE_FP;HAS_SSE_MMX")
print_sys_info("HAS_AMD_3DNOW;HAS_AMD_3DNOW_PLUS")
print_sys_info("HAS_SERIAL_NUMBER;PROCESSOR_SERIAL_NUMBER")
print_sys_info("PROCESSOR_NAME;PROCESSOR_DESCRIPTION")
print_sys_info("OS_NAME;OS_RELEASE;OS_VERSION;OS_PLATFORM")

#######################################################################################################################
# HEADER CMake options and global variables.
# CMake build types, specify with -DCMAKE_BUILD_TYPE={option}.
#   Debug (default), Release, RelWithDebInfo, FastDebug.
#   In practice people only use Debug or Release.
#######################################################################################################################

# Default to DEBUG builds if -DCMAKE_BUILD_TYPE was not specified.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif (NOT CMAKE_BUILD_TYPE)

# Everything else in this section will populate the following global variables.
set(JUCC_COMPILE_OPTIONS "")
set(JUCC_LINK_LIBRARIES "")
set(JUCC_LINK_OPTIONS "")
set(JUCC_INCLUDE_DIRECTORIES "")

# Add compilation flags to JUCC_COMPILE_OPTIONS based on the current CMAKE_BUILD_TYPE.
string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    list(APPEND JUCC_COMPILE_OPTIONS "-ggdb" "-O0" "-fno-omit-frame-pointer" "-fno-optimize-sibling-calls" "--coverage")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "FASTDEBUG")
    list(APPEND JUCC_COMPILE_OPTIONS "-ggdb" "-O1" "-fno-omit-frame-pointer" "-fno-optimize-sibling-calls")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
    list(APPEND JUCC_COMPILE_DEFINITIONS "-DNDEBUG")
    list(APPEND JUCC_COMPILE_OPTIONS "-O3")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RELWITHDEBINFO")
    list(APPEND JUCC_COMPILE_DEFINITIONS "-DNDEBUG")
    list(APPEND JUCC_COMPILE_OPTIONS "-ggdb" "-O2")
else ()
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif ()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

#######################################################################################################################
# HEADER JuCC libraries.
# jucc_objlib      :   JuCC object library, built once and linked into both static and shared targets.
# jucc_static      :   All of JuCC functionality exposed as a static library.
# jucc_shared      :   All of JuCC functionality exposed as a shared library.
#######################################################################################################################

# Get the list of all JuCC sources.
file(GLOB_RECURSE
        JUCC_SRCS                       # Store the list of files into the variable ${JUCC_SRCS}.
        CONFIGURE_DEPENDS               # See above. Ask CMake to regenerate the build system if these files change.
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/include/*.h
    )

# Remove the main program from JuCC sources.
list(REMOVE_ITEM JUCC_SRCS ${PROJECT_SOURCE_DIR}/src/main/main.cpp)

# Build JuCC as an OBJECT library first, i.e., a .o file per corresponding .cpp file.
# The OBJECT library is built first so that the same .o files can be linked into static and shared libraries.
# This allows both jucc_static and jucc_shared to be built with a single compilation of translation units.
add_library(jucc_objlib OBJECT ${JUCC_SRCS})

set_target_properties(jucc_objlib PROPERTIES
        POSITION_INDEPENDENT_CODE ON                # Required for static linking into other shared libraries.
        CXX_EXTENSIONS OFF                          # Disable compiler extensions (e.g., use c++17 not gnu17).
        )
target_compile_definitions(jucc_objlib PUBLIC       # PUBLIC: all consumers of the library inherit the following.
        ${JUCC_COMPILE_DEFINITIONS}
        )
target_compile_options(jucc_objlib PRIVATE          # PRIVATE: only jucc_objlib uses the following.
        "-Werror"                                   # Treat warnings as errors.
        "-Wall"                                     # Enable "all" warnings. (Not actually all warnings.)
        )
target_compile_options(jucc_objlib PUBLIC           # PUBLIC: all consumers of the library inherit the following.
        "-march=native"                             # Enable machine-specific instruction sets and optimizations.
        "-mcx16"                                    # Allow CMPXCHG16B (16-byte compare and exchange).
        ${JUCC_COMPILE_OPTIONS}
        )
target_compile_features(jucc_objlib PUBLIC          # PUBLIC: all consumers of the library inherit the following.
        cxx_std_17                                  # Require support for C++17.
        )
target_include_directories(jucc_objlib PUBLIC       # PUBLIC: all consumers of the library inherit the following.
        ${PROJECT_SOURCE_DIR}/src/include           # Include JuCC src/include/ headers.
        )
target_link_options(jucc_objlib PUBLIC              # PUBLIC: all consumers of the library inherit the following.
        ${JUCC_LINK_OPTIONS}
        )

# Create the jucc_static and jucc_shared libraries using the objects from jucc_objlib.
add_library(jucc_static STATIC $<TARGET_OBJECTS:jucc_objlib>)   # Bundle up these objects into static lib.
target_link_libraries(jucc_static PUBLIC jucc_objlib)           # Consumers will inherit this link.
target_compile_options(jucc_static PUBLIC                       # PUBLIC: all consumers of the library inherit the following.
        "-fvisibility=hidden"                                   # Hide symbols by default.
        )
target_link_options(jucc_static PUBLIC              # PUBLIC: all consumers of the library inherit the following.
        "-fvisibility=hidden"                       # Hide symbols by default.
        )

#######################################################################################################################
# HEADER jucc binary.
# jucc             :   The main compiler binary.
#######################################################################################################################

add_executable(jucc src/main/main.cpp)
target_compile_options(jucc PRIVATE "-Werror" "-Wall")
target_link_libraries(jucc jucc_static --coverage)

#######################################################################################################################
# HEADER Tests.
#######################################################################################################################

# ctest
enable_testing()

# gmock gtest https://crascit.com/2015/07/25/cmake-gtest/
configure_file("${PROJECT_SOURCE_DIR}/build_support/gtest_CMakeLists.txt.in" googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download")
if(result)
        message(FATAL_ERROR "CMake step for gtest failed: ${result}")
endif()
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download")
if(result)
        message(FATAL_ERROR "Build step for gtest failed: ${result}")
endif()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # don't override our compiler/linker options when building gtest
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src" "${CMAKE_BINARY_DIR}/googletest-build" EXCLUDE_FROM_ALL)

# Get the list of all JuCC tests.
file(GLOB_RECURSE
        JUCC_TEST_SOURCES                       # Store the list of files into the variable ${JUCC_TEST_SOURCES}.
        CONFIGURE_DEPENDS                       # See above. Ask CMake to regenerate the build system if these files change.
        ${PROJECT_SOURCE_DIR}/test/*.cpp
    )

add_executable(jucc_test ${JUCC_TEST_SOURCES})

add_test(NAME jucc_test COMMAND jucc_test)

target_link_libraries(jucc_test PUBLIC jucc_objlib gtest --coverage)

#######################################################################################################################
# HEADER Benchmarks.
#######################################################################################################################

set(BENCHMARK_ENABLE_TESTING OFF)

configure_file("${PROJECT_SOURCE_DIR}/build_support/benchmark_CMakeLists.txt.in" benchmark-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/benchmark-download")
if(result)
        message(FATAL_ERROR "CMake step for benchmark failed: ${result}")
endif()
execute_process(COMMAND "${CMAKE_COMMAND}" --build . --config "Release"
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/benchmark-download")
if(result)
        message(FATAL_ERROR "Build step for benchmark failed: ${result}")
endif()
set(benchmark_force_shared_crt ON CACHE BOOL "" FORCE)  # don't override our compiler/linker options when building benchmark
add_subdirectory("${CMAKE_BINARY_DIR}/benchmark-src" "${CMAKE_BINARY_DIR}/benchmark-build")

# Get the list of all JuCC benchmarks.
file(GLOB_RECURSE
        JUCC_BENCHMARK_SOURCES                          # Store the list of files into the variable ${JUCC_BENCHMARK_SOURCES}.
        CONFIGURE_DEPENDS                               # See above. Ask CMake to regenerate the build system if these files change.
        ${PROJECT_SOURCE_DIR}/benchmark/*.cpp
    )

add_executable(jucc_benchmark ${JUCC_BENCHMARK_SOURCES})

target_link_libraries(jucc_benchmark PUBLIC jucc_objlib benchmark --coverage)

#######################################################################################################################
# HEADER Generated file destinations.
#######################################################################################################################

set_target_properties(
        jucc_static
        jucc
        jucc_test
        jucc_benchmark
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

#######################################################################################################################
# HEADER Miscellaneous.
# This is where helper scripts and other random CMake stuff goes.
#######################################################################################################################

#######################################################################################################################
# Global variables for helper scripts.
# LINT_FILES    :   All the files that are to be linted. Exposed since other tools may want to use them.
# CLANG_TOOLS_SEARCH_PATH   :   Where to look for clang programs like clang-format and clang-tidy.
#######################################################################################################################

set(cpplint_SOURCE_DIR "${CMAKE_SOURCE_DIR}/build_support")
set(BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")

file(GLOB_RECURSE LINT_FILES
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/test/*.h"
        "${PROJECT_SOURCE_DIR}/test/*.cpp"
        "${PROJECT_SOURCE_DIR}/benchmark/*.h"
        "${PROJECT_SOURCE_DIR}/benchmark/*.cpp"
        )

set(CLANG_TOOLS_SEARCH_PATH
        "/usr/local/bin"
        "/usr/bin"
        "/usr/local/opt/llvm/bin"
        )

# clang-format
find_program(CLANG_FORMAT_BIN
        NAMES clang-format-11
        HINTS ${CLANG_TOOLS_SEARCH_PATH})
if ("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
    message(WARNING "JuCC/main couldn't find clang-format.")
else()
    message(STATUS "JuCC/main found clang-format at ${CLANG_FORMAT_BIN}")
endif()

# clang-tidy
find_program(CLANG_TIDY_BIN
        NAMES clang-tidy-11
        HINTS ${CLANG_TOOLS_SEARCH_PATH})
if ("${CLANG_TIDY_BIN}" STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
    message(WARNING "JuCC/main couldn't find clang-tidy.")
else()
    # Output compile_commands.json
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
    message(STATUS "JuCC/main found clang-tidy at ${CLANG_TIDY_BIN}")
endif()

# cpplint
find_program(CPPLINT_BIN
        NAMES cpplint cpplint.py
        HINTS ${CLANG_TOOLS_SEARCH_PATH})
if ("${CPPLINT_BIN}" STREQUAL "CPPLINT_BIN-NOTFOUND")
    message(WARNING "JuCC/main couldn't find cpplint.")
else()
    message(STATUS "JuCC/main found cpplint at ${CPPLINT_BIN}")
endif()

#######################################################################################################################
# check-lint        :   Run the cpplint python script.
#######################################################################################################################

find_program(CPPLINT_BIN NAMES cpplint.py HINTS ${cpplint_SOURCE_DIR})

if ("${CPPLINT_BIN}" STREQUAL "CPPLINT_BIN-NOTFOUND")
    message(STATUS "[MISSING] cpplint at ${cpplint_SOURCE_DIR}/cpplint.py, no check-lint.")
else ()
    # Balancing act: cpplint.py takes a non-trivial time to launch, so process 12 files per invocation with parallelism.
    add_custom_target(check-lint
            COMMENT "Running: echo LINT_FILES | xargs -n12 -P8 python3 ${CPPLINT_BIN} --verbose=2 --linelength=120 --quiet --filter=legal/copyright,-build/header_guard"
            COMMAND echo '${LINT_FILES}' | xargs -n12 -P8
            python3 ${CPPLINT_BIN}
            --verbose=2 --linelength=120 --quiet
            --filter=-legal/copyright,-build/header_guard
            USES_TERMINAL
            )
    message(STATUS "[ADDED] check-lint (${CPPLINT_BIN})")
endif ()
unset(${CPPLINT_BIN})

#######################################################################################################################
# format            :   Reformat the codebase according to standards.
# check-format      :   Check if the codebase is formatted according to standards.
#######################################################################################################################

find_program(CLANG_FORMAT_BIN NAMES clang-format-11 HINTS ${CLANG_TOOLS_SEARCH_PATH})
if ("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
    message(STATUS "[MISSING] clang-format not found, no format and no check-format.")
else ()
    # The directories to be formatted. Note that we modified the format script to take in multiple arguments.
    string(CONCAT FORMAT_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/benchmark,"
            "${CMAKE_CURRENT_SOURCE_DIR}/src,"
            "${CMAKE_CURRENT_SOURCE_DIR}/test,"
            )

    # Run clang-format and update files in place.
    add_custom_target(format
            ${BUILD_SUPPORT_DIR}/run_clang_format.py
            ${CLANG_FORMAT_BIN}
            ${BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
            --source_dirs
            ${FORMAT_DIRS}
            --fix
            --quiet
            USES_TERMINAL
            )

    # Run clang-format and exit with a non-zero exit code if any files need to be reformatted.
    add_custom_target(check-format
            ${BUILD_SUPPORT_DIR}/run_clang_format.py
            ${CLANG_FORMAT_BIN}
            ${BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
            --source_dirs
            ${FORMAT_DIRS}
            --quiet
            USES_TERMINAL
            )

    message(STATUS "[ADDED] clang-format and check-clang-format (${CLANG_FORMAT_BIN})")

    unset(FORMAT_DIRS)
endif ()
unset(CLANG_FORMAT_BIN)

#######################################################################################################################
# check-clang-tidy  :   Run clang-tidy static analysis on the codebase.
#######################################################################################################################

find_program(CLANG_TIDY_BIN NAMES clang-tidy-11 HINTS ${CLANG_TOOLS_SEARCH_PATH})
if ("${CLANG_TIDY_BIN}" STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
    message(STATUS "[MISSING] clang-tidy not found, no check-clang-tidy.")
else ()
    # Run clang-tidy and exit with a non-zero exit code if any errors are found.
    # clang-tidy automatically searches for a .clang-tidy file in parent directories.
    add_custom_target(check-clang-tidy
            ${BUILD_SUPPORT_DIR}/run_clang_tidy.py                            # Run LLVM's clang-tidy script.
            -clang-tidy-binary ${CLANG_TIDY_BIN}                              # Using the specified clang-tidy binary.
            -p ${CMAKE_BINARY_DIR}                                            # Using the generated compile commands.
            USES_TERMINAL
            )
    add_custom_command(TARGET check-clang-tidy DEPENDS benchmark gflags gtest)
    message(STATUS "[ADDED] check-clang-tidy (${CLANG_TIDY_BIN})")
endif ()
unset(CLANG_TIDY_BIN)

#######################################################################################################################
# Apply +x permissions to all scripts in the build-support folder.
#######################################################################################################################

file(GLOB_RECURSE
        BUILD_SUPPORT_SCRIPTS
        CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/build_support/*.pl
        ${PROJECT_SOURCE_DIR}/build_support/*.py
        ${PROJECT_SOURCE_DIR}/build_support/*.sh
        )

foreach (_var IN LISTS BUILD_SUPPORT_SCRIPTS)
    execute_process(COMMAND chmod +x "${_var}")
endforeach ()

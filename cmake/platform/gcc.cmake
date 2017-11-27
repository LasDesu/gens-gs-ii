# gcc (and other Unix-like compilers, e.g. MinGW)

# Compiler flag modules.
INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCXXCompilerFlag)

# Check what flag is needed for C99 support.
INCLUDE(CheckC99CompilerFlag)
CHECK_C99_COMPILER_FLAG(GENS_C99_CFLAG)

# Check what flag is needed for C++ 2011 support.
INCLUDE(CheckCXX11CompilerFlag)
CHECK_CXX11_COMPILER_FLAG(GENS_CXX11_CXXFLAG)

# Disable C++ RTTI.
INCLUDE(CheckCXXNoRTTICompilerFlag)
CHECK_CXX_NO_RTTI_COMPILER_FLAG(GENS_CXX_NO_RTTI_CXXFLAG)

# Disable C++ exceptions.
INCLUDE(CheckCXXNoExceptionsCompilerFlag)
CHECK_CXX_NO_EXCEPTIONS_COMPILER_FLAG(GENS_CXX_NO_EXCEPTIONS_CXXFLAG)

SET(GENS_C_FLAGS_COMMON "${GENS_C99_CFLAG}")
SET(GENS_CXX_FLAGS_COMMON "${GENS_CXX11_CXXFLAG} ${GENS_CXX_NO_RTTI_CXXFLAG} ${GENS_CXX_NO_EXCEPTIONS_CXXFLAG}")
SET(GENS_EXE_LINKER_FLAGS_COMMON "")
UNSET(GENS_C99_CFLAG)
UNSET(GENS_CXX11_CXXFLAG)
UNSET(GENS_CXX_NO_RTTI_CXXFLAG)
UNSET(GENS_CXX_NO_EXCEPTIONS_CXXFLAG)

# Check for link-time optimization.
# NOTE: LTO on MinGW is broken right now:
# `___mingw_raise_matherr' referenced in section `.text' of
# ../lib/libmingwex.a(lib32_libmingwex_a-log.o): defined in
# discarded section `.text' of lib32_libmingw32_a-merr.o (symbol from plugin)
IF(ENABLE_LTO)
	# We need two wrappers in order for LTO to work properly:
	# - gcc-ar: static library archiver
	# - gcc-ranlib: static library indexer
	# Without these wrappers, all sorts of undefined refernce errors
	# occur in gcc-4.9 due to "slim" LTO objects, and possibly
	# earlier versions for various reasons.
	IF("${CMAKE_AR}" MATCHES "gcc-ar$")
		# Already using the gcc-ar wrapper.
		SET(GCC_WRAPPER_AR "${CMAKE_AR}")
	ELSE()
		# Replace ar with gcc-ar.
		STRING(REGEX REPLACE "ar$" "gcc-ar" GCC_WRAPPER_AR "${CMAKE_AR}")
	ENDIF()
	IF("${CMAKE_RANLIB}" MATCHES "gcc-ranlib$")
		# Already using the gcc-ranlib wrapper.
		SET(GCC_WRAPPER_RANLIB "${CMAKE_RANLIB}")
	ELSE()
		# Replace ranlib with gcc-ranlib.
		STRING(REGEX REPLACE "ranlib$" "gcc-ranlib" GCC_WRAPPER_RANLIB "${CMAKE_RANLIB}")
	ENDIF()

	IF(EXISTS "${GCC_WRAPPER_AR}" AND EXISTS "${GCC_WRAPPER_RANLIB}")
		# Found gcc binutils wrappers.
		SET(CMAKE_AR "${GCC_WRAPPER_AR}")
		SET(CMAKE_RANLIB "${GCC_WRAPPER_RANLIB}")
		SET(HAS_BINUTILS_WRAPPERS 1)
	ELSE()
		MESSAGE(FATAL_ERROR "gcc binutils wrappers not found; cannot enable LTO.")
	ENDIF()

	IF(HAS_BINUTILS_WRAPPERS)
		CHECK_C_COMPILER_FLAG("-flto" CFLAG_LTO)
		IF(CFLAG_LTO)
			SET(GENS_CFLAGS_LTO "-flto")
			SET(GENS_LDFLAGS_LTO "${GENS_CFLAGS_LTO} -fuse-linker-plugin")
		ENDIF(CFLAG_LTO)
		UNSET(CFLAG_LTO)
	ENDIF(HAS_BINUTILS_WRAPPERS)
ENDIF(ENABLE_LTO)
SET(GENS_C_FLAGS_COMMON "${GENS_C_FLAGS_COMMON} ${GENS_CFLAGS_LTO}")
SET(GENS_CXX_FLAGS_COMMON "${GENS_CXX_FLAGS_COMMON} ${GENS_CFLAGS_LTO}")
SET(GENS_EXE_LINKER_FLAGS_COMMON "${GENS_LINKER_FLAGS_COMMON} ${GENS_LDFLAGS_LTO}")
UNSET(GENS_CFLAGS_LTO)
UNSET(GENS_LDFLAGS_LTO)

# Test for common CFLAGS and CXXFLAGS.
FOREACH(FLAG_TEST "-Wall" "-Wextra" "-fstrict-aliasing" "-fvisibility=hidden" "-Wno-multichar")
	CHECK_C_COMPILER_FLAG("${FLAG_TEST}" CFLAG_${FLAG_TEST})
	IF(CFLAG_${FLAG_TEST})
		SET(GENS_C_FLAGS_COMMON "${GENS_C_FLAGS_COMMON} ${FLAG_TEST}")
	ENDIF(CFLAG_${FLAG_TEST})
	UNSET(CFLAG_${FLAG_TEST})
	
	CHECK_CXX_COMPILER_FLAG("${FLAG_TEST}" CXXFLAG_${FLAG_TEST})
	IF(CXXFLAG_${FLAG_TEST})
		SET(GENS_CXX_FLAGS_COMMON "${GENS_CXX_FLAGS_COMMON} ${FLAG_TEST}")
	ENDIF(CXXFLAG_${FLAG_TEST})
	UNSET(CXXFLAG_${FLAG_TEST})
ENDFOREACH()

# Test for common CXXFLAGS.
FOREACH(FLAG_TEST "-fvisibility-inlines-hidden")
	CHECK_CXX_COMPILER_FLAG("${FLAG_TEST}" CXXFLAG_${FLAG_TEST})
	IF(CXXFLAG_${FLAG_TEST})
		SET(GENS_CXX_FLAGS_COMMON "${GENS_CXX_FLAGS_COMMON} ${FLAG_TEST}")
	ENDIF(CXXFLAG_${FLAG_TEST})
	UNSET(CXXFLAG_${FLAG_TEST})
ENDFOREACH()

# Test for common LDFLAGS.
# TODO: Doesn't work on OS X. (which means it's not really testing it!)
IF(NOT APPLE)
	FOREACH(FLAG_TEST "-Wl,-O1" "-Wl,--sort-common" "-Wl,--as-needed")
		CHECK_C_COMPILER_FLAG("${FLAG_TEST}" LDFLAG_${FLAG_TEST})
		IF(LDFLAG_${FLAG_TEST})
			SET(GENS_EXE_LINKER_FLAGS_COMMON "${GENS_EXE_LINKER_FLAGS_COMMON} ${FLAG_TEST}")
		ENDIF(LDFLAG_${FLAG_TEST})
		UNSET(LDFLAG_${FLAG_TEST})
	ENDFOREACH()
ENDIF(NOT APPLE)
SET(GENS_SHARED_LINKER_FLAGS_COMMON "${GENS_EXE_LINKER_FLAGS_COMMON}")

# Architecture-specific CFLAGS/CXXFLAGS.
IF(GENS_CPU_X86_32)
	# x86 32-bit: Compile for Pentium Pro or later.
	SET(GENS_C_FLAGS_COMMON "${GENS_C_FLAGS_COMMON} -march=i686")
	SET(GENS_CXX_FLAGS_COMMON "${GENS_CXX_FLAGS_COMMON} -march=i686")
ENDIF(GENS_CPU_X86_32)

# Check for -Og.
# This flag was added in gcc-4.8, and enables optimizations that
# don't interfere with debugging.
CHECK_C_COMPILER_FLAG("-Og" CFLAG_OPTIMIZE_DEBUG)
IF (CFLAG_OPTIMIZE_DEBUG)
	SET(CFLAG_OPTIMIZE_DEBUG "-Og")
ELSE(CFLAG_OPTIMIZE_DEBUG)
	SET(CFLAG_OPTIMIZE_DEBUG "-O0")
ENDIF(CFLAG_OPTIMIZE_DEBUG)

# Debug/release flags.
SET(GENS_C_FLAGS_DEBUG		"${CFLAG_OPTIMIZE_DEBUG} -ggdb -DDEBUG -D_DEBUG")
SET(GENS_CXX_FLAGS_DEBUG	"${CFLAG_OPTIMIZE_DEBUG} -ggdb -DDEBUG -D_DEBUG")
SET(GENS_C_FLAGS_RELEASE	"-O2 -ggdb -DNDEBUG")
SET(GENS_CXX_FLAGS_RELEASE	"-O2 -ggdb -DNDEBUG")

# Unset temporary variables.
UNSET(CFLAG_OPTIMIZE_DEBUG)

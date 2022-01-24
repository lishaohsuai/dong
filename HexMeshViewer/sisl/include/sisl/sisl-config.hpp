
/* Compile with debugging information and checks */
/* #undef ENABLE_DEBUG */
#ifdef ENABLE_DEBUG 
#define SISL_ENABLE_DEBUG 
#endif

/* Embed glue into the interpreter */
/* #undef ENABLE_EMBEDDED */
#ifdef ENABLE_EMBEDDED
/* #undef ENABLE_EMBEDDED */
#endif

/* Use C++ exception mechanism */
#define ENABLE_EXCEPTIONS
#ifdef ENABLE_EXCEPTIONS
#define SISL_ENABLE_EXCEPTIONS
#endif

/* With support for simd instructions */
/* #undef ENABLE_SIMD */
#ifdef SISL_ENABLE_SIMD
#define SISL_ENABLE_SIMD
#endif

/* With support for multi-threading */
/* #undef ENABLE_THREADS */
#ifdef ENABLE_THREADS
#define SISL_ENABLE_THREADS
#endif

/* Compile with unstable code */
/* #undef ENABLE_UNSTABLE */
#ifdef ENABLE_UNSTABLE
#define SISL_ENABLE_UNSTABLE
#endif 

/* Verify correctness of low-level operations */
/* #undef ENABLE_VERIFY */
#ifdef ENABLE_VERIFY
#define SISL_ENABLE_VERIFY
#endif

/* Define if you have the basix library */
/* #undef HAVE_BASIX */
#ifdef HAVE_BASIX
/* #undef SISL_HAVE_BASIX */
#endif

/* Define to 1 if you have the <cxxabi.h> header file. */
/* #undef HAVE_CXXABI_H */
#ifdef HAVE_CXXABI_H
#define SISL_HAVE_CXXABI_H 1
#endif

/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */
#ifdef HAVE_DLFCN_H
#define SISL_HAVE_DLFCN_H 1
#endif

/* Define to 1 if you have the <history.h> header file. */
/* #undef HAVE_HISTORY_H */
#ifdef HAVE_HISTORY_H
#define SISL_HAVE_HISTORY_H 1
#endif

/* Define to 1 if you have the <inttypes.h> header file. */
/* #undef HAVE_INTTYPES_H */
#ifdef HAVE_INTTYPES_H
#define SISL_HAVE_INTTYPES_H 1
#endif

/* Define to 1 if the system has the type `long long int'. */
#define HAVE_LONG_LONG_INT
#ifdef HAVE_LONG_LONG_INT
#define SISL_HAVE_LONG_LONG_INT 
#endif

/* Define to 1 if you have the <memory.h> header file. */
/* #undef HAVE_MEMORY_H */
#ifdef HAVE_MEMORY_H
#define SISL_HAVE_MEMORY_H 1
#endif


/* Define if you have the readline library */
/* #undef HAVE_READLINE */
#ifdef HAVE_READLINE 
#define SISL_HAVE_READLINE 1
#endif

/* Define to 1 if you have the <readline/history.h> header file. */
/* #undef HAVE_READLINE_HISTORY_H */
#ifdef HAVE_READLINE_HISTORY_H
#define SISL_HAVE_READLINE_HISTORY_H 1
#endif

/* Define to 1 if you have the <readline/readline.h> header file. */
/* #undef HAVE_READLINE_READLINE_H */
#ifdef HAVE_READLINE_READLINE_H
#define SISL_HAVE_READLINE_READLINE_H 1
#endif

/* Define to 1 if you have the <stdint.h> header file. */
/* #undef HAVE_STDINT_H */
#ifdef HAVE_STDINT_H
#define SISL_HAVE_STDINT_H 1
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
/* #undef HAVE_STDLIB_H */
#ifdef HAVE_STDLIB_H
#define SISL_HAVE_STDLIB_H 1
#endif


/* Define to 1 if you have the <strings.h> header file. */
/* #undef HAVE_STRINGS_H */
#ifdef HAVE_STRINGS_H
#define SISL_HAVE_STRINGS_H 1
#endif

/* Define to 1 if you have the <string.h> header file. */
/* #undef HAVE_STRING_H */
#ifdef HAVE_STRING_H
#define SISL_HAVE_STRING_H 1
#endif

/* Define to 1 if you have the <sys/stat.h> header file. */
/* #undef HAVE_SYS_STAT_H */
#ifdef HAVE_SYS_STAT_H
#define SISL_HAVE_SYS_STAT_H 1
#endif

/* Define to 1 if you have the <sys/types.h> header file. */
/* #undef HAVE_SYS_TYPES_H */
#ifdef HAVE_SYS_TYPES_H
#define SISL_HAVE_SYS_TYPES_H 1
#endif

/* Define to 1 if you have the `gettimeofday' function. */
/* #undef HAVE_GETTIMEOFDAY */
#ifdef HAVE_GETTIMEOFDAY
#define SISL_HAVE_GETTIMEOFDAY 1
#endif

/* Define to 1 if you have the <unistd.h> header file. */
/* #undef HAVE_UNISTD_H */
#ifdef HAVE_UNISTD_H
#define SISL_HAVE_UNISTD_H 1
#endif

/* Define to 1 if you have the ANSI C header files. */
/* #undef STDC_HEADERS */
#ifdef STDC_HEADERS
#define SISL_STDC_HEADERS 1
#endif

/* Prefix for the package */
#define SISL_PREFIX "/usr/local"

/* Name of package */
#define SISL_PACKAGE sisl

/* Define to the address where bug reports for this package should be sent. */
/* #undef SISL_PACKAGE_BUGREPORT */

/* Define to the full name of this package. */
/* #undef SISL_PACKAGE_NAME */

/* Define to the full name and version of this package. */
/* #undef SISL_PACKAGE_STRING */

/* Define to the one symbol short name of this package. */
/* #undef SISL_PACKAGE_TARNAME */

/* Define to the version of this package. */
/* #undef SISL_PACKAGE_VERSION */

/* Version number of package */
#define SISL_VERSION "0.1"


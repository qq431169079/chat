/* vim: et sw=4 sts=4 ts=4 : */
#ifndef CATCHAT_COMMON_HPP
#define CATCHAT_COMMON_HPP

/* ************************************************************************* *
 * Public API Visibility                                                     *
 * ************************************************************************* */
#if defined(_WIN32) || defined(__CYGWIN__)
#   define CATCHAT_SHARED_IMPORT __declspec(dllimport)
#   define CATCHAT_SHARED_EXPORT __declspec(dllexport)
#else /* WIN32 */
#   if __GNUC__ >= 4
#       define CATCHAT_SHARED_IMPORT __attribute__ ((visibility ("default")))
#       define CATCHAT_SHARED_EXPORT __attribute__ ((visibility ("default")))
#   else
#       define CATCHAT_SHARED_IMPORT
#       define CATCHAT_SHARED_EXPORT
#   endif /* __GNUC__ */
#endif

#ifdef CATCHAT_SHARED                             // Defined if using shared lib.
#   ifdef CATCHAT_SHARED_EXPORTS                  // Defined when building shock.
#       define CATCHAT_API CATCHAT_SHARED_EXPORT
#   else
#       define CATCHAT_API CATCHAT_SHARED_IMPORT
#   endif /* CATCHAT_SHARED_EXPORTS */
#else                                           // Must be a static library.
#   define CATCHAT_API
#endif /* CATCHAT_SHARED */

#endif /* CATCHAT_COMMON_HPP */

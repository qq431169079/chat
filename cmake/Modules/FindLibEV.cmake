find_path(LIBEV_INCLUDE_DIR NAMES ev.h)
find_library(LIBEV_LIBRARY NAMES ev)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibEV DEFAULT_MSG LIBEV_LIBRARY LIBEV_INCLUDE_DIR)

mark_as_advanced(LIBEV_INCLUDE_DIR LIBEV_LIBRARY)

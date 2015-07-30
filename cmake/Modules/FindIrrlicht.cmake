#  IRRLICHT_FOUND - System has LibXml2
#  IRRLICHT_INCLUDE_DIRS - The LibXml2 include directories
#  IRRLICHT_LIBRARIES - The libraries needed to use LibXml2
#  IRRLICHT_DEFINITIONS - Compiler switches required for using LibXml2

find_package(PkgConfig)
set(IRRLICHT_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})

find_path(IRRLICHT_DIR NAMES irrlicht.h
                PATHS "/usr/include/irrlicht/")

find_library(IRRLICHT_LIBRARY NAMES libIrrlicht.so
                PATHS "/usr/lib")

set(IRRLICHT_LIBRARIES ${IRRLICHT_LIBRARY} )
set(IRRLICHT_INCLUDE_DIRS ${IRRLICHT_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set IRRLICHT_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Irrlicht  DEFAULT_MSG
                                  IRRLICHT_LIBRARY IRRLICHT_DIR)

mark_as_advanced(IRRLICHT_INCLUDE_DIR IRRLICHT_LIBRARY )


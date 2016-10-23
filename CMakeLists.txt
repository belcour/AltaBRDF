cmake_minimum_required (VERSION 3.1)
project (AltaBRDF)

# Include directories, put ALTA first and MTS second to
# get ALTA's Eigen version.
include_directories ("$ENV{ALTA}/sources")
include_directories ("$ENV{ALTA}/external/build/include")
include_directories ("$ENV{MITSUBA_DIR}/include")
include_directories ("$ENV{MITSUBA_DIR}/dependencies/include")

# Set special defines flags
add_definitions( -DSINGLE_PRECISION -DSPECTRUM_SAMPLES=3)

# Link flags
find_library(ALTA_LIB   NAMES core
                        HINTS "$ENV{ALTA}"
                        PATH_SUFFIXES "sources/build" "build/core")
find_library(MTS_CORE   NAMES mitsuba-core
                        HINTS "$ENV{MITSUBA_DIR}"
                        PATH_SUFFIXES "Mitsuba.app/Contents/Frameworks" "dist")
find_library(MTS_RENDER NAMES mitsuba-render
                        HINTS "$ENV{MITSUBA_DIR}"
                        PATH_SUFFIXES "Mitsuba.app/Contents/Frameworks" "dist")
find_library(MTS_HW     NAMES mitsuba-hw
                        HINTS "$ENV{MITSUBA_DIR}"
                        PATH_SUFFIXES "Mitsuba.app/Contents/Frameworks" "dist")

find_path(EIGEN3_INCLUDE_DIR HINTS /usr/local/include/eigen3)
include_directories(${EIGEN3_INCLUDE_DIR})

add_library (alta_brdf SHARED brdf.cpp)
set_target_properties(alta_brdf PROPERTIES PREFIX "")

# Install directory (system dep)
install(TARGETS alta_brdf DESTINATION "$ENV{MITSUBA_DIR}/dist/plugins")

# Add C++11 features
target_compile_features(alta_brdf PRIVATE cxx_range_for)
target_link_libraries(alta_brdf ${ALTA_LIB} ${MTS_CORE} ${MTS_RENDER} ${MTS_HW})

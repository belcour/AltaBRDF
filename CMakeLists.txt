cmake_minimum_required (VERSION 2.8)
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
find_library(ALTA_LIB NAMES core 
                      HINTS "$ENV{ALTA}/sources/build}")
find_library(MTS_LIBS NAMES mitsuba-core mitsuba-hw mitsuba-render
                      HINTS "$ENV{MITSUBA_DIR}"
                      PATH_SUFFIXES "Mitsuba.app/Contents/Frameworks" "dist")


add_library (alta_brdf SHARED brdf.cpp)

# Add C++11 features
target_compile_features(alta_brdf PRIVATE cxx_range_for)
target_link_libraries(alta_brdf ${ALTA_LIB} ${MTS_LIBS})

project( toolsWithDGtal )
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Inclusion de DGtal
FIND_PACKAGE(DGtal REQUIRED)
MESSAGE(STATUS "Found DGtal: ${DGTAL_LIBRARIES}")
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})

#Inclusion de Boost
FIND_PACKAGE(Boost 1.46.0 COMPONENTS program_options)
MESSAGE(STATUS "Found Boost: ${Boost_LIBRARIES} ")
MESSAGE(STATUS "More precisely: ${Boost_PROGRAM_OPTIONS_LIBRARY} ")
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

# Make sure the compiler can find include files.
include_directories (inc)
# Read CMakeLists.txt in a subdirectory 
add_subdirectory(srcTools)


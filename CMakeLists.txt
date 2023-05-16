cmake_minimum_required(VERSION 3.13.4)
project(HE_Utility)
set(SOURCE_FILES src/main.cpp src/heutility.cpp src/about.cpp src/help.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_PREFIX_PATH "/usr/local/lib/cmake")
set(THREADS_PREFER_PTHREAD_FLAG ON)
add_compile_options(-Wno-reorder)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
option(BUILD_OPENFHE_STATIC "Set to ON to include static versions of the OpenFHE library" OFF)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost 1.67.0 COMPONENTS system date_time REQUIRED)
find_package(OpenFHE REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

## GTKMM
target_link_directories(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARIES_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE ${GTKMM_CFLAGS_OTHER})

## OpenFHE
set(CMAKE_CXX_FLAGS ${OpenFHE_CXX_FLAGS})
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenFHE_INCLUDE} ${OpenFHE_INCLUDE}/core ${OpenFHE_INCLUDE}/pke ${OpenFHE_INLCUDE}/third-party/include ${OPENMP_INCLUDES})
target_link_directories(${PROJECT_NAME} PRIVATE ${OpenFHE_LIBDIR} ${OPENMP_LIBRARIES})

if(BUILD_OPENFHE_STATIC)
    set(CMAKE_EXE_LINKER_FLAGS "${OpenFHE_EXE_LINKER_FLAGS} -static")
    target_link_libraries(${PROJECT_NAME} ${OpenFHE_STATIC_LIBRARIES})
else()
    set(CMAKE_EXE_LINKER_FLAGS ${OpenFHE_EXE_LINKER_FLAGS})
    target_link_libraries(${PROJECT_NAME} ${OpenFHE_SHARED_LIBRARIES})
endif()

## OpenMP
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENMP_INCLUDES})
target_link_directories(${PROJECT_NAME} PRIVATE ${OPENMP_LIBRARIES})

## Boost
target_link_libraries(${PROJECT_NAME} ${Boost_SYSTEM_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${Boost_DATE_TIME_LIBRARY})
target_link_libraries(${PROJECT_NAME} Threads::Threads)

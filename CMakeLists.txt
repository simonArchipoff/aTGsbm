cmake_minimum_required(VERSION 3.16)

project(aTGsbm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Multimedia REQUIRED)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Récupérer KissFFT
FetchContent_Declare(
    kissfft
    GIT_REPOSITORY https://github.com/mborgerding/kissfft.git
    GIT_TAG        master
)


# Faire le téléchargement et l'ajout à votre projet
option(KISSFFT_OPENMP "Build kissfft with OpenMP support" OFF)
option(KISSFFT_PKGCONFIG "Build pkg-config files" OFF)
option(KISSFFT_STATIC "Build kissfft as static (ON) or shared library (OFF)" ON)
option(KISSFFT_TEST "Build kissfft tests" OFF)
option(KISSFFT_TOOLS "Build kissfft command-line tools" OFF)
option(KISSFFT_USE_ALLOCA "Use alloca instead of malloc" OFF)
set(KISSFFT_DATATYPE "float")


FetchContent_MakeAvailable(kissfft)


FetchContent_Declare(
    DSPFilters
    GIT_REPOSITORY https://github.com/jcelerier/DSPFilters.git
    GIT_TAG        master
)


# Télécharger et rendre disponible DSPFilters
FetchContent_MakeAvailable(DSPFilters)


file(GLOB_RECURSE DSPFILTERS_HEADER_FILES
    "${dspfilters_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE DSPFILTERS_SOURCE_FILES
    "${dspfilters_SOURCE_DIR}/src/*.cpp"
)


add_library(DSPFilters STATIC
    ${DSPFILTERS_SOURCE_FILES}
    ${DSPFILTERS_HEADER_FILES}
)
target_include_directories(DSPFilters PUBLIC ${dspfilters_SOURCE_DIR}/DSPFilters/include ${dspfilters_SOURCE_DIR}/include)


qt_add_executable(appaTGsbm
    main.cpp
)



#target_include_directories(appaTGsbm PRIVATE kiss-fft)


qt_add_qml_module(appaTGsbm
    URI aTGsbm
    VERSION 1.0
    QML_FILES main.qml
    SOURCES analysesignal.h
            analysesignal.cpp
    inputsignal.h
    inputsignal.cpp
    ringbuffer.h
    ringbuffer.cpp
    chronogram.h
    chronogram.cpp
    signalprocess.h
    signalprocess.cpp
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appaTGsbm PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appaTGsbm
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


target_link_libraries(appaTGsbm
    PRIVATE Qt6::Quick
    kissfft
    DSPFilters
    Qt6::Multimedia)


include(GNUInstallDirs)
install(TARGETS appaTGsbm
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


add_executable(testapp
    test.cpp
    diraccombconv.cpp
)

target_link_libraries(testapp PUBLIC
    kissfft)
message(STATUS "KissFFT_FOUND: ${kissfft_FOUND}")

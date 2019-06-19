# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        AEC

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        lib/randomization_functions.c
        lib/spl_sqrt_floor.c
        lib/division_operations.c
        lib/vector_scaling_operations.c
        lib/downsample_fast.c
        lib/cross_correlation.c
        lib/spl_init.c
        lib/min_max_operations.c
        lib/complex_fft.c
        lib/complex_bit_reverse.c
        lib/real_fft.c
        lib/delay_estimator.c
        lib/delay_estimator_wrapper.c
        lib/ring_buffer.c
        lib/aecm_core.c
        lib/echo_control_mobile.c
        aecm/AEC.c)

include_directories(include)

add_definitions(-DWEBRTC_POSIX)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        AEC

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
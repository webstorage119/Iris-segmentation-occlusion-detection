cmake_minimum_required(VERSION 2.8)
project( main )
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )
set(REFL_CORR "${CMAKE_CURRENT_SOURCE_DIR}/reflection_correction.cpp")
set(EYE_LOC "${CMAKE_CURRENT_SOURCE_DIR}/eye_localization.cpp")
set(PREPRO "${CMAKE_CURRENT_SOURCE_DIR}/preprocessing.cpp")
set(NORM "${CMAKE_CURRENT_SOURCE_DIR}/normalization.cpp")
set(DETEC "${CMAKE_CURRENT_SOURCE_DIR}/occlusion_reflection_detec.cpp")
set(REGR "${CMAKE_CURRENT_SOURCE_DIR}/polynomial_regression.cpp")
set(TEST "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
set(IRIS "${CMAKE_CURRENT_SOURCE_DIR}/iris_localization.cpp")
add_executable( main main.cpp )
target_link_libraries( main ${OpenCV_LIBS} ${REFL_CORR} ${EYE_LOC} ${PREPRO} ${NORM} ${DETEC} ${REGR} ${TEST} ${IRIS} )
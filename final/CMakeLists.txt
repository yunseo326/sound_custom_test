cmake_minimum_required(VERSION 3.10)
project(CalDegree_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 소스 파일 목록
set(SOURCES
    src/D_calculate_8_angles.cpp
    src/D_categorize_values.cpp
    src/fft.cpp
    src/gcc_phat.cpp
    src/D_select_final_direction.cpp
    src/D_process_audio.cpp
)

set(PORTAUDIO_INCLUDE_DIR /usr/include)
include_directories(${PORTAUDIO_INCLUDE_DIR})

# # FFTW3 라이브러리 찾기
# find_package(FFTW3 REQUIRED)

# 실행 파일 생성
add_executable(caldegree_main Test_Degree.cpp ${SOURCES})

# 헤더 파일 포함 디렉토리
include_directories(${PROJECT_SOURCE_DIR})

# FFTW 라이브러리 연결
target_link_libraries(caldegree_main PRIVATE fftw3)

# 빌드 타입 설정
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message("Build Type: ${CMAKE_BUILD_TYPE}")

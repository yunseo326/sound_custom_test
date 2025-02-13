cmake_minimum_required(VERSION 3.10)
project(CalDegree_Project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 소스 파일 목록
set(SOURCES
    src/_calculate_8_angles.cpp
    src/_categorize_values.cpp
    src/_fft.cpp
    src/_gcc_phat.cpp
    src/_select_final_direction.cpp
    src/_process_audio.cpp
    # src/Audio.cpp
)

set(PORTAUDIO_INCLUDE_DIR /usr/local/include)
include_directories(${PORTAUDIO_INCLUDE_DIR})

# # FFTW3 라이브러리 찾기
# find_package(FFTW3 REQUIRED)\

# PortAudio 라이브러리 경로 수동 설정
set(PORTAUDIO_INCLUDE_DIR /usr/local/include)
set(PORTAUDIO_LIBRARY /usr/local/lib/libportaudio.so)

# PortAudio 경로 추가
include_directories(${PORTAUDIO_INCLUDE_DIR})
link_directories(${PORTAUDIO_LIBRARY})


# 실행 파일 생성
add_executable(caldegree_main Test_Degree.cpp ${SOURCES})
add_executable(main main.cpp ${SOURCES})
add_executable(main_thread main_thread.cpp ${SOURCES})
# add_executable(audio_main Test_audio.cpp ${SOURCES})

# Matplotlib-C++ 경로 설정 (GitHub에서 다운로드한 matplotlib-cpp 폴더 경로)
set(MATPLOTLIB_CPP_PATH /home/yunseo/sound_custom_test/plot_graph_2/test/matplotlib-cpp)
target_include_directories(main_thread PRIVATE ${MATPLOTLIB_CPP_PATH})
include_directories(${MATPLOTLIB_CPP_PATH})
find_package(Python3 REQUIRED COMPONENTS Development)  
target_link_libraries(main_thread PRIVATE Python3::Python)

# 헤더 파일 포함 디렉토리
include_directories(${PROJECT_SOURCE_DIR})

# FFTW 라이브러리 연결
target_link_libraries(caldegree_main PRIVATE fftw3)
target_link_libraries(main PRIVATE fftw3)
target_link_libraries(main_thread PRIVATE fftw3)


set(PORTAUDIO_LIBRARY /usr/local/lib/libportaudio.so)
target_link_libraries(main PRIVATE ${PORTAUDIO_LIBRARY})
target_link_libraries(main_thread PRIVATE ${PORTAUDIO_LIBRARY})
# target_link_libraries(audio_main PRIVATE ${PORTAUDIO_LIBRARY})

find_package(Threads REQUIRED)
target_link_libraries(main_thread PRIVATE Threads::Threads)

# 빌드 타입 설정
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message("Build Type: ${CMAKE_BUILD_TYPE}")

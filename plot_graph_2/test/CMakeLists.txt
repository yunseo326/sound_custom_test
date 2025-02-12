# CMake 최소 버전 설정
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름 설정
project(AudioGraphExample)

# C++ 11 표준 사용
set(CMAKE_CXX_STANDARD 11)

# PortAudio 라이브러리 경로 수동 설정
set(PORTAUDIO_INCLUDE_DIR /usr/lib/python3/dist-packages/numpy/core/include)
set(PORTAUDIO_LIBRARY /usr/lib/x86_64-linux-gnu/libportaudio.so)

# PortAudio 경로 추가
include_directories(${PORTAUDIO_INCLUDE_DIR})
link_directories(${PORTAUDIO_LIBRARY})

set(PORTAUDIO_INCLUDE_DIR /usr/include)
include_directories(${PORTAUDIO_INCLUDE_DIR})

# PortAudio 라이브러리 경로 설정
#find_package(PortAudio REQUIRED)

# Matplotlib-C++ 경로 설정 (GitHub에서 다운로드한 matplotlib-cpp 폴더 경로)
set(MATPLOTLIB_CPP_PATH //home/leemuwon/test/sound_custom_test/plot_graph_2/test/matplotlib-cpp)

# 소스 파일 목록
set(plot_graph2 plot_graph_2.cpp)
set(2audio_not_plot 2audio_not_graph.cpp)
set(2audio_not_plot_gcc_phat 2audio_not_graph_gcc_phat.cpp)
set(gcc_phat plot_gcc_phat.cpp)
set(caldegree caldegree.cpp)

# 실행 파일 생성
add_executable(audio_graph ${plot_graph2})
add_executable(audio_not_graph ${2audio_not_plot})
add_executable(audio_gcc_phat ${2audio_not_plot_gcc_phat})
add_executable(gcc_phat ${gcc_phat})
add_executable(caldegree ${caldegree})


# fftw link 
target_link_libraries(audio_gcc_phat PRIVATE fftw3)
target_link_libraries(gcc_phat PRIVATE fftw3)

find_package(ALSA REQUIRED)
target_link_libraries(audio_graph PRIVATE ${ALSA_LIBRARIES})

find_package(Threads REQUIRED)
target_link_libraries(audio_not_graph PRIVATE Threads::Threads)

# PortAudio 라이브러리 링크 (라이브러리 디렉터리 지정 대신 직접 target_link_libraries에 사용)
set(PORTAUDIO_LIBRARY /usr/lib/x86_64-linux-gnu/libportaudio.so)
target_link_libraries(audio_graph PRIVATE ${PORTAUDIO_LIBRARY})
target_link_libraries(audio_not_graph PRIVATE ${PORTAUDIO_LIBRARY})
target_link_libraries(audio_gcc_phat PRIVATE ${PORTAUDIO_LIBRARY})

# Matplotlib-C++ 경로에 있는 헤더 파일 포함
target_include_directories(audio_graph PRIVATE ${MATPLOTLIB_CPP_PATH})
target_include_directories(audio_gcc_phat PRIVATE ${MATPLOTLIB_CPP_PATH})

target_include_directories(audio_graph PRIVATE /usr/include/python3.12)
target_include_directories(audio_gcc_phat PRIVATE /usr/include/python3.12)

# NumPy 헤더 경로 추가 (Python3과 함께 사용)
target_include_directories(audio_graph PRIVATE /usr/lib/python3/dist-packages/numpy/core/include)
target_include_directories(audio_gcc_phat PRIVATE /usr/lib/python3/dist-packages/numpy/core/include)


# Matplotlib-C++의 경우, matplotlibcpp.h 헤더를 직접 사용할 수 있도록 설정
include_directories(${MATPLOTLIB_CPP_PATH})

find_package(Python3 REQUIRED COMPONENTS Development)  
target_link_libraries(audio_graph PRIVATE Python3::Python)
target_link_libraries(audio_gcc_phat PRIVATE Python3::Python)


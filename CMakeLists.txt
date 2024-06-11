cmake_minimum_required(VERSION 3.10)

project(StreamCiphers)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
        main.cpp
        Grain128/Grain128.cpp
        ChaCha20/ChaCha20.cpp
        RC4/RC4.cpp
        CryptMT/CryptMT.cpp
)

set(HEADERS
        StreamCipher/StreamCipher.h
        Grain128/Grain128.h
        ChaCha20/ChaCha20.h
        RC4/RC4.h
        CryptMT/CryptMT.h
)

add_executable(StreamCiphers ${SOURCES})

target_include_directories(StreamCiphers PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/StreamCipher
        ${CMAKE_CURRENT_SOURCE_DIR}/Grain128
        ${CMAKE_CURRENT_SOURCE_DIR}/ChaCha20
        ${CMAKE_CURRENT_SOURCE_DIR}/RC4
        ${CMAKE_CURRENT_SOURCE_DIR}/CryptMT
)
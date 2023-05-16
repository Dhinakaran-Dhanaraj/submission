cmake_minimum_required(VERSION 3.5)

project(submission_interview)


############################################################
# Create a library
############################################################
add_library(submission_interview 
    src/error.cpp
    src/crypto.cpp
    src/hmac.cpp
    src/file_handling.cpp

)
add_library(submission::helpers ALIAS submission_interview)


include_directories( submission_interview
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/
)

############################################################
# Create a Target
############################################################

add_executable(encrypt
    src/encrypt.cpp
)

add_executable(decrypt
    src/decrypt.cpp
)


############################################################
# Link Targets
############################################################

find_package(OpenSSL REQUIRED)

set (DEPS 
    submission::helpers
    OpenSSL::SSL
)

target_link_libraries( encrypt
    PRIVATE 
        ${DEPS}
)

target_link_libraries( decrypt
    PRIVATE 
        ${DEPS}
)

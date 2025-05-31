cmake_minimum_required(VERSION 3.10)

project(LearnVulkan)

file(GLOB SOURCES *.cpp)

add_executable(LearnVulkan WIN32 ${SOURCES})

set(VULKAN_SDK $ENV{VULKAN_SDK})
target_include_directories(LearnVulkan PRIVATE ${VULKAN_SDK}/Include)

target_link_directories(LearnVulkan PRIVATE ${VULKAN_SDK}/Lib)
target_link_libraries(LearnVulkan PRIVATE vulkan-1)
# # Install vcpkg and add it to the PATH.
# # If using VS Code, add this to settings.json:
# # "cmake.configureSettings": {
# #     "CMAKE_TOOLCHAIN_FILE": "path-to-vcpkg/scripts/buildsystems/vcpkg.cmake"
# # }
# # Install Vulkan SDK
# # On macOS make sure the binaries inside the SDK are executable by running:
# # xattr -d -r com.apple.quarantine vulkan-sdk-folder
# # Add the following to the PATH:
# # export VULKAN_SDK="vulkan-sdk-folder/macOS"
# # export PATH=$VULKAN_SDK/bin:$PATH
# # export DYLD_LIBRARY_PATH=$VULKAN_SDK/lib:$DYLD_LIBRARY_PATH
# # export VK_ICD_FILENAMES="$VULKAN_SDK/share/vulkan/icd.d/MoltenVK_icd.json"
# # export VK_LAYER_PATH="$VULKAN_SDK/share/vulkan/explicit_layer.d/VkLayer_khronos_validation.json"
# vcpkg install glfw3
# vcpkg install 'imgui[glfw-binding]' 'imgui[vulkan-binding]' --recurse

cmake_minimum_required(VERSION 3.17)
project(vulkan-imgui VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_executable(vulkan-imgui main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(Vulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(vulkan-imgui PRIVATE imgui::imgui)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(vulkan-imgui PRIVATE glfw)
include(FetchContent)

FetchContent_Declare(
  lvgl
  GIT_REPOSITORY  https://github.com/lvgl/lvgl.git
  GIT_TAG         v8.3.11
)

FetchContent_MakeAvailable(lvgl)

target_compile_definitions(lvgl PRIVATE LV_CONF_INCLUDE_SIMPLE)
target_include_directories(lvgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lvgl")

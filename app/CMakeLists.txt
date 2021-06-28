set(SRC_FILES
  "main.c")

add_executable(kyu_app
  "${SRC_FILES}"
)

target_compile_features(kyu_app PRIVATE c_std_99)
set_target_properties(kyu_app PROPERTIES C_EXTENSION OFF)
target_link_libraries(kyu_app PRIVATE "${PROJECT_NAME}")

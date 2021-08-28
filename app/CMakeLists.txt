set(SRC_FILES
  "main.c")

add_executable(kyu_app
  "${SRC_FILES}")

set_target_properties(kyu_app
  PROPERTIES
  C_EXTENSION OFF
  C_STANDARD 99
  VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
target_link_libraries(kyu_app PRIVATE "${PROJECT_NAME}")

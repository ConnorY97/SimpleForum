# markdown/CMakeLists.txt

include(FetchContent)

FetchContent_Declare(
  md4c
  GIT_REPOSITORY https://github.com/mity/md4c.git
  GIT_TAG master
)

FetchContent_MakeAvailable(md4c)

add_custom_target(render_markdown ALL
    COMMENT "Render Markdown docs (use cmake --build . --target render_markdown)"
    COMMAND ${CMAKE_COMMAND} -E echo "Run: cmake --build . --target render_markdown to generate HTML"
)

# Optional real script execution (replace with your script):
add_custom_command(
    TARGET render_markdown
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Generating HTML from Markdown..."
    COMMAND ${CMAKE_COMMAND} -E env python3 ${CMAKE_CURRENT_SOURCE_DIR}/render_markdown.py
)

add_library(ps1 ps1.cpp)
target_link_libraries(ps1 matchit)
target_include_directories(ps1 PUBLIC ${CMAKE_CURRENT_LIST_DIR})

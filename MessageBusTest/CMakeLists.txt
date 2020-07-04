include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#添加IocContainer测试所涉及到的文件
set(inc_path ${PROJECT_SOURCE_DIR}/include)
file(GLOB HEAD_FILE_LIST ${inc_path}/MessageBus.hpp ${inc_path}/NonCopyable.hpp ${inc_path}/function_traits.hpp ${inc_path}/Any.hpp)


add_executable(MessageBusTest main.cpp ${HEAD_FILE_LIST})



install (TARGETS MessageBusTest DESTINATION bin)
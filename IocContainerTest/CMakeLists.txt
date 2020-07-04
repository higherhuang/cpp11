include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#添加IocContainer测试所涉及到的文件
set(inc_path ${PROJECT_SOURCE_DIR}/include)
#set(inc_files ${inc_path}/Any.hpp ${inc_path}/IocContainer.h)
file(GLOB HEAD_FILE_LIST ${inc_path}/Any.hpp ${inc_path}/IocContainer.hpp ${inc_path}/NonCopyable.hpp ${inc_path}/Singleton.hpp)


add_executable(IocContainerTest main.cpp ${HEAD_FILE_LIST})



install (TARGETS IocContainerTest DESTINATION bin)
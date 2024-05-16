cmake_minimum_required(VERSION 3.8)
project(vacuum_cleaner)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(example src/main.cpp)
ament_target_dependencies(example rclcpp std_msgs)

install(TARGETS
  example
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY launch description
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  install(
    DIRECTORY rviz
    DESTINATION share/${PROJECT_NAME}
  )
endif()

ament_package()


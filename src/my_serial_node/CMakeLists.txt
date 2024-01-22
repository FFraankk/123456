cmake_minimum_required(VERSION 3.8)
project(my_serial_node)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)



# add executable
add_executable(my_serial_node src/my_serial_node.cpp)

# include directories
target_include_directories(my_serial_node PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# link with dependencies
ament_target_dependencies(my_serial_node
  rclcpp
  std_msgs
)




if(BUILD_TESTING)
 find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


install(TARGETS
  my_serial_node
  DESTINATION lib/${PROJECT_NAME}
)




ament_package()

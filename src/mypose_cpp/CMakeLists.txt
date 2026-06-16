cmake_minimum_required(VERSION 3.8)
project(mypose_cpp)

# Require C++17 for Jazzy
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find all required packages
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)

# ── Executable 1: Pose subscriber (existing) ──────────────────────────────
add_executable(myPose src/myPose.cpp)
ament_target_dependencies(myPose
  rclcpp
  nav_msgs
  tf2
  tf2_ros
  geometry_msgs
)

# ── Executable 2: Random velocity publisher (new) ─────────────────────────
add_executable(random_vel_publisher src/random_vel_publisher.cpp)
ament_target_dependencies(random_vel_publisher
  rclcpp
  geometry_msgs
)

# ── Install both executables ──────────────────────────────────────────────
install(TARGETS
  myPose
  random_vel_publisher
  DESTINATION lib/${PROJECT_NAME}
)

# ── Install the launch directory ──────────────────────────────────────────
install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

ament_package()

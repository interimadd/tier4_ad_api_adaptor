cmake_minimum_required(VERSION 3.14)
project(tier4_autoware_api_extension)

find_package(autoware_cmake REQUIRED)
autoware_package()

ament_auto_add_library(${PROJECT_NAME} SHARED
  src/route_distance.cpp
  src/traffic_light.cpp
  src/planning_factor.cpp
)

rclcpp_components_register_node(${PROJECT_NAME}
  PLUGIN "tier4_autoware_api_extension::RouteDistance"
  EXECUTABLE route_distance_node
)

rclcpp_components_register_node(${PROJECT_NAME}
  PLUGIN "tier4_autoware_api_extension::TrafficLight"
  EXECUTABLE traffic_light_node
)

rclcpp_components_register_node(${PROJECT_NAME}
  PLUGIN "tier4_autoware_api_extension::PlanningFactor"
  EXECUTABLE planning_factor_node
)

find_package(topic_tools REQUIRED)
ament_auto_add_executable(linktime_composition
  src/linktime_composition.cpp)
set(libs
  ${PROJECT_NAME}
  /opt/ros/humble/lib/librelay_node.so
)
set(libs
  "-Wl,--no-as-needed"
  ${libs}
  "-Wl,--as-needed")

target_link_libraries(linktime_composition ${libs})
ament_target_dependencies(linktime_composition
  "class_loader"
  "rclcpp"
  "rclcpp_components"
  "topic_tools"
)

ament_auto_package(INSTALL_TO_SHARE config launch)

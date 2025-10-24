#include "planning_factor.hpp"
#include "route_distance.hpp"
#include "traffic_light.hpp"
#include "topic_tools/relay_node.hpp"

using namespace tier4_autoware_api_extension;

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  rclcpp::executors::SingleThreadedExecutor exec;
  rclcpp::NodeOptions options;

  auto planning_factor = std::make_shared<PlanningFactor>(options);
  exec.add_node(planning_factor);
  auto route_distance = std::make_shared<RouteDistance>(options);
  exec.add_node(route_distance);
  auto traffic_light = std::make_shared<TrafficLight>(options);
  exec.add_node(traffic_light);

  // relay_virtual_traffic_light_statusのrelayノードを追加
  // launchファイルでは以下のように記述されていた
  /*
    <node pkg="topic_tools" exec="relay" name="relay_virtual_traffic_light_status">
      <param name="input_topic" value="/api/external/set/virtual_traffic_light/states"/>
      <param name="output_topic" value="/awapi/tmp/virtual_traffic_light_states"/>
      <param name="type" value="tier4_v2x_msgs/msg/VirtualTrafficLightStateArray"/>
    </node>
  */
  std::vector<rclcpp::Parameter> relay_virtual_traffic_light_status_params;
  relay_virtual_traffic_light_status_params.push_back(
    rclcpp::Parameter("input_topic", "/api/external/set/virtual_traffic_light/states"));
  relay_virtual_traffic_light_status_params.push_back(
    rclcpp::Parameter("output_topic", "/awapi/tmp/virtual_traffic_light_states"));
  relay_virtual_traffic_light_status_params.push_back(
    rclcpp::Parameter("type", "tier4_v2x_msgs/msg/VirtualTrafficLightStateArray"));

  rclcpp::NodeOptions virtual_traffic_light_options;
  virtual_traffic_light_options.parameter_overrides(relay_virtual_traffic_light_status_params);
  virtual_traffic_light_options.arguments({"--ros-args", "-r", "__node:=relay_virtual_traffic_light_status"});

  auto relay_virtual_traffic_light_status = std::make_shared<topic_tools::RelayNode>(
    virtual_traffic_light_options);
  exec.add_node(relay_virtual_traffic_light_status);

  // relay_virtual_traffic_light_commandのrelayノードを追加
  // launchファイルでは以下のように記述されていた
  /*
    <node pkg="topic_tools" exec="relay" name="relay_virtual_traffic_light_commands">
      <param name="input_topic" value="/planning/scenario_planning/status/infrastructure_commands"/>
      <param name="output_topic" value="/api/external/get/virtual_traffic_light/commands"/>
      <param name="type" value="tier4_v2x_msgs/msg/InfrastructureCommandArray"/>
    </node>
  */
  rclcpp::NodeOptions virtual_traffic_light_command_options;
  std::vector<rclcpp::Parameter> relay_virtual_traffic_light_command_params;
  relay_virtual_traffic_light_command_params.push_back(
    rclcpp::Parameter("input_topic", "/awapi/tmp/virtual_traffic_light_commands"));
  relay_virtual_traffic_light_command_params.push_back(
    rclcpp::Parameter("output_topic", "/planning/scenario_planning/status/infrastructure_commands"));
  relay_virtual_traffic_light_command_params.push_back(
    rclcpp::Parameter("type", "tier4_v2x_msgs/msg/InfrastructureCommandArray"));
  virtual_traffic_light_command_options.parameter_overrides(relay_virtual_traffic_light_command_params);
  virtual_traffic_light_command_options.arguments({"--ros-args", "-r", "__node:=relay_virtual_traffic_light_commands"});
  auto relay_virtual_traffic_light_command = std::make_shared<topic_tools::RelayNode>(
    virtual_traffic_light_command_options);
  exec.add_node(relay_virtual_traffic_light_command);

  exec.spin();

  rclcpp::shutdown();

  return 0;
}

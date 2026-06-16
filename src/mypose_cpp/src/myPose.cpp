// myPose.cpp
// ROS2 Jazzy - Subscribes to /odom topic and continuously
// displays x position, y position, and yaw (theta) of Turtlebot3
// during SLAM navigation.
 
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include <iomanip>
 
// -------------------------------------------------------
// OdomSubscriber Node
// Inherits from rclcpp::Node - this is the ROS2 way
// -------------------------------------------------------
class OdomSubscriber : public rclcpp::Node
{
public:
    // Constructor: sets up the subscriber
    OdomSubscriber() : Node("odom_subscriber")
    {
        // Subscribe to /odom topic
        // Queue size 1000, callback function bound to this instance
        subscription_ = this->create_subscription<nav_msgs::msg::Odometry>(
            "/odom",
            1000,
            std::bind(&OdomSubscriber::odomCallback, this, std::placeholders::_1)
        );
 
        RCLCPP_INFO(this->get_logger(),
            "Node started. Subscribing to /odom...");
    }
 
private:
    // -------------------------------------------------------
    // Callback: fires every time a new /odom message arrives
    // Extracts and displays x, y position and yaw orientation
    // -------------------------------------------------------
    void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
    {
        // --- Extract x and y position ---
        double x = msg->pose.pose.position.x;
        double y = msg->pose.pose.position.y;
 
        // --- Extract yaw from quaternion orientation ---
        // Turtlebot3 orientation is published as a quaternion
        // We must convert it to roll/pitch/yaw to get theta (yaw)
        tf2::Quaternion q(
            msg->pose.pose.orientation.x,
            msg->pose.pose.orientation.y,
            msg->pose.pose.orientation.z,
            msg->pose.pose.orientation.w
        );
        tf2::Matrix3x3 m(q);
        double roll, pitch, yaw;
        m.getRPY(roll, pitch, yaw);  // yaw = theta in 2D plane
 
        // --- Print pose to terminal continuously ---
        RCLCPP_INFO(this->get_logger(),
            "Position: (%.2f, %.2f)  Direction (yaw/theta): %.2f rad",
            x, y, yaw);
    }
 
    // Subscriber member variable - must stay in scope to keep receiving
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;
};
 
// -------------------------------------------------------
// Main: initialize ROS2 and spin node
// rclcpp::spin() keeps node alive and processes all callbacks
// -------------------------------------------------------
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OdomSubscriber>());
    rclcpp::shutdown();
    return 0;
}

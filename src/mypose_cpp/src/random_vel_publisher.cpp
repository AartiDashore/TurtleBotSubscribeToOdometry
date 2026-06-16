// random_vel_publisher.cpp
// ROS2 Jazzy - Publishes random TwistStamped velocity commands to /cmd_vel

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include <random>
#include <chrono>

class RandomVelPublisher : public rclcpp::Node
{
public:
    RandomVelPublisher() : Node("random_vel_publisher"),
        rng_(std::random_device{}()),
        linear_dist_(-0.2, 0.2),
        angular_dist_(-1.0, 1.0)
    {
        publisher_ = this->create_publisher<geometry_msgs::msg::TwistStamped>("/cmd_vel", 10);

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(2000),
            std::bind(&RandomVelPublisher::publishVelocity, this)
        );

        RCLCPP_INFO(this->get_logger(),
            "RandomVelPublisher started — publishing TwistStamped to /cmd_vel every 2s");
    }

private:
    void publishVelocity()
    {
        auto msg = geometry_msgs::msg::TwistStamped();
        msg.header.stamp = this->now();
        msg.header.frame_id = "base_link";
        msg.twist.linear.x  = linear_dist_(rng_);
        msg.twist.angular.z = angular_dist_(rng_);

        publisher_->publish(msg);

        RCLCPP_INFO(this->get_logger(),
            "Published cmd_vel  linear.x=%.3f  angular.z=%.3f",
            msg.twist.linear.x, msg.twist.angular.z);
    }

    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::mt19937 rng_;
    std::uniform_real_distribution<double> linear_dist_;
    std::uniform_real_distribution<double> angular_dist_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RandomVelPublisher>());
    rclcpp::shutdown();
    return 0;
}

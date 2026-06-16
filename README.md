# Subscribe to Odometry (ROS 2)

## Overview

This project demonstrates a ROS 2 subscriber node that listens to the `/odom` topic published by a TurtleBot3 robot running in Gazebo simulation. The node extracts and displays the robot's:

* X Position (meters)
* Y Position (meters)
* Yaw Orientation (radians)

The position and orientation data are continuously updated and printed to the terminal as the robot moves within the simulated environment.

---

## Objective

The goal of this assignment is to:

1. Subscribe to the `/odom` topic.
2. Receive odometry messages published by TurtleBot3.
3. Extract the robot's position (`x`, `y`) and orientation (`yaw`).
4. Display the information in real time while the robot moves.

---

## System Architecture

The project consists of the following components:

### 1. Gazebo Simulation

* Launches the TurtleBot3 Burger robot in a simulated world.
* Publishes odometry data on the `/odom` topic.

### 2. Odometry Subscriber Node

* Custom ROS 2 C++ node.
* Subscribes to `/odom`.
* Processes incoming odometry messages.
* Prints robot pose information to the terminal.

### 3. Teleoperation Keyboard

* Allows manual robot control using keyboard inputs.
* Publishes velocity commands to `/cmd_vel`.

### 4. RQT Graph

* Visualizes node and topic communication within the ROS 2 system.

---

## Package Structure

```text
my_pose_cpp/
├── src/
│   └── my_pose.cpp
│   └── random_vel_publisher.cpp
├── CMakeLists.txt
├── package.xml
└── README.md
```

---

## Dependencies

This project requires:

* ROS 2
* TurtleBot3 Packages
* Gazebo
* tf2
* tf2_geometry_msgs
* nav_msgs
* geometry_msgs
* rclcpp
* rqt_graph

Install TurtleBot3 packages if not already installed:

```bash
sudo apt install ros-<distro>-turtlebot3*
```

Replace `<distro>` with your ROS 2 distribution (e.g., humble, iron, jazzy).

---

## Launch Instructions

### Terminal 1: Launch TurtleBot3 in Gazebo

Set the robot model:

```bash
export TURTLEBOT3_MODEL=burger
```

Launch the simulation:

```bash
ros2 launch turtlebot3_gazebo turtlebot3_world.launch.py
```

This starts Gazebo and spawns the TurtleBot3 Burger robot.

---

### Terminal 2: Run the Odometry Subscriber

```bash
ros2 run my_pose_cpp my_pose
```

The node will begin listening to `/odom` and display:

```text
x: 0.52
y: 0.11
yaw: 0.03
```

---

### Terminal 3: Control the Robot

```bash
ros2 run turtlebot3_teleop teleop_keyboard
```

Keyboard controls:

| Key   | Action        |
| ----- | ------------- |
| W     | Move Forward  |
| X     | Move Backward |
| A     | Turn Left     |
| D     | Turn Right    |
| Space | Stop          |
| S     | Stop          |

As the robot moves, the subscriber node will continuously update the pose information.

---

### Terminal 4: Visualize ROS Graph

```bash
rqt_graph
```

The graph shows communication between:

```text
teleop_keyboard
       |
       v
    /cmd_vel
       |
       v
 TurtleBot3
       |
       v
     /odom
       |
       v
 odom_subscriber
```

---

## Implementation Details

### Node Class

The program defines an `OdomSubscriber` class that inherits from `rclcpp::Node`.

```cpp
class OdomSubscriber : public rclcpp::Node
{
    ...
};
```

---

### Subscriber Creation

The subscriber is created using:

```cpp
create_subscription<nav_msgs::msg::Odometry>()
```

Parameters:

* Topic: `/odom`
* Queue Size: `1000`
* Callback Function: `odom_callback`

```cpp
subscription_ =
create_subscription<nav_msgs::msg::Odometry>(
    "/odom",
    1000,
    std::bind(&OdomSubscriber::odom_callback,
              this,
              std::placeholders::_1));
```

---

### Callback Function

Every time a new odometry message arrives, ROS 2 automatically invokes the callback.

Inside the callback:

#### Extract Position

```cpp
double x = msg->pose.pose.position.x;
double y = msg->pose.pose.position.y;
```

#### Extract Orientation

The robot orientation is provided as a quaternion:

```cpp
x
y
z
w
```

Since yaw is easier to interpret, the quaternion is converted using TF2:

```cpp
tf2::Quaternion q(
    msg->pose.pose.orientation.x,
    msg->pose.pose.orientation.y,
    msg->pose.pose.orientation.z,
    msg->pose.pose.orientation.w);
```

```cpp
tf2::Matrix3x3(q).getRPY(roll, pitch, yaw);
```

For TurtleBot3:

* Roll ≈ 0
* Pitch ≈ 0
* Yaw represents the robot's heading direction

---

### Logging Output

The pose information is printed using:

```cpp
RCLCPP_INFO(
    this->get_logger(),
    "x: %.2f y: %.2f yaw: %.2f",
    x,
    y,
    yaw);
```

---

### Node Execution

The node remains active using:

```cpp
rclcpp::spin(node);
```

Without `spin()`, the node would immediately terminate and never receive any messages.

---

## Sample Output

```text
x: 0.00 y: 0.00 yaw: 0.00
x: 0.15 y: 0.00 yaw: 0.00
x: 0.32 y: 0.01 yaw: 0.02
x: 0.58 y: 0.05 yaw: 0.15
x: 0.81 y: 0.10 yaw: 0.28
```

---

## Understanding the Output

### X Coordinate

Robot position along the X-axis (meters).

### Y Coordinate

Robot position along the Y-axis (meters).

### Yaw

Robot heading angle in radians.

Interpretation:

* `yaw = 0` → Facing positive X direction
* `yaw > 0` → Turning left (counter-clockwise)
* `yaw < 0` → Turning right (clockwise)

---

## Demonstration Summary

During the demonstration:

1. Gazebo simulation was launched with TurtleBot3 Burger.
2. The custom Free Audio & Speech ToolsROS 2 subscriber node was started.
3. The robot was manually controlled using the keyboard.
4. Odometry data was continuously published on `/odom`.
5. The subscriber extracted and displayed the robot's position and orientation in real time.
6. RQT Graph was used to visualize communication between nodes and topics.

---

## Learning Outcomes

By completing this project, the following ROS 2 concepts were demonstrated:

* ROS 2 Nodes
* Publishers and Subscribers
* Topic Communication
* Odometry Messages
* Callback Functions
* Quaternion to Euler Conversion
* TF2 Library Usage
* Gazebo Simulation
* TurtleBot3 Integration
* RQT Graph Visualization

---

Watch the demo video here:
![Demo_Video]()

## Author

**Aarti Dashore Seattle University 2026**

ROS 2 TurtleBot3 Odometry Subscriber Project

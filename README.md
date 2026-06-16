# ROS 2 TurtleBot3 Odometry Monitoring and Autonomous Navigation

## Overview

This project demonstrates a complete ROS 2 application using TurtleBot3 and Gazebo simulation. The package includes:

* An **Odometry Subscriber Node** that monitors the robot's position and orientation.
* A **Random Velocity Publisher Node** that autonomously drives the robot.
* A **ROS 2 Launch File** that starts the entire system with a single command.
* Integration with **Gazebo**, **TurtleBot3**, and **RQT Graph** for visualization and debugging.

The system continuously publishes robot motion commands, receives odometry updates, and displays the robot's real-time pose as it navigates through a simulated environment.

---

# Project Objectives

The primary objectives of this project are:

1. Subscribe to the `/odom` topic published by TurtleBot3.
2. Extract the robot's:

   * X Position
   * Y Position
   * Yaw Orientation (θ)
3. Display odometry information in real time.
4. Generate random velocity commands using a custom publisher.
5. Drive the robot autonomously in Gazebo.
6. Launch the complete system using a single ROS 2 launch file.
7. Visualize ROS 2 communication using RQT Graph.

---

# System Architecture

The project consists of four major components:

## 1. Gazebo Simulator

Gazebo provides:

* Physics simulation
* TurtleBot3 visualization
* Odometry generation
* Topic communication

The TurtleBot3 Burger robot is spawned inside the simulation environment and publishes odometry information on the `/odom` topic.

---

## 2. Odometry Subscriber (MyPose)

A custom ROS 2 node that:

* Subscribes to `/odom`
* Reads odometry messages
* Extracts:

  * X coordinate
  * Y coordinate
  * Yaw angle
* Continuously prints pose information to the terminal

Example output:

```text
x: 1.24
y: 0.56
yaw: 0.31
```

---

## 3. Random Velocity Publisher

A custom ROS 2 publisher node that:

* Publishes random `Twist` messages
* Sends commands to `/cmd_vel`
* Updates velocity every 2 seconds
* Generates both:

  * Linear velocity
  * Angular velocity

This allows the robot to move autonomously without keyboard input.

---

## 4. ROS 2 Launch File

A launch file is used to start all system components simultaneously:

* Gazebo
* TurtleBot3 Robot
* Random Velocity Publisher
* Odometry Subscriber

A 5-second startup delay is added for the custom nodes to ensure Gazebo and TurtleBot3 are fully initialized before communication begins.

---

# Package Structure

```text
my_pose_cpp/
├── launch/
│   └── turtlebot.launch.py
├── src/
│   ├── my_pose.cpp
│   └── random_velocity.cpp
├── include/
├── CMakeLists.txt
├── package.xml
└── README.md
```

---

# Dependencies

This project requires:

* ROS 2
* TurtleBot3 Packages
* Gazebo
* rclcpp
* nav_msgs
* geometry_msgs
* tf2
* tf2_ros
* launch
* launch_ros
* rqt_graph

Install TurtleBot3 packages:

```bash
sudo apt install ros-<distro>-turtlebot3*
```

Replace `<distro>` with your ROS 2 distribution (e.g., Humble, Iron, Jazzy).

---

# Node Implementation

## Odometry Subscriber Node

### Class Definition

The node is implemented using a custom class derived from `rclcpp::Node`.

```cpp
class OdomSubscriber : public rclcpp::Node
{
    ...
};
```

---

### Subscriber Creation

The subscriber listens to the `/odom` topic:

```cpp
subscription_ =
create_subscription<nav_msgs::msg::Odometry>(
    "/odom",
    1000,
    std::bind(
        &OdomSubscriber::odom_callback,
        this,
        std::placeholders::_1));
```

Parameters:

* Topic: `/odom`
* Queue Size: `1000`
* Callback Function: `odom_callback`

---

### Position Extraction

Inside the callback:

```cpp
double x = msg->pose.pose.position.x;
double y = msg->pose.pose.position.y;
```

---

### Orientation Extraction

The robot orientation is provided as a quaternion:

```cpp
orientation.x
orientation.y
orientation.z
orientation.w
```

Using TF2, the quaternion is converted into Roll-Pitch-Yaw (RPY):

```cpp
tf2::Quaternion q(
    msg->pose.pose.orientation.x,
    msg->pose.pose.orientation.y,
    msg->pose.pose.orientation.z,
    msg->pose.pose.orientation.w);
```

```cpp
tf2::Matrix3x3(q).getRPY(
    roll,
    pitch,
    yaw);
```

For TurtleBot3:

* Roll ≈ 0
* Pitch ≈ 0
* Yaw = Heading Direction

---

### Logging Output

```cpp
RCLCPP_INFO(
    this->get_logger(),
    "x: %.2f y: %.2f yaw: %.2f",
    x,
    y,
    yaw);
```

---

### Keeping the Node Alive

```cpp
rclcpp::spin(node);
```

Without `spin()`, the node would immediately terminate.

---

# Random Velocity Publisher

The random velocity node publishes velocity commands every 2 seconds.

Example behavior:

```text
Linear Velocity: 0.25 m/s
Angular Velocity: -0.50 rad/s
```

Published Topic:

```text
/cmd_vel
```

Message Type:

```text
geometry_msgs/msg/Twist
```

The robot continuously changes direction and speed based on newly generated commands.

---

# Launch File Implementation

The launch file contains three major actions.

## 1. Gazebo Launch

Loads:

* Gazebo
* TurtleBot3 Burger
* Simulation World

```python
gazebo = IncludeLaunchDescription(...)
```

---

## 2. Random Velocity Node

```python
Node(
    package='my_pose_cpp',
    executable='random_velocity'
)
```

---

## 3. Odometry Subscriber Node

```python
Node(
    package='my_pose_cpp',
    executable='my_pose'
)
```

---

## Startup Delay

Both custom nodes are delayed using:

```python
TimerAction(
    period=5.0,
    actions=[node]
)
```

This prevents startup synchronization issues.

---

# Running the System

## Option 1: Manual Launch

### Terminal 1 – Start Gazebo

```bash
export TURTLEBOT3_MODEL=burger

ros2 launch turtlebot3_gazebo turtlebot3_world.launch.py
```

---

### Terminal 2 – Start Odometry Subscriber

```bash
ros2 run my_pose_cpp my_pose
```

---

### Terminal 3 – Keyboard Control

```bash
ros2 run turtlebot3_teleop teleop_keyboard
```

Keyboard Controls:

| Key   | Action        |
| ----- | ------------- |
| W     | Move Forward  |
| X     | Move Backward |
| A     | Turn Left     |
| D     | Turn Right    |
| Space | Stop          |
| S     | Stop          |

---

### Terminal 4 – Visualize ROS Graph

```bash
rqt_graph
```

---

## Option 2: Launch Everything Automatically

Run:

```bash
ros2 launch my_pose_cpp turtlebot.launch.py
```

This starts:

* Gazebo
* TurtleBot3
* Random Velocity Publisher
* Odometry Subscriber

with a single command.

---

# ROS 2 Communication Pipeline

## Manual Teleoperation Mode

```text
Teleop Keyboard
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
 Odom Subscriber
```

---

## Autonomous Navigation Mode

```text
Random Velocity Publisher
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
    Odom Subscriber
```

---

# Monitoring the System

## View Active Nodes

```bash
ros2 node list
```

Example:

```text
/odom_subscriber
/random_velocity
/gazebo
```

---

## View Odometry Data

```bash
ros2 topic echo /odom
```

This displays:

* Position
* Orientation
* Velocity
* Header Information

---

## Visualize Communication

```bash
rqt_graph
```

The graph displays all active nodes and topic connections.

---

# Understanding the Output

## X Position

Robot position along the X-axis in meters.

---

## Y Position

Robot position along the Y-axis in meters.

---

## Yaw (θ)

Robot heading angle in radians.

Interpretation:

| Yaw Value | Meaning                          |
| --------- | -------------------------------- |
| 0         | Facing positive X direction      |
| Positive  | Turning left (counter-clockwise) |
| Negative  | Turning right (clockwise)        |

---

# Sample Output

```text
x: 0.00 y: 0.00 yaw: 0.00
x: 0.21 y: 0.02 yaw: 0.08
x: 0.47 y: 0.05 yaw: 0.23
x: 0.81 y: 0.14 yaw: 0.41
x: 1.08 y: 0.31 yaw: 0.62
```

---

# Demonstration Summary

During the demonstrations:

* TurtleBot3 was launched in Gazebo.
* The odometry subscriber successfully monitored robot pose.
* Manual keyboard control was used to verify odometry updates.
* A random velocity publisher was implemented for autonomous navigation.
* A launch file was created to start the complete system automatically.
* ROS 2 communication was verified using:

  * `ros2 node list`
  * `ros2 topic echo /odom`
  * `rqt_graph`

The robot's position and orientation were continuously tracked and displayed in real time throughout both manual and autonomous operation modes.

---

# Learning Outcomes

This project demonstrates:

* ROS 2 Nodes
* Publishers and Subscribers
* Topic Communication
* Odometry Processing
* Quaternion to Euler Conversion
* TF2 Library Usage
* Gazebo Simulation
* TurtleBot3 Integration
* Timer Actions
* ROS 2 Launch Files
* Autonomous Robot Navigation
* Teleoperation Control
* RQT Graph Visualization
* Complete ROS 2 System Integration

---

# Authors

**Aarti Dashore Seattle University 2026** – Odometry Subscriber Implementation & Launch File and Autonomous Navigation Implementation

ROS 2 TurtleBot3 Odometry Monitoring and Autonomous Navigation Project

MIT License

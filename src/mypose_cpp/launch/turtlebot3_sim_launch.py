# turtlebot3_sim_launch.py
# ============================================================
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():

    # Locate the turtlebot3_gazebo package ─────────────────────────────
    turtlebot3_gazebo_dir = get_package_share_directory('turtlebot3_gazebo')

    # Element 1: TurtleBot3 in Gazebo (empty world) ────────────────────
    # Includes the upstream turtlebot3_gazebo empty_world launch file.
    # Make sure TURTLEBOT3_MODEL=burger is exported in your shell.
    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(turtlebot3_gazebo_dir, 'launch', 'empty_world.launch.py')
        )
    )

    # Element 2: Random Velocity Publisher
    # Waits 5 s to give Gazebo time to fully start before sending commands.
    random_vel_node = TimerAction(
        period=5.0,
        actions=[
            Node(
                package='mypose_cpp',
                executable='random_vel_publisher',
                name='random_vel_publisher',
                output='screen',
                emulate_tty=True,
            )
        ]
    )

    # Element 3: Pose Publisher (myPose)
    # Waits 5 s so /odom is available before subscribing.
    pose_node = TimerAction(
        period=5.0,
        actions=[
            Node(
                package='mypose_cpp',
                executable='myPose',
                name='odom_subscriber',
                output='screen',
                emulate_tty=True,
            )
        ]
    )

    # Assemble and return the LaunchDescription
    return LaunchDescription([
        gazebo_launch,
        random_vel_node,
        pose_node,
    ])

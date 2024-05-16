from ament_index_python.packages import get_package_share_directory
import launch
from launch.substitutions import Command, LaunchConfiguration
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node
import xacro
import os

def generate_launch_description():
    pkg_name = 'vacuum_cleaner'
    model_subpath = 'description/robot.urdf.xacro'

    pkg_share = get_package_share_directory(pkg_name)
    model_path = os.path.join(pkg_share, model_subpath)
    robot_description_raw = xacro.process_file(model_path).toxml()
    default_rviz_config_path = os.path.join(pkg_share, 'rviz/urdf_config.rviz')

    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description_raw}]
    )
    joint_state_publisher_node = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        name='joint_state_publisher',
        parameters=[{'robot_description': robot_description_raw}],
        condition=launch.conditions.UnlessCondition(LaunchConfiguration('gui'))
    )
    joint_state_publisher_gui_node = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        name='joint_state_publisher_gui',
        condition=launch.conditions.IfCondition(LaunchConfiguration('gui'))
    )
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', LaunchConfiguration('rviz_config')],
    )

    return launch.LaunchDescription([
        DeclareLaunchArgument(
            name='gui', default_value='True', description='Flag to enable joint_state_publisher_gui'
        ),
        DeclareLaunchArgument(
            name='model', default_value=model_subpath, description='Absolute path to robot urdf file'
        ),
        DeclareLaunchArgument(
            name='rviz_config', default_value=default_rviz_config_path, description='Absolute path to rviz config file'
        ),
        # joint_state_publisher_node,
        # joint_state_publisher_gui_node,
        robot_state_publisher_node
        # rviz_node
    ])
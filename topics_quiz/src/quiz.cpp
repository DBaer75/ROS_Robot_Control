#include "geometry_msgs/Twist.h" //publishing topic
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h" //subscribing topic
#include "std_msgs/String.h"

float robot_forward;    // variable to be written to linear velocity
float robot_left_right; // varible to be written to angular velocity

void counterCallback(const sensor_msgs::LaserScan::ConstPtr &msg) {
  // create variables to make code easier to write
  float df = msg->ranges[360];
  float dl = msg->ranges[719];
  float dr = msg->ranges[0];

  // print dist
  ROS_INFO("Distance forward is: [%f]", df);
  ROS_INFO("Distance left is: [%f]", dl);
  ROS_INFO("Distance right is: [%f]", dr);

  // if dist>1 move forward
  if (df > 1) {
    robot_forward = 0.5;
    robot_left_right = 0.0;
  }

  // if dist<1 turn left
  if (df < 1) {
    robot_forward = 0.1;
    robot_left_right = 0.2;
  }

  // if dist_at_right_side<1 turn left
  if (dr < 1) {
    robot_forward = 0.1;
    robot_left_right = 0.2;
  }

  // if dist_at_left_side<1 turn right
  if (dl < 1) {
    robot_forward = 0.1;
    robot_left_right = -0.2;
  }
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "topics_quiz_node"); // create node
  ros::NodeHandle n;                         // node handle

  // subscriber to scan and call counterCallback function after
  ros::Subscriber sub =
      n.subscribe("/kobuki/laser/scan", 1000, counterCallback);

  // publisher to the cmd_vel to move robot
  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

  // create object of type Twist
  geometry_msgs::Twist robot;

  // loop rate at 10Hz
  ros::Rate loop_rate(10);

  int count = 0;

  while (ros::ok()) {

    robot.linear.x = robot_forward;
    robot.angular.z = robot_left_right;

    pub.publish(robot);

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
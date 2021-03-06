#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"
#include <sstream>
#include <iostream>
#include "turtlebot_controller/Vel.h"

ros::Publisher pub;
ros::Publisher pub2;
ros::ServiceClient client2;
int count = 10;

void positionCallback(const turtlesim::Pose::ConstPtr& msg) {
  ROS_INFO("Robot position [%f, %f]", msg->x, msg->y);
  my_srv::Velocity rec_vel;
  // if (count==10){
  // count=0; 
  rec_vel.request.x = msg->x;
  client2.call(rec_vel); 
  geometry_msgs::Twist vel; 
  vel.linear.x=rec_vel.response.x; 
  pub.publish(vel); 
  turtlebot_controller::Vel mymsg;
  mymsg.name = "linear"; 
  mymsg.vel = vel.linear.x;
  pub2.publish(mymsg);
  // } 
  // count++;




int main(int argc, char **argv)
{
     ros::init(argc, argv, "turtlebot_controller");
     ros::NodeHandle n;

     ros::ServiceClient client = n.serviceClient<turtlesim::Spawn>("/spawn");
     turtlesim::Spawn srv;
     srv.request.x = 0;
     srv.request.y = 0;
     client.call(srv);
     client2 = n.serviceClient<my_srv::Velocity>("/velocity");
     pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
     pub2 = n.advertise<turtlebot_controller::Vel>("/turtle1/linear", 1000);
     ros::Subscriber sub = n.subscribe("/turtle1/pose", 1000, positionCallback);

    ros::spin();
 
    return 0;
}



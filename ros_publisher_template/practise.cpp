#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
int main(int argc,char **argv)
{
    ros::init(argc,argv,"Publish_Geometry_msgs_for_robot");
    ros::NodeHandle n;
    ros::Publisher pub_msg = n.advertise<geometry_msgs::Twist>("/cmd_vel",10);
    geometry_msgs::Twist *cmd_data = new geometry_msgs::Twist;
    cmd_data.x = 1.0;
    cmd_data.z = 1.0;
    ros::Date sleep_date(10);
    while(ros::ok())
    {
        pub_msg.publish(cmd_data);
        sleep_date.sleep();
        ROS_INFO("CMMAND : x = %d   ,   y = %d",cmd_data.x,cmd_data.y);
    }
    delet(cmd_data);
    return 0;
}
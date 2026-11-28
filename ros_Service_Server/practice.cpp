#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<std_srvs/Trigger.h>
bool trigger = false;
bool callback(std_srvs::Trigger::Request &req,std_srvs::Trigger::Response &res)
{
    trigger = !trigger;
    res.success = ture;
    res.message = "success";
    return true;
}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"spwaner");
    ros::NodeHandle tur_n("turtle");
    ros::NodeHandle n;
    ros::ServiceServer spawner  = tur_n.advertiseService("spwaner",callback);
    ros::Publisher pubber = n.advertise<geometry_msgs::Twist>("cmd_vel",10);
    ros::Rate sleep_rate(10);
    
    geometry_msgs::Twist command;
    command.linear.x = 1.0;
    command.angular.y = 1.0;
    while()
    {
        if(trigger)
        {
            pubber.publish(command);
        }
        sleep_rate.sleep();
    }
    return 0;
}
#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
class Base
{
public:
    virtual void setData(){};
};
class  turtle_cmd: public Base {
public:

    void setData(double x,double y,double z) override{
        x_data = x;
        y_data = y;
        z_data = z;
    }
private:
    double x_data;
    double y_data;
    double z_data;
};
void callback(const geometry_msgs::Twist::ConstPtr &msg)
{
    ptr_base->setData(msg->x,msg->y,msg->z);    
}
int main(int argc, char** argv)
{
    ros::init(argc,argv,"turtle_twist_subscriber");
    ros::NodeHandle n("turtle");

    ros::Subscriber sub_turtle_cmd_vel = n.subscribe("cmd_vel",10,callback);

    turrle_cmd a;
    Base* ptr_base = &a;

    ros::spin();

    while(ros::ok())
    {
        ros::spinOnce();
        ...
    }

}
#include<ros/ros.h>
#include<std_srvs/Trigger.h>
int mian(int argc,char**argv)
{
    ros::init(argc,argv,"call_cmd");
    ros::NodeHandle tur_n("turtle");
    ros::servicce::waitForService("/turtle/set_cmd");
    ros::ServiceClient client1 = n.serviceClient<std_srvs::Trigger>("set_cmd");

    std_srvs::Trigger srv;
    ros::Rate sleep_rate(10);
    while(ros::ok())
    {
        if(client1.call(srv))
        {
            ROS_INFO("SET COMMAND SECCESS" );
            ROS_INFO_STREAM(srv.response.message<<endl);
        }
        else
        {
            ROS_ERROR("SER COMMAND FAIL");
        }
        sleep_rate.sleep();
    }
}
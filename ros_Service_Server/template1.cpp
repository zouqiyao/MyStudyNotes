#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_srvs/Trigger.h>

bool pubcommand = false;

bool commandCallBack(std_srvs::Trigger::Request &req,std_srvs::Trigger::Response &res) 
                    //Client .call() 发过来的是一个标准的API(你也可以自己写)
                    //这个API是一个结构体 Trigger
                    //Trigger 包含了一个 request 结构体 和一个 response 结构体
                    //request 结构体是 Client 填好的请求的数据 比如说 Turtlesim::Spawn srv  srv.request.x = 1.0; srv.request.y = 0.2; 这里就写好了request 再通过 .call() 传过来
                    //resopnse 就是要传回去Client 的数据 比如说: response.name = "turtle2" Client 就可以受到一个 string 意思是turtle创建成功名字是 "turtle2"
                    //你会发现这个commandCallBack()是 bool 当它返会 true res才会被传回去Client
        
{
    pubcommand = !pubcommand; //受到一次call()就变换一次运动年状态
    res.success = true;
    res.message = pubcommand ? "start" : "stop";
    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "turtle_command_server");
    ros::NodeHandle n;

    ros::ServiceServer command_service =
        n.advertiseService("/turtle_command", commandCallBack); //在这里会向Master 注册一个Service 
                                                                // 分别告诉Master 这个Service 的 名字 还有发布这个 Service 的节点地址
                                                                // 这个 Service 是这样运转的 :: 
                                                                // 首先 有一个Client .serviceClient() 跟Master声明 "我要向这个Service 发call" Master 就会将Server 的地址返回给Client
                                                                //Client 利用句柄 .call() 就将请求的 Api 发送给了 Server
                                                                //Server 底层维护了一个 队列 CallBackQueue 
                                                                //这个队列就会受到这个API 将它打包成一个等待 丢入 回调函数的数据
                                                                //在 调用 ros::spinOnce()的时候 ros就会检查 CallBackQueue 还有多少个 没有丢进Callback 的数据 
                                                                // 让程序中断 调用 回调函数 知道在这一刻之前的所有数据被运用完毕
    ros::Publisher Turtle_vel = 
        n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);

    ROS_INFO("Ready to receive turtle command");

    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        ros::spinOnce();

        if (pubcommand)
        {  
            geometry_msgs::Twist vel_cmd;
            vel_cmd.linear.x = 0.5;
            vel_cmd.angular.z = 0.2;
            Turtle_vel.publish(vel_cmd);
            //这里讲一下小海龟的运动机制
            //当没有收到订阅的Twist 的 时候 计时开始
            //小海龟从/turtle1/cmd_vel 里面得到Twist 数据 然后将自身的运动状态和Twist同步 计时清0
            //当计时达到1秒 小海龟不动
        }

        loop_rate.sleep(); //这里限制了turtle/cmd_vel 的发布速度最快是 100ms
                           //也限制了Server 接收数据的最快速度是 100ms 
                           //如果你再 100ms内 call 了 多次 那么这些call 都会被压缩
                           //同样的 如果你在1s内变化了两次 turtle的运动状态 turtle 就不会察觉你两次变化中的停顿
    }
    return 0;
}
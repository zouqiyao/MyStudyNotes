#include<ros/ros.h>
#include<turtlesim/Spawn.h>//发送请求的格式封装在turtlesim包 的 Spawn.msg里面

int main(int argc ,char ** argv)
{
    ros::init(argc,argv,"turtle_spawn");
    ros::NodeHandle n;

    ros::service::waitForService("/spawn"); //这是堵塞型函数 如果没有找到 /spawn service 存在 就会一直等待

    ros::ServiceClient add_turtle = n.serviceClient<turtlesim::Spawn>("/spawn");
    //n.serviceClient() 函数是向 Master 查询一个 名为 /spawn 服务地址 然后返回给 add_turtle
    // 我们就可以通过 add_turtle 来调用 spawn 这个service 了
    //这里的ServiceClient 是一个本地客户端 和 NodeHandle 一样 通过它来发送 call 通过它来受到 resopnd
    turtlesim::Spawn srv; //turtlesim 库里面 封装了 Spawn 这个类 是Api 标准格式
                          // srv 里存两东西 request 和 response 
                          // request 是 发送给 servicce 的消息
                          // service 执行完毕以后将 完成消息 塞进 response 里面
    srv.request.x = 2.0;
    srv.request.y = 2.0;
    srv.request.name = "turtle2";

    ROS_INFO("ready to call spawn");

    if( add_turtle.call(srv) ) //这是一个堵塞型的函数 一直等到 有回答数据 才继续下一行
    {                     // 如果call成功 add_turtle.call() 会返回1;
 
    ROS_INFO("Spawn turtle successfully [name:%s]",srv.response.name.c_str()); //这里name就是一个string 但是ROS_INFO 看不懂string
                                                                               //所以调用 c_str() 转换成char* 让ROS_INFO看懂
    }
    else{
        ROS_ERROR("Fail to call spawn");
    }
    return 0;
}
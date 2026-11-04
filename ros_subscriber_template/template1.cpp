# include<ros/ros.h>
# include<turtlesim/Pose.h> //Pose 是turtlesim 包里面的一个 pose.msg 文件 它记录着 ”海龟位置“的信息结构
                            //我在CMakeLists.txt 里就已经写好了 我要依赖的包了 在这个包里面就有 Pose.h这个文件 所以编译会过..
void callback(const turtlesim::Pose::ConstPtr &msg) //回调函数 这个函数的参数是一个指针 指向 Pose 结构体
                                                    // ConstPtr 就是 shared_ptr<const turtlesim::Pose> 
                                                    //&msg 是一个引用 
                                                    // 所以callback 函数传入的是一个 Pose 结构体的智能指针
{
    ROS_INFO("x:%f,y:%f,theta:%f",msg->x,msg->y,msg->theta); //打印出海龟的位置
}

int main(int argc,char **argv) //依旧读取终端 输入命令
{
    ros::init(argc,argv,"my_subscriber"); //初始化ros节点 跟 Master 报道 将自己的 ip 地址和端口号告诉 Master 让 Master 记录下来
    ros::NodeHandle n; //创建 奴隶
    ros::Subscriber sub = n.subscribe("turtle1/pose",1000,callback); //订阅topic turtle/pose 1000个队列长度 
                                                                     // Subscriber sub 就是 一个管理通路这个通路的对象 里面主要包括了 一个 impl_指指向一个 SubscriberImpl类 
                                                                     // 这个SubscruberImpl 类里面记录了 这个通路的名字 用来解析数据的helper_ 还有一个nodehandle
                                                                     // 过程是 subscriptionQueue 收到数据
                                                                     //向 CallbackQueue 写入任务 （注意 CallbackQueue 也是一个队列）
                                                                     // 然后 ros::spin() 这个函数会去 CallbackQueue 里面取出任务 执行
                                                                     // 利用ros::Subscriber sub 这个对象里 impl -> Impl 里面的 helper_ 解析subscriptionQueue 里的数据 
                                                                     // 解析后的数据 放在一个 堆上的临时 Pose 结构体里 然后把这个临时的 Pose 结构体的地址传给 callback 函数
    ros::spin();
    return 0;
}
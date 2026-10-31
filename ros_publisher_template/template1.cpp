# include <ros/ros.h>  // 包含了ros命名空间  还有里面的自由 函数 如 ros:: init()
# include <geometry_msgs/Twist.h> 其实.h 文件才是c++语言 Twist 是 geometry包里面的一个 Twist.msg 文件格式 只不过catkin 在编译时 将 Twist.msg 写成了 c++的结构体 Twist 放在了 Twist.h 这个文件里面  这里个代码里的 Twist 都是一个结构体

int main(int argc, char **argv)  //atgc 这个是什么呢 这个记录了 终端在调用这个节点时的 字符个数
                                 //atgv 这个是一个字符串数组 记录了终端在调用这个节点时的 字符串内容
                                 // 比如 终端输入 rosrun learning_topic turtle_publisher 这个命令时 argc 就是3  
                                 //argv[0] 就是 “rosrun“ argv[1] 就是 “learning_topic“ argv[2] 就是 “turtle_publisher“
                                 //turrle_publisher 是在Master 注册的节点名字 和rosrun 可执行的binary 文件名没有关联
                                 //注意catkin 生成的可执行binary 文件名一般和 .cpp的文件名一样
{

    ros_init(argc,argv,"turtle_publisher"); //首先roscpp 这个库是一个整个ros系统都可以调用的全局函数库
                                            //"运行状态" 就是 在节点运行起来以后 roscpp里面的函数在内存里 维护的 一堆全局变量 
                                            // 这些全局变量 对于当前进程是可见的 对于其他进程（跑起来的节点）是不可见的

                                            //package.xml <build_depend>roscpp<buile_depend/> 还有 CMakeLists.txt 里面的 find_package(catkin REQUIRED COMPONENTS roscpp ) target_link_libraries()
                                            //它们是在告诉 catkin 这个包在编译的时候需要roscpp 这个库
                                            //include<ros/ros.h> 就是将roscpp 这个库里的函数 加载进来
                                            //当我们在代码里调用 roscpp 里面的函数比如 ros::init() 就会用到roscpp里面写好的init()
                                            //ros::init() 干了点什么？ 在这个进程里面 维护了 一堆全局变量           
                                            // 这些全局变量 有什么呢： ros Master的地址 "turtle_publisher" 这个节点名 还有这个节点的IP地址 端口号(tcp监听用)
 

    ros:: NodeHandle n;  // NodeHandle 是一个类 它用来干什么呢？
                         // 首先 在这个代码跑起来的时候 我们已经 创建了一个节点！ 我们还想将这个节点 作为publisher 注册到ros Master 
                         // 我们可以如何做呢? 我们可以自己 向ros Master 注册... 自己写函数 ..自己传参数... 
                         // 晕死...老大每次我们都要自己写函数注册节点吗？ 这太麻烦..
                         // 所以我们就需要一个roscpp 里的"奴隶" 一个NodeHandle 类对象 
                         // NodeHandle 里面封装了向rosMaster 注册节点 声明成为publiser 或者 声明成为 subscriber 的函数
                         // 所以每次我们想注册节点到 rosMaster 就可以用 NodeHandle 的对象... 让它去干
    ros::Publisher turtle_vel_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
                         // 首先n.advertise() 是NodeHandle封装的一个 函数 用于 在Master 上注册一个 topic
                         //n.advertise<geometry_msgs::Twist> 这是一个模板函数 <geometry_msgs::Twist> 是这个topic 需要发布的数据类型
                         //("/turtle1/cmd_vel/",10) 传入参数告诉Master我这个topic 的名字叫做 /turtle1/cmd_vel/ 发布的队列缓冲区是10个数据
                         //ros::Publiser 类 跟 NodeHandle类一样 也是一个奴隶...
                         //n.advertisr() 只是告诉在Master 上注册了 一个 topic 
                         //当要向这个 topic 发送mssage 的时候我又得找到这个topic 的地址 ，找到这个topic 的数据类型..等等 再将它们以参数的形式写入mssage 发送函数
                         //上帝，这实在是太麻烦了 ... 我用一个类来管理这个 topic 不就好了？
                         // 聪明！ Publisher 这个类就是用来管理 这个 topic 的 ，它里面有这个topic 的名字 数据类型 发送端口等等. 还封装了发送函数
                         //所以我们每次要在这个topic 上发消息就叫 一个 Publisher 类来帮忙!
    ros::Rate loop_rate(10);
                         //有参构造了一个 Rate 的实例化对象 loop_rate "10"代表10Hz
                         //Rate 对象里面装着什么 ? :上一次sleep()的时刻 还有一个sleep 函数
                         //如果调用loop_rate.sleep() 这个函数就会查看当前时刻和上一次sleep时刻的时间差距 如果时间差距小于100ms 就在当前代码行等到时间差距到达100ms
                         // 如果超过了 就马上运行下一行
                         // 整体效果就是 每一次 .sleep() 之间的时间间隔必须大于等于100ms 也就是 .sleep()之间调用频率不超过10Hz
    while(ros::ok())     // 调用 ok()函数 会返回当前节点的状态(是否还在运行)(是否被退出）
                         // 当在这个节点的终端 按下 Ctrl ^C 的时候，  或者在代码里调用 ros::shutdown() 的时候 这个节点的状态就会变成 false
                         // 节点状态为 false 再次调用 ros::ok() 就会输出 false
                         //ros::ok()有什么作用呢？ 
                         // 你可以在节点状态为 false 之后 调用一些析构函数 之后再main()return 0 可以优雅的释放内存
    {
        geometry_msgs::Twist vel_msg; //实例化一个消息对象
        vel_msg.linear.x = 1.0;
        vel_msg.angular.z = 1.0;  //设置运动参数
        turtle_ver_pub.publish(vel_msg);//叫 /turtle/cmd_vel 这个话题的管理员turtle_ver_pub过来  让它去发布vel_msg 这个message 到这个topic 上

        ROS_INFO(“Linear: %f  Angular %f”,vel_msg.linear.x,vel_msg.angular.z) //这个函数就是将某些数据打印到终端上
                                                                              // 就是c 语言风格的 printf一样的写法 用 %d %f %c 作为占位符
        POS_INFO_STREAM("Linear:" << vel_msg.linear.x << "Angular:" <<vel_msg_angular.z) //这个也是将数据打印到终端上
                                                                                         // 就是 c++ 的 cout <<" " << 一样
        loop_rate.sleep();// 这里就调用了 loop_rate.sleep() loop_rate 里记录了上一次 sleep()的时刻 和当前时刻的差如果 小于 100ms (含参构造的时候传进去的10 代表10Hz 也就是1s 10次 每次间隔100ms)
                          // 那么代码就停在这里 等到 100ms 再继续
                          // 如果时间大于 100ms 就直接放行
    }
    return 0; //main()退出
}
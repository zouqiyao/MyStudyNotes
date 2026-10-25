````markdown
# Docker 内 ROS turtlesim 无法显示图形界面s
## 错误
容器内运行：
```bash
rosrun turtlesim turtlesim_node
```
报错关键：
```text
Authorization required, but no authorization protocol specified
qt.qpa.xcb: could not connect to display :0
Could not load the Qt platform plugin "xcb"
Aborted (core dumped)
```

## 原因
宿主机 X Server 未授权容器内的 `root` 用户连接显示服务。  
之前执行的是 `xhost +local:docker`，只授权了 docker 用户，不够。

## 解决方法
在**宿主机**执行：
```bash
xhost +local:root
```

启动容器时确保包含：
```bash
-e DISPLAY=$DISPLAY \
-v /tmp/.X11-unix:/tmp/.X11-unix
```

然后在容器内重新运行：
```bash
source /opt/ros/noetic/setup.bash
rosrun turtlesim turtlesim_node
```

## 结论
根因是 **X11 权限问题**，不是缺 xcb 库，也不是 Qt 插件配置问题。
````
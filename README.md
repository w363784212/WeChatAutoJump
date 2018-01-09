#WeChatAutoJump 

环境：Android 、Visual Studio 2017、OpenCV3.4（64位）


跳跃原理：通过adb shell screencap -p /sdcard/jump/"当前时间".png 截图存放在Android设备上，adb pull 拉取到电脑上，然后VS读取图片进行操作，算出时间后用adb input swipe x1 y1 x2 y2 time(ms)控制跳跃（需要打开Android设备的USB调试和模拟触摸）。


图像处理部分：根据棋子颜色的RGB值，将图像中的棋子分离，获取棋子的底部中心（起始坐标），因为跳一跳在前一次跳到中心后，下一方块的中心会出现圆形白点，像素值为（245,245,245），在图像中找到这个像素值的点的中心（目标坐标），但是有的方块也会出现极少量的该像素值，所以进行了一次腐蚀，只留下目标白点，算出目标坐标，算出距离后乘上相应系数得出触屏时间。




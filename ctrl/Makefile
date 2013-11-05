# 错误2：java.lang.UnsatisfiedLinkError: XXXclass.XXXmethod（）
# 这个错误是 在这个dll里找不到方法的声明，（搞了一下午害我差点装VC了，还借用到了dll查看工具）
# 网上说是@符号的问题 ，解决方法是用gcc 编译的时候指定参数
# 第1种方法：
# gcc -Wl,--kill-at -shared -o jnihello.dll Native.c
# 这种方法生成不带@的函数声明
  
# 第2种方法：
# gcc -Wl,--add-stdcall-alias -shared -o jnihello22.dll Native.c
# 这种方法会生成2个函数声明，一个是带@的 一个是不带@的。
  
# 第3种方法：
# 在你的本地方法的头文件中中的函数前面加上下划线，比如以前是
# JNIEXPORT void JNICALL Java_TestNative_Hello (JNIEnv * , jobject );
# 现在改成
# JNIEXPORT void  JNICALL  _Java_TestNative_Hello (JNIEnv * , jobject );
# 同时你的实现的cpp文件或者c文件里的函数头也要一致 前面有下划线。
 
TARGET = ctrl.dll
LDFLAGS = -mwindows -shared -Wl,--kill-at
INC_PATH = /D/Progra~1/Java/jdk1.7.0_05/include /D/Progra~1/Java/jdk1.7.0_05/include/win32
LIBS_PATH = /D/Program~1/Java/jdk1.7.0_05/lib ../libs 
LIBS_NAME = ws2_32 winmm mapi32 imm32 winspool comctl32 comdlg32 gdi32 ole32 user32 uuid getPcMsgApi wininet
SRC_PATH = . ./util
C_PATH = $(SRC_PATH)
CPP_PATH = $(SRC_PATH)

#DEBUG=true
include ../include.mk
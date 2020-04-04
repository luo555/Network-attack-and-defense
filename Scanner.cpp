

#include<stdio.h>
#include<WINSOCK2.H>
#include<string.h>
#pragma comment(lib,"WS2_32.lib")
int main(){
    WSADATA WSAData;//这个结构被用来存储 被WSAStartup函数调用后返回的 Windows Sockets数据。
					//它包含Winsock.dll执行的数据。
    sockaddr_in addr; //用来创建socket的结构体
    char IpAddress[100]; //待扫描的主机IP地址
    char startPort[10],endPort[10]; //开始和结束的端口号
    printf("请输入要扫描的主机IP地址:");
    gets(IpAddress);
    printf("请输入开始扫描的端口号:");
    gets(startPort);
    printf("请输入开始结束的端口号:");
    gets(endPort);

	/*wsastartup主要就是进行相应的socket库绑定。
	当一个应用程序调用WSAStartup函数时，操作系统根据请求的Socket版本来搜索相应的Socket库，
	然后绑定找到的Socket库到该应用程序中。
	以后应用程序就可以调用所请求的Socket库中的其它Socket函数了。该函数执行成功后返回0。*/

				//程序要使用2.2版本的Socket
    if(WSAStartup(MAKEWORD(2,2),&WSAData)!=0){ //初始化Winsock2.2
          printf("无法完成初始化...");
          return 0;
    }
    addr.sin_family=AF_INET;
    addr.sin_addr.S_un.S_addr=inet_addr(IpAddress); //将点分十进制的IP地址转换为网络字节序
    for(int i=atoi(startPort);i<=atoi(endPort);i++){ //atoi函数将字符串型端口转换为int型的值
        SOCKET s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//和每一个端口建立相关的socket,用于TCP连接
        if(s==INVALID_SOCKET){  //若端口建立失败，则程序结束
            printf("建立Socket失败,错误代码:%d",WSAGetLastError());  
            return 0;
        }
        addr.sin_port=(htons(i));//将端口设置为遍历的每一个端口，用于TCP连接
        if((connect(s,(sockaddr*)&addr,sizeof(sockaddr)))==-1){
               printf("端口关闭,端口号为:%d\n",i);
        }else{
                      printf("端口开启,端口号为:%d\n",i);
        }
        closesocket(s); //得到端口是否开启后关闭socket
    }
    if(WSACleanup()==SOCKET_ERROR){
           printf("WSACleanUp出错!!!");
    }  //做一些清除工作
    system("pause");
    return 0;
}
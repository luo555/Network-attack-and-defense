

#include<stdio.h>
#include<WINSOCK2.H>
#include<string.h>
#pragma comment(lib,"WS2_32.lib")
int main(){
    WSADATA WSAData;//����ṹ�������洢 ��WSAStartup�������ú󷵻ص� Windows Sockets���ݡ�
					//������Winsock.dllִ�е����ݡ�
    sockaddr_in addr; //��������socket�Ľṹ��
    char IpAddress[100]; //��ɨ�������IP��ַ
    char startPort[10],endPort[10]; //��ʼ�ͽ����Ķ˿ں�
    printf("������Ҫɨ�������IP��ַ:");
    gets(IpAddress);
    printf("�����뿪ʼɨ��Ķ˿ں�:");
    gets(startPort);
    printf("�����뿪ʼ�����Ķ˿ں�:");
    gets(endPort);

	/*wsastartup��Ҫ���ǽ�����Ӧ��socket��󶨡�
	��һ��Ӧ�ó������WSAStartup����ʱ������ϵͳ���������Socket�汾��������Ӧ��Socket�⣬
	Ȼ����ҵ���Socket�⵽��Ӧ�ó����С�
	�Ժ�Ӧ�ó���Ϳ��Ե����������Socket���е�����Socket�����ˡ��ú���ִ�гɹ��󷵻�0��*/

				//����Ҫʹ��2.2�汾��Socket
    if(WSAStartup(MAKEWORD(2,2),&WSAData)!=0){ //��ʼ��Winsock2.2
          printf("�޷���ɳ�ʼ��...");
          return 0;
    }
    addr.sin_family=AF_INET;
    addr.sin_addr.S_un.S_addr=inet_addr(IpAddress); //�����ʮ���Ƶ�IP��ַת��Ϊ�����ֽ���
    for(int i=atoi(startPort);i<=atoi(endPort);i++){ //atoi�������ַ����Ͷ˿�ת��Ϊint�͵�ֵ
        SOCKET s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//��ÿһ���˿ڽ�����ص�socket,����TCP����
        if(s==INVALID_SOCKET){  //���˿ڽ���ʧ�ܣ���������
            printf("����Socketʧ��,�������:%d",WSAGetLastError());  
            return 0;
        }
        addr.sin_port=(htons(i));//���˿�����Ϊ������ÿһ���˿ڣ�����TCP����
        if((connect(s,(sockaddr*)&addr,sizeof(sockaddr)))==-1){
               printf("�˿ڹر�,�˿ں�Ϊ:%d\n",i);
        }else{
                      printf("�˿ڿ���,�˿ں�Ϊ:%d\n",i);
        }
        closesocket(s); //�õ��˿��Ƿ�����ر�socket
    }
    if(WSACleanup()==SOCKET_ERROR){
           printf("WSACleanUp����!!!");
    }  //��һЩ�������
    system("pause");
    return 0;
}
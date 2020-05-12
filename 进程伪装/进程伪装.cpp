
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
 
// 结构定义
typedef struct _PROCESS_BASIC_INFORMATION {
	DWORD ExitStatus;
	ULONG PebBaseAddress;
	ULONG AffinityMask;
	LONG BasePriority;
	ULONG UniqueProcessId;
	ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;
 
// API声明
typedef LONG (__stdcall *PZWQUERYINFORMATIONPROCESS) 
(	HANDLE ProcessHandle,
    ULONG ProcessInformationClass,
	PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength);
/********************************************************/
 
/*  函数：FxReplaceProcessPath
    功能：伪装进程路径
    参数：1-目标进程句柄
          2-假路径的字符串（UNICODE）
	返回值：TRUE-成功
			FALSE-失败*/
BOOL FxReplaceProcessPath(HANDLE hProcess, TCHAR *szNewPath)
{
	// 获取NTDLL.DLL的基址
	HMODULE hModule = GetModuleHandle(TEXT("NTDLL.DLL"));
	if (hModule == NULL) return FALSE;
 
	// 获取ZwQueryInformationProcess函数的指针
	PZWQUERYINFORMATIONPROCESS pZwQueryInformationProcess = 
		(PZWQUERYINFORMATIONPROCESS)GetProcAddress(hModule, "ZwQueryInformationProcess");
	
	// 查询进程基本信息（包含PEB地址）
	PROCESS_BASIC_INFORMATION pbi = {NULL};
	if(pZwQueryInformationProcess(hProcess, 0, (LPVOID)&pbi, sizeof(pbi), NULL) < 0)
		return FALSE;
	
	// 获取PEB+0X10处的_RTL_USER_PROCESS_PARAMETERS结构指针
	ULONG lpRUPP = NULL;
	ReadProcessMemory(hProcess, (LPVOID)(pbi.PebBaseAddress + 0x10), &lpRUPP, 4, NULL);
	
	// 修改进程路径
	ULONG lpOldPath = NULL;
	ReadProcessMemory(hProcess, (LPVOID)(lpRUPP + 0x3C), &lpOldPath, 4, NULL);
	WriteProcessMemory(hProcess, (LPVOID)lpOldPath, szNewPath, MAX_PATH, NULL);
 
	// 修改命令行为空
	ULONG lpOldCommand = NULL;
	ReadProcessMemory(hProcess, (LPVOID)(lpRUPP + 0x44), &lpOldCommand, 4, NULL);
	WriteProcessMemory(hProcess, (LPVOID)lpOldCommand, TEXT(""), MAX_PATH, NULL);
 
	printf("伪装成功！\n");
	return TRUE;
}
 
//入口函数
int main(int argc, char* argv[])
{
	FxReplaceProcessPath(GetCurrentProcess(), TEXT("C:\\WINDOWS\\system32\\svchost.exe"));
	system("pause");
	return 0;
}
 

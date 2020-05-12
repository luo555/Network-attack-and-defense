
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
 
// �ṹ����
typedef struct _PROCESS_BASIC_INFORMATION {
	DWORD ExitStatus;
	ULONG PebBaseAddress;
	ULONG AffinityMask;
	LONG BasePriority;
	ULONG UniqueProcessId;
	ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;
 
// API����
typedef LONG (__stdcall *PZWQUERYINFORMATIONPROCESS) 
(	HANDLE ProcessHandle,
    ULONG ProcessInformationClass,
	PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength);
/********************************************************/
 
/*  ������FxReplaceProcessPath
    ���ܣ�αװ����·��
    ������1-Ŀ����̾��
          2-��·�����ַ�����UNICODE��
	����ֵ��TRUE-�ɹ�
			FALSE-ʧ��*/
BOOL FxReplaceProcessPath(HANDLE hProcess, TCHAR *szNewPath)
{
	// ��ȡNTDLL.DLL�Ļ�ַ
	HMODULE hModule = GetModuleHandle(TEXT("NTDLL.DLL"));
	if (hModule == NULL) return FALSE;
 
	// ��ȡZwQueryInformationProcess������ָ��
	PZWQUERYINFORMATIONPROCESS pZwQueryInformationProcess = 
		(PZWQUERYINFORMATIONPROCESS)GetProcAddress(hModule, "ZwQueryInformationProcess");
	
	// ��ѯ���̻�����Ϣ������PEB��ַ��
	PROCESS_BASIC_INFORMATION pbi = {NULL};
	if(pZwQueryInformationProcess(hProcess, 0, (LPVOID)&pbi, sizeof(pbi), NULL) < 0)
		return FALSE;
	
	// ��ȡPEB+0X10����_RTL_USER_PROCESS_PARAMETERS�ṹָ��
	ULONG lpRUPP = NULL;
	ReadProcessMemory(hProcess, (LPVOID)(pbi.PebBaseAddress + 0x10), &lpRUPP, 4, NULL);
	
	// �޸Ľ���·��
	ULONG lpOldPath = NULL;
	ReadProcessMemory(hProcess, (LPVOID)(lpRUPP + 0x3C), &lpOldPath, 4, NULL);
	WriteProcessMemory(hProcess, (LPVOID)lpOldPath, szNewPath, MAX_PATH, NULL);
 
	// �޸�������Ϊ��
	ULONG lpOldCommand = NULL;
	ReadProcessMemory(hProcess, (LPVOID)(lpRUPP + 0x44), &lpOldCommand, 4, NULL);
	WriteProcessMemory(hProcess, (LPVOID)lpOldCommand, TEXT(""), MAX_PATH, NULL);
 
	printf("αװ�ɹ���\n");
	return TRUE;
}
 
//��ں���
int main(int argc, char* argv[])
{
	FxReplaceProcessPath(GetCurrentProcess(), TEXT("C:\\WINDOWS\\system32\\svchost.exe"));
	system("pause");
	return 0;
}
 

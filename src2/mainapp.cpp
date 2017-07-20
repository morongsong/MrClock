


#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "shellapi.h"

#include <TlHelp32.h>  
#include <vector>
#include <set>
#include <string>
using std::string;
using std::wstring;
namespace std
{
    typedef std::basic_string<TCHAR> tstring;
}
using std::tstring;

typedef std::vector<std::tstring> vectstring;
typedef std::set<std::tstring> settstring;

#include <map>
typedef std::map<std::tstring, std::tstring> maptstring;


using namespace std;

bool traverseProcesses(std::map<std::tstring, int>& _nameID)
{
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) 
    {
        //std::cout << "CreateToolhelp32Snapshot Error!" << std::endl;;
        return false;
    }

    BOOL bResult = Process32First(hProcessSnap, &pe32);

    int num(0);

    while (bResult)
    {
        std::tstring name = pe32.szExeFile;
        int id = pe32.th32ProcessID;

        //std::cout << "[" << ++num << "] : " << "Process Name:"
        //    << name << "  " << "ProcessID:" << id << std::endl;

        _nameID.insert(std::pair<tstring, int>(name, id)); //×Öµä´æ´¢  
        bResult = Process32Next(hProcessSnap, &pe32);
    }

    CloseHandle(hProcessSnap);

    return true;
}

void CheckAndRunProc()
{
    std::map<std::tstring, int> mapNameID;

    if (traverseProcesses(mapNameID))
    {
        if (mapNameID.find(_T("MrClock.exe")) == mapNameID.end())
        {
            SHELLEXECUTEINFO si;
            memset(&si, 0, sizeof(si));
            si.cbSize = sizeof(SHELLEXECUTEINFO);
            si.fMask = SEE_MASK_NOCLOSEPROCESS;
            si.lpFile = _T("MrClock.exe");
            si.lpVerb = _T("open");
            si.nShow = SW_SHOW;
            si.lpParameters = NULL;
            ::ShellExecuteEx(&si);
            WaitForSingleObject(si.hProcess, 60000);
            CloseHandle(si.hProcess);
        }
        else if (mapNameID.find(_T("MrClockPt.exe")) == mapNameID.end())
        {
            SHELLEXECUTEINFO si;
            memset(&si, 0, sizeof(si));
            si.cbSize = sizeof(SHELLEXECUTEINFO);
            si.fMask = SEE_MASK_NOCLOSEPROCESS;
            si.lpFile = _T("MrClockPt.exe");
            si.lpVerb = _T("open");
            si.nShow = SW_SHOW;
            si.lpParameters = NULL;
            ::ShellExecuteEx(&si);
            WaitForSingleObject(si.hProcess, 60000);
            CloseHandle(si.hProcess);
        }
    }
    return;
}

void CheckAndRunProcQk()
{
    std::map<std::tstring, int> mapNameID;

    if (!traverseProcesses(mapNameID))
    {
        return;
    }

    if (mapNameID.find(_T("MrClock.exe")) != mapNameID.end())
    {
        return;
    }
    //HANDLE handleApp = ::OpenMutex(NULL, FALSE, _T("MrClock[[Guid(C4BCFF7C-AC06-43FA-AE26-F6C0217CD657)]]"));
    //if (NULL == handleApp)
    {
        SHELLEXECUTEINFO si;
        memset(&si, 0, sizeof(si));
        si.cbSize = sizeof(SHELLEXECUTEINFO);
        si.fMask = SEE_MASK_NOCLOSEPROCESS;
        si.lpFile = _T("cmd.exe ");
        si.lpVerb = _T("open");
        si.nShow = SW_HIDE;
        si.lpParameters = _T("/c start \"\" MrClock");
        ::ShellExecuteEx(&si);
        WaitForSingleObject(si.hProcess, 60000);
        CloseHandle(si.hProcess);
    }
    return;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    //ShellExecute(NULL, _T("open"), _T("MrClock.exe"), _T(""), _T(""), SW_SHOW);

    for (;;)
    {
        MSG msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {

            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }
        else
        {
            Sleep(150);
            CheckAndRunProcQk();
        }
    }



	return (int) 1;
}

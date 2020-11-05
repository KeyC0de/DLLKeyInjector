#include <iostream>
#include <Windows.h>


int main()
{
	// 1. get dll path
	LPCSTR dllPath = R"(C:\Users\Nikos\Documents\Visual_Studio_Projects\DllInjector\x64\Debug\dllToInject.dll)";

	// 2. get process handle 
	DWORD iPid;
	std::cout << "Enter process id " << std::endl;
	std::cin >> iPid;
	
	HANDLE hProc = OpenProcess( PROCESS_ALL_ACCESS,
		0,
		iPid );
	if ( hProc == nullptr
		|| hProc == "ERROR_INVALID_PARAMETER"
		|| hProc == "ERROR_ACCESS_DENIED")
	{
		std::cout << "Invalid process handle." << __LINE__ << __FILE__ << std::endl;
		std::cout << GetLastError() << std::endl;
		exit( EXIT_FAILURE );
	}

	// 3. allocate memory for the dllpath in the target process
	LPVOID pDllPathBase = VirtualAllocEx( hProc,
		nullptr, 
		strlen( dllPath ) + 1, // + null terminator
		MEM_COMMIT,
		PAGE_READWRITE );

	// 4. copy dll path to allocated space
	SIZE_T cBytesWritten;
	WriteProcessMemory( hProc,
		pDllPathBase,
		(LPVOID)dllPath,
		strlen( dllPath ) + 1,
		&cBytesWritten );

	// 5. create a remote thread to host the dll
	int iTid = 0;
	HANDLE hLoaderThread = CreateRemoteThread( hProc,
		nullptr,
		0, 
		(LPTHREAD_START_ROUTINE)GetProcAddress( GetModuleHandleW( L"Kernel32.dll" ),
			"LoadLibraryA"),
			pDllPathBase,
		/* func parameter -> LoadLibrary(pDllPathBase) */ 0u,
		(LPDWORD)iTid );
	WaitForSingleObject( hLoaderThread,
		INFINITE );

	std::cout << "Dll path allocated @: " << pDllPathBase << " into the target program.\n";
	std::cout << "Injected Thread id: " << iTid << "\n";
	std::cin.get();

	// 5. free the memory allocated for our dll path
	VirtualFreeEx( hProc,
		pDllPathBase,
		strlen( dllPath ) + 1,
		MEM_RELEASE );

	return 0;
}




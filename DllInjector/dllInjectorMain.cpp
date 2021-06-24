#include <iostream>
#include <Windows.h>


int main()
{
	// 1.  Get pid of target process. `GetWindowThreadProcessId`
	DWORD pid;
	std::cout << "Enter process id \n";
	std::cin >> pid;
	
	// 2. Get process handle with appropriate permissions. `OpenProcess`
	HANDLE hProc = OpenProcess( PROCESS_ALL_ACCESS,
		0,
		pid );
	if ( hProc == nullptr
		|| hProc == "ERROR_INVALID_PARAMETER"
		|| hProc == "ERROR_ACCESS_DENIED")
	{
		std::cout << "Invalid process handle."
			<< __LINE__
			<< __FILE__
			<< '\n';
		std::cout << GetLastError()
			<< '\n';
		exit( EXIT_FAILURE );
	}

	// 3. Allocate memory inside the process. `VirtualAllocEx`
	LPCSTR dllPath = R"(C:\Users\Nikos\Documents\Visual_Studio_Projects\DllInjector\x64\Debug\dllToInject.dll)";
	LPVOID pDllPathBase = VirtualAllocEx( hProc,
		nullptr, 
		strlen( dllPath ) + 1, // + null terminator
		MEM_COMMIT,
		PAGE_READWRITE );

	// 4. Copy dll into that memory. `WriteProcessMemory`
	SIZE_T cBytesWritten;
	WriteProcessMemory( hProc,
		pDllPathBase,
		(LPVOID)dllPath,
		strlen( dllPath ) + 1,
		&cBytesWritten );
	std::cout << cBytesWritten
		<< " bytes written.\n";

	// 5. Create a remote thread to host the dll. `CreateRemoteThread`
	//	- Get address of "LoadLibraryA|W" function to place the dll. `GetProcAddress`
	//	- calling `CreateRemoteThread` on target process instructs it to execute the dll.
	//		This entails a call to `LoadLibraryA|W` in the target process, with the thread
	//		parameter being the memory address you've allocated which points to the dll base.
	int tid = 0;
	HANDLE hLoaderThread = CreateRemoteThread( hProc,
		nullptr,
		0, 
		(LPTHREAD_START_ROUTINE)GetProcAddress( GetModuleHandleW( L"Kernel32.dll" ),
			"LoadLibraryA" ),
		pDllPathBase,
		/* func parameter -> LoadLibrary(pDllPathBase) */ 0u,
		(LPDWORD)tid );

	std::cout << "Dll allocated @: "
		<< pDllPathBase
		<< " into the target program.\n";
	std::cout << "Injected Thread id: "
		<< tid << "\n";
	std::cin.get();

	// 6. `WaitForSingleObject` until the thread is finished executing.
	WaitForSingleObject( hLoaderThread,
		INFINITE );

	// 7. Free the target process's memory allocated for the needs of the dll
	VirtualFreeEx( hProc,
		pDllPathBase,
		strlen( dllPath ) + 1,
		MEM_RELEASE );

	return 0;
}

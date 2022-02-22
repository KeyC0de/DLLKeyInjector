#include <iostream>
#include "..\winner.h"


int main()
{
	// 1.
	DWORD pid;
	std::cout << "Enter process identification number: ";
	std::cin >> pid;
	
	// 2.
	HANDLE hProc = OpenProcess( PROCESS_ALL_ACCESS,
		0,
		pid );
	if ( hProc == nullptr || hProc == "ERROR_INVALID_PARAMETER"
		|| hProc == "ERROR_ACCESS_DENIED")
	{
		std::cout << "Invalid process handle.\n"
			<< GetLastError()
			<< '\n';
		exit( -1 );
	}

	// 3.
#if defined _DEBUG && !defined NDEBUG
	LPCSTR dllPath = R"(..\x64\Debug\dllToInject.dll)";
#else
	LPCSTR dllPath = R"(..\x64\Release\dllToInject.dll)";
#endif
	LPVOID pDllBase = VirtualAllocEx( hProc,
		nullptr, 
		strlen( dllPath ) + 1, // +1B for null terminator storage
		MEM_COMMIT,
		PAGE_READWRITE );

	// 4.
	SIZE_T cbytesWritten;
	WriteProcessMemory( hProc,
		pDllBase,
		(LPVOID)dllPath,
		strlen( dllPath ) + 1,
		&cbytesWritten );
	std::cout << cbytesWritten
		<< " bytes written.\n";

	// 5.
	DWORD tid = 0;
	HANDLE hLoaderThread = CreateRemoteThread( hProc,
		nullptr,
		0, 
		(LPTHREAD_START_ROUTINE)GetProcAddress( GetModuleHandleW( L"Kernel32.dll" ),
			"LoadLibraryA" ),
		pDllBase,
		0u,
		(LPDWORD)&tid );

	std::cout << "Dll allocated @: "
		<< pDllBase
		<< " into the target process.\n";
	std::cout << "Thread id: "
		<< tid
		<< "\n";
	std::cin.get();

	// 6.
	WaitForSingleObject( hLoaderThread,
		INFINITE );

	// 7.
	VirtualFreeEx( hProc,
		pDllBase,
		strlen( dllPath ) + 1,
		MEM_RELEASE );

	std::system( "pause" );
	return EXIT_SUCCESS;
}
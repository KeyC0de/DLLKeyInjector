#include <Windows.h>

// dll entry point
BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD ulReasonForCall,
	LPVOID lpReserved )
{
	if ( ulReasonForCall == DLL_PROCESS_ATTACH || ulReasonForCall == DLL_THREAD_ATTACH )
	{
		MessageBoxW( nullptr,
			L"Hello from dllToInject!",
			L"Hello",
			MB_ICONINFORMATION );
	}
	return TRUE;
}

#include "..\winner.h"


int WINAPI DllMain( HINSTANCE hInst,
	DWORD ulReasonForcall,
	LPVOID pReserved )
{
	switch ( ulReasonForcall )
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		MessageBoxW( nullptr,
			L"Hello from dllToInject!",
			L"Hello",
			MB_ICONINFORMATION );
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;
	default:
		break;
	}
	return TRUE;
}

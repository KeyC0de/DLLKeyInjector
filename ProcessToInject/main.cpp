#include <Windows.h>
#include <iostream>

// we will inject the dll to this process, to test for proper functionality

int main()
{
	MessageBoxW( nullptr,
		L"You're gonna love it",
		L"Love it",
		0u );

	while( true )
	{
		std::wcout << L"Echo" << std::endl;
		Sleep( 3000 );
	}

	return 0;
}


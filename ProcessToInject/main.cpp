#include <Windows.h>
#include <iostream>

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

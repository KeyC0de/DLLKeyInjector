#include <iostream>
#include "..\winner.h"


int main()
{
	std::cout << "pid = "
		<< GetProcessId( GetCurrentProcess() )
		<< '\n';
	while( !( GetAsyncKeyState( VK_F10 ) & 0x01 ) )
	{
		std::cout << "echo";
		Sleep( 1000u );
		std::cout << '.';
		Sleep( 1000u );
		std::cout << '.';
		Sleep( 1000u );
		std::cout << ".\n";
	}

	std::system( "pause" );
	return EXIT_SUCCESS;
}
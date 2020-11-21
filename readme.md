<h1 align="center">
	<a href="https://github.com/KeyC0de/DLLKeyInjector">DLL Key Injector</a>
</h1>
<hr>


A code injection demo.

Code injection is a technique where you insert/inject a piece of code (your custom code) into a target process and then reroute code execution to traverse through your injected code. We search for "code caves" (DB 00 in assembly as seen by Ollydbg) i.e. memory regions big enough to store our code.

One of the best ways to inject code is via dlls, because they are meant to be loaded by multiple processes at runtime.

*Let me just dll you a story Mr. Process...*

We create:

1. injector process - `DLLInjector` project
2. the process to inject - `ProcessToInject` project
3. the dll to inject - `dllToInject` project

Now the injector will grab the dll and inject it into the process to call custom code.

The discovery of memory addresses of certain API functions, such as `LoadLibrary` and `VirtualAlloc` is crucial. Those will be discovered in shared libraries, such as Kernel32.dll or ntddl.dll which are used by almost all windows processes certainly user processes that we will be targeting here. These functions will be used to load the dll into the target process's address space in RAM and call its entry point `DllMain`.

**Steps that DLLInjector has to perform:**

1. Get pid of target process. `GetWindowThreadProcessId`
2. Get process handle with appropriate permissions. `OpenProcess`
3. Allocate memory inside the process. `VirtualAllocEx`
4. Copy dll path into that memory. `WriteProcessMemory`
5. Create a remote thread to host the dll. `CreateRemoteThread`
	- Get address of "LoadLibraryA|W" function to place the dll. `GetProcAddress`
	- calling `CreateRemoteThread` on target process instructs it to execute the dll. This entails a call to `LoadLibraryA|W` in the target process, with the thread parameter being the memory address you've allocated which points to the dll base.
6. `WaitForSingleObject` until the thread is done.
7. Free the target process's memory allocated for the needs of the dll


# Contribute

Please submit any bugs you find through GitHub repository 'Issues' page with details describing how to replicate the problem. If you liked it or you learned something new give it a star, clone it, laugh at it, contribute to it whatever. I appreciate all of it. Enjoy.


# License

Distributed under the GNU GPL V3 License. See "GNU GPL license.txt" for more information.


# Contact

email: *nik.lazkey@gmail.com*</br>
website: *www.keyc0de.net*


# Acknowledgements

[ollydbg](http://www.ollydbg.de/)


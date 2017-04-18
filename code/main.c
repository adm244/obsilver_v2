/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

//IMPORTANT(adm244): SCRATCH VERSION JUST TO GET IT UP WORKING

/*
  RE:
    Game main loop, window is active: 0040F19D
    To avoid OBSE conflicts patch here: 0040F1A3
    
    ConsoleCommandExecute(?) address: 00585C90
    
    TODO:
      - Toggle UI Messages (get from OBSE)
      - something else ?
*/

#include <windows.h>

#define internal static

//NOTE(adm244): copied from OBSE source code
typedef unsigned char       UInt8;    //!< An unsigned 8-bit integer value
typedef unsigned short      UInt16;   //!< An unsigned 16-bit integer value
typedef unsigned long       UInt32;   //!< An unsigned 32-bit integer value
typedef unsigned long long  UInt64;   //!< An unsigned 64-bit integer value
typedef signed char         SInt8;    //!< A signed 8-bit integer value
typedef signed short        SInt16;   //!< A signed 16-bit integer value
typedef signed long         SInt32;   //!< A signed 32-bit integer value
typedef signed long long    SInt64;   //!< A signed 64-bit integer value
typedef float               Float32;  //!< A 32-bit floating point value
typedef double              Float64;  //!< A 64-bit floating point value

//NOTE(adm244): addresses for hooks (oblivion 1.2.416)
internal const UInt32 mainloop_hook_patch_address = 0x0040F1A3;
internal const UInt32 mainloop_hook_return_address = 0x0040F1A8;

//NOTE(adm244): returns whenever key is pressed or not
internal int GetKeyPressed(byte key)
{
  short keystate = (short)GetAsyncKeyState(key);
  return( (keystate & 0x8000) > 0 );
}

internal void GameLoop()
{
  if( GetKeyPressed(VK_HOME) ){
    MessageBox(NULL, "You just pressed a HOME key!", "Yey!", MB_OK);
  }
}

internal void __declspec(naked) GameLoop_Hook()
{
  __asm {
    pushad
    call GameLoop
    popad
    
    //NOTE(adm244): original instructions
    mov ecx, [eax]
    mov edx, [ecx + 0x0C]

    jmp [mainloop_hook_return_address]
  }
}

//NOTE(adm244): copied from OBSE source code
void SafeWrite8(UInt32 addr, UInt32 data)
{
  UInt32 oldProtect;

  VirtualProtect((void *)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
  *((UInt8 *)addr) = data;
  VirtualProtect((void *)addr, 4, oldProtect, &oldProtect);
}
void SafeWrite32(UInt32 addr, UInt32 data)
{
  UInt32 oldProtect;

  VirtualProtect((void *)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
  *((UInt32 *)addr) = data;
  VirtualProtect((void *)addr, 4, oldProtect, &oldProtect);
}
internal void WriteRelJump(UInt32 jumpSrc, UInt32 jumpTgt)
{
  // jmp rel32
  SafeWrite8(jumpSrc, 0xE9);
  SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
}

internal BOOL WINAPI DllMain(HANDLE procHandle, DWORD reason, LPVOID reserved)
{
  if(reason == DLL_PROCESS_ATTACH) {
    MessageBox(NULL, "Obsilver is loaded!", "Yey!", MB_OK);
    
    //NOTE(adm244): patching in the oblivion main loop right after obse
    //SafeWrite8(mainloop_hook_patch_address - 1, 0x90); // nop (for debugger friendliness)
    WriteRelJump(mainloop_hook_patch_address, (UInt32)&GameLoop_Hook);
  }

  return TRUE;
}

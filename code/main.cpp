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
//IMPORTANT(adm244): used code from OBSE (http://obse.silverlock.org/)

/*
  RE:
    Game main loop, window is active: 0040F19D
    To avoid OBSE conflicts patch here: 0040F1A3
    
    PrintCommandToConsole(?) address: 00585C90
    
    CompileAndRun address: 004FBF00
    CompileAndRun signature:
      int __stdcall (void *globalScriptState, uint32 unk1, void *objectRef);
    
    ScriptExecute address: 004FBE00
    ScriptExecute signature:
      int _stdcall (TESObjectREFR *thisObj, ScriptEventList *eventList, TESObjectREFR *containingObj, int arg3);
    
    TODO:
      - Toggle UI Messages (get from OBSE)
      - something else ?
*/

#include <windows.h>

#define internal static
#include "obse_data.cpp"

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
    //MessageBox(NULL, "You just pressed a HOME key!", "Yey!", MB_OK);
    RunScriptLine("ts");
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

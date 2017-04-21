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
//IMPORTANT(adm244): some code used from OBSE (http://obse.silverlock.org/)

/*
  RE:
    Game main loop, window is active: 0040F19D
    To avoid OBSE conflicts patch here: 0040F1A3
    
    PrintToConsole address: 00585C90
    
    CompileAndRun address: 004FBF00
    CompileAndRun signature:
      int __stdcall (void *globalScriptState, uint32 unk1, void *objectRef);
    
    ScriptExecute address: 004FBE00
    ScriptExecute signature:
      int __stdcall (TESObjectREFR *thisObj, ScriptEventList *eventList, TESObjectREFR *containingObj, int arg3);
    
    NOTES:
      - 'BAT' command algorithm (possibly)
        1) get filename
        2) try to open file
        3) read contents of file
        4) close file
        5) create Script object
          (loop start)
        6) set its text to current line
        7) call CompileAndRun on that object
          (loop end)
        8) destroy Script object
        
        So, that's probably why OBSE implemented exactly that,
        since 'bat' is not a command that can be compiled as a script
        you cannot set it as text for Script object and call CompileAndRun.
        
        Could try to RE to figure out how to call that 'bat' behavior,
        but it looks like it's much easier to implement everything myself.
    
    TODO:
      - (DONE) Toggle UI Messages (get from OBSE)
      - (DONE) Show UI Message (get from OBSE)
      - Gameplay state (loading screen, main menu, etc...)
      - Save game
      - something else ?
*/

#include <windows.h>

#define internal static
#include "obse_data.cpp"

//NOTE(adm244): addresses for hooks (oblivion 1.2.416)
internal const UInt32 mainloop_hook_patch_address = 0x0040F1A3;
internal const UInt32 mainloop_hook_return_address = 0x0040F1A8;
internal const UInt32 showuimessage_patch_address = 0x0057ACC0;
internal const UInt32 showuimessage_2_patch_address = 0x0057ADD0;

internal bool isKeyHomeEnabled = true;
internal bool isKeyEndEnabled = true;

//NOTE(adm244): returns whenever key is pressed or not
internal int GetKeyPressed(byte key)
{
  short keystate = (short)GetAsyncKeyState(key);
  return( (keystate & 0x8000) > 0 );
}

//NOTE(adm244): checks if key was pressed and locks its state
// returns true if key wasn't pressed in previous frame but it is in this one
// returns false if key is not pressed or is hold down
internal bool IsActivated(byte key, bool *enabled)
{
  if( GetKeyPressed(key) ){
    if( *enabled ){
      *enabled = false;
      return(true);
    }
  } else{
    *enabled = true;
  }
  return(false);
}

//NOTE(adm244): enables\disables any ui messages upper left courner of the screen
//NOTE(adm244): it doesn't disable some sounds (skill increase messages would still have sound played)
//NOTE(adm244): obse can patch this address as well
// suppress = true - disables ui messages
// suppress = false - enables ui messages
internal void SuppressUIMessages(bool suppress)
{
  //NOTE(adm244): oblivion 1.2.416
  // QUIMsg_2PatchAddr = 0057ADD0
  // Original instruction: D9EE (fldz - (fpu x87) load +0.0)
  // Patch: 0xC390 (ret, nop)
  //
  // QUIMsg_PatchAddr = 0057ACC0
  // Original instruction: 51 (push eax)
  // Patch: 0xC3 (ret)

  //IMPORTANT(adm244): looks like SafeWrite16(..) is all messed up
  // it writes the highest byte twice instead of two bytes as it should.
  // am I missing something here?
  if( suppress ){
    SafeWrite8(showuimessage_patch_address, 0xC3);
    SafeWrite8(showuimessage_2_patch_address, 0xC3);
    SafeWrite8(showuimessage_2_patch_address + 1, 0x90);
  } else{
    SafeWrite8(showuimessage_patch_address, 0x51);
    SafeWrite8(showuimessage_2_patch_address, 0xD9);
    SafeWrite8(showuimessage_2_patch_address + 1, 0xEE);
  }
}

internal void GameLoop()
{
  if( IsActivated(VK_HOME, &isKeyHomeEnabled) ){
    QueueUIMessage("You've just pressed HOME key!", 0, 1, 2.0f);
    
    RunScriptLine("ts");
    
    SuppressUIMessages(true);
    RunBatchScript("obatronach.txt");
    SuppressUIMessages(false);
  }
  
  if( IsActivated(VK_END, &isKeyEndEnabled) ){
    QueueUIMessage_2("You've just pressed END key!", 2.0f, 0, "UIQuestUpdate");
    
    RunScriptLine("ts");
    
    SuppressUIMessages(true);
    RunBatchScript("oblvlup.txt");
    SuppressUIMessages(false);
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
    WriteRelJump(mainloop_hook_patch_address, (UInt32)&GameLoop_Hook);
  }

  return TRUE;
}

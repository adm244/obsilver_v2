//TODO(adm244): throw away unimportant stuff; code cleanup

//IMPORTANT(adm244): COPIED FROM OBSE SOURCE

#include <stdio.h>

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

enum FormType
{
  kFormType_None = 0,
  kFormType_TES4,
  kFormType_Group,
  kFormType_GMST,
  kFormType_Global,
  kFormType_Class,
  kFormType_Faction,
  kFormType_Hair,
  kFormType_Eyes,
  kFormType_Race,
  kFormType_Sound,
  kFormType_Skill,
  kFormType_Effect,
  kFormType_Script,
  kFormType_LandTexture,
  kFormType_Enchantment,
  kFormType_Spell,    // 10
  kFormType_BirthSign,
  kFormType_Activator,
  kFormType_Apparatus,
  kFormType_Armor,
  kFormType_Book,
  kFormType_Clothing,
  kFormType_Container,
  kFormType_Door,
  kFormType_Ingredient,
  kFormType_Light,
  kFormType_Misc,
  kFormType_Stat, // ???
  kFormType_Grass,
  kFormType_Tree,
  kFormType_Flora,
  kFormType_Furniture,  // 20
  kFormType_Weapon,
  kFormType_Ammo,
  kFormType_NPC,
  kFormType_Creature,
  kFormType_LeveledCreature,
  kFormType_SoulGem,
  kFormType_Key,
  kFormType_AlchemyItem,
  kFormType_SubSpace,
  kFormType_SigilStone,
  kFormType_LeveledItem,
  kFormType_SNDG,
  kFormType_Weather,
  kFormType_Climate,
  kFormType_Region,
  kFormType_Cell,     // 30
  kFormType_REFR,
  kFormType_ACHR,
  kFormType_ACRE,
  kFormType_PathGrid,
  kFormType_WorldSpace,
  kFormType_Land,
  kFormType_TLOD, // tile low lod?
  kFormType_Road,
  kFormType_Dialog,
  kFormType_DialogInfo,
  kFormType_Quest,
  kFormType_Idle,
  kFormType_Package,
  kFormType_CombatStyle,
  kFormType_LoadScreen,
  kFormType_LeveledSpell, // 40
  kFormType_ANIO,
  kFormType_WaterForm,
  kFormType_EffectShader,
  kFormType_TOFT
};

// 8
class String
{
public:
  String();
  ~String();

  char  * m_data;
  UInt16  m_dataLen;
  UInt16  m_bufLen;

  bool  Set(const char * src);
  bool  Includes(const char* toFind) const;
  bool  Replace(const char* toReplace, const char* replaceWith); // replaces instance of toReplace with replaceWith
  bool  Append(const char* toAppend);
  double  Compare(const String& compareTo, bool caseSensitive = false);
};

// 004
class BaseFormComponent
{
public:
  BaseFormComponent();
  ~BaseFormComponent();

  virtual void Destructor(void); // 00
  virtual void Unk_01(void);
  virtual void CopyFromBase(BaseFormComponent * component);

  //  void    ** _vtbl; // 000
};

// C
class TESFullName : public BaseFormComponent
{
public:
  TESFullName();
  ~TESFullName();

  String  name;   // 004
};

//Game / Editor
// 018 / 024
class TESForm : public BaseFormComponent
{
public:
  TESForm();
  ~TESForm();

  enum
  {
    kModified_FormFlags = 0x00000001,
      // CHANGE_FORM_FLAGS
      // can change 0x00112860 for TESObjectREFR, 0x0C20 for others
      // UInt32 flags;
  };

  enum
  {
    // ### need to figure out which objects these belong to
    kModified_GoldValue = 0x08,
    kModified_Name = 0x80
  };

  // TODO: determine which of these are in BaseFormComponent
  virtual bool  Unk_03(BaseFormComponent * arg);  // return false if equal - compares names, typeID, flags
  virtual void  Destroy(bool noDealloc);  // delete form? pass false to free the object's memory
  virtual void  Unk_05(void); // destroy form components
  virtual void  Unk_06(void); // call unk01 on form components
  //virtual bool  LoadForm(ModEntry* modEntry);
  virtual bool  LoadForm(void* modEntry);
  virtual void  Unk_08(void); // calls load form
  virtual void  Unk_09(void); // save form
  virtual void  Unk_0A(void); // calls save form
  virtual void  Unk_0B(void);
  virtual void  Unk_0C(void);
  virtual void  Unk_0D(void);
  virtual void  Unk_0E(void);
  virtual void  Unk_0F(void);
  virtual void  MarkAsModified(UInt32 mask);  // 10
  virtual void  ClearModified(UInt32 mask);
  virtual void  Unk_12(void);
  virtual UInt32  GetSaveSize(UInt32 modifiedMask);
  virtual void  SaveGame(UInt32 modifiedMask);  // output buffer not passed as param, accessed indirectly via g_createdBaseObjList
  virtual void  LoadGame(UInt32 modifiedMask, UInt32 unk);
  virtual void  Unk_16(void); // post-load fixup (convert refids in to pointers, etc)
  virtual void  Unk_17(void);
  virtual void  Unk_18(void);
  virtual void  Unk_19(void);
  virtual void  Unk_1A(void);
  virtual void  DoPostFixup(void);  // initialize form after other forms loaded
  virtual void  Unk_1C(void);
  virtual void  GetDescription(String * dst);
  virtual void  Unk_1E(void);
  virtual void  Unk_1F(void);
  virtual void  Unk_20(void); // 20
  virtual void  Unk_21(void);
  virtual void  Unk_22(void);
  virtual void  Unk_23(void);
  virtual void  Unk_24(UInt8 arg);
  virtual void  SetQuestItem(bool isQuestItem);
  virtual void  Unk_26(void);
  virtual void  Unk_27(void);
  virtual void  Unk_28(void);
  virtual bool  Unk_29(void);
  virtual bool  Unk_2A(void); // return true if the form is a reference?
  virtual bool  Unk_2B(void);
  virtual void  Unk_2C(void);
  virtual void  CopyFrom(TESForm * form);
  virtual bool  Compare(TESForm * form);
  virtual void  Unk_2F(void);
  virtual void  Unk_30(void); // 30
  virtual void  Unk_31(void);
  virtual void  Unk_32(void);
  //virtual bool  Unk_33(TESObjectREFR* refr0, TESObjectREFR* refr1, UInt32 unk2); // related to activate, refr1 is activating refr, refr0 is a reference to this TESForm being activated, seen unk2 == 0
  virtual bool  Unk_33(void* refr0, void* refr1, UInt32 unk2); // related to activate, refr1 is activating refr, refr0 is a reference to this TESForm being activated, seen unk2 == 0
  virtual const char *  GetName(void);  // not sure which objects this works on, doesn't seem to work on player or random objects
  virtual const char *  GetEditorName(void);  // returns nothing at run-time
  virtual void  SetName(const char * name);

  struct EditorData {
    String    editorID;
    UInt32    unk08;
  };

  enum
  {
    kFormFlags_QuestItem =        0x00000400,
    kFormFlags_IgnoresFriendlyHits =  0x00100000
  };

  struct ModReferenceList
  {
    //ModEntry      * data;
    void      * data;
    ModReferenceList  * next;

    //ModEntry      * Info() const  { return data; }
    void      * Info() const  { return data; }
    ModReferenceList  * Next() const  { return next; }
  };

  UInt8 typeID;         // 004
  UInt8 typeIDPad[3];     // 005
  UInt32  flags;          // 008
                    // 00000040 - something (TESForm_Fn26)
                    // 00000400 - quest item
                    // 00004000 - temporary?
                    // 00010000 - something (TESForm_Fn27)
                    // 00020000 - something (TESForm_Fn28)
                    // 00080000 - something (sub_464A60)
                    // 00100000 - something (sub_464A30)
  UInt32  refID;          // 00C
  ModReferenceList  modRefList; // 010 / 01C

  bool  IsQuestItem() const;
  bool  IsCloned() const;
  bool  SupportsSimpleModel() const;
  UInt8 GetModIndex();
  void  MarkAsTemporary(void);
  bool  IsReference() {  return typeID >= kFormType_REFR && typeID <= kFormType_ACRE; }
  TESForm * TryGetREFRParent(void);
  TESFullName* GetFullName();
  const char* GetEditorID();
  bool  IsInventoryObject() const;
};

#define MEMBER_FN_PREFIX1(className)  \
  typedef className _MEMBER_FN_BASE_TYPE

// 50 / 54
class Script : public TESForm
{
public:
  // no changed flags (TESForm flags)
  //MEMBER_FN_PREFIX1(Script);

  Script();
  ~Script();

  // members

  struct RefVariable
  {
    String  name;   // variable name/editorID (not used at run-time)
    TESForm * form;
    UInt32  varIdx;   // always zero in editor

    //void  Resolve(ScriptEventList * eventList);
    void  Resolve(void * eventList);
  };

  struct RefListEntry
  {
    RefVariable   * var;
    RefListEntry  * next;

    RefVariable* Info() const { return var; }
    RefListEntry* Next() const { return next; }
    void SetNext(RefListEntry* nextEntry) { next = nextEntry; }
    RefVariable* GetRefVariableByName(const char* name);
    UInt32 GetIndex(RefVariable* refVar);
  };

  //typedef Visitor<RefListEntry, RefVariable> RefListVisitor;

  enum {
    eVarType_Float = 0,     //ref is also zero
    eVarType_Integer,

    // OBSE, return values only
    eVarType_String,
    eVarType_Array,
    eVarType_Ref,

    eVarType_Invalid
  };

  struct VariableInfo
  {
    UInt32      idx;    // 00
    UInt32      pad04;    // 04
    double      data;   // 08
    UInt8     type;   // 10
    UInt8     pad11[3]; // 11
    UInt32      unk14;    // 14
    String      name;   // 18
  };

  struct VarInfoEntry
  {
    VariableInfo  * data;
    VarInfoEntry  * next;

    VariableInfo* Info() const { return data; }
    VarInfoEntry* Next() const { return next; }

    VariableInfo* GetVariableByName(const char* name);
  };
  //typedef Visitor<VarInfoEntry, VariableInfo> VarListVisitor;

  // 14
  struct ScriptInfo
  {
    UInt32  unk0;   // 00 
    UInt32  numRefs;  // 04 
    UInt32  dataLength; // 08 
    UInt32  varCount; // 0C 
    UInt32  type;   // 10 
  };

  enum {
    eType_Object = 0,
    eType_Quest = 1,
    eType_Magic = 0x100
  };

  ScriptInfo    info;         // 018 / 024
  char      * text;         // 02C / 038
  void      * data;         // 030 / 03C
  UInt32      unk34;          // 034
  float     questDelayTimeCounter;  // 038      - init'd to fQuestDelayTime, decremented by frametime each frame
  float     secondsPassed;      // 03C      - only if you've modified fQuestDelayTime
  RefListEntry  refList;        // 040 / 040 - ref variables and immediates
  VarInfoEntry  varList;        // 048 / 048 - local variable list

  RefVariable * GetVariable(UInt32 reqIdx);
  VariableInfo* GetVariableInfo(UInt32 idx);

  UInt32      AddVariable(TESForm * form);
  void      CleanupVariables(void);

  UInt32      Type() const { return info.type; }
  bool      IsObjectScript() const {return info.type == eType_Object; }
  bool      IsQuestScript() const { return info.type == eType_Quest; }
  bool      IsMagicScript() const { return info.type == eType_Magic; }
  VariableInfo* GetVariableByName(const char* varName);
  UInt32      GetVariableType(VariableInfo* var);

  // arg3 appears to be true for result scripts (runs script even if dataLength <= 4)
  //DEFINE_MEMBER_FN(Execute, bool, kScript_ExecuteFnAddr, TESObjectREFR* thisObj, ScriptEventList* eventList, TESObjectREFR* containingObj, bool arg3);
  void      Constructor(void);
  void      StaticDestructor(void);
  void      SetText(const char * buf);
  //ScriptEventList * CreateEventList();
  void * CreateEventList();
  bool      CompileAndRun(void * unk0, UInt32 unk1, void * unk2);
};

// -------------------------------------------------------------------------
internal void SafeWrite8(UInt32 addr, UInt32 data)
{
  UInt32 oldProtect;

  VirtualProtect((void *)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
  *((UInt8 *)addr) = data;
  VirtualProtect((void *)addr, 4, oldProtect, &oldProtect);
}

internal void SafeWrite32(UInt32 addr, UInt32 data)
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

__forceinline UInt32 ThisStdCall(UInt32 _f,void* _t)
{
    class T {}; union { UInt32 x; UInt32 (T::*m)(); } u = { _f };
    return ((T*)_t->*u.m)();
}

template <typename T1>
__forceinline UInt32 ThisStdCall(UInt32 _f,void* _t,T1 a1)
{
    class T {}; union { UInt32 x; UInt32 (T::*m)(T1); } u = { _f };
    return ((T*)_t->*u.m)(a1);
}

template <typename T1,typename T2,typename T3>
__forceinline UInt32 ThisStdCall(UInt32 _f,void* _t,T1 a1,T2 a2, T3 a3)
{
    class T {}; union { UInt32 x; UInt32 (T::*m)(T1,T2,T3); } u = { _f };
    return ((T*)_t->*u.m)(a1,a2,a3);
}

bool Script::CompileAndRun(void * unk0, UInt32 unk1, void * unk2)
{
  return ThisStdCall(0x004FBF00, this, unk0, unk1, unk2) ? true : false;
}

typedef void * (* _GetGlobalScriptStateObj)(void);
extern const _GetGlobalScriptStateObj GetGlobalScriptStateObj;
const _GetGlobalScriptStateObj GetGlobalScriptStateObj = (_GetGlobalScriptStateObj)0x00585C10;

// unk1 = 0, unk2 = 1
typedef bool (* _QueueUIMessage)(const char * string, UInt32 unk1, UInt32 unk2, float duration);
extern const _QueueUIMessage QueueUIMessage;
const _QueueUIMessage QueueUIMessage = (_QueueUIMessage)0x0057ACC0;

//displays icon and plays sound (used by Additem, Addspell, etc...)
//ddsPath relative to Textures\Menus\...  soundID as defined in the CS
typedef bool (* _QueueUIMessage_2)(const char * string, float duration, const char * ddsPath, const char * soundID);
extern const _QueueUIMessage_2 QueueUIMessage_2;
const _QueueUIMessage_2 QueueUIMessage_2 = (_QueueUIMessage_2)0x0057ADD0;

void Script::Constructor(void)
{
  ThisStdCall(0x004FBAA0, this);
}

void TESForm::MarkAsTemporary(void)
{
  // only tested for Script objects in 'con_bat'
  ThisStdCall(0x0046B590, this);
}

void Script::StaticDestructor(void)
{
  ThisStdCall(0x004FC980, this);
}

void Script::SetText(const char * buf)
{
  ThisStdCall(0x004F9E50, this, buf);
}

bool RunScriptLine(const char* buf)
{
  // create a Script object
  UInt8 scriptObjBuf[sizeof(Script)];
  Script  * tempScriptObj = (Script *)scriptObjBuf;

  void  * scriptState = GetGlobalScriptStateObj();

  tempScriptObj->Constructor();
  tempScriptObj->MarkAsTemporary();
  tempScriptObj->SetText(buf);
  bool bResult = tempScriptObj->CompileAndRun(*((void**)scriptState), 1, NULL);
  tempScriptObj->StaticDestructor();

  return bResult;
}

bool RunBatchScript(char *filename)
{
  bool result = false;

  FILE *src = NULL;
  fopen_s(&src, filename, "r");
  
  if( src ){
    char line[4096];
    
    while( fgets(line, sizeof(line), src) ){
      UInt32 lineLen = strlen(line);
      
      if( lineLen > 1 ){
        if(line[lineLen - 1] == '\n'){
          line[lineLen - 1] = 0;
        }

        result = RunScriptLine(line);
        if( !result ){
          break;
        }
      }
    }

    fclose(src);
  }

  return result;
}
// -------------------------------------------------------------------------

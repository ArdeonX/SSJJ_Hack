#include "Engine.h"
spin_mutex* Lock = new spin_mutex;
extern Memory* g_Memory;
extern Config* g_Config;
DWORD BoneKey[16] = { 0 };
namespace GameInfo {
	DWORD Module;
	HWND hWindow;
	Math::Vector2 ScreenSize;
	float Matrix[4][4];
}
namespace Offsets {
	DWORD SharedInstance;
	DWORD Camera;

	DWORD Player;
	DWORD UserCommand;
	DWORD WorldCamera;
	DWORD UIEntityArray;

	DWORD Entities;
	DWORD Slots;
	DWORD Count;

	DWORD Info;
	DWORD Yaw;
	DWORD Pitch;
	DWORD Fov;

	DWORD Animator;
	DWORD mainCamera;

	DWORD Current;
	DWORD _playerName;
	DWORD _currentWeapon;
	DWORD _currentWeaponName;
	DWORD _isDead;
	DWORD _hp;
	DWORD _punchYaw;
	DWORD _punchPitch;
	DWORD _punchRoll;
	DWORD _team;
	DWORD _currentBagId;
	DWORD _hasC4;
	DWORD _maxHp;
	DWORD _isBot;
	DWORD Name;
	namespace Method {
		void* get_input;
		void* GetBone_PlayerEntity;
		void* GetBone_SJAnimator;
		void* get_position_1;
		void* get_position_2;
		void* set_isThirdPersonView;
		void* set_isFirstPersonView;
		void* set_ThirdMode;
		void* get_skill;
		void* get_unityObjects;
		void* Rotate;
		void* ProcessUiTouch;
		void* UiIEventCondition_Init;
	}

}
auto Engine::GetScreenX()->float {
	return GameInfo::ScreenSize.x;
}
auto Engine::GetScreenY()->float {
	return GameInfo::ScreenSize.y;
}
auto Engine::Init()->bool {


	GameInfo::Module = (DWORD)GetModuleHandleA(_xor_("SSJJ_BattleClient_Unity.exe"));
	while (!GameInfo::Module)
	{
		GameInfo::Module = (DWORD)GetModuleHandleA(_xor_("SSJJ_BattleClient_Unity.exe"));
		this_thread::sleep_for(std::chrono::milliseconds(500));
	}


	GameInfo::hWindow = FindWindowA(_xor_("UnityWndClass"), _xor_("生死狙击微端战斗"));
	while (!GameInfo::hWindow)
	{
		GameInfo::hWindow = FindWindowA(_xor_("UnityWndClass"), _xor_("生死狙击微端战斗"));
		this_thread::sleep_for(std::chrono::milliseconds(500));
	}

//GetWindow:
//	RECT Rect = {};
//	GetClientRect(GameInfo::hWindow, &Rect);
//	auto Width = Rect.right - Rect.left;
//	auto Height = Rect.bottom - Rect.top;
//	GameInfo::ScreenSize.x = Width / 2;
//	GameInfo::ScreenSize.y = Height / 2;
//	if (GameInfo::ScreenSize.x <= 0.0f || GameInfo::ScreenSize.y <= 0.0f) {
//		this_thread::sleep_for(std::chrono::milliseconds(500));
//		goto GetWindow;
//	}
	GameInfo::ScreenSize.x = (float)GetSystemMetrics(SM_CXSCREEN) * 0.5f;
	GameInfo::ScreenSize.y = (float)GetSystemMetrics(SM_CYSCREEN) * 0.5f;

GetCamera:

	Offsets::Camera = GameInfo::Module + 0x1000708;
	Offsets::Camera = g_Memory->Read<DWORD>(Offsets::Camera);
	Offsets::Camera = g_Memory->Read<DWORD>(Offsets::Camera + 0x10) + 0x2AC;
	if (Offsets::Camera < 0xC60000) {
		this_thread::sleep_for(std::chrono::milliseconds(500));
		goto GetCamera;
	}
	Engine::InitBoneName();
	return true;

}
auto Engine::WorldtoScreen(Math::Vector3* Coordinate, Math::Vector4* ScreenBox, int* Dis)->bool {
	float ViewX, ViewY, ViewZ, suo, ViewY2{};

	ViewZ = GameInfo::Matrix[0][2] * Coordinate->x + GameInfo::Matrix[1][2] * Coordinate->y + GameInfo::Matrix[2][2] * Coordinate->z + GameInfo::Matrix[3][2];
	suo = 1 / ViewZ;
	*Dis = (int)ViewZ / 100;
	if (*Dis <= 0.0f)
		return false;
	ViewX = GameInfo::ScreenSize.x + (GameInfo::Matrix[0][0] * Coordinate->x + GameInfo::Matrix[1][0] * Coordinate->y + GameInfo::Matrix[2][0] * Coordinate->z + GameInfo::Matrix[3][0]) * suo * GameInfo::ScreenSize.x;
	ViewY = GameInfo::ScreenSize.y - (GameInfo::Matrix[0][1] * Coordinate->x + GameInfo::Matrix[1][1] * (Coordinate->y + 175.0f) + GameInfo::Matrix[2][1] * Coordinate->z + GameInfo::Matrix[3][1]) * suo * GameInfo::ScreenSize.y;
	ViewY2 = GameInfo::ScreenSize.y - (GameInfo::Matrix[0][1] * Coordinate->x + GameInfo::Matrix[1][1] * (Coordinate->y - 1.4f) + GameInfo::Matrix[2][1] * Coordinate->z + GameInfo::Matrix[3][1]) * suo * GameInfo::ScreenSize.y;
	ScreenBox->x = ViewX - (ViewY2 - ViewY) * 0.25f;
	ScreenBox->y = ViewY + ((ViewY2 - ViewY) * 0.85f);
	ScreenBox->w = (ViewY2 - ViewY) * 0.5f;
	ScreenBox->h = ViewY2 - ViewY;
	return true;
}
auto Engine::WorldtoScreen(Math::Vector3* Coordinate, Math::Vector2* ScreenCoordinate)->bool {
	float ViewZ, Scale{};
	ViewZ = GameInfo::Matrix[0][2] * Coordinate->x + GameInfo::Matrix[1][2] * Coordinate->y + GameInfo::Matrix[2][2] * Coordinate->z + GameInfo::Matrix[3][2];
	Scale = 1 / ViewZ;
	if ((Scale / 100.0f) <= 0)
		return false;
	ScreenCoordinate->x = GameInfo::ScreenSize.x + (GameInfo::Matrix[0][0] * Coordinate->x + GameInfo::Matrix[1][0] * Coordinate->y + GameInfo::Matrix[2][0] * Coordinate->z + GameInfo::Matrix[3][0]) * Scale * GameInfo::ScreenSize.x;
	ScreenCoordinate->y = GameInfo::ScreenSize.y - (GameInfo::Matrix[0][1] * Coordinate->x + GameInfo::Matrix[1][1] * Coordinate->y + GameInfo::Matrix[2][1] * Coordinate->z + GameInfo::Matrix[3][1]) * Scale * GameInfo::ScreenSize.y;
	return true;
}
auto Engine::Refresh()->void {

	memcpy(&GameInfo::Matrix, (void*)Offsets::Camera, 64); //4*4*4=64

}
auto  Engine::GetCameraPos()->Math::Vector3 {
	Math::Vector3 Temp{};

	Temp =g_Memory->Read<Math::Vector3>(Offsets::Camera + 0xF0);
	
	return Temp;
}
auto Engine::GetCenterDistance(float x, float y)->float
{
	float v1 = x - GameInfo::ScreenSize.x;
	float v2 = y - GameInfo::ScreenSize.y;
	return sqrt(v1 * v1 + v2 * v2);
}
auto Engine::CalcAim(Math::Vector3* MyPos, Math::Vector3* AimPos, Math::Vector2* Mouse)->bool
{

	Math::Vector3 Aim{};
	Aim.x = MyPos->x - AimPos->x;
	Aim.y = MyPos->y - AimPos->y;
	Aim.z = MyPos->z - AimPos->z;

	if (Aim.x >= 0 && Aim.z >= 0)
	{
		Mouse->x = (float)atanf(Aim.z / Aim.x) / (float)PI * 180.0f + 90.0f;
	}
	else if (Aim.x <= 0 && Aim.z >= 0)
	{
		Mouse->x = (float)atanf(Aim.z / Aim.x) / (float)PI * 180.0f - 90.0f;
	}
	else if (Aim.x <= 0 && Aim.z <= 0)
	{
		Mouse->x = (float)atanf(Aim.z / Aim.x) / (float)PI * 180.0f - 90.0f;
	}
	else if (Aim.x >= 0 && Aim.z <= 0)
	{
		Mouse->x = (float)atanf(Aim.z / Aim.x) / (float)PI * 180.0f + 90.0f;
	}
	else {
		return false;
	}
	Mouse->y = (float)-atanf(Aim.y / sqrt(Aim.x * Aim.x + Aim.z * Aim.z)) / (float)PI * 180.0f;
	return true;


}
auto Engine::GetContexts()->Contexts* {
	Contexts* pContext = g_Memory->Read<Contexts*>(Offsets::SharedInstance);
	return pContext;
}
auto Engine::CheckOffsets()->bool {
	if (!Offsets::SharedInstance) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::SharedInstance"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Player) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Player"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::UserCommand) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::UserCommand"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::WorldCamera) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::WorldCamera"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::mainCamera) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::mainCamera"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Entities) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Entities"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Slots) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Slots"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Count) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Count"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Current) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Current"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Info) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Info"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Yaw) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Yaw"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Pitch) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Pitch"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Fov) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Fov"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_playerName) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_playerName"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_currentWeapon) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_currentWeapon"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_currentWeaponName) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_currentWeaponName"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_isDead) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_isDead"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_hp) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_hp"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_punchYaw) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_punchYaw"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_punchPitch) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_punchPitch"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_punchRoll) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_punchRoll"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_team) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_team"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_currentBagId) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_currentBagId"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_hasC4) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_hasC4"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_maxHp) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_maxHp"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::_isBot) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::_isBot"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Name) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Name"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Method::get_input) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::get_input"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Method::GetBone_PlayerEntity) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::GetBone_PlayerEntity"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Method::GetBone_SJAnimator) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::GetBone_SJAnimator"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Method::get_position_1) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::get_position_1"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Method::get_position_2) {
	Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::get_position_2"), GetCurrentProcessId());
	return false;
	}
	else if (!Offsets::Method::set_isFirstPersonView) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::set_isFirstPersonView"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Method::set_isThirdPersonView) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::set_isThirdPersonView"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Method::get_skill) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::get_skill"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Method::get_unityObjects) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::get_unityObjects"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Method::Rotate) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::Rotate"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Method::ProcessUiTouch) {
		Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::ProcessUiTouch"), GetCurrentProcessId());
		return false;
	}
	else if (!Offsets::Method::UiIEventCondition_Init) {
	    Log(_xor_("[-]Thrad:%x NULL: Offsets::Method::UiIEventCondition_Init"), GetCurrentProcessId());
	}
	return true;

}
auto Engine::GetOffsets()->bool {

	Offsets::SharedInstance = Mono::GetStaticFieldValue(_xor_("SSJJEntitas_Library.dll"), _xor_("Contexts"), _xor_("_sharedInstance"));

	Offsets::Player = Mono::GetFieldOffset(_xor_("SSJJEntitas_Library.dll"), _xor_("Contexts"), _xor_("<player>k__BackingField"), false);
	Offsets::UserCommand = Mono::GetFieldOffset(_xor_("SSJJEntitas_Library.dll"), _xor_("Contexts"), _xor_("<userCommand>k__BackingField"), false);
	Offsets::WorldCamera = Mono::GetFieldOffset(_xor_("SSJJEntitas_Library.dll"), _xor_("Contexts"), _xor_("<worldCamera>k__BackingField"), false);

	Offsets::Entities = Mono::GetFieldOffset(_xor_("Entitas.dll"), _xor_("IContext`1"), _xor_("_entities"), false);
	Offsets::Slots = Mono::GetFieldOffset(_xor_("System.Core.dll"), _xor_("HashSet`1"), _xor_("slots"), false);
	Offsets::Count = Mono::GetFieldOffset(_xor_("System.Core.dll"), _xor_("HashSet`1"), _xor_("count"), false);

	Offsets::Info = Mono::GetFieldOffset(_xor_("SSJJEntitas_Library.dll"), _xor_("SkillComponent"), _xor_("Info"), false);
	Offsets::Fov = Mono::GetFieldOffset(_xor_("SSJJEntitas_Library.dll"), _xor_("InputComponent"), _xor_("Fov"), false);
	Offsets::Pitch = Mono::GetFieldOffset(_xor_("SSJJEntitas_Library.dll"), _xor_("InputComponent"), _xor_("Pitch"), false);
	Offsets::Yaw = Mono::GetFieldOffset(_xor_("SSJJEntitas_Library.dll"), _xor_("InputComponent"), _xor_("Yaw"), false);

	Offsets::mainCamera = Mono::GetFieldOffset(_xor_("SSJJEntitas_Library.dll"), _xor_("UnityObjectsComponent"), _xor_("mainCamera"), false);
	Offsets::Animator = Mono::GetFieldOffset(_xor_("SSJJEntitas_Library.dll"), _xor_("ThirdPersonUnityObjectsComponent"), _xor_("Animator"), false);
	Offsets::Current = Mono::GetFieldOffset(_xor_("SSJJEntitas_Library.dll"), _xor_("BasicInfoComponent"), _xor_("Current"), false);

	Offsets::_playerName = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_playerName"), false);
	Offsets::_currentWeapon = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_currentWeapon"), false);
	Offsets::_currentWeaponName = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_currentWeaponName"), false);
	Offsets::_isDead = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_isDead"), false);
	Offsets::_hp = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_hp"), false);
	Offsets::_punchYaw = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_punchYaw"), false);
	Offsets::_punchPitch = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_punchPitch"), false);
	Offsets::_punchRoll = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_punchRoll"), false);
	Offsets::_team = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_team"), false);
	Offsets::_currentBagId = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_currentBagId"), false);
	Offsets::_hasC4 = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_hasC4"), false);
	Offsets::_maxHp = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_maxHp"), false);
	Offsets::_isBot = Mono::GetFieldOffset(_xor_("SSJJ.Contract.dll"), _xor_("PlayerEntityData"), _xor_("_isBot"), false);
	Offsets::Name = Mono::GetFieldOffset(_xor_("SSJJEntitas_Library.dll"), _xor_("CurrentWeaponComponent"), _xor_("Name"), false);



	Offsets::Method::GetBone_SJAnimator = Mono::GetMethod(Mono::GetImage(_xor_("SSJJAnim_Library")), _xor_("Assets.Libs.SJAnimation.Animator"), _xor_("SJAnimator"), _xor_("GetBone"));


	Offsets::Method::get_input = Mono::GetMethod(Mono::GetImage(_xor_("SSJJEntitas_Library")), "", _xor_("UserCommandEntity"), _xor_("get_input"));


	Offsets::Method::GetBone_PlayerEntity = Mono::GetMethod(Mono::GetImage(_xor_("SSJJEntitas_Library")), "", _xor_("PlayerEntity"), _xor_("GetBone"));
	Offsets::Method::get_position_1 = Mono::GetMethod(Mono::GetImage(_xor_("UnityEngine")), _xor_("UnityEngine"), _xor_("Transform"), _xor_("INTERNAL_get_position"));
	Offsets::Method::get_position_2 = (void*)(GameInfo::Module + 0x6DC040);

	Offsets::Method::set_isThirdPersonView = Mono::GetMethod(Mono::GetImage(_xor_("SSJJEntitas_Library")), "", _xor_("PlayerEntity"), _xor_("set_isThirdPersonView"));
	Offsets::Method::set_isFirstPersonView = Mono::GetMethod(Mono::GetImage(_xor_("SSJJEntitas_Library")), "", _xor_("PlayerEntity"), _xor_("set_isFirstPersonView"));
	Offsets::Method::set_ThirdMode = Mono::GetMethod(Mono::GetImage(_xor_("SSJJ.Contract")), _xor_("NetData"), _xor_("PlayerInfoData"), _xor_("set_ThirdMode"));
	Offsets::Method::get_skill = Mono::GetMethod(Mono::GetImage(_xor_("SSJJEntitas_Library")), "", _xor_("PlayerEntity"), _xor_("get_skill"));
	Offsets::Method::get_unityObjects = Mono::GetMethod(Mono::GetImage(_xor_("SSJJEntitas_Library")), "", _xor_("WorldCameraEntity"), _xor_("get_unityObjects"));
	Offsets::Method::Rotate = Mono::GetMethod(Mono::GetImage(_xor_("UnityEngine")), _xor_("UnityEngine"), _xor_("Transform"), _xor_("Rotate"), 3);
	Offsets::Method::ProcessUiTouch = Mono::GetMethod(Mono::GetImage(_xor_("Assembly-CSharp")), _xor_("Assets.Sources.Systems.UserCommand"), _xor_("ComputeUserCommandSystem"), _xor_("ProcessUiTouch"), 2);
	Offsets::Method::UiIEventCondition_Init = Mono::GetMethod(Mono::GetImage(_xor_("Assembly-CSharp")), _xor_("Assets.Sources.Modules.Ui.UiEventCondition"), _xor_("UiIEventCondition"), _xor_("Init"));
	
	

GetUIEntity:
	auto TryAddress = g_Memory->Read<DWORD>((DWORD)Offsets::Method::UiIEventCondition_Init + 0x2C);
	if (IsBadReadPtr((void*)TryAddress, 4) || g_Memory->Read<DWORD>(g_Memory->Read<DWORD>(TryAddress) + 0x20) <= 0) {
		this_thread::sleep_for(std::chrono::microseconds(500));
		goto GetUIEntity;
	}
	Offsets::UIEntityArray = TryAddress;

	return true;
}
auto Engine::InitBoneName()->void {
	const wchar_t* BoneText[16] = { L"Bip01_Head", L"Bip01_Neck", L"Bip01_Spine", L"Bip01_Pelvis", L"Bip01_L_UpperArm", L"Bip01_L_Forearm", L"Bip01_L_Hand", L"Bip01_R_UpperArm", L"Bip01_R_Forearm", L"Bip01_R_Hand", L"Bip01_L_Thigh", L"Bip01_L_Calf",L"Bip01_L_Foot", L"Bip01_R_Thigh", L"Bip01_R_Calf", L"Bip01_R_Foot" };
	for (int i = 0; i < 16; i++)
	{
		DWORD NewAD = (DWORD)VirtualAllocEx(GetCurrentProcess(), NULL, 48, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		int Textlen = lstrlenW(BoneText[i]);
		BoneKey[i] = NewAD;
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)(NewAD + 8), &Textlen, sizeof(int), NULL);
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)(NewAD + 12), BoneText[i], Textlen * 2, NULL);
	}
}
auto Engine::SetRightBottonProcessUiTouch(bool DoLock)->bool {
	if (DoLock) {
		g_Memory->Write<BYTE>((DWORD)Offsets::Method::ProcessUiTouch + 0x7C, { 0x71 });//改为jno
	}
	else {
		g_Memory->Write<BYTE>((DWORD)Offsets::Method::ProcessUiTouch + 0x7C, { 0x74 });//改回jn
	}
	
	return true;
}
auto  Engine::Other_GetOwnEntity()->PlayerEntity* {
	return  g_Memory->Read<PlayerEntity*>(Offsets::UIEntityArray + 0x4);
}
auto   Engine::Other_GetCount() ->DWORD {
	return g_Memory->Read<DWORD>(g_Memory->Read<DWORD>(Offsets::UIEntityArray) + 0x20);
}
auto  Engine::Other_GetPlayerEntity(DWORD Count)->PlayerEntity* {
	return g_Memory->Read<PlayerEntity*>(
		g_Memory->Read<DWORD>(
			g_Memory->Read<DWORD>(Offsets::UIEntityArray)
			+ 0x14)
		+ 0x10 + Count * 0x4);
}
auto SkillComponent::GetInfo()->PlayerInfoData* {
	auto PlayerInfo = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Offsets::Info);//0x8
	return (PlayerInfoData*)PlayerInfo;
}


auto InputComponent::GetYaw()->float
{
	auto Yaw = g_Memory->Read<float>(reinterpret_cast<DWORD>(this) + Offsets::Yaw);
	return Yaw;
}
auto InputComponent::GetPitch()->float
{
	auto Yaw = g_Memory->Read<float>(reinterpret_cast<DWORD>(this) + Offsets::Pitch);
	return Yaw;
}
auto InputComponent::GetFov()->float
{
	auto Yaw = g_Memory->Read<float>(reinterpret_cast<DWORD>(this) + Offsets::Fov);
	return Yaw;

}
auto InputComponent::SetYaw(float Yaw)->void
{
	
	g_Memory->Write<float>(reinterpret_cast<DWORD>(this) + Offsets::Yaw, Yaw);
	return;
}
auto InputComponent::SetPitch(float Pitch)->void
{

	g_Memory->Write<float>(reinterpret_cast<DWORD>(this) + Offsets::Pitch, Pitch);
	return;
}

auto Camera::Rotate(float xAngle, float yAngle, float zAngle)->void {
	if (Offsets::Method::Rotate) {
		FRotate gRotate = (FRotate)Offsets::Method::Rotate;
		gRotate(this, xAngle, yAngle, zAngle);
	}
	return;
}

auto UserCommandEntity::GetInput()->InputComponent* {
	if (Offsets::Method::get_input) {
		get_input Call_get_input = (get_input)Offsets::Method::get_input;
		auto pInput = Call_get_input(this);
		return pInput;
	}
	return nullptr;

}
auto UnityObjectsComponent::GetCamera()->Camera* {
	auto pmainCamera = g_Memory->Read<Camera*>(reinterpret_cast<DWORD>(this) + Offsets::mainCamera);
	return pmainCamera;
}
auto WorldCameraContext::GetUnityObjects()->UnityObjectsComponent* {
	if (Offsets::Method::get_unityObjects) {
		get_unityObjects Call_get_unityObjects = (get_unityObjects)Offsets::Method::get_unityObjects;
		auto pUobj = Call_get_unityObjects(this);
		return pUobj;
	}
	return nullptr;
}
std::string PlayerEntity::UnicodeToUtf8(wchar_t* wstr)
{
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
	char* pAssii = (char*)malloc(sizeof(char) * ansiiLen);//申请内存
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);//释放内存
	return ret_str;
}

auto  PlayerEntity::SetThirdPerson()->void {
	if (g_Config->HaveThirdMode)
		return;
	if (Offsets::Method::set_isFirstPersonView && Offsets::Method::set_isThirdPersonView &&
		Offsets::Method::get_skill && Offsets::Method::set_ThirdMode)
	{
		SetValue Call_set_isFirstPersonView = (SetValue)Offsets::Method::set_isFirstPersonView;
		SetValue Call_set_isThirdPersonView = (SetValue)Offsets::Method::set_isThirdPersonView;
		SetValue Call_set_ThirdMode = (SetValue)Offsets::Method::set_ThirdMode;
		GetValue Call_get_skill = (GetValue)Offsets::Method::get_skill;
		auto pSkill = (SkillComponent*)Call_get_skill(this);
		auto pInfo = pSkill->GetInfo();
		Call_set_ThirdMode(pInfo, true);
		Call_set_isFirstPersonView(this, false);
		Call_set_isThirdPersonView(this, true);
	}

	g_Config->HaveThirdMode = true;
}

auto  PlayerEntity::SetFirstPerson()->void {
	if (!g_Config->HaveThirdMode)
		return;
	if (Offsets::Method::set_isFirstPersonView && Offsets::Method::set_isThirdPersonView &&
		Offsets::Method::get_skill && Offsets::Method::set_ThirdMode)
	{
		SetValue Call_set_isFirstPersonView = (SetValue)Offsets::Method::set_isFirstPersonView;
		SetValue Call_set_isThirdPersonView = (SetValue)Offsets::Method::set_isThirdPersonView;
		SetValue Call_set_ThirdMode = (SetValue)Offsets::Method::set_ThirdMode;
		GetValue Call_get_skill = (GetValue)Offsets::Method::get_skill;
		auto pSkill = (SkillComponent*)Call_get_skill(this);
		auto pInfo = pSkill->GetInfo();


		Call_set_ThirdMode(pInfo, false);
		Call_set_isFirstPersonView(this, true);
		Call_set_isThirdPersonView(this, false);

		
	}

	g_Config->HaveThirdMode = false;
}
auto  PlayerEntity::GetAnimator()->DWORD {
	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + ThirdPersonUnityObjectsComponent);
	auto pAnimator = g_Memory->Read<DWORD>(pTemp + Offsets::Animator);
	return pAnimator;
}
auto  PlayerEntity::GetPlayerName()->std::string {
	std::string Result = "";
	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + BasicInfo);
	pTemp = g_Memory->Read<DWORD>(pTemp + Offsets::Current);
	auto pPlayerName = g_Memory->Read<Ustring*>(pTemp + Offsets::_playerName);

	wchar_t* String = new wchar_t[pPlayerName->Length * 2 + 1];
	String[pPlayerName->Length] = '\0';//手动截断
	memcpy(String, &pPlayerName->Value, pPlayerName->Length * 2);
	Result = this->UnicodeToUtf8(String);
	delete[] String;
	return Result;
}
auto  PlayerEntity::GetWeaponName()->std::string {
	std::string Result = "";
	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + CurrentWeaponComponent);
	auto pWeaponName = g_Memory->Read<Ustring*>(pTemp + Offsets::Name);

	wchar_t* String = new wchar_t[pWeaponName->Length * 2 + 1];
	String[pWeaponName->Length] = '\0';//手动截断
	memcpy(String, &pWeaponName->Value, pWeaponName->Length * 2);
	Result = this->UnicodeToUtf8(String);
	delete[] String;
	return Result;
}
auto  PlayerEntity::GetCurrentWeapon()->int {
	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + BasicInfo);
	pTemp = g_Memory->Read<DWORD>(pTemp + Offsets::Current);
	auto pcurrentWeapon = g_Memory->Read<int>(pTemp + Offsets::_currentWeapon);
	return pcurrentWeapon;
}
auto  PlayerEntity::GetHp()->int {
	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + BasicInfo);
	pTemp = g_Memory->Read<DWORD>(pTemp + Offsets::Current);
	auto pHp = g_Memory->Read<int>(pTemp + Offsets::_hp);
	return pHp;
}
auto PlayerEntity::GetMaxHp()->int {
	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + BasicInfo);
	pTemp = g_Memory->Read<DWORD>(pTemp + Offsets::Current);
	auto pmaxHp = g_Memory->Read<int>(pTemp + Offsets::_maxHp);
	return pmaxHp;
}
auto  PlayerEntity::GetTeam()->int {
	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + BasicInfo);
	pTemp = g_Memory->Read<DWORD>(pTemp + Offsets::Current);
	auto pteam = g_Memory->Read<int>(pTemp + Offsets::_team);
	return pteam;
}
auto  PlayerEntity::GetisDead()->bool {
	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + BasicInfo);
	pTemp = g_Memory->Read<DWORD>(pTemp + Offsets::Current);
	auto pisDead = g_Memory->Read<bool>(pTemp + Offsets::_isDead);
	return pisDead;
}
auto  PlayerEntity::GetisBot()->bool {
	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + BasicInfo);
	pTemp = g_Memory->Read<DWORD>(pTemp + Offsets::Current);
	auto pisBot = g_Memory->Read<bool>(pTemp + Offsets::_isBot);
	return pisBot;
}
auto  PlayerEntity::GethasC4()->bool {
	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + BasicInfo);
	pTemp = g_Memory->Read<DWORD>(pTemp + Offsets::Current);
	auto  phasC4 = g_Memory->Read<bool>(pTemp + Offsets::_hasC4);
	return phasC4;
}

#pragma optimize( "", off )
auto PlayerEntity::GetBoneByPlayerEntity(int index)->Math::Vector3 {
	Math::Vector3 Result = { 0.0f,0.0f,0.0f };

	GetBoneFn_PlayerEntity GetBone = (GetBoneFn_PlayerEntity)Offsets::Method::GetBone_PlayerEntity;
	GetPosFn GetPos = (GetPosFn)Offsets::Method::get_position_2;

	auto Transform = GetBone((DWORD)this, BoneKey[index]);
	if ((DWORD)Transform > 0xC60000) {
		GetPos((DWORD)Transform, &Result);
	}

	
	return Result;
}
auto PlayerEntity::GetBoneByAnimator(int index)->Math::Vector3 {
	Math::Vector3 Result = { 0.0f,0.0f,0.0f };


	GetBoneFn_Animator GetBone = (GetBoneFn_Animator)Offsets::Method::GetBone_SJAnimator;
	GetPosFn GetPos = (GetPosFn)Offsets::Method::get_position_2;
	auto Animator = this->GetAnimator();
	if (Animator > 0xC60000) {

		auto Transform = GetBone(Animator, BoneKey[index]);
		if ((DWORD)Transform > 0xC60000) {
			GetPos((DWORD)Transform, &Result);
		}
	}


	return Result;
}
#pragma optimize( "", on )
auto  PlayerEntity::SetNoRecoilPunch()->void {

	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + 0x130);
	g_Memory->Write(pTemp + 0x8, 0.0f);
	g_Memory->Write(pTemp + 0xC, 0.0f);

	DWORD OldProtect = 0;
	VirtualProtect((LPVOID)(pTemp + 0x8), 4, PAGE_READONLY, &OldProtect);
	VirtualProtect((LPVOID)(pTemp + 0xC), 4, PAGE_READONLY, &OldProtect);
}
auto  PlayerEntity::GetYaw() -> float{

	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + 0x130);
	return g_Memory->Read<float>(pTemp+0x8);

}
auto  PlayerEntity::GetPitch() -> float {
	auto pTemp = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Component);
	pTemp = g_Memory->Read<DWORD>(pTemp + 0x130);
	return g_Memory->Read<float>(pTemp + 0xC);
}

auto UserCommandContext::GetEntitiesNum()->int
{
	auto Entities = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Offsets::Entities);
	if (Entities)
	{
		return g_Memory->Read<int>(Entities + Offsets::Count);
	}
	return 0;
}

auto UserCommandContext::GetEntityByIndex(int index)->UserCommandEntity*
{
	auto Entities = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Offsets::Entities);
	if (Entities)
	{
		auto Slots = g_Memory->Read<DWORD>(Entities + Offsets::Slots);
		if (Slots)
		{
			//return Mem->Read<PlayerEntity*>(Slots + 0x20 + index * 8); 64
			return g_Memory->Read<UserCommandEntity*>(Slots + 0x10 + index * 4);
		}
	}
	return nullptr;
}


auto PlayerContext::GetEntitiesNum()->int
{
	auto Entities = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Offsets::Entities);
	if (Entities)
	{
		return g_Memory->Read<int>(Entities + Offsets::Count);
	}
	return 0;
}

auto PlayerContext::GetEntityByIndex(int index)->PlayerEntity*
{
	auto Entities = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Offsets::Entities);
	if (Entities)
	{
		auto Slots = g_Memory->Read<DWORD>(Entities + Offsets::Slots);
		if (Slots)
		{
			//return Mem->Read<PlayerEntity*>(Slots + 0x20 + index * 8); 64
			return g_Memory->Read<PlayerEntity*>(Slots + 0x10 + index * 4);
		}
	}
	return nullptr;
}
auto WorldCameraContext::GetEntitiesNum()->int {
	auto Entities = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Offsets::Entities);
	if (Entities)
	{
		return g_Memory->Read<int>(Entities + Offsets::Count);
	}
	return 0;
}
auto WorldCameraContext::GetEntityByIndex(int index)->WorldCameraEntity* {
	auto Entities = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Offsets::Entities);
	if (Entities)
	{
		auto Slots = g_Memory->Read<DWORD>(Entities + Offsets::Slots);
		if (Slots)
		{
			//return Mem->Read<PlayerEntity*>(Slots + 0x20 + index * 8); 64
			return g_Memory->Read<WorldCameraEntity*>(Slots + 0x10 + index * 4);
		}
	}
	return nullptr;
}



auto Contexts::GetPlayer()->PlayerContext*
{
	auto Player = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Offsets::Player);
	if (Player)
	{
		return (PlayerContext*)Player;
	}
	return nullptr;
}
auto Contexts::GetUserCommand()->UserCommandContext*
{
	auto UserCommand = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Offsets::UserCommand);
	if (UserCommand)
	{
		return (UserCommandContext*)UserCommand;
	}
	return nullptr;
}
auto Contexts::GetWorldCamera()->WorldCameraContext* {
	auto WorldCamera = g_Memory->Read<DWORD>(reinterpret_cast<DWORD>(this) + Offsets::WorldCamera);
	if (WorldCamera)
	{
		return (WorldCameraContext*)WorldCamera;
	}
	return nullptr;
}
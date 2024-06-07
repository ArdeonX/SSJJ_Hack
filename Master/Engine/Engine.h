#pragma once
#include "../GloBals.h"
#ifndef _Include_Engine_
#define _Include_Engine_



extern Memory* g_Memory;
enum BoneType {
	Head,
	Neck,
	Spine,
	Pelvis,
	L_UpperArm,
	L_Forearm,
	L_Hand,
	R_UpperArm,
	R_Forearm,
	R_Hand,
	L_Thigh,
	L_Calf,
	L_Foot,
	R_Thigh,
	R_Calf,
	R_Foot,
};
class Ustring {
public:
	char pad_C[0x8];
	int Length;
	wchar_t* Value;
};

class PlayerInfoData {

};
class SkillComponent {
public:
	auto GetInfo()->PlayerInfoData*;
};
class InputComponent {
public:
	auto GetYaw()->float;
	auto GetPitch()->float;
	auto GetFov()->float;
	auto SetYaw(float Yaw)->void;
	auto SetPitch(float Pitch)->void;
};
class Camera {
private:
	typedef void(_stdcall* FRotate)(void* Class, float xAngle, float yAngle, float zAngle);
public:
	auto Rotate(float xAngle, float yAngle, float zAngle)->void;
};
class UnityObjectsComponent {
public:
	auto GetCamera()->Camera*;
};
class UserCommandEntity {
private:
	typedef InputComponent* (_stdcall* get_input)(void* Class);
public:
	auto GetInput()->InputComponent*;
};
class PlayerEntity {
private:
#define Component 0x1C
#define BasicInfo 0x54
#define ThirdPersonUnityObjectsComponent 0x1B0
#define CurrentWeaponComponent 0x6C
	typedef void* (_stdcall* GetBoneFn_Animator)(DWORD Animator, DWORD BoneName);
	typedef void* (_stdcall* GetBoneFn_PlayerEntity)(DWORD PlayerEntity, DWORD BoneName);
	typedef void* (_stdcall* GetPosFn)(DWORD Transform, Math::Vector3* Result);
	typedef void(_stdcall* SetValue)(void* Class, bool Value);
	typedef void* (_stdcall* GetValue)(void* Class);
	std::string  UnicodeToUtf8(wchar_t* wstr);
public:
	auto SetThirdPerson()->void;
	auto SetFirstPerson()->void;
	auto GetAnimator()->DWORD;
	auto GetPlayerName()->std::string;
	auto GetWeaponName()->std::string;
	auto GetCurrentWeapon()->int;
	auto GetHp()->int;
	auto GetMaxHp()->int;
	auto GetTeam()->int;
	auto GetisDead()->bool;
	auto GetisBot()->bool;
	auto GethasC4()->bool;
	auto GetBoneByPlayerEntity(int index)->Math::Vector3;
	auto GetBoneByAnimator(int index)->Math::Vector3;
	auto SetNoRecoilPunch()->void;
	auto GetYaw() -> float;
	auto GetPitch() -> float;
};


class WorldCameraEntity {
private:
	
public:
	//auto GetUnityObjects()->UnityObjectsComponent*;
};
class UserCommandContext {
public:
	auto GetEntitiesNum()->int;

	auto GetEntityByIndex(int index)->UserCommandEntity*;
};
class PlayerContext
{
public:
	auto GetEntitiesNum()->int;

	auto GetEntityByIndex(int index)->PlayerEntity*;
};
class WorldCameraContext {
private:
	typedef UnityObjectsComponent* (_stdcall* get_unityObjects)(void* Class);
public:
	auto GetEntitiesNum()->int;

	auto GetEntityByIndex(int index)->WorldCameraEntity*;
	auto GetUnityObjects()->UnityObjectsComponent*;
};
class Contexts
{
public:
	auto GetPlayer()->PlayerContext*;
	auto GetUserCommand()->UserCommandContext*;
	auto GetWorldCamera()->WorldCameraContext*;
};

namespace Engine {
	auto GetContexts()->Contexts*;
	auto CheckOffsets()->bool;
	auto GetOffsets()->bool;
	auto InitBoneName()->void;
	auto Init()->bool;
	auto WorldtoScreen(Math::Vector3* Coordinate, Math::Vector4* ScreenBox, int* Dis)->bool;
	auto WorldtoScreen(Math::Vector3* Coordinate, Math::Vector2* ScreenCoordinate)->bool;
	auto Refresh()->void;
	auto GetCameraPos()->Math::Vector3;
	auto GetCenterDistance(float x, float y)->float;
	auto CalcAim(Math::Vector3* MyPos, Math::Vector3* AimPos, Math::Vector2* Mouse)->bool;
	auto GetScreenX()->float;
	auto GetScreenY()->float;
	auto SetRightBottonProcessUiTouch(bool DoLock)->bool;

	auto  Other_GetOwnEntity()->PlayerEntity*;
	auto   Other_GetCount()->DWORD;
	auto   Other_GetPlayerEntity(DWORD Count)->PlayerEntity*;

}
#endif // !_Inlcude_Engine_

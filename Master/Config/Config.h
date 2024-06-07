#ifndef  _Includes_Config_
#define _Includes_Config_
#include "../Overlay/ImGui/imgui.h"
using namespace std;
class Config {
private:
	std::string ConfigPath = (const char*)_xor_("C:\\ZaMs\\Config.ini");
	char Temp[20] = { 0 };
	char* floattochar(float a)
	{
		sprintf(this->Temp, "%f", a);
		return Temp;
	}
private:


	BOOL KeepImVec4(std::string AppName, std::string KeyName, ImVec4 Value)
	{
		std::string Keyw = KeyName + "w";
		std::string Keyx = KeyName + "x";
		std::string Keyy = KeyName + "y";
		std::string Keyz = KeyName + "z";

		WritePrivateProfileStringA(AppName.c_str(), Keyw.c_str(), to_string(Value.w).c_str(), ConfigPath.c_str());
		WritePrivateProfileStringA(AppName.c_str(), Keyx.c_str(), to_string(Value.x).c_str(), ConfigPath.c_str());
		WritePrivateProfileStringA(AppName.c_str(), Keyy.c_str(), to_string(Value.y).c_str(), ConfigPath.c_str());
		WritePrivateProfileStringA(AppName.c_str(), Keyz.c_str(), to_string(Value.z).c_str(), ConfigPath.c_str());
		return TRUE;
	}
	BOOL Keepbool(std::string AppName, std::string KeyName, bool Value)
	{
		WritePrivateProfileStringA(AppName.c_str(), KeyName.c_str(), Value == true ? "1" : "0", ConfigPath.c_str());
		return TRUE;

	}
	BOOL Keepfloat(std::string AppName, std::string KeyName, float Value)
	{
		WritePrivateProfileStringA(AppName.c_str(), KeyName.c_str(), std::to_string(Value).c_str(), ConfigPath.c_str());
		return TRUE;
	}
	BOOL Keepint(std::string AppName, std::string KeyName, int Value)
	{
		WritePrivateProfileStringA(AppName.c_str(), KeyName.c_str(), std::to_string(Value).c_str(), ConfigPath.c_str());
		return TRUE;
	}

	BOOL ReadImVec4(std::string AppName, std::string KeyName, ImVec4* Value)
	{
		std::string Keyw = KeyName + "w";
		std::string Keyx = KeyName + "x";
		std::string Keyy = KeyName + "y";
		std::string Keyz = KeyName + "z";
		GetPrivateProfileStringA(AppName.c_str(), Keyw.c_str(), "0", this->Temp, 20, ConfigPath.c_str());
		Value->w = (float)atof(this->Temp);
		ZeroMemory(&Temp, sizeof(Temp));

		GetPrivateProfileStringA(AppName.c_str(), Keyx.c_str(), "0", this->Temp, 20, ConfigPath.c_str());
		Value->x = (float)atof(this->Temp);
		ZeroMemory(&Temp, sizeof(Temp));

		GetPrivateProfileStringA(AppName.c_str(), Keyy.c_str(), "0", this->Temp, 20, ConfigPath.c_str());
		Value->y = (float)atof(this->Temp);
		ZeroMemory(&Temp, sizeof(Temp));

		GetPrivateProfileStringA(AppName.c_str(), Keyz.c_str(), "0", this->Temp, 20, ConfigPath.c_str());
		Value->z = (float)atof(this->Temp);
		ZeroMemory(&Temp, sizeof(Temp));
		return TRUE;
	}
	BOOL Readbool(std::string AppName, std::string KeyName, bool* Value)
	{
		GetPrivateProfileStringA(AppName.c_str(), KeyName.c_str(), "0", this->Temp, 2, ConfigPath.c_str());
		std::string b = this->Temp;
		b == "1" ? *Value = true : *Value = false;
		ZeroMemory(&Temp, sizeof(Temp));
		return TRUE;

	}
	BOOL Readfloat(std::string AppName, std::string KeyName, float* Value)
	{
		GetPrivateProfileStringA(AppName.c_str(), KeyName.c_str(), "0", this->Temp, 20, ConfigPath.c_str());
		*Value = (float)atof(this->Temp);
		ZeroMemory(&Temp, sizeof(Temp));
		return TRUE;
	}
	BOOL Readint(std::string AppName, std::string KeyName, int* Value)
	{
		GetPrivateProfileStringA(AppName.c_str(), KeyName.c_str(), "0", this->Temp, 20, ConfigPath.c_str());
		*Value = (int)atoi(this->Temp);
		ZeroMemory(&Temp, sizeof(Temp));
		return TRUE;
	}
public:
	const char* ComboHotKey[6] = { (char*)u8"->[×ó¼ü]\n",(char*)u8"->[ÓÒ¼ü]\n","->[Shift]\n","->[Ctrl]\n","->[F]\n","->[Alt]\n" };
	const char* ComboAimWhere[16] = { (char*)u8"->[Í·²¿]\n",(char*)u8"->[¾±²¿]\n",(char*)u8"->[¼¹Öù]\n",(char*)u8"->[Åè¹Ç]\n",(char*)u8"->[×óÉÏ±Û]\n",(char*)u8"->[×óÇ°±Û]\n",(char*)u8"->[×óÊÖ]\n",(char*)u8"->[ÓÒÉÏ±Û]\n",(char*)u8"->[ÓÒÇ°±Û]\n",(char*)u8"->[ÓÒÊÖ]\n" ,(char*)u8"->[×ó´óÍÈ]\n",(char*)u8"->[×óÐ¡ÍÈ]\n",(char*)u8"->[×ó½Å]\n" ,(char*)u8"->[ÓÒ´óÍÈ]\n",(char*)u8"->[ÓÒÐ¡ÍÈ]\n",(char*)u8"->[ÓÒ½Å]\n" };
	void* PreviewAimEntity = nullptr;
	void* OwnPlayer = nullptr;
	bool UseNotSuitWeapon = false;
	bool HaveThirdMode = false;
	bool GameInitful = false;
	bool CanDraw = true;
	bool ShowMenu = true;
	bool ShowFunction = true;
	bool ShowBox = true;
	bool ShowLine = true;
	bool ShowMessageBox = true;
	bool ShowHP = true;
	bool ShowBone = true;

	bool DoAim = true;
	bool ShowAimRange = true;
	bool NoGunNoAim = true;
	bool NoRecoil = true;
	bool LockRightInput = true;
	bool ThirdMode = true;
	bool SmoothAim = true;

	float AimRangeValue = 150.0f;
	int SmoothValue = 50;

	int MenuChoiceHotKey = 0;
	int AimHotkey=0;
	int AimWhere = 0;
	bool isAimNow = false;
	ImVec4 BoxColor;
	ImVec4 LineColor;
	ImVec4 MessageBackGroundColor;
	ImVec4 MessageTextColor;
	ImVec4 MessageTriangleColor;
	ImVec4 BoneColor;
	ImVec4 AimRangeColor;
public:
	void ReadConfig() {
		this->ReadImVec4((const char*)_xor_("Color"), (const char*)_xor_("BoxColor"), &this->BoxColor);
		this->ReadImVec4((const char*)_xor_("Color"), (const char*)_xor_("LineColor"), &this->LineColor);
		this->ReadImVec4((const char*)_xor_("Color"), (const char*)_xor_("BoneColor"), &this->BoneColor);
		this->ReadImVec4((const char*)_xor_("Color"), (const char*)_xor_("AimRangeColor"), &this->AimRangeColor);
		this->ReadImVec4((const char*)_xor_("Color"), (const char*)_xor_("MessageBackGroundColor"), &this->MessageBackGroundColor);
		this->ReadImVec4((const char*)_xor_("Color"), (const char*)_xor_("MessageTextColor"), &this->MessageTextColor);
		this->ReadImVec4((const char*)_xor_("Color"), (const char*)_xor_("MessageTriangleColor"), &this->MessageTriangleColor);

		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("ShowBox"), &this->ShowBox);
		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("ShowLine"), &this->ShowLine);
		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("ShowHP"), &this->ShowHP);
		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("ShowBone"), &this->ShowBone);
		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("ShowMessageBox"), &this->ShowMessageBox);
		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("ShowAimRange"), &this->ShowAimRange);

		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("NoRecoil"), &this->NoRecoil);
		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("LockRightInput"), &this->LockRightInput);
		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("DoAim"), &this->DoAim);
		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("NoGunNoAim"), &this->NoGunNoAim);
		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("ThirdMode"), &this->ThirdMode);
		this->Readbool((const char*)_xor_("Function"), (const char*)_xor_("SmoothAim"), &this->SmoothAim);

		this->Readfloat((const char*)_xor_("Function"), (const char*)_xor_("AimRangeValue"), &this->AimRangeValue);
		this->Readint((const char*)_xor_("Function"), (const char*)_xor_("AimHotkey"), &this->AimHotkey);
		this->Readint((const char*)_xor_("Function"), (const char*)_xor_("MenuChoiceHotKey"), &this->MenuChoiceHotKey);
		this->Readint((const char*)_xor_("Function"), (const char*)_xor_("AimWhere"), &this->AimWhere);
		this->Readint((const char*)_xor_("Function"), (const char*)_xor_("SmoothValue"), &this->SmoothValue);
	}
	void KeepConfig() {
		this->KeepImVec4((const char*)_xor_("Color"), (const char*)_xor_("BoxColor"), this->BoxColor);
		this->KeepImVec4((const char*)_xor_("Color"), (const char*)_xor_("LineColor"), this->LineColor);
		this->KeepImVec4((const char*)_xor_("Color"), (const char*)_xor_("BoneColor"), this->BoneColor);
		this->KeepImVec4((const char*)_xor_("Color"), (const char*)_xor_("AimRangeColor"), this->AimRangeColor);
		this->KeepImVec4((const char*)_xor_("Color"), (const char*)_xor_("MessageBackGroundColor"), this->MessageBackGroundColor);
		this->KeepImVec4((const char*)_xor_("Color"), (const char*)_xor_("MessageTextColor"), this->MessageTextColor);
		this->KeepImVec4((const char*)_xor_("Color"), (const char*)_xor_("MessageTriangleColor"), this->MessageTriangleColor);

		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("ShowBox"), this->ShowBox);
		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("ShowLine"), this->ShowLine);
		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("ShowHP"), this->ShowHP);
		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("ShowBone"), this->ShowBone);
		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("ShowMessageBox"), this->ShowMessageBox);
		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("ShowAimRange"), this->ShowAimRange);

		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("NoRecoil"), this->NoRecoil);
		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("LockRightInput"), this->LockRightInput);
		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("DoAim"), this->DoAim);
		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("NoGunNoAim"), this->NoGunNoAim);
		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("ThirdMode"), this->ThirdMode);
		this->Keepbool((const char*)_xor_("Function"), (const char*)_xor_("SmoothAim"), this->SmoothAim);

		this->Keepfloat((const char*)_xor_("Function"), (const char*)_xor_("AimRangeValue"), this->AimRangeValue);
		this->Keepint((const char*)_xor_("Function"), (const char*)_xor_("AimHotkey"), this->AimHotkey);
		this->Keepint((const char*)_xor_("Function"), (const char*)_xor_("MenuChoiceHotKey"), this->MenuChoiceHotKey);
		this->Keepint((const char*)_xor_("Function"), (const char*)_xor_("AimWhere"), this->AimWhere);
		this->Keepint((const char*)_xor_("Function"), (const char*)_xor_("SmoothValue"), this->SmoothValue);
	}
};
#endif // ! _Includes_Config_


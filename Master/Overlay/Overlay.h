#pragma once
#ifndef _Include_Overlay_
#define _Include_Overlay_
#include "../GloBals.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

bool CreatDirectxHook(std::function<void(void)> CallBackFn);
class HealthBar
{
private:
	using TimePoint_ = std::chrono::steady_clock::time_point;
private:
	// ��ʾ����Ѫ��ʱ��(ms)
	const int ShowBackUpHealthDuration = 500;
	// ���Ѫ��
	float MaxHealth = 0.f;
	// ��ǰѪ��
	float CurrentHealth = 0.f;
	// �������Ѫ����С
	float LastestBackupHealth = 0.f;
	// Ѫ������
	ImVec2 RectPos{};
	// Ѫ����С
	ImVec2 RectSize{};
	// ������ʾ����Ѫ��
	bool InShowBackupHealth = false;
	// ��ʾ����Ѫ����ʼʱ���
	TimePoint_ BackupHealthTimePoint{};
public:
	HealthBar() {}
	// ����
	void DrawHealthBar_Horizontal(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size);
	// ����
	void DrawHealthBar_Vertical(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size);
private:
	// ��ɫ����
	ImColor Mix(ImColor Col_1, ImColor Col_2, float t);
	// ��һ�׶�Ѫ����ɫ 0.5-1
	ImColor FirstStageColor = ImColor(96, 246, 113, 220);
	// �ڶ��׶�Ѫ����ɫ 0.5-0.2
	ImColor SecondStageColor = ImColor(247, 214, 103, 220);
	// �����׶�Ѫ����ɫ 0.2-0.0
	ImColor ThirdStageColor = ImColor(255, 95, 95, 220);
	// ����Ѫ����ɫ
	ImColor BackupHealthColor = ImColor(255, 255, 255, 220);
	// �߿���ɫ
	ImColor FrameColor = ImColor(45, 45, 45, 220);
	// ������ɫ
	ImColor BackGroundColor = ImColor(90, 90, 90, 220);
};
namespace Overlay {

	void AddLine(float Fx, float Fy, float Ex, float Ey, float Width, ImU32 Color);
	void AddRectFilled(float Lx, float Ly, float Weight, float Height, ImU32 Color);
	void AddText(float Lx, float Ly, char* Text, float Fontsize, ImU32 Color);
	void AddRect(float Lx, float Ly, float Weight, float Height, float Rounding, float LineWidth, ImU32 Color);
	void AddCircle(float Lx, float Ly, float Width, float Num, float Radius, ImU32 Color);
	void AddCirleFilled(float Lx, float Ly, float Num, float Radius, ImU32 Color);
	void AddStrokeTeLxt(const char* Text, float Lx, float Ly, ImU32 Color, ImU32 Color_T);
	void AddHelpMarker(const char* desc);
	void DrawHealthBar(DWORD Sign, float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size, bool Horizontal);
	ImU32 toU32(ImVec4 Color);
};




#endif
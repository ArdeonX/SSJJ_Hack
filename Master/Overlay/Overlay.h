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
	// 显示备份血条时间(ms)
	const int ShowBackUpHealthDuration = 500;
	// 最大血量
	float MaxHealth = 0.f;
	// 当前血量
	float CurrentHealth = 0.f;
	// 最近备份血量大小
	float LastestBackupHealth = 0.f;
	// 血条坐标
	ImVec2 RectPos{};
	// 血条大小
	ImVec2 RectSize{};
	// 正在显示备份血量
	bool InShowBackupHealth = false;
	// 显示备份血量起始时间戳
	TimePoint_ BackupHealthTimePoint{};
public:
	HealthBar() {}
	// 横向
	void DrawHealthBar_Horizontal(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size);
	// 纵向
	void DrawHealthBar_Vertical(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size);
private:
	// 颜色缓动
	ImColor Mix(ImColor Col_1, ImColor Col_2, float t);
	// 第一阶段血条颜色 0.5-1
	ImColor FirstStageColor = ImColor(96, 246, 113, 220);
	// 第二阶段血条颜色 0.5-0.2
	ImColor SecondStageColor = ImColor(247, 214, 103, 220);
	// 第三阶段血条颜色 0.2-0.0
	ImColor ThirdStageColor = ImColor(255, 95, 95, 220);
	// 备份血条颜色
	ImColor BackupHealthColor = ImColor(255, 255, 255, 220);
	// 边框颜色
	ImColor FrameColor = ImColor(45, 45, 45, 220);
	// 背景颜色
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
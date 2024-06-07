#include "Overlay.h"
void Overlay::AddLine(float Fx, float Fy, float Ex, float Ey, float Width, ImU32 Color)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(Fx, Fy), ImVec2(Ex, Ey), Color, Width);
}
void Overlay::AddRectFilled(float Lx, float Ly, float Weight, float Height, ImU32 Color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(Lx, Ly), ImVec2(Lx + Weight, Ly + Height), Color, 0, 15);
}
void Overlay::AddText(float Lx, float Ly, char* Text, float Fontsize, ImU32 Color)
{
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), Fontsize, ImVec2(Lx, Ly), Color, Text);
}
void Overlay::AddRect(float Lx, float Ly, float Weight, float Height, float Rounding, float LineWidth, ImU32 Color)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(Lx, Ly), ImVec2(Lx + Weight, Ly + Height), Color, Rounding, 15, LineWidth);
}
void Overlay::AddCircle(float Lx, float Ly, float Width, float Num, float Radius, ImU32 Color)
{
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(Lx, Ly), Radius, Color, Num, Width);
}
void Overlay::AddCirleFilled(float Lx, float Ly, float Num, float Radius, ImU32 Color)
{
	ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(Lx, Ly), Radius, Color, Num);
}
void Overlay::AddStrokeTeLxt(const char* Text, float Lx, float Ly, ImU32 Color, ImU32 Color_T)
{
	ImGui::GetOverlayDrawList()->AddText(ImVec2(Lx - 1, Ly + 1), Color_T, Text);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(Lx - 1, Ly - 1), Color_T, Text);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(Lx + 1, Ly + 1), Color_T, Text);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(Lx + 1, Ly - 1), Color_T, Text);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(Lx, Ly), Color, Text);
}
void Overlay::AddHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}


void HealthBar::DrawHealthBar_Horizontal(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size)
{
	auto InRange = [&](float value, float min, float max) -> bool
	{
		return value > min && value <= max;
	};

	ImDrawList* DrawList = ImGui::GetForegroundDrawList();

	this->MaxHealth = MaxHealth;
	this->CurrentHealth = CurrentHealth;
	this->RectPos = Pos;
	this->RectSize = Size;

	// ռ��
	float Proportion = CurrentHealth / MaxHealth;
	// Ѫ�������
	float Width = RectSize.x * Proportion;
	// Ѫ������ɫ
	ImColor Color;

	// ����
	DrawList->AddRectFilled(RectPos,
		{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
		BackGroundColor, 5, 15);

	// ��ɫ�л�
	float Color_Lerp_t = pow(Proportion, 2.5);
	if (InRange(Proportion, 0.5, 1))
		Color = Mix(FirstStageColor, SecondStageColor, Color_Lerp_t * 3 - 1);
	else
		Color = Mix(SecondStageColor, ThirdStageColor, Color_Lerp_t * 4);

	// �����������Ѫ��
	if (LastestBackupHealth == 0
		|| LastestBackupHealth < CurrentHealth)
		LastestBackupHealth = CurrentHealth;

	if (LastestBackupHealth != CurrentHealth)
	{
		if (!InShowBackupHealth)
		{
			BackupHealthTimePoint = std::chrono::steady_clock::now();
			InShowBackupHealth = true;
		}

		std::chrono::steady_clock::time_point CurrentTime = std::chrono::steady_clock::now();
		if (CurrentTime - BackupHealthTimePoint > std::chrono::milliseconds(ShowBackUpHealthDuration))
		{
			// ��ʱ��ֹͣ��ʾ����Ѫ�������Ҹ����������Ѫ��
			LastestBackupHealth = CurrentHealth;
			InShowBackupHealth = false;
		}

		if (InShowBackupHealth)
		{
			// ����Ѫ�����ƿ��
			float BackupHealthWidth = LastestBackupHealth / MaxHealth * RectSize.x;
			// ����Ѫ��alpha����
			float BackupHealthColorAlpha = 1 - 0.95 * (std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - BackupHealthTimePoint).count() / (float)ShowBackUpHealthDuration);
			ImColor BackupHealthColorTemp = BackupHealthColor;
			BackupHealthColorTemp.Value.w = BackupHealthColorAlpha;
			// ����Ѫ����Ȼ���
			float BackupHealthWidth_Lerp = 1 * (std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - BackupHealthTimePoint).count() / (float)ShowBackUpHealthDuration);
			BackupHealthWidth_Lerp *= (BackupHealthWidth - Width);
			BackupHealthWidth -= BackupHealthWidth_Lerp;
			// ����Ѫ��
			DrawList->AddRectFilled(RectPos,
				{ RectPos.x + BackupHealthWidth,RectPos.y + RectSize.y },
				BackupHealthColorTemp, 5);
		}
	}

	// Ѫ��
	DrawList->AddRectFilled(RectPos,
		{ RectPos.x + Width,RectPos.y + RectSize.y },
		Color, 5);

	// �߿�
	DrawList->AddRect(RectPos,
		{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
		FrameColor, 5, 15, 2);
}

void HealthBar::DrawHealthBar_Vertical(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size)
{
	auto InRange = [&](float value, float min, float max) -> bool
	{
		return value > min && value <= max;
	};

	ImDrawList* DrawList = ImGui::GetForegroundDrawList();

	this->MaxHealth = MaxHealth;
	this->CurrentHealth = CurrentHealth;
	this->RectPos = Pos;
	this->RectSize = Size;

	// ռ��
	float Proportion = CurrentHealth / MaxHealth;
	// Ѫ�����߶�
	float Height = RectSize.y * Proportion;
	// Ѫ������ɫ
	ImColor Color;

	// ����
	DrawList->AddRectFilled(RectPos,
		{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
		BackGroundColor, 5, 15);

	// ��ɫ�л�
	float Color_Lerp_t = pow(Proportion, 2.5);
	if (InRange(Proportion, 0.5, 1))
		Color = Mix(FirstStageColor, SecondStageColor, Color_Lerp_t * 3 - 1);
	else
		Color = Mix(SecondStageColor, ThirdStageColor, Color_Lerp_t * 4);

	// �����������Ѫ��
	if (LastestBackupHealth == 0
		|| LastestBackupHealth < CurrentHealth)
		LastestBackupHealth = CurrentHealth;

	if (LastestBackupHealth != CurrentHealth)
	{
		if (!InShowBackupHealth)
		{
			BackupHealthTimePoint = std::chrono::steady_clock::now();
			InShowBackupHealth = true;
		}

		std::chrono::steady_clock::time_point CurrentTime = std::chrono::steady_clock::now();
		if (CurrentTime - BackupHealthTimePoint > std::chrono::milliseconds(ShowBackUpHealthDuration))
		{
			// ��ʱ��ֹͣ��ʾ����Ѫ�������Ҹ����������Ѫ��
			LastestBackupHealth = CurrentHealth;
			InShowBackupHealth = false;
		}

		if (InShowBackupHealth)
		{
			// ����Ѫ�����Ƹ߶�
			float BackupHealthHeight = LastestBackupHealth / MaxHealth * RectSize.y;
			// ����Ѫ��alpha����
			float BackupHealthColorAlpha = 1 - 0.95 * (std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - BackupHealthTimePoint).count() / (float)ShowBackUpHealthDuration);
			ImColor BackupHealthColorTemp = BackupHealthColor;
			BackupHealthColorTemp.Value.w = BackupHealthColorAlpha;
			// ����Ѫ���߶Ȼ���
			float BackupHealthHeight_Lerp = 1 * (std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - BackupHealthTimePoint).count() / (float)ShowBackUpHealthDuration);
			BackupHealthHeight_Lerp *= (BackupHealthHeight - Height);
			BackupHealthHeight -= BackupHealthHeight_Lerp;
			// ����Ѫ��
			DrawList->AddRectFilled({ RectPos.x,RectPos.y + RectSize.y - BackupHealthHeight },
				{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
				BackupHealthColorTemp, 5);
		}
	}

	// Ѫ��
	DrawList->AddRectFilled({ RectPos.x,RectPos.y + RectSize.y - Height },
		{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
		Color, 5);

	// �߿�
	DrawList->AddRect(RectPos,
		{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
		FrameColor, 5, 15, 2);
}

ImColor HealthBar::Mix(ImColor Col_1, ImColor Col_2, float t)
{
	ImColor Col;
	Col.Value.x = t * Col_1.Value.x + (1 - t) * Col_2.Value.x;
	Col.Value.y = t * Col_1.Value.y + (1 - t) * Col_2.Value.y;
	Col.Value.z = t * Col_1.Value.z + (1 - t) * Col_2.Value.z;
	Col.Value.w = Col_1.Value.w;
	return Col;
}

// Sign�����κ����͵��˱�ʶ��Ĭ�Ͽɴ����˵�ַ
void  Overlay::DrawHealthBar(DWORD Sign, float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size, bool Horizontal)
{
	static std::map<DWORD, HealthBar> HealthBarMap;
	if (!HealthBarMap.count(Sign))
	{
		HealthBarMap.insert({ Sign,HealthBar() });
	}
	if (HealthBarMap.count(Sign))
	{
		if (Horizontal)
			HealthBarMap[Sign].DrawHealthBar_Horizontal(MaxHealth, CurrentHealth, Pos, Size);
		else
			HealthBarMap[Sign].DrawHealthBar_Vertical(MaxHealth, CurrentHealth, Pos, Size);
	}
}

ImU32 Overlay::toU32(ImVec4 Color)
{
	return ImGui::GetColorU32({ Color.x,Color.y,Color.z,1.0f });
}


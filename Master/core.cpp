#include "GloBals.h"
bool DoCheckLegal();
Memory* g_Memory = new Memory;
Config* g_Config = new Config;
Contexts* pContexts = nullptr;
InputComponent* Input = nullptr;
namespace MyColor
{
	ImU32 ���� = IM_COL32(0, 255, 255, 255);
	ImU32 ��ɫ = IM_COL32(255, 0, 0, 255);
	ImU32 ��ɫ = IM_COL32(255, 255, 0, 255);
	ImU32 ��ɫ = IM_COL32(0, 0, 255, 255);
	ImU32 ��ɫ = IM_COL32(255, 165, 0, 255);
	ImU32 �ۺ� = IM_COL32(255, 192, 203, 255);
	ImU32 ��ɫ = IM_COL32(255, 255, 255, 255);
	ImU32 ��ɫ = IM_COL32(0, 255, 0, 255);
	ImU32 ��ɫ = IM_COL32(128, 0, 128, 255);
	ImU32 ���� = IM_COL32(135, 206, 235, 255);
	ImU32 ��ɫ = IM_COL32(119, 136, 153, 255);
	ImU32 ��ɫ = IM_COL32(0, 0, 0, 255);
	ImU32 ��� = IM_COL32(255, 0, 255, 255);

	ImU32 ������ = IM_COL32(0, 191, 255, 255);
	ImU32 �̻�ɫ = IM_COL32(173, 255, 47, 255);
	ImU32 ����ɫ = IM_COL32(0, 245, 255, 255);
	ImU32 ������ = IM_COL32(84, 255, 159, 255);
	ImU32 �Ⱥ�ɫ = IM_COL32(255, 69, 0, 255);
	ImU32 ����ɫ = IM_COL32(160, 82, 45, 255);
	ImU32 ������ = IM_COL32(77, 204, 229, 255);
	ImU32 ������ = IM_COL32(109, 229, 192, 255);
	ImU32 ʳ��� = IM_COL32(244, 241, 115, 255);
	ImU32 ������ = IM_COL32(231, 112, 75, 255);
}
void DoAim() {
	g_Config->isAimNow = true;


	while (GetAsyncKeyState(g_Config->AimHotkey) 
		& 0x8000 && g_Config->PreviewAimEntity != nullptr
		&& ((PlayerEntity*)g_Config->PreviewAimEntity)->GetisDead() == false 
		&& ((PlayerEntity*)g_Config->PreviewAimEntity)->GetAnimator()>0xC60000)
	{


		if (g_Config->NoGunNoAim) {
			if (g_Config->UseNotSuitWeapon)
				break;
		}

		auto CameraPos = Engine::GetCameraPos();
		auto AimPos = ((PlayerEntity*)g_Config->PreviewAimEntity)->GetBoneByAnimator(g_Config->AimWhere);

		if (AimPos.x == 0.0f && AimPos.y == 0.0f && AimPos.z == 0.0f)
			continue;
		if (g_Config->AimWhere == Head)
			AimPos.y = AimPos.y + 10.0f;

		Math::Vector2 AimMouse{};


		if (Engine::CalcAim(&CameraPos, &AimPos, &AimMouse)) {
			if (AimMouse.x != 0.0f && AimMouse.y != 0.0f) {
				if (g_Config->NoRecoil) {
					float Yaw = ((PlayerEntity*)g_Config->OwnPlayer)->GetYaw();
					float Pitch = ((PlayerEntity*)g_Config->OwnPlayer)->GetPitch();

					AimMouse.x -= Yaw * 2.0f;
					AimMouse.y -= Pitch * 2.0f;

					Input->SetYaw(AimMouse.x);
					Input->SetPitch(AimMouse.y);
				}
				else {
						Input->SetYaw(AimMouse.x);
						Input->SetPitch(AimMouse.y);
				}

			}
		

		}
	};
	g_Config->isAimNow = false;
	g_Config->PreviewAimEntity = nullptr;


	return;
}

void AimFn() {

	while (!g_Config->GameInitful) {
		this_thread::sleep_for(std::chrono::microseconds(500));
	}

	while (true) {
		
		if (g_Config->DoAim) {
			float MostDis = 10000.0f;
			if (Engine::Other_GetCount()> 0)
			{
				for (DWORD i = 0; i < Engine::Other_GetCount(); i++) {
					auto Player = Engine::Other_GetPlayerEntity(i);
					if (Player->GetAnimator()<0xC60000|| Player->GetTeam() == ((PlayerEntity*)g_Config->OwnPlayer)->GetTeam() || Player->GetisDead() == true) {
						continue;
					}
				
					Math::Vector2 ScreenPos = {};
					auto Pos = Player->GetBoneByAnimator(g_Config->AimWhere);//ȡԤ�鲿λ
					if (Engine::WorldtoScreen(&Pos, &ScreenPos)) {
						auto Dis = Engine::GetCenterDistance(ScreenPos.x, ScreenPos.y);
						if (Dis < MostDis && Dis < g_Config->AimRangeValue && g_Config->isAimNow == false) {
							g_Config->PreviewAimEntity = Player;
							MostDis = Dis;
						}


					}
				}
				if (g_Config->PreviewAimEntity != nullptr && g_Config->isAimNow == false) {
					DoAim();
				}
			}


		}

	}
}
void EspFn() {

	if (Engine::Other_GetCount() > 0) {
		auto OwnPlayer =Engine::Other_GetOwnEntity();//0���Լ�
	
		if (OwnPlayer->GetAnimator() >0xC60000) {
	
			g_Config->OwnPlayer = OwnPlayer;
			//------------------------------
			if (g_Config->LockRightInput) {
				if (OwnPlayer->GetCurrentWeapon() > 2) {
					Engine::SetRightBottonProcessUiTouch(false);
				}
				else {
					Engine::SetRightBottonProcessUiTouch(true);
				}
			}
			else {
				Engine::SetRightBottonProcessUiTouch(false);
			}
			//------------------------------
			if (g_Config->ThirdMode) {
				OwnPlayer->SetThirdPerson();
			}
			else {
				OwnPlayer->SetFirstPerson();
			}
			//------------------------------
			if (g_Config->NoGunNoAim) {
				if (OwnPlayer->GetCurrentWeapon() > 2) {
					g_Config->UseNotSuitWeapon = true;
				}
				else {
					g_Config->UseNotSuitWeapon = false;
				}
			}
			//------------------------------
			for (DWORD i = 0; i < Engine::Other_GetCount(); i++) {
				auto Player = Engine::Other_GetPlayerEntity(i);
				if (Player->GetAnimator() < 0xC60000 || Player->GetTeam() == OwnPlayer->GetTeam() || Player->GetisDead() == true) {
					continue;
				}
			

				auto Pos = Player->GetBoneByAnimator(Head);

				Math::Vector4 BoxInfo = {};
				int Distance = 0;
				if (Engine::WorldtoScreen(&Pos, &BoxInfo, &Distance)) {
					float Ratio = (float)Player->GetHp() / (float)Player->GetMaxHp();
					if (g_Config->ShowBox) {
						Overlay::AddRect(BoxInfo.x, BoxInfo.y, BoxInfo.w, BoxInfo.h, 0.0f, 1.5f, Overlay::toU32(g_Config->BoxColor));
					}
					if (g_Config->ShowLine) {
						Overlay::AddLine(Engine::GetScreenX(), 0.0f, BoxInfo.x + BoxInfo.w * 0.5f, BoxInfo.y, 1.0f, Overlay::toU32(g_Config->LineColor));
					}
					if (g_Config->ShowHP) {
						Overlay::DrawHealthBar((DWORD)Player, Player->GetMaxHp(), Player->GetHp(),
							ImVec2(BoxInfo.x - 6, BoxInfo.y), ImVec2(6, BoxInfo.h), false);
					/*	if (Ratio >= 0.8f)
						{
							Overlay::AddRectFilled(BoxInfo.x - 6, BoxInfo.y, 6, BoxInfo.h * Ratio, MyColor::��ɫ);
							Overlay::AddRect(BoxInfo.x - 6, BoxInfo.y, 6, BoxInfo.h, 0.0f, 1.0f, MyColor::��ɫ);

						}
						else if (Ratio >= 0.5f)
						{
							Overlay::AddRectFilled(BoxInfo.x - 6, BoxInfo.y, 6, BoxInfo.h * Ratio, MyColor::��ɫ);
							Overlay::AddRect(BoxInfo.x - 6, BoxInfo.y, 6, BoxInfo.h, 0.0f, 1.0f, MyColor::��ɫ);

						}
						else if (Ratio >= 0.2f)
						{
							Overlay::AddRectFilled(BoxInfo.x - 6, BoxInfo.y, 6, BoxInfo.h * Ratio, MyColor::��ɫ);
							Overlay::AddRect(BoxInfo.x - 6, BoxInfo.y, 6, BoxInfo.h, 0.0f, 1.0f, MyColor::��ɫ);

						}
						else
						{
							Overlay::AddRectFilled(BoxInfo.x - 6, BoxInfo.y, 6, BoxInfo.h * Ratio, MyColor::��ɫ);
							Overlay::AddRect(BoxInfo.x - 6, BoxInfo.y, 6, BoxInfo.h, 0.0f, 1.0f, MyColor::��ɫ);
						}*/
					}
					if (g_Config->ShowMessageBox) {
						Math::Vector4 ThisMessageBox{};
						std::string PlayerName = Player->GetPlayerName();
						std::string WeaponName = Player->GetWeaponName();

						ImVec2 TextName = ImGui::CalcTextSize(PlayerName.c_str());
						ImVec2 TextWeaponName = ImGui::CalcTextSize(WeaponName.c_str());
						ThisMessageBox.h = TextName.y + TextWeaponName.y + 6;
						if (TextName.x >= TextWeaponName.x)
							ThisMessageBox.w = TextName.x + 4;
						else
							ThisMessageBox.w = TextWeaponName.x + 4;
						ThisMessageBox.x = BoxInfo.x - (ThisMessageBox.w - BoxInfo.w) / 2;
						ThisMessageBox.y = BoxInfo.y - 50.0f;

						Overlay::AddRectFilled(ThisMessageBox.x, ThisMessageBox.y, ThisMessageBox.w, ThisMessageBox.h, Overlay::toU32(g_Config->MessageBackGroundColor));

						Overlay::AddText(ThisMessageBox.x + 2, ThisMessageBox.y + 2, const_cast<char*>(PlayerName.c_str()), 15.0f, Overlay::toU32(g_Config->MessageTextColor));
						Overlay::AddText(ThisMessageBox.x + 2, ThisMessageBox.y + TextName.y + 4, const_cast<char*>(WeaponName.c_str()), 15.0f, Overlay::toU32(g_Config->MessageTextColor));
						if (Ratio >= 0.8f)
						{
							Overlay::AddRectFilled(ThisMessageBox.x, ThisMessageBox.y + ThisMessageBox.h, ThisMessageBox.w * Ratio, 3.0f, MyColor::��ɫ);
							Overlay::AddRect(ThisMessageBox.x, ThisMessageBox.y + ThisMessageBox.h, ThisMessageBox.w, 3.0f, 0.0f, 1.0f, MyColor::��ɫ);
						}
						else if (Ratio >= 0.5f)
						{
							Overlay::AddRectFilled(ThisMessageBox.x, ThisMessageBox.y + ThisMessageBox.h, ThisMessageBox.w * Ratio, 3.0f, MyColor::��ɫ);
							Overlay::AddRect(ThisMessageBox.x, ThisMessageBox.y + ThisMessageBox.h, ThisMessageBox.w, 3.0f, 0.0f, 1.0f, MyColor::��ɫ);

						}
						else if (Ratio >= 0.2f)
						{
							Overlay::AddRectFilled(ThisMessageBox.x, ThisMessageBox.y + ThisMessageBox.h, ThisMessageBox.w * Ratio, 3.0f, MyColor::��ɫ);
							Overlay::AddRect(ThisMessageBox.x, ThisMessageBox.y + ThisMessageBox.h, ThisMessageBox.w, 3.0f, 0.0f, 1.0f, MyColor::��ɫ);

						}
						else
						{
							Overlay::AddRectFilled(ThisMessageBox.x, ThisMessageBox.y + ThisMessageBox.h, ThisMessageBox.w * Ratio, 3.0f, MyColor::��ɫ);
							Overlay::AddRect(ThisMessageBox.x, ThisMessageBox.y + ThisMessageBox.h, ThisMessageBox.w, 3.0f, 0.0f, 1.0f, MyColor::��ɫ);
						}
						ImVec2  TriangleLeft, TriangleRight, TriangleBottom{};
						TriangleLeft.x = ThisMessageBox.x + ThisMessageBox.w * 0.4f;
						TriangleRight.x = ThisMessageBox.x + ThisMessageBox.w * 0.6f;
						TriangleBottom.x = ThisMessageBox.x + ThisMessageBox.w * 0.5f;
						TriangleLeft.y = ThisMessageBox.y + ThisMessageBox.h + 6.0f;
						TriangleRight.y = TriangleLeft.y;
						TriangleBottom.y = TriangleLeft.y + 4.0f;
						ImGui::GetOverlayDrawList()->AddTriangleFilled(TriangleLeft, TriangleRight, TriangleBottom, Overlay::toU32(g_Config->MessageTriangleColor));
					}

					if (g_Config->ShowBone) {
						Math::Vector2 Bone[16]{};
						for (int Times = 0; Times < 16; Times++)
						{
							auto BonePos = Player->GetBoneByAnimator(Times);
							if (Times == Head)
								BonePos.y = BonePos.y + 10.0f;
							Engine::WorldtoScreen(&BonePos, &Bone[Times]);
						}


						Overlay::AddCircle(Bone[Head].x, Bone[Head].y, 1.0f, 50, BoxInfo.w * 0.15f, Overlay::toU32(g_Config->BoneColor));
						//���ֱ�
						Overlay::AddLine(Bone[Neck].x, Bone[Neck].y, Bone[L_UpperArm].x, Bone[L_UpperArm].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						Overlay::AddLine(Bone[L_UpperArm].x, Bone[L_UpperArm].y, Bone[L_Forearm].x, Bone[L_Forearm].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						Overlay::AddLine(Bone[L_Forearm].x, Bone[L_Forearm].y, Bone[L_Hand].x, Bone[L_Hand].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						//���ֱ�
						Overlay::AddLine(Bone[Neck].x, Bone[Neck].y, Bone[R_UpperArm].x, Bone[R_UpperArm].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						Overlay::AddLine(Bone[R_UpperArm].x, Bone[R_UpperArm].y, Bone[R_Forearm].x, Bone[R_Forearm].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						Overlay::AddLine(Bone[R_Forearm].x, Bone[R_Forearm].y, Bone[R_Hand].x, Bone[R_Hand].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						//���Ĺ�
						Overlay::AddLine(Bone[Neck].x, Bone[Neck].y, Bone[Spine].x, Bone[Spine].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						Overlay::AddLine(Bone[Spine].x, Bone[Spine].y, Bone[Pelvis].x, Bone[Pelvis].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						//����
						Overlay::AddLine(Bone[Pelvis].x, Bone[Pelvis].y, Bone[L_Thigh].x, Bone[L_Thigh].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						Overlay::AddLine(Bone[L_Thigh].x, Bone[L_Thigh].y, Bone[L_Calf].x, Bone[L_Calf].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						Overlay::AddLine(Bone[L_Calf].x, Bone[L_Calf].y, Bone[L_Foot].x, Bone[L_Foot].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						//����
						Overlay::AddLine(Bone[Pelvis].x, Bone[Pelvis].y, Bone[R_Thigh].x, Bone[R_Thigh].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						Overlay::AddLine(Bone[R_Thigh].x, Bone[R_Thigh].y, Bone[R_Calf].x, Bone[R_Calf].y, 1.0f, Overlay::toU32(g_Config->BoneColor));
						Overlay::AddLine(Bone[R_Calf].x, Bone[R_Calf].y, Bone[R_Foot].x, Bone[R_Foot].y, 1.0f, Overlay::toU32(g_Config->BoneColor));


					}
				}
			}


		}
	}

}

void MenuFn() {
	if (ImGui::Begin((const char*)_xor_(u8"Ardeon [Home ��ʾ ����]"))) {
		if (ImGui::CollapsingHeader((const char*)_xor_(u8"����ѡ��"))) {
			if (ImGui::TreeNode((const char*)_xor_(u8"����ѡ��")))
			{
				ImGui::Checkbox((const char*)_xor_(u8"<-��ʾ����"), &g_Config->ShowBox);
				ImGui::Checkbox((const char*)_xor_(u8"<-��ʾ����"), &g_Config->ShowLine);
				ImGui::Checkbox((const char*)_xor_(u8"<-��ʾ��Ϣ"), &g_Config->ShowMessageBox);
				ImGui::Checkbox((const char*)_xor_(u8"<-��ʾѪ��"), &g_Config->ShowHP);
				ImGui::Checkbox((const char*)_xor_(u8"<-��ʾ����"), &g_Config->ShowBone);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode((const char*)_xor_(u8"����ѡ��")))
			{
				ImGui::Text((const char*)_xor_(u8"������ɫ->")); ImGui::SameLine();
				ImGui::ColorEdit4((const char*)_xor_(u8"������ɫ##1"), (float*)&g_Config->BoxColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

				ImGui::Text((const char*)_xor_(u8"������ɫ->")); ImGui::SameLine();
				ImGui::ColorEdit4((const char*)_xor_(u8"������ɫ##1"), (float*)&g_Config->LineColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

				ImGui::Text((const char*)_xor_(u8"��Ϣ������ɫ->")); ImGui::SameLine();
				ImGui::ColorEdit4((const char*)_xor_(u8"��Ϣ������ɫ##1"), (float*)&g_Config->MessageBackGroundColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

				ImGui::Text((const char*)_xor_(u8"��Ϣ������ɫ->")); ImGui::SameLine();
				ImGui::ColorEdit4((const char*)_xor_(u8"��Ϣ������ɫ##1"), (float*)&g_Config->MessageTextColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

				ImGui::Text((const char*)_xor_(u8"��Ϣ������ɫ->")); ImGui::SameLine();
				ImGui::ColorEdit4((const char*)_xor_(u8"��Ϣ������ɫ##1"), (float*)&g_Config->MessageTriangleColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

				ImGui::Text((const char*)_xor_(u8"������ɫ->")); ImGui::SameLine();
				ImGui::ColorEdit4((const char*)_xor_(u8"������ɫ##1"), (float*)&g_Config->BoneColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

				ImGui::TreePop();
			}

		}
		if (ImGui::CollapsingHeader((const char*)_xor_(u8"��׼ѡ��")))
		{
			if (ImGui::TreeNode((const char*)_xor_(u8"����ѡ��")))
			{
				ImGui::Checkbox((const char*)_xor_(u8"<-��������"), &g_Config->DoAim);
				ImGui::Checkbox((const char*)_xor_(u8"<-��ʾ��Χ"), &g_Config->ShowAimRange);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode((const char*)_xor_(u8"����ѡ��")))
			{
				ImGui::Text((const char*)_xor_(u8"��Χ��ɫ->")); ImGui::SameLine();
				ImGui::ColorEdit4((const char*)_xor_(u8"��Χ��ɫ##1"), (float*)&g_Config->AimRangeColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::Separator();
				ImGui::Combo((const char*)_xor_(u8"<-�����ȼ�"), &g_Config->MenuChoiceHotKey, g_Config->ComboHotKey, IM_ARRAYSIZE(g_Config->ComboHotKey));
				ImGui::Combo((const char*)_xor_(u8"<-��׼��λ"), &g_Config->AimWhere, g_Config->ComboAimWhere, IM_ARRAYSIZE(g_Config->ComboAimWhere));
				ImGui::SliderFloat((const char*)_xor_(u8"���鷶Χ"), &g_Config->AimRangeValue, 10.0f, 300.0f);
				ImGui::TreePop();
			}
		}
		if (ImGui::CollapsingHeader((const char*)_xor_(u8"�ڴ湦��")))
		{
			if (ImGui::TreeNode((const char*)_xor_(u8"����ѡ��")))
			{
				ImGui::Checkbox((const char*)_xor_(u8"<-�����˳�"), &g_Config->ThirdMode);
				ImGui::Checkbox((const char*)_xor_(u8"<-��ǹ����"), &g_Config->NoGunNoAim);
				ImGui::Checkbox((const char*)_xor_(u8"<-�����Ҽ�"), &g_Config->LockRightInput);
				ImGui::Checkbox((const char*)_xor_(u8"<-����У׼"), &g_Config->NoRecoil);
				ImGui::TreePop();
			}


		}
		if (ImGui::CollapsingHeader((const char*)_xor_(u8"����ѡ��"))) {
			if (ImGui::TreeNode((const char*)_xor_(u8"����ѡ��")))
			{
				if (ImGui::Button((const char*)_xor_(u8"��ȡ����")))
				{

					g_Config->ReadConfig();
				}
				ImGui::SameLine();
				if (ImGui::Button((const char*)_xor_(u8"��������")))
				{
					g_Config->KeepConfig();
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

void RenderLoop() {
	if (g_Config->ShowMenu) {
		MenuFn();
	}
	if (g_Config->ShowFunction) {
		if (g_Config->ShowAimRange) {
			Overlay::AddCircle(Engine::GetScreenX(), Engine::GetScreenY(), 1.0f, 50, g_Config->AimRangeValue, Overlay::toU32(g_Config->AimRangeColor));
		}
		if (g_Config->GameInitful) {
			EspFn();
		}
	}
}
void Refresh() {
	g_Config->GameInitful = false;
	auto pContexts = Engine::GetContexts();
	while (Engine::Other_GetCount()<=0) {
		this_thread::sleep_for(std::chrono::microseconds(500));
	}
	g_Config->GameInitful = true;

	while (!Input) {
		auto pUserCommand = pContexts->GetUserCommand();
		if (pUserCommand) {
			if (pUserCommand->GetEntitiesNum() > 0) {
				auto pCommandEntity = pUserCommand->GetEntityByIndex(0);
				if (pCommandEntity) {
					auto pInput = pCommandEntity->GetInput();
					if (pInput) {
						Input = pInput;
					}


				}
			}
		}
	}



	while (true) {
		Engine::Refresh();
		switch (g_Config->MenuChoiceHotKey)
		{
		case 0:
			g_Config->AimHotkey = 1;
			break;
		case 1:
			g_Config->AimHotkey = 2;
			break;
		case 2:
			g_Config->AimHotkey = VK_LSHIFT;
			break;
		case 3:
			g_Config->AimHotkey = VK_LCONTROL;
			break;
		case 4:
			g_Config->AimHotkey = 70;
			break;
		case 5:
			g_Config->AimHotkey = 18;
			break;
		}
		if (GetAsyncKeyState(VK_HOME) & 0x8000)
		{
			g_Config->ShowMenu = !g_Config->ShowMenu;
			this_thread::sleep_for(chrono::milliseconds(500));
		}
	}
}

void Entry() {
	/*if (!CheckLoadFile()) {
		MessageBoxA(NULL, _xor_("������Ȩ�ѵ���,����ϵ���߹�����Ȩ."), _xor_("�Ϸ��Լ��"), MB_OK| MB_ICONERROR);
		exit(0);
	}*/
	g_Config->ReadConfig();
	while (MH_Initialize() != MH_OK) {
		Log(_xor_("[-]Thrad:%x Fail MH_Initialize()"), GetCurrentProcessId());
		this_thread::sleep_for(std::chrono::microseconds(500));
	}

	Engine::Init();
	while (!Mono::InitMono()) {
		Log(_xor_("[-]Thrad:%x Fail Mono::InitMono()"), GetCurrentProcessId());
		this_thread::sleep_for(std::chrono::microseconds(500));
	}

	Engine::GetOffsets();
	while (!Engine::CheckOffsets()) {
		Log(_xor_("[-]Thrad:%x Fail Engine::CheckOffsets()"), GetCurrentProcessId());
		this_thread::sleep_for(std::chrono::microseconds(500));
	}

	//thread AntiCheatThread(AntiCheatFn);
	thread DirectxHookThread(CreatDirectxHook, RenderLoop);
	thread RefreshThread(Refresh);
	thread AimThread(AimFn);


	//AntiCheatThread.detach();
	DirectxHookThread.detach();
	RefreshThread.detach();
	AimThread.detach();

	


	return;
}
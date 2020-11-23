#pragma once

#include <Windows.h>
#include <vector>
#include "imgui.h"
#include "menu.h"
#include "singleton.h"
#pragma warning(disable : 4244 4996 ) 

using namespace std;

class Config
	: public Singleton<Config>
{
	friend class Singleton<Config>;

public:

	string CODE, ID, PW, Key, Key2;
	BOOL IsLogin = false;
	/* Memory Access */
	HANDLE hProcess;
	HMODULE g_Module;
	DWORD64 BaseAddress;
	int Result;
	int Result2;
	ImFont* pFont;

	void* o_ComponentHook;

	template <typename RPMType>
	RPMType RPM(DWORD64 Address)
	{
		RPMType Buffer;
		ReadProcessMemory(hProcess, LPVOID(Address), &Buffer, sizeof(Buffer), NULL);
		return Buffer;
	}

	template <typename WPMType>
	BOOL WPM(DWORD64 Address, WPMType value)
	{
		return WriteProcessMemory(hProcess, LPVOID(Address), &value, sizeof(value), NULL);
	}

	uintptr_t MouseClickCheck;

	struct TW
	{
		ImVec2 A = ImVec2(0, 0);
		string Text;
		bool Alive;
	};
	TW DW[100];

	/* Cheat Settings */



	bool DrawFov = false;
	bool Draw2DBox = false;
	bool DrawEdge = false;
	bool DrawHeroName = false;
	bool DrawDist = false;












	bool Ultmate = true;
	bool UseAim = true;
	bool UseA1mb0t = true;
	bool UseAIMBOT = true;
	bool UseBox2SP = false;
	bool DiscoOutLine = false;
	bool DistanceESP = false;
	bool BoxESP = false;
	bool UseGlowESP = true;
	bool UseHP = true;
	bool Fov360 = false;
	bool UsePredict = false;
	bool UseGenjiBB = false;
	bool UseAutoMelee = false;
	bool °ÕÁö¿ë°Ë = false;
	bool °ÕÁö½ä±â = false;
	bool ESkill = false;
	bool ÀÚµ¿ÆòÅ¸ = false;
	bool UseAutoShot = false;
	bool UseNoRecoil = false;
	bool AnaSkill = false;
	bool DogSkill = false;
	bool TrQSkill = false;
	bool LESkill = false;
	bool PlayerTag = false;
	bool TriggerBot = false;
	bool DomPredit = false;
	bool Korean = true;
	bool English = false;
	//bool UseAllChamp = false;
	ImVec4 ESPColor = ImVec4(0.00f, 2.55f, 0.0f, 1.00f);
	ImVec4 ESPColor2 = ImVec4(0.00f, 2.55f, 0.0f, 1.00f);
	ImVec4 ESPColor3 = ImVec4(0.00f, 2.55f, 0.0f, 1.00f);
	ImVec4 ESPColor4 = ImVec4(0.00f, 2.55f, 0.0f, 1.00f);
	ImVec4 ESPColor5 = ImVec4(0.00f, 2.55f, 0.0f, 1.00f);
	ImVec4 ESPColorEdge = ImVec4(0.00f, 2.55f, 0.0f, 1.00f);
	ImVec4 ESPColorEdge2 = ImVec4(2.55f, 0.00f, 0.00f, 1.00f);
	ImVec4 ESPColor6 = ImVec4(2.55f, 0.00f, 0.00f, 1.00f);
	ImVec4 ESPColor7 = ImVec4(2.55f, 0.00f, 0.00f, 1.00f);
	ImVec4 ESPColor8 = ImVec4(2.55f, 0.00f, 0.00f, 1.00f);
	ImVec4 ESPColor9 = ImVec4(2.55f, 0.00f, 0.00f, 1.00f);
	ImVec4 ESPColor10 = ImVec4(2.55f, 0.00f, 0.00f, 1.00f);
	ImVec4 ESPColor11 = ImVec4(1.00f, 0.49f, 0.70f, 1.00f);
	float Fov = 0.110f;
	float GenjiFOV = 9.5f;
	float GenjiSpeed = 1.0f;
	float DomGunBulletSpeed = 80.0f;
	float DomBulletSpeed = 40.0f;
	float TriggerFOV = 10.F;
	float TRIGGER = 0.02f;
	float AimSpeed = 0.085f;
	float PredictVal = 24.0f;
	float PredictDistance = 10.0f;
	float YPITCH = 0.20f;
	float Humanize = 0.250f;
	float UltSpeed = 0.150f;
	bool UseYAxis = false;
	bool GENJISHIFT = false;
	bool GENJIQ = false;
	bool RFLICK = false;
	bool Silent = false;
	bool Predict = false;
	bool SombraESP = false;
	bool FovDraw = false;
	bool Genyata = false;
	bool Reinhardt = false;
	bool Roadhog = false;
	bool Mccree = false;
	bool NameESP = false;
	bool Ult_Zarya = true;
	bool Ult_Reinhardt = true;
	bool Ult_Tracer = true;
	bool Ult_Mei = true;
	bool Ult_LUCIO = true;
	bool Ult_Genji = true;
	bool LUCIO_Sombra = false;
	bool Sigma_Sombra = false;
	bool Hanjo_Sombra = false;
	bool moira_Sombra = false;
	bool Zarya_Dva = false;
	bool Tracer_Dva = false;
	bool Reaper_Sombra = false; 
	bool Tracer_Sigma = false;
	bool Mei_Dva = false;
	bool Reinhardt_Sombra = false;
	bool Reinhardt_Reinhardt = false;
	bool Mei_Sigma = false;
	bool Genji_Ana = false;
	bool Zarya_Sigma = false;
	bool HanzoFlick = false;
	float BulletSpeed = 105.0f;
	float HanzoSpeed = 110.f;
	float Ana_BulletSpeed = 125.0f;
	float BulletSpeed2 = 110.0f;
	float AnaBulletSpeed = 60.0f;
	float DogBulletSpeed = 35.0f;
	float GenjiBulletSpeed = 50.0f;
	float TracerBombSpeed = 15.0f;
	float Genji_BulletSpeed = 60.0f;
	float TorBulletSpeed = 70.0f;
	float SigmaBulletSpeed = 37.5;
	float ZenBulletSpeed = 90.0f;
	float EcoBulletSpeed = 75.0f;
	float OrisaBulletSpeed = 90.0f;
	float Sigma_BulletSpeed = 50.0f;
	float PHARAH_BulletSpeed = 35.0f;
	float Lucio_BulletSpeed = 50.0f;
	float °Å¸® = 15.0f;
	DWORD A1mH0tKey = VK_LBUTTON ;
	DWORD EVKey = 0x45;
	DWORD QVKey = 0x51;
	int ShiftKey = VK_LSHIFT;
	int AutoShotHotKey = VK_MBUTTON;
	int RButtonKey = VK_RBUTTON;
	uint32_t Color = 0xFF;

	// Menu
	bool menuEnabled = true;
};

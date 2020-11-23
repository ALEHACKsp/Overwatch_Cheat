#pragma region include
#include "D3dhook.h"
#include "Input.h"
#include <iostream>
#include "BreakPoint.h"
#include <fstream>
#include "Offset.h"
#include "Vector3.h"
#include "Memory.h"
#include <string>
#include <time.h>
#include <Windows.h>
#include <Tlhelp32.h>
#include <atlstr.h>
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <vector>
#include <thread>
#include "RGB.h" 
#include "Skin/Heroes.h"
#include "renderer.h"
#include "Skin/BotUtils.h"

#define M_PI       3.14159265358979323846
#define DEG2RAD(x) x * M_PI / 180.0
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment (lib, "winhttp.lib")
#pragma endregion

#define RAD2DEG(x)  ((x) * (180.f / M_PI) )

typedef struct KeyDownParam
{
	bool* KeyDwon = 0;
	int duration = 0;
	DWORD Key = 0;
}*key;

#pragma region 선언
INT AIMINDEX;
float pitch = 1.0f;
BOOL IsMouse = FALSE;
BOOL AutoShot = FALSE;
BOOL AutoSkill = FALSE;
BOOL AutoSkillL = FALSE;
BOOL AutoSkillLL = FALSE;
BOOL AutoSkillShift = FALSE;
BOOL AutoSkillShiftL = FALSE;
BOOL HanzoL = FALSE;
BOOL HanzoS = FALSE;
BOOL AutoSkillE = FALSE;
BOOL AutoSkillEL = FALSE;
BOOL AutoShotBOOL = FALSE;
BOOL Ana_Sleep = FALSE;
BOOL Ana_Sleep_Gun = FALSE;
BOOL IsMouseGen = FALSE;
BOOL SkillFuckingGenji = FALSE;
BOOL AutoSkillQ = FALSE;
BOOL bPress = FALSE;
BOOL Punch = FALSE;
BOOL EKey = FALSE;
BOOL QPress = FALSE;
BOOL Press = FALSE;
BOOL SkillFuckingsex = FALSE;
BOOL GenjiPress = FALSE;
BOOL SkillFuckingBOBO = FALSE;
BOOL Flick = FALSE;

BYTE ON = 0x1;
BYTE OFF = 0x0;

float Y = 1.0f;
float Distance;
float eVec;
float originDis = 100000.f;
float Humanize = Config::Get().AimSpeed;
Vector3 MyPos, EnPos, MyPos2, EnPos2, Velocity;
bool Alive;
bool TeamAlive;
bool VisWall;
DWORD_PTR SkillPress;
DWORD64 AnglePTR;
DWORD64 VisPTR;
BYTE Wall, Wall2, ow;
DWORD64 viewMatrixPtr;
DWORD64 EnSkill;
DWORD64 viewMatrixPtrFinal;
DWORD_PTR SkillBase;
DWORD_PTR HealthBase;
Heroes eSkinEnum;
Heroes mSkinEnum;
Matrix viewMatrix;

uint32_t TEE;
uint32_t TEE2;
uint32_t TEE3;

struct Player
{
	Vector3 Location, Velocity;
	bool Enemy, Visible, Alive;
	BYTE Wall;
	ULONG64 VisibleA;
	float Distance;

	Heroes Skin;

	Vector3 lastPos;
	clock_t lastVelocityUpdate;
	Vector3 savedVelocity;
};

int H{};

vector<ULONG64> NAMETagPTR;
Player Players[100];

#define GravityForce 9.81f * 0.5f * Dist / BulletSpeed * Dist / BulletSpeed
Vector3 MyAngle, TargetAngle;

static Vector3 staticAngle;
static Vector3 SilentAngle;

#pragma endregion

#pragma region Angle
Vector3 Ca1cAng12(Vector3 MyPos, Vector3 EnPos) // 앵글
{
	float Distance = MyPos.Distance(EnPos);

	Vector3 Result;

	Result.x = (EnPos.x - MyPos.x) / Distance;
	Result.y = (EnPos.y - MyPos.y) / Distance;
	Result.z = (EnPos.z - MyPos.z) / Distance;

	return Result;
}

Vector3 SilnetCa1c(Vector3 MyPos, Vector3 EnPos)
{
	float Distance = MyPos.Distance(EnPos);

	Vector3 Result;

	Result.x = (EnPos.x - MyPos.x) / Distance;
	Result.y = (EnPos.y - MyPos.y) / Distance;
	Result.z = (EnPos.z - MyPos.z) / Distance;

	return Result - Result;
}

Vector3 GetAngle(Vector3 RAngle, Vector3 MPos, Vector3 EPos)
{
	float Distance = MPos.Distance(EPos);

	Vector3 Result;

	Result.x = (EPos.x - MPos.x) / Distance;
	Result.y = (EPos.y - MPos.y) / Distance;
	Result.z = (EPos.z - MPos.z) / Distance;

	return Result;
}

int GetAimIndex() // 다중인식
{
	vector<DWORD64>tempNAMETagPTR = NAMETagPTR;
	int MI = 0, Result = -1;
	vector<float> Dist;
	for (int i = 0; i < tempNAMETagPTR.size(); i++)
	{
		Vector3 Screen = GetAngle(MyAngle, MyPos, Players[i].Location);
		if (Players[i].Enemy && Players[i].Alive && Players[i].Wall)
		{
			Dist.push_back(MyAngle.Distance(Screen));
		}
		else
		{
			Dist.push_back(0xFFFFFFFF);
		}
	}
	for (int i = 0; i < Dist.size(); i++)
	{
		if (Dist[MI] > Dist[i])
		{
			MI = i;
		}
	}
	Result = MI;
	return Result;
}

Vector3 Sm00thAng12(Vector3 LocalAngle, Vector3 TargetAngle, float X_Speed, float Y_Speed) // 스무스 앵글
{
	Vector3 Result;
	if (Config::Get().UseYAxis)
	{
		Result.x = (TargetAngle.x - LocalAngle.x) * X_Speed + LocalAngle.x;
		Result.y = (TargetAngle.y - LocalAngle.y) * 0.0 + LocalAngle.y;
		Result.z = (TargetAngle.z - LocalAngle.z) * X_Speed + LocalAngle.z;
	}
	else
	{
		Result.x = (TargetAngle.x - LocalAngle.x) * X_Speed + LocalAngle.x;
		Result.y = (TargetAngle.y - LocalAngle.y) * Y_Speed + LocalAngle.y;
		Result.z = (TargetAngle.z - LocalAngle.z) * X_Speed + LocalAngle.z;
	}

	return Result;
}

Vector3 F11ckSm00thAng12(Vector3 LocalAngle, Vector3 TargetAngle, float X_Speed, float Y_Speed) // 플릭 앵글
{
	Vector3 Result;
	if (Config::Get().UseYAxis)
	{
		Result.x = (TargetAngle.x - LocalAngle.x) * X_Speed / 1.0f / Config::Get().Humanize + LocalAngle.x;
		Result.y = (TargetAngle.y - LocalAngle.y) * 0.0 + LocalAngle.y;
		Result.z = (TargetAngle.z - LocalAngle.z) * X_Speed / 1.0f / Config::Get().Humanize + LocalAngle.z;
	}
	else
	{
		Result.x = (TargetAngle.x - LocalAngle.x) * X_Speed / 1.0f / Config::Get().Humanize + LocalAngle.x;
		Result.y = (TargetAngle.y - LocalAngle.y) * Y_Speed / 1.0f / Config::Get().Humanize + LocalAngle.y;
		Result.z = (TargetAngle.z - LocalAngle.z) * X_Speed / 1.0f / Config::Get().Humanize + LocalAngle.z;
	}

	return Result;
}

Vector3 Pr2dictCa1c(Vector3 MyPos, Vector3 EnPos, float BulletSpeed, float Dist) // 예측 앵글
{
	Vector3 Predict;
	Vector3 Result;
	Predict.x = EnPos.x + Velocity.x * Dist / BulletSpeed;
	if (mSkinEnum == 8)
	{
		Predict.y = EnPos.y + Velocity.y * GravityForce + Dist / BulletSpeed;
	}
	else
	{
		Predict.y = EnPos.y + Velocity.y * Dist / BulletSpeed;
	}
	Predict.z = EnPos.z + Velocity.z * Dist / BulletSpeed;

	Result.x = (Predict.x - MyPos.x) / Dist;
	Result.y = (Predict.y - MyPos.y) / Dist;
	Result.z = (Predict.z - MyPos.z) / Dist;
	return Result;
}
#pragma endregion
#pragma region NAMETAG
vector<MEMORY_BASIC_INFORMATION> mbis;
bool UpdateMemoryQuery()
{
	MEMORY_BASIC_INFORMATION mbi = { 0, };
	MEMORY_BASIC_INFORMATION old = { 0, };
	ULONG64 current_address = 0x7ffe0000;
	vector<MEMORY_BASIC_INFORMATION> addresses;
	while (true)
	{
		if (!VirtualQueryEx(GetCurrentProcess(), (PVOID)current_address, &mbi, sizeof(MEMORY_BASIC_INFORMATION)))
			break;
		if ((mbi.State & 0x1000) != 0 && (mbi.Protect & 0x100) == 0)
		{
			addresses.push_back(mbi);

			old = mbi;
		}
		current_address = ULONG64(mbi.BaseAddress) + mbi.RegionSize;
	}

	mbis = addresses;


	return (mbis.size() > 0);
}


ULONG64 FindPattern2(BYTE* buffer, BYTE* pattern, string mask, int bufSize)
{
	int pattern_len = mask.length();

	for (int i = 0; i < bufSize - pattern_len; i++)
	{
		bool found = true;
		for (int j = 0; j < pattern_len; j++)
		{
			if (mask[j] != '?' && pattern[j] != buffer[(i + j)])
			{
				found = false;
				break;
			}
		}
		if (found)
			return i;
	}
	return -1;
}

vector<ULONG64> FindPatternEx(ULONG64 start, ULONG64 end, BYTE* pattern, string mask, MEMORY_BASIC_INFORMATION mbi, ULONG64 RgSize)
{
	ULONG64 current_chunk = start;
	vector<ULONG64> found;
	if ((end - current_chunk > RgSize && RgSize != 0) || (end - current_chunk < RgSize && RgSize != 0))
		return found;
	while (current_chunk < end)
	{
		int bufSize = (int)(end - start);
		BYTE* buffer = new BYTE[bufSize];
		if (!ReadProcessMemory(GetCurrentProcess(), (LPVOID)current_chunk, buffer, bufSize, nullptr))
		{
			current_chunk += bufSize;
			delete[] buffer;
			continue;
		}

		ULONG64 internal_address = FindPattern2(buffer, pattern, mask, bufSize);
		if (internal_address != -1)
		{
			found.push_back(current_chunk + internal_address);
		}
		current_chunk += bufSize;
		delete[] buffer;

	}
	return found;
}

vector<ULONG64> _FindPatterns(BYTE* buffer, BYTE* pattern, string mask, int bufSize)
{
	vector<ULONG64> ret;
	int pattern_len = mask.length();
	for (int i = 0; i < bufSize - pattern_len; i++)
	{
		bool found = true;
		for (int j = 0; j < pattern_len; j++)
		{
			if (mask[j] != '?' && pattern[j] != buffer[i + j])
			{
				found = false;
				break;
			}
		}
		if (found)
			ret.push_back(i);
	}
	return ret;
}

ULONG64 FindPattern(BYTE* pattern, string mask, ULONG64 RgSize)
{
	if (!UpdateMemoryQuery())
		return 0;

	for (int i = 0; i < mbis.size(); i++) {
		MEMORY_BASIC_INFORMATION info = mbis[i];

		vector<ULONG64> arr = FindPatternEx(ULONG64(info.BaseAddress), info.RegionSize + ULONG64(info.BaseAddress), pattern, mask, info, RgSize);
		if (arr.size() > 0)
			return arr[0];
	}

	return 0;
}


vector<ULONG64> FindPatterns(BYTE* pattern, string mask, ULONG64 RgSize)
{
	vector<ULONG64> Result;
	ULONG64 PatternStart = FindPattern(pattern, mask, RgSize);
	if (PatternStart)
	{
		for (int i = 0; i < mbis.size(); i++)
		{
			if (ULONG64(mbis[i].BaseAddress) < PatternStart && PatternStart - ULONG64(mbis[i].BaseAddress) < mbis[i].RegionSize)
			{
				PatternStart = ULONG64(mbis[i].BaseAddress);
			}
		}

		BYTE* buf = new BYTE[RgSize];
		memcpy_s(buf, RgSize, PVOID(PatternStart), RgSize);

		vector<ULONG64> Pointers = _FindPatterns(buf, pattern, mask, RgSize);
		delete[] buf;

		for (int i = 0; i < Pointers.size(); i++)
			Pointers[i] += PatternStart;

		Result = Pointers;
	}

	return Result;
}
#pragma endregion

#pragma region Thread
uint64_t viewMarixP;
DWORD WINAPI Po1nt2rT(PVOID) // 네임태그
{
	while (true)
	{
		NAMETagPTR = FindPatterns((PBYTE)"\xFA\x42\x00\x00\x00\x00\x00\x00\x00\x00\x01\x04\x1E\x00\x07", "xx????xxxxxxxxx", 0x16000);

		viewMarixP = Config::Get().RPM<uint64_t>(Config::Get().BaseAddress + 0x2EF7B58);  // 뷰 메트릭스 패치 
		viewMarixP = Config::Get().RPM<uint64_t>(viewMarixP + 0x478);
		viewMarixP = Config::Get().RPM<uint64_t>(viewMarixP + 0x560);
		viewMarixP = Config::Get().RPM<uint64_t>(viewMarixP + 0x198);
		viewMarixP = Config::Get().RPM<uint64_t>(viewMarixP + 0x60);
		viewMatrix = Config::Get().RPM<Matrix>(viewMarixP + 0x470);

		H++;
		Sleep(50);
	}
	return 0;
}

void G2tV210c1ty() // 예측
{
	clock_t currTime = clock();

	for (int i = 0; i < NAMETagPTR.size(); i++)
	{
		if (Players[i].lastVelocityUpdate + 70 < currTime)
		{
			Players[i].savedVelocity = Players[i].Location - Players[i].lastPos;

			Players[i].savedVelocity.x *= 20.0f;
			Players[i].savedVelocity.y *= 20.0f;
			Players[i].savedVelocity.z *= 20.0f;

			Players[i].lastPos = Players[i].Location;

			Players[i].lastVelocityUpdate = currTime;
		}
		Players[i].Velocity = Players[i].savedVelocity;
	}
}

void DrawEdges(const ImVec2& top, const ImVec2& bot, const ImVec2& base, ImU32 col)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	float scale = (bot.y - top.y) / 3.0f;
	float length = scale / 2;
	window->DrawList->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale + length, top.y), col, 2.0f); //  --- Top left
	window->DrawList->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale, top.y + length), col, 2.0f); // | Top left
	window->DrawList->AddLine(ImVec2(base.x + scale / 3, top.y), ImVec2(base.x + scale / 3 + length, top.y), col, 2.0f); // --- Top right
	window->DrawList->AddLine(ImVec2(base.x + scale / 3 + length, top.y), ImVec2(base.x + scale / 3 + length, top.y + length), col, 2.0f); // | Top right
	window->DrawList->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale + length, bot.y), col, 2.0f); // --- Bottom left
	window->DrawList->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale, bot.y - length), col, 2.0f); // | Bottom left
	window->DrawList->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3, bot.y), col, 2.0f); // --- Bottom right
	window->DrawList->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3 + length, bot.y - length), col, 2.0f); // | Bottom right
}

void esp() // 거리 esp 기능 
{
	ImGuiWindow* window = ImGui::GetCurrentWindow(); // 임구이 오버레이

	for (int i = 0; i < NAMETagPTR.size(); i++)
	{
		//int AI = GetAimIndex();
		viewMatrix = Config::Get().RPM<Matrix>(viewMarixP + 0x470);
		Vector2 output{}, output2{};
		if (Players[i].Alive && Players[i].Enemy)
		{
			Vector3 Vec3 = Players[i].Location;
			if (viewMatrix.WorldToScreen(Vector3(Vec3.x, Vec3.y - 2.f, Vec3.z), 1920, 1080, output) &&
				viewMatrix.WorldToScreen(Vector3(Vec3.x, Vec3.y - 0.f, Vec3.z), 1920, 1080, output2))
			{
				float Size = abs(output.y - output2.y) / 2.0f;
				float Size2 = abs(output.y - output2.y) / 20.0f;
				float xpos = (output.x + output2.x) / 2;
				float ypos = output.y + Size / 5;
				//Config::Get().ESPColor5.x * 255, Config::Get().ESPColor5.y * 255, Config::Get().ESPColor5.z * 255, Config::Get().ESPColor5.w * 255
				ImVec4 distvisibleColor = Players[i].Wall ? ImVec4(Config::Get().ESPColor2.x * 255, Config::Get().ESPColor2.y * 255, Config::Get().ESPColor2.z * 255, Config::Get().ESPColor2.w * 255)
					: ImVec4(Config::Get().ESPColor7.x * 255, Config::Get().ESPColor7.y * 255, Config::Get().ESPColor7.z * 255, Config::Get().ESPColor7.w * 255); // 보일때 색바끼는거 - 박스 esp 색

				ImVec4 boxvisibleColor = Players[i].Wall ? ImVec4(Config::Get().ESPColor.x * 255, Config::Get().ESPColor.y * 255, Config::Get().ESPColor.z * 255, Config::Get().ESPColor.w * 255)
					: ImVec4(Config::Get().ESPColor6.x * 255, Config::Get().ESPColor6.y * 255, Config::Get().ESPColor6.z * 255, Config::Get().ESPColor6.w * 255); // 보일때 색바끼는거 - 박스 esp 색

				ImVec4 herovisibleColor = Players[i].Wall ? ImVec4(Config::Get().ESPColor4.x * 255, Config::Get().ESPColor4.y * 255, Config::Get().ESPColor4.z * 255, Config::Get().ESPColor4.w * 255)
					: ImVec4(Config::Get().ESPColor9.x * 255, Config::Get().ESPColor9.y * 255, Config::Get().ESPColor9.z * 255, Config::Get().ESPColor9.w * 255); // 보일때 색바끼는거 - 박스 esp 색

				if (Config::Get().Draw2DBox)// r / 255.f, g / 255.f, b / 255.f, a / 255.f
					window->DrawList->AddRect(ImVec2(xpos - Size / 1.8f, output.y), ImVec2(xpos + Size / 1.8f, output2.y), ImGui::GetColorU32(boxvisibleColor)); // 박스

				string dist = to_string((int)MyPos.Distance(Vec3)) + "M";
				if (Config::Get().DrawDist)
				{
					ImVec2 TextSize = ImGui::CalcTextSize(dist.c_str());
					window->DrawList->AddText(ImVec2(xpos - TextSize.x / 2.0f, output.y - TextSize.y / 2.0f), ImGui::GetColorU32(distvisibleColor), dist.c_str()); // 거리 색변경 
				}
				if (Config::Get().DrawHeroName)
				{
					ImVec2 TextSize = ImGui::CalcTextSize(Hero2Str(Players[i].Skin));
					window->DrawList->AddText(ImVec2(xpos - TextSize.x / 2.0f, output.y - TextSize.y / 2.0f - 15), ImGui::GetColorU32(herovisibleColor), Hero2Str(Players[i].Skin)); // 이영웅
				}
				ImVec4 edgeCol = Players[i].Wall ? ImVec4(Config::Get().ESPColorEdge.x * 255, Config::Get().ESPColorEdge.y * 255, Config::Get().ESPColorEdge.z * 255, Config::Get().ESPColorEdge.w * 255)
					: ImVec4(Config::Get().ESPColorEdge2.x * 255, Config::Get().ESPColorEdge2.y * 255, Config::Get().ESPColorEdge2.z * 255, Config::Get().ESPColorEdge2.w * 255); // 보일때 색바끼는거 - 박스 esp 색
				if (Config::Get().DrawEdge)
				{
					DrawEdges(ImVec2(xpos - Size / 1.5f, output.y), ImVec2(xpos + Size / 1.5f, output2.y), ImVec2(xpos - Size2 / 1.5f, output.y), ImGui::GetColorU32(edgeCol));
				}
				//if (i = AIMINDEX)
				//{
				//	ImVec4 TarGetColor = ImVec4(1, 1, 1, 1); // 박스 타켓 esp 색변겅

				//	if (Config::Get().Draw2DBox)// r / 255.f, g / 255.f, b / 255.f, a / 255.f
				//		window->DrawList->AddRect(ImVec2(xpos - Size / 1.8f, output.y), ImVec2(xpos + Size / 1.8f, output2.y), ImGui::GetColorU32(TarGetColor)); // 박스

				//	string dist = to_string((int)MyPos.Distance(Vec3)) + "M";
				//	if (Config::Get().DrawDist)
				//	{
				//		ImVec2 TextSize = ImGui::CalcTextSize(dist.c_str());
				//		window->DrawList->AddText(ImVec2(xpos - TextSize.x / 2.0f, output.y - TextSize.y / 2.0f), ImGui::GetColorU32(TarGetColor), dist.c_str()); // 거리 색변경 
				//	}
				//	if (Config::Get().DrawHeroName)
				//	{
				//		ImVec2 TextSize = ImGui::CalcTextSize(Hero2Str(Players[i].Skin));
				//		window->DrawList->AddText(ImVec2(xpos - TextSize.x / 2.0f, output.y - TextSize.y / 2.0f - 15), ImGui::GetColorU32(TarGetColor), Hero2Str(Players[i].Skin)); // 이영웅
				//	}
				//}

			}

		}
	}

}

class OWCOMPONENT
{
public:
	uint64_t componentBase;
	uint64_t componentParentPtr;
	uint8_t componentType;
	OWCOMPONENT() {}
};

std::vector<OWCOMPONENT> VelocityComponentList;
std::vector<OWCOMPONENT> OutlineComponentList;
std::vector<OWCOMPONENT> skillComponentList;//healthComponentList
std::vector<OWCOMPONENT> healthComponentList;

OWCOMPONENT HeroIDComponent;
OWCOMPONENT skillComponent;
OWCOMPONENT healthComponen;

unsigned __int64 __fastcall DecryptVis(__int64 a1)
{

	__int64 v1; // rbx
	unsigned __int64 v2; // rdi
	unsigned __int64 v3; // r9
	__int64 v4; // rbx
	unsigned __int64 v5; // rdx
	unsigned __int64 v6; // rcx
	__m128i v7; // xmm1
	__m128i v8; // xmm2
	__m128i v9; // xmm0
	__m128i v10; // xmm1

	v1 = a1;
	v2 = Config::Get().BaseAddress + 0x5F3DD0;
	v3 = v2 + 0x8;

	DWORD64* VisibleKeyPTR = (DWORD64*)(Config::Get().BaseAddress + 0x2BEFA40);
	v4 = v2 ^ *(DWORD64*)((char*)&VisibleKeyPTR[((BYTE)v1 - 0x6) & 0x7F] + (((unsigned __int64)(v1 - 0x257FABADDFDD7B06) >> 7) & 7)) ^ (v1 - 0x257FABADDFDD7B06);
	v5 = (v3 - v2 + 0x7) >> 0x3;
	v6 = 0x0;
	if (v2 > v3)
		v5 = 0x0;
	if (v5)
	{
		if (v5 >= 0x4)
		{
			ZeroMemory(&v7, sizeof(v7));
			ZeroMemory(&v8, sizeof(v8));
			do
			{
				v6 += 0x4;
				v7 = _mm_xor_si128(v7, _mm_loadu_si128((const __m128i*)v2));
				v9 = _mm_loadu_si128((const __m128i*)(v2 + 0x10));
				v2 += 0x20;
				v8 = _mm_xor_si128(v8, v9);
			} while (v6 < (v5 & 0xFFFFFFFFFFFFFFFCui64));
			v10 = _mm_xor_si128(v7, v8);
			v4 ^= *(DWORD64*)&_mm_xor_si128(v10, _mm_srli_si128(v10, 0x8));
		}
		for (; v6 < v5; ++v6)
		{
			v4 ^= *(DWORD64*)v2;
			v2 += 8i64;
		}
	}
	return v4 ^ ~v3 ^ 0xDA805452202284FAu;


}

DWORD WINAPI StructT(PVOID)
{
	while (true)
	{
		vector<DWORD64>tempNAMETagPTR = NAMETagPTR;
		if (tempNAMETagPTR.size())
		{
			for (int i = 0; i < tempNAMETagPTR.size(); i++)
			{
				Players[i].Location = Config::Get().RPM<Vector3>(tempNAMETagPTR[i] + 0x5A);
				Players[i].Location.y -= Config::Get().YPITCH;
				Players[i].Enemy = (Config::Get().RPM<BYTE>(tempNAMETagPTR[i] + 0x4) >= 0xA0) && (Config::Get().RPM<BYTE>(tempNAMETagPTR[i] + 0x4) < 0xB0) || (Config::Get().RPM<BYTE>(NAMETagPTR[i] + 0x4) == 0x80 || Config::Get().RPM<BYTE>(NAMETagPTR[i] + 0x4) == 0x99) ? true : false;
				Players[i].Alive = (Config::Get().RPM<BYTE>(tempNAMETagPTR[i] + 0x4) != 0x80) && Config::Get().RPM<BYTE>(tempNAMETagPTR[i] + 0x4) != 0xC5 ? true : false;
				G2tV210c1ty();
			}			
			this_thread::sleep_for(5ms);
		}
		else
		{
			this_thread::sleep_for(5ms);
		}
	}
	return 0;
}

#pragma endregion

#pragma region SendInPut

void SendVKcodesUp(BYTE vk)
{
	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = vk;

	//SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, &input, sizeof(INPUT));
}

void SendVKcodes(BYTE vk)
{
	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = vk;

	SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, &input, sizeof(INPUT));
}

#pragma endregion

#pragma region Function

int  __fastcall _SombraWallHook(PCONTEXT ctx)
{
	int ret = 0x14;
	__try
	{
		ctx->Rsp -= 0x30;

		if (*(WORD*)(ctx->Rdx + 0x30) == 0x0250 || *(WORD*)(ctx->Rdx + 0x30) == 0x01B3 || *(WORD*)(ctx->Rdx + 0x30) == 0x02DA)
		{
			if (*(BYTE*)(ctx->Rbx + 0x78) != 0x0A && *(BYTE*)(ctx->Rbx + 0x78) != 0x11)
			{
				ret = 0x16;
			}
			else
			{
				ret = (*(BYTE*)(ctx->Rcx + 0x268) == 0) ? 0x14 : 0x16;
			}
		}
		else
		{
			ret = (*(BYTE*)(ctx->Rcx + 0x268) == 0) ? 0x14 : 0x16;
		}


		ctx->Rdi = ctx->Rdx;
		ctx->Rbx = ctx->Rcx;
		ctx->R8 = ctx->Rdx;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return ret;
	}

	return ret;
}

typedef struct _PEB_LDR_DATA {
	UINT8 _PADDING_[12];
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB {
#ifdef _WIN64
	UINT8 _PADDING_[24];
#else
	UINT8 _PADDING_[12];
#endif
	PEB_LDR_DATA* Ldr;
} PEB, * PPEB;

typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	VOID* DllBase;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _UNLINKED_MODULE
{
	HMODULE hModule;
	PLIST_ENTRY RealInLoadOrderLinks;
	PLIST_ENTRY RealInMemoryOrderLinks;
	PLIST_ENTRY RealInInitializationOrderLinks;
	PLDR_DATA_TABLE_ENTRY Entry;
} UNLINKED_MODULE;

#define UNLINK(x)					\
	(x).Flink->Blink = (x).Blink;	\
	(x).Blink->Flink = (x).Flink;

#define RELINK(x, real)			\
	(x).Flink->Blink = (real);	\
	(x).Blink->Flink = (real);	\
	(real)->Blink = (x).Blink;	\
	(real)->Flink = (x).Flink;

std::vector<UNLINKED_MODULE> UnlinkedModules;

struct FindModuleHandle
{
	HMODULE m_hModule;
	FindModuleHandle(HMODULE hModule) : m_hModule(hModule)
	{
	}
	bool operator() (UNLINKED_MODULE const& Module) const
	{
		return (Module.hModule == m_hModule);
	}
};

void RemovePeHeader(HMODULE hModule)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((PBYTE)pDosHeader + (DWORD)pDosHeader->e_lfanew);

	if (pNTHeader->FileHeader.SizeOfOptionalHeader)
	{
		DWORD Protect;
		WORD Size = pNTHeader->FileHeader.SizeOfOptionalHeader;
		VirtualProtect((void*)hModule, Size, PAGE_EXECUTE_READWRITE, &Protect);
		SecureZeroMemory((void*)hModule, Size);
		VirtualProtect((void*)hModule, Size, Protect, &Protect);
	}
}

void RelinkModuleToPEB(HMODULE hModule)
{
	std::vector<UNLINKED_MODULE>::iterator it = std::find_if(UnlinkedModules.begin(), UnlinkedModules.end(), FindModuleHandle(hModule));

	if (it == UnlinkedModules.end())
	{
		//DBGOUT(TEXT("Module Not Unlinked Yet!"));
		return;
	}

	RELINK((*it).Entry->InLoadOrderLinks, (*it).RealInLoadOrderLinks);
	RELINK((*it).Entry->InInitializationOrderLinks, (*it).RealInInitializationOrderLinks);
	RELINK((*it).Entry->InMemoryOrderLinks, (*it).RealInMemoryOrderLinks);
	UnlinkedModules.erase(it);
}

void UnlinkModuleFromPEB(HMODULE hModule)
{
	std::vector<UNLINKED_MODULE>::iterator it = std::find_if(UnlinkedModules.begin(), UnlinkedModules.end(), FindModuleHandle(hModule));
	if (it != UnlinkedModules.end())
	{
		//DBGOUT(TEXT("Module Already Unlinked!"));
		return;
	}

#ifdef _WIN64
	PPEB pPEB = (PPEB)__readgsqword(0x60);
#else
	PPEB pPEB = (PPEB)__readfsdword(0x30);
#endif

	PLIST_ENTRY CurrentEntry = pPEB->Ldr->InLoadOrderModuleList.Flink;
	PLDR_DATA_TABLE_ENTRY Current = NULL;

	while (CurrentEntry != &pPEB->Ldr->InLoadOrderModuleList && CurrentEntry != NULL)
	{
		Current = CONTAINING_RECORD(CurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (Current->DllBase == hModule)
		{
			UNLINKED_MODULE CurrentModule = { 0 };
			CurrentModule.hModule = hModule;
			CurrentModule.RealInLoadOrderLinks = Current->InLoadOrderLinks.Blink->Flink;
			CurrentModule.RealInInitializationOrderLinks = Current->InInitializationOrderLinks.Blink->Flink;
			CurrentModule.RealInMemoryOrderLinks = Current->InMemoryOrderLinks.Blink->Flink;
			CurrentModule.Entry = Current;
			UnlinkedModules.push_back(CurrentModule);

			UNLINK(Current->InLoadOrderLinks);
			UNLINK(Current->InInitializationOrderLinks);
			UNLINK(Current->InMemoryOrderLinks);

			break;
		}

		CurrentEntry = CurrentEntry->Flink;
	}
}


void GenjiCombo() // 자동 용검 크힛
{
	Vector2 Delta = Config::Get().RPM<Vector2>(AnglePTR + 0x1090 + 0x4C);

	Vector3 AngleCompensated = staticAngle;

	if (Delta.x != 0 && Delta.y != 0)
	{
		AngleCompensated.x -= Delta.x;
		AngleCompensated.y -= Delta.y;
	}

	Config::Get().WPM<Vector3>(AnglePTR + 0x1090, AngleCompensated);

	if (AnglePTR)
	{
		Config::Get().WPM<DWORD>(AnglePTR + 0x1034, 0x8);

		if (Distance < 5.0f)
		{
			Config::Get().WPM<DWORD>(AnglePTR + 0x1034, 0x1);
			Sleep(10);
		}
	}
}

void TAimLock()
{
	Vector2 Delta = Config::Get().RPM<Vector2>(AnglePTR + 0x1090 + 0x4C);

	Vector3 AngleCompensated = staticAngle;

	if (Delta.x != 0 && Delta.y != 0)
	{
		AngleCompensated.x -= Delta.x;
		AngleCompensated.y -= Delta.y;
	}

	Config::Get().WPM<Vector3>(AnglePTR + 0x1090, AngleCompensated);
}

void PAimLock()
{
	Vector2 Delta = Config::Get().RPM<Vector2>(AnglePTR + 0x1090 + 0x4C);

	Vector3 AngleCompensated = staticAngle;

	if (Delta.x != 0 && Delta.y != 0)
	{
		AngleCompensated.x -= Delta.x;
		AngleCompensated.y -= Delta.y;
	}

	Config::Get().WPM<Vector3>(AnglePTR + 0x1090, AngleCompensated);

	Config::Get().WPM<DWORD>(AnglePTR + 0x1034, 0x800);
}

void GAimLock()
{
	Vector2 Delta = Config::Get().RPM<Vector2>(AnglePTR + 0x1090 + 0x4C);

	Vector3 AngleCompensated = staticAngle;

	if (Delta.x != 0 && Delta.y != 0)
	{
		AngleCompensated.x -= Delta.x;
		AngleCompensated.y -= Delta.y;
	}

	Config::Get().WPM<Vector3>(AnglePTR + 0x1090, AngleCompensated);

	Config::Get().WPM<DWORD>(AnglePTR + 0x1034, 0x8);
}

void FAimLock()
{
	Vector2 Delta = Config::Get().RPM<Vector2>(AnglePTR + 0x1090 + 0x4C);

	Vector3 AngleCompensated = staticAngle;

	if (Delta.x != 0 && Delta.y != 0)
	{
		AngleCompensated.x -= Delta.x;
		AngleCompensated.y -= Delta.y;
	}

	Config::Get().WPM<Vector3>(AnglePTR + 0x1090, AngleCompensated);

	Config::Get().WPM<uint32_t>(AnglePTR + 0x1034, 1);
}

void HAimLock()
{
	Vector2 Delta = Config::Get().RPM<Vector2>(AnglePTR + 0x1090 + 0x4C);

	Vector3 AngleCompensated = staticAngle;

	if (Delta.x != 0 && Delta.y != 0)
	{
		AngleCompensated.x -= Delta.x;
		AngleCompensated.y -= Delta.y;
	}

	Config::Get().WPM<Vector3>(AnglePTR + 0x1090, AngleCompensated);

	bool check = false;
}

void SilentAimLock() // 에임락
{
	Vector2 Delta = Config::Get().RPM<Vector2>(AnglePTR + 0x1090 + 0x4C);

	Vector3 AngleCompensated = staticAngle;

	if (Delta.x != 0 && Delta.y != 0)
	{
		AngleCompensated.x -= Delta.x;
		AngleCompensated.y -= Delta.y;
	}
	Config::Get().WPM<Vector3>(AnglePTR + 0x1090, AngleCompensated);

	Config::Get().WPM<uint32_t>(AnglePTR + 0x1034, 0x1);
}

void clearTimestamp(uint64_t m_getSkillStruct, uint32_t skillIdx, uint32_t skillIdx2)
{
	__try
	{
		if (TeamAlive && Alive)
		{
			__m128 skillStruct{};
			skillStruct.m128_u64[1] = SkillBase + OFFSET__SKillBase;
			uint16_t skillId[20] = { skillIdx, skillIdx2 };
			uint64_t skill = reinterpret_cast<uint64_t(__fastcall*)(__m128*, uint16_t*)>(m_getSkillStruct)(&skillStruct, skillId);
			if (!skill)
				return;
			Config::Get().WPM<uint32_t>(skill + 0x60, 1);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {}
}

uint32_t getTimestamp(uint64_t m_getSkillStruct, uint32_t skillIdx, uint32_t skillIdx2)
{
	__try
	{
		if (TeamAlive && Alive)
		{
			__m128 skillStruct{};
			skillStruct.m128_u64[1] = SkillBase + OFFSET__SKillBase;
			uint16_t skillId[20] = { skillIdx, skillIdx2 };
			uint64_t skill = reinterpret_cast<uint64_t(__fastcall*)(__m128*, uint16_t*)>(m_getSkillStruct)(&skillStruct, skillId);
			if (!skill)
				return NULL;
			return Config::Get().RPM<uint32_t>(skill + 0x60);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {}
	return NULL;
}

bool isSkillAvtivate(uint64_t m_getSkillStruct, uint16_t skillIdx, uint16_t skillIdx2)
{
	__try
	{
		if (TeamAlive && Alive)
		{
			__m128 skillStruct{};
			skillStruct.m128_u64[1] = SkillBase + OFFSET__SKillBase;
			uint16_t skillId[20] = { skillIdx, skillIdx2 };
			uint64_t skill = reinterpret_cast<uint64_t(__fastcall*)(__m128*, uint16_t*)>(m_getSkillStruct)(&skillStruct, skillId);
			if (!skill)
				return false;
			return Config::Get().RPM<uint8_t>(skill + 0x58) == 1; // 0x70
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {}
	return false;
}

LONG WINAPI Exc2pt10nHand1111er(EXCEPTION_POINTERS* e) // 모든 기능 테월 에임봇 등등
{
	auto ctx = e->ContextRecord;
	auto Addresss = e->ExceptionRecord->ExceptionAddress;

	if (Addresss == (void*)(Config::Get().BaseAddress + offset::FovHook))
	{
		__try
		{
			DWORD Filter = *(DWORD*)(*(DWORD64*)(ctx->Rsp + 0x38) - 0xD); //0x28 - 0xD, 0x38 - 0xD
			if (Filter == 0x8E8B49A5) //8E8B49A5
			{
				DWORD mSkinChecker = *(DWORD*)(*(DWORD64*)(ctx->R14 + 0x40) + 0x48); //내 스킨
				DWORD eSkinChecker = *(DWORD*)(*(DWORD64*)(ctx->R14 + 0x48) + 0x48); //적 스킨

				mSkinEnum = Skin2Hero(mSkinChecker);
				eSkinEnum = Skin2Hero(eSkinChecker);

				/*
				지금은 대충 짜드릴테니 그 직접 imgui 해가지고 한번에 정렬 하세여 ㅇㅋ ? ?? 무슨뜻인가욤 imgui에 셋팅 불러올때 해야할거아녀요 그건하는뎅 저거 예측이나..
			   지금 예측 짜놓은게 님이 짜놓은건 Velocity를 가져오는 기준이 NameTag 좌표니까 문제점이에요 m후킹에서 50ms 주고 하게될경우 프레임드랍이
			   발생하니 지금 방식에선 저렇게 하는게 맞고 님이 적의 모든 자료를 가져올 수 있으면 상관이 없는데 그걸 못가져오잖아요 ? 네
			   예를들어 벨로시티 , 체력 , 궁 게이지 , 스킬 쿨 , 캐릭 ID , 뼈 좌표 이런것들을 못가져오니까 지금 님이 쓴거처럼 Thread 돌려야되요
			   ㅇㅋ ? 네


			   eSkinEnum 에 담기는게 0 ~ 34 까지임 기준이고 ㅇㅋ ? 네네 그리고 캐릭마다 헤드값이 다른건알죠 ? 근데 앉은거 안맞아요 아넵
	아나 0
	애쉬 1
	바티 2
	바스 3
	브리 4
	둠피 5
	디바 6
	겐지 7
	한조 8 // Prediction 할때 Gravity 인짜 1
	정크 9
	루시우 10
	맥 11
	메이 12
	메르시 13
	모이라 14
	오리사 15
	파라 16
	리퍼 17
	라인 18
	로독 19
	솔저 20
	솜브라 21
	시메 22
	톨비 23 // Prediction 할때 Gravity 인자 1
	트레 24
	위메 25
	윈스 26
	레킹 27
	자리야 28
	젠야타 29
	시그마 30
	에코 31
	트레이닝 봇 32
	모든 히어로 33
	etc 34
	이외 나머지 Gravity 인자 0
				*/
				if (*(float*)(&ctx->Xmm7) == 0.01f || *(float*)(&ctx->Xmm8) == 0.01f)
				{

					AIMINDEX = GetAimIndex();

					MyPos = *(Vector3*)(ctx->Rsp + 0x3B0); // 28 - D -> 3A0, 38 - D 3C0 내 좌표
					EnPos = *(Vector3*)(ctx->Rsp + 0x1F0); // 28 - D -> 300, 38 - D 200 적 좌표 
					VisPTR = *(DWORD64*)(*(DWORD64*)(ctx->Rsp + 0x8C0) + 0x60);

					for (int i = 0; i < NAMETagPTR.size(); i++)
					{
						if (abs(EnPos.x - Players[i].Location.x) <= 0.5f && abs(EnPos.z - Players[i].Location.z) <= 2.0f)
						{
							Players[i].Wall = DecryptVis(*(DWORD64*)(VisPTR + 0x90)) ^ *(DWORD64*)(VisPTR + 0x88);// 이건 어케가지고있음? 씨발련ㄴ아 ? 줍줍했다  V-Aim한태 줬는데 
							// 벽구분은 30 주시면 할게요 ㅇㅋ ? 왜욘 씨발련 팔고 돈 가져온다음 추가하세요 예? ㅠㅠ 시발 
						}
						Players[i].Skin = eSkinEnum;
					}

					EnPos = Players[AIMINDEX].Location;
					Alive = Players[AIMINDEX].Alive;
					TeamAlive = Players[AIMINDEX].Enemy;
					Velocity = Players[AIMINDEX].Velocity;
					Distance = MyPos.Distance(Players[AIMINDEX].Location); // 거리
					// Visibile 넣고 
					float Distance2 = MyPos.Distance(EnPos); // 거리


					float Hanzo_BulletSpeed = Config::Get().HanzoSpeed;
					float ana_Gun_BulletSpeed = Config::Get().Ana_BulletSpeed;
					float Ana_BulletSpeed = Config::Get().AnaBulletSpeed;
					float Road_BulletSpeed = Config::Get().DogBulletSpeed;
					float Tracer_BombSpeed = Config::Get().TracerBombSpeed;
					float Genji_BulletSpeed = Config::Get().GenjiBulletSpeed;
					float Genji_Gun_BulletSpeed = Config::Get().Genji_BulletSpeed;
					float Sigma_BulletSpeed = Config::Get().SigmaBulletSpeed;
					float Tor_Gun_BulletSpeed = Config::Get().TorBulletSpeed;
					float Zen_Gun_BulletSpeed = Config::Get().ZenBulletSpeed;
					float Eco_Gun_BulletSpeed = Config::Get().EcoBulletSpeed;
					float Orisa_Gun_BulletSpeed = Config::Get().OrisaBulletSpeed;
					float DomFist_Gun_BulletSpeed = Config::Get().DomGunBulletSpeed;
					float DomFist_BulletSpeed = Config::Get().DomBulletSpeed;
					float Sigma_Gun_BulletSpeed = Config::Get().Sigma_BulletSpeed;
					float PHARAH_Gun_Speed = Config::Get().PHARAH_BulletSpeed;
					float Lucio_Gun_Speed = Config::Get().Lucio_BulletSpeed;

					/*for (int i = 0; i < 500; i++)
					{
						Heroes Test = Skin2Hero(*(DWORD*)(*(DWORD64*)(ctx->R14 + 0x40) + i));
						if (Test == Heroes::ECHO)
						{
							Config::Get().Result = i;
							break;
						}
					}*/
					if (MyAngle.Distance(Ca1cAng12(MyPos, EnPos)) < DEG2RAD(Config::Get().TriggerFOV / MyPos.Distance(EnPos)) && Config::Get().TriggerBot)
					{
						*(DWORD*)(AnglePTR + 0x1034) = 0x1;
					}

					if (MyAngle.Distance(Ca1cAng12(MyPos, EnPos)) < DEG2RAD(Config::Get().Fov))
					{

						if (AIMINDEX != -1 && Players[AIMINDEX].Wall) // 벽구분
						{
							if (Config::Get().UseA1mb0t)
							{
								if (TeamAlive && Alive && GetAsyncKeyState(VK_LBUTTON) && Players[AIMINDEX].Wall)
								{
									staticAngle = Sm00thAng12(MyAngle, Ca1cAng12(MyPos, EnPos), Config::Get().AimSpeed, Config::Get().AimSpeed);
									TAimLock();	
								}
							}

							if (Config::Get().GENJISHIFT)
							{
								if (mSkinEnum)
								{
									if (mSkinEnum == 7)
									{
										if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().RButtonKey))
										{
											if (Distance < Config::Get().거리)
											{
												staticAngle = F11ckSm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Genji_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
												GAimLock();
											}
										}
									}
								}
							}

							if (Config::Get().자동평타)
							{
								if (TeamAlive && Alive)
								{
									if (Distance < 3.2f)
									{
										staticAngle = Sm00thAng12(MyAngle, Ca1cAng12(MyPos, EnPos), Config::Get().AimSpeed, Config::Get().AimSpeed);
										PAimLock();
									}
								}
							}

							if (Config::Get().ESkill)
							{
								if (TeamAlive && Alive)
								{
									if (Distance < Config::Get().거리)
									{
										staticAngle = Sm00thAng12(MyAngle, Ca1cAng12(MyPos, EnPos), Config::Get().AimSpeed, Config::Get().AimSpeed);
										TAimLock();
									}
								}
							}

							if (Config::Get().DogSkill)
							{
								if (mSkinEnum)
								{
									if (mSkinEnum == 19)
									{
										if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().ShiftKey))
										{
											if (Distance < 20.f)
											{
												staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Road_BulletSpeed, MyPos.Distance(EnPos)), Config::Get().AimSpeed, Config::Get().AimSpeed);
												TAimLock();
											}
										}
									}
								}
							}

							if (Config::Get().겐지용검)
							{
								if (mSkinEnum == 7)
								{
									if (*(BYTE*)(AnglePTR + 0x25C) == 0xB6)//궁인식 //23E
									{
										if (TeamAlive && Alive)
										{
											if (Distance < Config::Get().거리)
											{
												bPress = TRUE;// 질풍 && 썰기
											}
										}
									}
								}
							}

							if (Config::Get().LESkill)
							{
								if (mSkinEnum)
								{
									if (mSkinEnum == 30)
									{
										if (TeamAlive && Alive && GetAsyncKeyState(0x45))
										{
											staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Sigma_BulletSpeed, MyPos.Distance(EnPos)), Config::Get().AimSpeed, Config::Get().AimSpeed);
											TAimLock();
										}
									}
								}
							}

							if (Config::Get().TrQSkill)
							{
								if (mSkinEnum)
								{
									if (mSkinEnum == 24)
									{
										if (TeamAlive && Alive && GetAsyncKeyState(0x51))
										{
											if (Distance < 6.0f)
											{
												staticAngle = F11ckSm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Tracer_BombSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
												TAimLock();
											}
										}
									}
								}
							}

							if (Config::Get().DomPredit)
							{
								if (mSkinEnum == 5) //둠피
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().RButtonKey))
									{
										if (Distance < 20.0f)
										{
											staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, DomFist_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
											TAimLock();
										}
									}
								}
							}

							if (Config::Get().AnaSkill)
							{
								if (mSkinEnum == 0)
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().ShiftKey))
									{
										staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Ana_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
										TAimLock();
									}
								}
							}

							if (Config::Get().RFLICK)
							{
								if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
								{
									staticAngle = F11ckSm00thAng12(MyAngle, Ca1cAng12(MyPos, EnPos), Config::Get().AimSpeed, Config::Get().AimSpeed);
									FAimLock();
								}
							}

							if (Config::Get().Ultmate)
							{
								if (eSkinEnum == 18) // 라인 궁 막기
								{
									if (!getTimestamp(Config::Get().BaseAddress + offset::skill, 0, 0x156))
									{
										clearTimestamp(Config::Get().BaseAddress + offset::skill, 0, 0x156);

										if (mSkinEnum == 21 && TeamAlive && Alive && Distance < 15.f) // 솜브라로 라인 궁 차단
										{
											Config::Get().WPM<uint32_t>(AnglePTR + 0x1034, 0x20);
										}
									}
								}
							}

							if (Config::Get().Silent)
							{
								if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
								{
									Vector3 originalAngle = MyAngle;
									staticAngle = F11ckSm00thAng12(MyAngle, Ca1cAng12(MyPos, EnPos), 2, 2);
									SilentAimLock();
									uintptr_t tmp = *(uintptr_t*)AnglePTR + 0x1090;
									tmp = *(uintptr_t*)AnglePTR - 0x5C;
									*(BYTE*)tmp = 1;
									staticAngle = MyAngle;
									SilentAimLock();
								}
							}

							if (Config::Get().Predict)
							{
								if (mSkinEnum == 8) //한조
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
									{
										staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, ana_Gun_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
										TAimLock();
									}
								}

								if (mSkinEnum == 0) //아나
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
									{
										staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, ana_Gun_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
										TAimLock();
									}
								}

								if (mSkinEnum == 5) //둠피
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
									{
										staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, DomFist_Gun_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
										TAimLock();

									}
								}

								if (mSkinEnum == 7) //겐지
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
									{
										staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Genji_Gun_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
										TAimLock();
									}
								}

								if (mSkinEnum == 23) //토르비욘
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
									{
										staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Tor_Gun_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
										TAimLock();
									}
								}

								if (mSkinEnum == 29) //젠야타
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
									{
										staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Zen_Gun_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
										TAimLock();
									}
								}

								if (mSkinEnum == 31) //에코
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
									{
										staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Eco_Gun_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
										TAimLock();
									}
								}

								if (mSkinEnum == 15) //오리사
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
									{
										staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Orisa_Gun_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
										TAimLock();
									}
								}

								if (mSkinEnum == 30) //시그마
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
									{
										if (Distance < 22.f)
										{
											staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Sigma_Gun_BulletSpeed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
											TAimLock();
										}
									}
								}

								if (mSkinEnum == 16) //파라
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
									{
										if (Distance < 50.f)
										{
											staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, PHARAH_Gun_Speed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
											TAimLock();
										}
									}
								}

								if (mSkinEnum == 10) //루시우
								{
									if (TeamAlive && Alive && GetAsyncKeyState(Config::Get().A1mH0tKey))
									{
										if (Distance < 22.f)
										{
											staticAngle = Sm00thAng12(MyAngle, Pr2dictCa1c(MyPos, EnPos, Lucio_Gun_Speed, Distance), Config::Get().AimSpeed, Config::Get().AimSpeed);
											TAimLock();
										}
									}
								}


							}
						}
					}
				}
			}
		}

		__except (EXCEPTION_EXECUTE_HANDLER) // 저희가 짠 명령어 중에서, 예외 (오류) 가 발생할 수 있으니 그것도 잡아줍시다.
		{

		}

		ctx->Rax ^= ctx->R9;
		ctx->Rip += 0x3;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	if (Addresss == (PVOID)(Config::Get().BaseAddress + offset::BorderLine)) // 색변경 테월
	{
		ctx->Rcx = ctx->Rsi;
		Vector3 BorderPos = *(Vector3*)(ctx->Rcx + 0x140);
		if (Config::Get().UseBox2SP)
		{
			ctx->Rdx = 0xC0;
			if (Config::Get().DiscoOutLine)
			{
				Color Color = HSVToRGB(1, H % 360, 1);
				ctx->R8 = RGBA2ARGB(Color.R, Color.G, Color.B, 0xFF);
			}
			else
			{
				if (abs(BorderPos.x - EnPos.x) < 0.5f && abs(BorderPos.z - EnPos.z) < 2.0f)
				ctx->R8 = RGBA2ARGB(255, 0, 0, 255);

				if (abs(BorderPos.x - EnPos.x) < 0.5f && abs(BorderPos.z - EnPos.z) < 0.5f)
				{
					ctx->R8 = RGBA2ARGB(Config::Get().ESPColor5.x * 100, Config::Get().ESPColor5.y * 100, Config::Get().ESPColor5.z * 100, Config::Get().ESPColor5.w * 255);
				}

			}

		}

		ctx->Rip += 0x3;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	if (Addresss == (PVOID)(Config::Get().BaseAddress + offset::AngleHook)) // 앵글
	{
		MyAngle = *(Vector3*)(ctx->Rsp + 0x20);

		AnglePTR = ctx->Rdi;

		if (IsMouse == TRUE)
		{
			Vector3 Multiple = { 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed };
			TargetAngle.x = ((MyAngle.x - TargetAngle.x) * Multiple.x) + TargetAngle.x;
			TargetAngle.y = ((MyAngle.y - TargetAngle.y) * Multiple.y) + TargetAngle.y;
			TargetAngle.z = ((MyAngle.z - TargetAngle.z) * Multiple.z) + TargetAngle.z;
			*(Vector3*)(ctx->Rsp + 0x20) = TargetAngle;
			IsMouse = FALSE;
		}
		
		if (AutoSkill == TRUE)
		{
			AutoSkillQ = TRUE;
			AutoSkill = FALSE;
		}

		if (AutoSkillL == TRUE)
		{
			Vector3 Multiple = { 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed };
			TargetAngle.x = ((MyAngle.x - TargetAngle.x) * Multiple.x) + TargetAngle.x;
			TargetAngle.y = ((MyAngle.y - TargetAngle.y) * Multiple.y) + TargetAngle.y;
			TargetAngle.z = ((MyAngle.z - TargetAngle.z) * Multiple.z) + TargetAngle.z;
			*(Vector3*)(ctx->Rsp + 0x20) = TargetAngle;
			AutoSkillLL = TRUE;
			AutoSkillL = FALSE;
		}

		if (AutoSkillShift == TRUE)
		{
			Vector3 Multiple = { 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed };
			TargetAngle.x = ((MyAngle.x - TargetAngle.x) * Multiple.x) + TargetAngle.x;
			TargetAngle.y = ((MyAngle.y - TargetAngle.y) * Multiple.y) + TargetAngle.y;
			TargetAngle.z = ((MyAngle.z - TargetAngle.z) * Multiple.z) + TargetAngle.z;
			*(Vector3*)(ctx->Rsp + 0x20) = TargetAngle;
			AutoSkillShiftL = TRUE;
			AutoSkillShift = FALSE;
		}

		if (AutoSkillE == TRUE)
		{
			Vector3 Multiple = { 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed };
			TargetAngle.x = ((MyAngle.x - TargetAngle.x) * Multiple.x) + TargetAngle.x;
			TargetAngle.y = ((MyAngle.y - TargetAngle.y) * Multiple.y) + TargetAngle.y;
			TargetAngle.z = ((MyAngle.z - TargetAngle.z) * Multiple.z) + TargetAngle.z;
			*(Vector3*)(ctx->Rsp + 0x20) = TargetAngle;
			AutoSkillEL = TRUE;
			AutoSkillE = FALSE;
		}

		if (AutoShot == TRUE)
		{
			Vector3 Multiple = { 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed };
			TargetAngle.x = ((MyAngle.x - TargetAngle.x) * Multiple.x) + TargetAngle.x;
			TargetAngle.y = ((MyAngle.y - TargetAngle.y) * Multiple.y) + TargetAngle.y;
			TargetAngle.z = ((MyAngle.z - TargetAngle.z) * Multiple.z) + TargetAngle.z;
			*(Vector3*)(ctx->Rsp + 0x20) = TargetAngle;
			AutoShot = FALSE;
		}

		if (IsMouseGen == TRUE)
		{
		    Vector3 Multiple = { 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed };
			TargetAngle.x = ((MyAngle.x - TargetAngle.x) * Multiple.x) + TargetAngle.x;
			TargetAngle.y = ((MyAngle.y - TargetAngle.y) * Multiple.y) + TargetAngle.y;
			TargetAngle.z = ((MyAngle.z - TargetAngle.z) * Multiple.z) + TargetAngle.z;
			*(Vector3*)(ctx->Rsp + 0x20) = TargetAngle;
			SkillFuckingGenji = TRUE;
			IsMouseGen = FALSE;
		}

		if (Punch)
		{
			Vector3 Multiple = { 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed, 1.0f - Config::Get().AimSpeed };
			TargetAngle.x = ((MyAngle.x - TargetAngle.x) * Multiple.x) + TargetAngle.x;
			TargetAngle.y = ((MyAngle.y - TargetAngle.y) * Multiple.y) + TargetAngle.y;
			TargetAngle.z = ((MyAngle.z - TargetAngle.z) * Multiple.z) + TargetAngle.z;
			*(Vector3*)(ctx->Rsp + 0x20) = TargetAngle;
			SkillFuckingBOBO = TRUE;
			Punch = FALSE;
		}
		*(Vector3*)(&ctx->Xmm0) = *(Vector3*)(ctx->Rsp + 0x20);
		ctx->Rip += 0x5;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	if (Addresss == (PVOID)(Config::Get().BaseAddress + offset::CompoenetHook))
	{
		uint64_t ComponentBase = ctx->Rax;
		uint64_t ComponentParent = ctx->Rsi;  // 컴포넌트 패치 2
		byte ComponentIdx = LOBYTE(ctx->Rcx);

		if (ComponentIdx == Compo__SKILL) // 적 스킬 가져오는 컴포 id
		{
			OWCOMPONENT c = OWCOMPONENT();
			c.componentBase = ComponentBase;
			c.componentParentPtr = ComponentParent;
			c.componentType = Compo__SKILL;
			skillComponentList.push_back(c);
			SkillBase = c.componentBase;
		}

		else if (ComponentIdx == Compo__HEALTH)// 적 체력 가져오는 컴포 id
		{
			OWCOMPONENT c = OWCOMPONENT();
			c.componentBase = ComponentBase;
			c.componentParentPtr = ComponentParent;
			c.componentType = Compo__HEALTH;
			healthComponentList.push_back(c);
			HealthBase = c.componentBase;
		}

		ctx->Rbp = ctx->Rax;    // 컴포넌트 패치 1
		ctx->Rip += 0x3;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}
#pragma endregion

#pragma region initsk
void In1tHardWareBr2akP01nt() // rip veh 훅 등등
{
	HANDLE hMainThread = HW1BP->G2tMa1nThre2d();
	srand(GetTickCount64());
	PVOID pHandler = AddVectoredExceptionHandler(1, Exc2pt10nHand1111er);
	CONTEXT c{};
	c.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	SuspendThread(hMainThread);
	c.Dr0 = Config::Get().BaseAddress + offset::BorderLine;
	c.Dr1 = Config::Get().BaseAddress + offset::FovHook;
	c.Dr2 = Config::Get().BaseAddress + offset::AngleHook;
	c.Dr3 = Config::Get().BaseAddress + offset::CompoenetHook;
	c.Dr7 = (1 << 0) | (1 << 2) | (1 << 4) | (1 << 6);
	SetThreadContext(hMainThread, &c);
	ResumeThread(hMainThread);

	////////////////// Thread //////////////////
	CloseHandle(CreateThread(nullptr, 0, Po1nt2rT, nullptr, 0, nullptr));
	CloseHandle(CreateThread(nullptr, 0, StructT, nullptr, 0, nullptr));
	////////////////// Thread //////////////////
}

void In1tBas2S2tt1ngs() // 프로세스 얻기
{
	Config::Get().hProcess = GetCurrentProcess();
	Config::Get().BaseAddress = (DWORD64)GetModuleHandleA(("Overwatch.exe"));
}
#pragma endregion

#pragma region Dllmain
DWORD WINAPI IMGUILOGIN(LPVOID lpParam)
{
	/*if (AllocConsole())
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}*/

	InputSys::Get().Initialize();
	D3dHook::AttachHook();
	ImGuiStyle* style = &ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsClassic(); // 디자인 변경 임구이

	Config::Get().pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\cs.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());

	while (!Config::Get().IsLogin)
	{

	}

	In1tBas2S2tt1ngs();
	In1tHardWareBr2akP01nt();

	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD0)) // 핫키
		{
			Config::Get().Draw2DBox = !Config::Get().Draw2DBox; 
			Sleep(100); 
		}
		if (GetAsyncKeyState(VK_NUMPAD1)) // 핫키
		{
			Config::Get().DrawDist = !Config::Get().DrawDist; 
			Sleep(100); 
		}
		if (GetAsyncKeyState(VK_NUMPAD2)) // 핫키
		{
			Config::Get().DrawHeroName = !Config::Get().DrawHeroName; 
			Sleep(100);
		}
		if (GetAsyncKeyState(VK_NUMPAD3)) // 핫키
		{
			Config::Get().UseBox2SP = !Config::Get().UseBox2SP; 
			Sleep(100); 
		}
		if (GetAsyncKeyState(VK_NUMPAD4)) // 핫키
		{
			Config::Get().UseA1mb0t = !Config::Get().UseA1mb0t;
			Sleep(100);
		}
		if (GetAsyncKeyState(VK_NUMPAD5)) // 핫키
		{
			Config::Get().RFLICK = !Config::Get().RFLICK;
			Sleep(100); 
		}
		if (GetAsyncKeyState(VK_NUMPAD6)) // 핫키
		{
			Config::Get().Predict = !Config::Get().Predict;
			Sleep(100); 
		}
		if (GetAsyncKeyState(VK_NUMPAD7)) // 핫키
		{
			Config::Get().UseYAxis = !Config::Get().UseYAxis; 
			Sleep(100);
		}
		if (GetAsyncKeyState(VK_NUMPAD8)) // 핫키
		{
			Config::Get().DrawFov = !Config::Get().DrawFov;
			Sleep(100); 
		}
		if (GetAsyncKeyState(VK_NUMPAD9)) // 핫키
		{
			Config::Get().자동평타 = !Config::Get().자동평타; 
			Sleep(100); 
		}

		if (AnglePTR)
		{
			if (bPress)
			{
				staticAngle = F11ckSm00thAng12(MyAngle, Ca1cAng12(MyPos, EnPos), Config::Get().AimSpeed, Config::Get().AimSpeed);
				GenjiCombo();
				bPress = FALSE;
				Sleep(10);
			}

			if (SkillFuckingBOBO)
			{
				*(WORD*)(AnglePTR + 0x1034) = 0x800; // 주먹
				SkillFuckingBOBO = FALSE;
				Sleep(270);
			}

			if (SkillFuckingsex)
			{
				*(BYTE*)(AnglePTR + 0x1034) = 0x8; // 질풍
				if (Distance < 5.0f)
				{
					*(WORD*)(AnglePTR + 0x1034) = 0x1; // 평타
					Sleep(1);
				}
				SkillFuckingsex = FALSE;
				Sleep(1);
			}

			if (AutoSkillQ)
			{
				*(BYTE*)(AnglePTR + 0x1034) = 0x20;
				AutoSkillQ = FALSE;
				Sleep(5);
			}

			if (AutoSkillLL)
			{
				*(BYTE*)(AnglePTR + 0x1034) = 0x2;
				AutoSkillLL = FALSE;
				Sleep(5);
			}

			if (AutoSkillShiftL)
			{
				*(BYTE*)(AnglePTR + 0x1034) = 0x8;
				AutoSkillShiftL = FALSE;
				Sleep(5);
			}

			if (AutoSkillEL)
			{
				*(BYTE*)(AnglePTR + 0x1034) = 0x10;
				AutoSkillEL = FALSE;
				Sleep(5);
			}

			if (SkillFuckingGenji)
			{
				if (Distance < Config::Get().거리)
				{
					*(WORD*)(AnglePTR + 0x1034) = 0x8;
					Sleep(200);
				}
				SkillFuckingGenji = FALSE;
				Sleep(200);
			}
		}
	}
	D3dHook::DetachHook();
	FreeLibraryAndExitThread((HMODULE)lpParam, 0);
	return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved)
{

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hInst);
		RemovePeHeader((HMODULE)hInst); // 우회
		CloseHandle(CreateThread(nullptr, 0, IMGUILOGIN, (LPVOID)hInst, 0, nullptr));
	}
	return TRUE;
}

#pragma endregion
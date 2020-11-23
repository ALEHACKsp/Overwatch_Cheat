#include "menu.h"
#include <urlmon.h>
#include <winhttp.h>
#include <iostream>
#include<stdio.h>
#include <time.h>
#include "Vector3.h"
#include "Skin/Heroes.h"
#define DEG2RAD(x) x * M_PI / 180.0
#define M_PI       3.14159265358979323846

BYTE GlowOFF = 0x40;
BYTE GlowON = 0xFF;
static int TabCount = 0;
const char* hotkeylist[] = { u8"��Ŭ��", (u8"��Ŭ��"), (u8"��Ŭ��"), (u8"����Ʈ") };
static const char* current_hotkey = (u8"��Ŭ��");

const char* bonelist[] = { ("�Ӹ�"), ("��"), ("����") };
static const char* current_bonepos = ("HEAD");

const char* PredictList[] = { u8"��Ŭ�� ����", (u8"����Ʈ ����"), (u8"EŰ ����"), (u8"RŰ ����")	 };
static const char* current_predict = (u8"��Ŭ�� ����");


float NeckVal = 0.85f;
float BodyVal = 0.50f;


void Menu::Initialize(ID3D11Device *pDev, ID3D11DeviceContext *pCont) {

	pDevice = pDev;
	pContext = pCont;
	HWND hWindow = FindWindow("TankWindowClass", NULL);


	ImGui_ImplDX11_Init(hWindow, pDevice, pContext);
	ImGui_ImplDX11_CreateDeviceObjects();

	CreateStyle();
}

//
//ULONG KeyState(UINT VKey)
//{
//	VK_LBUTTON
//	if (GetAsyncKeyState(VKey) & 0x8000)
//		return 0x8000;
//	else
//		return 0;
//}
//
//void a()
//{
//	KeyState();
//
//
//	return;
//}
bool onkey;

#pragma region Project

static const int B64index[256] =
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  62, 63, 62, 62, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0,  0,  0,
	0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,  0,  0,  0,  63,
	0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

const string b64decode(const void* data, const size_t& len)
{
	if (len == 0) return "";

	unsigned char* p = (unsigned char*)data;
	size_t j = 0,
		pad1 = len % 4 || p[len - 1] == '=',
		pad2 = pad1 && (len % 4 > 2 || p[len - 2] != '=');
	const size_t last = (len - pad1) / 4 << 2;
	string result(last / 4 * 3 + pad1 + pad2, '\0');
	unsigned char* str = (unsigned char*)&result[0];

	for (size_t i = 0; i < last; i += 4)
	{
		int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
		str[j++] = n >> 16;
		str[j++] = n >> 8 & 0xFF;
		str[j++] = n & 0xFF;
	}
	if (pad1)
	{
		int n = B64index[p[last]] << 18 | B64index[p[last + 1]] << 12;
		str[j++] = n >> 16;
		if (pad2)
		{
			n |= B64index[p[last + 2]] << 6;
			str[j++] = n >> 8 & 0xFF; // ����� e
		}
	}
	return result;
}

string b64decode(const string& str64)
{
	return b64decode(str64.c_str(), str64.size());
}

std::wstring get_utf16(const std::string& str, int codepage)
{
	if (str.empty()) return std::wstring();
	int sz = MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), 0, 0);
	std::wstring res(sz, 0);
	MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), &res[0], sz);
	return res;
}


string WebWinhttp(string details) {
	DWORD dwSize = 0, dwDownloaded;
	LPSTR source;
	source = (char*)"";
	string responsed = "";

	HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
	BOOL bResults = FALSE;

	hSession = WinHttpOpen(L"Winhttp API", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	if (hSession)
		hConnect = WinHttpConnect(hSession, get_utf16("hong333.dothome.co.kr", CP_UTF8).c_str(), INTERNET_DEFAULT_HTTP_PORT, 0); // �ڽ��� ��Ȩ���� 

	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET", get_utf16(details, CP_UTF8).c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

	if (hRequest)
		bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	if (bResults) {
		do {
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u", GetLastError());

			source = (char*)malloc(dwSize + 1);
			if (!source) {
				printf("Out of memory\n");
				dwSize = 0;
			}
			else {
				ZeroMemory(source, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)source, dwSize, &dwDownloaded))
					printf("Error %u", GetLastError());
				else
					responsed = responsed + source;
				free(source);
			}
		} while (dwSize > 0);
	}

	if (!bResults) {
		exit(0);
	}

	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	return responsed;
}

string encryptDecrypt(string toEncrypt) {
	char key = 'K';
	string output = toEncrypt;
	for (int i = 0; i < toEncrypt.size(); i++)
		output[i] = toEncrypt[i] ^ key;
	return output;
}

string getBoolString(bool Bool, string A, string B) {
	if (Bool)
		return A;
	else
		return B;// 
}

///<summary>
///BOOL�� ���¿� ���� ���ϰ�� ON��, �����ϰ�� OFF�� ��ȯ�մϴ�.
///</summary>
///<param name="Bool">���� BOOL</param>
string getOnOff(bool Bool) {
	if (Bool)
		return "ON";
	else
		return "OFF";
}

#pragma endregion

wstring GetStringenum(const string& str, int codepage)
{
	if (str.empty()) return  wstring();
	int sz = MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), 0, 0);
	wstring res(sz, 0);
	MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), &res[0], sz);
	return res;
}

std::string get_system_uuid()
{
	if (std::system("wmic csproduct get uuid > HWID.txt") == 0)
	{
		auto file = ::fopen("HWID.txt", "rt, ccs=UNICODE"); // open the file for unicode input

		enum { BUFFSZ = 1000, UUID_SZ = 36 };
		wchar_t wbuffer[BUFFSZ]; // buffer to hold unicode characters

		if (file && // file was succesffully opened
			::fgetws(wbuffer, BUFFSZ, file) && // successfully read (and discarded) the first line
			::fgetws(wbuffer, BUFFSZ, file)) // yfully read the second line
		{
			char cstr[BUFFSZ]; // buffer to hold the converted c-style string
			if (::wcstombs(cstr, wbuffer, BUFFSZ) > UUID_SZ) // convert unicode to utf-8
			{
				std::string uuid = cstr;
				while (!uuid.empty() && std::isspace(uuid.back())) uuid.pop_back(); // discard trailing white space
				return uuid;
			}
		}
	}
	return {}; // failed, return empty string
}

void SaveSetting(const char* HeroName)
{
	LPCSTR binPath = (LPCSTR)(u8"C:\\Settings\\" + (std::string)HeroName + u8".ini").c_str();
	WritePrivateProfileString(u8"Setting", u8"AimSpeed", (LPCSTR)to_string(Config::Get().AimSpeed).c_str(), binPath);
	WritePrivateProfileString(u8"Setting", u8"Humanize", (LPCSTR)to_string(Config::Get().Humanize).c_str(), binPath);
	WritePrivateProfileString(u8"Setting", u8"AimFov", (LPCSTR)to_string(Config::Get().Fov).c_str(), binPath);
	WritePrivateProfileString(u8"Setting", u8"Prediction Level", (LPCSTR)to_string(Config::Get().BulletSpeed).c_str(), binPath);
	WritePrivateProfileString(u8"Setting", u8"Y PITCH", (LPCSTR)to_string(Config::Get().YPITCH).c_str(), binPath);
	WritePrivateProfileString(u8"Setting", u8"GENJI Distance", (LPCSTR)to_string(Config::Get().�Ÿ�).c_str(), binPath);
	WritePrivateProfileString(u8"Setting", u8"Aimbot HotKey", (LPCSTR)to_string(Config::Get().A1mH0tKey).c_str(), binPath);
}


void LoadSetting(const char* HeroName)
{
	LPCSTR binPath = (LPCSTR)(u8"C:\\Settings\\" + (std::string)HeroName + u8".ini").c_str();

	char Strs[MAX_PATH] = { NULL };
	GetPrivateProfileString(u8"Setting", u8"AimSpeed", "", Strs, MAX_PATH, binPath);
	Config::Get().AimSpeed = atof(Strs);

	*Strs = { NULL };
	GetPrivateProfileString(u8"Setting", u8"Humanize", "", Strs, MAX_PATH, binPath);
	Config::Get().Humanize = atof(Strs);

	*Strs = { NULL };
	GetPrivateProfileString(u8"Setting", u8"AimFov", "", Strs, MAX_PATH, binPath);
	Config::Get().Fov = atof(Strs);

	*Strs = { NULL };
	GetPrivateProfileString(u8"Setting", u8"Prediction Level", "", Strs, MAX_PATH, binPath);
	Config::Get().BulletSpeed = atof(Strs);

	*Strs = { NULL };
	GetPrivateProfileString(u8"Setting", u8"Y PITCH", "", Strs, MAX_PATH, binPath);
	Config::Get().YPITCH = atof(Strs);

	*Strs = { NULL };
	GetPrivateProfileString(u8"Setting", u8"GENJI Distance", "", Strs, MAX_PATH, binPath);
	Config::Get().�Ÿ� = atof(Strs);

	*Strs = { NULL };
	GetPrivateProfileString(u8"Setting", u8"Aimbot HotKey", "0x", Strs, MAX_PATH, binPath);
	Config::Get().A1mH0tKey = strtoul(Strs, NULL, 16);

	//GetPrivateProfileStringA("SIGMA", "Prediction Projectile", "", (LPSTR)sda8h7sd8gs7d8tsdlkhjgnsadhizsjifdkyfhzz, MAX_PATH, "C:\\Favian_Settings\\SIGMA.ini");
	//�䷱������ ���ϴ� ��.. ���� �ֽżҽ��� �ƴϴϱ� .. �۳� 12�� �� �ҽ��� .. �� ���� ������ �����Ͻ� �뿩 :)��! �� ������ �׳� ����ϳ�..
	//		
}
const char* SettingItemList_English[] = { u8"���", u8"�����ϸ�Ʈ", u8"��ŷ��", u8"�ε�ȣ��", u8"�ñ׸�", u8"������", u8"������", u8"�ڸ���", u8"Ʈ���̼�", u8"����", u8"����", u8"���ǽ�Ʈ", u8"�ø�Ʈ��", u8"��ũ��", u8"�Ķ�", u8"�丣���", u8"��ũ��", u8"���������Ŀ", u8"����", u8"����", u8"����", u8"����: 76", u8"�ֽ�", u8"�غ��", u8"����Ÿ", u8"�Ƴ�", u8"�ٽ�Ƽ��", u8"���̶�", u8"��Ƽ��Ʈ", u8"�޸���", u8"�긮����", u8"��ÿ�" };
static const char* CurrentSettingItem_English = u8"����";

const string currentDateTime() {
	time_t     now = time(0); //���� �ð��� time_t Ÿ������ ����
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); // YYYY-MM-DD.HH:mm:ss ������ ��Ʈ��

	return buf;
}

string CodeCheck()
{
	unsigned char s[] =
	{

		0x58, 0x4d, 0x66, 0x3f, 0x8a, 0x27
	};

	for (unsigned int m = 0; m < sizeof(s); ++m)
	{
		unsigned char c = s[m];
		c = ~c;
		c += 0x48;
		c = ~c;
		c += 0x51;
		c = (c >> 0x6) | (c << 0x2);
		c = -c;
		c = ~c;
		c ^= 0x3d;
		c += m;
		c ^= 0xed;
		c = (c >> 0x5) | (c << 0x3);
		c += m;
		c = -c;
		c = (c >> 0x7) | (c << 0x1);
		c += 0x52;
		c = ~c;
		c -= 0x5d;
		c = ~c;
		c = (c >> 0x2) | (c << 0x6);
		c = -c;
		s[m] = c;
	}
	return string((char*)s);

}


const char* Time;
void esp();

void Menu::Render() { 
	ImGui_ImplDX11_NewFrame();
	Renderer::GetInstance()->BeginScene();
	Renderer::GetInstance()->DrawScene();
	Renderer::GetInstance()->EndScene();
	if (Config::Get().menuEnabled) {
		ImGui::SetNextWindowPos(ImVec2{ 50, 30 }, ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2{ 430, 420 }, ImGuiSetCond_Once);
		bool _visible = true;
		if (ImGui::Begin(u8" Signature ", &_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) //�̸� ���� ���̸� 
		{
			ImGui::SetWindowSize(ImVec2(300, 1000), 0);  // �ӱ��� ũ������
			//if (!Config::Get().IsLogin)
			//{
			//	ImGui::InputText(u8" �ڵ��Է�", (char*)Config::Get().CODE.c_str(), 40);
			//	if (ImGui::Button(u8" �α���", ImVec2(125, 35))) // �α��� ���� 11
			//	{
			//		if ((char*)Config::Get().CODE.c_str() != "")
			//		{
			//			string strname;
			//			string strname2 = (char*)Config::Get().CODE.c_str() + CodeCheck() + get_system_uuid(); // �ƴ� ����ȯ�� �� �ȵǴµ� �� ���� ���̳빫�������� ��
			//			strname = "/evolution.php?mode=login&Code=";// �ҽ� �״�γ��� �ù߷þ� �����ϰ� ����? ���� ���ڷ� ������ 
			//			string strsum = strname + strname2;
			//			string CodeVal = Post_Query("hong333.dothome.co.kr/", strsum.c_str()); // ��Ȩ ���� 2 php
			//			// �� �ȵǴµ� �� ���� �빫�������� �װ� �� ��
			//			string decrypted = encryptDecrypt(CodeVal);
			//			{
			//			string decoded = b64decode(decrypted.c_str(), decrypted.size());
			//			if (CodeVal.find(u8"SpaceMain") != string::npos)
			//				Config::Get().A1mH0tKey = 0x01;
			//				Config::Get().IsLogin = true;
			//			}
			//		}    // �α��� ���� 11 
			//	}
			//}
			if (!Config::Get().IsLogin)
			{
				string codeStr = (char*)Config::Get().CODE.c_str();
				ImGui::InputText(u8" �ڵ��Է�", (char*)Config::Get().CODE.c_str(), 40);
				if (ImGui::Button(u8" �α���", ImVec2(125, 35))) // �α��� ���� 11
				{
					if ((char*)Config::Get().CODE.c_str() != "")
					{
						string CodeVal = WebWinhttp("evolution.php?mode=login&Code=" + codeStr + "&COM=" + get_system_uuid()); // ��Ȩ ���� 2 php
						string decrypted = encryptDecrypt(CodeVal);
						string decoded = b64decode(decrypted.c_str(), decrypted.size());
						if (CodeVal.find(u8"SpaceMain") != string::npos)
						{
							Config::Get().A1mH0tKey = 0x01;
							Config::Get().IsLogin = true;
						}
					}    // �α��� ���� 11 
				}
			}

			
			else
			{
				if (ImGui::CollapsingHeader(u8"ESP"))  // �ӱ��� üũ�ڽ� �� ī�װ� ���� ���� 
				{
					ImGui::Checkbox((u8" è�Ǿ� ESP"), &Config::Get().DrawHeroName); //è�Ǿ� esp
					ImGui::Checkbox((u8" �׵θ� ESP"), &Config::Get().UseBox2SP);
					ImGui::Checkbox((u8" 2D�ڽ� ESP"), &Config::Get().Draw2DBox); //�ڽ� esp
					ImGui::Checkbox((u8" ��Ʈ�ڽ� ESP"), &Config::Get().DrawEdge); //���� esp
					ImGui::Checkbox((u8" �Ÿ� ESP"), &Config::Get().DrawDist); //�Ÿ� esp
				}
				if (ImGui::CollapsingHeader(u8"���Ӻ�"))
				{
					ImGui::Checkbox((u8" ����"), &Config::Get().DrawFov); //���� esp
					ImGui::Checkbox((u8" Ʈ��ŷ"), &Config::Get().UseA1mb0t);
					ImGui::Checkbox((u8" �ø�"), &Config::Get().RFLICK);
					ImGui::Checkbox((u8" ����"), &Config::Get().Predict);
					ImGui::Checkbox((u8" Y�� ����"), &Config::Get().UseYAxis);
					ImGui::Checkbox((u8" Ʈ���ź�"), &Config::Get().TriggerBot);
					ImGui::Checkbox((u8" �ڵ�����"), &Config::Get().�ڵ���Ÿ);
				}

				if (ImGui::CollapsingHeader(u8"����"))
				{
					ImGui::SliderFloat((u8""), &Config::Get().AimSpeed, 1.0f, 0.00000f, "%.3f Speed");
					ImGui::SliderFloat((u8""), &Config::Get().Fov, 180.f, 0.0f, "%.0f FOV"); 
					ImGui::SliderFloat((u8""), &Config::Get().TriggerFOV, 10.f, 0.0f, "%.0f FOV");
					ImGui::SliderFloat((u8""), &Config::Get().YPITCH, 1.0, 0.00000f, "%.2f Y PITCH");
				}

				//if (ImGui::CollapsingHeader(u8"Ŀ����"))  // �ӱ��� üũ�ڽ� �� ī�װ� ���� ���� 
				//{
				//	ImGui::ColorEdit3((u8"���� ����"), (float*)&Config::Get().ESPColor11);
				//	//ImGui::ColorEdit3((u8"�ڽ� ����"), (float*)&Config::Get().ESPColor);
				//	//ImGui::ColorEdit3((u8"�Ÿ� ����"), (float*)&Config::Get().ESPColor2);
				//	//ImGui::ColorEdit3((u8"è�Ǿ� ����"), (float*)&Config::Get().ESPColor3);
				//	//ImGui::ColorEdit3((u8"�׵θ� ����"), (float*)&Config::Get().ESPColor5);

				//	//ImGui::ColorEdit3((u8"���� �ڽ� ����"), (float*)&Config::Get().ESPColor6);
				//	//ImGui::ColorEdit3((u8"���� �Ÿ� ����"), (float*)&Config::Get().ESPColor7);
				//	//ImGui::ColorEdit3((u8"���� è�Ǿ� ����"), (float*)&Config::Get().ESPColor8);
				//	//ImGui::ColorEdit3((u8"���� �׵θ� ����"), (float*)&Config::Get().ESPColor10);
				//}

				//if (ImGui::CollapsingHeader(u8"����"))
				//{
				//	if (ImGui::BeginCombo(u8"##setting", CurrentSettingItem_English))
				//	{
				//		for (auto i = 0; i < IM_ARRAYSIZE(SettingItemList_English); i++)
				//		{
				//			bool is_selected = (CurrentSettingItem_English == SettingItemList_English[i]);
				//			if (ImGui::Selectable(SettingItemList_English[i], is_selected))
				//			{
				//				CurrentSettingItem_English = SettingItemList_English[i];
				//			}
				//		}
				//		ImGui::EndCombo();
				//	}

				//	if (ImGui::Button(u8"", ImVec2(80, 29)))
				//	{
				//		SaveSetting(CurrentSettingItem_English);
				//	}
				//	ImGui::SameLine();
				//	if (ImGui::Button(u8"�ҷ�����", ImVec2(80, 29)))
				//	{
				//		LoadSetting(CurrentSettingItem_English);
				//	}
				//}
			}
			ImGui::End();
		}
	}
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.16f, 0.15f, 0.17f, 0.00f));
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2{ 1920, 1080 }, ImGuiSetCond_Once);
		bool _visible = true;
		if (ImGui::Begin(u8"##Fov", &_visible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs))
		{
			ImGui::PushFont(Config::Get().pFont);

			ImGuiWindow* window = ImGui::GetCurrentWindow();
			float pxR = tanf(DEG2RAD(Config::Get().Fov) / 2.0f) / tanf(DEG2RAD(103.0f) / 2.0f) * 1920;
			if(Config::Get().DrawFov)
			window->DrawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), pxR, ImGui::GetColorU32(ImVec4(Config::Get().ESPColor11.x * 255, Config::Get().ESPColor11.y * 255, Config::Get().ESPColor11.z * 255, Config::Get().ESPColor11.w * 255)), 100, 0.5f);	 // ���������

			float pxR_ = tanf(DEG2RAD(Config::Get().TriggerFOV) / 2.0f) / tanf(DEG2RAD(103.0f) / 2.0f) * 1920;
			if (Config::Get().DrawFov && Config::Get().TriggerBot)
				window->DrawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), pxR_, ImGui::GetColorU32(ImVec4(Config::Get().ESPColor11.x * 255, Config::Get().ESPColor11.y * 255, Config::Get().ESPColor11.z * 255, Config::Get().ESPColor11.w * 255)), 100, 0.5f);	 // ���������

			esp();
		}
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.16f, 0.15f, 0.17f, 1.00f));
		ImGui::End();

	ImGui::Render();
}


void Menu::Shutdown() {
	ImGui_ImplDX11_Shutdown();
}

void Menu::CreateStyle()
{

}
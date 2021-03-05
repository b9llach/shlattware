#include <iostream>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <stdio.h>
#include <string>
#include <fcntl.h> 
#include "Tables.h"
#include <io.h>
#include <fcntl.h> 
#include <cstdio>
#include <conio.h>

float sensitivity = .35;
int fov = 90;
int currentwep = 0;
int scope = 0;
int barrel = 0;
std::string weapon = "";
int randomizer = 1;
bool enabled = false;

using namespace std;

float Randomize(float val, int perc)
{
	srand(time(NULL));
	float range = val * perc / 100;

	if (range <= 0.5) return val;
	if (range > 0.5) range = 1;

	int result = 1 + (rand() % (int)range);

	if ((1 + (rand() % 1) > 0)) return val + result;
	else return val + (result * -1);
}

void QuerySleep(int ms) // Sleep / Delay
{
	LONGLONG timerResolution;
	LONGLONG wantedTime;
	LONGLONG currentTime;

	QueryPerformanceFrequency((LARGE_INTEGER*)&timerResolution);
	timerResolution /= 1000;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	wantedTime = currentTime / timerResolution + ms;
	currentTime = 0;
	while (currentTime < wantedTime)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		currentTime /= timerResolution;
	}
}


void Smoothing(double delay, double control_time, float x, float y)
{
	int x_ = 0, y_ = 0, t_ = 0;

	for (int i = 1; i <= (int)control_time; ++i)
	{
		int xI = i * x / (int)control_time;
		int yI = i * y / (int)control_time;
		int tI = i * (int)control_time / (int)control_time;
		int xX = (int)xI - (int)x_;
		int yY = (int)yI - (int)y_;
		mouse_event(1, xX, yY, 0, 0);
		QuerySleep((int)tI - (int)t_);
		x_ = xI; y_ = yI; t_ = tI;
	}
	QuerySleep((int)delay - (int)control_time);

}

float getScope(float val)
{
	if (scope == 1)
		return val * 1.18605;
	if (scope == 2)
		return val * 3.83721;
	if (scope == 3)
		return val * 7.65116;
	if (scope == 4)
		return val * .8;
	return val;
}

float tofovandsens(float sens, int fov, float val)
{
	float a = (0.5 * fov * val) / (sens * 90);

	return getScope(a);

}

string isEnabled(bool value) {
	switch (value) {
	case true:
		return "ON";
	case false:
		return "OFF";
	}
}

string scopeAsString(int s) {
	switch (s) {
	case 1:
		return "HOLO";
	case 2:
		return "8X";
	case 3:
		return "16X";
	case 4:
		return "HANDMADE";
	case 0:
		return "NO SCOPE";
	}
}

string isCrosshairEnabled(bool c) {
	if (c == true) {
		return "ON";

		if (c == false)
			return "OFF";
	}
}

string barrelAsString(int b) {
	switch (b) {
	case 1:
		return "SURPRESSOR";
	default:
		return "NO BARREL";
	}
}

string GetWeapon(int num) {
	switch (num) {
	case 0:
		weapon = "AK-47";
		return weapon;
	case 1:
		weapon = "Tommy";
		return weapon;
	case 2:
		weapon = "Custom";
		return weapon;
	case 3:
		weapon = "LR-300";
		return weapon;
	case 4:
		weapon = "MP5A4";
		return weapon;
	case 5:
		weapon = "S.A.R";
		return weapon;
	case 6:
		weapon = "M249";
		return weapon;
	}
}

void DrawGui() {

	using namespace std;
	system("cls");
	system("color d");

	_setmode(_fileno(stdout), _O_TEXT);


	cout << "\n\n" << endl;

	cout << "		        " << "Enabled (INS): " << endl;
	cout << "		             " << isEnabled(enabled) << endl;

	//cout << "		        " << "Smoothing (RCTL): " << endl;
	//cout << "		             " << randomizer << endl;

	//cout << "		       " << "Current Weapon: " << endl;
	//cout << "		            " << GetWeapon(currentwep) << endl;

	cout << "\n\n" << endl;

	cout << "	   " << "AK-47 - F2" << "		     " << "LR-300 - F5\n" << endl;
	cout << "	   " << "TOMMY - F3" << "		     " << "MP5 - F6\n" << endl;
	cout << "	   " << "CUSTOM - F4" << "		     " << "SAR - F7\n" << endl;
	cout << "	   " << "SILENCER - DOWN" << "	     " << "M249 - F8\n" << endl;
	cout << "	   " << "HOLO - UP" << "		     " << "HANDMADE - LEFT\n" << endl;
	cout << "	   " << "8x - PAGE UP" << "		     " << "NO BARREL - RIGHT\n" << endl;
	cout << "	   " << "16x - F11" << "		     " << "NO SCOPE - F10\n" << endl;
	cout << "\n\n	      " << GetWeapon(currentwep) << "  ||  " << scopeAsString(scope) << "  ||  " << barrelAsString(barrel) << endl;
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << "	                " << "billy.#4671" << endl;


}

int main()
{

	int count = 0;

	SetConsoleTitle(L"$hlattware");

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	COORD new_size =
	{
		(info.srWindow.Right - info.srWindow.Left + 1),
		(info.srWindow.Bottom - info.srWindow.Top + 1)
	};
	SetConsoleScreenBufferSize(handle, new_size);

	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) { MoveWindow(hwnd, 1280, 720, 540, 780, TRUE); }
	SetLayeredWindowAttributes(hwnd, 1000, 1000, LWA_ALPHA);
	SetConsoleScreenBufferSize(handle, new_size);

	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);


	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	system("color d");
	_setmode(_fileno(stdout), 0x20000);

	DrawGui();

	while (true) {

		/*cout << "going";

		if (showch == true) {
			crosshair();
			DrawGui();
			continue;
		}*/

		if (GetKeyState(VK_F2) & 0x8000)
		{
			if (currentwep != 0)
			{
				currentwep = 0;
				DrawGui();
			}
		}
		if (GetKeyState(VK_F3) & 0x8000)
		{
			if (currentwep != 1)
			{
				currentwep = 1;
				DrawGui();
			}
		}
		if (GetKeyState(VK_F4) & 0x8000)
		{
			if (currentwep != 2)
			{
				currentwep = 2;
				DrawGui();
			}
		}
		if (GetKeyState(VK_F5) & 0x8000)
		{
			if (currentwep != 3)
			{
				currentwep = 3;
				DrawGui();
			}
		}
		if (GetKeyState(VK_F6) & 0x8000)
		{
			if (currentwep != 4)
			{
				currentwep = 4;
				DrawGui();
			}
		}
		if (GetKeyState(VK_F7) & 0x8000)
		{
			if (currentwep != 5)
			{
				currentwep = 5;
				DrawGui();
			}
		}
		if (GetKeyState(VK_F8) & 0x8000)
		{
			if (currentwep != 6)
			{
				currentwep = 6;
				DrawGui();
			}
		}
		if (GetAsyncKeyState(VK_INSERT) == -32767)
		{
			enabled = !enabled;
			DrawGui();
		}
		if (GetAsyncKeyState(VK_F10) == -32767)
		{
			scope = 0;
			DrawGui();
		}
		if (GetAsyncKeyState(VK_UP) == -32767)
		{
			scope = 1;
			DrawGui();
		}
		if (GetAsyncKeyState(VK_PRIOR) == -32767) {
			scope = 2;
			DrawGui();
		}
		if (GetAsyncKeyState(VK_F11) == -32767) {
			scope = 3;
			DrawGui();
		}
		if (GetAsyncKeyState(VK_LEFT) == -32767) {
			scope = 4;
			DrawGui();
		}
		if (GetAsyncKeyState(VK_DOWN) == -32767)
		{
			barrel = 1;
			DrawGui();
		}
		if (GetAsyncKeyState(VK_RIGHT) == -32767)
		{
			barrel = 0;
			DrawGui();
		}
		if (enabled == true)
		{
			if (GetAsyncKeyState(VK_LBUTTON) && GetAsyncKeyState(VK_RBUTTON))
			{

				switch (currentwep)
				{
				case 0:
					if (count < Weapons::ak::pattern.size())
					{
						Smoothing(Weapons::ak::delay, Weapons::ak::controltime.at(count), Randomize(tofovandsens(sensitivity, fov, Weapons::ak::pattern.at(count).x), randomizer), Randomize(tofovandsens(sensitivity, fov, Weapons::ak::pattern.at(count).y), randomizer));
						count++;
					}
					break;
				case 1:
					if (count < Weapons::thompson::pattern.size())
					{
						Smoothing(Weapons::thompson::delay, Weapons::thompson::delay, Randomize(tofovandsens(sensitivity, fov, Weapons::thompson::pattern.at(count).x), randomizer), Randomize(tofovandsens(sensitivity, fov, Weapons::thompson::pattern.at(count).y), randomizer));
						count++;
					}
					break;
				case 2:
					if (count < Weapons::smg::pattern.size())
					{
						Smoothing(Weapons::smg::delay, Weapons::smg::delay, Randomize(tofovandsens(sensitivity, fov, Weapons::smg::pattern.at(count).x), randomizer), Randomize(tofovandsens(sensitivity, fov, Weapons::smg::pattern.at(count).y), randomizer));
						count++;
					}
					break;
				case 3:
					if (count < Weapons::lr::pattern.size())
					{
						Smoothing(Weapons::lr::delay, Weapons::lr::delay, Randomize(tofovandsens(sensitivity, fov, Weapons::lr::pattern.at(count).x), randomizer), Randomize(tofovandsens(sensitivity, fov, Weapons::lr::pattern.at(count).y), randomizer));
						count++;
					}
					break;
				case 4:
					if (count < Weapons::mp5::pattern.size())
					{
						Smoothing(Weapons::mp5::delay, Weapons::mp5::delay, Randomize(tofovandsens(sensitivity, fov, Weapons::mp5::pattern.at(count).x), randomizer), Randomize(tofovandsens(sensitivity, fov, Weapons::mp5::pattern.at(count).y), randomizer));
						count++;
					}
					break;
				case 5:
					if (count < Weapons::semi::pattern.size())
					{
						Smoothing(Weapons::semi::delay, Weapons::semi::delay, Randomize(tofovandsens(sensitivity, fov, Weapons::semi::pattern.at(count).x), randomizer), Randomize(tofovandsens(sensitivity, fov, Weapons::semi::pattern.at(count).y), randomizer));
					}
					break;
				case 6:
					Smoothing(Weapons::m249::delay, Weapons::m249::delay, Randomize(tofovandsens(sensitivity, fov, Weapons::m249::pattern.at(count).x), randomizer), Randomize(tofovandsens(sensitivity, fov, Weapons::m249::pattern.at(count).y), randomizer));
					break;
				default:
					break;
				}

			}
			else
				count = 0;
		}

	}
}


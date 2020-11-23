#pragma once
#pragma once
#include <math.h>
#include <Windows.h>

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3() { x = 0; y = 0; z = 0; return; }

	Vector3(float X, float Y, float Z) {
		x = X;
		y = Y;
		z = Z;
	}

	float Distance(Vector3 v)
	{
		return sqrtf(powf(v.x - x, 2.0f) + powf(v.y - y, 2.0f) + powf(v.z - z, 2.0f));
	}

	inline Vector3 operator+(Vector3 v)
	{
		return { x + v.x, y + v.y, z + v.z };
	}

	inline Vector3 operator-(Vector3 v)
	{
		return { x - v.x, y - v.y, z - v.z };
	}
};

class Vector2 {
public:
	float x, y;

	Vector2() { x = 0; y = 0; }

	Vector2(float X, float Y)
	{
		x = X;
		y = Y;
	}

	inline float Distance(Vector2 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0)));
	}
};


struct Matrix {
	float m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44;

	bool WorldToScreen(Vector3 worldPos, int width, int height, Vector2& OutPos) {
		float screenX = (m11 * worldPos.x) + (m21 * worldPos.y) + (m31 * worldPos.z) + m41;
		float screenY = (m12 * worldPos.x) + (m22 * worldPos.y) + (m32 * worldPos.z) + m42;
		float screenW = (m14 * worldPos.x) + (m24 * worldPos.y) + (m34 * worldPos.z) + m44;

		float camX = width / 2.0f;
		float camY = height / 2.0f;

		float x = camX + (camX * screenX / screenW);
		float y = camY - (camY * screenY / screenW);

		OutPos = { x, y };

		return (screenW > 0.001f);
	}

	bool WorldToScreen(Vector3 worldPos, int width, int height, ImVec2& OutPos) {
		float screenX = (m11 * worldPos.x) + (m21 * worldPos.y) + (m31 * worldPos.z) + m41;
		float screenY = (m12 * worldPos.x) + (m22 * worldPos.y) + (m32 * worldPos.z) + m42;
		float screenW = (m14 * worldPos.x) + (m24 * worldPos.y) + (m34 * worldPos.z) + m44;

		float camX = width / 2.0f;
		float camY = height / 2.0f;

		float x = camX + (camX * screenX / screenW);
		float y = camY - (camY * screenY / screenW);

		OutPos = ImVec2(x, y);

		return (screenW > 0.001f);
	}
};
#include "windows.h"
#include <corecrt_math.h>
struct Color {
	int R, G, B, A;
};
Color HSVToRGB(double H, double V, double S);

unsigned long RGBA2ARGB(int r, int g, int b, int a)
{
	return ((a & 0xFF) << 24) + ((b & 0xFF) << 16) + ((g & 0xFF) << 8) + (r & 0xFF);
}


Color HSVToRGB(double H, double V, double S)
{
	double r = 0, g = 0, b = 0;
	int i;
	double f, p, q, t;
	if (H == 360)
		H = 0;
	else
		H = H / 60;
	i = (int)trunc(H);
	f = H - i;
	p = V * (1.0 - S);
	q = V * (1.0 - (S * f));
	t = V * (1.0 - (S * (1.0 - f)));
	switch (i)
	{
	case 0:
		r = V;
		g = t;
		b = p;
		break;

	case 1:
		r = q;
		g = V;
		b = p;
		break;

	case 2:
		r = p;
		g = V;
		b = t;
		break;

	case 3:
		r = p;
		g = q;
		b = V;
		break;

	case 4:
		r = t;
		g = p;
		b = V;
		break;

	default:
		r = V;
		g = p;
		b = q;
		break;
	}

	return { (byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)255 };
}
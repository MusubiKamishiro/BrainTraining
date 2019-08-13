#pragma once
#include "Geometry.h"

class Peripheral;

class Button
{
private:
	Rect rect;
	bool flag;

public:
	Button(Rect rect);
	~Button();

	bool Update(const Peripheral& p);
	void Draw();
};


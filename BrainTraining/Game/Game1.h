#pragma once
#include "../Scene/Scene.h"
#include <memory>
#include <vector>

class Button;

///��o������񂯂�
///LaterRPS
class Game1 :
	public Scene
{
private:
	void (Game1::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	int rock, paper, scissors;	// �O�[���傫�ρ[�̉摜�n���h��

	int count = 0;
	int n = 0;

	std::vector<std::shared_ptr<Button>> buttons;

public:
	Game1();
	~Game1();

	void Update(const Peripheral& p);
	void Draw();
};


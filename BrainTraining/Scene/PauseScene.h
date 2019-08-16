#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include <map>

class Button;


class PauseScene : public Scene
{
private:
	void (PauseScene::*updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p)override final;
	void FadeoutUpdate(const Peripheral& p)override final;
	void WaitUpdate(const Peripheral& p)override final;

	std::vector<std::shared_ptr<Button>> buttons;
	std::map<int, std::string> buttonStatements;	// ê‡ñæï∂

	int select;

public:
	PauseScene();
	~PauseScene();

	void Update(const Peripheral& p)override final;
	void Draw()override final;
};


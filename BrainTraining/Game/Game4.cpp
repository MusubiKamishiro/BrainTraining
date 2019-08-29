#include "Game4.h"

#include <DxLib.h>
#include <random>
#include <string>

#include "../Peripheral.h"
#include "../Button.h"
#include "../Game.h"

#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/PauseScene.h"

#include "../System/FileSystem.h"
#include "../System/ImageLoader.h"
#include "../System/SoundLoader.h"

constexpr int qMax = 20;	// ���̍ő吔
constexpr int maxQNun = 100;	// �ő�l

void Game4::FadeinUpdate(const Peripheral & p)
{
	if (pal > 255)
	{
		if (p.IsTrigger(MOUSE_INPUT_LEFT))
		{
			pal = 255;
			updater = &Game4::DescriptionUpdate;
			drawer = &Game4::DescriptionDraw;
		}
	}
	else
	{
		pal += 20;
	}
}

void Game4::FadeoutUpdate(const Peripheral & p)
{
	if (pal <= 0)
	{
		SceneManager::Instance().ChangeScene(std::make_unique<ResultScene>(qMax, trueNum));
	}
	else
	{
		pal -= 20;
	}
}

void Game4::WaitUpdate(const Peripheral & p)
{
	if (waitTime <= 0 && !CheckSoundMem(_startSE))
	{
		updater = &Game4::QuestionDisplayUpdate;
	}

	--waitTime;
	if (!(waitTime % 60) && waitTime > 0)
	{
		if (waitTime / 60 == 1)
		{
			PlaySoundMem(_startSE, DX_PLAYTYPE_BACK);
		}
		else
		{
			PlaySoundMem(_cntDownSE, DX_PLAYTYPE_BACK);
		}
	}
}

void Game4::DescriptionUpdate(const Peripheral & p)
{
	if (p.IsTrigger(MOUSE_INPUT_LEFT))
	{
		PlaySoundMem(_cntDownSE, DX_PLAYTYPE_BACK);
		updater = &Game4::WaitUpdate;
		drawer = &Game4::GameDraw;
	}
	++count;
}

void Game4::QuestionDisplayUpdate(const Peripheral & p)
{
	if (!CheckSoundMem(_gameBGM))
	{
		PlaySoundMem(_gameBGM, DX_PLAYTYPE_BACK);
	}
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i]->Update(p))
		{
			PlaySoundMem(_btnSE, DX_PLAYTYPE_BACK);
			EditMyAnswer(i);
		}
	}
	if (decide->Update(p))
	{
		updater = &Game4::AnswerCheckUpdate;
	}
	if (del->Update(p))
	{
		PlaySoundMem(_btnSE, DX_PLAYTYPE_BACK);
		myAnswer = 0;
	}

	if (p.IsTrigger(MOUSE_INPUT_RIGHT))
	{
		SceneManager::Instance().PushScene(std::make_unique<PauseScene>());
	}
}

void Game4::AnswerCheckUpdate(const Peripheral & p)
{
	// ���̉񓚂��m�F
	if (op == static_cast<int>(Operator::PLUS))
	{
		qAnswer = firstNum + secondNum;
	}
	else if (op == static_cast<int>(Operator::MINUS))
	{
		qAnswer = firstNum - secondNum;
	}
	else if (op == static_cast<int>(Operator::MULTI))
	{
		qAnswer = firstNum * secondNum;
	}

	if (qAnswer == myAnswer)
	{
		result = true;
	}
	else
	{
		result = false;
	}

	if (result)
	{
		++trueNum;
		DxLib::PlaySoundMem(trueSE, DX_PLAYTYPE_BACK, true);
	}
	else
	{
		DxLib::PlaySoundMem(falseSE, DX_PLAYTYPE_BACK, true);
	}

	updater = &Game4::AnswerDisplayUpdate;
}

void Game4::AnswerDisplayUpdate(const Peripheral & p)
{
	if (displayCount <= 0)
	{
		if (nowQNum >= qMax)
		{
			updater = &Game4::FadeoutUpdate;
		}
		else
		{
			CreateQuestion();
			++nowQNum;
			displayCount = 30;

			updater = &Game4::QuestionDisplayUpdate;
		}
	}
	--displayCount;
}

void Game4::TitleDraw()
{
	auto size = Game::Instance().GetScreenSize();

	int strwidth, strheight;
	strwidth = strheight = 0;

	SetFontSize(250);
	std::string s = "�Ђ炪�Ȍv�Z";
	GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
	DrawString(size.x / 2 - strwidth / 2, size.y / 3 - strheight / 2, s.c_str(), 0x000000);

	SetFontSize(100);
	GetDrawFormatStringSize(&strwidth, &strheight, nullptr, "�S%d��", qMax);
	DrawFormatString(size.x / 2 - strwidth / 2, size.y / 3 * 2 - strheight / 2, 0x000000, "�S%d��", qMax);
}

void Game4::DescriptionDraw()
{
	auto size = Game::Instance().GetScreenSize();

	DxLib::SetFontSize(96);
	DxLib::DrawFormatString(100, 350, 0x000000, "���� ����\n�@�@���� �́H");
	DxLib::DrawFormatString(500, 800, 0x000000, "�����̉�:%d", 2);
	DxLib::DrawLine(100, 295, 100, 620, 0xff0000, 10);
	DxLib::DrawLine(770, 295, 770, 620, 0xff0000, 10);
	DxLib::DrawLine(100, 295, 770, 295, 0xff0000, 10);
	DxLib::DrawLine(100, 620, 770, 620, 0xff0000, 10);
	
	
	int strwidth, strheight;
	strwidth = strheight = 0;
	// �e��{�^���̕`��
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		buttons[i]->Draw();
		auto rect = buttons[i]->GetButtonRect();
		std::string s = std::to_string(i);
		GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
		DxLib::DrawFormatString(rect.center.x - strwidth / 2, rect.center.y - strheight / 2, 0x000000, "%s", s.c_str());
	}
	decide->Draw();
	auto rect = decide->GetButtonRect();
	std::string s = "�񓚂���";
	GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
	DxLib::DrawFormatString(rect.center.x - strwidth / 2, rect.center.y - strheight / 2, 0x000000, "%s", s.c_str());
	del->Draw();
	rect = del->GetButtonRect();
	s = "���";
	GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
	DxLib::DrawFormatString(rect.center.x - strwidth / 2, rect.center.y - strheight / 2, 0x000000, "%s", s.c_str());


	DxLib::SetFontSize(80);
	DxLib::DrawString(20, 20, "<���>", 0x000000);

	DxLib::DrawString(270, 20, "�ȒP�Ȍv�Z��\n�Ђ炪�Ȃŕ\��������\n�E�̃{�^���œ�������͂���\n�@�@�@�@�@�@�@�񓚂����\n�@�@�@�@�@�@�@�N���b�N�I", 0x000000);

	if ((count / 40) % 2 == 0)
	{
		int strWidth, strHeight;
		GetDrawStringSize(&strWidth, &strHeight, nullptr, "���N���b�N�ŃX�^�[�g!", strlen("���N���b�N�ŃX�^�[�g!"));
		DrawString(size.x / 2 - strWidth / 2 - 100, size.y - strHeight - 50, "���N���b�N�ŃX�^�[�g!", 0x0000ff);
	}
}

void Game4::GameDraw()
{
	if (updater != &Game4::WaitUpdate)
	{
		DxLib::SetFontSize(96);
		DxLib::DrawFormatString(100, 100, 0x000000, "��%d��", nowQNum);
		DxLib::DrawFormatString(100, 350, 0x000000, "%s", question.c_str());

		DxLib::DrawFormatString(500, 800, 0x000000, "�����̉�:%d", myAnswer);
	}
	else
	{
		DxLib::SetFontSize(96 * 2);
		auto size = Game::Instance().GetScreenSize();
		int strWidth, strHeight;
		if (waitTime <= 60)
		{
			SetFontSize(130);
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "�X�^�[�g!", strlen("�X�^�[�g!"));
			DrawString(size.x / 2 - strWidth / 2, size.y / 10 - strHeight / 2, "�X�^�[�g�I", 0x000000);
		}
		else
		{
			GetDrawStringSize(&strWidth, &strHeight, nullptr, "0", strlen("0"));
			DrawFormatString(size.x / 2 - strWidth / 2, size.y / 10 - strHeight / 2, 0x000000, "%d", (waitTime / 60));
		}
	}

	int strwidth, strheight;
	strwidth = strheight = 0;
	DxLib::SetFontSize(96);
	// �e��{�^���̕`��
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		buttons[i]->Draw();
		auto rect = buttons[i]->GetButtonRect();
		std::string s = std::to_string(i);
		GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
		DxLib::DrawFormatString(rect.center.x - strwidth / 2, rect.center.y - strheight / 2, 0x000000, "%s", s.c_str());
	}
	decide->Draw();
	auto rect = decide->GetButtonRect();
	std::string s = "�񓚂���";
	GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
	DxLib::DrawFormatString(rect.center.x - strwidth / 2, rect.center.y - strheight / 2, 0x000000, "%s", s.c_str());

	del->Draw();
	rect = del->GetButtonRect();
	s = "���";
	GetDrawStringSize(&strwidth, &strheight, nullptr, s.c_str(), strlen(s.c_str()));
	DxLib::DrawFormatString(rect.center.x - strwidth / 2, rect.center.y - strheight / 2, 0x000000, "%s", s.c_str());

	if (updater == &Game4::AnswerDisplayUpdate)
	{
		if (result)
		{
			auto size = Game::Instance().GetScreenSize();
			Vector2 imgSize;
			GetGraphSize(_correctImg, &imgSize.x, &imgSize.y);
			DrawGraph(size.x / 2 - imgSize.x / 2, size.y / 3 * 2 - imgSize.y / 2, _correctImg, true);
		}
		else
		{
			auto size = Game::Instance().GetScreenSize();
			Vector2 imgSize;
			GetGraphSize(_missImg, &imgSize.x, &imgSize.y);
			DrawRotaGraph(size.x - imgSize.x / 2, size.y - imgSize.y / 4, 0.4, 0, _missImg, true);
		}
	}
}

void Game4::EditMyAnswer(unsigned int num)
{
	myAnswer *= 10;
	
	if (myAnswer > maxQNun * maxQNun)
	{
		myAnswer = 0;
	}

	myAnswer += num;
}

int Game4::RandomNum(int parameter)
{
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());

	int num = engine() % parameter;

	return num;
}

std::string Game4::CreateHiraganaNum(int num)
{
	std::string s = "";

	// 1����2����
	if (num / 10 == 0)
	{
		s = hiraganaNum[num];
	}
	else
	{
		if (num / 10 != 1)
		{
			s = hiraganaNum[num / 10];
		}
		s += hiraganaNum[10];

		if (num % 10 != 0)
		{
			s += hiraganaNum[num % 10];
		}
	}

	return s;
}

void Game4::CreateQuestion()
{
	// ��蕶�Ǝ����̉񓚂�������
	myAnswer = 0;
	question = "";

	// �܂��A���Z�ɂȂ邩�����߂�
	SelectOperator();

	// 1�߂̐��l������
	if (nowQNum < qMax / 4)
	{
		SelectNum(maxQNun / 10, firstNum);
	}
	else
	{
		SelectNum(maxQNun, firstNum);
	}
	
	// ��蕶�͂ɉ��Z�q�̒ǉ�
	question += questionOperators[op];
	question += "\n�@�@";

	// ���Z�q���݂āA2�߂̐��l������
	if (op == static_cast<int>(Operator::PLUS))
	{
		SelectNum(maxQNun, secondNum);
	}
	else if (op == static_cast<int>(Operator::MINUS))
	{
		SelectNum(firstNum, secondNum);
	}
	else if (op == static_cast<int>(Operator::MULTI))
	{
		SelectNum(maxQNun/10, secondNum);
	}
	
	question += "�́H";
}

void Game4::SelectNum(int num, int& qnum)
{
	if (num != 0)
	{
		qnum = RandomNum(num);
	}
	else
	{
		qnum = 0;
	}
	
	std::string s = CreateHiraganaNum(qnum);

	question += s;
	question += " ";
}

void Game4::SelectOperator()
{
	if (nowQNum > qMax / 2)
	{
		op = RandomNum(static_cast<int>(Operator::MAX));
	}
	else
	{
		op = RandomNum(static_cast<int>(Operator::MULTI));
	}
}

Game4::Game4()
{
	updater = &Game4::FadeinUpdate;
	drawer = &Game4::TitleDraw;

	questionOperators[static_cast<int>(Operator::PLUS)] = "����";
	questionOperators[static_cast<int>(Operator::MINUS)] = "�Ђ�";
	questionOperators[static_cast<int>(Operator::MULTI)] = "����";

	hiraganaNum[0] = "����";
	hiraganaNum[1] = "����";
	hiraganaNum[2] = "��";
	hiraganaNum[3] = "����";
	hiraganaNum[4] = "���";
	hiraganaNum[5] = "��";
	hiraganaNum[6] = "�낭";
	hiraganaNum[7] = "�Ȃ�";
	hiraganaNum[8] = "�͂�";
	hiraganaNum[9] = "���イ";
	hiraganaNum[10] = "���イ";

	firstNum = secondNum = 0;
	qAnswer = myAnswer = 0;
	trueNum = 0;

	waitTime = 240;
	count = 0;

	nowQNum = 1;
	displayCount = 30;
	result = false;

	SoundData sData;
	Game::Instance().GetFileSystem()->Load("SE/correct1.mp3", sData);
	trueSE = sData.GetHandle();
	Game::Instance().GetFileSystem()->Load("SE/incorrect1.mp3", sData);
	falseSE = sData.GetHandle();
	Game::Instance().GetFileSystem()->Load("SE/countDown.mp3", sData);
	_cntDownSE = sData.GetHandle();
	Game::Instance().GetFileSystem()->Load("SE/start.mp3", sData);
	_startSE = sData.GetHandle();
	Game::Instance().GetFileSystem()->Load("SE/button.mp3", sData);
	_btnSE = sData.GetHandle();
	Game::Instance().GetFileSystem()->Load("BGM/game.mp3", sData);
	_gameBGM = sData.GetHandle();

	ImageData data;
	Game::Instance().GetFileSystem()->Load("img/Button/light green.png", data);
	buttonImg.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/Button/blue.png", data);
	buttonImg.emplace_back(data.GetHandle());
	Game::Instance().GetFileSystem()->Load("img/Button/red.png", data);
	buttonImg.emplace_back(data.GetHandle());

	Game::Instance().GetFileSystem()->Load("img/Game2/maru.png", data);
	_correctImg = data.GetHandle();
	Game::Instance().GetFileSystem()->Load("img/Game2/batu.png", data);
	_missImg = data.GetHandle();

	// �e���L�[
	Vector2 buttonSize = Vector2(200, 200);
	buttons.emplace_back(new Button(Rect(1400, 720, buttonSize.x, buttonSize.y), buttonImg[0]));
	for (int i = 0; i < 9; ++i)
	{
		buttons.emplace_back(new Button(Rect(1400 + i % 3 * buttonSize.x, 520 - i / 3 * buttonSize.y, buttonSize.x, buttonSize.y), buttonImg[0]));
	}
	// ����,�������{�^��
	decide.reset(new Button(Rect(1600, 950, buttonSize.x * 3, buttonSize.y), buttonImg[2]));
	del.reset(new Button(Rect(1700, 720, buttonSize.x * 2, buttonSize.y), buttonImg[1]));

	CreateQuestion();
}


Game4::~Game4()
{
	StopSoundMem(_gameBGM);
}

void Game4::Update(const Peripheral & p)
{
	(this->*updater)(p);
}

void Game4::Draw()
{
	auto size = Game::Instance().GetScreenSize();
	DxLib::DrawBox(0, 0, size.x, size.y, 0xffffff, true);
	(this->*drawer)();
}

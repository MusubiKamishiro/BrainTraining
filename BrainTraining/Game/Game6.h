#pragma once

#include <vector>
#include <array>
#include "../Geometry.h"
#include "../Scene/Scene.h"

class Button;

/// �F��ID
enum class COLOR
{
	RED,
	BLUE,
	YELLOW,
	GREEN,
	MAX
};

/// �F�̍ő吔
#define COLOR_SIZE (static_cast<int>(COLOR::MAX))

class Game6 :
	public Scene
{
private:
	void (Game6::*_updater)(const Peripheral& p);
	void (Game6::*_drawer)();

	/// �֐��|�C���^�[���g�p���āA�Q�[���̏�Ԃ��Ǘ��������
	void FadeinUpdate(const Peripheral& p);
	void FadeoutUpdate(const Peripheral& p);

	void StartUpdate(const Peripheral& p);
	void ExpUpdate(const Peripheral& p);
	void CntDownUpdate(const Peripheral& p);
	void GameUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	void ChangeButton();						// �{�^���̈ʒu�ύX
	void ChangeColor();							// ���̕ύX
	void ButtonUpdater(const Peripheral& p);	// �{�^�������͂��ꂽ���̍X�V�p
	
	void StartDraw();							// �Q�[���J�n�O�̕`��
	void ExpDraw();								// �����e�L�X�g�̕`��
	void CntDownDraw();							// �J�E���g�_�E�����̕`��
	void GameDraw();							// �Q�[�����̕`��

	std::vector<COLOR> _btnType;									// �{�^���̐F���
	std::array<std::shared_ptr<Button>, COLOR_SIZE> _buttons;		// �Q�[�����̃{�^��
	std::array<std::string, COLOR_SIZE> _texts;						// �F��ID 
	std::array<int, COLOR_SIZE> _colors;							// �Q�[�������p�̉摜
	std::array<int, 3> _expImgs;

	Vector2 _strSize;		// �����̑傫��		

	int _questions;
	int _corrects;			// ����
	int _timeCnt;
	int _expCnt;
	int _blindCnt;
	int _correctImg, _missImg;
	int _textNum, _colorNum;		// ����̕����ƐF�̎擾�p
	int _correctSE, _missSE, _cntDownSE, _startSE;
	int _gameBGM;

	bool _isColor;			// �F��ύX�������̔���p

	const Size _btnSize;

public:
	Game6();
	~Game6();

	void Update(const Peripheral& p);
	void Draw();

};
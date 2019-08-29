#pragma once
#include <memory>
#include "Scene/Scene.h"
#include "Geometry.h"


class FileSystem;


// �ްёS�̂𐧌䂷��׽
class Game
{
private:
	// �ݸ���݂̌��܂�
	Game();							// �����֎~
	Game(const Game&);				// ��߰�֎~
	void operator=(const Game&);	// ����֎~

	std::shared_ptr<FileSystem> fileSystem;

	const Vector2 screenSize;
	unsigned int fontSize;

	bool _end;
	
public:
	// �ݸ���ݲ���̪��
	static Game& Instance()
	{
		static Game instance;
		return instance;
	}
	~Game();

	// �������n
	// ��{�f�ނ�۰�ނƂ������������s��
	void Initialize();

	// ����ٰ�߂���
	// ������ĂԂƹްт��n�܂�
	void Run();

	// �㏈��
	// ײ���؂̌㏈����
	void Terminate();

	// �X�N���[���T�C�Y�̎擾
	const Vector2& GetScreenSize()const;

	const std::shared_ptr <FileSystem> GetFileSystem()const;

	void END();
};


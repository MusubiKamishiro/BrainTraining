#pragma once
#include <string>

///データ基底クラス
class Data
{
public:
	virtual ~Data() {};

	///ダミー関数
	virtual bool IsAvailable() = 0;
};

///ロード基底クラス
class Loader
{
public:
	Loader();
	virtual ~Loader();

	///ロード
	///@param path ファイルのパス
	///@param data データオブジェクトへの参照
	///@retval true 成功
	///@retval false 失敗
	virtual bool Load(std::string path, Data& data) = 0;

	///データのアンロード
	///@attention	これで指定されたパスに対応するデータはメモリ上から無くなるので
	///				もう一回使用するときはストレージアクセスが発生するよ
	virtual void UnLoad(const char* path) = 0;
};


#pragma once
#include "Loader.h"
#include <map>


class ImageLoader : public Loader
{
private:
	std::map<std::string, int> table;

public:
	ImageLoader();
	~ImageLoader();

	bool Load(std::string path, Data& data)override final;

	void UnLoad(const char* path)override final;
};


class ImageData : public Data
{
	friend ImageLoader;
private:
	int handle;
	bool IsAvailable();
public:
	int GetHandle()const;
};

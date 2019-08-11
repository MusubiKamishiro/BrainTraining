#include "FileSystem.h"
#include "Loader.h"
#include "ImageLoader.h"
#include "SoundLoader.h"


FileSystem::FileSystem()
{
	imageLoader = std::unique_ptr<ImageLoader>();
	soundLoader = std::unique_ptr<SoundLoader>();

	loaders["bmp"] = loaders["png"] = loaders["jpg"] = std::make_shared<ImageLoader>();
	loaders["mp3"] = loaders["wav"] = std::make_shared<SoundLoader>();
}


FileSystem::~FileSystem()
{
}

std::string FileSystem::GetExtension(const char* path)
{
	std::string str = path;
	auto idx = str.rfind('.') + 1;
	return str.substr(idx, str.length() - idx);
}

bool FileSystem::Load(const char * filePath, Data & data)
{
	auto ext = GetExtension(filePath);
	return loaders[ext]->Load(filePath, data);
}

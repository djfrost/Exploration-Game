#if !defined PATH_RESOURCE
#define PATH_RESOURCE

#include "GameResource.h"

class GameManager;

class PathResource: GameResource{
	private:
		GameManager* gm;
	public:
		PathResource(std::string level_name, std::string path, GameResourceType type, GameManager* game_manager);
		virtual ~PathResource();
		virtual void load();
		virtual void unload();
};

#endif
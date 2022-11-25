#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "entt/entt.hpp"
struct message
{
	char type = 1;
	char name[64];
	float x;
	float y;
	uint32_t getSize() 
	{ 
		return 64 + sizeof(float) * 2 + 1 + sizeof(char*); 
	}
};

class EntityManager : public entt::registry
{
public:
	
};


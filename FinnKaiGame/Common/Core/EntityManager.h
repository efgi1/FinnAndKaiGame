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
	void extractType(char* data)
	{
		type = data[0];
	}
	void extractMessage(char* data)
	{
		for (int i = 0; i < 64; ++i)
			name[i] = data[i];
		data = &data[64];
		x = atof(data);
		data = &data[sizeof(float)];
		y = atof(data);
		data = &data[sizeof(float)];
	}
	uint32_t getSize() 
	{ 
		return 64 + sizeof(float) * 2 + 1 + sizeof(char*); 
	}
};

class EntityManager : public entt::registry
{
public:
	
};


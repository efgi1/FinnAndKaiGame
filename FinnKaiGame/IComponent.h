#pragma once

class IComponent {
public:
	IComponent() : implemented(false) {}
	bool implemented;
};
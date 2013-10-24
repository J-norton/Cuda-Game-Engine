#pragma once
#include <string>
#include <iostream>
class ScenarioException
{
private:
	std::string error_message;
public:
	std::string what();
	ScenarioException(std::string error_message);
	ScenarioException(void);
	~ScenarioException(void);
};


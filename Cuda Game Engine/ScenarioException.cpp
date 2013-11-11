#include "ScenarioException.h"

ScenarioException::ScenarioException(std::string error) :
	error_message(error)
{
}

ScenarioException::ScenarioException(void)
{
}


ScenarioException::~ScenarioException(void)
{
}

std::string ScenarioException::what()
{
	return error_message;
}
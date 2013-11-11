#include "ErrorMessage.h"

ErrorMessage::ErrorMessage(std::string message) :
    error_message(message)
{
}

ErrorMessage::ErrorMessage(void)
{
}


ErrorMessage::~ErrorMessage(void)
{
}

void ErrorMessage::setText(std::string message)
{
    Magick::Image error_message;
    error_texture = Texture(error_message);
    mesh_state.setTexture(&error_texture);
}

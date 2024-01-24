#pragma once
#include <string>
 
class Texture
{
private:
	unsigned int _rendererId;
	std::string _fileSrc;
	int _height;
	int _width;
	int _channels;
public:
	Texture(std::string src, int width, int height, int channels);
	~Texture();

	void Bind() const;
	void Unbind() const;

};

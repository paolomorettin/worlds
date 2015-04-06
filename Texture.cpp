#include "Texture.h"
#include "util.h"

Texture::Texture(string path)
{
	load(path);
}
Texture::~Texture()
{
}

//get
GLuint Texture::getTextureObject() const
{
	return textureObject;
}

void Texture::load(string path)
{
    int width, height;
	void *pixels = read_tga(path.c_str(), &width, &height);
    if (!pixels)
        throw TextureLoadException();
    glGenTextures(1, &textureObject);
    glBindTexture(GL_TEXTURE_2D, textureObject);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
    free(pixels);
}

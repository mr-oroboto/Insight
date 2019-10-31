#ifndef INSIGHT_CORE_TEXTURECOLLECTION_H
#define INSIGHT_CORE_TEXTURECOLLECTION_H

#include <string>
#include <unordered_map>

#include "shader/StandardShader.h"
#include "Texture.h"

class TextureCollection
{
public:
    TextureCollection(StandardShader* object_shader) : object_shader_(object_shader) {}
    ~TextureCollection();

    bool registerTexture(const std::string& path, const std::string& name);
    Texture* getTexture(const std::string& name);

private:
    std::unordered_map<std::string, Texture*> textures_;
    StandardShader* object_shader_;
};


#endif //INSIGHT_CORE_TEXTURECOLLECTION_H

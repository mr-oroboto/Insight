#include "TextureCollection.h"

namespace insight {


TextureCollection::~TextureCollection()
{
    for (std::unordered_map<std::string, Texture*>::iterator it = textures_.begin(); it != textures_.end(); it++)
    {
        if (it->second != nullptr)
        {
            delete it->second;
        }
    }
}

bool TextureCollection::registerTexture(const std::string &path, const std::string &name)
{
    bool registered = false;

    Texture* texture = new Texture(path, object_shader_);
    if (texture->initialise())
    {
        if (textures_.find(name) != textures_.end() && textures_[name] != nullptr)
        {
            delete textures_[name];
        }

        textures_[name] = texture;

        registered = true;
    }

    return registered;
}

Texture* TextureCollection::getTexture(const std::string &name)
{
    if (textures_.find(name) == textures_.end())
    {
        return nullptr;
    }

    return textures_[name];
}


}   // namespace insight
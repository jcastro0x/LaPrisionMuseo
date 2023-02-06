// Copyright (c) 2022 Javier Castro - jcastro0x@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NON INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include "AssetManager.hpp"

#include <filesystem>
#include <memory>


namespace lpm
{
    class TextResource
    {
    public:
        void loadFromFile(std::filesystem::path const& path);
        operator sf::String*();

        TextResource() = default;
        TextResource(sf::String* text) : text_(*text) {}

    private:
        sf::String text_;
    };

    class AssetManagerSFML : public IAssetManager
    {
        template<typename Resource>
        using resource = std::unordered_map<std::string, Resource>;

    public:
        sf::Texture* loadTexture(std::string_view path)                             override;
        const sf::SoundBuffer* loadSoundBuffer(std::string_view path)               override;
        const sf::Font* loadFont(std::string_view path)                             override;
        const sf::String* loadText(std::string_view path)                           override;
        sf::Shader* loadShader(std::string_view path)                               override;
        std::vector<std::string> getAssetsInDirectory(std::string_view path) const  override;


        bool unloadTexture(sf::Texture* texture)                                    override;
        bool unloadSoundBuffer(sf::SoundBuffer* sound)                              override;
        bool unloadFont(sf::Font* font)                                             override;
        bool unloadText(sf::String* text)                                           override;
        bool unloadShader(sf::Shader* shader)                                       override;

    private:
        resource<sf::Texture> textures_;
        resource<sf::SoundBuffer> sounds_;
        resource<sf::Font> fonts_;
        resource<TextResource> texts_;
        resource<std::unique_ptr<sf::Shader>> shaders_;
    };
}

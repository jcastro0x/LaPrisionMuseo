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
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NON INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace lpm
{
    /**
     * A class that is responsible for loading and unloading assets
     **/
    class IAssetManager
    {
    public:
        virtual ~IAssetManager() = default;

        virtual sf::Texture* loadTexture(std::string_view path)                             = 0;
        virtual const sf::SoundBuffer* loadSoundBuffer(std::string_view path)               = 0;
        virtual const sf::Font* loadFont(std::string_view path)                             = 0;
        virtual const sf::String* loadText(std::string_view path)                           = 0;
        virtual sf::Shader* loadShader(std::string_view path)                               = 0;
        virtual std::vector<std::string> getAssetsInDirectory(std::string_view path) const  = 0;


        virtual bool unloadTexture(sf::Texture* texture)                                    = 0;
        virtual bool unloadSoundBuffer(sf::SoundBuffer* sound)                              = 0;
        virtual bool unloadFont(sf::Font* font)                                             = 0;
        virtual bool unloadText(sf::String* text)                                           = 0;
        virtual bool unloadShader(sf::Shader* shader)                                       = 0;

    };
}

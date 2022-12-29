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

#include "AssetManagerSFML.hpp"

#include <filesystem>
#include <fstream>

#include <iostream>

using namespace lpm;

template<typename Resource, typename Container>
Resource* loadOrCreate(std::string_view path, Container& container)
{
    // TODO: Obviously temporal approach...
    static std::filesystem::path root("/home/jcastro/Development/prisi_assets");
    auto const final_path = root / path.data();

    if(auto ptr = container.find(final_path); ptr != container.end())
    {
        return &ptr->second;
    }
    else
    {
        Resource res;
        res.loadFromFile(final_path);
        container.try_emplace(final_path, std::move(res));

        std::cout << "AssetManager: Loaded " << final_path << "\n";

        return &container[final_path];
    }
}


sf::Texture* AssetManagerSFML::loadTexture(std::string_view path)
{
    return loadOrCreate<sf::Texture>(path, textures_);
}

const sf::SoundBuffer* AssetManagerSFML::loadSoundBuffer(std::string_view path)
{
    return loadOrCreate<sf::SoundBuffer>(path, sounds_);
}

const sf::Font* AssetManagerSFML::loadFont(std::string_view path)
{
    return loadOrCreate<sf::Font>(path, fonts_);
}

void TextResource::loadFromFile(std::filesystem::path const& path)
{
    std::ifstream ifstream(path);
    std::string str;
    if(ifstream) {
        std::ostringstream ss;
        ss << ifstream.rdbuf(); // reading data
        str = ss.str();
    }

    text_ = str;
}

TextResource::operator sf::String*() {
    return &text_;
}


const sf::String* AssetManagerSFML::loadText(std::string_view path)
{
    return (const sf::String*)loadOrCreate<TextResource>(path, texts_);
}

sf::Shader* AssetManagerSFML::loadShader(std::string_view path)
{
    // TODO: Obviously temporal approach...
    static std::filesystem::path root("/home/jcastro/Development/prisi_assets");
    auto const final_path = root / path.data();

    if(auto ptr = shaders_.find(final_path); ptr != shaders_.end())
    {
        return ptr->second.get();
    }
    else
    {
        auto res = std::make_unique<sf::Shader>();
        const auto code = loadText(path);
        res->loadFromMemory(*code, sf::Shader::Type::Fragment);
        shaders_.try_emplace(final_path, std::move(res));

        return shaders_[final_path].get();
    }
}

std::vector<std::string> AssetManagerSFML::getAssetsInDirectory(std::string_view path) const
{
    std::vector<std::string> files;

    // TODO: Obviously temporal approach...
    static std::filesystem::path root("/home/jcastro/Development/prisi_assets");
    for(auto const& entry : std::filesystem::directory_iterator(root / path))
    {
        files.emplace_back(entry.path().filename());
    }

    return files;
}

bool AssetManagerSFML::unloadAsset(std::string_view path)
{
    return false;
}

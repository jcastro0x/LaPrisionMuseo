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

#include <unordered_map>
#include <string>
#include <memory>
#include <optional>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


#include <iostream>
#include <fstream>
#include <filesystem>

namespace sf
{
    class Texture;
    class SoundBuffer;
    class Font;
}

namespace lpm
{
    class resource_exception final : public std::exception
    {
    };

    struct ResourceAsset
    {
        uint64_t hash;
        class ResourcesManager* manager;
        std::vector<uint8_t*> data;
    };

    class ResourcesManager
    {
        template<typename Type>
        using Resource = std::unordered_map<std::string, std::unique_ptr<Type>>;

    public:
        template<typename T> requires true
        void foo(std::string_view path)
        {
            sf::FileInputStream ifs;
            ifs.open(path.data());

            sf::Texture t;
            t.loadFromStream(ifs);

            //std::ifstream ifs(path.data(), std::ios::binary);
            //if(ifs.is_open())
            //{
            //
            //}
        }

        static void pushNumber64(std::ofstream& out, uint64_t number)
        {
            out << static_cast<uint8_t>((number & 0xFF00'0000'0000'0000) >> 56);
            out << static_cast<uint8_t>((number & 0x00FF'0000'0000'0000) >> 48);
            out << static_cast<uint8_t>((number & 0x0000'FF00'0000'0000) >> 40);
            out << static_cast<uint8_t>((number & 0x0000'00FF'0000'0000) >> 32);
            out << static_cast<uint8_t>((number & 0x0000'0000'FF00'0000) >> 24);
            out << static_cast<uint8_t>((number & 0x0000'0000'00FF'0000) >> 16);
            out << static_cast<uint8_t>((number & 0x0000'0000'0000'FF00) >> 8);
            out << static_cast<uint8_t>((number & 0x0000'0000'0000'00FF) >> 0);
        }

        static void createPackFile(std::string_view relativePath)
        {
            auto path = std::filesystem::current_path() / relativePath.data();

            std::cout << "Packing files from " << path << "\n";

            std::ofstream out(path / "out.bin", std::ios::binary);

            // Magic Number
            out << "LA PRISION MUSEO RESOURCE FILE";

            // Version
            out << static_cast<uint8_t>(0);

            // ZLib Compression level
            out << static_cast<uint8_t>(0);

            // Endianness
            out << static_cast<uint8_t>(0);

            // Hashing Algorithm
            out << static_cast<uint8_t>(0);

            // Assets in resource file
            uint64_t filesCount = std::count_if(std::filesystem::recursive_directory_iterator(path), std::filesystem::recursive_directory_iterator{}, [](auto& entry){
                return entry.is_regular_file();
            });
            pushNumber64(out, filesCount);


            for(auto& entry : std::filesystem::recursive_directory_iterator(path))
            {
                if(entry.is_directory()) continue;
                if(entry.path().string().find("out.bin") != std::string::npos) continue;

                std::ifstream ifs(entry.path().c_str(), std::ios::binary);
                if(ifs.is_open())
                {
                    std::cout << "Packing " << entry.path() << "...\n";

                    // Asset name
                    out << entry.path().c_str() << static_cast<uint8_t>(0x00);

                    // Asset size
                    pushNumber64(out, entry.file_size());
                    //out << static_cast<uint8_t>((entry.file_size() & 0xFF00'0000'0000'0000) >> 56);
                    //out << static_cast<uint8_t>((entry.file_size() & 0x00FF'0000'0000'0000) >> 48);
                    //out << static_cast<uint8_t>((entry.file_size() & 0x0000'FF00'0000'0000) >> 40);
                    //out << static_cast<uint8_t>((entry.file_size() & 0x0000'00FF'0000'0000) >> 32);
                    //out << static_cast<uint8_t>((entry.file_size() & 0x0000'0000'FF00'0000) >> 24);
                    //out << static_cast<uint8_t>((entry.file_size() & 0x0000'0000'00FF'0000) >> 16);
                    //out << static_cast<uint8_t>((entry.file_size() & 0x0000'0000'0000'FF00) >> 8);
                    //out << static_cast<uint8_t>((entry.file_size() & 0x0000'0000'0000'00FF) >> 0);

                    // Asset data
                    uint8_t byte;
                    for(size_t i = 0; i < entry.file_size(); i++)
                    {
                        ifs >> byte;
                        out << byte;
                    }
                }
            }
        }

        virtual ~ResourcesManager();


    public:
        [[nodiscard]] static std::optional<const sf::Texture*> getTexture(std::string_view key);
        [[nodiscard]] static std::optional<const sf::SoundBuffer*> getSoundBuffer(std::string_view key);
        [[nodiscard]] static std::optional<const sf::Font*> getFont(std::string_view key);


    private:
        inline static Resource<sf::Texture> textures_;
        inline static Resource<sf::SoundBuffer> sounds_;
        inline static Resource<sf::Font> fonts_;
    };
}

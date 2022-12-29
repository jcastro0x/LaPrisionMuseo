//// Copyright (c) 2022 Javier Castro - jcastro0x@gmail.com
////
//// Permission is hereby granted, free of charge, to any person obtaining
//// a copy of this software and associated documentation files (the
//// "Software"), to deal in the Software without restriction, including
//// without limitation the rights to use, copy, modify, merge, publish,
//// distribute, sublicense, and/or sell copies of the Software, and to
//// permit persons to whom the Software is furnished to do so, subject to
//// the following conditions:
////
//// The above copyright notice and this permission notice shall be
//// included in all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//// NON INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
//// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
//// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//#pragma once
//
//#include <unordered_map>
//#include <string>
//#include <memory>
//#include <optional>
//
//namespace sf
//{
//    class Texture;
//    class SoundBuffer;
//    class Font;
//}
//
//namespace lpm
//{
//    class resource_exception final : public std::exception
//    {
//    };
//
//    class Resources
//    {
//        template<typename Type>
//        using Resource = std::unordered_map<std::string, std::unique_ptr<Type>>;
//
//    public:
//        Resources();
//        ~Resources();
//
//    public:
//        [[nodiscard]] std::optional<const sf::Texture*> getTexture(std::string_view key) const;
//        [[nodiscard]] std::optional<const sf::SoundBuffer*> getSoundBuffer(std::string_view key) const;
//        [[nodiscard]] std::optional<const sf::Font*> getFont(std::string_view key) const;
//
//
//    private:
//        Resource<sf::Texture> textures_;
//        Resource<sf::SoundBuffer> sounds_;
//        Resource<sf::Font> fonts_;
//    };
//}

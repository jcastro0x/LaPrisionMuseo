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

#include <bit>
#include <cstdint>
#include <array>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <map>
#include <type_traits>
#include <cassert>

#include <zlib.h>

namespace lpm
{
    template<size_t ArrSize>
    std::ostream& operator<<(std::ostream& os, const std::array<uint8_t, ArrSize>& arr)
    {
        for(const auto& byte : arr)
        {
            os << byte;
        }

        return os;
    }

    template<typename SizeType>
    void fill_ostream(std::ostream& os, uint8_t byte)
    {
        for(size_t i = 0; i < sizeof(SizeType); i++) os.put(byte);
    }

    class BinaryFormatter
    {


    public:
        bool compress(std::vector<uint8_t>& bytes) const
        {
            std::vector<uint8_t> compressed(bytes.size());
            auto compressedSize = static_cast<uLongf>(compressed.size());

            if(::compress2(compressed.data(), &compressedSize, bytes.data(), static_cast<uLongf>(bytes.size()), zlibCompressionLevel_) != Z_OK)
            {
                return false;
            }

            compressed.resize(compressedSize);
            bytes = compressed;

            return true;
        }

        bool decompress(std::vector<uint8_t>& bytes) const
        {
            std::vector<uint8_t> compressed(bytes.size());
            auto compressedSize = static_cast<uLongf>(compressed.size());

            if(::uncompress(compressed.data(), &compressedSize, bytes.data(), static_cast<uLongf>(bytes.size())) != Z_OK)
            {
                return false;
            }

            compressed.resize(compressedSize);
            bytes = compressed;

            return true;
        }

        template<typename Func>
        void iterateFiles(std::filesystem::path const& origin, Func func)
        {
            for(auto const& entry : std::filesystem::recursive_directory_iterator(origin))
            {
                // Dont process directories
                if(entry.is_directory()) continue;

                // Don't process binary file itself
                if(entry.path().string().find(outFileName_) != std::string::npos) continue;

                // Get relative path of asset
                auto path = entry.path().string().substr(origin.string().size() + 1,
                                                         entry.path().string().size() - origin.string().size());

                // Convert separator to unix style
                #if WIN32
                std::ranges::for_each(path, [](auto& c){
                    if(c == std::filesystem::path::preferred_separator) c = '/';
                });
                #endif

                // lower path
                std::ranges::transform(path.begin(), path.end(), path.begin(),
                                       [](uint8_t c) { return static_cast<uint8_t>(std::tolower(c)); });

//                std::ifstream ifs(entry.path(), std::ios::in | std::ios::binary);
//                std::vector<char> bytes(entry.file_size());
//                ifs.read(bytes.data(), static_cast<std::streamsize>(bytes.size()));

                func(path, entry.file_size());
            }
        }

        std::vector<uint8_t> readAsset(std::string_view path) const
        {
            assert(!std::ranges::any_of(path.begin(), path.end(), ::isupper) && "readAsset only accepts lower strings");

            const auto dataPath = std::filesystem::current_path() / outFileName_;
            std::cout << "Data File size: " << file_size(dataPath) << "\n";
            if(file_size(dataPath) < strlen(magicNumber_) + 1)
            {
                std::cerr << "Wrong header: File too small.\n";
            }

            std::ifstream ifs(std::filesystem::current_path() / outFileName_, std::ios::in | std::ios::binary);
            std::string mm;
            ifs >> mm;

            std::cout << "Magic Number read: "     << mm << "\n";
            std::cout << "Magic Number expected: " << magicNumber_ << "\n";
            if(mm != std::string(magicNumber_))
            {
                std::cerr << "Wrong header: Magic number doesn't match.\n";
                return {};
            }

            uint8_t zlibCompressionLevel = 0;
            ifs >> zlibCompressionLevel;

            uint64_t filesCount = 0;
            ifs >> filesCount;



            return {};
        }

        void createFile(std::filesystem::path const& assetsPath, uint8_t zlibCompressionLevel = Z_BEST_COMPRESSION)
        {
            zlibCompressionLevel_ = zlibCompressionLevel;

            std::cout << "Packing files from " << assetsPath << "\n";
            std::ofstream out(assetsPath / outFileName_, std::ios::out | std::ios::binary);

            // Write header
            {
                out << magicNumber_; out.put(0);
                out << extractNumber(version_);
                out.put(zlibCompressionLevel_);

                // Assets in resource file
                const uint64_t filesCount = std::count_if(std::filesystem::recursive_directory_iterator(assetsPath), std::filesystem::recursive_directory_iterator{}, [](auto& entry){
                    if(entry.path().string().find(outFileName_) != std::string::npos) return false;
                    return !entry.is_directory();
                });
                out << extractNumber(filesCount);
            }
            
            std::map<std::string, uint64_t> positions;

            // Creating access table
            iterateFiles(assetsPath, [&](auto& entry, size_t size)
            {
                // Asset name
                out << entry; out.put(0);

                // Store asset position
                positions.try_emplace(entry, out.tellp());

                // Fill with zeroes
                fill_ostream<uint64_t>(out, 0);

                std::cout << "Table created for " << entry << "\n";
            });



            iterateFiles(assetsPath, [&](auto& entry, size_t size)
            {
                std::ifstream ifs(entry, std::ios::in | std::ios::binary);
                if(ifs.is_open())
                {
                    std::vector<char> assetBytes(size);
                    ifs.read(&assetBytes[0], static_cast<std::streamsize>(size));

                    auto assetPosition = out.tellp();
                    auto tablePosition = positions[entry];

                    // Fill asset's data position into access table
                    out.seekp(tablePosition, std::ios::beg);
                    out << extractNumber(assetPosition);
                    out.seekp(assetPosition, std::ios::beg);

                    // Compress asset
                    if (zlibCompressionLevel_ > 0) {

                        std::vector<unsigned char> v(assetBytes.begin(), assetBytes.end());
                        compress(v);
                        assetBytes.resize(v.size());
                        for(size_t i = 0; i < v.size(); i++)
                        {
                            assetBytes[i] = static_cast<char>(v[i]);
                        }
                    }

                    // Write asset size
                    out << extractNumber(assetBytes.size());

                    // Put asset into pack file
                    out.write(assetBytes.data(), static_cast<std::streamsize>(assetBytes.size()));

                    std::cout << "Binary pushed for " << entry << "\n";
                }
            });
        }

    private:
        /**
         * Extract bytes from an unsigned 64 bits in sharedEndianness_ order.
         * @return array with bytes.
         */
        static std::array<uint8_t, 8> extractNumber(uint64_t number)
        {
            static_assert(sizeof(number) == 8, "extractNumber only accepts 64bits integers");

            if constexpr(std::endian::native != sharedEndianness_)
            {
                number = byteswap(number);
            }

            std::array<uint8_t, 8> values;
            values[0] = static_cast<uint8_t>((number & 0xFF00'0000'0000'0000) >> 56);
            values[1] = static_cast<uint8_t>((number & 0x00FF'0000'0000'0000) >> 48);
            values[2] = static_cast<uint8_t>((number & 0x0000'FF00'0000'0000) >> 40);
            values[3] = static_cast<uint8_t>((number & 0x0000'00FF'0000'0000) >> 32);
            values[4] = static_cast<uint8_t>((number & 0x0000'0000'FF00'0000) >> 24);
            values[5] = static_cast<uint8_t>((number & 0x0000'0000'00FF'0000) >> 16);
            values[6] = static_cast<uint8_t>((number & 0x0000'0000'0000'FF00) >> 8);
            values[7] = static_cast<uint8_t>((number & 0x0000'0000'0000'00FF) >> 0);

            return values;
        }

        /**
         * Invert order of an unsigned 64 (little to big, or big to little).
         * @return Integer swapped.
         */
        static auto byteswap(uint64_t v) noexcept -> uint64_t
        {
            return ((v & uint64_t(0x0000'0000'0000'00FF)) << 56) |
                   ((v & uint64_t(0x0000'0000'0000'FF00)) << 40) |
                   ((v & uint64_t(0x0000'0000'00FF'0000)) << 24) |
                   ((v & uint64_t(0x0000'0000'FF00'0000)) <<  8) |
                   ((v & uint64_t(0x0000'00FF'0000'0000)) >>  8) |
                   ((v & uint64_t(0x0000'FF00'0000'0000)) >> 24) |
                   ((v & uint64_t(0x00FF'0000'0000'0000)) >> 40) |
                   ((v & uint64_t(0xFF00'0000'0000'0000)) >> 56);
        }

    private:
        inline static uint8_t zlibCompressionLevel_     = Z_BEST_COMPRESSION;
        static constexpr const char* magicNumber_       = "LAPRISIMUSEO";
        static constexpr const char* outFileName_       = "museo.dat";
        static constexpr uint64_t version_              = 0;
        static constexpr std::endian sharedEndianness_  = std::endian::big;
    };
}

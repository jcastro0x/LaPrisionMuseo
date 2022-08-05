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

#include <zlib.h>

namespace lpm
{
    class BinaryFormatter
    {
    public:

        void createFile(std::string_view relativePath, uint8_t zlibCompressionLevel = Z_BEST_COMPRESSION)
        {
            zlibCompressionLevel_ = zlibCompressionLevel;

            auto path = std::filesystem::current_path() / relativePath.data();
            std::cout << "Packing files from " << path << "\n";

            std::ofstream out(path / outFileName_, std::ios::binary);

            out << magicNumber_;
            for(const auto& byte : extractNumber(version_))
            {
                out << byte;
            }
            out.put(zlibCompressionLevel_);
            out.put(endianness_);

            // Assets in resource file
            const uint64_t filesCount = std::count_if(std::filesystem::recursive_directory_iterator(path), std::filesystem::recursive_directory_iterator{}, [](auto& entry){
                if(entry.path().string().find(outFileName_) != std::string::npos) return false;
                return !entry.is_directory();
            });
            for(const auto& byte : extractNumber(filesCount))
            {
                out << byte;
            }

            std::map<std::string, uint64_t> positions;

            // Creating access table
            for(auto& entry : std::filesystem::recursive_directory_iterator(path))
            {
                if(entry.is_directory()) continue;
                if(entry.path().string().find(outFileName_) != std::string::npos) continue;

                std::ifstream ifs(entry.path().c_str(), std::ios::binary);
                if(ifs.is_open())
                {
                    std::cout << "Packing " << entry.path().filename() << "...\n";

                    // Asset name
                    out << entry.path().c_str();
                    out.put(0);

                    // Store asset position
                    positions.try_emplace(entry.path(), out.tellp());

                    // Fill with zeroes
                    for(int i = 0; i < 8; i++) out.put(0);
                }
            }

            // Creating assets data
            for(auto& entry : std::filesystem::recursive_directory_iterator(path))
            {
                if(entry.is_directory()) continue;
                if(entry.path().string().find(outFileName_) != std::string::npos) continue;

                std::ifstream ifs(entry.path().c_str(), std::ios::binary);
                if(ifs.is_open())
                {
                    auto currentPosition = out.tellp();
                    auto tablePosition = positions[entry.path()];

                    // Fill asset's data position
                    out.seekp(tablePosition, std::ios::beg);
                    for(const auto& byte : extractNumber(currentPosition))
                    {
                        out << byte;
                    }


                    out.seekp(currentPosition, std::ios::beg);

                    // No compression
                    if(zlibCompressionLevel_ == 0)
                    {
                        // Asset size
                        for(const auto& byte : extractNumber(entry.file_size()))
                        {
                            out << byte;
                        }

                        // Asset data
                        uint8_t byte;
                        for(size_t i = 0; i < entry.file_size(); i++)
                        {
                            ifs >> byte;
                            out << byte;
                        }
                    }
                    else
                    {
                        //constexpr long int CHUNK = 16384;

                        std::cout << "Compressing...\n";

                        z_stream strm;
                        strm.zalloc  = Z_NULL;
                        strm.zfree   = Z_NULL;
                        strm.opaque  = Z_NULL;
//
//                        int flush = Z_NO_FLUSH;
//                        int ret   = deflateInit(&strm, zlibCompressionLevel_);
//
//                        if(ret != Z_OK)
//                        {
//                            std::cerr << "Error compressing " << entry.path().filename() << "\n";
//                            continue;
//                        }
//
//                        do
//                        {
//                            strm.avail_in = fread(in, 1, CHUNK, source);
//                            if (ferror(source)) {
//                                (void)deflateEnd(&strm);
//                                return Z_ERRNO;
//                            }
//                            flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
//                            strm.next_in = in;
//
//                            do {
//                                strm.avail_out = CHUNK;
//                                strm.next_out = out;
//                                ret = deflate(&strm, flush);    /* anyone error value */
//                                assert(ret != Z_STREAM_ERROR);
//                                have = CHUNK - strm.avail_out;
//                                if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
//                                    (void)deflateEnd(&strm);
//                                    return Z_ERRNO;
//                                }
//                        }
//                        while(flush != Z_FINISH)
                    }



                }
            }
        }

    private:
        /**
         * Extract bytes from an unsigned 64 bits in big endianness order.
         * @return array with bytes.
         */
        static std::array<uint8_t, 8> extractNumber(uint64_t number)
        {
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
        static auto byteswap(uint64_t v) noexcept
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
        inline static uint8_t zlibCompressionLevel_ = Z_BEST_COMPRESSION;
        static constexpr const char* magicNumber_   = "LAPRISIMUSEO";
        static constexpr const char* outFileName_   = "out.bin";
        static constexpr uint64_t version_          = 0;
        static constexpr uint8_t endianness_        = std::endian::native == std::endian::little ? 0 : 1;
    };
}

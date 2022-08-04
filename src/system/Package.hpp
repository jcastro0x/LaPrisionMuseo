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

#include <concepts>

#include <array>
#include <vector>
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <stdexcept>
#include <algorithm>

//TODO: std::endian is implementation defined. All compilers uses sames values to little and big? (tip: check on a mac)
#include <bit>
#define ENDIANNESS_LITTLE   static_cast<uint8_t>(std::endian::little)
#define ENDIANNESS_BIG      static_cast<uint8_t>(std::endian::big)
#define MACHINE_ENDIANNESS  static_cast<uint8_t>(std::endian::native)

#define THROW_POP_OVERFLOW(Type)  throw package_overflow_exception("Package::operator>>(" #Type ") would generate overflow")
#define THROW_PUSH_OVERFLOW(Type) throw package_overflow_exception("Package::operator<<(" #Type ") would generate overflow")


template <typename T>
concept ArithmeticConcept = std::is_arithmetic_v<T>;


template <typename T>
concept BooleanConcept = std::convertible_to<T, bool>;

namespace mof
{
    /**
     * LIFO container of arithmetic, std::string, booleans and std::vector<T> types.
     * Stores values as binary data in continuous memory and an identifier of packet.
     * Buffer are shared within an union with struct to handle id and bytes uses more easy.
     *
     * @tparam IDTYPE Type to package type. Use an user enum would be a good choice.
     * @tparam MAX Max bytes allocated into the buffer. The buffer doesn't deallocate till class is destructed.
     * @tparam ENDIANNESS Default endianness used to push values. If platform uses one different, convert in place when push.
     * When pop a value, if the platform uses one different, convert in place. Server's endianness should define this
     * value to avoid unnecessary operations inside it.
     */
    template<typename IDTYPE = uint16_t, size_t MAX = 1024, uint8_t ENDIANNESS = ENDIANNESS_LITTLE>
    struct Package
    {
        class package_overflow_exception final : public std::runtime_error
        {
            public: explicit package_overflow_exception(const char* Message) : std::runtime_error(Message) {}
        };

        union
        {
            //TODO: bodySize and id have NOT Endianness support
            struct Header
            {
                int16_t bodySize;
                IDTYPE id;
            };

            mutable Header header {};
            std::array<uint8_t, MAX> raw_bytes; //TODO: I don't like this... might a vector? Check Connection::transmit to see my concern... It's over-allocated
        } Buffer;

        static constexpr size_t MaxSize = MAX - sizeof(Buffer.header);
        static_assert(MaxSize <= (2 << (sizeof(Buffer.header.bodySize)*8))
                , "SIZE must have a size that header.bodySize is able to point");


        using type_id   = IDTYPE;
        using type_body = decltype(Buffer.header.bodySize);

    public:
        explicit Package() = default;
        explicit Package(const IDTYPE& type) noexcept
        {
            std::ranges::fill(Buffer.raw_bytes.begin(), Buffer.raw_bytes.end(), uint8_t{});
            Buffer.header.id = type;
        }

        virtual ~Package() = default; //std::array destroy itself, don't need more specific actions

        Package(const Package& package) noexcept
        {
            std::ranges::copy(
                package.Buffer.raw_bytes.cbegin(),
                package.Buffer.raw_bytes.cend(),
                this->Buffer.raw_bytes.begin()
            );
        }

        Package& operator=(const Package& package) noexcept
        {
            std::ranges::copy(
                package.Buffer.raw_bytes.cbegin(),
                package.Buffer.raw_bytes.cend(),
                this->Buffer.raw_bytes.begin()
            );
            return *this;
        }

        Package(Package&& package) noexcept
        {
            this->Buffer.raw_bytes.swap(package.Buffer.raw_bytes);
        }

        Package& operator=(Package&& package) noexcept
        {
            this->Buffer.raw_bytes.swap(package.Buffer.raw_bytes);
            return *this;
        }

        template<typename OtherPackage>
        void merge(OtherPackage other)
        {
            static_assert(other.MaxSize <= MaxSize, "Can't merge origin package bigger than destiny");
            std::ranges::copy(
                other.Buffer.raw_bytes.cbegin(),
                other.Buffer.raw_bytes.cend(),
                this->Buffer.raw_bytes.begin()
            );
        }

        void clear(bool bFill = false)
        {
            if(bFill) std::ranges::fill(Buffer.raw_bytes.begin(), Buffer.raw_bytes.end(), uint8_t{});
            Buffer.header.bodySize = 0;
        }

    public:
        //~=============================================================================================================
        // PUSH/POP OPERATORS
        //~=============================================================================================================

        /**
         * Generic push Arithmetic-value into the package.
         * @param in Read-only reference to be added into the package
         */
        template<ArithmeticConcept Data>
        Package& operator<<(const Data& in)
        {
            static_assert(sizeof(Data) <= sizeof(uint32_t), "operator<< with more than 32 bits");
            static_assert(std::is_arithmetic_v<Data>, "Generic operator<< only accepts arithmetic values");

            constexpr auto in_size = sizeof(Data); // not ODR-used
            if(header().bodySize + in_size > MaxSize) THROW_PUSH_OVERFLOW(Data);

            if constexpr(sizeof(Data) != 1 && MACHINE_ENDIANNESS != ENDIANNESS)
            {
                Data copy = byteswap(in);
                copyToBuffer(&copy, in_size);
            }
            else
            {
                copyToBuffer(&in, in_size);
            }

            return *this;
        }

        /**
         * Specialization of push c++ string into the package
         * @param in C++ string read-only reference to be added
         */
        Package& operator<<(std::string_view in)
        {
            if(header().bodySize + in.size() + 1 > MaxSize) THROW_PUSH_OVERFLOW(String);

            zeroToBuffer();
            copyToBuffer(in.data(), in.size());
            return *this;
        }

        /**
         * Specialization of push c string into the package
         * @param in C string read-only reference to be added
         */
        Package& operator<<(const char* in)
        {
            std::string string(in);
            *this << string;
            return *this;
        }

        /**
         * Specialization of push c++ container into the package
         * @param in C++ container read-only reference to be added
         */
        template<typename T>
        Package& operator<<(const std::vector<T>& in)
        {
            const auto size = static_cast<type_body>(in.size());
            if(header().bodySize + size + bodySizeSizeof() > MaxSize) THROW_PUSH_OVERFLOW(Vector);

            static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, std::string>
                    , "Vector<T> operator<< only accepts arithmetic and std::string values");

            for(const auto& v: in) *this << v;
            *this << size;
            return *this;
        }

        /**
         * Specialization of push booleans into the package
         * @param in Boolean to be added
         */
        Package& operator<<(bool boolean)
        {
            uint8_t value = boolean ? 1 : 0;
            *this << value;
            return *this;
        }

        /**
         * Generic pop Arithmetic-value from the package.
         * @param in Reference to be populated from the package
         */
        template<class Data>
        const Package& operator>>(Data& out) const
        {
            static_assert(sizeof(Data) <= sizeof(uint32_t), "operator>> with more than 32 bits");
            static_assert(std::is_arithmetic_v<Data>, "Generic operator>> only accepts arithmetic values");

            constexpr auto in_size = sizeof(Data); // not ODR-used
            if(in_size > static_cast<size_t>(header().bodySize))
            {
                THROW_POP_OVERFLOW(Data);
            }

            Buffer.header.bodySize -= in_size;
            std::memcpy(&out, Buffer.raw_bytes.data() + Buffer.header.bodySize + headerSizeof(), in_size);

            if constexpr(sizeof(Data) != 1 && MACHINE_ENDIANNESS != ENDIANNESS)
            {
                out = byteswap(out);
            }



            return *this;
        }

        /**
         * Specialization of pop 16-bit string from the package
         * @param in C-string reference to populate
         */
        //template<>
        const Package& operator>>(std::string& out) const
        {
            const auto ptr = Buffer.raw_bytes.data() + Buffer.header.bodySize + headerSizeof() - 1;

            std::size_t len {0};
            while( *(ptr - len++) != '\0' );
            if(len > static_cast<size_t>(header().bodySize))
            {
                THROW_POP_OVERFLOW(String);  
            } 

            out = std::string(std::bit_cast<const std::string::value_type*>(ptr-len+2), len-1);
            Buffer.header.bodySize -= static_cast<type_body>(len);


            return *this;
        }

        template<typename T>
        const Package& operator>>(std::vector<T>& out) const
        {
            type_body size {};
            *this >> size;
            if(sizeof(T) * size > header().bodySize) THROW_POP_OVERFLOW(Vector);

            out.clear();
            out.resize(size);

            Buffer.header.bodySize -= static_cast<type_body>(sizeof(T) * size);
            std::memcpy(out.data(), Buffer.raw_bytes.data() + Buffer.header.bodySize + headerSizeof(), sizeof(T) * size);

            static_assert(std::is_arithmetic_v<T>, "Vector<T> operator>> only accepts arithmetic values");
            if constexpr(sizeof(T) != 1 && MACHINE_ENDIANNESS != ENDIANNESS)
            {
                for(auto& v: out)
                {
                    v = byteswap(v);
                }
            }

            return *this;
        }

        /**
         * Specialization of pop boolean from the package
         * @param in Boolean to populate
         */
        const Package& operator>>(bool& out) const
        {
            uint8_t value;
            *this >> value;

            out = (value == 1);
            return *this;
        }

        //~=============================================================================================================
        // BUFFER OPERATIONS
        //~=============================================================================================================

        /**
         * Query the type' size of header
         * @return Type' size of header (at compile time)
         */
        [[nodiscard]] static constexpr size_t headerSizeof() noexcept { return sizeof(Buffer.header); }

        /**
        * Query the type' size of body size within header
        * @return Type' size of body size within header (at compile time)
        */
        [[nodiscard]] static constexpr size_t bodySizeSizeof() noexcept { return sizeof(Buffer.header.bodySize); }

        /**
         * Query header
         * @return Read-only header reference
         */
        [[nodiscard]] const auto& header() const { return Buffer.header; }

        auto& mutable_header() { return Buffer.header; }

        /**
         * Query buffer size
         * @return Total current bytes used in the buffer to be read
         */
        [[nodiscard]] type_body size() const noexcept { return Buffer.header.bodySize; }

        /**
         * Query of buffer reference
         * @return Writeable buffer
         */
        [[nodiscard]] auto& buffer() { return Buffer.raw_bytes; }

        /**
         * Query of buffer reference
         * @return Read-only buffer
         */
        [[nodiscard]] const auto& buffer() const { return const_cast<Package*>(this)->buffer(); }

        /**
         * Dump all bytes from buffer into a string
         * @param lineWidth Max width of bytes before create new line
         * @param bUseASCIIExtended When dump, values from 128 to 254 will be printed
         * @return String with all bytes dumped
         */
        std::string dumpBuffer(size_t lineWidth = 16, bool bUseASCIIExtended = false) const
        {
            const auto totalSize = static_cast<unsigned>(headerSizeof() + header().bodySize);

            const std::string firstLine = "[" + std::to_string(totalSize) + " bytes]";          // header of output text
            const std::string lineAddress = "\n00000000 | ";                                    // address of 4 bytes in hex

            const auto bytesHex   = lineWidth*3;                                                // space to print bytes in hex format with one space
            const auto bytesChars = lineWidth+1;                                                // space to print bytes as chars with one space between bytes and chars and return char
            const auto line  = lineAddress.size() + bytesHex + bytesChars;                      // space to print one line, including address
            const auto lines = unsigned(totalSize/lineWidth) + (totalSize%lineWidth!=0 ? 1:0);  // total lines to print all bytes in groups of lineWidth bytes

            std::string output(line * lines + firstLine.size(), ' ');
            output.replace(0, firstLine.size(), firstLine);

            for(unsigned i = 0; i < totalSize; i++)
            {
                const auto posBeginAddress = firstLine.size() + line*(i/lineWidth);
                const auto posBeginBytes   = posBeginAddress  + lineAddress.size();
                const auto posBeginChars   = posBeginBytes    + bytesHex;

                // Print line address
                if(i % lineWidth == 0)
                {
                    std::stringstream address;
                    address << std::hex << std::setw(8) << std::setfill('0') << i;

                    std::string thisAddress = lineAddress;
                    thisAddress.replace(1, address.str().size(), address.str());

                    output.replace(posBeginAddress, thisAddress.size(), thisAddress);
                }

                // Print bytes
                std::stringstream byte;
                byte << std::hex << std::setw(2) << std::setfill('0') << +buffer()[i] << " ";
                output.replace(posBeginBytes + 3*(i%lineWidth), byte.str().size(), byte.str());

                // Print byte's character
                if(buffer()[i] >= 32 && buffer()[i] < (bUseASCIIExtended?255:128) )
                {
                    std::stringstream character;
                    character << buffer()[i];
                    output.replace(posBeginChars + i%lineWidth, 1, character.str());
                }
                else output.replace(posBeginChars + i%lineWidth, 1, ".");
            }

            return output;
        }

    private:
        //~=============================================================================================================
        // HANDLE BUFFER
        //~=============================================================================================================

        constexpr void copyToBuffer(const void* source, size_t size)
        {
            std::memcpy(Buffer.raw_bytes.data() + Buffer.header.bodySize + headerSizeof(), source, size);
            Buffer.header.bodySize += static_cast<type_body>(size);
        }
        constexpr void zeroToBuffer()
        {
            *(Buffer.raw_bytes.data() + Buffer.header.bodySize + headerSizeof()) = 0;
            Buffer.header.bodySize += 1;
        }

        //~=============================================================================================================
        // BYTE-SWAP
        //~=============================================================================================================

        static auto byteswap(uint16_t v) noexcept
        {
            return ((v & uint16_t(0x00FF)) << 8) |
                   ((v & uint16_t(0xFF00)) >> 8);
        }
        static auto byteswap(uint32_t v) noexcept
        {
            return ((v & uint32_t(0x0000'00FF)) << 24) |
                   ((v & uint32_t(0x0000'FF00)) <<  8) |
                   ((v & uint32_t(0x00FF'0000)) >>  8) |
                   ((v & uint32_t(0xFF00'0000)) >> 24);
        }
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


        static auto byteswap(int16_t v) noexcept
        {
            return ((v & int16_t(0x00FF)) << 8) |
                   ((v & int16_t(0xFF00)) >> 8);
        }
        static auto byteswap(int32_t v) noexcept
        {
            return ((v & int32_t(0x0000'00FF)) << 24) |
                   ((v & int32_t(0x0000'FF00)) <<  8) |
                   ((v & int32_t(0x00FF'0000)) >>  8) |
                   ((v & int32_t(0xFF00'0000)) >> 24);
        }
        static auto byteswap(int64_t v) noexcept
        {
            return ((v & int64_t(0x0000'0000'0000'00FF)) << 56) |
                   ((v & int64_t(0x0000'0000'0000'FF00)) << 40) |
                   ((v & int64_t(0x0000'0000'00FF'0000)) << 24) |
                   ((v & int64_t(0x0000'0000'FF00'0000)) <<  8) |
                   ((v & int64_t(0x0000'00FF'0000'0000)) >>  8) |
                   ((v & int64_t(0x0000'FF00'0000'0000)) >> 24) |
                   ((v & int64_t(0x00FF'0000'0000'0000)) >> 40) |
                   ((v & int64_t(0xFF00'0000'0000'0000)) >> 56);
        }


        static auto byteswap(float v) noexcept
        {
            float retVal;
            char *floatToConvert = (char*)&v;
            char *returnFloat = (char*)&retVal;

            // swap the bytes into a temporary buffer
            returnFloat[0] = floatToConvert[3];
            returnFloat[1] = floatToConvert[2];
            returnFloat[2] = floatToConvert[1];
            returnFloat[3] = floatToConvert[0];

            return retVal;
        }
        static auto byteswap(double v) noexcept
        {
            double retVal;
            char *doubleToConvert = (char*) & v;
            char *returnDouble = (char*) & retVal;

            // swap the bytes into a temporary buffer
            returnDouble[0] = doubleToConvert[7];
            returnDouble[1] = doubleToConvert[6];
            returnDouble[2] = doubleToConvert[5];
            returnDouble[3] = doubleToConvert[4];
            returnDouble[4] = doubleToConvert[3];
            returnDouble[5] = doubleToConvert[2];
            returnDouble[6] = doubleToConvert[1];
            returnDouble[7] = doubleToConvert[0];

            return retVal;
        }
    };
}

#undef THROW_POP_OVERFLOW
#undef THROW_PUSH_OVERFLOW

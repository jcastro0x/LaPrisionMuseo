# Resources

All game's resources are stored into a unique .bin file as binary array. I'm use the Package.h code I created to ISC (https://github.com/jcastro0x/isc) to do that.

## Layout

__0x0000__ - __8B__: MAGIC NUMBER SIGNATURE (0xFACEFEEDDEADBEEF)
__0x0008__ - __2B__: Version (from 0 to 2^16)
__0x00A0__ - __1B__: ZLIB compression level
__0x00B0__ - __1B__: Endianess (0=Little | 1=Big)
__0x00C0__ - __2B__: Assets in file count
__0x00F0__ - __2B__: Assets in file count


__0x0000__ - __2B__: Compressed size
__0x0002__ - __2B__: Compressed size


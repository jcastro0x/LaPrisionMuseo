# Resources

All game's resources are stored into a unique .bin file as binary array. I'm use the Package.h code I created to ISC
(https://github.com/jcastro0x/isc) to do that.

## Layout

### Main (absolute)
```text
__0x0000__ - __8B__: MAGIC NUMBER SIGNATURE (0xFACEFEEDDEADBEEF)
__0x0008__ - __2B__: Version (from 0 to 2^16)
__0x00A0__ - __1B__: ZLIB compression level (0=None|1=Level1|2=Level2|...)
__0x00B0__ - __1B__: Endianness (0=Little | 1=Big)
__0x00C0__ - __1B__: Hashing algorithm (0=MD5|1=SHA-1|2=SHA-2|3=SHA-3)
__0x00D0__ - __2B__: Assets in file count
__0x00F0__ - __??__: Assets locations
__0x????__ - __??__: Assets data
```

### Asset locations (relative)
```text
__0x0000__ - __8B__: identifier (hash)
__0x0002__ - __8B__: relative jump (asset's location)
```

### Asset data (relative)
```text
__0x0000__ - __8B__: Compressed size
__0x0008__ - __??__: Compressed data
```

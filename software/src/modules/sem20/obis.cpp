#include "obis.h"

#ifndef __bswap32
#define __bswap32(x) \
  ((uint32_t)((((uint32_t)(x) & 0xff000000) >> 24) | \
              (((uint32_t)(x) & 0x00ff0000) >>  8) | \
              (((uint32_t)(x) & 0x0000ff00) <<  8) | \
              (((uint32_t)(x) & 0x000000ff) << 24)))
#endif

#ifndef __bswap64
#define __bswap64(x) \
  ((uint64_t)((((uint64_t)(x) & 0xff00000000000000ULL) >> 56) | \
              (((uint64_t)(x) & 0x00ff000000000000ULL) >> 40) | \
              (((uint64_t)(x) & 0x0000ff0000000000ULL) >> 24) | \
              (((uint64_t)(x) & 0x000000ff00000000ULL) >>  8) | \
              (((uint64_t)(x) & 0x00000000ff000000ULL) <<  8) | \
              (((uint64_t)(x) & 0x0000000000ff0000ULL) << 24) | \
              (((uint64_t)(x) & 0x000000000000ff00ULL) << 40) | \
              (((uint64_t)(x) & 0x00000000000000ffULL) << 56)))
#endif

obis::obis(uint8_t kanal, uint8_t messgroesse, uint8_t messart, uint8_t tarifstufe, double faktor, uint8_t valueSize)
  : _faktor(faktor), _valueSize(valueSize), _position(0)
{
  _obis[0] = kanal;
  _obis[1] = messgroesse;
  _obis[2] = messart;
  _obis[3] = tarifstufe;
}

String obis::name() const
{
  char buf[32];
  snprintf(buf, sizeof(buf)/sizeof(buf[0]), "%u.%u.%u", _obis[1], _obis[2], _obis[3]);

  return buf;
}

double obis::value(uint8_t buf[], size_t bufSize)
{
  if (_position == 0) {
    for (auto pos = 28; pos < bufSize - 4; pos += 4) {
      if (compareObis(buf + pos)) {
        _position = pos + 4;
        break;
      }
    }
  }

  if (_position > 0) {
    switch (_valueSize) {
      case 4:
        return convert_uint32(buf + _position) / _faktor;
      case 8:
        return convert_uint64(buf + _position) / _faktor;
      default:
        break;
    }
  }

  return 0.0;
}

bool obis::compareObis(uint8_t buf[]) const
{
  for (auto pos = 0; pos < 4; pos++) {
    if (buf[pos] != _obis[pos]) {
      return false;
    }
  }

  return true;
}

uint32_t obis::convert_uint32(uint8_t buf[])
{
  uint8_t tmp[4];
  memcpy(tmp, buf, 4);
  return __bswap32(*reinterpret_cast<uint32_t*>(tmp));
}

uint64_t obis::convert_uint64(uint8_t buf[])
{
  uint8_t tmp[8];
  memcpy(tmp, buf, 8);
  return __bswap64(*reinterpret_cast<uint64_t*>(tmp));
}

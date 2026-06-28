#include "blake2b.h"
#include <cstring>

namespace em {

  namespace {

    const std::uint64_t kBlake2bIV[8] = {
      0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
      0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
      0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
      0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
    };

    const std::uint8_t kBlake2bSigma[12][16] = {
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
      {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
      {11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4},
      {7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8},
      {9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13},
      {2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9},
      {12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11},
      {13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10},
      {6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5},
      {10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0},
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
      {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3}
    };

    std::uint64_t Rotr64(std::uint64_t w, unsigned c) {
      return (w >> c) | (w << (64 - c));
    }

    struct Blake2bState {
      std::uint64_t h[8];
      std::uint64_t t[2];
      std::uint64_t f[2];
      std::uint8_t buf[128];
      std::size_t buflen;
      std::size_t outlen;
    };

    void Blake2bCompress(Blake2bState* s, const std::uint8_t block[128]) {
      std::uint64_t m[16];
      std::uint64_t v[16];

      for (int i = 0; i < 16; ++i) {
        m[i] = 0;
        for (int j = 0; j < 8; ++j) {
          m[i] |= static_cast<std::uint64_t>(block[i * 8 + j]) << (8 * j);
        }
      }

      for (int i = 0; i < 8; ++i) {
        v[i] = s->h[i];
      }
      v[8] = kBlake2bIV[0];
      v[9] = kBlake2bIV[1];
      v[10] = kBlake2bIV[2];
      v[11] = kBlake2bIV[3];
      v[12] = kBlake2bIV[4] ^ s->t[0];
      v[13] = kBlake2bIV[5] ^ s->t[1];
      v[14] = kBlake2bIV[6] ^ s->f[0];
      v[15] = kBlake2bIV[7] ^ s->f[1];

#define G(r, i, a, b, c, d) \
      do { \
        a += b + m[kBlake2bSigma[r][2 * i + 0]]; \
        d = Rotr64(d ^ a, 32); \
        c += d; \
        b = Rotr64(b ^ c, 24); \
        a += b + m[kBlake2bSigma[r][2 * i + 1]]; \
        d = Rotr64(d ^ a, 16); \
        c += d; \
        b = Rotr64(b ^ c, 63); \
      } while (0)

      for (int r = 0; r < 12; ++r) {
        G(r, 0, v[0], v[4], v[8], v[12]);
        G(r, 1, v[1], v[5], v[9], v[13]);
        G(r, 2, v[2], v[6], v[10], v[14]);
        G(r, 3, v[3], v[7], v[11], v[15]);
        G(r, 4, v[0], v[5], v[10], v[15]);
        G(r, 5, v[1], v[6], v[11], v[12]);
        G(r, 6, v[2], v[7], v[8], v[13]);
        G(r, 7, v[3], v[4], v[9], v[14]);
      }

#undef G

      for (int i = 0; i < 8; ++i) {
        s->h[i] = s->h[i] ^ v[i] ^ v[i + 8];
      }
    }

    void Blake2bInit(Blake2bState* s, std::size_t outlen) {
      std::memset(s, 0, sizeof(Blake2bState));
      for (int i = 0; i < 8; ++i) {
        s->h[i] = kBlake2bIV[i];
      }
      s->h[0] ^= 0x01010000 ^ outlen;
      s->outlen = outlen;
    }

    void Blake2bUpdate(
      Blake2bState* s,
      const std::uint8_t* in,
      std::size_t inlen) {
      while (inlen > 0) {
        std::size_t left = s->buflen;
        std::size_t fill = 128 - left;
        if (inlen > fill) {
          std::memcpy(s->buf + left, in, fill);
          s->t[0] += 128;
          if (s->t[0] < 128) {
            s->t[1]++;
          }
          Blake2bCompress(s, s->buf);
          s->buflen = 0;
          in += fill;
          inlen -= fill;
        } else {
          std::memcpy(s->buf + left, in, inlen);
          s->buflen += inlen;
          inlen = 0;
        }
      }
    }

    void Blake2bFinal(Blake2bState* s, std::uint8_t* out) {
      s->t[0] += s->buflen;
      if (s->t[0] < s->buflen) {
        s->t[1]++;
      }
      s->f[0] = static_cast<std::uint64_t>(-1);
      std::memset(s->buf + s->buflen, 0, 128 - s->buflen);
      Blake2bCompress(s, s->buf);

      std::uint8_t buffer[64];
      for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
          buffer[i * 8 + j] = (s->h[i] >> (8 * j)) & 0xFF;
        }
      }
      std::memcpy(out, buffer, s->outlen);
    }

  }

  std::size_t Blake2bHash::operator()(const std::string& key) const {
    Blake2bState state;
    Blake2bInit(&state, 8);
    Blake2bUpdate(&state, reinterpret_cast<const std::uint8_t*>(key.data()),
                  key.size());
    std::uint8_t out[8];
    Blake2bFinal(&state, out);

    std::size_t result = 0;
    for (int i = 0; i < 8; ++i) {
      result |= static_cast<std::size_t>(out[i]) << (8 * i);
    }
    return result;
  }

  std::size_t Blake2bPairHash::operator()(
    const std::pair<std::string, std::string>& key) const {
    Blake2bState state;
    Blake2bInit(&state, 8);
    Blake2bUpdate(&state,
                  reinterpret_cast<const std::uint8_t*>(key.first.data()),
                  key.first.size());
    Blake2bUpdate(&state,
                  reinterpret_cast<const std::uint8_t*>(key.second.data()),
                  key.second.size());
    std::uint8_t out[8];
    Blake2bFinal(&state, out);

    std::size_t result = 0;
    for (int i = 0; i < 8; ++i) {
      result |= static_cast<std::size_t>(out[i]) << (8 * i);
    }
    return result;
  }

  bool StringEqual::operator()(
    const std::string& a,
    const std::string& b) const {
    return a == b;
  }

  bool PairEqual::operator()(
    const std::pair<std::string, std::string>& a,
    const std::pair<std::string, std::string>& b) const {
    return a.first == b.first && a.second == b.second;
  }

}

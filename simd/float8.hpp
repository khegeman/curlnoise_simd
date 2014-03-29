#pragma once 
#include <x86intrin.h> 

struct float8 {

    union { __m256 m256; float v[8]; };
    
    
    float8(const float v0, const float v1, 
                        const float v2, const float v3,
                        const float v4, const float v5,
                        const float v6, const float v7
                       )
    : m256(_mm256_set_ps(v7,v6,v5,v4,v3, v2, v1, v0)) {
        // empty
    }
    
    float8(const float v) : m256(_mm256_set1_ps(v)) { 
        // empty
    }
    
    // Copy constructor
    float8(const float8& a) : m256(a.m256) {
        // empty
    }
    
    float8(const __m256 a) : m256(a) {
        // empty
    }

    // Assignment
    float8& operator=(const float8 a) {
        if(&a != this) {
            m256 = a.m256;
        }
        return *this;
    }
        
    float operator[](std::size_t i) const {
        return v[i];
    }
    
    float& operator[](std::size_t i) {
        return v[i];
    }
    
    /// arithmetic vector operators
    //@{
    float8& operator+=(const float8 a) {
        m256 = _mm256_add_ps(m256, a.m256);
        return *this;
    }
    
    float8& operator+=(const float a) {
        m256 = _mm256_add_ps(m256, _mm256_set1_ps(a));
        return *this;
    }
    
    float8& operator-=(const float8 a) {
        m256 = _mm256_sub_ps(m256, a.m256);
        return *this;
    }
    
    float8& operator-=(const float a) {
        m256 = _mm256_sub_ps(m256, _mm256_set1_ps(a));
        return *this;
    }
    
    float8& operator*=(const float8 a) {
        m256 = _mm256_mul_ps(m256, a.m256);
        return *this;
    }
    
    float8& operator*=(const float a) {
        m256 = _mm256_mul_ps(m256, _mm256_set1_ps(a));
        return *this;
    }
    
    float8& operator/=(const float8 a) {
        m256 = _mm256_div_ps(m256, a.m256);
        return *this;
    }
    
    float8& operator/=(const float a) {
        m256 = _mm256_div_ps(m256, _mm256_set1_ps(a));
        return *this;
    }

}; // class float8

inline const float8 abs(const float8 a) {
    const __m256 mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff));
    return _mm256_and_ps(a.m256, mask);
}
inline const float8 floor(const float8 a) { 
    return _mm256_floor_ps(a.m256);
}
#if 0
inline const float reduce_add(const float8 v) {

  float8 ones(1.0f);
  float8 r = _mm256_dp_ps(v, ones, 0xf1);
  r = r + _mm256_permute2f128_ps(r.m256, r.m256, 0x81);
  return r.v[0];
}
#endif
inline const float8 sqrt (const float8 a) { 
    return _mm256_sqrt_ps(a.m256); 
}

inline const float8 sqr  (const float8 a) { 
    return _mm256_mul_ps(a.m256, a.m256); 
}
#if 0
inline const float dist (const float8 a, const float8 b) {
    float8 v = a - b;
    float dist2 = reduce_add(sqr(v));
    return sqrt(dist2);
}
#endif
inline const float8 operator+(const float8 a, const float8 b) {
    return _mm256_add_ps(a.m256, b.m256);
}

inline const float8 operator+(const float8 a, const float b) {
    return a + float8(b);
}

inline const float8 operator+(const float a, const float8 b) {
    return float8(a) + b;
}

inline const float8 operator-(const float8 a, const float8 b) {
    return _mm256_sub_ps(a.m256, b.m256);
}

inline const float8 operator-(const float8 a, const float b) {
    return a - float8(b);
}
    
inline const float8 operator-(const float a, const float8 b) {
    return float8(a) - b;
}

inline const float8 operator*(const float8 a, const float8 b) {
    return _mm256_mul_ps(a.m256, b.m256);
}

inline const float8 operator*(const float8 a, const float b) {
    return a * float8(b);
}

inline const float8 operator*(const float a, const float8 b) {
    return float8(a) * b;
}

inline const float8 operator/(const float8 a, const float8 b) {
    return _mm256_div_ps(a.m256, b.m256);
}

inline const float8 operator/(const float8 a, const float b) {
    return a / float8(b);
}

inline const float8 operator/(const float a, const float8 b) {
    return float8(a) / b;
}

inline const float8 min(const float8 a, const float8 b ) { 
    return _mm256_min_ps(a.m256, b.m256); 
}

inline const float8 max(const float8 a, const float8 b ) { 
    return _mm256_max_ps(a.m256, b.m256); 
}

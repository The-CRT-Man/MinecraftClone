#include "perlin.hpp"

#include <math.h>

float Perlin::getPerlin(float x, float y) {
    // Determine grid cell coordinates
    int x0 = (int)floor(x);
    int x1 = x0 + 1;
    int y0 = (int)floor(y);
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return value;
}

float Perlin::interpolate(float a0, float a1, float w) {
    return (a1 - a0) * w + a0;
}

vector2 Perlin::randomGradient(int ix, int iy) {
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;

    unsigned long aSeed = seed * 1.05f;
    unsigned long bSeed = seed * 0.85f;
    unsigned long cSeed = seed * 1.15f;
    
    unsigned a = ix, b = iy;

    a *= aSeed; b ^= a << s | a >> (w - s);
    b *= bSeed; a ^= b << s | b >> (w - s);
    a *= cSeed;

    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    vector2 v = {0, 0};
    v.x = cos(random); v.y = sin(random);
    return v;
}

float Perlin::dotGridGradient(int ix, int iy, float x, float y) {
    // Get gradient from integer coordinates
    vector2 gradient = randomGradient(ix, iy);

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx * gradient.x + dy * gradient.y);
}

#include "font.h"

// Basic 8x8 font lookup table for ASCII characters 32 to 127
const uint8_t font_8x8[128 * 8] = {
    // Character 32: Space
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // (A simplified subset or placeholder pattern for chars. We'll fill letters A-Z, 0-9)
    // For brevity and clean rendering, let's provide a compact generator/lookup or basic font data.
    // Let's populate standard ASCII printable range with simple fallback block shapes if unmapped, 
    // and distinct patterns for letters/numbers.
};
// We will implement a procedural or robust bitmap font helper below.

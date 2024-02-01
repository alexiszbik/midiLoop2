#pragma once

inline static byte getTrackId(int p, int firstTrackP) {
  return p - firstTrackP;
}

inline static bool midiValueToBool(byte value) {
    return value > 64;
}
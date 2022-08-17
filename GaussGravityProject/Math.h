#pragma once

#ifdef ARDUINO
    #include "Arduino.h"
#else
    #include <cmath>
    #include <algorithm>

    using namespace  std;

    template<class T>
    T sq(T value) {
            return value * value;
    }

#endif

#ifdef FLOAT_AS_GEOMTYPE
    typedef float GeomType;
#elif defined (LONG_DOUBLE_AS_GEOMTYPE)
    typedef long double GeomType;
#else
    typedef double GeomType;
#endif

namespace Bismath {
    class Vector2d;
    class Point;

    /**Returns true if distance beetwen v and o is less than or equal to eps*/
    bool inArea(Vector2d v, Vector2d o, GeomType eps);
    bool inRange(Vector2d v, GeomType eps);

    template<typename T>
    T sign(T x) {
        return (x > 0) - (x < 0);
    }
}

#include "Vector2d.h"
#include "Math.h"

Bismath::Vector2d Bismath::Vector2d::operator+ (const Vector2d& v) const {
    return Vector2d(x + v.x, y + v.y);
}

Bismath::Vector2d Bismath::Vector2d::operator- (const Vector2d& v) const {
    return Vector2d(x - v.x, y - v.y);
}

Bismath::Vector2d Bismath::Vector2d::operator* (const GeomType& k) const {
    return Vector2d(k * x, k * y);
}

Bismath::Vector2d Bismath::Vector2d::operator/ (const GeomType& k) const {
    return Vector2d(x / k, y / k);
}

bool Bismath::Vector2d::operator== (const Vector2d& v) const {
    return x == v.x && y == v.y;
}

bool Bismath::Vector2d::operator!= (const Vector2d& v) const {
    return x != v.x || y != v.y;
}

Bismath::Vector2d Bismath::Vector2d::operator+= (const Vector2d& v) {
    x += v.x;
    y += v.y;
    return *this;
}

Bismath::Vector2d Bismath::Vector2d::operator-= (const Vector2d& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

Bismath::Vector2d Bismath::Vector2d::operator*= (const GeomType& k) {
    x *= k;
    y *= k;
    return *this;
}

Bismath::Vector2d Bismath::Vector2d::operator/= (const GeomType& k) {
    x /= k;
    y /= k;
    return *this;
}

Bismath::Vector2d Bismath::Vector2d::normalize() const {
    return *this / length();
}

GeomType Bismath::Vector2d::length() const {
    GeomType a = min(x, y);
    GeomType b = max(x, y);
    long double r = (long double)a / b;

    return (long double)b * sqrt(1 + r * r);
}

GeomType Bismath::Vector2d::sqLength() const {
    return x * x + y * y;
}

GeomType Bismath::Vector2d::dot(const Vector2d& v) const {
    return x * v.x + y * v.y;
}

GeomType Bismath::Vector2d::cross(const Vector2d& v) const {
    return x * v.y - y * v.x;
}

double Bismath::Vector2d::angle(const Vector2d& v) const {
    return atan2(cross(v), dot(v));
}

double Bismath::Vector2d::angle() const {
    return atan2(double(y), double(x));
}

Bismath::Vector2d Bismath::Vector2d::rotated(double phi) const {
    auto cCos = std::cos(phi), cSin = std::sin(phi);
    return {x * cCos - y * cSin, x * cSin + y * cCos};
}

Bismath::Vector2d& Bismath::Vector2d::rotate(double phi) {
    auto v = rotated(phi);
    x = v.x;
    y = v.y;
    return *this;
}

GeomType Bismath::dot(const Vector2d& a, const Vector2d& b) {
    return a.dot(b);
}

GeomType Bismath::cross(const Vector2d& a, const Vector2d& b) {
    return a.cross(b);
}

GeomType abs(const Bismath::Vector2d& vector) {
    return vector.length();
}

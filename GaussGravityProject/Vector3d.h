#pragma once

#include <cmath>
#include <algorithm>
#include <QtMath>

template <typename T>
struct Vector3d {
public:
    T x;
    T y;
    T z;

    Vector3d() : x(T()), y(T()), z(T()) {}
    Vector3d(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {}
    // Vector3d(const Vector3d& copied) : x(copied.x), y(copied.y), z(copied.z) {}

    Vector3d operator+ (const Vector3d&) const;
    Vector3d operator- (const Vector3d&) const;
    Vector3d operator* (const T&) const;
    Vector3d operator/ (const T&) const;
    bool operator== (const Vector3d&) const;
    bool operator!= (const Vector3d&) const;
    Vector3d operator+= (const Vector3d&);
    Vector3d operator-= (const Vector3d&);
    Vector3d operator*= (const T&);
    Vector3d operator/= (const T&);

    Vector3d normalized() const;
    long double length() const;
    T sqLength() const;
    /***|this| * |v| * cos(this^v)*/
    T dot(const Vector3d& v) const;
    /***|this| * |v| * sin(this^v)*/
    Vector3d<T> cross(const Vector3d& vector) const;
    long double angle(const Vector3d& vector) const;
    /**Angle between vector and OX axis*/
    // long double angle() const;

    long double projection(const Vector3d&) const;
    Vector3d<T> component(const Vector3d&) const;
};

template <typename T>
Vector3d<T> Vector3d<T>::operator+ (const Vector3d<T>& v) const {
    return Vector3d<T>(x + v.x, y + v.y, z + v.z);
}

template <typename T>
Vector3d<T> Vector3d<T>::operator- (const Vector3d<T>& v) const {
    return Vector3d<T>(x - v.x, y - v.y, z - v.z);
}

template <typename T>
Vector3d<T> Vector3d<T>::operator* (const T& k) const {
    return Vector3d<T>(k * x, k * y, k * z);
}

template <typename T>
Vector3d<T> Vector3d<T>::operator/ (const T& k) const {
    return Vector3d<T>(x / k, y / k, z / k);
}

template <typename T>
bool Vector3d<T>::operator== (const Vector3d<T>& v) const {
    return x == v.x && y == v.y && z == v.z;
}

template <typename T>
bool Vector3d<T>::operator!= (const Vector3d<T>& v) const {
    return x != v.x || y != v.y || z != v.z;
}

template <typename T>
Vector3d<T> Vector3d<T>::operator+= (const Vector3d<T>& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

template <typename T>
Vector3d<T> Vector3d<T>::operator-= (const Vector3d<T>& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

template <typename T>
Vector3d<T> Vector3d<T>::operator*= (const T& k) {
    x *= k;
    y *= k;
    z *= k;
    return *this;
}

template <typename T>
Vector3d<T> Vector3d<T>::operator/= (const T& k) {
    x /= k;
    y /= k;
    z /= k;
    return *this;
}

template <typename T>
Vector3d<T> Vector3d<T>::normalized() const {
    return *this / length();
}

template <typename T>
long double Vector3d<T>::length() const {
    T a = std::min(std::abs(x), std::abs(y));
    T b = std::max(std::abs(x), std::abs(y));
    T c = std::min(b, std::abs(z));
    b = std::max(b, std::abs(z));

    long double ra = (long double)a / b;
    long double rc = (long double)c / b;

    return (long double)b * sqrt(1 + ra * ra + rc * rc);
}

template <typename T>
T Vector3d<T>::sqLength() const {
    return x * x + y * y + z * z;
}

template <typename T>
T Vector3d<T>::dot(const Vector3d<T>& v) const {
    return x * v.x + y * v.y + z * v.z;
}

template <typename T>
Vector3d<T> Vector3d<T>::cross(const Vector3d<T>& v) const {
    return Vector3d<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

template <typename T>
long double Vector3d<T>::projection(const Vector3d<T>& d) const {
    return dot(d) / d.length();
}

template <typename T>
Vector3d<T> Vector3d<T>::component(const Vector3d<T>& d) const {
    long double l = d.length();
    return d * (dot(d) / l / l);
}

template <typename T>
long double Vector3d<T>::angle(const Vector3d<T>& v) const {
    /* if (dot(v)) return atan2l(cross(v).length(), dot(v));  // doesn't work with coperpendicular vectors
       return asin(cross(v).length() / length() / v.length()); */
    return acos(dot(v) / length() / v.length());
}

template <typename T>
Vector3d<T> cross(const Vector3d<T>& v, const Vector3d<T>& u) {
    return v.cross(u);
}

/*
template <typename T>
double Vector3d<T>::angle() const {
    return atan2(double(y), double(x));
}
*/

#pragma once
#include "Math.h"

namespace Bismath {

    class Vector2d {
    public:
        GeomType x;
        GeomType y;

        Vector2d() : x(0), y(0) {}
        Vector2d(const GeomType& _x, const GeomType& _y) : x(_x), y(_y) {}
        Vector2d(const Vector2d& v) : x(v.x), y(v.y) {}

        Vector2d operator+ (const Vector2d& v) const;
        Vector2d operator- (const Vector2d& v) const;
        Vector2d operator* (const GeomType& k) const;
        Vector2d operator/ (const GeomType& k) const;
        bool operator== (const Vector2d& v) const;
        bool operator!= (const Vector2d& v) const;
        Vector2d operator+= (const Vector2d& v);
        Vector2d operator-= (const Vector2d& v);
        Vector2d operator*= (const GeomType& k);
        Vector2d operator/= (const GeomType& k);

        Vector2d normalize() const;
        GeomType length() const;
        GeomType sqLength() const;
        /**|this| * |v| * cos(this^v)*/
        GeomType dot(const Vector2d& v) const;
        /**|this| * |v| * sin(this^v)*/
        GeomType cross(const Vector2d& v) const;
        double angle(const Vector2d& vector) const;
        /**Angle between vector and OX axis*/
        double angle() const;

        /// Return rotated counterclockwise vector
        Vector2d rotated(double angleInRadians) const;
        /// Rotates vector counterclockwise
        Vector2d& rotate(double angleInRadians);
    };

    GeomType dot(const Vector2d& a, const Vector2d& b);
    GeomType cross(const Vector2d& a, const Vector2d& b);

}

GeomType abs(const Bismath::Vector2d& vector);

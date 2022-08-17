#pragma once

#include "Vector3d.h"

template <typename T>
struct NormalizedVector3d : public Vector3d<T> {
public:
    NormalizedVector3d(const NormalizedVector3d<T>& copied) : Vector3d<T>(copied) {}
    explicit NormalizedVector3d(const Vector3d<T>& toNormalized) : Vector3d<T>(toNormalized.normalized()) {}

    /** One of types could be nullptr */
    template <typename A, typename B, typename C>
    NormalizedVector3d(A x, B y, C z) { set(x, y, z); }

    template <typename A, typename B, typename C>
    void set(A x, B y, C z);

    Vector3d<T> normalized() override { return Vector3d<T>(this->x, this->y, this->z); }
};
/*
template <typename T>
template <>
void NormalizedVector3d<T>::set<T, T, decltype(NULL)>(T x, T y, decltype(NULL) z) {
    // Call an error;
}

template <typename T>
template <typename A, typename B, typename C>
void NormalizedVector3d<T>::set(A x, B y, C z) {
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(z);
    // Call an error;
}
*/

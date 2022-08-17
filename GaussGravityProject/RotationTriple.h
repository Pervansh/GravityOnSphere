#pragma once

#include "QtDebug"
#include "Vector3d.h"
#include <cmath>

template <typename T>
class RotationTriple {
private:
    Vector3d<T> pointer;  // pointing to rotated point, represent i
    Vector3d<T> tangent;  // collinear to velocity vector of rotated point, represent j
    Vector3d<T> guide;    // vector multiplication of pointer and tangent vectors, represent k

protected:
    RotationTriple(const Vector3d<T>& newPointer,
                   const Vector3d<T>& newTangent,
                   const Vector3d<T>& newGuide);

public:
    // RotationTriple(const RotationTriple<T>& copied);
    RotationTriple(const Vector3d<T>& pointerDirection, const Vector3d<T>& rotationDirection);

    void set(const Vector3d<T>& pointerDirection, const Vector3d<T>& rotationDirection);
    void reset();

    /**
     * Returns a pointer rotated counterclockwise relative to the guide,
     * enlarged to the specified length
     */
    Vector3d<T>       rotate(double angle, T lenght = 1) const;
    /// Returns rotated pointer and tangent counterclockwise relative to the guide
    RotationTriple<T> rotatedTriple(double angle)        const;

    /// Rotates the pointer and tangent counterclockwise relative to the guide
    void rotateTriple(double angle);

    /**
     * Returns a new RotationTriple<T> where
     * current pointer becomes tangent,
     * current tangent - guide,
     * current guide - pointer
     */
    RotationTriple<T> nextCyclicShift()     const;
    /**
     * Returns a new RotationTriple<T> where
     * current pointer becomes guide,
     * current tangent - pointer,
     * current guide - tangent
     */
    RotationTriple<T> previousCyclicShift() const;

    void debug();

    const Vector3d<T>& getPointer() const { return pointer; }
    const Vector3d<T>& getTangent() const { return tangent; }
    const Vector3d<T>& getGuide()   const { return guide;   }

};

template <typename T>
RotationTriple<T>::RotationTriple(const Vector3d<T>& newPointer,
                                  const Vector3d<T>& newTangent,
                                  const Vector3d<T>& newGuide)
    : pointer(newPointer), tangent(newTangent), guide(newGuide) {}

/*
template <typename T>
RotationTriple<T>::RotationTriple(const RotationTriple<T>& copied)
    : pointer(copied.pointer), tangent(copied.tangent), guide(copied.guide) {}
*/

template <typename T>
RotationTriple<T>::RotationTriple(const Vector3d<T>& pointerDirection,
                                  const Vector3d<T>& rotationDirection) {
    set(pointerDirection, rotationDirection);
}

template <typename T>
void RotationTriple<T>::set(const Vector3d<T>& pointerDirection,
                            const Vector3d<T>& rotationDirection) {
    pointer = pointerDirection.normalized();
    tangent = rotationDirection - rotationDirection.component(pointerDirection);

    if (tangent.length() == 0) {
        qFatal("rotationDirection should not be proportional to pointerDirection");
    }

    tangent = tangent.normalized();
    guide = pointer.cross(tangent);
}

template <typename T>
void RotationTriple<T>::reset() {
    set(pointer, tangent);
}

template <typename T>
Vector3d<T> RotationTriple<T>::rotate(double angle, T lenght) const {
    T cosine = (T)std::cos(angle);
    T sine = (T)std::sin(angle);

    Vector3d<T> rotatedPointer;

    rotatedPointer.x = pointer.x * cosine + tangent.x * sine;
    rotatedPointer.y = pointer.y * cosine + tangent.y * sine;
    rotatedPointer.z = pointer.z * cosine + tangent.z * sine;

    return rotatedPointer * lenght;
}

template <typename T>
void RotationTriple<T>::rotateTriple(double angle) {
    T cosine = (T)std::cos(angle);
    T sine = (T)std::sin(angle);

    Vector3d<T> newPointer, newTangent;

    newPointer.x = pointer.x * cosine + tangent.x * sine;
    newPointer.y = pointer.y * cosine + tangent.y * sine;
    newPointer.z = pointer.z * cosine + tangent.z * sine;

    newTangent.x = tangent.x * cosine - pointer.x * sine;
    newTangent.y = tangent.y * cosine - pointer.y * sine;
    newTangent.z = tangent.z * cosine - pointer.z * sine;

    pointer = newPointer;
    tangent = newTangent;
}

template <typename T>
RotationTriple<T> RotationTriple<T>::rotatedTriple(double angle) const {
    T cosine = (T)std::cos(angle);
    T sine = (T)std::sin(angle);

    Vector3d<T> newPointer, newTangent;

    newPointer.x = pointer.x * cosine + tangent.x * sine;
    newPointer.y = pointer.y * cosine + tangent.y * sine;
    newPointer.z = pointer.z * cosine + tangent.z * sine;

    newTangent.x = tangent.x * cosine - pointer.x * sine;
    newTangent.y = tangent.y * cosine - pointer.y * sine;
    newTangent.z = tangent.z * cosine - pointer.z * sine;

    return RotationTriple<T>(newPointer, newTangent, guide);
}

template <typename T>
RotationTriple<T> RotationTriple<T>::nextCyclicShift() const {
    return RotationTriple<T>(guide, pointer, tangent);
}

template <typename T>
RotationTriple<T> RotationTriple<T>::previousCyclicShift() const {
    return RotationTriple<T>(tangent, guide, pointer);
}
template <typename T>
void RotationTriple<T>::debug() {
    qDebug() << "Pointer: { " << pointer.x << ", "
                              << pointer.y << ", "
                              << pointer.z << "}";

    qDebug() << "Tangent: { " << tangent.x << ", "
                              << tangent.y << ", "
                              << tangent.z << "}";

    qDebug() << "Guide: { " << guide.x << ", "
                            << guide.y << ", "
                            << guide.z << "}";
}

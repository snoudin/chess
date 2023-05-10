//
// Created by vitali on 12.03.23.
//
#include "gvector.h"

using namespace geometry;

Vector& Vector::operator+=(const Vector& v) {
    x += v.x;
    y += v.y;
    return *this;
}

Vector& Vector::operator-=(const Vector& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector& Vector::operator*=(value_type scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector Vector::operator-() const {
    Vector v(-x, -y);
    return v;
}

value_type geometry::cross_product(const Vector& a, const Vector& b) {
    return a.x * b.y - a.y * b.x;
}

value_type geometry::dot_product(const Vector& a, const Vector& b) {
    return a.x * b.x + a.y * b.y;
}

bool Vector::onLine(const Point& p, const Vector& v) const {
    return cross_product(*this - p, v) == 0;
}

bool Vector::onRay(const Point& p, const Vector& v) const {
    if (!onLine(p, v)) return false;
    return dot_product(*this - p, v) >= 0;
}

Vector geometry::operator-(const Vector& a, const Vector& b) {
    Vector copy = a;
    copy -= b;
    return copy;
}

Vector geometry::operator+(const Vector& a, const Vector& b) {
    Vector copy = a;
    copy += b;
    return copy;
}

Vector geometry::operator*(const Vector& v, value_type scalar) {
    Vector copy = v;
    copy *= scalar;
    return copy;
}

bool Vector::operator==(const Vector& v) const {
    return x == v.x && y == v.y;
}
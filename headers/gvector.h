//
// Created by vitali on 12.03.23.
//
#pragma once

#include <algorithm>

namespace geometry {
    using value_type = int;
    const value_type board_size = 8;

    class Vector {
        value_type x = 0, y = 0;

      public:
        Vector(value_type x, value_type y) : x(x), y(y) {}

        Vector() = default;

        Vector& operator+=(const Vector&);

        Vector& operator-=(const Vector&);

        Vector& operator*=(value_type);

        Vector operator-() const;

        bool operator==(const Vector&) const;

        bool onLine(const Vector&, const Vector&) const;
//    is Point (*this) on line associated with Point and vector

        bool onRay(const Vector&, const Vector&) const;
//            same as func before, but for ray

        inline bool outOfBounds() const { return x < 0 || x >= 8 || y < 0 || y >= 8; }

        inline value_type len() const { return std::abs(x) + std::abs(y); }

        inline value_type get_x() const { return x; }

        inline value_type get_y() const { return y; }

        friend value_type dot_product(const Vector&, const Vector&);

        friend value_type cross_product(const Vector&, const Vector&);
    };

    using Point = Vector;

// are 2 Vectors collinear
// 0 vector is collinear with any vector
    Vector operator+(const Vector&, const Vector&);

    Vector operator-(const Vector&, const Vector&);

    Vector operator*(const Vector&, value_type);
}  // namespace geometry

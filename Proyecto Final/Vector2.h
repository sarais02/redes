#pragma once

#include <iostream>
class Vector2{
    public:
    Vector2();
    Vector2(double t_x, double t_y);
    ~Vector2() = default;
    void setVector2(double t_x, double t_y);

    double getX();
    double getY();

    void operator+=(const Vector2& other);
    void operator-=(const Vector2& other);

    void operator*=(float v);
    void operator/=(float v);

    bool operator==(const Vector2& other);
    bool operator!=(const Vector2& other);

    Vector2 operator+(const Vector2& other);
    Vector2 operator-(const Vector2& other);
    Vector2 operator*(const float& v);
    Vector2 operator/(const float& v);

    friend std::ostream& operator<<(std::ostream& out, const Vector2& v);
    friend std::istream& operator>>(std::istream& in, Vector2& v);

    double magnitude();
    double sqrMagnitude();
    Vector2 normalized();
    void normalize();
    private:
    double m_x;
    double m_y;
};
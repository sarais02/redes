#include "Vector2.h"
#include <math.h>

    Vector2::Vector2()
        : m_x(0)
        , m_y(0)
    {
    }

    Vector2::Vector2(double t_x, double t_y)
        : m_x(t_x)
        , m_y(t_y)
    {
    }

    void Vector2::setVector2(double t_x, double t_y)
    {
        m_x = t_x;
        m_y = t_y;
    }

    double Vector2::getX() 
    {
        return m_x;
    }

    double Vector2::getY() 
    {
        return m_y;
    }

    void Vector2::operator+=(const Vector2& other)
    {
        m_x += other.m_x;
        m_y += other.m_y;
    }

    void Vector2::operator-=(const Vector2& other)
    {
        m_x -= other.m_x;
        m_y -= other.m_y;
    }

    void Vector2::operator*=(float v)
    {
        m_x *= v;
        m_y *= v;
    }

    void Vector2::operator/=(float v)
    {
        m_x /= v;
        m_y /= v;
    }

    bool Vector2::operator==(const Vector2& other)
    {
        return m_x == other.m_x && m_y == other.m_y;
    }

    bool Vector2::operator!=(const Vector2& other)
    {
        return m_x != other.m_x || m_y != other.m_y;
    }

    Vector2 Vector2::operator+(const Vector2& other)
    {
        return Vector2(m_x + other.m_x, m_y + other.m_y);
    }

    Vector2 Vector2::operator-(const Vector2& other)
    {
        return Vector2(m_x - other.m_x, m_y - other.m_y);
    }

    Vector2 Vector2::operator*(const float& v)
    {
        return Vector2(m_x * v, m_y * v);
    }

    Vector2 Vector2::operator/(const float& v)
    {
        return Vector2(m_x / v, m_y / v);
    }

    std::ostream& operator<<(std::ostream& out, const Vector2& v)
    {
        out << v.m_x << " "
            << v.m_y << " "
            << std::endl;
        return out;
    }

    std::istream& operator>>(std::istream& in, Vector2& v)
    {
        in >> v.m_x;
        in >> v.m_y;
        return in;
    }

    double Vector2::magnitude()
    {
        return sqrtf((m_x * m_x) + (m_y * m_y));
    }

    double Vector2::sqrMagnitude()
    {
        return (m_x * m_x) + (m_y * m_y);
    }

    Vector2 Vector2::normalized()
    {
        auto v = magnitude();

        if (v == 0)
            return Vector2(m_x, m_y);
        else
            return Vector2(m_x / v, m_y / v);
    }

    void Vector2::normalize()
    {
        auto v = magnitude();
        if (v != 0)
        {
            m_x /= v;
            m_y /= v;
        }
    }

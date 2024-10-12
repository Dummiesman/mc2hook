#pragma once

struct Vector3
{
public:
    float X, Y, Z;
public:
    const static Vector3 ORIGIN;
    const static Vector3 XAXIS;
    const static Vector3 YAXIS;
    const static Vector3 ZAXIS;
public:
    Vector3();
    Vector3(float x, float y, float z);

    float InvMag(void) const;
    float Mag(void) const;
    float Mag2(void) const;
    void Set(float x, float y, float z);
    void Set(const Vector3& vec);
    void Add(const Vector3& vec);
    void AddScaled(const Vector3& vec, float scale);
    void Subtract(const Vector3& vec);
    void SubtractScaled(const Vector3& vec, float scale);
    void Scale(float scale);
    void InvScale(float scale);
    void RotateX(float angle); // Rotate the X axis of this vector by angle degrees (in radians)
    void RotateY(float angle); // Rotate the Y axis of this vector by angle degrees (in radians)
    void RotateZ(float angle); // Rotate the Z axis of this vector by angle degrees (in radians)
    void RotateAboutAxis(float angle, int axis);
    float Angle(const Vector3& vec) const;
    float FastAngle(const Vector3& vec) const;
    float Dist(const Vector3& vec) const;
    float FlatDist(const Vector3& vec) const;
    float Dot(const Vector3& vec) const;
    void Lerp(float t, const Vector3& vec1, const Vector3& vec2);
    void Negate();
    void Negate(const Vector3& vec);
    void Normalize();
    void Min(const Vector3& vec1, const Vector3& vec2);
    void Max(const Vector3& vec1, const Vector3& vec2);
    bool IsEqual(const Vector3& vec) const;
    Vector3 Multiply(const Vector3& vec) const;
    void Print() const;
    void Print(const char* caption);
    void operator+=(const Vector3& vec);
    void operator-=(const Vector3& vec);
    void operator+=(Vector3& vec);
    void operator-=(Vector3& vec);
    Vector3 operator+(const Vector3& vec) const;
    Vector3 operator-(const Vector3& vec) const;
    Vector3 operator/(float value) const;
    Vector3 operator*(float value) const;
};
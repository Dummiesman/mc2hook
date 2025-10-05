#pragma once
#include "vector3.h"
#include "vector4.h"
#include "matrix34.h"

class Matrix34;

class Matrix44 
{
public:
    float m00;
    float m01;
    float m02;
    float m03;

    float m10;
    float m11;
    float m12;
    float m13;

    float m20;
    float m21;
    float m22;
    float m23;

    float m30;
    float m31;
    float m32;
    float m33;
public:
    const static Matrix44 I;
public:
    Matrix44();
    Matrix44(float m00, float m01, float m02, float m03, 
                float m10, float m11, float m12, float m13, 
                float m20, float m21, float m22, float m23, 
                float m30, float m31, float m32, float m33);

    Vector4 GetColumn(int column) const;
    Vector4 GetRow(int row) const;
    void SetColumn(int column, const Vector4& value);
    void SetRow(int row, const Vector4& value);

    void Transform3x3(const Vector3& vector, Vector3& out) const;
    Vector3 Transform3x3(const Vector3& vector) const;
    void FromMatrix34(Matrix34 const& m34);
    Matrix34 ToMatrix34();
};
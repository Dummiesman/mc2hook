#pragma once
#include "vector3.h"
#include "matrix44.h"

class Matrix34 
{
public:
    float m00;
    float m01;
    float m02;

    float m10;
    float m11;
    float m12;

    float m20;
    float m21;
    float m22;

    float m30;
    float m31;
    float m32;
public:
    const static Matrix34 I;
public:
    Matrix34(void);
    Matrix34(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float m30, float m31, float m32);

    void MakeRotate(Vector3 const& axis, float angle);  // Initializes this matrix to a rotation of angle radians around axis
    void MakeRotateUnitAxis(Vector3 const& axis, float angle);
    void MakeScale(float xScale, float yScale, float zScale);
    void MakeScale(float scale);
    void MakeRotateZ(float angle); // Initializes this matrix to a Z rotation of angle radians
    void MakeRotateY(float angle); // Initializes this matrix to a Y rotation of angle radians
    void MakeRotateX(float angle); // Initializes this matrix to a X rotation of angle radians
    void Rotate(Vector3 const& axis, float angle); // Rotate this matrix around axis by angle degrees (in radians)
    void RotateFull(Vector3 const& axis, float angle); // Rotate this matrix and its position around axis by angle degrees (in radians)
    void RotateX(float angle); // Rotate the X axis of this matrix by angle degrees (in radians)
    void RotateY(float angle); // Rotate the Y axis of this matrix by angle degrees (in radians)
    void RotateZ(float angle); // Rotate the Z axis of this matrix by angle degrees (in radians)
    void RotateFullX(float angle); // Rotate the X axis and position of this matrix by angle degrees (in radians)
    void RotateFullY(float angle); // Rotate the Y axis and position of this matrix by angle degrees (in radians)
    void RotateFullZ(float angle); // Rotate the Z axis and position of this matrix by angle degrees (in radians)
    Vector3 GetEulers();
    void Add(const Matrix34& values);
    void Zero();
    void Dot(const Matrix34& rhs);
    void Dot3x3(const Matrix34& rhs);
    void Scale(float amount);
    void Scale(float xAmount, float yAmount, float zAmount);
    void ScaleFull(float amount);
    void ScaleFull(float xAmount, float yAmount, float zAmount);
    void Identity();
    void Identity3x3();
    void Transform(const Vector3& vector, Vector3& out) const;
    Vector3 Transform(const Vector3& vector) const;
    void Transform3x3(const Vector3& vector, Vector3& out) const;
    Vector3 Transform3x3(const Vector3& vector) const;
    Vector4 GetColumn(int column) const;
    Vector3 GetRow(int row) const;
    void SetColumn(int column, const Vector4& value);
    void SetRow(int row, const Vector3& value);
    void Set(const Matrix34 &a1);
    void Print(const char* caption);
};
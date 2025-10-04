#pragma once
#include "matrix34.h"
#include <age/core/output.h>

Matrix34::Matrix34() {}
Matrix34::Matrix34(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float m30, float m31, float m32) 
: m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20), m21(m21), m22(m22), m30(m30), m31(m31), m32(m32) {}

const Matrix34 Matrix34::I = Matrix34(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0);

void Matrix34::MakeRotate(Vector3 const& axis, float angle)
{
    if (angle == 0.0f)
    {
        this->Identity3x3();
    }
    else
    {
        if (axis.X != 0.0f && axis.Y == 0.0f && axis.Z == 0.0f)
        {
            this->MakeRotateX(angle);
        }
        else if (axis.X == 0.0f && axis.Y != 0.0f && axis.Z == 0.0f)
        {
            this->MakeRotateY(angle);
        }
        else if (axis.X == 0.0f && axis.Y == 0.0f && axis.Z != 0.0f)
        {
            this->MakeRotateX(angle);
        }
        else 
        {
            Vector3 unitAxis = Vector3(axis);
            unitAxis.Normalize();
            this->MakeRotateUnitAxis(unitAxis, angle);
        }
    }
}

void Matrix34::MakeRotateUnitAxis(Vector3 const& axis, float angle)
{
    float cosine = cos(angle);
    float sine = sin(angle);
    float versine = 1.0f - cosine;

    SetRow(0, Vector3(axis.X * axis.X * versine + cosine,
        axis.Y * axis.X * versine + sine * axis.Z,
        axis.Z * axis.X * versine - sine * axis.Y));

    SetRow(1, Vector3(axis.Y * axis.X * versine - sine * axis.Z,
        axis.Y * axis.Y * versine + cosine,
        axis.Z * axis.Y * versine + sine * axis.X));

    SetRow(2, Vector3(axis.Z * axis.X * versine + sine * axis.Y,
        axis.Z * axis.Y * versine - sine * axis.X,
        axis.Z * axis.Z * versine + cosine));
}

void Matrix34::MakeScale(float xScale, float yScale, float zScale) {
    SetRow(0, Vector3(xScale, 0.0f, 0.0f));
    SetRow(1, Vector3(0.0f, yScale, 0.0f));
    SetRow(2, Vector3(0.0f, 0.0f, zScale));
}

void Matrix34::MakeScale(float scale) {
    SetRow(0, Vector3(scale, 0.0f, 0.0f));
    SetRow(1, Vector3(0.0f, scale, 0.0f));
    SetRow(2, Vector3(0.0f, 0.0f, scale));
}

void Matrix34::MakeRotateZ(float angle) {
    float sine = sinf(angle);
    float cosine = cosf(angle);

    SetRow(0, Vector3(0.0f, cosine, sine));
    SetRow(1, Vector3(-sine, cosine, 0.0f));
    SetRow(2, Vector3(0.0f, 0.0f, 1.0f));
}

void Matrix34::MakeRotateY(float angle) {
    float cosine = cosf(angle);
    float sine = sinf(angle);

    SetRow(0, Vector3(cosine, 0.0f, -sine));
    SetRow(1, Vector3(0.0f, 1.0f, 0.0f));
    SetRow(2, Vector3(sine, 0.0f, cosine));
}

void Matrix34::MakeRotateX(float angle) {
    float sine = sinf(angle);
    float cosine = cosf(angle);

    SetRow(0, Vector3(1.0f, 0.0f, 0.0f));
    SetRow(1, Vector3(0.0f, cosine, sine));
    SetRow(2, Vector3(0.0f, -sine, cosine));
}

void Matrix34::Rotate(Vector3 const& axis, float angle)
{
    Matrix34 temp;
    temp.MakeRotate(axis, angle);
    this->Dot3x3(temp);
}

void Matrix34::RotateFull(Vector3 const& axis, float angle)
{
    Matrix34 temp;
    temp.MakeRotate(axis, angle);
    temp.SetRow(3, Vector3::ORIGIN);
    this->Dot(temp);
}

void Matrix34::RotateX(float angle) 
{ 
    Matrix34 rotMatrix = Matrix34();
    rotMatrix.MakeRotateX(angle);
    this->Dot3x3(rotMatrix);
}

void Matrix34::RotateY(float angle) 
{
    Matrix34 rotMatrix = Matrix34();
    rotMatrix.MakeRotateY(angle);
    this->Dot3x3(rotMatrix);
}

void Matrix34::RotateZ(float angle) 
{
    Matrix34 rotMatrix = Matrix34();
    rotMatrix.MakeRotateZ(angle);
    this->Dot3x3(rotMatrix);
}

void Matrix34::RotateFullX(float angle) 
{
    Matrix34 rotMatrix = Matrix34();
    rotMatrix.MakeRotateX(angle);
    rotMatrix.SetRow(3, Vector3::ORIGIN);
    this->Dot(rotMatrix);
}

void Matrix34::RotateFullY(float angle) 
{
    Matrix34 rotMatrix = Matrix34();
    rotMatrix.MakeRotateY(angle);
    rotMatrix.SetRow(3, Vector3::ORIGIN);
    this->Dot(rotMatrix);
}

void Matrix34::RotateFullZ(float angle) 
{
    Matrix34 rotMatrix = Matrix34();
    rotMatrix.MakeRotateZ(angle);
    rotMatrix.SetRow(3, Vector3::ORIGIN);
    this->Dot(rotMatrix);
}

Vector3 Matrix34::GetEulers()
{
    Vector3 vec;
    vec.X = atan2f(this->m12, this->m22);
    vec.Y = asinf(-this->m02);
    vec.Z = atan2f(this->m01, this->m00);
    return vec;
}

void Matrix34::Add(const Matrix34& values) {
    this->m00 += values.m00;
    this->m01 += values.m01;
    this->m02 += values.m02;
    this->m10 += values.m10;
    this->m11 += values.m11;
    this->m12 += values.m12;
    this->m20 += values.m20;
    this->m21 += values.m21;
    this->m22 += values.m22;
    this->m30 += values.m30;
    this->m31 += values.m31;
    this->m32 += values.m32;
}

void Matrix34::Zero() {
    this->m02 = 0.0;
    this->m01 = 0.0;
    this->m00 = 0.0;
    this->m12 = 0.0;
    this->m11 = 0.0;
    this->m10 = 0.0;
    this->m22 = 0.0;
    this->m21 = 0.0;
    this->m20 = 0.0;
    this->m32 = 0.0;
    this->m31 = 0.0;
    this->m30 = 0.0;
}

void Matrix34::Dot3x3(const Matrix34& rhs) {
    float v3; // st7
    float v4; // [esp+0h] [ebp-18h]
    float v5; // [esp+4h] [ebp-14h]
    float v6; // [esp+8h] [ebp-10h]
    float v7; // [esp+Ch] [ebp-Ch]
    float v8; // [esp+10h] [ebp-8h]
    float v9; // [esp+14h] [ebp-4h]
    float v10; // [esp+20h] [ebp+8h]

    v3 = rhs.m11 * this->m01 + this->m02 * rhs.m21 + rhs.m01 * this->m00;
    v10 = rhs.m12 * this->m01 + rhs.m22 * this->m02 + rhs.m02 * this->m00;
    v9 = rhs.m10 * this->m11 + this->m12 * rhs.m20 + this->m10 * rhs.m00;
    v8 = rhs.m11 * this->m11 + this->m12 * rhs.m21 + rhs.m01 * this->m10;
    v7 = rhs.m12 * this->m11 + rhs.m02 * this->m10 + rhs.m22 * this->m12;
    v6 = rhs.m10 * this->m21 + this->m20 * rhs.m00 + rhs.m20 * this->m22;
    v5 = rhs.m11 * this->m21 + rhs.m01 * this->m20 + rhs.m21 * this->m22;
    v4 = rhs.m22 * this->m22 + rhs.m12 * this->m21 + rhs.m02 * this->m20;
    this->m00 = rhs.m10 * this->m01 + rhs.m00 * this->m00 + this->m02 * rhs.m20;
    this->m01 = v3;
    this->m02 = v10;
    this->m10 = v9;
    this->m11 = v8;
    this->m12 = v7;
    this->m20 = v6;
    this->m21 = v5;
    this->m22 = v4;
}

void Matrix34::Dot(const Matrix34& rhs) {
    float v3; // st7
    float v4; // st6
    float v5; // st5
    float v6; // [esp+0h] [ebp-24h]
    float v7; // [esp+4h] [ebp-20h]
    float v8; // [esp+8h] [ebp-1Ch]
    float v9; // [esp+Ch] [ebp-18h]
    float v10; // [esp+10h] [ebp-14h]
    float v11; // [esp+14h] [ebp-10h]
    float v12; // [esp+18h] [ebp-Ch]
    float v13; // [esp+1Ch] [ebp-8h]
    float v14; // [esp+20h] [ebp-4h]
    float rhsa; // [esp+2Ch] [ebp+8h]

    v3 = rhs.m11 * this->m01 + this->m02 * rhs.m21 + rhs.m01 * this->m00;
    rhsa = rhs.m12 * this->m01 + rhs.m22 * this->m02 + rhs.m02 * this->m00;
    v14 = rhs.m10 * this->m11 + this->m12 * rhs.m20 + this->m10 * rhs.m00;
    v13 = rhs.m11 * this->m11 + this->m12 * rhs.m21 + rhs.m01 * this->m10;
    v12 = rhs.m12 * this->m11 + rhs.m02 * this->m10 + rhs.m22 * this->m12;
    v11 = rhs.m10 * this->m21 + this->m20 * rhs.m00 + rhs.m20 * this->m22;
    v10 = rhs.m11 * this->m21 + rhs.m01 * this->m20 + rhs.m21 * this->m22;
    v9 = rhs.m22 * this->m22 + rhs.m12 * this->m21 + rhs.m02 * this->m20;
    v8 = rhs.m10 * this->m31 + this->m30 * rhs.m00 + this->m32 * rhs.m20 + rhs.m30;
    v7 = rhs.m11 * this->m31 + rhs.m01 * this->m30 + this->m32 * rhs.m21 + rhs.m31;
    v6 = rhs.m22 * this->m32 + rhs.m12 * this->m31 + rhs.m02 * this->m30 + rhs.m32;
    v4 = rhs.m10 * this->m01 + rhs.m00 * this->m00;
    v5 = this->m02 * rhs.m20;
    this->m02 = rhsa;
    this->m00 = v4 + v5;
    this->m01 = v3;
    this->m10 = v14;
    this->m11 = v13;
    this->m12 = v12;
    this->m20 = v11;
    this->m21 = v10;
    this->m22 = v9;
    this->m30 = v8;
    this->m31 = v7;
    this->m32 = v6;
}

void Matrix34::Scale(float amount) {
    this->m00 = amount * this->m00;
    this->m01 = amount * this->m01;
    this->m02 = amount * this->m02;
    this->m10 = amount * this->m10;
    this->m11 = amount * this->m11;
    this->m12 = amount * this->m12;
    this->m20 = amount * this->m20;
    this->m21 = amount * this->m21;
    this->m22 = amount * this->m22;
}

void Matrix34::Scale(float xAmount, float yAmount, float zAmount) {
    this->m00 = xAmount * this->m00;
    this->m10 = xAmount * this->m10;
    this->m20 = xAmount * this->m20;
    this->m01 = yAmount * this->m01;
    this->m11 = yAmount * this->m11;
    this->m21 = yAmount * this->m21;
    this->m02 = zAmount * this->m02;
    this->m12 = zAmount * this->m12;
    this->m22 = zAmount * this->m22;
}

void Matrix34::ScaleFull(float amount) {
    this->m00 = amount * this->m00;
    this->m01 = amount * this->m01;
    this->m02 = amount * this->m02;
    this->m10 = amount * this->m10;
    this->m11 = amount * this->m11;
    this->m12 = amount * this->m12;
    this->m20 = amount * this->m20;
    this->m21 = amount * this->m21;
    this->m22 = amount * this->m22;
    this->m30 = amount * this->m30;
    this->m31 = amount * this->m31;
    this->m32 = amount * this->m32;
}

void Matrix34::ScaleFull(float xAmount, float yAmount, float zAmount) {
    this->m00 = xAmount * this->m00;
    this->m10 = xAmount * this->m10;
    this->m20 = xAmount * this->m20;
    this->m30 = xAmount * this->m30;
    this->m01 = yAmount * this->m01;
    this->m11 = yAmount * this->m11;
    this->m21 = yAmount * this->m21;
    this->m31 = yAmount * this->m31;
    this->m02 = zAmount * this->m02;
    this->m12 = zAmount * this->m12;
    this->m22 = zAmount * this->m22;
    this->m32 = zAmount * this->m32;
}

void Matrix34::Identity() {
    this->m00 = 1.0;
    this->m01 = 0.0;
    this->m02 = 0.0;
    this->m10 = 0.0;
    this->m11 = 1.0;
    this->m12 = 0.0;
    this->m20 = 0.0;
    this->m21 = 0.0;
    this->m22 = 1.0;
    this->m30 = 0.0;
    this->m31 = 0.0;
    this->m32 = 0.0;
}

void Matrix34::Identity3x3() {
    this->m00 = 1.0;
    this->m01 = 0.0;
    this->m02 = 0.0;
    this->m10 = 0.0;
    this->m11 = 1.0;
    this->m12 = 0.0;
    this->m20 = 0.0;
    this->m21 = 0.0;
    this->m22 = 1.0;
}

void Matrix34::Transform(const Vector3& vector, Vector3& out) const
{
    out.X = this->m00 * vector.X + this->m10 * vector.Y + this->m20 * vector.Z + this->m30;
    out.Y = this->m01 * vector.X + this->m11 * vector.Y + this->m21 * vector.Z + this->m31;
    out.Z = this->m02 * vector.X + this->m12 * vector.Y + this->m22 * vector.Z + this->m32;
}

Vector3 Matrix34::Transform(const Vector3& vector) const
{
    Vector3 returnVec;
    Transform(vector, returnVec);
    return returnVec;
}

void Matrix34::Transform3x3(const Vector3& vector, Vector3& out) const
{
    out.X = this->m00 * vector.X + this->m10 * vector.Y + this->m20 * vector.Z;
    out.Y = this->m01 * vector.X + this->m11 * vector.Y + this->m21 * vector.Z;
    out.Z = this->m02 * vector.X + this->m12 * vector.Y + this->m22 * vector.Z;
}

Vector3 Matrix34::Transform3x3(const Vector3& vector) const
{
    Vector3 returnVec;
    Transform3x3(vector, returnVec);
    return returnVec;
}

Vector4 Matrix34::GetColumn(int column) const
{
    switch (column)
    {
    case 0:
        return Vector4(this->m00, this->m10, this->m20, this->m30);
    case 1:
        return Vector4(this->m01, this->m11, this->m21, this->m31);
    case 2:
        return Vector4(this->m02, this->m12, this->m22, this->m32);
    default:
        return Vector4();
    }
}

Vector3 Matrix34::GetRow(int row) const
{
    switch (row)
    {
    case 0:
        return Vector3(this->m00, this->m01, this->m02);
    case 1:
        return Vector3(this->m10, this->m11, this->m12);
    case 2:
        return Vector3(this->m20, this->m21, this->m22);
    case 3:
        return Vector3(this->m30, this->m31, this->m32);
    default:
        return Vector3();
    }
}

void Matrix34::SetColumn(int column, const Vector4& value)
{
    switch (column)
    {
    case 0:
        this->m00 = value.X;
        this->m10 = value.Y;
        this->m20 = value.Z;
        this->m30 = value.W;
        break;
    case 1:
        this->m01 = value.X;
        this->m11 = value.Y;
        this->m21 = value.Z;
        this->m31 = value.W;
        break;
    case 2:
        this->m02 = value.X;
        this->m12 = value.Y;
        this->m22 = value.Z;
        this->m32 = value.W;
        break;
    }
}

void Matrix34::SetRow(int row, const Vector3& value)
{
    switch (row)
    {
    case 0:
        this->m00 = value.X;
        this->m01 = value.Y;
        this->m02 = value.Z;
        break;
    case 1:
        this->m10 = value.X;
        this->m11 = value.Y;
        this->m12 = value.Z;
        break;
    case 2:
        this->m20 = value.X;
        this->m21 = value.Y;
        this->m22 = value.Z;
        break;
    case 3:
        this->m30 = value.X;
        this->m31 = value.Y;
        this->m32 = value.Z;
        break;
    }
}

void Matrix34::Set(const Matrix34 &to) 
{
    *this = to;
}

void Matrix34::Print(const char* caption)
{
    if (caption)
        Printf("%s {", caption);
    else
        Printf("matrix {");

    GetRow(0).Print(" a");
    Displayf("");
    GetRow(1).Print(" b");
    Displayf("");
    GetRow(2).Print(" c");
    Displayf("");
    GetRow(3).Print(" d");
    Displayf("");
    Printf("}\n");
}
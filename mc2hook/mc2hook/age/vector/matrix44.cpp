#include "matrix44.h"

Matrix44::Matrix44() {};
Matrix44::Matrix44(float m00, float m01, float m02, float m03,
                    float m10, float m11, float m12, float m13,
                    float m20, float m21, float m22, float m23,
                    float m30, float m31, float m32, float m33)
    : m00(m00), m01(m01), m02(m02), m03(m03),
        m10(m10), m11(m11), m12(m12), m13(m13),
        m20(m20), m21(m21), m22(m22), m23(m23),
        m30(m30), m31(m31), m32(m32), m33(m33) {}

const Matrix44 Matrix44::I = Matrix44(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 ,0.0, 0.0, 0.0, 1.0);
   
Vector4 Matrix44::GetColumn(int column) const
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

Vector4 Matrix44::GetRow(int row) const
{
    switch (row)
    {
    case 0:
        return Vector4(this->m00, this->m01, this->m02, this->m03);
    case 1:
        return Vector4(this->m10, this->m11, this->m12, this->m13);
    case 2:
        return Vector4(this->m20, this->m21, this->m22, this->m23);
    case 3:
        return Vector4(this->m30, this->m31, this->m32, this->m33);
    default:
        return Vector4();
    }
}

void Matrix44::SetColumn(int column, const Vector4& value)
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

void Matrix44::SetRow(int row, const Vector4& value)
{
    switch (row)
    {
    case 0:
        this->m00 = value.X;
        this->m01 = value.Y;
        this->m02 = value.Z;
        this->m03 = value.W;
        break;
    case 1:
        this->m10 = value.X;
        this->m11 = value.Y;
        this->m12 = value.Z;
        this->m13 = value.W;
        break;
    case 2:
        this->m20 = value.X;
        this->m21 = value.Y;
        this->m22 = value.Z;
        this->m23 = value.W;
        break;
    case 3:
        this->m30 = value.X;
        this->m31 = value.Y;
        this->m32 = value.Z;
        this->m33 = value.W;
        break;
    }
}

void Matrix44::Transform3x3(const Vector3& vector, Vector3& out) const
{
    out.X = this->m00 * vector.X + this->m10 * vector.Y + this->m20 * vector.Z;
    out.Y = this->m01 * vector.X + this->m11 * vector.Y + this->m21 * vector.Z;
    out.Z = this->m02 * vector.X + this->m12 * vector.Y + this->m22 * vector.Z;
}

Vector3 Matrix44::Transform3x3(const Vector3& vector) const
{
    Vector3 returnVec;
    Transform3x3(vector, returnVec);
    return returnVec;
}

void Matrix44::FromMatrix34(Matrix34 const& m34)
{
    Vector3 m34a = m34.GetRow(0);
    Vector3 m34b = m34.GetRow(1);
    Vector3 m34c = m34.GetRow(2);
    Vector3 m34d = m34.GetRow(3);

    SetRow(0, Vector4(m34a.X, m34a.Y, m34a.Z, 0.0f));
    SetRow(0, Vector4(m34b.X, m34b.Y, m34b.Z, 0.0f));
    SetRow(0, Vector4(m34c.X, m34c.Y, m34c.Z, 0.0f));
    SetRow(0, Vector4(m34d.X, m34d.Y, m34d.Z, 1.0f));
}

Matrix34 Matrix44::ToMatrix34()
{
    Vector4 m34a = GetRow(0);
    Vector4 m34b = GetRow(1);
    Vector4 m34c = GetRow(2);
    Vector4 m34d = GetRow(3);

    Matrix34 returnMat;
    returnMat.SetRow(0, Vector3(m34a.X, m34a.Y, m34a.Z));
    returnMat.SetRow(1, Vector3(m34b.X, m34b.Y, m34b.Z));
    returnMat.SetRow(2, Vector3(m34c.X, m34c.Y, m34c.Z));
    returnMat.SetRow(3, Vector3(m34d.X, m34d.Y, m34d.Z));
    return returnMat;
}

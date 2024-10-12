#include "vector3.h"
#include <print>
#include <age/core/output.h>

Vector3::Vector3() {}
Vector3::Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

const Vector3 Vector3::ORIGIN = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::XAXIS = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::YAXIS = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::ZAXIS = Vector3(0.0f, 0.0f, 1.0f);

float Vector3::InvMag(void) const {
    return 1.0f / this->Mag();
}
float Vector3::Mag(void) const {
    return sqrtf(this->Mag2());
}
float Vector3::Mag2(void) const {
    return (X * X + Y * Y + Z * Z);
}
void Vector3::Set(float x, float y, float z) {
    this->X = x;
    this->Y = y;
    this->Z = z;
}
void Vector3::Set(const Vector3& vec) {
    this->X = vec.X;
    this->Y = vec.Y;
    this->Z = vec.Z;
}
void Vector3::Add(const Vector3& vec) {
    this->X += vec.X;
    this->Y += vec.Y;
    this->Z += vec.Z;
}
void Vector3::AddScaled(const Vector3& vec, float scale) {
    this->X += (vec.X * scale);
    this->Y += (vec.Y * scale);
    this->Z += (vec.Z * scale);
}
void Vector3::Subtract(const Vector3& vec) {
    this->X -= vec.X;
    this->Y -= vec.Y;
    this->Z -= vec.Z;
}
void Vector3::SubtractScaled(const Vector3& vec, float scale) {
    this->X -= (vec.X * scale);
    this->Y -= (vec.Y * scale);
    this->Z -= (vec.Z * scale);
}
void Vector3::Scale(float scale) {
    this->X *= scale;
    this->Y *= scale;
    this->Z *= scale;
}
void Vector3::InvScale(float scale) {
    this->X *= 1.0f / scale;
    this->Y *= 1.0f / scale;
    this->Z *= 1.0f / scale;
}
void Vector3::RotateX(float angle) {
    float tmp = cosf(angle) * this->Y - sinf(angle) * this->Z;
    this->Z = cosf(angle) * this->Z + sinf(angle) * this->Y;
    this->Y = tmp;
}
void Vector3::RotateY(float angle) {
    float tmp = cosf(angle) * this->Z - sinf(angle) * this->X;
    this->X = sinf(angle) * this->Z + cosf(angle) * this->X;
    this->Z = tmp;
}
void Vector3::RotateZ(float angle) {
    float tmp = cosf(angle) * this->X - sinf(angle) * this->Y;
    this->Y = cosf(angle) * this->Y + sinf(angle) * this->X;
    this->X = tmp;
}
void Vector3::RotateAboutAxis(float angle, int axis) {
    float tmp;
    switch (axis)
    {
    case 'x':
        tmp = cosf(angle) * this->Y - sinf(angle) * this->Z;
        this->Z = cosf(angle) * this->Z + sinf(angle) * this->Y;
        this->Y = tmp;
        break;
    case 'y':
        tmp = cosf(angle) * this->Z - sinf(angle) * this->X;
        this->X = sinf(angle) * this->Z + cosf(angle) * this->X;
        this->Z = tmp;
        break;
    case 'z':
        tmp = cosf(angle) * this->X - sinf(angle) * this->Y;
        this->Y = cosf(angle) * this->Y + sinf(angle) * this->X;
        this->X = tmp;
        break;
    }
}
float Vector3::Angle(const Vector3& vec) const {
    float mag = sqrtf(vec.Mag2() * this->Mag2());
    float dotDivMag = this->Dot(vec) / mag;
    if (dotDivMag > 0.99999988)
        return 0.0;
    if (dotDivMag >= -1.0)
        return acosf(dotDivMag);
    return 3.1415927f;
}
float Vector3::FastAngle(const Vector3& vec) const {
    return acosf(vec.Z * this->Z + vec.Y * this->Y + vec.X * this->X);
}

float Vector3::Dist(const Vector3& vec) const {
    return (vec - *this).Mag();
}

float Vector3::FlatDist(const Vector3& vec) const {
    float dX = vec.X - this->X;
    float dZ = vec.Z - this->Z;
    return sqrtf(dX * dX + dZ * dZ);
}

float Vector3::Dot(const Vector3& vec) const {
    return vec.Z * this->Z + vec.Y * this->Y + vec.X * this->X;
}

void Vector3::Lerp(float t, const Vector3& vec1, const Vector3& vec2) {
    this->X = (vec2.X - vec1.X) * t + vec1.X;
    this->Y = (vec2.Y - vec1.Y) * t + vec1.Y;
    this->Z = (vec2.Z - vec1.Z) * t + vec1.Z;
}

void Vector3::Negate()
{
    this->X = -this->X;
    this->Y = -this->Y;
    this->Z = -this->Z;
}

void Vector3::Negate(const Vector3& vec) {
    this->X = -vec.X;
    this->Y = -vec.Y;
    this->Z = -vec.Z;
}

void Vector3::Normalize()
{
    float len = this->Mag2();
    float mul = (len != 0.0f) ? 1.0f / sqrtf(len) : 0.0f;

    this->X *= mul;
    this->Y *= mul;
    this->Z *= mul;
}

void Vector3::Min(const Vector3& vec1, const Vector3& vec2)
{
    this->X = (vec1.X < vec2.X) ? vec1.X : vec2.X;
    this->Y = (vec1.Y < vec2.Y) ? vec1.Y : vec2.Y;
    this->Z = (vec1.Z < vec2.Z) ? vec1.Z : vec2.Z;
}

void Vector3::Max(const Vector3& vec1, const Vector3& vec2)
{
    this->X = (vec1.X > vec2.X) ? vec1.X : vec2.X;
    this->Y = (vec1.Y > vec2.Y) ? vec1.Y : vec2.Y;
    this->Z = (vec1.Z > vec2.Z) ? vec1.Z : vec2.Z;
}

bool Vector3::IsEqual(const Vector3& vec) const {
    return vec.X == this->X && vec.Y == this->Y && vec.Z == this->Z;
}

Vector3 Vector3::Multiply(const Vector3& vec) const {
    return Vector3(this->X * vec.X, this->Y * vec.Y, this->Z * vec.Z);
}

void Vector3::Print() const
{
    Printf("%f,%f,%f", this->X, this->Y, this->Z);
}

void Vector3::Print(const char* caption)
{
    Printf("%s: %f,%f,%f", caption, this->X, this->Y, this->Z);
}

void Vector3::operator+=(const Vector3& vec) {
    this->X += vec.X;
    this->Y += vec.Y;
    this->Z += vec.Z;
}
void Vector3::operator-=(const Vector3& vec) {
    this->X -= vec.X;
    this->Y -= vec.Y;
    this->Z -= vec.Z;
}
void Vector3::operator+=(Vector3& vec) {
    this->X += vec.X;
    this->Y += vec.Y;
    this->Z += vec.Z;
}
void Vector3::operator-=(Vector3& vec) {
    this->X -= vec.X;
    this->Y -= vec.Y;
    this->Z -= vec.Z;
}

Vector3 Vector3::operator+(const Vector3& vec) const {
    return Vector3(this->X + vec.X, this->Y + vec.Y, this->Z + vec.Z);
}
Vector3 Vector3::operator-(const Vector3& vec) const {
    return Vector3(this->X - vec.X, this->Y - vec.Y, this->Z - vec.Z);
}
Vector3 Vector3::operator/(float value) const {
    return Vector3(this->X / value, this->Y / value, this->Z / value);
}
Vector3 Vector3::operator*(float value) const {
    return Vector3(this->X * value, this->Y * value, this->Z * value);
}
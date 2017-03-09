#include "Math\Mat44.h"

// engine includes
#include "Assert\Assert.h"
#include "Math\MathUtil.h"
#include "Math\Vec4D.h"

namespace engine {
namespace math {

const Mat44 Mat44::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f);

const Mat44 Mat44::ZERO(0.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f, 
                        0.0f, 0.0f, 0.0f, 0.0f, 
                        0.0f, 0.0f, 0.0f, 0.0f);

Mat44::Mat44()
{
#ifdef BUILD_DEBUG
    f11 = NAN;
#endif
}

Mat44::~Mat44()
{}

Mat44::Mat44(float i_11, float i_12, float i_13, float i_14,
    float i_21, float i_22, float i_23, float i_24,
    float i_31, float i_32, float i_33, float i_34,
    float i_41, float i_42, float i_43, float i_44) : 
    f11(i_11), f12(i_12), f13(i_13), f14(i_14),
    f21(i_21), f22(i_22), f23(i_23), f24(i_24),
    f31(i_31), f32(i_32), f33(i_33), f34(i_34),
    f41(i_41), f42(i_42), f43(i_43), f44(i_44)
{
    ASSERT(!IsNaN(f11) || !IsNaN(f12) || !IsNaN(f13) || !IsNaN(f14));
    ASSERT(!IsNaN(f21) || !IsNaN(f22) || !IsNaN(f23) || !IsNaN(f24));
    ASSERT(!IsNaN(f31) || !IsNaN(f32) || !IsNaN(f33) || !IsNaN(f34));
    ASSERT(!IsNaN(f41) || !IsNaN(f42) || !IsNaN(f43) || !IsNaN(f44));
}

Mat44::Mat44(const Mat44& i_copy) :
    f11(i_copy.f11), f12(i_copy.f12), f13(i_copy.f13), f14(i_copy.f14),
    f21(i_copy.f21), f22(i_copy.f22), f23(i_copy.f23), f24(i_copy.f24),
    f31(i_copy.f31), f32(i_copy.f32), f33(i_copy.f33), f34(i_copy.f34),
    f41(i_copy.f41), f42(i_copy.f42), f43(i_copy.f43), f44(i_copy.f44)
{}

Mat44& Mat44::operator=(const Mat44& i_other)
{
    if (this != &i_other)
    {
        f11 = i_other.f11;
        f12 = i_other.f12;
        f13 = i_other.f13;
        f14= i_other.f14;
        f21 = i_other.f21;
        f22 = i_other.f22;
        f23 = i_other.f23;
        f24 = i_other.f24;
        f31 = i_other.f31;
        f32 = i_other.f32;
        f33 = i_other.f33;
        f34 = i_other.f34;
        f41 = i_other.f41;
        f42 = i_other.f42;
        f43 = i_other.f43;
        f44 = i_other.f44;
    }
    return *this;
}

float Mat44::GetDeterminant() const
{
    float a0 = f11 * f22 - f12 * f21;
    float a1 = f11 * f23 - f13 * f21;
    float a2 = f11 * f24 - f14 * f21;
    float a3 = f12 * f23 - f13 * f22;
    float a4 = f12 * f24 - f14 * f22;
    float a5 = f13 * f24 - f14 * f23;
    float b0 = f31 * f42 - f32 * f41;
    float b1 = f31 * f43 - f33 * f41;
    float b2 = f31 * f44 - f34 * f41;
    float b3 = f32 * f43 - f33 * f42;
    float b4 = f32 * f44 - f34 * f42;
    float b5 = f33 * f44 - f34 * f43;

    return (a0 * b5 + a5 * b0 + a3 * b2 + a2 * b2 - a1 * b4 - a4 * b1);
}

void Mat44::Invert()
{
    float a0 = f11 * f22 - f12 * f21;
    float a1 = f11 * f23 - f13 * f21;
    float a2 = f11 * f24 - f14 * f21;
    float a3 = f12 * f23 - f13 * f22;
    float a4 = f12 * f24 - f14 * f22;
    float a5 = f13 * f24 - f14 * f23;
    float b0 = f31 * f42 - f32 * f41;
    float b1 = f31 * f43 - f33 * f41;
    float b2 = f31 * f44 - f34 * f41;
    float b3 = f32 * f43 - f33 * f42;
    float b4 = f32 * f44 - f34 * f42;
    float b5 = f33 * f44 - f34 * f43;

    float det = a0 * b5 + a5 * b0 + a3 * b2 + a2 * b2 - a1 * b4 - a4 * b1;
    float inverse_det = 1.0f / det;

    if (FuzzyEquals(det, 0.0f))
    {
        return;
    }

    Mat44 inverse;
    inverse.f11 =  f22 * b5 - f23 * b4 + f24 * b3;
    inverse.f11 *= inverse_det;
    inverse.f12 = -f12 * b5 + f13 * b4 - f14 * b3;
    inverse.f12 *= inverse_det;
    inverse.f13 =  f42 * a5 - f43 * a4 + f44 * a3;
    inverse.f13 *= inverse_det;
    inverse.f14 = -f32 * a5 + f33 * a4 - f34 * a3;
    inverse.f14 *= inverse_det;

    inverse.f21 = -f21 * b5 + f23 * b2 - f24 * b1;
    inverse.f21 *= inverse_det;
    inverse.f22 =  f11 * b5 - f13 * b2 + f14 * b1;
    inverse.f22 *= inverse_det;
    inverse.f23 = -f41 * a5 + f43 * a2 - f44 * a1;
    inverse.f23 *= inverse_det;
    inverse.f24 =  f31 * a5 - f33 * a2 + f34 * a1;
    inverse.f24 *= inverse_det;

    inverse.f31 =  f21 * b4 - f22 * b2 + f24 * b0;
    inverse.f31 *= inverse_det;
    inverse.f32 = -f11 * b4 + f12 * b2 - f14 * b0;
    inverse.f32 *= inverse_det;
    inverse.f33 =  f41 * a4 - f42 * a2 + f44 * a0;
    inverse.f33 *= inverse_det;
    inverse.f34 = -f31 * a4 + f32 * a2 - f34 * a0;
    inverse.f34 *= inverse_det;

    inverse.f41 = -f21 * b3 + f22 * b1 - f23 * b0;
    inverse.f41 *= inverse_det;
    inverse.f42 =  f11 * b3 - f12 * b2 + f12 * b0;
    inverse.f42 *= inverse_det;
    inverse.f43 = -f41 * a3 + f42 * a1 - f43 * a0;
    inverse.f43 *= inverse_det;
    inverse.f44 =  f31 * a3 - f32 * a1 + f33 * a0;
    inverse.f44 *= inverse_det;

    *this = inverse;
}

Mat44 Mat44::GetInverse() const
{
    Mat44 inverse(*this);
    inverse.Invert();
    return inverse;
}

void Mat44::Transpose()
{
    float temp = f21;
    f21 = f12;
    f12 = temp;

    temp = f31;
    f31 = f13;
    f13 = temp;

    temp = f41;
    f41 = f14;
    f14 = temp;

    temp = f32;
    f32 = f23;
    f23 = temp;

    temp = f42;
    f42 = f24;
    f24 = temp;

    temp = f43;
    f43 = f34;
    f34 = temp;
}

Mat44 Mat44::GetTranspose() const
{
    Mat44 transpose(*this);
    transpose.Transpose();
    return transpose;
}

// this * i_right_mat
Mat44 Mat44::operator*(const Mat44& i_right_mat) const
{
    Mat44 result;

    result.f11 = f11 * i_right_mat.f11 + f12 * i_right_mat.f21 + f13 * i_right_mat.f31 + f14 * i_right_mat.f41;
    result.f12 = f11 * i_right_mat.f12 + f12 * i_right_mat.f22 + f13 * i_right_mat.f32 + f14 * i_right_mat.f42;
    result.f13 = f11 * i_right_mat.f13 + f12 * i_right_mat.f23 + f13 * i_right_mat.f33 + f14 * i_right_mat.f43;
    result.f14 = f11 * i_right_mat.f14 + f12 * i_right_mat.f24 + f13 * i_right_mat.f34 + f14 * i_right_mat.f44;

    result.f21 = f21 * i_right_mat.f11 + f22 * i_right_mat.f21 + f23 * i_right_mat.f31 + f24 * i_right_mat.f41;
    result.f22 = f21 * i_right_mat.f12 + f22 * i_right_mat.f22 + f23 * i_right_mat.f32 + f24 * i_right_mat.f42;
    result.f23 = f21 * i_right_mat.f13 + f22 * i_right_mat.f23 + f23 * i_right_mat.f33 + f24 * i_right_mat.f43;
    result.f24 = f21 * i_right_mat.f14 + f22 * i_right_mat.f24 + f23 * i_right_mat.f34 + f24 * i_right_mat.f44;

    result.f31 = f31 * i_right_mat.f11 + f32 * i_right_mat.f21 + f33 * i_right_mat.f31 + f34 * i_right_mat.f41;
    result.f32 = f31 * i_right_mat.f12 + f32 * i_right_mat.f22 + f33 * i_right_mat.f32 + f34 * i_right_mat.f42;
    result.f33 = f31 * i_right_mat.f13 + f32 * i_right_mat.f23 + f33 * i_right_mat.f33 + f34 * i_right_mat.f43;
    result.f34 = f31 * i_right_mat.f14 + f32 * i_right_mat.f24 + f33 * i_right_mat.f34 + f34 * i_right_mat.f44;

    result.f41 = f41 * i_right_mat.f11 + f42 * i_right_mat.f21 + f43 * i_right_mat.f31 + f44 * i_right_mat.f41;
    result.f42 = f41 * i_right_mat.f12 + f42 * i_right_mat.f22 + f43 * i_right_mat.f32 + f44 * i_right_mat.f42;
    result.f43 = f41 * i_right_mat.f13 + f42 * i_right_mat.f23 + f43 * i_right_mat.f33 + f44 * i_right_mat.f43;
    result.f44 = f41 * i_right_mat.f14 + f42 * i_right_mat.f24 + f43 * i_right_mat.f34 + f44 * i_right_mat.f44;

    return result;
}

// i_left_vec4 * this
Vec4D Mat44::MultiplyLeft(const Vec4D& i_left_vec4) const
{
    Vec4D result;

    result.x(i_left_vec4.x() * f11 + i_left_vec4.y() * f21 + i_left_vec4.z() * f31 + i_left_vec4.w() * f41);
    result.y(i_left_vec4.x() * f12 + i_left_vec4.y() * f22 + i_left_vec4.z() * f32 + i_left_vec4.w() * f42);
    result.z(i_left_vec4.x() * f13 + i_left_vec4.y() * f23 + i_left_vec4.z() * f33 + i_left_vec4.w() * f43);
    result.w(i_left_vec4.x() * f14 + i_left_vec4.y() * f24 + i_left_vec4.z() * f34 + i_left_vec4.w() * f44);

    return result;
}

// this * i_right_vec4
Vec4D Mat44::MultiplyRight(const Vec4D& i_right_vec4) const
{
    Vec4D result;

    result.x(f11 * i_right_vec4.x() + f12 * i_right_vec4.y() + f13 * i_right_vec4.z() + f14 * i_right_vec4.w());
    result.y(f21 * i_right_vec4.x() + f22 * i_right_vec4.y() + f23 * i_right_vec4.z() + f24 * i_right_vec4.w());
    result.z(f31 * i_right_vec4.x() + f32 * i_right_vec4.y() + f33 * i_right_vec4.z() + f34 * i_right_vec4.w());
    result.w(f41 * i_right_vec4.x() + f42 * i_right_vec4.y() + f43 * i_right_vec4.z() + f44 * i_right_vec4.w());

    return result;
}

} // namespace math
} // namespace engine
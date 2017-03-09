#ifndef MAT44_H_
#define MAT44_H_

#include "Vec4D.h"

namespace engine {
namespace math {

class Mat44
{
public:
    Mat44();
    ~Mat44();
    Mat44(float i_11, float i_12, float i_13, float i_14,
        float i_21, float i_22, float i_23, float i_24,
        float i_31, float i_32, float i_33, float i_34,
        float i_41, float i_42, float i_43, float i_44);

    // copy constructor
    Mat44(const Mat44& i_copy);
    // copy assignment operator
    Mat44& operator=(const Mat44& i_other);

    // get the determinant
    float GetDeterminant() const;

    // invert this matrix
    void Invert();
    // return an inverse of this matrix
    Mat44 GetInverse() const;

    // transpose this matrix
    void Transpose();
    // return a transpose of this matrix
    Mat44 GetTranspose() const;

    // multiply a matrix with this one viz. this * i_right_mat
    Mat44 operator*(const Mat44& i_right_mat) const;

    // multiply a Vec4D with this matrix viz. i_vec4 * this
    Vec4D MultiplyLeft(const Vec4D& i_left_vec4) const;
    // multiply a Vec4D with this matrix viz. this * i_vec4
    Vec4D MultiplyRight(const Vec4D& i_right_vec4) const;
    
    static const Mat44 IDENTITY;
    static const Mat44 ZERO;

private:
    float       f11, f12, f13, f14;
    float       f21, f22, f23, f24;
    float       f31, f32, f33, f34;
    float       f41, f42, f43, f44;

}; // class Mat44

// matrix multiplication
// V = M * V
inline Vec4D operator*(const Mat44& i_left, const Vec4D& i_right);
// V = V * M
inline Vec4D operator*(const Vec4D& i_left, const Mat44& i_right);

} // namespace math
} // namespace engine

#include "Mat44-inl.h"

#endif // MAT44_H_

#include "definitions.h"

D3DXMATRIX* WINAPI D3DXMatrixMultiply(D3DXMATRIX* pOut, CONST D3DXMATRIX* pM1, CONST D3DXMATRIX* pM2)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            pOut->m[i][j] = pM1->m[i][0] * pM2->m[0][j] + pM1->m[i][1] * pM2->m[1][j] + pM1->m[i][2] * pM2->m[2][j] + pM1->m[i][3] * pM2->m[3][j];
        }
    }
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixTranslation(D3DXMATRIX* pOut, FLOAT x, FLOAT y, FLOAT z)
{
    D3DXMatrixIdentity(pOut);
    pOut->m[3][0] = x;
    pOut->m[3][1] = y;
    pOut->m[3][2] = z;
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixScaling(D3DXMATRIX* pOut, FLOAT x, FLOAT y, FLOAT z)
{
    D3DXMatrixIdentity(pOut);
    pOut->m[0][0] = x;
    pOut->m[1][1] = y;
    pOut->m[2][2] = z;
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixRotationZ(D3DXMATRIX* pOut, FLOAT Angle)
{
    D3DXMatrixIdentity(pOut);

    float c = cosf(Angle);
    float s = sinf(Angle);
    pOut->m[0][0] = c;
    pOut->m[1][1] = c;
    pOut->m[0][1] = s;
    pOut->m[1][0] = -s;
    return pOut;
}

D3DXVECTOR4* WINAPI D3DXVec3Transform(D3DXVECTOR4* pOut, CONST D3DXVECTOR3* pV, CONST D3DXMATRIX* pM)
{
    const D3DXVECTOR3 v = *pV;
    D3DXVECTOR4 out;

    out.x = pM->m[0][0] * v.x + pM->m[1][0] * v.y + pM->m[2][0] * v.z + pM->m[3][0];
    out.y = pM->m[0][1] * v.x + pM->m[1][1] * v.y + pM->m[2][1] * v.z + pM->m[3][1];
    out.z = pM->m[0][2] * v.x + pM->m[1][2] * v.y + pM->m[2][2] * v.z + pM->m[3][2];
    out.w = pM->m[0][3] * v.x + pM->m[1][3] * v.y + pM->m[2][3] * v.z + pM->m[3][3];

    *pOut = out;
    return pOut;
}

D3DXVECTOR3* WINAPI D3DXVec3TransformNormal(D3DXVECTOR3* pOut, CONST D3DXVECTOR3* pV, CONST D3DXMATRIX* pM)
{
    const D3DXVECTOR3 v = *pV;
    D3DXVECTOR3 out;

    out.x = pM->m[0][0] * v.x + pM->m[1][0] * v.y + pM->m[2][0] * v.z;
    out.y = pM->m[0][1] * v.x + pM->m[1][1] * v.y + pM->m[2][1] * v.z;
    out.z = pM->m[0][2] * v.x + pM->m[1][2] * v.y + pM->m[2][2] * v.z;

    *pOut = out;
    return pOut;
}

D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX* pOut)
{
    pOut->m[0][1] = pOut->m[0][2] = pOut->m[0][3] =
        pOut->m[1][0] = pOut->m[1][2] = pOut->m[1][3] =
        pOut->m[2][0] = pOut->m[2][1] = pOut->m[2][3] =
        pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;

    pOut->m[0][0] = pOut->m[1][1] = pOut->m[2][2] = pOut->m[3][3] = 1.0f;
    return pOut;
}

BOOL D3DXMatrixIsIdentity(CONST D3DXMATRIX* pM)
{
    return	pM->m[0][0] == 1.0f && pM->m[0][1] == 0.0f && pM->m[0][2] == 0.0f && pM->m[0][3] == 0.0f &&
        pM->m[1][0] == 0.0f && pM->m[1][1] == 1.0f && pM->m[1][2] == 0.0f && pM->m[1][3] == 0.0f &&
        pM->m[2][0] == 0.0f && pM->m[2][1] == 0.0f && pM->m[2][2] == 1.0f && pM->m[2][3] == 0.0f &&
        pM->m[3][0] == 0.0f && pM->m[3][1] == 0.0f && pM->m[3][2] == 0.0f && pM->m[3][3] == 1.0f;
}

FLOAT D3DXVec3Length(CONST D3DXVECTOR3* pV)
{
    return sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
}

FLOAT D3DXVec3Dot(CONST D3DXVECTOR3* pV1, CONST D3DXVECTOR3* pV2)
{
    return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

D3DXFLOAT16::D3DXFLOAT16(CONST D3DXFLOAT16& f)
{
    value = f.value;
}

BOOL D3DXFLOAT16::operator == (CONST D3DXFLOAT16& f) const
{
    return value == f.value;
}

BOOL D3DXFLOAT16::operator != (CONST D3DXFLOAT16& f) const
{
    return value != f.value;
}

D3DXMATRIX::D3DXMATRIX(CONST FLOAT* pf)
{
    memcpy(&_11, pf, sizeof(D3DXMATRIX));
}

D3DXMATRIX::D3DXMATRIX(CONST D3DMATRIX& mat)
{
    memcpy(&_11, &mat, sizeof(D3DXMATRIX));
}

D3DXMATRIX::D3DXMATRIX(FLOAT f11, FLOAT f12, FLOAT f13, FLOAT f14,
    FLOAT f21, FLOAT f22, FLOAT f23, FLOAT f24,
    FLOAT f31, FLOAT f32, FLOAT f33, FLOAT f34,
    FLOAT f41, FLOAT f42, FLOAT f43, FLOAT f44)
{
    _11 = f11; _12 = f12; _13 = f13; _14 = f14;
    _21 = f21; _22 = f22; _23 = f23; _24 = f24;
    _31 = f31; _32 = f32; _33 = f33; _34 = f34;
    _41 = f41; _42 = f42; _43 = f43; _44 = f44;
}

FLOAT& D3DXMATRIX::operator () (UINT iRow, UINT iCol)
{
    return m[iRow][iCol];
}

FLOAT D3DXMATRIX::operator () (UINT iRow, UINT iCol) const
{
    return m[iRow][iCol];
}

D3DXMATRIX::operator FLOAT* ()
{
    return (FLOAT*)&_11;
}

D3DXMATRIX::operator CONST FLOAT* () const
{
    return (CONST FLOAT*) & _11;
}

D3DXMATRIX& D3DXMATRIX::operator *= (CONST D3DXMATRIX& mat)
{
    D3DXMatrixMultiply(this, this, &mat);
    return *this;
}

D3DXMATRIX& D3DXMATRIX::operator += (CONST D3DXMATRIX& mat)
{
    _11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
    _21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
    _31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
    _41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
    return *this;
}

D3DXMATRIX& D3DXMATRIX::operator -= (CONST D3DXMATRIX& mat)
{
    _11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
    _21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
    _31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
    _41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
    return *this;
}

D3DXMATRIX& D3DXMATRIX::operator *= (FLOAT f)
{
    _11 *= f; _12 *= f; _13 *= f; _14 *= f;
    _21 *= f; _22 *= f; _23 *= f; _24 *= f;
    _31 *= f; _32 *= f; _33 *= f; _34 *= f;
    _41 *= f; _42 *= f; _43 *= f; _44 *= f;
    return *this;
}

D3DXMATRIX& D3DXMATRIX::operator /= (FLOAT f)
{
    FLOAT fInv = 1.0f / f;
    _11 *= fInv; _12 *= fInv; _13 *= fInv; _14 *= fInv;
    _21 *= fInv; _22 *= fInv; _23 *= fInv; _24 *= fInv;
    _31 *= fInv; _32 *= fInv; _33 *= fInv; _34 *= fInv;
    _41 *= fInv; _42 *= fInv; _43 *= fInv; _44 *= fInv;
    return *this;
}

D3DXMATRIX D3DXMATRIX::operator + () const
{
    return *this;
}

D3DXMATRIX D3DXMATRIX::operator - () const
{
    return D3DXMATRIX(-_11, -_12, -_13, -_14,
        -_21, -_22, -_23, -_24,
        -_31, -_32, -_33, -_34,
        -_41, -_42, -_43, -_44);
}

D3DXMATRIX D3DXMATRIX::operator * (CONST D3DXMATRIX& mat) const
{
    D3DXMATRIX matT;
    D3DXMatrixMultiply(&matT, this, &mat);
    return matT;
}

D3DXMATRIX D3DXMATRIX::operator + (CONST D3DXMATRIX& mat) const
{
    return D3DXMATRIX(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
        _21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
        _31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
        _41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
}

D3DXMATRIX D3DXMATRIX::operator - (CONST D3DXMATRIX& mat) const
{
    return D3DXMATRIX(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
        _21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
        _31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
        _41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
}

D3DXMATRIX D3DXMATRIX::operator * (FLOAT f) const
{
    return D3DXMATRIX(_11 * f, _12 * f, _13 * f, _14 * f,
        _21 * f, _22 * f, _23 * f, _24 * f,
        _31 * f, _32 * f, _33 * f, _34 * f,
        _41 * f, _42 * f, _43 * f, _44 * f);
}

D3DXMATRIX D3DXMATRIX::operator / (FLOAT f) const
{
    FLOAT fInv = 1.0f / f;
    return D3DXMATRIX(_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv,
        _21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
        _31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv,
        _41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);
}

D3DXMATRIX operator * (FLOAT f, CONST D3DXMATRIX& mat)
{
    return D3DXMATRIX(f * mat._11, f * mat._12, f * mat._13, f * mat._14,
        f * mat._21, f * mat._22, f * mat._23, f * mat._24,
        f * mat._31, f * mat._32, f * mat._33, f * mat._34,
        f * mat._41, f * mat._42, f * mat._43, f * mat._44);
}

BOOL D3DXMATRIX::operator == (CONST D3DXMATRIX& mat) const
{
    return 0 == memcmp(this, &mat, sizeof(D3DXMATRIX));
}

BOOL D3DXMATRIX::operator != (CONST D3DXMATRIX& mat) const
{
    return 0 != memcmp(this, &mat, sizeof(D3DXMATRIX));
}

D3DXVECTOR3::D3DXVECTOR3(CONST FLOAT* pf)
{
    x = pf[0];
    y = pf[1];
    z = pf[2];
}

D3DXVECTOR3::D3DXVECTOR3(CONST D3DVECTOR& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

D3DXVECTOR3::D3DXVECTOR3(FLOAT fx, FLOAT fy, FLOAT fz)
{
    x = fx;
    y = fy;
    z = fz;
}

D3DXVECTOR3::operator FLOAT* ()
{
    return (FLOAT*)&x;
}

D3DXVECTOR3::operator CONST FLOAT* () const
{
    return (CONST FLOAT*) & x;
}

D3DXVECTOR3& D3DXVECTOR3::operator += (CONST D3DXVECTOR3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

D3DXVECTOR3& D3DXVECTOR3::operator -= (CONST D3DXVECTOR3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

D3DXVECTOR3& D3DXVECTOR3::operator *= (FLOAT f)
{
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

D3DXVECTOR3& D3DXVECTOR3::operator /= (FLOAT f)
{
    FLOAT fInv = 1.0f / f;
    x *= fInv;
    y *= fInv;
    z *= fInv;
    return *this;
}

D3DXVECTOR3 D3DXVECTOR3::operator + () const
{
    return *this;
}

D3DXVECTOR3 D3DXVECTOR3::operator - () const
{
    return D3DXVECTOR3(-x, -y, -z);
}

D3DXVECTOR3 D3DXVECTOR3::operator + (CONST D3DXVECTOR3& v) const
{
    return D3DXVECTOR3(x + v.x, y + v.y, z + v.z);
}

D3DXVECTOR3 D3DXVECTOR3::operator - (CONST D3DXVECTOR3& v) const
{
    return D3DXVECTOR3(x - v.x, y - v.y, z - v.z);
}

D3DXVECTOR3 D3DXVECTOR3::operator * (FLOAT f) const
{
    return D3DXVECTOR3(x * f, y * f, z * f);
}

D3DXVECTOR3 D3DXVECTOR3::operator / (FLOAT f) const
{
    FLOAT fInv = 1.0f / f;
    return D3DXVECTOR3(x * fInv, y * fInv, z * fInv);
}

D3DXVECTOR3 operator * (FLOAT f, CONST struct D3DXVECTOR3& v)
{
    return D3DXVECTOR3(f * v.x, f * v.y, f * v.z);
}

BOOL D3DXVECTOR3::operator == (CONST D3DXVECTOR3& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

BOOL D3DXVECTOR3::operator != (CONST D3DXVECTOR3& v) const
{
    return x != v.x || y != v.y || z != v.z;
}

D3DXVECTOR4::D3DXVECTOR4(CONST FLOAT* pf)
{
    x = pf[0];
    y = pf[1];
    z = pf[2];
    w = pf[3];
}

D3DXVECTOR4::D3DXVECTOR4(CONST D3DVECTOR& v, FLOAT f)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = f;
}

D3DXVECTOR4::D3DXVECTOR4(FLOAT fx, FLOAT fy, FLOAT fz, FLOAT fw)
{
    x = fx;
    y = fy;
    z = fz;
    w = fw;
}

D3DXVECTOR4::operator FLOAT* ()
{
    return (FLOAT*)&x;
}

D3DXVECTOR4::operator CONST FLOAT* () const
{
    return (CONST FLOAT*) & x;
}

D3DXVECTOR4& D3DXVECTOR4::operator += (CONST D3DXVECTOR4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

D3DXVECTOR4& D3DXVECTOR4::operator -= (CONST D3DXVECTOR4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

D3DXVECTOR4& D3DXVECTOR4::operator *= (FLOAT f)
{
    x *= f;
    y *= f;
    z *= f;
    w *= f;
    return *this;
}

D3DXVECTOR4& D3DXVECTOR4::operator /= (FLOAT f)
{
    FLOAT fInv = 1.0f / f;
    x *= fInv;
    y *= fInv;
    z *= fInv;
    w *= fInv;
    return *this;
}

D3DXVECTOR4 D3DXVECTOR4::operator + () const
{
    return *this;
}

D3DXVECTOR4 D3DXVECTOR4::operator - () const
{
    return D3DXVECTOR4(-x, -y, -z, -w);
}

D3DXVECTOR4 D3DXVECTOR4::operator + (CONST D3DXVECTOR4& v) const
{
    return D3DXVECTOR4(x + v.x, y + v.y, z + v.z, w + v.w);
}

D3DXVECTOR4 D3DXVECTOR4::operator - (CONST D3DXVECTOR4& v) const
{
    return D3DXVECTOR4(x - v.x, y - v.y, z - v.z, w - v.w);
}

D3DXVECTOR4 D3DXVECTOR4::operator * (FLOAT f) const
{
    return D3DXVECTOR4(x * f, y * f, z * f, w * f);
}

D3DXVECTOR4 D3DXVECTOR4::operator / (FLOAT f) const
{
    FLOAT fInv = 1.0f / f;
    return D3DXVECTOR4(x * fInv, y * fInv, z * fInv, w * fInv);
}

D3DXVECTOR4 operator * (FLOAT f, CONST D3DXVECTOR4& v)
{
    return D3DXVECTOR4(f * v.x, f * v.y, f * v.z, f * v.w);
}

BOOL D3DXVECTOR4::operator == (CONST D3DXVECTOR4& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

BOOL D3DXVECTOR4::operator != (CONST D3DXVECTOR4& v) const
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}

d3dmatrix game_matrix::medium =
{
    1.0,		0.0,		0.0,		0.0,
    0.0,		-1.0,		0.0,		0.0,
    0.0,		0.0,		1.0,		0.0,
    0.0,		0.0,		0.0,		1.0
};

namespace math
{
    d3dvector vector_from(vector3<int>&& right)
    {
        return d3dvector {
            static_cast<FLOAT>(right.x()), 
            static_cast<FLOAT>(right.y()), 
            static_cast<FLOAT>(right.z())
        };
    }

    d3dmatrix translation_from(vector3<float_t> right)
    {
        D3DXMATRIX translation, identity;

        D3DXMatrixIdentity(&identity);
        D3DXMatrixTranslation(&translation, right.x(), right.y(), right.z());

        return identity * translation;
    }

    d3dvector fructum_transformation(const rect& visual, const d3dvector& model_coords)
    {
        float_t w = static_cast<float_t>(visual.right - visual.left);
        float_t h = static_cast<float_t>(visual.bottom - visual.top);
        const float_t f = 5000.0f;

        d3dvector result;

        result.x = w / 2.0f + (model_coords.x - model_coords.y) / sqrt(2.0f);
        result.y = h / 2.0f + (model_coords.x + model_coords.y) / 2.0f / sqrt(2.0f) - model_coords.z * sqrt(3.0f) / 2.0f;
        result.z = sqrt(3.0f) / 2.0f / f * (4000.0f * sqrt(2.0f) / 3.0f - (model_coords.x + model_coords.y) / sqrt(2.0f) - model_coords.z / sqrt(3.0f));

        return result;
    }
};

d3dvector& operator*(d3dvector& left, d3dmatrix& right)
{
    D3DXVECTOR4 result;
    D3DXVec3Transform(&result, &left, &right);

    left = { result.x,result.y,result.z };
    return left;
}

d3dvector& operator*=(d3dvector& left, d3dmatrix& right)
{
    return left = left * right;
}

double operator*(d3dvector& left, d3dvector&& right)
{
    return D3DXVec3Dot(&left, &right);
}

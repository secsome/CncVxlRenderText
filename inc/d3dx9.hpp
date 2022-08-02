// Following are definitions from d3dx9.h
// We don't want to depend on this d3dx9.dll but we need some of its functions
// So we manually define it here
// THIS FILE SHOULD NEVER BE INCLUDED IN ANY OTHER FILES THAN DEFINITIONS.H

typedef struct _D3DMATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float m[4][4];
	};
} D3DMATRIX;

typedef struct D3DXFLOAT16
{
#ifdef __cplusplus
public:
	D3DXFLOAT16() {};
	D3DXFLOAT16(const D3DXFLOAT16&);

	// binary operators
	bool operator == (const D3DXFLOAT16&) const;
	bool operator != (const D3DXFLOAT16&) const;

protected:
#endif //__cplusplus
	int16_t value;
} D3DXFLOAT16, * LPD3DXFLOAT16;

typedef struct D3DXMATRIX : public D3DMATRIX
{
public:
	D3DXMATRIX() {};
	D3DXMATRIX(const float*);
	D3DXMATRIX(const D3DMATRIX&);
	D3DXMATRIX(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44);


	// access grants
	float& operator () (uint32_t Row, uint32_t Col);
	float  operator () (uint32_t Row, uint32_t Col) const;

	// casting operators
	operator float* ();
	operator const float* () const;

	// assignment operators
	D3DXMATRIX& operator *= (const D3DXMATRIX&);
	D3DXMATRIX& operator += (const D3DXMATRIX&);
	D3DXMATRIX& operator -= (const D3DXMATRIX&);
	D3DXMATRIX& operator *= (float);
	D3DXMATRIX& operator /= (float);

	// unary operators
	D3DXMATRIX operator + () const;
	D3DXMATRIX operator - () const;

	// binary operators
	D3DXMATRIX operator * (const D3DXMATRIX&) const;
	D3DXMATRIX operator + (const D3DXMATRIX&) const;
	D3DXMATRIX operator - (const D3DXMATRIX&) const;
	D3DXMATRIX operator * (float) const;
	D3DXMATRIX operator / (float) const;

	friend D3DXMATRIX operator * (float, const D3DXMATRIX&);

	bool operator == (const D3DXMATRIX&) const;
	bool operator != (const D3DXMATRIX&) const;

} D3DXMATRIX, * LPD3DXMATRIX;

typedef struct _D3DVECTOR {
	float x;
	float y;
	float z;
} D3DVECTOR;

typedef struct D3DXVECTOR3 : public D3DVECTOR
{
public:
	D3DXVECTOR3() {};
	D3DXVECTOR3(const float*);
	D3DXVECTOR3(const D3DVECTOR&);
	D3DXVECTOR3(float x, float y, float z);

	// casting
	operator float* ();
	operator const float* () const;

	// assignment operators
	D3DXVECTOR3& operator += (const D3DXVECTOR3&);
	D3DXVECTOR3& operator -= (const D3DXVECTOR3&);
	D3DXVECTOR3& operator *= (float);
	D3DXVECTOR3& operator /= (float);

	// unary operators
	D3DXVECTOR3 operator + () const;
	D3DXVECTOR3 operator - () const;

	// binary operators
	D3DXVECTOR3 operator + (const D3DXVECTOR3&) const;
	D3DXVECTOR3 operator - (const D3DXVECTOR3&) const;
	D3DXVECTOR3 operator * (float) const;
	D3DXVECTOR3 operator / (float) const;

	friend D3DXVECTOR3 operator * (float, const struct D3DXVECTOR3&);

	bool operator == (const D3DXVECTOR3&) const;
	bool operator != (const D3DXVECTOR3&) const;

} D3DXVECTOR3, * LPD3DXVECTOR3;

typedef struct D3DXVECTOR4
{
#ifdef __cplusplus
public:
	D3DXVECTOR4() {};
	D3DXVECTOR4(const float*);
	D3DXVECTOR4(const D3DVECTOR& xyz, float w);
	D3DXVECTOR4(float x, float y, float z, float w);

	// casting
	operator float* ();
	operator const float* () const;

	// assignment operators
	D3DXVECTOR4& operator += (const D3DXVECTOR4&);
	D3DXVECTOR4& operator -= (const D3DXVECTOR4&);
	D3DXVECTOR4& operator *= (float);
	D3DXVECTOR4& operator /= (float);

	// unary operators
	D3DXVECTOR4 operator + () const;
	D3DXVECTOR4 operator - () const;

	// binary operators
	D3DXVECTOR4 operator + (const D3DXVECTOR4&) const;
	D3DXVECTOR4 operator - (const D3DXVECTOR4&) const;
	D3DXVECTOR4 operator * (float) const;
	D3DXVECTOR4 operator / (float) const;

	friend D3DXVECTOR4 operator * (float, const D3DXVECTOR4&);

	bool operator == (const D3DXVECTOR4&) const;
	bool operator != (const D3DXVECTOR4&) const;

public:
#endif //__cplusplus
	float x, y, z, w;
} D3DXVECTOR4, * LPD3DXVECTOR4;

D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX* pOut);
bool D3DXMatrixIsIdentity(const D3DXMATRIX* pM);
float D3DXVec3Length(const D3DXVECTOR3* pV);
float D3DXVec3Dot(const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2);

extern "C" {
	D3DXMATRIX* WINAPI D3DXMatrixMultiply(D3DXMATRIX* pOut, const D3DXMATRIX* pM1, const D3DXMATRIX* pM2);
	D3DXMATRIX* WINAPI D3DXMatrixTranslation(D3DXMATRIX* pOut, float x, float y, float z);
	D3DXMATRIX* WINAPI D3DXMatrixScaling(D3DXMATRIX* pOut, float sx, float sy, float sz);
	D3DXMATRIX* WINAPI D3DXMatrixRotationZ(D3DXMATRIX* pOut, float Angle);
	D3DXVECTOR4* WINAPI D3DXVec3Transform(D3DXVECTOR4* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
	D3DXVECTOR3* WINAPI D3DXVec3TransformNormal(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
}

#define D3DX_PI 3.1415926535897932384626
#define D3DX_SQRT2 1.4142135623730950488017

using d3dmatrix = D3DXMATRIX;
using d3dvector = D3DXVECTOR3;
struct rect
{
	int32_t    left;
	int32_t    top;
	int32_t    right;
	int32_t    bottom;
};

struct noinit_t final {};

struct color
{
	byte r = 0;
	byte g = 0;
	byte b = 0;
};

struct rectangle
{
	int32_t x = 0;
	int32_t y = 0;
	size_t width = 0;
	size_t height = 0;
};

template<typename type, size_t size,
	typename = typename std::enable_if<
	std::is_integral<type>::value ||
	std::is_floating_point<type>::value
	>::type
>
class vector
{
public:
	using value_type = type;

	~vector() = default;

	vector()
	{
		for (auto& item : _data)
			item = std::numeric_limits<type>::max();
	}

	template<typename... init_args, typename = typename std::enable_if <
		(sizeof ...(init_args) > 1)
	>::type
	>
		vector(init_args&&... args) : vector()
	{
		static_assert(sizeof... (args) <= size, "Too many args.");
		init_data<0>(args...);
	}

	template<typename r_type, size_t r_size>
	vector(const vector<r_type, r_size>& right) noexcept : vector()
	{
		for (size_t i = 0; i < std::min(size, r_size); i++)
			_data[i] = right._data[i];
	}

	operator bool() const
	{
		for (auto item : _data)
			if (item == std::numeric_limits<type>::max())
				return false;
		return true;
	}

	vector operator*(double scalar)
	{
		vector tmp = *this;
		for (size_t i = 0; i < size; i++)
			tmp._data[i] *= static_cast<type>(scalar);
		return tmp;
	}

	template<typename r_type, size_t r_size>
	vector operator*(const vector<r_type, r_size>& scalar)
	{
		vector tmp = *this;
		for (size_t i = 0; i < std::min(size, r_size); i++)
			tmp._data[i] *= scalar._data[i];
		return tmp;
	}

	vector& operator*=(const double scalar)
	{
		*this = *this * scalar;
		return *this;
	}

	template<typename r_type, size_t r_size>
	vector& operator*=(const vector<r_type, r_size>& scalar)
	{
		*this = *this * scalar;
		return *this;
	}

	template<typename r_type, size_t r_size>
	vector operator+(const vector<r_type, r_size>& right)
	{
		vector tmp = *this;
		for (size_t i = 0; i < std::min(size, r_size); i++)
			tmp._data[i] += right._data[i];
		return tmp;
	}

	vector operator+()
	{
		return *this;
	}

	template<typename r_type, size_t r_size>
	vector& operator+=(const vector<r_type, r_size>& right)
	{
		*this = *this + right;
		return *this;
	}

	template<typename r_type, size_t r_size>
	vector operator-(const vector<r_type, r_size>& right)
	{
		vector tmp = *this;
		for (size_t i = 0; i < std::min(size, r_size); i++)
			tmp._data[i] -= right._data[i];
		return tmp;
	}

	vector operator-()
	{
		vector tmp = *this;
		for (size_t i = 0; i < size; i++)
			tmp._data[i] = -tmp._data[i];
		return tmp;
	}

	template<typename r_type, size_t r_size>
	vector& operator=(const vector<r_type, r_size>& right)
	{
		for (size_t i = 0; i < std::min(size, r_size); i++)
			_data[i] = right._data[i];
		return *this;
	}

	template<typename r_type, size_t r_size>
	vector& operator-=(const vector<r_type, r_size>& right)
	{
		*this = *this - right;
		return *this;
	}

	bool operator==(const vector& right)
	{
		for (int i = 0; i < size; i++)
			if (_data[i] != right._data[i])
				return false;
		return true;
	}

	bool operator!=(const vector& right)
	{
		return !*this == right;
	}

	type& operator[](size_t idx)
	{
		return _data[idx];
	}

	type _data[size]{};

private:
	template<size_t idx, typename head_arg, typename... tail_args>
	void init_data(head_arg head, tail_args&&... tails)
	{
		static_assert(std::is_integral<head_arg>::value || std::is_floating_point<head_arg>::value, "Not numeric args!");
		_data[idx] = static_cast<type>(std::move(head));
		init_data<idx + 1>(tails...);
	}

	template<size_t idx, typename head_arg>
	void init_data(head_arg arg)
	{
		static_assert(std::is_integral<head_arg>::value || std::is_floating_point<head_arg>::value, "Not numeric args!");
		_data[idx] = static_cast<type>(std::move(arg));
	}
};

template<typename type>
class vector2 :public vector<type, 2>
{
public:
	vector2() :vector<type, 2>()
	{}

	template<typename... init_args, typename = typename std::enable_if <
		(sizeof ...(init_args) > 1)
	>::type
		>
		vector2(init_args&&... args) : vector<type, 2>(args...)
	{}

	template<typename r_type, size_t r_size>
	vector2(const vector<r_type, r_size>& right) noexcept : vector<type, 2>(right)
	{}

	type& x() { return (*this)[0]; }
	type& y() { return (*this)[1]; }
};

template<typename type>
class vector3 :public vector<type, 3>
{
public:
	vector3() :vector<type, 3>()
	{}

	template<typename... init_args, typename = typename std::enable_if <
		(sizeof ...(init_args) > 1)
	>::type
		>
		vector3(init_args&&... args) : vector<type, 3>(args...)
	{}

	template<typename r_type, size_t r_size>
	vector3(const vector<r_type, r_size>& right) noexcept : vector<type, 3>(right)
	{}

	type& x() { return (*this)[0]; }
	type& y() { return (*this)[1]; }
	type& z() { return (*this)[2]; }
};

template<typename type>
class vector4 :public vector<type, 4>
{
public:
	vector4() :vector<type, 4>()
	{}

	template<typename... init_args, typename = typename std::enable_if <
		(sizeof ...(init_args) > 1)
	>::type
		>
		vector4(init_args&&... args) : vector<type, 4>(args...)
	{}

	template<typename r_type, size_t r_size>
	vector4(const vector<r_type, r_size>& right) noexcept : vector<type, 4>(right)
	{}

	type& x() { return (*this)[0]; }
	type& y() { return (*this)[1]; }
	type& z() { return (*this)[2]; }
	type& w() { return (*this)[3]; }
};

class arithmatic_color :public vector4<float>
{
public:
	arithmatic_color() :vector4<value_type>(0.0, 0.0, 0.0, 0.0)
	{}

	template<typename... init_args, typename = typename std::enable_if <
		(sizeof ...(init_args) > 1)
	>::type
		>
		arithmatic_color(init_args&&... args) : vector4<value_type>(args...)
	{}

	template<typename r_type, size_t r_size>
	arithmatic_color(const vector<r_type, r_size>& right) noexcept : vector4<value_type>(right)
	{}

	arithmatic_color(const color& color)noexcept : vector4<value_type>()
	{
		r() = color.r / 255.0f;
		g() = color.g / 255.0f;
		b() = color.b / 255.0f;
	}

	arithmatic_color& lerp(arithmatic_color&& target, double weight)
	{
		*this = *this + (target - *this) * weight;
		return *this;
	}

	arithmatic_color& clamp(arithmatic_color&& low, arithmatic_color&& high)
	{
		(void)std::clamp(r(), low.r(), high.r());
		(void)std::clamp(g(), low.g(), high.g());
		(void)std::clamp(b(), low.b(), high.b());
		(void)std::clamp(a(), low.a(), high.a());

		return *this;
	}

	arithmatic_color& saturate()
	{
		return clamp(arithmatic_color(0.0, 0.0, 0.0, 0.0), arithmatic_color(1.0, 1.0, 1.0, 1.0));
	}

	arithmatic_color& blend(arithmatic_color&& color)
	{
		*this += color;

		if (a() > 1.0)
			*this *= 1.0 / a();

		return *this;
	}

	color as_binrary_color()
	{
		saturate();
		return color{ byte(r() * 255.0),byte(g() * 255.0),byte(b() * 255.0) };
	}

	value_type& r() { return x(); }
	value_type& g() { return y(); }
	value_type& b() { return z(); }
	value_type& a() { return w(); }
};

struct game_matrix
{
	static d3dmatrix medium;

	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
		};
		float _data[3][4];
	};

	float& operator[](size_t index)
	{
		return ((float*)(_data))[index];
	}

	d3dmatrix d3d_matrix(float scale = 1.0f)
	{
		return scale_rotation_part() * translation_part(scale);
	}

	d3dmatrix d3d_matrix_moved(d3dvector&& move)
	{
		d3dmatrix translation;
		d3dmatrix mydata = d3d_matrix();

		D3DXMatrixTranslation(&translation, move.x, move.y, move.z);
		return mydata * translation;
	}

	d3dmatrix translation_part(float scale = 1.0f)
	{
		d3dmatrix matrix;

		D3DXMatrixIdentity(&matrix);
		matrix.m[3][0] = this->_data[0][3] * scale;
		matrix.m[3][1] = -this->_data[1][3] * scale;
		matrix.m[3][2] = this->_data[2][3] * scale;

		return matrix;
	}

	d3dmatrix scale_rotation_part()
	{
		d3dmatrix matrix;
		D3DXMatrixIdentity(&matrix);

		matrix.m[0][0] = this->_data[0][0];
		matrix.m[0][1] = this->_data[1][0];
		matrix.m[0][2] = this->_data[2][0];

		matrix.m[1][0] = this->_data[0][1];
		matrix.m[1][1] = this->_data[1][1];
		matrix.m[1][2] = this->_data[2][1];

		matrix.m[2][0] = this->_data[0][2];
		matrix.m[2][1] = this->_data[1][2];
		matrix.m[2][2] = this->_data[2][2];

		return medium * matrix * medium;
	}

	d3dmatrix integrate_matrix(const d3dvector& scale, const float det)
	{
		d3dmatrix matrix;

		D3DXMatrixIdentity(&matrix);

		matrix.m[0][0] = this->_data[0][0];
		matrix.m[0][1] = this->_data[1][0];
		matrix.m[0][2] = this->_data[2][0];

		matrix.m[1][0] = this->_data[0][1];
		matrix.m[1][1] = this->_data[1][1];
		matrix.m[1][2] = this->_data[2][1];

		matrix.m[2][0] = this->_data[0][2];
		matrix.m[2][1] = this->_data[1][2];
		matrix.m[2][2] = this->_data[2][2];

		matrix.m[3][0] = this->_data[0][3] * det * scale.x;
		matrix.m[3][1] = this->_data[1][3] * det * scale.y;
		matrix.m[3][2] = this->_data[2][3] * det * scale.z;

		return matrix;
	}
};

namespace math
{
	d3dvector vector_from(vector3<int>&& right);
	d3dmatrix translation_from(vector3<float_t> right);
	d3dvector fructum_transformation(const rect& visual, const d3dvector& model_coords);
};

d3dvector& operator*(d3dvector& left, d3dmatrix& right);/*
{
	D3DXVECTOR4 result;
	D3DXVec3Transform(&result, &left, &right);

	left = { result.x,result.y,result.z };
	return left;
}*/

d3dvector& operator*=(d3dvector& left, d3dmatrix& right);/*
{
	return left = left * right;
}*/

double operator*(d3dvector& left, d3dvector&& right);/*
{
	return D3DXVec3Dot(&left, &right);
}*/
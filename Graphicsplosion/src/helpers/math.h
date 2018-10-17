#pragma once

class Vec4 {
public:
	Vec4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {};

public:
	float x, y, z, w;
};

class Matrix {
public:
	Matrix() = default;
	Matrix(const Vec4& x, const Vec4& y, const Vec4& z, const Vec4& w) :
		m11(x.x), m21(x.y), m31(x.z), m41(x.w), m12(y.x), m22(y.y), m32(y.z), m42(y.w), m13(z.x), m23(z.y), m33(z.z), m43(z.w),
		m14(w.x), m24(w.y), m34(w.z), m44(w.w) {};

	// Identity constructor
	static Matrix Matrix::FromIdentity() {
		return Matrix(Vec4(1.0f, 0.0f, 0.0f, 0.0f), Vec4(0.0f, 1.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 1.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

public:
	union {
		struct {
			float m11, m12, m13, m14;
			float m21, m22, m23, m24;
			float m31, m32, m33, m34;
			float m41, m42, m43, m44;
		};

		float m[4][4];
	};

public:
	// Matrix multiplication operator
	void operator*=(const Matrix& other) {
		Matrix temp{};
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				for (int s = 0; s < 4; s++) {
					temp.m[r][c] += m[r][s] * other.m[s][c];
				}
			}
		}

		*this = temp;
	}
};

template<typename DimensionType>
class Vec2Base {
public:
	Vec2Base() = default;
	Vec2Base(DimensionType x_, DimensionType y_) : x(x_), y(y_) {};

public:
	// Operator overloads
	bool operator!=(const Vec2Base& other) { return (x != other.x || y != other.y); }

public:
	DimensionType x, y;
};

typedef Vec2Base<int> Vec2I;

// Rectangle base class with optional variable type for the dimensions
template<typename DimensionType>
class Rect2Base {
	DimensionType x, y;
	DimensionType width, height;
};
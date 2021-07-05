#pragma once

// Note: Most of these functions were written before the glm library was used, so are unused

// General math functions
namespace Math {
	// Clamps a value between -1 and 1
	inline float ClampNormal(float value) { return value < -1.0f ? -1.0f : (value > 1.0f ? 1.0f : value); }

	// If a value is smaller than deadzoneRange, clamps the value to 0
	inline float ClampDeadzone(float value, float deadzoneRange) { return (value > -deadzoneRange && value < deadzoneRange) ? 0.0f : value;  }
};

// 2D vector template
template<typename DimensionType>
class Vec2Base {
public:
	// Constructors (with or without initial values)
	Vec2Base() = default;
	Vec2Base(DimensionType x_, DimensionType y_) : x(x_), y(y_) {};

public:
	// Comparison operators
	bool operator!=(const Vec2Base& other) { return (x != other.x || y != other.y); }

public:
	// Math operators
	void operator+=(const Vec2Base& other) { x += other.x; y += other.y; }

public:
	DimensionType x, y;
};

// 2D vector with int components
typedef Vec2Base<int> Vec2I;

// 2D rectangle template
template<typename DimensionType>
class Rect2Base {
public:
	Rect2Base() = default;
	Rect2Base(DimensionType x_, DimensionType y_, DimensionType width_, DimensionType height_) : x(x_), y(y_), width(width_), height(height_) {}

	DimensionType x, y;
	DimensionType width, height;
};

// Rectangle with float components
typedef Rect2Base<float> Rect2F;

// 4D float vector
template<typename DimensionType>
class Vec4Base {
public:
	Vec4Base(DimensionType x_, DimensionType y_, DimensionType z_, DimensionType w_) : x(x_), y(y_), z(z_), w(w_) {};

public:
	DimensionType x, y, z, w;
};

typedef Vec4Base<float> Vec4F;

// 4x4 matrix
class Matrix {
public:
	Matrix() = default;
	Matrix(const Vec4F& x, const Vec4F& y, const Vec4F& z, const Vec4F& w) :
		m11(x.x), m21(x.y), m31(x.z), m41(x.w), m12(y.x), m22(y.y), m32(y.z), m42(y.w), m13(z.x), m23(z.y), m33(z.z), m43(z.w),
		m14(w.x), m24(w.y), m34(w.z), m44(w.w) {};

	// Identity constructor
	static Matrix Matrix::FromIdentity() {
		return Matrix(Vec4F(1.0f, 0.0f, 0.0f, 0.0f), Vec4F(0.0f, 1.0f, 0.0f, 0.0f), Vec4F(0.0f, 0.0f, 1.0f, 0.0f), Vec4F(0.0f, 0.0f, 0.0f, 1.0f));
	}

public:
	union {
		struct {
			// Matrix components as variables
			float m11, m12, m13, m14;
			float m21, m22, m23, m24;
			float m31, m32, m33, m34;
			float m41, m42, m43, m44;
		};

		// Matrix components as an array
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
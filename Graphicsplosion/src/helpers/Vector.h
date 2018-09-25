#pragma once

template<typename DimensionType>
class Vec2Base {
public:
	Vec2Base(DimensionType x_, DimensionType y_) : x(x_), y(y_) {};

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
#pragma once


// �^�����m��̂܂܎g����(�g���Ƃ��Ɍ^�����肷��)
template<typename T>
struct Vector2D
{
	Vector2D() : x(0), y(0) {}
	Vector2D(T inx, T iny) : x(inx), y(iny) {}
	T x;
	T y;

	Vector2D<T> operator+(const Vector2D<T>& in)
	{
		return Vector2D<T>(in.x + x, in.y + y);
	}
	void operator+=(const Vector2D<T>& in)
	{
		x += in.x;
		y += in.y;
	}
	void operator-=(const Vector2D<T>& in)
	{
		x -= in.x;
		y -= in.y;
	}
	void operator*=(const T& in)
	{
		x *= in;
		y *= in;
	}
	void operator/=(const T& in)
	{
		x /= in;
		y /= in;
	}
	void operator/=(const Vector2D<T>& in)
	{
		x /= in.x;
		y /= in.y;
	}

	bool operator==(const Vector2D<T>& in)const
	{
		return ((x == in.x) && (y == in.y));
	}
	bool operator!=(const Vector2D<T>& in)const
	{
		return ((x != in.x) || (y != in.y));
	}
	bool operator|=(const Vector2D<T>& in)const
	{
		return ((x == in.x) || (y == in.y));
	}
};

// �����^�޸��
typedef Vector2D<int> Vector2;

// �����^�޸��
typedef Vector2D<float> Vector2f;

/// ���ނ�\���\����
struct Size {
	Size() : width(0), height(0) {}
	Size(int inw, int inh) : width(inw), height(inh) {}
	int width;	// ��
	int height;	// ����
};

/// ��`��\������\����
struct Rect {
	Rect() : center(0, 0), size(0, 0) {}
	Rect(float x, float y, int inw, int inh) : center(x, y), size(inw, inh) {}
	Rect(const Vector2& inc, const Size& ins) : center(inc), size(ins) {}

	Vector2 center;	// ���S
	Size size;		// ���ƍ���


	const int Left()const
	{
		return center.x - size.width / 2;
	}
	const int Top()const
	{
		return center.y - size.height / 2;
	}
	const int Right()const
	{
		return center.x + size.width / 2;
	}
	const int Bottom()const
	{
		return center.y + size.height / 2;
	}
	const int Width()const
	{
		return size.width;
	}
	const int Height()const
	{
		return size.height;
	}
};


class Geometry
{
public:
	Geometry();
	~Geometry();
};


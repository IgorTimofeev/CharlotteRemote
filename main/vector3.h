#pragma once

#include <cstdint>
#include <cmath>

namespace pizdanc {
	class SinAndCos {
		public:
			float getSin() const;
			void setSin(float sin);

			float getCos() const;
			void setCos(float cos);

		private:
			float _sin;
			float _cos;
	};


	template<typename T>
	class Vector3 {
		public:
			Vector3();
			Vector3(T x, T y, T z);

			template<class TOther>
			explicit Vector3(const Vector3<TOther>& other);

			T getX() const;
			void setX(T value);

			T getY() const;
			void setY(T value);

			T getZ() const;
			void setZ(T value);

			T getLength() const;
			Vector3 normalize() const;

			Vector3 operator+(const Vector3& right) const;
			Vector3 operator-(const Vector3& right) const;
			Vector3 operator*(T factor) const;
			Vector3 operator/(T factor) const;
			bool operator==(const Vector3 &right) const;
			bool operator!=(const Vector3 &right) const;

		private:
			T _x;
			T _y;
			T _z;
	};

	template<typename T>
	Vector3<T>::Vector3(T x, T y, T z) : _x(x), _y(y), _z(z) {

	}

	template<typename T>
	Vector3<T>::Vector3() : Vector3(0, 0, 0) {

	}

	template<typename T>
	template<typename TOther>
	Vector3<T>::Vector3(const Vector3<TOther>& other) : Vector3((T) other.getX(), (T) other.getY(), (T) other.getZ()) {

	}

	template<typename T>
	T Vector3<T>::getX() const {
		return _x;
	}

	template<typename T>
	void Vector3<T>::setX(T value) {
		_x = value;
	}

	template<typename T>
	T Vector3<T>::getY() const {
		return _y;
	}

	template<typename T>
	void Vector3<T>::setY(T value) {
		_y = value;
	}

	template<typename T>
	T Vector3<T>::getZ() const {
		return _z;
	}

	template<typename T>
	void Vector3<T>::setZ(T value) {
		_z = value;
	}

	template<typename T>
	T Vector3<T>::getLength() const {
		return (T) std::sqrt(
			(float) _x * (float) _x
			+ (float) _y * (float) _y
			+ (float) _z * (float) _z
		);
	}

	template<typename T>
	Vector3<T> Vector3<T>::normalize() const {
		auto length = getLength();

		return {
			_x / length,
			_y / length,
			_z / length
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*(T factor) const {
		return {
			_x * factor,
			_y * factor,
			_z * factor
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator/(T factor) const {
		return {
			_x / factor,
			_y / factor,
			_z / factor
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3& right) const {
		return {
			_x + right._x,
			_y + right._y,
			_z + right._z,
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3& right) const {
		return {
			_x - right._x,
			_y - right._y,
			_z - right._z,
		};
	}

	template<typename T>
	bool Vector3<T>::operator==(const Vector3& right) const {
		return
			_x == right._x
			&& _y == right._y
			&& _z == right._z;
	}

	template<typename T>
	bool Vector3<T>::operator!=(const Vector3& right) const {
		return !operator==(right);
	}

	using Vector3F = Vector3<float>;
}
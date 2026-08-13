#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Vector4D.h>

#define _USE_MATH_DEFINES 
#include <math.h>

class Quaternion {
public:
	Quaternion() : m_x(0), m_y(0), m_z(0), m_w(1) {

	}

	Quaternion(f32 x, f32 y, f32 z, f32 w) : m_x(x), m_y(y), m_z(z), m_w(w) {

	}

	Quaternion(const Vector4D v) : m_x(v.m_x), m_y(v.m_y), m_z(v.m_z), m_w(v.m_w) {

	}


	Quaternion(const Quaternion& q) : m_x(q.m_x), m_y(q.m_y), m_z(q.m_z), m_w(q.m_w) {

	}

	static Quaternion identity() {
		return Quaternion(0,0,0,1);
	}

	f32 length() const {
		return sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
	}

	Quaternion normalized() const {
		f32 l = length();
		if (!l) return Quaternion::identity();
		return Quaternion(m_x, m_y, m_z, m_w) * (1.0f / l);
	}

	Quaternion conjugate() const {
		return Quaternion(-m_x, -m_y, -m_z, m_w);
	}

	// 
	static Quaternion fromAxisAngle(const Vector3D& axis, f32 angleRad) {
		f32 half = angleRad * 0.5f;
		f32 s = sinf(half);
		return Quaternion(axis.m_x * s, axis.m_y * s, axis.m_z * s, cosf(half));
	}

	static Quaternion fromEuler(const Vector3D& eulerRadians) {
		Quaternion qx = fromAxisAngle(Vector3D(1, 0, 0), eulerRadians.m_x);
		Quaternion qy = fromAxisAngle(Vector3D(0, 1, 0), eulerRadians.m_y);
		Quaternion qz = fromAxisAngle(Vector3D(0, 0, 1), eulerRadians.m_z);
		return qx * qy * qz;
	}

	Vector3D toEuler() const {
		Vector3D euler;

		f32 siny = 2.0f * (m_w * m_y + m_x * m_z);

		// check for gimbal lock 
		if (fabsf(siny) >= 0.9999f) {
			euler.m_y = copysignf( M_PI / 2.0f, siny); 
			euler.m_x = 2.0f * atan2f(m_x, m_w);             
			euler.m_z = 0.0f;
		}
		else {
			euler.m_y = asinf(siny);
			euler.m_x = atan2f(2.0f * (m_w * m_x - m_y * m_z), 1.0f - 2.0f * (m_x * m_x + m_y * m_y));
			euler.m_z = atan2f(2.0f * (m_w * m_z - m_x * m_y), 1.0f - 2.0f * (m_y * m_y + m_z * m_z));
		}

		return euler;
	}
	//

	Quaternion operator* (f32 f) const {
		return Quaternion(m_x * f, m_y * f, m_z * f, m_w * f);
	}

	Quaternion operator*(const Quaternion& q) const {
		f32 w = m_w * q.m_w - m_x * q.m_x - m_y * q.m_y - m_z * q.m_z;
		f32 x = m_x * q.m_w + m_w * q.m_x + m_y * q.m_z - m_z * q.m_y;
		f32 y = m_y * q.m_w + m_w * q.m_y + m_z * q.m_x - m_x * q.m_z;
		f32 z = m_z * q.m_w + m_w * q.m_z + m_x * q.m_y - m_y * q.m_x;

		return Quaternion(x, y, z, w);
	}

	Quaternion operator*(const Vector3D& v) const {
		f32 w = -m_x * v.m_x - m_y * v.m_y - m_z * v.m_z;
		f32 x = m_w * v.m_x + m_y * v.m_z - m_z * v.m_y;
		f32 y = m_w * v.m_y + m_z * v.m_x - m_x * v.m_z;
		f32 z = m_w * v.m_z + m_x * v.m_y - m_y * v.m_x;

		return Quaternion(x, y, z, w);
	}
private:
	static f32 clampf(f32 v, f32 lo, f32 hi) {
		return v < lo ? lo : (v > hi ? hi : v);
	}
public:
	f32 m_x, m_y, m_z, m_w;
};
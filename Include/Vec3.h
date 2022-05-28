#pragma once

#include<iostream>
#include<stdlib.h>
#include<math.h>
#include <random>
class Vec3 {
public:
	Vec3() {}
	Vec3(float E0, float E1, float E2) {
		E[0] = E0;
		E[1] = E1;
		E[2] = E2;
	}
	//返回单个元素的值
	//作为XYZ坐标
	inline float X() const { return E[0]; };
	inline float Y() const { return E[1]; };
	inline float Z() const { return E[2]; };
	//作为RGB颜色
	inline float R() const { return E[0]; };
	inline float G() const { return E[1]; };
	inline float B() const { return E[2]; };

	//重载运算符
	inline const Vec3& operator+() const { return *this; };
	inline const Vec3 operator-() const { return Vec3(-E[0], -E[1], -E[2]); };
	inline float operator[](int i) const { return E[i]; };
	inline float& operator[](int i) { return E[i]; };

	inline Vec3& operator+=(const Vec3& V);
	inline Vec3& operator-=(const Vec3& V);
	inline Vec3& operator*=(const Vec3& V);
	inline Vec3& operator/=(const Vec3& V);
	inline Vec3& operator*=(const float T);
	inline Vec3& operator/=(const float T);

	inline float Vec3Length() const { return sqrt(E[0] * E[0] + E[1] * E[1] + E[2] * E[2]); };
	inline float SquaredLength() const { return E[0] * E[0] + E[1] * E[1] + E[2] * E[2]; };
	//使向量变为单位向量
	inline void MakeUnitVector();

	friend inline Vec3 UnitVector(Vec3 V);
	friend inline std::istream& operator>>(std::istream& is, Vec3& T);
	friend inline std::ostream& operator<<(std::ostream& os, const Vec3& T);
	friend inline Vec3 operator+(const Vec3& V1, const Vec3& V2);
	friend inline Vec3 operator-(const Vec3& V1, const Vec3& V2);
	friend inline Vec3 operator*(const Vec3& V1, const Vec3& V2);
	friend inline Vec3 operator*(const float T, const Vec3& V);
	friend inline Vec3 operator*(const Vec3& V, const float T);
	friend inline Vec3 operator/(const Vec3& V1, const Vec3& V2);
	friend inline Vec3 operator/(const Vec3& V, const float T);
	
	friend inline float Dot(const Vec3& V1, const Vec3& V2);
	friend inline Vec3 Cross(const Vec3& V1, const Vec3& V2);
	float E[3];
};


inline Vec3 UnitVector(Vec3 V)
{
	return V / V.Vec3Length();
}

inline std::istream& operator>>(std::istream& is, Vec3& V) {
	is >> V.E[0] >> V.E[1] >> V.E[2];
	return is;
}

inline std::ostream& operator<<(std::ostream& os, Vec3& V) {
	os << V.E[0] << V.E[1] << V.E[2];
	return os;
}

inline Vec3 operator+(const Vec3& V1, const Vec3& V2)
{
	return Vec3(V1.E[0] + V2.E[0], V1.E[1] + V2.E[1], V1.E[2] + V2.E[2]);
}

inline Vec3 operator-(const Vec3& V1, const Vec3& V2)
{
	return Vec3(V1.E[0] - V2.E[0], V1.E[1] - V2.E[1], V1.E[2] - V2.E[2]);
}

inline Vec3 operator*(const Vec3& V1, const Vec3& V2)
{
	return Vec3(V1.E[0] * V2.E[0], V1.E[1] * V2.E[1], V1.E[2] * V2.E[2]);
}

inline Vec3 operator*(const float T, const Vec3& V)
{
	return Vec3(V.E[0] * T, V.E[1] * T, V.E[2] * T);
}

inline Vec3 operator*(const Vec3& V, const float T)
{
	return Vec3(V.E[0] * T, V.E[1] * T, V.E[2] * T);
}

inline Vec3 operator/(const Vec3& V1, const Vec3& V2)
{
	return Vec3(V1.E[0] / V2.E[0], V1.E[1] / V2.E[1], V1.E[2] / V2.E[2]);
}

inline Vec3 operator/(const Vec3& V, const float T)
{
	return Vec3(V.E[0] / T, V.E[1] / T, V.E[2] / T);
}

inline float Dot(const Vec3& V1, const Vec3& V2)
{
	return V1.E[0] * V2.E[0] + V1.E[1] * V2.E[1] + V1.E[2] * V2.E[2];
}

inline Vec3 Cross(const Vec3& V1, const Vec3& V2)
{
	return Vec3(

		(V1.E[1] * V2.E[2] - V1.E[2] * V2.E[1]),

		(-(V1.E[0] * V2.E[2] - V1.E[2] * V2.E[0])),

		(V1.E[0] * V2.E[1] - V1.E[1] * V2.E[0])

	);
}

inline Vec3& Vec3::operator+=(const Vec3& V)
{
	E[0] += V.E[0];
	E[1] += V.E[1];
	E[2] += V.E[2];
	return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& V)
{
	E[0] -= V.E[0];
	E[1] -= V.E[1];
	E[2] -= V.E[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& V)
{
	E[0] *= V.E[0];
	E[1] *= V.E[1];
	E[2] *= V.E[2];
	return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& V)
{
	E[0] /= V.E[0];
	E[1] /= V.E[1];
	E[2] /= V.E[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const float T)
{
	E[0] *= T;
	E[1] *= T;
	E[2] *= T;
	return *this;
}

inline Vec3& Vec3::operator/=(const float T)
{
	E[0] /= T;
	E[1] /= T;
	E[2] /= T;
	return *this;
}

inline void Vec3::MakeUnitVector() {
	float k = 1.0 / Vec3Length();
	E[0] *= k;
	E[1] *= k;
	E[2] *= k;
}

inline float random_double() {
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(float min, float max) {
	// Returns a random real double in [min,max).
	return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
	// Returns a random real int in [min,max).
	return int(min + (max - min) * random_double());
}

inline Vec3 random() {
	return Vec3(random_double(), random_double(), random_double());
}

inline  Vec3 random(float min, float max) {
	return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline Vec3 random_in_unit_sphere() {
	while (true) {
		auto p = random(-1, 1);
		if (p.SquaredLength() >= 1) continue;
		return p;
	}
}


inline Vec3 randomInUnitDisk() {
	while (true) {
		auto p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.SquaredLength() >= 1) continue;
		return p;
	}
}


#pragma once
#include "Vec3.h"
#include "Ray.h"

class AABB {
public:
	Vec3 BorderMin, BorderMax;
public:
	AABB(){}
	AABB(const Vec3& a, const Vec3& b) : BorderMin(a), BorderMax(b) { }

	Vec3 min() const { return BorderMin; }
	Vec3 max() const { return BorderMax; }

	bool Hit(const Ray& r, double tMin, double tMax) const {
		for (int i = 0; i < 3; ++i) // 3 dimension
		{
			auto invD = 1.0 / r.Direction()[i];
			auto t0 = (min()[i] - r.Origin()[i]) * invD;
			auto t1 = (max()[i] - r.Origin()[i]) * invD;
			if (invD < 0.0) std::swap(t0, t1);
			tMin = t0 > tMin ? t0 : tMin;
			tMax = t1 < tMax ? t1 : tMax;
			if (tMax <= tMin) return false;
		}
		return true;
	}
};

AABB SurroundingBox(AABB Box0, AABB Box1) {
	Vec3 Small(fmin(Box0.min().X(), Box1.min().X()),
		fmin(Box0.min().Y(), Box1.min().Y()),
		fmin(Box0.min().Z(), Box1.min().Z()));
	Vec3 Big(fmax(Box0.max().X(), Box1.max().X()),
		fmax(Box0.max().Y(), Box1.max().Y()),
		fmax(Box0.max().Z(), Box1.max().Z()));
	return AABB(Small, Big);
}

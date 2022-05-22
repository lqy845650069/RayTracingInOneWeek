#pragma once

#include"Ray.h"
#include"Utils.h"
class Camera {
public:
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 up, float vFov, float Aspect, float Aperture, float focusDist, double t0, double t1) {
		Origin = lookFrom;
		lensRadius = Aperture / 2;

		time0 = t0;
		time1 = t1;

		auto theta = degreesToRadians(vFov);
		auto halfHeight = tan(theta / 2);
		auto halfWidth = Aspect * halfHeight;

		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(Cross(up, w));
		v = UnitVector(Cross(w, u));



		LowerLeftCorner = Origin - halfWidth * u * focusDist - halfHeight * v * focusDist - w * focusDist;
		Horizontal = 2 * halfWidth * u * focusDist;
		Vertical = 2 * halfHeight * v * focusDist;
	}
	Ray GetRay(float U, float V) {
		Vec3 rd = lensRadius * randomInUnitDisk();
		Vec3 offset = u * rd.X() + v * rd.Y();
		return Ray(Origin + offset, LowerLeftCorner + U * Horizontal + V * Vertical - Origin - offset, random_double(time0, time1));
	}

	Vec3 LowerLeftCorner;
	Vec3 Horizontal;
	Vec3 Vertical;
	Vec3 Origin;
	Vec3 u, v, w;
	float lensRadius;
	double time0;
	double time1;
};
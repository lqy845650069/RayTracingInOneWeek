#pragma once
#include"Hitable.h"
#include"HitableList.h"
class BVH_Node : public Hitable {
public:
	shared_ptr<Hitable> left;
	shared_ptr<Hitable> right;
	AABB Box;
public:
	BVH_Node();
	//BVH_Node(const HitableList& List, double time0, double time1) : BVH_Node(List.objects, 0, List.objects.size(), time0, time1){}
	BVH_Node(const std::vector<shared_ptr<Hitable>>& srcObjects, size_t start, size_t end, double time0, double time1);
	inline virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;
	inline virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
};

inline bool boxCmp(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b, int axis) {
	AABB boxA, boxB;
	if(!a->BoundingBox(0, 0, boxA))
}

inline bool BVH_Node::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
{
	if (!Box.Hit(r, tMin, tMax)) return false;

	bool hitL = left->Hit(r, tMin, tMax, rec);
	bool hitR = right->Hit(r, tMin, hitL ? rec.T : tMax, rec);
	return hitL || hitR;
}

inline bool BVH_Node::BoundingBox(double time0, double time1, AABB& outputBox) const
{
	outputBox = Box;
	return true;
}
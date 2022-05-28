#pragma once
#include"Hitable.h"
#include"HitableList.h"
#include<algorithm>

class BVH_Node : public Hitable {
public:
	shared_ptr<Hitable> left;
	shared_ptr<Hitable> right;
	AABB Box;
public:
	BVH_Node(){}
	BVH_Node(const std::vector<shared_ptr<Hitable>>& srcObjects, size_t start, size_t end, double time0, double time1);
	BVH_Node(const HitableList& List, double time0, double time1) : BVH_Node(List.objects, 0, List.objects.size(), time0, time1) {}
	inline virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;
	inline virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
};

inline bool boxCmp(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b, int axis) {
	AABB boxA, boxB;
	if (!a->BoundingBox(0, 0, boxA) || !b->BoundingBox(0, 0, boxB)) std::cerr << "No BoundingBox in BVH_Node constructior.\n";
	return boxA.min()[axis] < boxB.min()[axis];
}


inline BVH_Node::BVH_Node(const std::vector<shared_ptr<Hitable>>& srcObjects, size_t start, size_t end, double time0, double time1)
{
	std::vector<shared_ptr<Hitable>> tempObects = srcObjects;
	int axis = random_int(0, 2);
	auto cmp = boxCmp;
	size_t objectSpan = end - start;
	if (objectSpan == 1) left = right = tempObects[start];
	else if (objectSpan == 2) {
		if (cmp(tempObects[start], tempObects[start + 1], axis)) {
			left = tempObects[start];
			right = tempObects[start + 1];
		}
		else {
			left = tempObects[start + 1];
			right = tempObects[start];
		}
	}
	else {
		std::sort(tempObects.begin() + start, tempObects.begin() + end, [axis = axis](const shared_ptr<Hitable> a, const shared_ptr<Hitable> b) {return boxCmp(a, b, axis); });
		auto mid = start + objectSpan / 2;
		left = make_shared<BVH_Node>(tempObects, start, mid, time0, time1);
		right = make_shared<BVH_Node>(tempObects, mid, end, time0, time1);
	}

	AABB boxLeft, boxRight;
	if (!left->BoundingBox(0, 0, boxLeft) || !right->BoundingBox(0, 0, boxRight)) std::cerr << "No BoundingBox in BVH_Node constructior.\n";
	Box = SurroundingBox(boxLeft, boxRight);
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
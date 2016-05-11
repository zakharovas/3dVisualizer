//
// Created by User on 11.05.2016.
//

#include <algorithm>
#include "KdTree.h"


KdTree::KdTree(const std::vector<std::shared_ptr<Primitive>> &objects) : objects_(objects) {
    std::function<bool(const PointWithNumber &, const PointWithNumber &)> x_compare = [](const PointWithNumber &a,
                                                                                         const PointWithNumber &b) -> bool {
        return (a.point_.x < b.point_.x);
    };
    std::function<bool(const PointWithNumber &, const PointWithNumber &)> y_compare = [](const PointWithNumber &a,
                                                                                         const PointWithNumber &b) -> bool {
        return (a.point_.y < b.point_.y);
    };
    std::function<bool(const PointWithNumber &, const PointWithNumber &)> z_compare = [](const PointWithNumber &a,
                                                                                         const PointWithNumber &b) -> bool {
        return (a.point_.z < b.point_.z);
    };
    comparators_.push_back(x_compare);
    comparators_.push_back(y_compare);
    comparators_.push_back(z_compare);

}

void KdTree::BuildTree() {
    root_ = BuildVertex_(objects_, 0);

}

std::shared_ptr<Node> KdTree::BuildVertex_(const const std::vector<std::shared_ptr<Primitive>> &objects,
                                           size_t coordinate) {
    if (objects.size() == 0) {
        return nullptr;
    }
    std::vector<PointWithNumber> points;
    for (size_t i = 0; i < objects.size(); ++i) {
        Point important_point = objects[i]->GetImportantPoint();
        points.push_back(PointWithNumber(important_point, i));
    }
    std::nth_element(points.begin(), points.end(), points.begin() + points.size() / 2, comparators_[coordinate]);
    std::vector<std::shared_ptr<Primitive>> left_objects;
    std::vector<std::shared_ptr<Primitive>> right_objects;
    for (auto it = points.begin(); it != points.begin() + points.size() / 2 + 1; it++) {
        left_objects.push_back(objects[it->number]);
    }
    for (auto it = points.begin() + points.size() / 2 + 1; it != points.end(); it++) {
        right_objects.push_back(objects[it->number]);
    }
    std::shared_ptr<Node> left = BuildVertex_(left_objects, (coordinate + 1) % 3);
    std::shared_ptr<Node> right = BuildVertex_(right_objects, (coordinate + 1) % 3);
    
}





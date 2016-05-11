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

std::shared_ptr<KdTree::Node> KdTree::BuildVertex_(const std::vector<std::shared_ptr<Primitive>> &objects,
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
    std::vector<std::shared_ptr<Primitive>> own_objects;
    double imprtant_point;
    switch (coordinate) {
        case 0:
            imprtant_point = (points.begin() + points.size() / 2)->point_.x;
            break;
        case 1:
            imprtant_point = (points.begin() + points.size() / 2)->point_.y;
            break;
        case 2:
            imprtant_point = (points.begin() + points.size() / 2)->point_.z;
            break;
        default:
            imprtant_point = 0;
            break;
    }
    for (auto it = points.begin(); it != points.begin() + points.size() / 2 + 1; it++) {
        if (objects[it->number]->GetMaxCoordinate(coordinate) > imprtant_point) {
            own_objects.push_back(objects[it->number]);
        } else {
            left_objects.push_back(objects[it->number]);
        }
    }
    for (auto it = points.begin() + points.size() / 2 + 1; it != points.end(); it++) {
        if (objects[it->number]->GetMinCoordinate(coordinate) < imprtant_point) {
            own_objects.push_back(objects[it->number]);
        } else {
            right_objects.push_back(objects[it->number]);
        }
    }
    std::shared_ptr<Node> left = BuildVertex_(left_objects, (coordinate + 1) % 3);
    std::shared_ptr<Node> right = BuildVertex_(right_objects, (coordinate + 1) % 3);
    std::vector<double> max_coordinates;
    max_coordinates.assign(3, std::numeric_limits<double>::min());
    for (auto object: own_objects) {
        for (size_t i = 0; i < 3; ++i) {
            max_coordinates[i] = std::max(max_coordinates[i], object->GetMaxCoordinate(i));
        }
    }
    double max_x = max_coordinates[0];
    double max_y = max_coordinates[1];
    double max_z = max_coordinates[2];
    std::vector<double> min_coordinates;
    max_coordinates.assign(3, std::numeric_limits<double>::max());
    for (auto object: own_objects) {
        for (size_t i = 0; i < 3; ++i) {
            min_coordinates[i] = std::min(min_coordinates[i], object->GetMinCoordinate(i));
        }
    }
    double min_x = max_coordinates[0];
    double min_y = max_coordinates[1];
    double min_z = max_coordinates[2];
    UpdateExtremePoints_(left, max_x, max_y, max_z, min_x,
                         min_y, min_z);
    UpdateExtremePoints_(right, max_x, max_y, max_z, min_x,
                         min_y, min_z);
    std::shared_ptr<Node> this_node(new Node(left, right, own_objects, min_x, max_x, min_y, max_y, min_z, max_z));
    return this_node;
}

void KdTree::UpdateExtremePoints_(const std::shared_ptr<KdTree::Node> &vertex, double &max_x, double &max_y,
                                  double &max_z, double &min_x, double &min_y, double &min_z) const {
    if (vertex != nullptr) {
        min_x = std::min(vertex->min_x, min_x);
        min_y = std::min(vertex->min_y, min_y);
        min_z = std::min(vertex->min_z, min_z);
        max_x = std::max(vertex->max_x, max_x);
        max_y = std::max(vertex->max_y, max_y);
        max_z = std::max(vertex->max_z, max_z);
    }
}

std::shared_ptr<Primitive> KdTree::FindIntersection(const Ray &ray) const {
    return FindIntersection_(root_, ray);
}

std::shared_ptr<Primitive> KdTree::FindIntersection_(std::shared_ptr<Node> vertex, const Ray &ray) const {
    if (vertex == nullptr) {
        return nullptr;
    }
    if (!IntersectWithBoundingBox_(vertex, ray)) {
        return nullptr;
    }
    std::shared_ptr<Node> first;
    std::shared_ptr<Node> second;
    ObjectWithDistance current_best(nullptr, std::numeric_limits<double>::max());
    for (auto object: vertex->own_primitives) {
        if (object->TryToIntersect(ray)) {
            Point intersection = object->Intersect(ray);
            Vector normal = object->GetNormal(intersection);
            if (normal.DotProduct(ray.get_vector()) < -Primitive::kAccuracy) {
                double distance = (ray.get_vector() - intersection).Length();
                if (current_best.distance > distance) {
                    current_best.object = object;
                    current_best.distance = distance;
                }
            }
        }
    }
    first = vertex->left_;
    second = vertex->right;
    double first_intersection = TimeOfIntersectWithBoundingBox_(first, ray);
    double second_intersection = TimeOfIntersectWithBoundingBox_(second, ray);
    if (first_intersection > second_intersection) {
        std::swap(first, second);
    }
    std::shared_ptr<Primitive> childrens_object = FindIntersection_(first, ray);
    if (childrens_object == nullptr) {
        childrens_object = FindIntersection_(second, ray);
    }
    if (childrens_object == nullptr) {
        return current_best.object;
    }
    double distance_to_childrens_object = (childrens_object->Intersect(ray) - ray.get_point()).Length();
    if (distance_to_childrens_object > current_best.distance) {
        return current_best.object;
    } else {
        return childrens_object;
    }
}

bool KdTree::IntersectWithBoundingBox_(std::shared_ptr<Node> node, const Ray &ray) const {
    double min_t = std::numeric_limits<double>::min();
    double max_t = std::numeric_limits<double>::max();
    if (fabs(ray.get_vector().x) < Primitive::kAccuracy) {
        if (ray.get_point().x > node->max_x + Primitive::kAccuracy ||
            ray.get_point().x < node->min_x - Primitive::kAccuracy) {
            return false;
        }
    } else {
        double t1 = (node->max_x - ray.get_point().x) / ray.get_vector().x;
        double t2 = (node->min_x - ray.get_point().x) / ray.get_vector().x;
        min_t = std::max(min_t, std::min(t1, t2));
        max_t = std::min(max_t, std::max(t1, t2));
    }
    if (fabs(ray.get_vector().y) < Primitive::kAccuracy) {
        if (ray.get_point().y > node->max_y + Primitive::kAccuracy ||
            ray.get_point().y < node->min_y - Primitive::kAccuracy) {
            return false;
        }
    } else {
        double t1 = (node->max_y - ray.get_point().y) / ray.get_vector().y;
        double t2 = (node->min_y - ray.get_point().y) / ray.get_vector().y;
        min_t = std::max(min_t, std::min(t1, t2));
        max_t = std::min(max_t, std::max(t1, t2));
    }
    if (fabs(ray.get_vector().z) < Primitive::kAccuracy) {
        if (ray.get_point().z > node->max_z + Primitive::kAccuracy ||
            ray.get_point().z < node->min_z - Primitive::kAccuracy) {
            return false;
        }
    } else {
        double t1 = (node->max_z - ray.get_point().z) / ray.get_vector().z;
        double t2 = (node->min_z - ray.get_point().z) / ray.get_vector().z;
        min_t = std::max(min_t, std::min(t1, t2));
        max_t = std::min(max_t, std::max(t1, t2));
    }

    return (max_t > -Primitive::kAccuracy) && (max_t > min_t - Primitive::kAccuracy);
}

double KdTree::TimeOfIntersectWithBoundingBox_(std::shared_ptr<Node> node, const Ray &ray) const {
    double min_t = std::numeric_limits<double>::min();
    double max_t = std::numeric_limits<double>::max();
    if (fabs(ray.get_vector().x) < Primitive::kAccuracy) {
        if (ray.get_point().x > node->max_x + Primitive::kAccuracy ||
            ray.get_point().x < node->min_x - Primitive::kAccuracy) {
            return false;
        }
    } else {
        double t1 = (node->max_x - ray.get_point().x) / ray.get_vector().x;
        double t2 = (node->min_x - ray.get_point().x) / ray.get_vector().x;
        min_t = std::max(min_t, std::min(t1, t2));
        max_t = std::min(max_t, std::max(t1, t2));
    }
    if (fabs(ray.get_vector().y) < Primitive::kAccuracy) {
        if (ray.get_point().y > node->max_y + Primitive::kAccuracy ||
            ray.get_point().y < node->min_y - Primitive::kAccuracy) {
            return false;
        }
    } else {
        double t1 = (node->max_y - ray.get_point().y) / ray.get_vector().y;
        double t2 = (node->min_y - ray.get_point().y) / ray.get_vector().y;
        min_t = std::max(min_t, std::min(t1, t2));
        max_t = std::min(max_t, std::max(t1, t2));
    }
    if (fabs(ray.get_vector().z) < Primitive::kAccuracy) {
        if (ray.get_point().z > node->max_z + Primitive::kAccuracy ||
            ray.get_point().z < node->min_z - Primitive::kAccuracy) {
            return false;
        }
    } else {
        double t1 = (node->max_z - ray.get_point().z) / ray.get_vector().z;
        double t2 = (node->min_z - ray.get_point().z) / ray.get_vector().z;
        min_t = std::max(min_t, std::min(t1, t2));
        max_t = std::min(max_t, std::max(t1, t2));
    }
    if ((max_t > -Primitive::kAccuracy) && (max_t > min_t - Primitive::kAccuracy)) {
        return std::numeric_limits<double>::max();
    }
    return std::max(0.0, min_t);
}

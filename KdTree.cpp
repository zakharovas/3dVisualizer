//
// Created by User on 11.05.2016.
//

#include <algorithm>
#include <iostream>
#include <limits>
#include <future>
#include "KdTree.h"

KdTree::KdTree(const std::vector<std::shared_ptr<Primitive>> &objects) : objects_(objects), kNumberOfSplitPoints(10) {
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
    root_ = BuildVertex_(objects_);
}

std::shared_ptr<KdTree::Node> KdTree::BuildVertex_(const std::vector<std::shared_ptr<Primitive>> &objects) {
    if (objects.size() > 200) {
        std::cout << objects.size() << std::endl;
    }
    if (objects.size() == 0) {
        return nullptr;
    }
    Splitter split = Split_(objects);
    std::vector<std::shared_ptr<Primitive>> left_objects;
    std::vector<std::shared_ptr<Primitive>> right_objects;
    std::vector<std::shared_ptr<Primitive>> own_objects;
    for (auto object: objects) {
        if (object->GetMinCoordinate(split.coordinate) < split.plain + Primitive::kAccuracy) {
            left_objects.push_back(object);
        }
        if (object->GetMaxCoordinate(split.coordinate) > split.plain - Primitive::kAccuracy) {
            right_objects.push_back(object);
        }
    }
    if (CountCost_(objects) < CountCost_(left_objects) + CountCost_(right_objects)) {
        std::vector<double> max_coordinates(3, -std::numeric_limits<double>::max());
        std::vector<double> min_coordinates(3, std::numeric_limits<double>::max());
        UpdateExtremePointsOnObjects_(objects, max_coordinates, min_coordinates);
        std::shared_ptr<Node> this_node(
                new Node(nullptr, nullptr, objects, min_coordinates[0], max_coordinates[0], min_coordinates[1],
                         max_coordinates[1], min_coordinates[2], max_coordinates[2]));
        return this_node;
    }
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    left = BuildVertex_(left_objects);
    right = BuildVertex_(right_objects);
    std::vector<double> max_coordinates(3, -std::numeric_limits<double>::max());
    double max_x = max_coordinates[0];
    double max_y = max_coordinates[1];
    double max_z = max_coordinates[2];
    std::vector<double> min_coordinates(3, std::numeric_limits<double>::max());
    double min_x = min_coordinates[0];
    double min_y = min_coordinates[1];
    double min_z = min_coordinates[2];
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
                double distance = (ray.get_point() - intersection).Length();
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
    if (node == nullptr) {
        return false;
    }
    double min_t = -std::numeric_limits<double>::max();
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

    return ((max_t > -Primitive::kAccuracy) && (max_t > min_t - Primitive::kAccuracy));
}

double KdTree::TimeOfIntersectWithBoundingBox_(std::shared_ptr<Node> node, const Ray &ray) const {
    if (node == nullptr) {
        return std::numeric_limits<double>::max();
    }
    double min_t = -std::numeric_limits<double>::max();
    double max_t = std::numeric_limits<double>::max();
    if (fabs(ray.get_vector().x) < Primitive::kAccuracy) {
        if (ray.get_point().x > node->max_x + Primitive::kAccuracy ||
            ray.get_point().x < node->min_x - Primitive::kAccuracy) {
            return std::numeric_limits<double>::max();
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
            return std::numeric_limits<double>::max();
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
            return std::numeric_limits<double>::max();
        }
    } else {
        double t1 = (node->max_z - ray.get_point().z) / ray.get_vector().z;
        double t2 = (node->min_z - ray.get_point().z) / ray.get_vector().z;
        min_t = std::max(min_t, std::min(t1, t2));
        max_t = std::min(max_t, std::max(t1, t2));
    }
    if ((max_t < -Primitive::kAccuracy) || (max_t < min_t - Primitive::kAccuracy)) {
        return std::numeric_limits<double>::max();
    }
    return std::max(0.0, min_t);
}

void KdTree::UpdateExtremePointsOnObjects_(const std::vector<std::shared_ptr<Primitive>> &objects,
                                           std::vector<double> &max_coordinates, std::vector<double> &min_coordinates) {
    for (auto object: objects) {
        for (size_t i = 0; i < 3; ++i) {
            max_coordinates[i] = std::max(max_coordinates[i], object->GetMaxCoordinate(i));
            min_coordinates[i] = std::min(min_coordinates[i], object->GetMinCoordinate(i));
        }
    }

}

KdTree::Splitter KdTree::Split_(const std::vector<std::shared_ptr<Primitive>> &objects) {
    std::vector<double> max_coordinates(3, -std::numeric_limits<double>::max());
    std::vector<double> min_coordinates(3, std::numeric_limits<double>::max());
    UpdateExtremePointsOnObjects_(objects, max_coordinates, min_coordinates);
    Splitter current_best(0, 0);
    double cost = std::numeric_limits<double>::max();
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < kNumberOfSplitPoints; ++j) {
            Splitter splitter(i, min_coordinates[i] + (j + 1) * (max_coordinates[i] - min_coordinates[i]) /
                                                      (kNumberOfSplitPoints + 1));
            std::vector<std::shared_ptr<Primitive> > left;
            std::vector<std::shared_ptr<Primitive> > right;
            for (auto object : objects) {
                if (object->GetMaxCoordinate(splitter.coordinate) > splitter.plain) {
                    left.push_back(object);
                }
                if (object->GetMinCoordinate(splitter.coordinate) < splitter.plain) {
                    right.push_back(object);
                }
            }
            double cost_of_split = CountCost_(left) + CountCost_(right);
            if (cost_of_split < cost) {
                current_best = splitter;
                cost = cost_of_split;
            }
        }
    }
    return current_best;
}

double KdTree::CountCost_(const std::vector<std::shared_ptr<Primitive>> &objects) {
    std::vector<double> max_coordinates(3, -std::numeric_limits<double>::max());
    std::vector<double> min_coordinates(3, std::numeric_limits<double>::max());
    UpdateExtremePointsOnObjects_(objects, max_coordinates, min_coordinates);
    double cost = 0;
    double current_cost = 0;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            if (i == j) continue;
            current_cost += (max_coordinates[i] - min_coordinates[i]) * (max_coordinates[j] - min_coordinates[j]);
        }
    }
    cost += objects.size() * current_cost;
    return cost;
}












//
// Created by User on 11.05.2016.
//

#ifndef INC_3DVISUALIZER_KDTREE_H
#define INC_3DVISUALIZER_KDTREE_H


#include <memory>
#include <vector>
#include "Primitive.h"

class KdTree {
private:
    struct Node;

public:
    KdTree(const std::vector<std::shared_ptr<Primitive>> &objects);

    void BuildTree();

    std::shared_ptr<Primitive> FindIntersection(const Ray &ray) const;


private:
    struct Node {
        Node(const std::shared_ptr<Node> &left_, const std::shared_ptr<Node> &right,
             const std::vector<std::shared_ptr<Primitive>> &own_primitives, double min_x, double max_x, double min_y,
             double max_y, double min_z, double max_z) : left_(left_), right(right), own_primitives(own_primitives),
                                                         min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y),
                                                         min_z(min_z), max_z(max_z) { }

        std::shared_ptr<Node> left_;
        std::shared_ptr<Node> right;
        std::vector<std::shared_ptr<Primitive>> own_primitives;
        double min_x;
        double max_x;
        double min_y;
        double max_y;
        double min_z;
        double max_z;
    };

    struct PointWithNumber {
        PointWithNumber(const Point &point_, size_t number) : point_(point_), number(number) { }

        Point point_;
        size_t number;
    };

    struct ObjectWithDistance {
        ObjectWithDistance(const std::shared_ptr<Primitive> &object, double distance) : object(object),
                                                                                        distance(distance) { }

        std::shared_ptr<Primitive> object;
        double distance;
    };

    struct Splitter {
        Splitter(size_t coordinate, double plain) : coordinate(coordinate), plain(plain) { }

        double plain;
        size_t coordinate;
    };

    const double kNumberOfSplitPoints;

    std::vector<std::shared_ptr<Primitive>> objects_;
    std::shared_ptr<Node> root_;
    std::vector<std::function<bool(const PointWithNumber &, const PointWithNumber &)>> comparators_;

    std::shared_ptr<KdTree::Node> BuildVertex_(const std::vector<std::shared_ptr<Primitive>> &objects);

    std::shared_ptr<Primitive> FindIntersection_(std::shared_ptr<Node> vertex, const Ray &ray) const;

    void UpdateExtremePoints_(const std::shared_ptr<Node> &vertex, double &max_x, double &max_y, double &max_z,
                              double &min_x, double &min_y, double &min_z) const;

    bool IntersectWithBoundingBox_(std::shared_ptr<Node> node, const Ray &ray) const;

    double TimeOfIntersectWithBoundingBox_(std::shared_ptr<Node> node, const Ray &ray) const;

    void UpdateExtremePointsOnObjects_(const std::vector<std::shared_ptr<Primitive>> &objects,
                                       std::vector<double> &max_coordinates, std::vector<double> &min_coordinates);

    bool TryToSplit_(const std::vector<std::shared_ptr<Primitive>> &objects);

    Splitter Split_(const std::vector<std::shared_ptr<Primitive>> &objects);

    bool TryToSplit_(const Splitter &splitter, const std::vector<std::shared_ptr<Primitive>> &objects);

    double CountCost_(const std::vector<std::shared_ptr<Primitive>> &objects);
};


#endif //INC_3DVISUALIZER_KDTREE_H

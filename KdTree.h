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

    void FindIntersection(const Ray &ray) const;


private:
    struct Node {
        Node(const std::shared_ptr<Node> &left_, const std::shared_ptr<Node> &right,
             const std::vector<std::shared_ptr<Primitive>> &own_primitives) : left_(left_), right(right),
                                                                              own_primitives(own_primitives) { }

        std::shared_ptr<Node> left_;
        std::shared_ptr<Node> right;
        std::vector<std::shared_ptr<Primitive>> own_primitives;
    };

    struct PointWithNumber {
        PointWithNumber(const Point &point_, size_t number) : point_(point_), number(number) { }

        Point point_;
        size_t number;
    };

    std::vector<std::shared_ptr<Primitive>> objects_;
    std::shared_ptr<Node> root_;
    std::vector<std::function<bool(const PointWithNumber &, const PointWithNumber &)>> comparators_;

    std::shared_ptr<Node> BuildVertex_(const std::vector<std::shared_ptr<Primitive>> &objects, size_t coordinate);
};


#endif //INC_3DVISUALIZER_KDTREE_H

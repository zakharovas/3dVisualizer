//
// Created by User on 12.04.2016.
//

#ifndef INC_3DVISUALIZER_COLOR_H
#define INC_3DVISUALIZER_COLOR_H


class Color {
public:
    Color(double r, double g, double b) : r_(r), g_(g), b_(b) { };

    Color() : r_(0), g_(0), b_(0) { };

    unsigned int GetR() const;

    unsigned int GetG() const;

    unsigned int GetB() const;

    double get_r() const { return r_; };

    double get_g() const { return g_; }

    double get_b() const { return b_; }

private:
    double r_;
    double g_;
    double b_;
};


#endif //INC_3DVISUALIZER_COLOR_H

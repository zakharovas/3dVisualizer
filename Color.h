//
// Created by User on 12.04.2016.
//

#ifndef INC_3DVISUALIZER_COLOR_H
#define INC_3DVISUALIZER_COLOR_H

class HslColor;

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

    void AddLight(double lightness);

    HslColor ToHsl() const;

    static const Color kDefaultOutsideColor;

    static const Color kDefaultInsideColor;

private:
    double r_;
    double g_;
    double b_;


};

class HslColor {
public:
    HslColor(const Color &color);

    void AddLight(double lightness);

    Color ToRgb() const;

    void RemoveLight();

private:
    double h_;
    double s_;
    double l_;
};

#endif //INC_3DVISUALIZER_COLOR_H

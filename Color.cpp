//
// Created by User on 12.04.2016.
//

#include <cmath>
#include <algorithm>
#include "Color.h"

double kEpsilon = 1e-6;

unsigned int Color::GetR() const {
    return static_cast<unsigned int>(round(r_));
}

unsigned int Color::GetG() const {
    return static_cast<unsigned int>(round(g_));
}

unsigned int Color::GetB() const {
    return static_cast<unsigned int>(round(b_));
}

void Color::AddLight(double lightness) {
    HslColor new_color = ToHsl();
    new_color.AddLight(lightness);
    Color new_rgb_color = new_color.ToRgb();
    r_ = new_rgb_color.get_r();
    g_ = new_rgb_color.get_g();
    b_ = new_rgb_color.get_b();
}

Color::HslColor Color::ToHsl() const {
    return Color::HslColor(Color(r_, g_, b_));
}

//formula from ru.wikipedia.org/wiki/HSL
Color::HslColor::HslColor(const Color &color) {
    double r = color.get_r() / 255.0;
    double g = color.get_g() / 255.0;
    double b = color.get_b() / 255.0;
    double max = std::max(std::max(r, g), b);
    double min = std::min(std::min(r, g), b);
    l_ = 0.5 * (max + min);
    s_ = (max - min) / (1 - fabs(1 - (max + min)));
    if (fabs(max - min) < kEpsilon) {
        h_ = 0;
    } else if (fabs(max - r) < kEpsilon) {
        if (g >= b) {
            h_ = 60 * (g - b) / (max - min);
        } else {
            h_ = 60 * (g - b) / (max - min) + 360;
        }
    } else if (fabs(max - g) < kEpsilon) {
        h_ = 60 * (b - r) / (max - min) + 120;
    } else {
        h_ = 60 * (r - g) / (max - min) + 240;
    }
}

void Color::HslColor::AddLight(double lightness) {
    l_ = std::min(1.0, l_ + lightness);
}

Color Color::HslColor::ToRgb() const {
    double q;
    if (l_ < 0.5) {
        q = l_ * (1.0 + s_);
    } else {
        q = l_ + s_ - (l_ * s_);
    }
    double p = 2.0 * l_ - q;
    double h = h_ / 360;
    double t_r = h + 1.0 / 3.0;
    std::vector<double> t; //vector containg t_r, t_g and t_b
    t.push_back(h + 1.0 / 3.0);
    t.push_back(h);
    t.push_back(h - 1.0 / 3.0);
    for (double &t_cur: t) {
        if (t_cur < 0) {
            t_cur += 1;
        }
        if (t_cur > 1) {
            t_cur -= 1;
        }
        if (t_cur < 1.0 / 6.0) {
            t_cur = p + ((q - p) * 6.0 * t_cur);
        } else if (t_cur < 1.0 / 2.0) {
            t_cur = q;
        } else if (t_cur < 2.0 / 3.0) {
            t_cur = p + ((q - p) * (2.0 / 3.0 - t_cur) * 6.0);
        } else {
            t_cur = p;
        }
    }
    double r = t[0] * 255;
    double g = t[1] * 255;
    double b = t[2] * 255;

    return Color(r, g, b);
}






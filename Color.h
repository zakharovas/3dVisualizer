//
// Created by User on 12.04.2016.
//

#ifndef INC_3DVISUALIZER_COLOR_H
#define INC_3DVISUALIZER_COLOR_H


class Color {
public:
    unsigned int GetR() const;

    unsigned int GetG() const;

    unsigned int GetB() const;

private:
    double r_;
    double g_;
    double b_;
};


#endif //INC_3DVISUALIZER_COLOR_H

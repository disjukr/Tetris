#pragma once

enum Color {
    NONE,
    WHITE,
    GREY,
    BLACK,
    BLUE,
    CYAN,
    GREEN,
    MAGENTA,
    RED,
    YELLOW
};

class ColorUtil {
public:
    static Color GetComplementary(Color sample) {
        switch (sample) {
        case NONE: return NONE;
        case WHITE: return BLACK;
        case GREY: return GREY;
        case BLACK: return WHITE;
        case BLUE: return YELLOW;
        case CYAN: return RED;
        case GREEN: return MAGENTA;
        case MAGENTA: return GREEN;
        case RED: return CYAN;
        case YELLOW: return BLUE;
        }
    }
};

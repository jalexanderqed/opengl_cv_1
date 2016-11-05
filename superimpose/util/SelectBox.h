//
// Created by john on 11/4/16.
//

#ifndef GLHW_01_SELECTBOX_H
#define GLHW_01_SELECTBOX_H


#include <jmorecfg.h>

class SelectBox {
public:
    SelectBox(int t, int d) {
        threshold = t;
        delay = d;
    }

    boolean updateState(int val) {
        boolean turnedOn = false;
        if (val >= threshold) {
            turnedOn = !clicked;
            updateCounter = 0;
            clicked = true;
        } else {
            if (updateCounter >= delay) {
                updateCounter = 0;
                clicked = false;
            } else {
                updateCounter++;
            }
        }

        return turnedOn;
    }

    bool isClicked() { return clicked; }

private:
    bool clicked = true;
    int threshold;
    int delay;
    int updateCounter = 0;
};


#endif //GLHW_01_SELECTBOX_H

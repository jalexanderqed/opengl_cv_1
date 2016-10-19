//
// Created by john on 10/18/16.
//

#ifndef GLHW_01_IMAGESQUARE_H
#define GLHW_01_IMAGESQUARE_H


class ImageSquare {
public:
    ImageSquare() {}

    ImageSquare(bool placeHolder);

    void draw();

private:
    GLuint VAO;
};


#endif //GLHW_01_IMAGESQUARE_H

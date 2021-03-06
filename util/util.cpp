#include "util.h"

string myReadFile(string name) {
    ifstream ifs(name);
    string content((istreambuf_iterator<char>(ifs)),
                   (istreambuf_iterator<char>()));
    return content;
}

GLint TextureFromFile(const char* path, string directory)
{
    //Generate texture ID and load texture data
    string filename = string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}

int sumPixels(int upperX, int upperY, int lowerX, int lowerY, cv::Mat& image) {
    int sum = 0;
    for (int i = upperY; i <= lowerY; i++) {
        uchar *row = image.ptr<uchar>(i);
        for (int j = upperX; j <= lowerX; j++) {
            sum += row[j];
        }
    }
    return sum;
}
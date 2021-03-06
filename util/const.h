//
// Created by john on 10/15/16.
//

#ifndef GLHW_01_CONST_H
#define GLHW_01_CONST_H

#define MODEL_MAT_NAME "modelMat"
#define VIEW_MAT_NAME "viewMat"
#define PROJECTION_MAT_NAME "projectionMat"
#define NORMAL_MODEL_MAT_NAME "normalModelMat"

enum shaderProgramsEnum{
    BASIC_PROGRAM = 0,
    MODEL_PROGRAM,
    IMAGE_PROGRAM,
    BASIC_GLOBAL_COLOR_PROGRAM,
    BASIC_GLOBAL_COLOR_NO_LIGHT_PROGRAM,
    SHADER_MAX
};

#endif //GLHW_01_CONST_H
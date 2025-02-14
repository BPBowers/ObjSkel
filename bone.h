#ifndef BONE_H_INCLUDED
#define BONE_H_INCLUDED

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <glm/gtx/string_cast.hpp>

#include "LoadShaders.h"
#include "ProgramDefines.h"
#include "Material.h"
#include "Light.h"
#include "Shape.h"

class objBone{
    private:
        GLuint vboptr;
        GLuint colorbufptr;
        GLuint bufptr;

        std::string baseFileName;
        std::string skelFileName;
        std::vector<glm::vec4> worldSpacePos;//For every vertex
        std::vector<glm::vec4> newNormal;
        //glm::vec4 localSpacePos;//This runs for every vertex
        //glm::mat4 bindMatrix;//Regular position of the
        //glm::mat4 jointAttachWorld;//This is what is changed in keyframes to create rotations
        std::vector<glm::mat4> jointAttachWorld;
        std::vector<glm::mat4> changeMachine;//Initially equal to jointAttachWorld, is changed to alter the bones and verticies
        std::vector<float> weight;//Different for each bone.
        int totalJoints = 0;
        int totalVerts = 0;

        bool firstTime = true;

    public:
        objBone();
        ~objBone();

        std::vector<glm::vec3> ogVert;

        void exportBoneData();
        void calcWorldSpacePos(std::vector<glm::vec3> verticies);
        glm::mat4 calcBindMatrix(int jointNum);

        void setJoint(float x, float y, float z);
        void setFileName(std::string fn);

        void updateWeights();

        std::vector<glm::vec3> getVertPrime()
        {
            std::vector<glm::vec3> vertPrime;
            glm::vec3 tempVec;
            for(int i = 0; i < totalVerts; i++)
            {
                tempVec.x = worldSpacePos[i].x;
                tempVec.y = worldSpacePos[i].y;
                tempVec.z = worldSpacePos[i].z;
                vertPrime.push_back(tempVec);
            }
            return vertPrime;
        }

        std::vector<glm::vec3> getNormPrime()
        {
            std::vector<glm::vec3> normPrime;
            glm::vec3 tempVec;
            for(int i = 0; i < totalVerts; i++)
            {
                tempVec.x = newNormal[i].x;
                tempVec.y = newNormal[i].y;
                tempVec.z = newNormal[i].z;
                normPrime.push_back(tempVec);
            }
            return normPrime;
        }

        void initChangeMachine();
        void rotateMore();

        void draw();

        void setUpDisplayBone();

};

#endif // BONE_H_INCLUDED

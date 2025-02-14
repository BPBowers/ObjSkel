/*
    Was Originally Included with a pre-existing OpenGL Engine
    But has been seperated, this code will not run on its own.
    Uses OpenGL for matrix library
*/

#include "bone.h"

//While it is called "Bone", it contains all joint pairs

objBone::objBone()
{
    //Initialize all of the stuff to be empty dum dum
    weight.clear();
    worldSpacePos.clear();
    newNormal.clear();
    jointAttachWorld.clear();
    changeMachine.clear();
    ogVert.clear();
    //totalJoints = 0;
}

objBone::~objBone()
{
    weight.clear();
    worldSpacePos.clear();
    newNormal.clear();
    jointAttachWorld.clear();
    changeMachine.clear();
    ogVert.clear();
    //weight.clear();
}

//Writes the Bone Skeletal data to the "{objFileName}Skel.txt" file
void objBone::exportBoneData()
{
    std::ofstream outFile(skelFileName);
    for (int i = 0; i < totalJoints; i++)
    {
        outFile << "b " << glm::to_string(jointAttachWorld[i]) << "\n";
        outFile << "i " << glm::to_string(calcBindMatrix(i)) << "\n";
    }
    //Printed for debug to make sure all verts are back to reg when all weights applied
    for(int i = 0; i < totalVerts; i++)
        outFile << "v " << worldSpacePos[i].x << " " << worldSpacePos[i].y << " " << worldSpacePos[i].z << " " << worldSpacePos[i].w << "\n";
}

//Take in the vector of vertices from objModel to calculate the new position of each vertex in the world after transformation is performed
void objBone::calcWorldSpacePos(std::vector<glm::vec3> vertices)
{
    glm::vec4 blank(0.0);
    totalVerts = vertices.size();
    if(firstTime)
    {
        initChangeMachine();
        ogVert = vertices;
        firstTime = false;
    }
    //if(worldSpacePos.size() != totalVerts)
    //{
        worldSpacePos.clear();
        for(int i = 0; i < totalVerts; i++)
            worldSpacePos.push_back(blank);
    //}
    glm::vec4 tempVec(vertices[0].x, vertices[0].y, vertices[0].z, 1);
    worldSpacePos[0] = worldSpacePos[0] + (weight[0] * tempVec) * (calcBindMatrix(0) * jointAttachWorld[0]);
    for (int vert = 0; vert < totalVerts; vert++)
    {
        glm::vec4 tempVec(vertices[vert].x, vertices[vert].y, vertices[vert].z, 1);
        for(int jointN = 1; jointN < totalJoints; jointN++)
            worldSpacePos[vert] = worldSpacePos[vert] + (weight[jointN] * tempVec) * (calcBindMatrix(jointN) * changeMachine[jointN]);
            //worldSpacePos[vert] = worldSpacePos[vert] + (weight[jointN] * tempVec) * (calcBindMatrix(jointN) * jointAttachWorld[jointN]);
            //worldSpacePos[vert] = blank * (calcBindMatrix(jointN) * jointAttachWorld[jointN]);
            //worldSpacePos[vert] = tempVec;
    }
}
/*
glm::mat4 objBone::calcBindMatrix(glm::vec4 vert, int vertNum)
{
    glm::mat4 bindMat = jointAttachWorld[vertNum];
    glm::mat4 invBind = glm::inverse(bindMat);
    return invBind;
}
*/
glm::mat4 objBone::calcBindMatrix(int jointNum)
{
    glm::mat4 invMat = glm::inverse(jointAttachWorld[jointNum]);
    return invMat;
}
void objBone::setJoint(float x, float y, float z)
{
    glm::mat4 tempJoint;
    //Row 1
    tempJoint[0][0] = 1;
    tempJoint[0][1] = 0;
    tempJoint[0][2] = 0;
    tempJoint[0][3] = 0;
    //Row 2
    tempJoint[1][0] = 0;
    tempJoint[1][1] = 1;
    tempJoint[1][2] = 0;
    tempJoint[1][3] = 1;
    //Row Three
    tempJoint[2][0] = 0;
    tempJoint[2][1] = 0;
    tempJoint[2][2] = 1;
    tempJoint[2][3] = 0;
    //Row Four (Where you actually store the joint location)
    tempJoint[3][0] = x;
    tempJoint[3][1] = y;
    tempJoint[3][2] = z;
    tempJoint[3][3] = 1;
    jointAttachWorld.push_back(tempJoint);
    totalJoints++;
    updateWeights();
    //exportBoneData();
}

void objBone::setFileName(std::string fn)
{
    baseFileName = fn;
    skelFileName = baseFileName.substr(0, baseFileName.find_last_of('.'));
    skelFileName += "Skel.txt";
    exportBoneData();
}

void objBone::updateWeights()
{
    float disWeight = (1/(float)totalJoints);
    weight.clear();
    for(int i = 0; i < totalJoints; i++){
        weight.push_back(disWeight);
        // std::cout << weight[i] << std::endl;
    }
}

void objBone::initChangeMachine()
{
    changeMachine = jointAttachWorld;
    glm::mat4 tempMat = changeMachine[0];
    //tempMat[1][1] = cos(30);
    //tempMat[1][2] = sin(30);
    //tempMat[2][1] = -sin(30);
    //tempMat[2][2] = cos(30);
    //tempMat = glm::rotate(tempMat, 30*degf, glm::vec3(1, 0, 0));
    changeMachine[0] = tempMat;
    //std::cout<< glm::to_string(changeMachine[0]) << std::endl;
}

void objBone::rotateMore()
{
    changeMachine[0] = glm::rotate(changeMachine[0], 30*degf, glm::vec3(1, 0, 0));
    //for(int i = 1; i < jointAttachWorld.size(); i++)
    //    changeMachine[i] *= changeMachine[i-1];
    //std::cout<< glm::to_string(changeMachine[0]) << std::endl;
}

void objBone::draw()
{
    glBindVertexArray(vboptr);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trackEBO);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glDrawArrays(GL_LINES, 0, totalJoints);
}

void objBone::setUpDisplayBone()
{
    glGenVertexArrays(1, &vboptr);
    glGenBuffers(1, &colorbufptr);
    glGenBuffers(1, &bufptr);
    GLuint vPosition = 0;
    GLuint vColor = 1;
    std::cout<< totalJoints << std::endl;
    glm::mat4 tempMat;
    GLfloat origin[totalJoints*4];
    for(int i = 0; i < totalJoints; i++)
    {
        tempMat = jointAttachWorld[i];
        origin[i] = tempMat[3][0];
        origin[i+1] = tempMat[3][1];
        origin[i+1] = tempMat[3][2];
        origin[i+1] = tempMat[3][3];
        //origin[i+1] = tempMat[3][0];
        //origin[i+1] = tempMat[3][1]+0.5;
        //origin[i+1] = tempMat[3][2];
        //origin[i+1] = tempMat[3][3];
    }
    GLfloat colors[3*totalJoints];
    for (int i = 0; i < totalJoints; i++)
    {
        colors[3*i] = 1;
        colors[3*i+1] = 1;
        colors[3*i+2] = 1;
    }
    glBindVertexArray(vboptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(origin), origin, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glBindBuffer(GL_ARRAY_BUFFER, colorbufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

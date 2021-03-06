#ifndef H_MESHDATA
#define H_MESHDATA

#include "msbObject.h"

#define MESH_VBO 0
#define MESH_POINTSPRITE 1


class MeshData: public MsbObject{

public:



    std::vector<int>    vertexCount;                    //how many vertices does this mesh have
    std::vector<GLuint> vertexIDObject;
    std::vector<GLuint> vertexBufferObject;
    std::vector<GLuint> normalBufferObject;
    std::vector<GLuint> colorBufferObject;
    std::vector<GLuint> secondaryColorBufferObject;
    std::vector<GLuint> texCoordBufferObject;

    std::vector<GLuint> vertexWeightsObject; //Vertex Bone Weights 1-4
    std::vector<GLuint> boneReferenceObject; //Bone References

    std::vector<vertexData> vData;

    bool bIsSkeletal;                       //has bones and stuff
    bool bIsHead;                           // to be implemented someday?
    bool bVertexColor;                      //uses vertex color object - COLLADA doesn't use this usually
    bool bEditable;                         //can be changed within MSB - COLLADA cannot be changed for now

    bool bUnsavedChanges;           //any changes that have not been saved yet?

    int boneCount;                          //how many bones
    int texCoordPerVertexCount;             //how many texture coordinaes per vertex?
    int verticesPerShapeCount;              //TODO: should be "numbers per Vertex count" - 3 or 4
    int meshType;                               //how to interpret data? Used by renderer to determine spritemesh and could be used for triangulation or similar things...

    GLenum  vertexInterpretation;           //vertices interpreted as QUADS or TRIANGLES
    std::vector<bone*>   bones;
    Matrix4f* bindShapeMatrix;              //points to an array of bone matrices (according to number of bones in boneCount)

    //bounding box related
    Vector3f    lowerLeftBack,                  //point starts lower left hand corner (as seen from user)
                upperRightFront,
                center;                     //center of bounding box!


    MeshData();
    virtual ~MeshData();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(MsbObject* other);

    virtual void update(double deltaTime);

};
#endif // MESHDATA

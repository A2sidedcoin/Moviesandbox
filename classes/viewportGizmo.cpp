

#include "viewportGizmo.h"
#include "renderer.h"
#include "input.h"
ViewportGizmo::ViewportGizmo(){

    name="View Gizmo";
    textureID="viewportGizmo";
    bTextured=true;
    sceneShaderID="buttonTexture";
    viewport_fb = 0;
    cubeSide=0;

    texRotation=Vector3f(180,0,0);
    texTranslation=Vector3f(0,1,0);
    texScale=Vector3f(1,1,1);
    registerProperties();
}

ViewportGizmo::~ViewportGizmo(){}


void ViewportGizmo::setup(){

    BasicButton::setup();
    mouseOverColor=buttonColor;

    //create a 100x100 texture
    renderer->createEmptyTexture("viewportGizmo",GL_RGBA,GL_FLOAT,84,84);

    //also, create an FBO
    renderer->createFBO(&viewport_fb,&(sceneData->textureList["viewportGizmo"]->texture),&myDepthBuffer,84 ,84,false, "viewportGizmo");


    scale.x=84;
    scale.y=84;

}

void ViewportGizmo::update(double deltaTime){

    BasicButton::update(deltaTime);

    setLocation(Vector3f( renderer->windowX-100, -10, 0.0));

        //setup FBO

    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0,0,84 * renderer->screenX/renderer->screenY,84);
    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, viewport_fb);
    //renderer->setupOrthoCamera(64,64);

    glDrawBuffers(1,renderer->drawBuffers);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    renderer->setupOrthoCamera(renderer->screenX,renderer->screenY);
    renderer->setupShading("colorAlphaID");
    updateShaders();

    //construct matrix
    Matrix4f myMatrix=sceneData->controller->controlledActor->baseMatrix;
    myMatrix.setTranslation(Vector3f(0,0,0));
    myMatrix = myMatrix.inverse();
    myMatrix.setTranslation(Vector3f(250,400,0));
    glMultMatrixf(myMatrix);

    renderer->drawCube(90.0);
    glEnable(GL_BLEND);
    glBindFramebufferEXT (GL_FRAMEBUFFER_EXT,0);
    glPopAttrib();

}

void ViewportGizmo::mouseOver(){

    BasicButton::mouseOver();

    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, viewport_fb);

    renderer->setupOrthoCamera(64,64);

    //texTranslation.x-=0.01;


    GLuint pickTexture;
    //setup picking texture!
    glGenTextures(1, &pickTexture);
    glBindTexture(GL_TEXTURE_2D, pickTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, renderer->depthPrecision,  1, 1, 0, GL_BGRA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //create small picking texture
    glBindTexture(GL_TEXTURE_2D,pickTexture);

    float cubeRead[4];
    glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,input->mouseX-location.x,(location.y+scale.y)-input->mouseY,1 ,1 );
    glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,&cubeRead[0]);

    if ((int) (cubeRead[0]*100+ 0.5)==85)
        cubeSide=1;
    if ((int) (cubeRead[0]*100+ 0.5)==75)
        cubeSide=2;
    if ((int) (cubeRead[0]*100+ 0.5)==70)
        cubeSide=3;
    if ((int) (cubeRead[0]*100+ 0.5)==65)
        cubeSide=4;
    if ((int) (cubeRead[0]*100+ 0.5)==55)
        cubeSide=5;
    if ((int) (cubeRead[0]*100.0 + 0.5)==90)
        cubeSide=6;

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);

    //cout << cubeSide << "aa:v " << cubeRead[0] << " " << cubeRead[1] << " " << cubeRead[2] << endl;

}

void ViewportGizmo::mouseDrag(){}
void ViewportGizmo::finishDrag(){}

void ViewportGizmo::clickedLeft(){


    //find center
    Vector3f oldV, newV, driftV;

    oldV=input->center3D;

    cout << "old Vector: " << oldV << endl;
    //depending on where we clicked, this Vector should be different

    //also, switching left and right...

    if (cubeSide==1){    //front
        newV=sceneData->controller->controlledActor->location + Vector3f(0,0,1* oldV.length());
        sceneData->controller->controlledActor->setRotation(Vector3f(1,0,0),Vector3f(0,1,0),Vector3f(0,0,1));
    }

    if (cubeSide==2){   //right
        newV=sceneData->controller->controlledActor->location + Vector3f(1* oldV.length(),0,0);
        sceneData->controller->controlledActor->setRotation(Vector3f(0,0,-1),Vector3f(0,1,0),Vector3f(1,0,0));
    }

    if (cubeSide==4){   //top
        newV=sceneData->controller->controlledActor->location + Vector3f(0,-1* oldV.length(),0);
        sceneData->controller->controlledActor->setRotation(Vector3f(1,0,0),Vector3f(0,0,1),Vector3f(0,-1,0));
        //add a tiny bit of rotation, so we don't deadlock...
        sceneData->controller->controlledActor->addRotation(-1.0, sceneData->controller->controlledActor->xAxis);

    }

    if (cubeSide==3){   //bottom
        newV=sceneData->controller->controlledActor->location + Vector3f(0,1* oldV.length(),0);
        sceneData->controller->controlledActor->setRotation(Vector3f(1,0,0),Vector3f(0,0,-1),Vector3f(0,1,0));
    }

    if (cubeSide==5){   //left
        newV=sceneData->controller->controlledActor->location + Vector3f(-1* oldV.length(),0,0);
        sceneData->controller->controlledActor->setRotation(Vector3f(0,0,1),Vector3f(0,1,0),Vector3f(-1,0,0));
    }


    driftV= newV-oldV;

    sceneData->controller->controlledActor->setLocation(sceneData->controller->controlledActor->location - driftV);
    sceneData->updateView();
    //int oldTool=sceneData->controller->tool;
    //sceneData->controller->switchTool(TOOL_NAV);
    //sceneData->controller->switchTool(oldTool);


}

void ViewportGizmo::clickedRight(){}
void ViewportGizmo::focusClick(){

    BasicButton::focusClick();
}

void ViewportGizmo::deselect(int depth){

    BasicButton::deselect(depth);
}

void ViewportGizmo::create(){sceneData->addButton(this);}

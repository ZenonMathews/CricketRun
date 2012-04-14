/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.h
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreSceneNode.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>

#include <qstring.h>




//YARP includes
#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>
// ZM: QT includes
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<qstring.h>
#include<qstringlist.h>
#include <OgreMath.h>
#include "Euler.h"
#include <OgreQuaternion.h>

#include<map>

using namespace yarp::os;


struct YarpSceneObject { //
  int objType; // 0 for cam, 10-100 for cylinders, 
  float x;   // x coordinate
  float y;   // y coordinate
  float z;     // z coordinate
  
  //quarternion xaxis, yaxis, zaxis
  float q1,q2,q3,q4,q5,q6,q7,q8,q9;

  // object ID is the map key itself
};


double const lowPassVal = 0.9; // ZM



class BaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    BaseApplication(void);
    virtual ~BaseApplication(void);

    virtual void go(void);

    Network yarp;
    BufferedPort<Bottle> yarpInputPortZ;
    BufferedPort<Bottle> yarpInputPortYaw;
    

protected:
    virtual bool setup();
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void createScene(void) = 0; // Override me!
    virtual void destroyScene(void);
    virtual void createViewports(void);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Ogre::WindowEventListener
    //Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);
   
    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    
    double deltaMove; //ZM
    double yawAngleRadian; //ZM
    void updateInsectMovement(Bottle* zb, Bottle* yawb);
    int objIDcount;
    std::map<int,YarpSceneObject > yarpObjectsMap; 
    void addObjectToYarpScene(YarpSceneObject yarpObj);
    void removeObjectFromYarpScene(int key);
    void updateYarpSceneObject(int key, float x, float y, float z, float q1, float q2, float q3, float q4, float q5, float q6, float q7, float q8, float q9);
    void updateAllYarpSceneObjects();
    void sendYarpSceneObjects();
    QString  getSendString();
    Port outYarpSceneObjs; // for sending out yarp scene objects

    // OgreBites
    OgreBites::SdkTrayManager* mTrayMgr;
    OgreBites::SdkCameraMan* mCameraMan;       // basic camera controller
    OgreBites::ParamsPanel* mDetailsPanel;     // sample details panel
    bool mCursorWasVisible;                    // was cursor visible before dialog appeared
    bool mShutDown;

    //OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;
};

#endif // #ifndef __BaseApplication_h_

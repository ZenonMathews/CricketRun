/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

*/
#include <time.h> 
#include <sys/time.h>

#include "BaseApplication.h"
using namespace yarp::os;



#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include <iostream>
#include <fstream>
using namespace std;



    int camX = 0;
    int camY = 14;
    int camZ = -590;



//--------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mDetailsPanel(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
      mKeyboard(0),
      objIDcount(0)
{
}




void BaseApplication:: addObjectToYarpScene(YarpSceneObject yarpObj){
  yarpObjectsMap.insert ( std::pair<int,YarpSceneObject>(objIDcount, yarpObj) );
  objIDcount++;
}
void BaseApplication::removeObjectFromYarpScene(int key){
  std::map<int,YarpSceneObject>::iterator it;
  it=yarpObjectsMap.find(key);
  yarpObjectsMap.erase (it); 

}
    
void BaseApplication::updateYarpSceneObject(int key, float x, float y, float z, float q1, float q2, float q3, float q4, float q5, float q6, float q7, float q8, float q9){
  std::map<int,YarpSceneObject>::iterator it;
  it=yarpObjectsMap.find(key);

  (*it).second.x = x;
  (*it).second.y = y;
  (*it).second.z = z;
  (*it).second.q1 = q1;
  (*it).second.q2 = q2;
  (*it).second.q3 = q3;
  (*it).second.q4 = q4;
  (*it).second.q5 = q5;
  (*it).second.q6 = q6;
  (*it).second.q7 = q7;
  (*it).second.q8 = q8;
  (*it).second.q9 = q9;


}

//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
    if (mTrayMgr) delete mTrayMgr;
    if (mCameraMan) delete mCameraMan;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;




}

//-------------------------------------------------------------------------------------
bool BaseApplication::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "CricketRun");


     	//mWindow->setFullscreen(false, 400,300); // just remove this to get Fullscreen
        return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");
   
   // Position it at 500 in Z direction
   // int camX = -590;
   // int camY = 14;
   // int camZ = -590;


    mCamera->setPosition(Ogre::Vector3(camX,camY,camZ));

    


    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,14,590)); // keep ",14,", since it is related with Y axis position.  
    mCamera->setNearClipDistance(5);
    deltaMove = 0.0; //cricket
    yawAngleRadian = 0.0; //cricket



    // insert camera into the yarp scene
    YarpSceneObject yarpObj;
    Ogre::Vector3 vec = mCamera->getPosition();
    yarpObj.objType = 0; // 0 for camera
    yarpObj.x = vec.x;
    yarpObj.y = vec.y;
    yarpObj.z = vec.z;
    
    Ogre::Quaternion quat = mCamera->getOrientation();
    Ogre::Vector3 xaxis = quat.xAxis();
    Ogre::Vector3 yaxis = quat.yAxis();
    Ogre::Vector3 zaxis = quat.zAxis();
    
    yarpObj.q1 = xaxis.x;
    yarpObj.q2 = xaxis.y;
    yarpObj.q3 = xaxis.z;
    yarpObj.q4 = yaxis.x;
    yarpObj.q5 = yaxis.y;
    yarpObj.q6 = yaxis.z;
    yarpObj.q7 = zaxis.x;
    yarpObj.q8 = zaxis.y;
    yarpObj.q9 = zaxis.z;

    addObjectToYarpScene( yarpObj);
    
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller






}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    //mMouse->setEventCallback(this); //ZM commented mouse out
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
    //mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT); //ZM
    //mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT); // ZM
    mTrayMgr->hideCursor();

    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();

    mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    vp->setSkiesEnabled(true); //ZM

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void BaseApplication::setupResources(void)
{


    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }

    // create the yarp ports
    yarpInputPortZ.open("/ogre/trackball/in/z");
    yarpInputPortYaw.open("/ogre/trackball/in/yaw");
    outYarpSceneObjs.open("/ogre/yarpobjs/out");
}
//-------------------------------------------------------------------------------------
void BaseApplication::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}
//-------------------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
    createScene();
     
    createFrameListener();

    return true;
};




 void BaseApplication::updateInsectMovement(Bottle* zb, Bottle* yawb){

 int leftSpeed = 0; int rightSpeed = 0;


  if (zb != NULL){
    QString qCommand;
    qCommand = zb->toString().c_str();

    QStringList fields = QStringList::split( ',', qCommand );
    QStringList::Iterator it = fields.begin();
    ++it;

    if (it != fields.end()) {leftSpeed = (*it).toInt();}
    ++it;
    if (it != fields.end()) {rightSpeed = (*it).toInt();}
  }


  // yaw is prioretized over z (as leftSpeed and rightSpeed are overwritten)
  if (yawb != NULL){
    QString qCommand;
    qCommand = yawb->toString().c_str();

    QStringList fields = QStringList::split( ',', qCommand );
    QStringList::Iterator it = fields.begin();
    ++it;


    if (it != fields.end()) {leftSpeed = (*it).toInt();}
    ++it;
    if (it != fields.end()) {rightSpeed = (*it).toInt();}
  }


   // turn right
   if (leftSpeed > rightSpeed){
      //yawAngleRadian = lowPassVal * yawAngleRadian + (1.0 - lowPassVal) * 0.01;    
      yawAngleRadian = lowPassVal * yawAngleRadian + (1.0 - lowPassVal) * -0.015;       
   }
   // turn left
   else if (rightSpeed > leftSpeed){
      //yawAngleRadian = lowPassVal * yawAngleRadian + (1.0 - lowPassVal) * -0.01;
      yawAngleRadian = lowPassVal * yawAngleRadian + (1.0 - lowPassVal) * 0.015;

    }

    // here both wheel speeds the same
    else{

      if ((leftSpeed > 0) && (rightSpeed > 0)){
	deltaMove = lowPassVal * deltaMove + (1.0 - lowPassVal) *  0.5;
      }
      else if ((leftSpeed < 0) && (rightSpeed < 0)){
	deltaMove = lowPassVal * deltaMove + (1.0 - lowPassVal) * -0.5;
      }
      
    }
    
    if (zb == NULL && yawb == NULL){ // i.e. no mouse data..then slow down
      deltaMove = deltaMove * 0.9;
      yawAngleRadian = yawAngleRadian * 0.9;
    }

}







QString  BaseApplication::getSendString(){
  QString retStr = "";
  std::map<int,YarpSceneObject >::iterator it;
  for ( it=yarpObjectsMap.begin() ; it != yarpObjectsMap.end(); it++){
    retStr = retStr + QString("%1,").arg((*it).first); 
    retStr = retStr + QString("%1,").arg((*it).second.objType); 
    retStr = retStr + QString("%1,").arg((*it).second.x);
    retStr = retStr + QString("%1,").arg((*it).second.y);
    retStr = retStr + QString("%1,").arg((*it).second.z);
    retStr = retStr + QString("%1,").arg((*it).second.q1);
    retStr = retStr + QString("%1,").arg((*it).second.q2);
    retStr = retStr + QString("%1,").arg((*it).second.q3);
    retStr = retStr + QString("%1,").arg((*it).second.q4);
    retStr = retStr + QString("%1,").arg((*it).second.q5);
    retStr = retStr + QString("%1,").arg((*it).second.q6);
    retStr = retStr + QString("%1,").arg((*it).second.q7);
    retStr = retStr + QString("%1,").arg((*it).second.q8);
    retStr = retStr + QString("%1,").arg((*it).second.q9);
    
    retStr = retStr + "#";
  }
  
  return  retStr;
}






/////////////////////////////
// output data from Ogre ////
/////////////////////////////

void  BaseApplication::sendYarpSceneObjects(){
  QString curString = getSendString();
  Bottle bot; 
  bot.addString(curString);
  // send the message
  outYarpSceneObjs.write(bot);
  //std::cout << "server sending: " << curString << std::endl;
}










void  BaseApplication::updateAllYarpSceneObjects(){
   // update the camera
    Ogre::Quaternion quat = mCamera->getOrientation();
    Ogre::Vector3 vec = mCamera->getPosition();
    Ogre::Vector3 xaxis = quat.xAxis();
    Ogre::Vector3 yaxis = quat.yAxis();
    Ogre::Vector3 zaxis = quat.zAxis();

    float x = vec.x;
    float y = vec.y;
    float z = vec.z;

    // cricket
    float q1 = xaxis.x;
    float q2 = xaxis.y;
    float q3 = xaxis.z;
    float q4 = yaxis.x;
    float q5 = yaxis.y;
    float q6 = yaxis.z;
    float q7 = zaxis.x;
    float q8 = zaxis.y;
    float q9 = zaxis.z;

    int posX = (vec.x + 590);
    int posZ = (vec.z + 590); 


  //////////////////////////////////////////////
 // walls collision detection - arena limits/// 
//////////////////////////////////////////////

if (vec.x < -590){
mCamera->setPosition(Ogre::Vector3(-590,14, z));
}

if (vec.x > 590){
mCamera->setPosition(Ogre::Vector3(590,14, z));
}

if (vec.z < -590){
mCamera->setPosition(Ogre::Vector3(x,14,-590));
}

if (vec.z > 590){
mCamera->setPosition(Ogre::Vector3(x,14,590));
}



//print time and position
	struct tm *current;
	time_t now;
	time(&now);
	current = localtime(&now);
        time_t m_sec = time(NULL);
 


/*
// axis values such as:
// (top-view of arena)
//       ^
//  1180 | \        up
//       |    \     Area  
//   z   | down  \
//       | Area     \
//       |-------------\->
//       0      x      1180
*/

int xAxisValue = ((-1*(vec.x)) + 590);
int zAxisValue = (vec.z + 590);

bool upArea = false;
bool downArea = false;

if ((xAxisValue + zAxisValue) < 1180)
   {
    upArea = false;
    downArea = true;  
   }

else if ((xAxisValue + zAxisValue) > 1180)
   {
    upArea = true;
    downArea = false;  
   }



//Stereo function: meaningful data extracted only from q1 and q3 to define wich speaker is playing 
int stereo;



/*
Sound stimuli parameters:
intervals of angles were defined like: 


            \       |       /  
             \      |      /
              \rotH |rotA /
               \    |    /
                \   |   /
        rotG     \  |  /  rotB
                  \ | /
      _____________\|/_____________
                   /|\
                  / | \     rotC   
        rotF     /  |  \
                /   |   \
               /    |    \
              /     |     \
             / rotE | rotD \
            /       |       \
*/


bool rotA = false;
bool rotB = false;
bool rotC = false;
bool rotD = false;
bool rotE = false;
bool rotF = false;
bool rotG = false;
bool rotH = false;




//*****DEFINE ANGULAR AREA: A, B, C, D, E, F, G, H.

 if (((-1) < q1 < (-0.5)) && ((-0.5) < q3 < 0))
   {
   rotA = true;
   rotB = false; 
   rotC = false;
   rotD = false;
   rotE = false;
   rotF = false;
   rotG = false;
   rotH = false;
   //std::cout << "rotA" << std::endl;
   }


else if ( ( (-0.5) < q1 < 0) && ((-1) < q3 < (-0.5)) )
   {
   rotA = false; 
   rotB = true;
   rotC = false;
   rotD = false;
   rotE = false;
   rotF = false;
   rotG = false;
   rotH = false;
   //std::cout << "rotB" << std::endl;
   }

else if (((0) <= q1 < (0.5)) && ((-1) < q3 < (-0.5)))
   {
   rotA = false;
   rotB = false; 
   rotC = true;
   rotD = false;
   rotE = false;
   rotF = false;
   rotG = false;
   rotH = false;  
   //std::cout << "rotC" << std::endl;
   }


else if (((0.5) <= q1 < (1)) && ((-0.5) < q3 < 0))
    {
   rotA = false;
   rotB = false; 
   rotC = false;
   rotD = true;
   rotE = false;
   rotF = false;
   rotG = false;
   rotH = false; 
   //std::cout << "rotD" << std::endl;
    }

else if (((0.5) <= q1 < (1)) && (0 < q3 < 0.5))
    {
   rotA = false;
   rotB = false; 
   rotC = false;
   rotD = false;
   rotE = true;
   rotF = false;
   rotG = false;
   rotH = false;
   //std::cout << "rotE" << std::endl;
    }

else if (((0) <= q1 < (0.5)) && ((0.5) < q3 < 1))
   {
   rotA = false;
   rotB = false; 
   rotC = false;
   rotD = false;
   rotE = false;
   rotF = true;
   rotG = false;
   rotH = false;
   //std::cout << "rotF" << std::endl;
   }

else if (((-0.5) <= q1 < (0)) && ((0.5) < q3 < 1))
   {
   rotA = false;
   rotB = false; 
   rotC = false;
   rotD = false;
   rotE = false;
   rotF = false;
   rotG = true;
   rotH = false;
   //std::cout << "rotG" << std::endl;
   }

else/*(((-1) <= q1 < (-0.5)) && ((0) < q3 < 0.5))*/
   {
   rotA = false;
   rotB = false; 
   rotC = false;
   rotD = false;
   rotE = false;
   rotF = false;
   rotG = false;
   rotH = true;
   //std::cout << "rotH" << std::endl; 
   }











/*
//****DEFINE SPEAKER******
note: stereo equals to: 
0(left speaker) 
1(right speaker) 
2(both speakers volume down)
3(both speaker volume up)
*/

if (rotA == true){
stereo = 0;
//return;
}

else if (rotB == true){
stereo = 0;
//return;
}

else if (rotC == true){
stereo = 2;
//return;
}

else if (rotD == true){
stereo = 2;
//return;
}

else if (rotE == true){
stereo = 1;
//return;
}

else if (rotF == true){
stereo = 1;
//return;
}

else if ((rotG == true) && (upArea == true)){
stereo = 0;
//return;
}

else if ((rotG == true) && (downArea == true)){
stereo = 1;
//return;
}

else if(rotH == true) {
stereo = 3;
//return;
}

		


//    std::cout << current->tm_hour << ":" <<  current->tm_min << ":" << current->tm_sec << ":" <<" pos: " << xAxisValue << ", "  << zAxisValue << ", orient: " << q1 << ", " << q3 <<  ", "  << std::endl;

std::cout  << xAxisValue << ";" << zAxisValue << ";"  << stereo << ";" << std::endl;

//  std::cout << " pos: " << vec.x << ", " << vec.y << ", " << vec.z << ", orient: " << q1 << ", " << q2 << ", " << q3 << ", " << q4 << ", " << q5 << ", " << q6 << ", " << q7 << ", " << q8 << ", " << q9 << std::endl;
   updateYarpSceneObject(0,x,y,z,q1,q2,q3,q4,q5,q6,q7,q8,q9);


  
//printing:::::: 
//std::cout << current->tm_hour << ":" <<  current->tm_min << ":" << current->tm_sec << ":"  << " X " << vec.x  << " , " <<  " Z " << vec.z  << std::endl;
//std::cout << posX  << " . " << posZ << " . " << q1 <<";" << std::endl;
//std::cout << current->tm_hour << ":" <<  current->tm_min << ":" << current->tm_sec << "  " << posX  << " . " << posZ << " . " << q1 << ";"  << std::endl;




//ofstream logFile;
//logFile.open ("somethingLog");
//logFile << current->tm_hour << ":" <<  current->tm_min << ":" << current->tm_sec << ":"  << " X " << vec.x  << " , " <<  " Z " << vec.z  << std::endl ;
//logFile.close();
//return;






}

//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  
  Bottle* zb = yarpInputPortZ.read(false);
  Bottle* yawb = yarpInputPortYaw.read(false);

  //std::cout << " --------------- GOING TO UPDATE INSECT MOVEMENT ---------" << std::endl;
  updateInsectMovement(zb, yawb);
  //std::cout << " --------------- OK UPDATED INSECT MOVE ---------" << std::endl;



  // now move forward
  if (deltaMove != 0.0){
    Ogre::Vector3 newVec(0,0,deltaMove);
    mCamera->moveRelative(newVec);
  }
  // or yaw
  if (yawAngleRadian != 0.0){
    mCamera->yaw(Ogre::Radian(yawAngleRadian));
  }


    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }

    //std::cout << " --------------- GOING TO UPDATE SCENE ---------" << std::endl;
    updateAllYarpSceneObjects();

    //std::cout << " --------------- OK UPDATED SCENE ---------" << std::endl;
    sendYarpSceneObjects();
    

    return true;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;

        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

    mCameraMan->injectKeyDown(arg);
    return true;
}

bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
    mCameraMan->injectKeyUp(arg);
    return true;
}

bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
    if (mTrayMgr->injectMouseMove(arg)) return true;
    mCameraMan->injectMouseMove(arg);

    return true;
}

bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);
    return true;
}

bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    mCameraMan->injectMouseUp(arg, id);
    return true;
}

//Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}


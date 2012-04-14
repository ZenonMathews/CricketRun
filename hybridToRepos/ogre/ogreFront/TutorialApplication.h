/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
#include <OgreManualObject.h> //ZM
#include <qfile.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <map>

//------- ZM: for memory of the VR objects
struct VRObject {
  QString typeName;
  double posx, posy, posz;
  double lookx, looky, lookz; // only set for camera
};
typedef std::map<int, VRObject> VR_MAP;

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);
    void drawCylinder(double x, double y, double z, double radius, double height, QString name); //ZM

protected:
    virtual void createScene(void);

    // ZM input parameters
    int objRadius, objHeight; 
    double ambLight1,ambLight2, ambLight3;
    QString groundMaterial;
    VR_MAP vrMap; // this will contain all the VR objects
    // end ZM input parameters
    
};

#endif // #ifndef __TutorialApplication_h_

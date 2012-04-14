

/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/



#include "TutorialApplication.h"

//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}




//-------------------------------------------------------------------------------------
/*
void TutorialApplication::drawCylinder(double x, double y, double z, double rad, double height, QString name){
  Ogre::ManualObject * circle = mSceneMgr->createManualObject(name);
  float const radius = rad,
        thickness = 20.0, // Of course this must be less than the radius value.
        accuracy = 50;

  
  
  circle->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  
  unsigned point_index = 0;
  for(float theta = 0; theta <= 2 * Ogre::Math::PI; theta += Ogre::Math::PI / accuracy) {
    circle->position(radius * cos(theta),
		     0,
		     radius * sin(theta));
    circle->position(radius * cos(theta),
		     height,
		     radius * sin(theta)); // ZM

    circle->position(radius * cos(theta - Ogre::Math::PI / accuracy),
		     0,
                         radius * sin(theta - Ogre::Math::PI / accuracy));
     circle->position(radius * cos(theta - Ogre::Math::PI / accuracy),
		     height,
		      radius * sin(theta - Ogre::Math::PI / accuracy)); //ZM

    circle->position((radius - thickness) * cos(theta - Ogre::Math::PI / accuracy),
		     0,
		     (radius - thickness) * sin(theta - Ogre::Math::PI / accuracy));
    circle->position((radius - thickness) * cos(theta - Ogre::Math::PI / accuracy),
		     height,
		     (radius - thickness) * sin(theta - Ogre::Math::PI / accuracy)); //ZM
    circle->position((radius - thickness) * cos(theta),
		     0,
		     (radius - thickness) * sin(theta));
    circle->position((radius - thickness) * cos(theta),
		     height,
		     (radius - thickness) * sin(theta)); //ZM

    

    // Join the 4 vertices created above to form a quad.
    circle->quad(point_index, point_index + 1, point_index + 2, point_index + 3);
    point_index += 4;
    // Join the 4 vertices created above to form a quad.
    circle->quad(point_index+4, point_index + 5, point_index + 6, point_index + 7);
    point_index += 4;
    circle->quad(point_index, point_index + 4, point_index + 1, point_index + 7);
    point_index += 4;
    circle->quad(point_index+1, point_index + 5, point_index + 2, point_index + 6);
    point_index += 4;
  }
  
  circle->end();
  QString cylName = name + "Cyl";
  Ogre::SceneNode* cylinderNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(cylName);
  cylinderNode->attachObject(circle);
  cylinderNode->setPosition(x,y,z);
  
}


*/


void TutorialApplication::createScene(void)
{

  //--------- read in the input parameter file
//  QFile f( "../../../inputParams.txt" );
  QFile f( "../../inputParams.txt" );

  //QFile f( "/home/prospectif/hybrid-system/ogre_src_v1-7-3/cricketTest/inputParams.txt" );
  if( !f.exists() )
  {    
    std::cout << "The input parameter file does not exist." << std::endl;
    exit(0);
  }
  if( !f.open( IO_ReadOnly ) )
  {
    std::cout << "Failed to open input parameter file." << std::endl;
    exit(0);
   }
  // read in the file
  QTextStream ts( &f );
  QString line;
  while( !ts.eof() ) {
    line = ts.readLine();
    std::cout << line << std::endl;
    //<process your line and repeat>
    QStringList fields = QStringList::split( ' ', line);
    QStringList::Iterator it = fields.begin();
    if (*it == "AMBIENT_LIGHT"){
      it++;
      ambLight1 = (*it).toDouble();
      it++;
      ambLight2 = (*it).toDouble();
      it++;
      ambLight3 = (*it).toDouble();
    }
    else if (*it == "OBJECT_RADIUS"){
      it++;
      objRadius = (*it).toInt();
    }
    else if (*it == "OBJECT_HEIGHT"){
      it++;
      objHeight = (*it).toInt();
    }
    else if (*it == "GROUND_MATERIAL_NAME"){
      it++;
      groundMaterial = *it;
    }

    else if (*it == "OBSTACLE"){
      it++;
      int x = (*it).toInt();
      it++;
      int y = (*it).toInt();
      it++;
      int z = (*it).toInt();
      it++;
      int rad = (*it).toInt();
      it++;
      int height = (*it).toInt();
      it++;
      QString name = *it;
      //      drawCylinder(x,y,z,rad,height, name); 
    }
    
		
  } // while

  // end read in file

    // create your scene here 
    mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
        

    //Yellow Visual Object : Cube
    Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "cube.mesh");    
    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode", Ogre::Vector3( 390, 30, 590 ));
    headNode->attachObject(ogreHead);
    ogreHead->setMaterialName("Examples/tusk");
    ogreHead->setCastShadows(false);
    headNode->scale( .2, .8, .2 );


    //Red Visual Object : Cube 
    Ogre::Entity* cube1 = mSceneMgr->createEntity("Cube", "cube.mesh");    
    Ogre::SceneNode* cube1Node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Cube1Node", Ogre::Vector3( -390, 30, 590 ));
    cube1Node->attachObject(cube1);
    cube1->setMaterialName("Examples/red");
    cube1->setCastShadows(false);
    cube1Node->scale( .2, .8, .2 );

    /* 
    //Starting point area : Gray ground square
    Ogre::Entity* cube2 = mSceneMgr->createEntity("Cube2", "cube.mesh");    
    Ogre::SceneNode* cube2Node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Cube2Node", Ogre::Vector3( -570, 0, -570 ));
    cube2Node->attachObject(cube2);
    cube2->setMaterialName("Examples/start");
    cube2->setCastShadows(false);
    cube2Node->scale( .6, .001, .6 );
    cube2Node->yaw(Ogre::Radian(Ogre::Math::PI/2));
    */





     //Wall 1
    Ogre::Entity* wall1 = mSceneMgr->createEntity("Wall1", "cube.mesh");    
    Ogre::SceneNode* wall1Node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Wall1Node", Ogre::Vector3( 0, 30, 600 ));
    wall1Node->attachObject(wall1);
    wall1->setMaterialName("Examples/wall");
    wall1->setCastShadows(false);
    wall1Node->scale( 12, 1, .008 );
    
    // Wall 2
    Ogre::Entity* wall2 = mSceneMgr->createEntity("Wall2", "cube.mesh");    
    Ogre::SceneNode* wall2Node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Wall2Node", Ogre::Vector3( 600, 30, 0 ));
    wall2Node->attachObject(wall2);
    wall2->setMaterialName("Examples/wall");
    wall2->setCastShadows(false);
    wall2Node->scale( .008, 1, 12 );


    // Wall 3
    Ogre::Entity* wall3 = mSceneMgr->createEntity("Wall3", "cube.mesh");    
    Ogre::SceneNode* wall3Node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Wall3Node", Ogre::Vector3( 0, 30, -600 ));
    wall3Node->attachObject(wall3);
    wall3->setMaterialName("Examples/wall");
    wall3->setCastShadows(false);
    wall3Node->scale( 12, 1, .008 );

    // Wall 4
    Ogre::Entity* wall4 = mSceneMgr->createEntity("Wall4", "cube.mesh");    
    Ogre::SceneNode* wall4Node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Wall4Node", Ogre::Vector3( -600, 30, 0 ));
    wall4Node->attachObject(wall4);
    wall4->setMaterialName("Examples/wall");
    wall4->setCastShadows(false);
    wall4Node->scale( .008, 1, 12 );

//    mSceneMgr->setAmbientLight(Ogre::ColourValue(ambLight1, ambLight2, ambLight3)); //cricket
//    mSceneMgr->setSkyDome(true, "Examples/CloudySky", 4, 8, 5000, true); //ZM       //cricket




     //plane
     Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
     Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      plane, 1200, 1200, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

     Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
     mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
     entGround->setMaterialName("Examples/Skin");
     entGround->setCastShadows(false);




}



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif















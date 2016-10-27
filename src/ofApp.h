#pragma once

#include<fstream>
#include<iostream>
#include <time.h>

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxGui.h"

#include "ofxObjLoader.h"

#define STRINGIFY(A) #A


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void detailCamReset();
    void animeCamReset();
    
    void resetCam();
    void gridSwitch();
    void debugInfoSwitch();
    void turnMeshSwitch();
    void setPos();
    void dataLoad();
    void saveMapFile();
    void loadMapFile();
    
    ofxTrueTypeFontUC font;
    ofxTrueTypeFontUC fontSmall;
    ofxTrueTypeFontUC fontLarge;
    
    ofxAssimpModelLoader asModelObj[64][300];
    ofMesh modelList[64][300];
    ofImage modelImageList[64][300];
    int modelFlagList[64];
    int scanTimeRecordList[64][300][4];
    time_t scanTimeRecordTimeStructureList[64][1000];
    long scanUnixTimeLongIntList[64][1000];
    long scanUnixTimeModelMinList[64];
    long scanUnixTimeModelMaxList[64];
    long scanUnixTimeSelectedModelsMin;
    long scanUnixTimeSelectedModelsMax;
    long scanUnixTimeAllItemMin;
    long scanUnixTimeAllItemMax;
    int selectedItemIdOnScene[64];
    int scanTimeRecordMaxTime[64];
    string meshNameList[1024];
    int maxMeshNumList[1024];
    string mapId[1024];
    int mapNum[1024][16];
    float modelHeightList[1024];
    float modelPosXList[1024];
    float modelPosZList[1024];
    vector<string> dataDirNameList;
    vector< vector<uint64_t> > modelFileSizeList;
    vector <uint64_t> oneModelFileSizeList;
    int modelDataNum;
    uint64_t loadFileSizeAll;
    int totalMaxMeshNum;
    int totalScanTimeRecordMaxTime;
    long seekbarAddTime;

    ofMesh mesh;
    ofLight light;
    
    ofEasyCam eCam;
    
    // openframeworksではofShaderオブジェクトを通してシェーダを使う
    ofShader shader;
    
    ofFile ofFileObj;
    ofCamera ofCam;
    
    string meshDataDirPath;
    string defaultMeshDataDirPath;
    
    // GUIのパラメーター
    ofxPanel gui;
    ofxPanel guiMapEdit;
    ofxPanel guiPlayItem;
    ofxFloatSlider radius;
    ofxFloatSlider uiThumbnailIconDistance;
    ofxIntSlider uiFramerate;
    ofxIntSlider uiIconNumX;
    ofxIntSlider uiMeshDrawType;
    ofxIntSlider uiPlayMode;
    ofxIntSlider uiStreetViewDistance;
    ofxIntSlider uiBgColor;
    ofxIntSlider uiStreetViewStartFrame;
    ofxIntSlider uiDemoPatternInterval;
    ofxIntSlider uiDemoMoveSpeed;
    ofxIntSlider uiLightPos;
    ofxIntSlider uiTestSlider;
    ofxColorSlider color;
    ofxVec2Slider position;
    ofxToggle uiBtnPlayPause;
    ofxToggle uiBtnPlayRealtime;
    ofxToggle uiBtnLight;
    ofxToggle uiBtnStreetView;
    ofxButton uiBtnReset;
    ofxButton uiBtnCamReset;
    ofxButton uiBtnSelectReset;
    ofxButton uiBtnPlaySelectA;
    ofxButton uiBtnPlaySelectB;
    ofxButton uiBtnPlaySelectC;
    ofxButton uiBtnPlaySelectBack;
    ofxToggle uiBtnDemoPlay;
    ofxToggle uiBtnGrid;
    ofxToggle uiBtnDebugInfo;
    ofxToggle uiBtnTurnMesh;
    ofxToggle uiBtnLoopPlay;
    ofxToggle uiBtnOrtho;
    
    ofxFloatSlider uiEditPosX;
    ofxFloatSlider uiEditPosY;
    ofxFloatSlider uiEditPosZ;
    ofxFloatSlider uiEditRotX;
    ofxFloatSlider uiEditRotY;
    ofxFloatSlider uiEditRotZ;
    ofxFloatSlider uiEditHolizontalAngleAdjuster;
    ofxFloatSlider uiEditAdjustZ;
    ofxFloatSlider uiEditScale;
    ofxIntSlider uiEditModelSelector;
    ofxIntSlider uiEditReadStartFrame;
    ofxIntSlider uiEditReadEndFrame;
    ofxIntSlider uiEditDisplayFlag;
        
    float prevPosX;
    float prevPosY;

    unsigned int frameCount;
    unsigned int playCount;
    
    
    int maxLoadMeshNum;
    int startPlayMeshAnimNum;
    
    int totalLoadedModelNum;
    int maxLoadedMeshNumInAllMesh;
    
    int prevSelectModel;
    
    int selectMeshId;
    
    float thumbnailIconDistance;

    int viewerMode;         // Detail / List / Mapping /  MapEdit
    
    string mapFilePath;
    ofFile mapFile;
    ofBuffer mapBuffer;

    time_t unixTimeOnOfStarted;
    struct tm *unixTimeOnOfStartedTmStruct;
    
    bool mapFileExists;
    int mapDataColumns;
    
    bool dispGui;
    
    int displayTotalVertices;
    int displayTotalFaces;
    
    int appInitStartTime;
    int appInitEndTime;
    int modeldataLoadingStartTime;
    int modeldataLoadingEndTime;
    
    int modeldataFiles;
    int modeldataDatasetNum;
    
    
    int playSeekTime;
    int nowTime;
    
    bool onPlay;
    int playMode;
    
    int prevPlayTime;
    
    uint64_t  playStartDateTime;
    uint64_t  playStartPrevPos;
    
    bool loopPlay;
    
    int nowPlayTime;
    
    bool prevFramePlayState;
    
    bool demoCamFlag;
    bool prevDemoCamFlag;
    int demoCamCounter;
    int demoCamMaxCount;
    int demoCamStartPosX;
    int demoCamStartPosY;
    int demoCamStartPosZ;
    int demoMode;
};


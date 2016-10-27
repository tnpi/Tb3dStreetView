#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    appInitStartTime = ofGetElapsedTimeMillis();
    
    defaultMeshDataDirPath = "/Users/artdkt/Desktop/3dscan_data_for0630/artdkt_structure3d";
    
    scanUnixTimeAllItemMin = LONG_MAX;
    scanUnixTimeAllItemMax = LONG_MIN;
    
    viewerMode = 0;
    //uiMeshDrawType = 1;
    selectMeshId = 0;
    mapFileExists = false;
    mapDataColumns = 16;
    
    totalLoadedModelNum = 0;
    maxLoadedMeshNumInAllMesh = 0;
    
    frameCount = 0;
    playCount = 0;
    totalMaxMeshNum = 0;
    maxLoadMeshNum = 400;
    startPlayMeshAnimNum = 0;
    
    demoCamFlag = true;
    prevDemoCamFlag = true;
    demoCamCounter = 0;
    demoCamMaxCount = 200;
    demoCamStartPosX = 0;
    demoCamStartPosY = -3000;
    demoCamStartPosZ = 1500;
    demoMode = 1;

    
    nowPlayTime = 0;
    
    dispGui = true;
    loopPlay = true;
    playMode = 0;   // 1:timebased 0: frame
    
    prevSelectModel = -1;

    playSeekTime = 0;
    onPlay = true;
    
    playStartPrevPos = 0;
    prevFramePlayState = true;

    seekbarAddTime = 0;
    
    //meshDataDirPath = "/Users/artdkt_3dscan_20160124_zenhan/artdkt_structure3d";
    //meshDataDirPath = "/Users/doc100/Desktop/tempData/artdkt_structure3d";
    
    ofFileDialogResult fileDialogResult = ofSystemLoadDialog("Select Time-based 3D Record base directory.", true, defaultMeshDataDirPath);
    
    meshDataDirPath = fileDialogResult.getPath();
    //meshDataDirPath = "/Users/cr_mac03/Desktop/scanData/";//fileDialogResult.getPath();

    stringstream mapSs;
    mapSs << fileDialogResult.getPath() << "/mapFile.csv";
    mapFilePath = mapSs.str();
    
    cout << "mapFilePath: " << mapFilePath;

    
    ofSetFrameRate(60);
    
    
    
    // Read Map File ----------------------------------------------------------------
    ///Users/doc100/Desktop/tempData/artdkt_structure3d
    //mapFilePath = "/Users/artdkt_3dscan_20160124_zenhan/artdkt_structure3d/mapFile.csv";
    //mapFilePath = "/Users/artdkt_structure3d/mapFile.csv";
    
    ofFile mapFile(mapFilePath);
    
    if(!mapFile.exists()){
        
        ofLogError("The file " + mapFilePath + " is missing. I make it.");
        
        for(int i=0; i<256; i++) {
            mapId[i] = "";
            for(int j=0; j<16; j++) {
                mapNum[i][j] = 0;
                if (j == 6) {
                    mapNum[i][j] = 0;
                }
            }
        }
    } else {
        
        mapFileExists = true;
        ofBuffer buffer(mapFile);
    
        // ----------------------------------------------------------------------------------
        // Read file line by line
        // ------------------------------------------------------------------------------
        int bufCounter = 0;
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
            string line = *it;
            //Split line into strings
            vector<string> words = ofSplitString(line, ",");
            
            //Store strings into a custom container
            if (words.size()>=2) {
                
                mapId[bufCounter] = words[0];
                
                for(int i=0; i<words.size()-1; i++) {
                    mapNum[bufCounter][i] = stoi(words[i+1]);
                }
            }
            
            cout << line << endl;
            bufCounter++;
        }
        
    }
    // --------------------------------------------------------------
    
    font.loadFont("hira.otf", 38);
    fontSmall.loadFont("hira.otf", 20);
    fontLarge.loadFont("hira.otf", 100);
    ofxGuiSetFont("hira.otf", 13, true, true);
    ofxGuiSetTextPadding(4);
    ofxGuiSetDefaultWidth(300);
    
    // OfxGUI setup -----------------------------------------------------------------
    ofColor initColor = ofColor(0, 127, 255, 255);
    ofColor minColor = ofColor(0,0,0,0);
    ofColor maxColor = ofColor(255,255,255,255);
 
    ofVec2f initPos = ofVec2f(ofGetWidth(), ofGetHeight());
    ofVec2f minPos = ofVec2f(-ofGetWidth() * 3, -ofGetHeight() * 4);
    ofVec2f maxPos = ofVec2f(ofGetWidth() * 2, ofGetHeight() * 3);
   
    uiBtnPlayPause.setSize(100, 100);
    
    gui.setup("settings");
    //gui.setFillColor(<#const ofColor &color#>)
    
    gui.setDefaultBackgroundColor(ofColor(255,0,0));
    
    gui.setPosition(500, 50);
    gui.setDefaultHeight(30);

    // Add GUI parts -------------------------------------------------------------------------------

    //gui.add(radius.setup("radius", 200, 0, 400));
    //gui.add(color.setup("color", initColor, minColor, maxColor));
    //gui.add(position.setup("position", initPos, minPos, maxPos));
    gui.add(uiThumbnailIconDistance.setup("thumbnailIconDistance", 3000, 0, 5000));
    //gui.add(uiStreetViewDistance.setup("StreetViewDistance", 3000, 0, 5000));
    gui.add(uiStreetViewStartFrame.setup("StreetViewStartFrame", 0, 0, 100));
    gui.add(uiBgColor.setup("BgColor", 240, 0, 255));
    gui.add(uiIconNumX.setup("iconNumX", 4, 1, 8));
    gui.add(uiFramerate.setup("Framerate", 15,5, 60));
    gui.add(uiDemoPatternInterval.setup("DemoInterval", 1000,10, 5000));
    gui.add(uiDemoMoveSpeed.setup("DemoMoveSpeed", 100,10, 1000));
    gui.add(uiLightPos.setup("LightPos", 250000,0, 500000));
    gui.add(uiTestSlider.setup("TestSlider", 0, -100000, 100000));
    gui.add(uiMeshDrawType.setup("meshDrawType", 1, 0, 2));
    gui.add(uiPlayMode.setup("playMode", 0, 0, 2));
    gui.add(uiBtnDemoPlay.setup("DemoPlay", true, 40, 40));
    gui.add(uiBtnPlayPause.setup("Play / Stop", true, 40, 40));
    //gui.add(uiBtnPlayRealtime.setup("Play Realtime mode", true, 40, 40));
    gui.add(uiBtnLight.setup("Light on/off", true, 40, 40));
    gui.add(uiBtnGrid.setup("Grid", false, 40, 40));
    gui.add(uiBtnDebugInfo.setup("DebugInfo", true, 40, 40));
    gui.add(uiBtnTurnMesh.setup("TurnMesh", true, 40, 40));
    gui.add(uiBtnLoopPlay.setup("LoopPlay", true, 40, 40));
    gui.add(uiBtnStreetView.setup("StreetView", true, 40, 40));
    gui.add(uiBtnOrtho.setup("Ortho", false, 40, 40));
    gui.add(uiBtnReset.setup("CameraReset", 40, 40));
    gui.add(uiBtnSelectReset.setup("quit", 40, 40));
    
    guiMapEdit.setup("MapEdit");
    guiMapEdit.setPosition(800, 100);
    guiMapEdit.setDefaultWidth(500);
    guiMapEdit.setDefaultHeight(30);
    guiMapEdit.add(uiEditPosX.setup("posX",0,-10000,10000));
    guiMapEdit.add(uiEditPosY.setup("posY",0,-10000,10000));
    guiMapEdit.add(uiEditPosZ.setup("posZ",0,-50000,50000));
    guiMapEdit.add(uiEditRotX.setup("rotX",0,-360,360));
    guiMapEdit.add(uiEditRotY.setup("rotY",0,-360,360));
    guiMapEdit.add(uiEditRotZ.setup("rotZ",0,-360,360));
    guiMapEdit.add(uiEditHolizontalAngleAdjuster.setup("H_AngleAdjust",0,-180,180));
    guiMapEdit.add(uiEditAdjustZ.setup("adjustZ",-10000,0,10000));
    guiMapEdit.add(uiEditScale.setup("StreetViewDistance",100,0,5000));
    guiMapEdit.add(uiEditReadStartFrame.setup("startF",0,0,500));
    guiMapEdit.add(uiEditReadEndFrame.setup("endF",0,0,500));

    guiMapEdit.add(uiEditModelSelector.setup("modelSel",0,0,100));
    guiMapEdit.add(uiEditDisplayFlag.setup("display",0,0,2));
    
    prevPosX = position->x;
    prevPosY = position->y;
    
    uiBtnReset.addListener(this, &ofApp::resetCam);
    //position.addListener(this, &ofApp::setPos);
    
    
    //uiBtnGrid.addListener(this, &ofApp::gridSwitch);
    //uiBtnDebugInfo.addListener(this, &ofApp::debugInfoSwitch);
    //uiBtnTurnMesh.addListener(this, &ofApp::turnMeshSwitch);
    uiBtnSelectReset.addListener(this, &ofApp::turnMeshSwitch);
    
    guiPlayItem.setup("playItem");
    
    guiPlayItem.add(uiBtnPlaySelectA.setup("A", 50, 40));
    guiPlayItem.add(uiBtnPlaySelectB.setup("B", 50, 40));
    guiPlayItem.add(uiBtnPlaySelectC.setup("C", 50, 40));
    guiPlayItem.add(uiBtnPlaySelectBack.setup("Reset", 50, 40));
    
    
    appInitEndTime = ofGetElapsedTimeMillis();

    
    playStartDateTime = ofGetElapsedTimeMillis();
    
    uiPlayMode = playMode;
    
    ::time(&unixTimeOnOfStarted);

}


void ofApp::turnMeshSwitch(){
    
    OF_EXIT_APP(0);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    int i = uiEditModelSelector;
    
    if (i != prevSelectModel) {
        uiEditPosX = mapNum[i][0];
        uiEditPosY = mapNum[i][1];
        uiEditPosZ = mapNum[i][2];
        uiEditRotX = mapNum[i][3];
        uiEditRotY = mapNum[i][4];
        uiEditRotZ = mapNum[i][5];
        uiEditScale = mapNum[i][6];
        uiEditReadStartFrame = mapNum[i][7];
        uiEditReadEndFrame = mapNum[i][8];
        uiEditDisplayFlag = mapNum[i][9];
        uiEditHolizontalAngleAdjuster = mapNum[i][10];
        uiEditAdjustZ = mapNum[i][12];
    }

    
    mapNum[i][0] = uiEditPosX;
    mapNum[i][1] = uiEditPosY;
    mapNum[i][2] = uiEditPosZ;
    mapNum[i][3] = uiEditRotX;
    mapNum[i][4] = uiEditRotY;
    mapNum[i][5] = uiEditRotZ;
    mapNum[i][6] = uiEditScale;
    mapNum[i][7] = uiEditReadStartFrame;
    mapNum[i][8] = uiEditReadEndFrame;
    mapNum[i][9] = uiEditDisplayFlag;
    mapNum[i][10] = uiEditHolizontalAngleAdjuster;
    mapNum[i][12] = uiEditAdjustZ;
    
    prevSelectModel = i;
    
    
    //eCam.setPosition(0,0,-300);
    //eCam.setTarget(ofVec3f(0,0,0));
    
    /*
    if (mouseY >= (ofGetHeight() - 150)) {
        eCam.disableMouseInput();
    } else {
        eCam.enableMouseInput();
    }*/
    
    if (prevFramePlayState == false && uiBtnPlayPause) {
        playStartPrevPos = nowPlayTime;
        playStartDateTime = ofGetElapsedTimeMillis();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetFrameRate((int)uiFramerate);

    displayTotalVertices = 0;
   
    prevDemoCamFlag = demoCamFlag;
    
    if (frameCount == 1) {  // app start
        dataLoad();
    }
    
    // --------------------------
    ofBackground(uiBgColor, uiBgColor, uiBgColor);        // gray bg
    
    ofEnableSmoothing();
    
    
    if(frameCount==1){
        eCam.begin();/*
       // eCam.reset();
        //eCam.setPosition(0,0,5000);
                      */
        eCam.end();
        
    }
    
    //eCam.disableMouseInput();

    
    //  ---------------------------------------
    ofSetColor(255,255,255,255);

    if (uiBtnLight) {
        ofEnableLighting();
        light.enable();
        light.setSpotlight();
        if (uiBtnTurnMesh) {
            //light.setPosition(10000,1000,20000);
            light.setPosition(10000,1000,uiLightPos);
            //light.setPosition(000,10000,-10000);
            //light.setDirectional();
        } else {
            light.setPosition(-1000,1000,uiLightPos);
            //light.setDirectional();
        }
        //light.setAmbientColor(ofFloatColor(0.2, 0.2, 0.2, 1.0));
        light.setAmbientColor(ofFloatColor(0.3, 0.3, 0.3, 1.0));
        light.setDiffuseColor(ofFloatColor(0.4, 0.4, 0.4));
        light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
        ofEnableDepthTest();
    }

    
    eCam.setFarClip( 100000.0f );
    
    //ofTranslatef(0.0,0.0,200.0);
    
    ofSetColor(255,255,255,255);
    //ofRect(0,0,100,100);
    
    if (frameCount == 0) {
        font.drawString("Now Loading...", 500, 400);
    }
    
    
    eCam.begin();
    
    if (uiBtnDemoPlay && uiBtnPlayPause && viewerMode == 0) {
        if (demoCamCounter % (int)uiDemoPatternInterval == 0) {
            if (((int)(demoCamCounter / uiDemoPatternInterval)) % 3 == 0) {
                eCam.reset();
                eCam.setPosition(demoCamStartPosX, demoCamStartPosY, demoCamStartPosZ);
                eCam.lookAt(ofVec3f(0,10000,demoCamStartPosZ), ofVec3f(0,0,1));
                
            }
            /*

            else if (((int)(demoCamCounter / uiDemoPatternInterval)) % 3 == 1) {
                eCam.reset();
                eCam.setPosition(demoCamStartPosX*2, demoCamStartPosY, demoCamStartPosZ*12);
                eCam.lookAt(ofVec3f(0,demoCamStartPosY,0), ofVec3f(-1,0,0));
                
            } else {
                eCam.reset();
                eCam.setPosition(demoCamStartPosX, demoCamStartPosY, demoCamStartPosZ*5);
                eCam.lookAt(ofVec3f(0,demoCamStartPosY,0), ofVec3f(0,1,0));
            }
             */
        }
    }
    
    if (uiBtnOrtho) {
        eCam.enableOrtho();
    } else {
        eCam.disableOrtho();
    }
    //ofCam.begin();
    
    ofScale(1,-1);      // y-axis reverse! (for fix drawString text flips promblem)

    
    if (viewerMode == 0) {
//        eCam.disableMouseInput();
        
        if (demoCamFlag && uiBtnDemoPlay && uiBtnPlayPause) {
            eCam.move(0,uiDemoMoveSpeed,0);
            
            //eCam.setTarget(ofVec3f(0,-20000,500));
            //eCam.setAutoDistance(<#bool bAutoDistance#>)
            //eCam.
            
            demoCamCounter++;
        }
        
        
    } else {
       // eCam.enableMouseInput();
    }
    
    glPushMatrix();

    if (uiBtnGrid) {
        ofSetColor(255,255,255,255);
        ofDrawGrid(uiThumbnailIconDistance);
    }
    
    
    ofSetColor(255,255,255,255);
    //ofRect(0,0,100,100);


    
    //  -------------------
    //glRotatef(180, 0, 1, 0);        //
    
//    glTranslatef(ofGetWidth()/4.0, ofGetHeight()/10*0, 400); //
    glTranslatef(0, 0, 0); //
    
    ofSetColor(255,255,255,255);
    int indexX = mouseX / uiThumbnailIconDistance;
    int indexY = mouseY / uiThumbnailIconDistance;
    //ofRect(indexX * uiThumbnailIconDistance, indexY * uiThumbnailIconDistance, uiThumbnailIconDistance, uiThumbnailIconDistance);
    
    ofSetColor(255,255,255);
    for(int i=0; i<modelDataNum; i++) {
        
        
        if (viewerMode == 0 && selectMeshId != i) {     // select display mesh
            //continue;
        }
        
        
        /*
        if (maxMeshNumList) {
            continue;
        }
        */
        
        
        int counter = playCount;// % maxMeshNumList[i];
        
        
        
        if (viewerMode == 0) {
            //glRotatef(mouseY, 0, 1, 0);
            //glRotatef(mouseX*0.1, 0, 1, 0);
        }
        
  
        
        
        
        int playFrameSelector = 0;
        ofSetLineWidth(1);
        
        
        if (viewerMode == 0) {          // detail view -------------------------------------------------------------------
            
            float modelSizeX = modelPosXList[selectMeshId]*1000;
            float modelSizeY = modelHeightList[selectMeshId]*1000;
            float modelSizeZ = modelPosZList[selectMeshId]*1000;
            
            // draw 3axis
            if (dispGui) {
                ofSetLineWidth(10);
                ofSetColor(255,64,64);
                ofLine(0,0,0,modelSizeX,0,0);
                //font.drawString("X", 10200, 0);
                ofSetColor(64,192,64);
                ofLine(0,0,0,0,modelSizeY,0);
                //font.drawString("Y", 0, 10200);
                ofSetColor(64,64,255);
                ofLine(0,0,0,0,0,modelSizeZ);
            }
            
            for(int svIndex = 0; svIndex < maxMeshNumList[i]-mapNum[i][7]; svIndex++) {
                playFrameSelector = mapNum[i][7]+svIndex;
                
                displayTotalVertices += modelList[i][playFrameSelector].getNumVertices();
                glPushMatrix();  //
                
                //glTranslatef(((i%uiIconNumX))*uiThumbnailIconDistance, (i/uiIconNumX)*uiThumbnailIconDistance, 0); //√Å√Æ¬™√à√π¬¢‚Ä∞‚àè‚â†√Ç√∏√â‚Äû√Ö¬¥√Å√ü¬™√Ç√£√Ø
                // Draw Model Name
                {
                    glPushMatrix();  //
                    ofSetColor(255,255,255,255);
                    //glTranslatef(180, 400, 700); //
                    glTranslatef(-0,0,2);
                    
                    //glRotatef(180, 0, 0, 1);        // 6/29
                    
                    
                    ofDisableLighting();        //
                    fontLarge.drawString(dataDirNameList[i],0,0);        // display model name
                    ofEnableLighting();
                    
                    //font.drawString(to_string(asModelObj[i][counter].getNumMeshes()),600,500);
                    glPopMatrix();
                }
                
                
                if (uiBtnTurnMesh) {
                    glRotatef(-90, 1, 0, 0);
                    
                }
                
                //glRotatef(180, 0, 1, 0);        // ‚Ä∞‚àè√§‚Ä∞‚àè√£√ä√±œÄ√Ç√™√´√Å√µ¬•‚Äû√Ö√¥
                
                ofTranslate(0,-1*modelHeightList[i]*1000,0);    // set y pos
                ofTranslate(0,-0,modelPosZList[i]*1000);        // hosei

                // all model same rotation mapfile
                ofRotateX(uiEditRotX);
                ofRotateY(uiEditRotY);
                ofRotateZ(uiEditRotZ);

                
                
                //ofTranslate(0,-0,mapNum[i][6] * -svIndex);        // street view distance uiStreetViewDistance
                
                double totalRecTime = scanUnixTimeAllItemMax - scanUnixTimeAllItemMin;
                double frameTime = scanUnixTimeLongIntList[i][playFrameSelector] - scanUnixTimeAllItemMin;
                
                double perTime = frameTime / totalRecTime ;
                
                //ofTranslate(0,-0,mapNum[i][6] * -svIndex);        // street view distance uiStreetViewDistance
                if (uiPlayMode == 2) {
                    ofTranslate(0, -0, -uiEditScale *10 * perTime );        // street view distance uiStreetViewDistance
                    //ofTranslate(0, -0, mapNum[i][6] * -frameTime );        // street view distance uiStreetViewDistance
                }

                ofTranslate((float)mapNum[i][0],(float)mapNum[i][1],(float)mapNum[i][2]);        // map

                ofScale(1000, 1000, 1000);  // temp debug
                
                ofScale(1, 1, -1);      // fix model direction

                // adjust dual sync cam
                ofRotateY(mapNum[i][10]);
                ofTranslate(0,0,mapNum[i][11]);

                // 6/29
                //ofScale(1, -1);
                
                if (uiMeshDrawType == 1) {
                    //asModelObj[i][counter].draw(OF_MESH_WIREFRAME);
                    //            asModelObj[i][counter].drawWireframe();
                    ofSetLineWidth(1);
                    modelList[i][playFrameSelector].drawWireframe();
                } else if (uiMeshDrawType == 2) {
                    //asModelObj[i][counter].draw(OF_MESH_POINTS);
                    //asModelObj[i][counter].draw(OF_MESH_POINTS);
                    //stroke(10);
                    glPointSize(5);
                    ofBlendMode(OF_BLENDMODE_ALPHA);

                    modelList[i][playFrameSelector].drawVertices();
                } else {
                    
                    modelList[i][playFrameSelector].draw();
                    
                    //asModelObj[i][counter].drawFaces();
                    //asModelObj[i][counter].draw(OF_MESH_FILL);
                }
            
                glPopMatrix();
            }

            
        }
        else if (viewerMode <= 1) {     // file list view
            
            // draw 3axis
            ofSetLineWidth(10);
            ofSetColor(255,64,64);
            ofLine(0,0,0,10000,0,0);
            font.drawString("X", 10200, 0);
            ofSetColor(64,192,64);
            ofLine(0,0,0,0,10000,0);
            font.drawString("Y", 0, 10200);
            ofSetColor(64,64,255);
            ofLine(0,0,0,0,0,10000);
            
            // draw base grid
            ofSetColor(128,224,255, 64);
            ofLine(0,0,0,0,0,0);
            ofRect(0, 0, 10000, 10000);
            
            ofSetLineWidth(5);
            ofSetColor(128,224,255, 128);
            for(int j=0; j<=10; j++) {
                ofLine(0,1000*j,1,10000,1000*j,1);
                stringstream tSs;
                tSs.str("");
                tSs << "" << j << "m";
                fontLarge.drawString(tSs.str(), -300, 1000*j+50);
                
            }
            
            // d
            for(int j=0; j<=10; j++) {
                ofLine(1000*j,0,1,1000*j,10000,1);
                
                stringstream tSs;
                tSs.str("");
                tSs << "" << j << "m";
                fontLarge.drawString(tSs.str(), 1000*j-100, -100);
            }
            
            displayTotalVertices += modelList[i][playFrameSelector].getNumVertices();
            //displayTotalFaces += modelList[i][counter].getNum;
            
            glPushMatrix();  //
            
            glTranslatef(((i%uiIconNumX))*uiThumbnailIconDistance, (i/uiIconNumX)*uiThumbnailIconDistance, 0); //√Å√Æ¬™√à√π¬¢‚Ä∞‚àè‚â†√Ç√∏√â‚Äû√Ö¬¥√Å√ü¬™√Ç√£√Ø
            
            //glRotatef(180, 0, 1, 0);        //

            int gridSize = 1000;
            int gridDiv = 10;
            int gridRowSize = gridSize / gridDiv;

            // Draw Model Name
            {
                glPushMatrix();  //
                ofSetColor(255,255,255,255);
                //glTranslatef(180, 400, 700); //
                glTranslatef(-0,0,2 );
                
                //glRotatef(180, 0, 0, 1);        // 6/29
                

                ofDisableLighting();        //
                fontLarge.drawString(dataDirNameList[i],0,0);        // display model name
                ofEnableLighting();

                //font.drawString(to_string(asModelObj[i][counter].getNumMeshes()),600,500);
                glPopMatrix();
            }
            
            
            if (uiBtnTurnMesh) {
                glRotatef(-90, 1, 0, 0);
                
            }

            //glRotatef(180, 0, 1, 0);        // ‚Ä∞‚àè√§‚Ä∞‚àè√£√ä√±œÄ√Ç√™√´√Å√µ¬•‚Äû√Ö√¥

            //ofTranslate(0,-1000);
            ofTranslate(0,-1*modelHeightList[i]*1000,0);
            
            
            // all model same rotation mapfile
            ofRotateX(uiEditRotX);
            ofRotateY(uiEditRotY);
            ofRotateZ(uiEditRotZ);

            //ofTranslate(0,-0,modelPosZList[i]*1000);        // hosei
            
            ofTranslate(0,-1000,0);        // temp
            
            //cout << "height: " << modelHeightList[i] << endl;
            
            ofScale(1000, 1000, 1000);  // temp debug
            
            // 6/29
            ofScale(1, 1, -1);      // fix model direction
            
            
            if (uiMeshDrawType == 1) {
                //asModelObj[i][counter].draw(OF_MESH_WIREFRAME);
    //            asModelObj[i][counter].drawWireframe();
                ofSetColor( 255, 255, 255 );
                modelImageList[i][playFrameSelector].bind();

                modelList[i][playFrameSelector].drawWireframe();
                modelImageList[i][playFrameSelector].unbind();
            } else if (uiMeshDrawType == 2) {
                //asModelObj[i][counter].draw(OF_MESH_POINTS);
                //asModelObj[i][counter].draw(OF_MESH_POINTS);
                modelList[i][playFrameSelector].drawVertices();
            } else {

                modelList[i][playFrameSelector].draw();
                
                //asModelObj[i][counter].drawFaces();
                //asModelObj[i][counter].draw(OF_MESH_FILL);
            }
            glPopMatrix();
            
        }
        else  {           // mapView -----------------------------------------------------------
            
            if (frameCount >= 1) {
                modelFlagList[i] = 0;
                
                if (maxMeshNumList[i] >= 2 && scanTimeRecordMaxTime[i] > 0) {
                    
                    if (uiBtnPlayPause) {
                        //nowPlayTime =  (ofGetElapsedTimeMillis() % (scanUnixTimeAllItemMax - scanUnixTimeAllItemMin)) - playStartDateTime + playStartPrevPos;     // 0 start realtime incremental num (msec)
                        nowPlayTime =  ( (ofGetElapsedTimeMillis() + seekbarAddTime) % (scanUnixTimeAllItemMax - scanUnixTimeAllItemMin)) + playStartPrevPos;     // 0 start realtime incremental num (msec)
                    }
                    cout << "playStartDateTime: " << playStartDateTime << " playStartPrevPos:" << playStartPrevPos << endl;
                    
                    bool dispFlag = false;
                    
                    long virtualPlayUnixTime = nowPlayTime + scanUnixTimeAllItemMin;
                    
                    // scan play frame by time
                    for (int j=0; j<maxMeshNumList[i]; j++) {
                        if (virtualPlayUnixTime >= scanUnixTimeLongIntList[i][j] &&
                            virtualPlayUnixTime <= scanUnixTimeLongIntList[i][j+1]  ) {
                            playFrameSelector = j;
                            dispFlag = true;
                            break;
                        }
                    }
                    // }
                    
                    if (!dispFlag){
                        playFrameSelector = 0;
                        modelFlagList[i] = 2;
                    }
                    
                }
            }
            
            /*
            cout << "playFrameSelector:" << playFrameSelector << endl;
             */
            
            glPushMatrix();

            
            
            float modelSizeX = modelPosXList[selectMeshId]*1000;
            float modelSizeY = modelHeightList[selectMeshId]*1000;
            float modelSizeZ = modelPosZList[selectMeshId]*1000;
            
            // draw 3axis
            ofSetLineWidth(10);
            ofSetColor(255,64,64);
            ofLine(0,0,0,modelSizeX,0,0);
            //font.drawString("X", 10200, 0);
            ofSetColor(64,192,64);
            ofLine(0,0,0,0,modelSizeY,0);
            //font.drawString("Y", 0, 10200);
            ofSetColor(64,64,255);
            ofLine(0,0,0,0,0,modelSizeZ);
            
            
            //glRotatef(180, 0, 1, 0);        // ‚Ä∞‚àè√§‚Ä∞‚àè√£√ä√±œÄ√Ç√™√´√Å√µ¬•‚Äû√Ö√¥
            
            if (uiBtnTurnMesh) {
                glRotatef(-90, 1, 0, 0);
                
            }

            ofTranslate(0,-1*modelHeightList[i]*1000,0);    // set y pos
            ofTranslate(0,-0,modelPosZList[i]*1000);        // hosei
            
            ofTranslate(-modelSizeX/4,0,0);// add tanaka 20160922
            
            
            ofTranslate((float)mapNum[i][0],(float)mapNum[i][1],(float)mapNum[i][2]);        // map
            
            
            ofScale(1000, 1000, 1000);  // temp debug
            
            ofScale(1, 1, -1);      // fix model direction
            
            
            // adjust dual sync cam
            ofRotateY(mapNum[i][10]);
            ofTranslate(0,0,mapNum[i][11]);
            
            ofSetColor(255,255,255);
            
            // 6/29
            //ofScale(1, -1);
            
            if (uiMeshDrawType == 1) {
                //asModelObj[i][counter].draw(OF_MESH_WIREFRAME);
                //            asModelObj[i][counter].drawWireframe();
                ofSetLineWidth(1);
                modelList[i][playFrameSelector].drawWireframe();
            } else if (uiMeshDrawType == 2) {
                //asModelObj[i][counter].draw(OF_MESH_POINTS);
                //asModelObj[i][counter].draw(OF_MESH_POINTS);
                //stroke(10);
                glPointSize(5);
                ofBlendMode(OF_BLENDMODE_ALPHA);
                
                modelList[i][playFrameSelector].drawVertices();
            } else {
                
                modelList[i][playFrameSelector].draw();
                
                //asModelObj[i][counter].drawFaces();
                //asModelObj[i][counter].draw(OF_MESH_FILL);
            }
            
            glPopMatrix();
            

            /*
            float modelSizeX = modelPosXList[selectMeshId]*1000;
            float modelSizeY = modelHeightList[selectMeshId]*1000;
            float modelSizeZ = modelPosZList[selectMeshId]*1000;
        
            ofTranslate(0,-1*modelHeightList[i]*1000,0);    // set y pos
            ofTranslate(0,-0,modelPosZList[i]*1000);        // hosei
            
            // all model same rotation mapfile
            ofRotateX(uiEditRotX);
            ofRotateY(uiEditRotY);
            ofRotateZ(uiEditRotZ);
            
            double totalRecTime = scanUnixTimeAllItemMax - scanUnixTimeAllItemMin;
            double frameTime = scanUnixTimeLongIntList[i][playFrameSelector] - scanUnixTimeAllItemMin;
            
            //ofTranslate(0,-0,mapNum[i][6] * -svIndex);        // street view distance uiStreetViewDistance
            ofTranslate(0,-0,mapNum[i][6] * -frameTime / 1000.0);        // street view distance uiStreetViewDistance
            
            ofTranslate((float)mapNum[i][0],(float)mapNum[i][1],(float)mapNum[i][2]);        // map
            
            ofScale(1000, 1000, 1000);  // temp debug
            
            ofScale(1, 1, -1);      // fix model direction

            
            
            // adjust dual sync cam
            ofRotateY(mapNum[i][10]);
            ofTranslate(0,0,mapNum[i][11]);
            
            if (uiMeshDrawType == 1) {
                //asModelObj[i][counter].draw(OF_MESH_WIREFRAME);
                //            asModelObj[i][counter].drawWireframe();
                ofSetLineWidth(1);
                modelList[i][playFrameSelector].drawWireframe();
            } else if (uiMeshDrawType == 2) {
                //asModelObj[i][counter].draw(OF_MESH_POINTS);
                //asModelObj[i][counter].draw(OF_MESH_POINTS);
                //stroke(10);
                glPointSize(5);
                ofBlendMode(OF_BLENDMODE_ALPHA);
                
                modelList[i][playFrameSelector].drawVertices();
            } else {
                
                modelList[i][playFrameSelector].draw();
                
                //asModelObj[i][counter].drawFaces();
                //asModelObj[i][counter].draw(OF_MESH_FILL);
            }
             */
            
            glPopMatrix();

            
        }
    }
        
    
    //mesh.draw(); // wavefront
    
    glPopMatrix();  //√ã¬Æ√≤√ä√ú‚àÇ‚Äû√Ö√≥‚Äû√Ö√º‚Ä∞Œ©√ß√ÅŒ©√Ü‚Äû√Ö¬¥√ä√†¬™‚Äû√Ö√¥
    
    eCam.end();
    //ofCam.end();
    
    
    
    

    // UI ------------------------------------------------------------------------------------
    
    if (dispGui) {

        
        if (uiBtnLight) {
            
            ofDisableLighting();        // ‚Äû√Ö√¨‚Äû√á√•‚Äû√á√≠‚Äû√Ö√≥‚Äû√Ö‚Ñ¢‚Äû√Ö√ë‚Äû√Ö¬ÆUI‚Äû√Ö‚Ñ¢‚Äû√Ö¬©‚Äû√Ö√Ü√ã√¢‚â§‚Äû√Ö√•‚Äû√Ö√§‚Äû√Ö√£‚Äû√Ö√≥‚Äû√Ö√®‚Äû√Ö‚Ñ¢‚Äû√á√£
            light.disable();
            /*
             light.setAmbientColor(ofFloatColor(0.2, 0.2, 0.2, 1.0));
             light.setDiffuseColor(ofFloatColor(0.4, 0.4, 0.4));
             light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
             */
            ofDisableDepthTest();
        }

        stringstream tSs;
        ofSetColor(255,255,255,255);

        if (uiBtnDebugInfo) {

            tSs.str("");
            tSs << "FPS: " << fixed << setprecision(1) << ofGetFrameRate() << "fps" << resetiosflags(ios_base::floatfield);
            fontSmall.drawString(tSs.str(), 40, 80);
            
            tSs.str("");
            tSs << "Vertices: " << displayTotalVertices << "pts";
            fontSmall.drawString(tSs.str(), 40, 120);
            
            tSs.str("");
            tSs << "AppInitTime: " << (appInitEndTime - appInitStartTime) << "ms";
            fontSmall.drawString(tSs.str(), 40, 160);

            tSs.str("");
            tSs << "modelLoadingTime: " << (modeldataLoadingEndTime - modeldataLoadingStartTime) << "ms";
            fontSmall.drawString(tSs.str(), 40, 200);
            
            tSs.str("");
            tSs << "modelNum: " << modelDataNum;
            fontSmall.drawString(tSs.str(), 40, 240);
            
            tSs.str("");
            tSs << "files: " << modeldataFiles;
            fontSmall.drawString(tSs.str(), 40, 280);

            tSs.str("");
            tSs << "totalLoadFileSize: " << (loadFileSizeAll/1000/1000) << "MB";
            fontSmall.drawString(tSs.str(), 40, 320);

            tSs.str("");
            tSs << "PlayStartFrame: " << startPlayMeshAnimNum;
            fontSmall.drawString(tSs.str(), 40, 360);
            tSs.str("");
            
            tSs << "maxLoadMeshNum: " << maxLoadMeshNum;
            fontSmall.drawString(tSs.str(), 40, 400);
            
            tSs.str("");
            tSs << "maxLoadedModelNum: " << maxLoadedMeshNumInAllMesh;
            fontSmall.drawString(tSs.str(), 40, 440);

            tSs.str("");
            tSs << "SelectItemMeshNum: " << maxMeshNumList[selectMeshId];
            fontSmall.drawString(tSs.str(), 40, 480);
            
            tSs.str("");
            tSs << "SelectMeshName: " << meshNameList[selectMeshId];
            fontSmall.drawString(tSs.str(), 40, 520);
            
            //maxPlayMeshAnimNum = 10;
            //startPlayMeshAnimNum = 9;


            
            //light.disable();
            
            ofSetColor(255,255,255,255);
            
            //font.drawString("TimeBased 3d Viewer", 50, 70);
            
    //            stringstream tSs;
            tSs.str("");
            tSs << "mouseX: " << mouseX << " mouseY: " << mouseY << "eCam.x: " << eCam.getX() << " eCam.y: " << eCam.getY() << " eCam.z" << eCam.getZ();
            
            fontSmall.drawString(tSs.str(), 50, 700);

            
            ofVec3f worldPos = eCam.screenToWorld(ofVec3f(mouseX, mouseY, 0));
            stringstream tSs2;
            tSs2.str("");
            tSs2 << "worldX: " << worldPos.x << " worldY: " << worldPos.y << " worldZ: " << worldPos.z;
            fontSmall.drawString(tSs2.str(), 50, 750);

            stringstream tSs3;
            
            tSs3 << "viewerMode: " << viewerMode << endl;
            
            fontSmall.drawString(tSs3.str(), 50, 800);
        }

        
        //ofEllipse((playCount%50)*50, 1000, 50, 50);

        if (mouseX >= gui.getPosition().x
            && mouseX < (gui.getPosition().x + gui.getWidth())
            && mouseY >= gui.getPosition().y
            && mouseY < (gui.getPosition().y + gui.getHeight()) ) {
            eCam.disableMouseInput();
        } else {
            eCam.enableMouseInput();
        }
        
        auto posX = position->x;
        auto posY = position->y;
        if (prevPosX != posX || prevPosY != posY) {
            //eCam.setPosition(posX, posY, eCam.getZ());
        }
        
        
        int barWidth = ofGetWidth()*10/10;
        int barX = 0;//ofGetWidth() / 10;
        int progressPosX;
        
        if (frameCount>1) {
            
            if (uiPlayMode == 2) {
                progressPosX = (nowPlayTime  * barWidth ) / (scanUnixTimeAllItemMax - scanUnixTimeAllItemMin);
            } else if (uiPlayMode == 1) {
                progressPosX = (nowPlayTime  * barWidth ) / totalScanTimeRecordMaxTime;
            } else {
                
                if (viewerMode == 0  || viewerMode == 2) {
                    progressPosX = (playCount * barWidth ) / maxMeshNumList[selectMeshId];
                } else if (viewerMode == 1) {
                    progressPosX = (playCount * barWidth ) / maxLoadedMeshNumInAllMesh;
                }
            }

        }

        if (uiBtnDebugInfo) {

            
            ofSetColor(64,64,64, 64);
            ofSetLineWidth(3);
            ofDrawLine(barX, ofGetHeight() - 130, barX+barWidth, ofGetHeight() - 130);
            ofDrawCircle(progressPosX+barX+2, ofGetHeight() - 130+2, 24);
            ofSetColor(255,255,255);
            ofDrawCircle(progressPosX+barX, ofGetHeight() - 130, 20);
            
            tSs.str("");
            tSs << "nowPlayTime: " << nowPlayTime << "    /  endTime: " << totalScanTimeRecordMaxTime;
            fontSmall.drawString(tSs.str(), 40, ofGetHeight() - 230);

            tSs.str("");
            tSs << "playCount: " << playCount << "   /  endMeshNum: " << totalMaxMeshNum;
            fontSmall.drawString(tSs.str(), 40, ofGetHeight() - 190);
        }

        if (uiPlayMode == 2) {
            tSs.str("");
            long nowPlayTimeTemp = nowPlayTime + scanUnixTimeAllItemMin;
            long nowPlayTimeForSeek = nowPlayTimeTemp / 1000;
            cout << "nowPlayTimeForSeek: " << nowPlayTimeForSeek << " playStartDateTime:" << playStartDateTime << endl;
            char charDateTime[100];
            //time(&timer);
            struct tm tempTimeStruct;
            memset(&tempTimeStruct,0x00,sizeof(struct tm));               // Initialize important!
            tempTimeStruct = *localtime(&nowPlayTimeForSeek);
            ::strftime(charDateTime, sizeof(charDateTime), "%Y-%m-%d %H:%M:%S", &tempTimeStruct);
            string strCharDateTime = charDateTime;
            tSs <<  "VirtualTime: " << strCharDateTime << "." << nowPlayTimeTemp%1000;
            fontSmall.drawString(tSs.str(), 40, ofGetHeight() - 150);
        }
        
        if (uiBtnPlayPause) {
    //        fontSmall.drawString("now playing...", 100, 700);
        }


        ofDisableDepthTest();
        if (viewerMode == 0) {
            ofSetColor(255, 0, 0, 192);
        } else {
            ofSetColor(128, 128, 128, 192);
        }
        //ofRect(0, 700, 200, 50);
        ofRectRounded(ofRectangle(10, 700, 180, 50), 10);
        
        ofSetColor(255,255,255,255);
        fontSmall.drawString("Detail", 40, 735);

        if (viewerMode == 1) {
            ofSetColor(255, 1, 0, 192);
        } else {
        //ofRect(0, 700, 200, 50);
            ofSetColor(128, 128, 128, 192);
        }
        ofRectRounded(ofRectangle(210, 700, 180, 50), 10);
        
        ofSetColor(255,255,255,255);
        fontSmall.drawString("File List", 240, 735);
        
        
        if (viewerMode == 2) {
            ofSetColor(255, 0, 0, 192);
        } else {
            //ofRect(0, 700, 200, 50);
            ofSetColor(128, 128, 128, 192);
        }
        ofRectRounded(ofRectangle(410, 700, 180, 50), 10);

        ofSetColor(255,255,255,255);
        fontSmall.drawString("3D Anime", 440, 735);
/*
        if (uiBtnDebugInfo) {
            if (viewerMode == 3) {
                ofSetColor(255, 0, 0, 192);
            } else {
                ofSetColor(128, 128, 128, 192);
            }
            ofRectRounded(ofRectangle(610, 700, 180, 50), 10);
            
            ofSetColor(255,255,255,255);
            fontSmall.drawString("MapEdit", 640, 735);
        }
         */
        
        if (viewerMode == 0 || viewerMode == 2) {
            if (mouseX < 150 && mouseY<600 && mouseY >= 150) {
                ofSetColor(255, 128, 128, 192);
            } else {
                ofSetColor(128, 128, 128, 192);
            }
            ofRectRounded(ofRectangle(35, ofGetHeight()/2+0, 80, 80), 40);
            
            if (mouseX >= (ofGetWidth()-150) && mouseY<600 && mouseY >= 150) {
                ofSetColor(255, 128, 128, 192);
            } else {
                ofSetColor(128, 128, 128, 192);
            }
            
            ofRectRounded(ofRectangle(ofGetWidth()-115, ofGetHeight()/2+0, 80, 80), 40);
            ofSetColor(255, 255, 255, 255);
            font.drawString("<", 60, ofGetHeight()/2+54);
            font.drawString(">", ofGetWidth()-87, ofGetHeight()/2+54);
            
            
        }
    }
    
    // GUI‚Äû√á√≠√ã¬∞¬Æ√Å¬ß‚à´
    
    if (dispGui) {
        if (uiBtnDebugInfo){
            guiMapEdit.draw();
        }
        gui.draw();
    }
    

    
    frameCount++;
    
    if (uiBtnPlayPause) {
        playCount++;
    }

    if (frameCount>1) {
        /*
        if (!uiBtnLoopPlay) {
            if (uiBtnPlayMode) {
                if (nowPlayTime >= totalScanTimeRecordMaxTime) {
                    uiBtnPlayPause = false;
                    nowPlayTime = totalScanTimeRecordMaxTime-1;
                    //playCount = totalMaxMeshNum;
                }
                
            } else {
                if (playCount >= totalMaxMeshNum) {
                    uiBtnPlayPause = false;
                    playCount = totalMaxMeshNum-1;
                }
            }
        } else {
            if (uiBtnPlayMode) {
                nowPlayTime %= totalScanTimeRecordMaxTime;
            } else {
                playCount %= totalMaxMeshNum;
            }
        }
         */
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    //cout << "key number: " << key << endl;
    
    
}
    


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    cout << "key number: " << key << endl;
    
    switch (key) {
            
        case OF_KEY_LEFT:
            selectMeshId = ((selectMeshId - 1) + modelDataNum) % modelDataNum;
            uiEditModelSelector = uiEditModelSelector - 1;
            if (viewerMode == 0 || viewerMode == 2) {
                uiEditModelSelector = selectMeshId;
            }
            demoCamCounter = 0;
            detailCamReset();
            break;

        case OF_KEY_RIGHT:
            selectMeshId = (selectMeshId + 1) % modelDataNum;
            if (viewerMode == 0 || viewerMode == 2) {
                uiEditModelSelector = selectMeshId;
            }
            
            demoCamCounter = 0;
            detailCamReset();
            break;
            
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    if (y >= 500 && y < 700) {
        
        if (uiPlayMode == 2) {
            
            long seekbarCalcTime = (int)(((double)mouseX / (double)ofGetWidth()) * (scanUnixTimeAllItemMax - scanUnixTimeAllItemMin));
            
            seekbarAddTime = (scanUnixTimeAllItemMax - scanUnixTimeAllItemMin) - ((ofGetElapsedTimeMillis() - scanUnixTimeAllItemMin) - (seekbarCalcTime - scanUnixTimeAllItemMin));
            
        } else if (uiPlayMode == 1) {
            
            seekbarAddTime = (int)(((double)mouseX / (double)ofGetWidth()) * totalScanTimeRecordMaxTime);
            
        } else {
            
            playCount = (int)(((double)mouseX / (double)ofGetWidth() ) * totalMaxMeshNum );
            
        }
        
    }

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
   // eCam.setTarget(ofVec3f(1000.0,1000.0,0.0));
    /*
    eCam.setPosition(ofVec3f(1000.0,1000.0,1000.0));
    //eCam.setDistance(2000);
    */
}



void ofApp::exit() {
    
    if ( uiBtnDebugInfo ) {      // EDIT‚Äû√â¬¢‚Äû√â¬∫‚Äû√â√¢
        cout << "save map file." << endl;
        saveMapFile();
    }
    
    
    cout << "program exit." << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    if (x<50 && y<50) {
        if (!dispGui) {
            dispGui = true;
        } else {
            dispGui = false;
        }
    }
    
    if (y >= 700 && y<900) {
        
        if (x >= 0 && x < 200) {
            viewerMode = 0;
            
                selectMeshId = 0;
                
                //int posX2 = int(0 % uiIconNumX) * uiThumbnailIconDistance + uiThumbnailIconDistance/2;
                ///int posY2 = -1 * (int(0 / uiIconNumX) * uiThumbnailIconDistance - uiThumbnailIconDistance/2);
                
                eCam.reset();
                //eCam.setPosition(0, 400, 500);
            
            /*
                eCam.setPosition(0, 0, 1000);
                eCam.setTarget(ofVec3f(0, 0, 0));
            */
            
            
                float modelSizeX = modelPosXList[selectMeshId]*1000 / 2;
                float modelSizeY = modelHeightList[selectMeshId]*1000 / 2;
                float modelSizeZ = modelPosZList[selectMeshId]*1000 / 2;
              /*
                eCam.setPosition(0, -2000, 1300);
                eCam.setTarget(ofVec3f(0, -20000, 1300));
            */
            
            demoMode = 1;
            demoCamCounter = 0;

            detailCamReset();
            

        }

        if (x >= 200 && x < 400) {
            viewerMode = 1;
            eCam.reset();
            eCam.setPosition(3000, -3000, 9000);
            eCam.setTarget(ofVec3f(3000, -3000, 0));
            
        }
        if (x >= 400 && x < 600) {
            viewerMode = 2;
            selectMeshId = 0;
            animeCamReset();
        
        }
        /*
        if (uiBtnDebugInfo) {
            if (x >= 600 && x < 800) {
                viewerMode = 3;
            }
        }
         */
    }
    
    if (viewerMode == 0  && viewerMode == 2) {
        
        if (x < 150 && y<600 && y>=150) {
            
            selectMeshId = ((selectMeshId - 1) + modelDataNum) % modelDataNum;
            uiEditModelSelector = uiEditModelSelector - 1;
                uiEditModelSelector = selectMeshId;
            
            
            demoCamCounter = 0;

            if (viewerMode == 0 ) {
                detailCamReset();
            } else {
                animeCamReset();
            }

        }
        if (x >= (ofGetWidth()-150) && y<600 && y>=150) {

            selectMeshId = (selectMeshId + 1) % modelDataNum;

            uiEditModelSelector = selectMeshId;
            
            demoCamCounter = 0;
            if (viewerMode == 0 ) {
                detailCamReset();
            } else {
                animeCamReset();
            }
        }
    }
    
    

    
}

void ofApp::detailCamReset(){

    demoCamCounter = 0;
    eCam.reset();
    eCam.setPosition(demoCamStartPosX,demoCamStartPosY,  demoCamStartPosZ);
    //eCam.setTarget(ofVec3f(0,10000,1300));
    eCam.lookAt(ofVec3f(0,10000,demoCamStartPosZ), ofVec3f(0,0,1));

    
}

void ofApp::animeCamReset(){
    
    eCam.reset();
    
    float modelSizeX = modelPosXList[selectMeshId]*1000 / 2;
    float modelSizeY = modelHeightList[selectMeshId]*1000 / 2;
    float modelSizeZ = modelPosZList[selectMeshId]*1000 / 2;
    
    eCam.setPosition(modelSizeX, -1300, modelSizeZ/4);
    eCam.setTarget(ofVec3f(modelSizeX, 10000,  modelSizeZ/4));
}




//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

void ofApp::resetCam( ) {
    
    detailCamReset();
}

void ofApp::setPos( ) {
    
    //position.setPosition(eCam.getX(), eCam.getY());
    auto posX = position->x;
    auto posY = position->y;
//    eCam.setPosition(posX, posY, eCam.getZ());
    
}

void ofApp::saveMapFile() {
    
    ofstream ofs( mapFilePath);
    
    for(int i=0; i<modelDataNum; i++) {
        
        ofs << mapId[i] << ",";
        
        for(int j=0; j<mapDataColumns-1; j++) {
            ofs << int(mapNum[i][j]) << ",";
        }
        
        ofs << mapNum[i][mapDataColumns-1];
        
        ofs << endl;
    }
    
    return;
    
}



// Read mesh files ----------------------------------------------------------------
void ofApp::dataLoad() {
    
    modeldataLoadingStartTime = ofGetElapsedTimeMillis();
    
    modeldataDatasetNum = 0;
    modeldataFiles = 0;
    
    
    ofDirectory ofDir;
    //        ofDir.listDir("./");
    ofDir.listDir(meshDataDirPath);
    
    vector<ofFile> files = ofDir.getFiles();
    vector<ofFile>::iterator itr = files.begin();
    int tCount = 0;
    while( itr != files.end() )
    {
        if (itr->isDirectory()) {
            string s = itr->getFileName();
            cout << s << endl;
            dataDirNameList.push_back(s);
            
            if (!mapFileExists) {
                mapId[tCount] = s;
            }
            tCount++;
        }
        ++itr;
    }
    

    
    //  --------------------------------------------------------------------
    
    // Assimp model loader
    int dirNameLoopCount = 0;
    for (auto dirName : dataDirNameList)
    {
        stringstream ss;
        ofSetWindowTitle(ss.str());
        
        {
            cout << "dirName: " << dirName << endl;
            
            stringstream dirPath;
            dirPath.str("");
            int meshFileNum;
            meshFileNum = 0;
            
            ofDirectory ofDir;
            //dirPath << "./" << dirName << "/";
            dirPath << meshDataDirPath << "/" << dirName << "/";
            ofDir.listDir(dirPath.str());
            //ofDir.listDir(meshDataDirPath);
            
            
            meshNameList[dirNameLoopCount] = dirName;
            
            bool staticModelFlag = false;
            
            vector<ofFile> files = ofDir.getFiles();
            vector<ofFile>::iterator itr = files.begin();  //
            while( itr != files.end() )  //
            {
                //cout << "*" << itr->getFileName() << endl;  //
                if (!itr->isDirectory()) {
                    
                    if (itr->getFileName().substr(0,4) == "mesh") {
                        meshFileNum++;
                        
                        //cout << "***" << itr->getFileName() << endl;  //
                    }
                    
                    if (itr->getFileName().substr(0,9) == "Model.obj") {
                        meshFileNum++;
                        staticModelFlag = true;
                    }
                    
                    //dataDirNameList.push_back(itr->getFileName());
                }
                ++itr;                 //
            }
            
            maxMeshNumList[dirNameLoopCount] = meshFileNum;
            
            
            
            
            cout << "mesh file num: " << meshFileNum << endl;  // *√ä¬∫√Æ√Å√Ü√≥√Ç‚â†√™‚Äû√Ö√ü√à√±√¨√ä√©‚Ä¢√Ç√®√á√Å√ñ√ü
            
            
            
            if (staticModelFlag == false) {
                
                struct tm tempTmStruct;
                memset(&tempTmStruct,0x00,sizeof(struct tm));               // Initialize important!

                // getTimestamp
                ss.str("");
                ss << dirPath.str() << "scanTimeRecord.csv";

                ofFileObj.open(ss.str());
                int idx = 0;
                if (ofFileObj.exists()) {
                    ofBuffer buffer(ofFileObj);
                    
                    while (!buffer.isLastLine() && idx<maxLoadMeshNum) {
                        string line = buffer.getNextLine();
                        //cout << ""line;
                        if (line != "") {
                            auto itemList = ofSplitString(line, ",");
                            //cout << itemList[1] << endl;
                            scanTimeRecordList[dirNameLoopCount][idx][0] = stoi(itemList[0]);
                            scanTimeRecordList[dirNameLoopCount][idx][1] = stoi(itemList[1]);
                            
                            //itemlist[2] ... "2016/10/09 17:26:56.432"
                            string strDateTime = itemList[2].substr(0, 19);           // 2016/10/09 17:26:56
                            string strMilliSec = itemList[2].substr(20, 3);           // .432
                            
                            const char *cStrDateTime = strDateTime.c_str();
                            
                            ::strptime(cStrDateTime, "%Y/%m/%d %H:%M:%S", &tempTmStruct);
                            
                            scanTimeRecordTimeStructureList[dirNameLoopCount][idx] = mktime(&tempTmStruct);
                            scanUnixTimeLongIntList[dirNameLoopCount][idx] = ((long)mktime(&tempTmStruct) * 1000) + stoi(strMilliSec);
                            
                            // min/max keep -----------------
                            if (scanUnixTimeModelMinList[dirNameLoopCount] >= scanUnixTimeLongIntList[dirNameLoopCount][idx]) {
                                scanUnixTimeModelMinList[dirNameLoopCount] = scanUnixTimeLongIntList[dirNameLoopCount][idx];
                            }
                            
                            if (scanUnixTimeModelMaxList[dirNameLoopCount] <= scanUnixTimeLongIntList[dirNameLoopCount][idx]) {
                                scanUnixTimeModelMaxList[dirNameLoopCount] = scanUnixTimeLongIntList[dirNameLoopCount][idx];
                            }
                            
                            if (scanUnixTimeAllItemMin >= scanUnixTimeLongIntList[dirNameLoopCount][idx]) {
                                scanUnixTimeAllItemMin = scanUnixTimeLongIntList[dirNameLoopCount][idx];
                            }
                            
                            if (scanUnixTimeAllItemMax <= scanUnixTimeLongIntList[dirNameLoopCount][idx]) {
                                scanUnixTimeAllItemMax = scanUnixTimeLongIntList[dirNameLoopCount][idx];
                            }
                            // --------------------------
                            
                            //cout << strDateTime << "  ." << strMilliSec << " --- " << scanUnixTimeLongIntList[dirNameLoopCount][idx] << endl;

                            idx++;
                        }
                        
                    }
                    scanTimeRecordMaxTime[dirNameLoopCount] = scanTimeRecordList[dirNameLoopCount][idx-1][1];
                    cout << "scanTimeRecordMaxTime: " << scanTimeRecordMaxTime[dirNameLoopCount] << endl;
                    cout << "idx: " << idx << endl;
                    
                    ofFileObj.close();
                    
                }
                
                maxMeshNumList[dirNameLoopCount] = idx - 2;
                if (idx > maxLoadedMeshNumInAllMesh) {
                    maxLoadedMeshNumInAllMesh = idx;
                }
                oneModelFileSizeList.clear();
                
                for(int i=0; i<maxMeshNumList[dirNameLoopCount]; i++) {

                    auto& model = modelList[dirNameLoopCount][i];
                    
                    ss.str("");
                    ss << dirPath.str() << "mesh_" << (i+2+startPlayMeshAnimNum) << ".obj";
                    
                    ofFileObj.open(ss.str());
                    if (ofFileObj.exists()) {
                        
                        ss.str("");
                        ss << dirPath.str() << "mesh_" << (i+2+startPlayMeshAnimNum) << ".obj";
                        //ss << "F:/ArtDKT_kuwakubo_3dscan_20160123to25/artdkt_3dscan_20160124_kouhan/artdkt_structure3d/38/mesh_" << (i+2) << ".obj";
                        
                        cout << ss.str() << endl;
                        
                        oneModelFileSizeList.push_back(ofFileObj.getSize()); // getFileSize
                        
                        ofFileObj.close();
                        //ofxObjLoader::load(ss.str(), modelList[dirNameLoopCount][i], false);
                        ofxObjLoader::load(ss.str(), modelList[dirNameLoopCount][i], true, false);



                        ss.str("");
                        ss << dirPath.str() << "mesh_" << (i+2+startPlayMeshAnimNum) << ".jpg";
                        
                        //modelImageList[dirNameLoopCount][i].allocate(640,480,OF_IMAGE_COLOR_ALPHA);
                        modelImageList[dirNameLoopCount][i].allocate(640,480,OF_IMAGE_COLOR_ALPHA);
                        modelImageList[dirNameLoopCount][i].loadImage(ss.str());
                        //ofImage tImg;
                        //tImg.loadImage(ss.str());
                        //ofPixels pixels = tImg.getPixels();
                        //modelImageList[dirNameLoopCount][i].loadData(pixels);
                        /*
                        auto texes = modelList[dirNameLoopCount][i].getTexCoords();
                        for(auto tex : texes) {
                            cout << "tex: (" << tex.x << ", " << tex.y << ")" << endl;
                        }
                         */
                        
                        //modelList[dirNameLoopCount][i].disableColors();
                        modelList[dirNameLoopCount][i].enableIndices();
                        modelList[dirNameLoopCount][i].enableNormals();
                        modelList[dirNameLoopCount][i].enableTextures();
                        
                        auto vertices = modelList[dirNameLoopCount][i].getVertices();
                        
                        float maxPosY = 0;
                        for(auto vertice : vertices) {
                            if (maxPosY <= vertice.y) {
                                maxPosY = vertice.y;
                            }
                        }
                        modelHeightList[dirNameLoopCount] = maxPosY;
                        
                        
                        float maxPosX = 0;
                        for(auto vertice : vertices) {
                            if (maxPosX <= vertice.x) {
                                maxPosX = vertice.x;
                            }
                        }
                        modelPosXList[dirNameLoopCount] = maxPosX;

                        float maxPosZ = 0;
                        for(auto vertice : vertices) {
                            if (maxPosZ <= vertice.z) {
                                maxPosZ = vertice.z;
                            }
                        }
                        modelPosZList[dirNameLoopCount] = maxPosX;

                        
                        cout << "vertice.y: " << maxPosY;
                        
                    } else {
                        cout << dirPath.str() << "mesh_" << (i+2+startPlayMeshAnimNum) << ".obj file not found" << endl;
                    }
                    /*

                     Assimp ver.
                     
                     //asModelObj[dirNameLoopCount][i].loadModel(ss.str(), true );   true:ÊúÄÈÅ©Âåñ„Åô„Çã„Å®ÂÆüË°åÊôÇ„Å´ËêΩ„Å°„Çã! oF0.9.3 OSX10.11.5 LLVM0.7.1

                    auto& model = asModelObj[dirNameLoopCount][i];
                     
                    model.loadModel(ss.str());
                    
                    //auto& material = model.getMaterialForMesh(0);
                    //material.begin();
                    //model.enableMaterials();
                    //model.enableTextures();

                    //model.draw(OF_MESH_FILL);
                    //material.end();
                    model.disableMaterials();
                    model.disableTextures();
                     */
                    
                    cout << "file load: " << ss.str() << endl;
                }
                modelFileSizeList.push_back(oneModelFileSizeList);
                
            } else {        // static model
                
                maxMeshNumList[dirNameLoopCount] = 1;
                scanTimeRecordList[dirNameLoopCount][0][0] = -1;
                scanTimeRecordMaxTime[dirNameLoopCount] = 0;
                
                ss.str("");
                ss << dirPath.str() << "/Model.obj";
                
                ofFileObj.open(ss.str());
                
                if (ofFileObj.exists()) {
                    
                    oneModelFileSizeList.push_back(ofFileObj.getSize()); // getFileSize
                    
                    ofFileObj.close();
                    
                    ofxObjLoader::load(ss.str(), modelList[dirNameLoopCount][0], false);
                }
                
                /*
                 Assimp ver
                asModelObj[dirNameLoopCount][0].loadModel(ss.str() , true);
                
                asModelObj[dirNameLoopCount][0].disableMaterials();
                asModelObj[dirNameLoopCount][0].disableTextures();
                 */
                
                cout << "Model load: " << ss.str() << endl;
                
            }
            
            //gui->addWidgetRight(new ofxUIDropDownList("PARTICLE_IMAGE", items,150));
        }
        
        modeldataFiles += maxMeshNumList[dirNameLoopCount];

        /*
         */
        dirNameLoopCount++;
    }
    modelDataNum = dirNameLoopCount;
    //modeldataDatasetNum = dirNameLoopCount;
    
    
    
    loadFileSizeAll = 0;
    for(auto oneModelList : modelFileSizeList) {
        for(auto fileSize : oneModelFileSizeList) {
            loadFileSizeAll += fileSize;
        }
    }
    
    
    totalMaxMeshNum = 0;
    for (auto meshNum : maxMeshNumList) {
        if (meshNum > totalMaxMeshNum) {
            totalMaxMeshNum = meshNum;
        }
    }

    totalScanTimeRecordMaxTime = 0;
    for (auto time : scanTimeRecordMaxTime) {
        if (time > totalScanTimeRecordMaxTime) {
            totalScanTimeRecordMaxTime = time;
        }
    }
    
    cout<<"modelDataNum:"<<modelDataNum<<endl;
    
    
    modeldataLoadingEndTime = ofGetElapsedTimeMillis();

}


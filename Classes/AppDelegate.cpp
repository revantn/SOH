#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Game/Board.h"

USING_NS_CC;

#define DESIGN_WIDTH 960.0
#define DESIGN_HEIGHT 640.0

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
	std::vector<std::string> resourceOrder;
    if(!glview) {
        glview = GLViewImpl::create("7 Of Hearts");
        director->setOpenGLView(glview);
		//glview->setFrameSize(1280, 720);
		//glview->setFrameSize(800, 480);
		//glview->setFrameSize(960, 640);

		// Set the design resolution
		//glview->set
		CCSize frameSize = glview->getFrameSize();

		//if (frameSize.width >= 960)
		//{
			//resourceOrder.push_back("960X640");
		//}
		//else //if (frameSize.width < 960)
		//{
//			resourceOrder.push_back("800X480");
	//	}
		
		//FileUtils::getInstance()->setSearchPaths( resourceOrder );
		//FileUtils::getInstance()->setSearchResolutionsOrder(resourceOrder);
		director->setContentScaleFactor(frameSize.height / DESIGN_HEIGHT);
		glview->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, kResolutionFixedHeight);

		
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
#ifdef USE_SCALE_FACTOR
	auto scaleX = glview->getFrameSize().width / DESIGN_WIDTH;
	auto scaleY = glview->getFrameSize().height / DESIGN_HEIGHT;

	Board::setScaleFactors(scaleX, scaleY);
#endif
    // create a scene. it's an autorelease object
	auto scene = Board::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

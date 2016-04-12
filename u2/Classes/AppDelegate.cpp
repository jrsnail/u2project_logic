#include "AppDelegate.h"

#include "U2Core.h"
#include "U2Mvc.h"
#include "application/ApplicationFacade.h"
#include "application/ApplicationPrerequisites.h"
#include "cg/CgFacade.h"
#include "U2TaskGroup.h"
#include "U2FilterStream.h"
#include "U2FileStream.h"
#include "U2FileHandleStream.h"
#include "U2DataFilterStream.h"
#include "U2StreamQueue.h"
#include "U2PipedStream.h"



USING_NS_CC;

static cocos2d::Size winResolutionSize = cocos2d::Size(640, 1136);
static cocos2d::Size designResolutionSize = cocos2d::Size(640, 1136);
static cocos2d::Size smallResolutionSize = cocos2d::Size(320, 480);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(768, 1024);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1536, 2048);

AppDelegate::AppDelegate() 
	: m_pLogManager(nullptr)
{

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
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 8, 8 };
#else
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
#endif

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

class TestInThread : public ThreadPoolTask
{
public:
	TestInThread(PipedInStream* in) 
		: ThreadPoolTask()
		, m_pInStream(in)
	{

	}

	virtual void operator()() override
	{
// 		size_t uTotal = 23;
// 		size_t uCurCount = 0;
// 		u2byte* pBuf = static_cast<u2byte*>(U2_MALLOC(uTotal, MEMCATEGORY_GENERAL));
// 		while (uCurCount < uTotal)
// 		{
// 			uCurCount += m_pInStream->read(pBuf + uCurCount, uTotal - uCurCount);
// 		}
// 
// 		u2::String s((const char*)pBuf);
// 		int a = 0;

		FileOutStream* pFileOut = U2_NEW FileOutStream("bbb", "D://bbb.zip", std::ios_base::out | std::ios_base::binary);
		const size_t BUF_SIZE = 1000;
		u2char s[BUF_SIZE] = { 0 };
		size_t uTotal = 0;
		while (!m_pInStream->eof())
		{
			size_t uReadCount = m_pInStream->read(s, BUF_SIZE);
			size_t uWriteCount = 0;
			while (uWriteCount < uReadCount)
			{
				uWriteCount += pFileOut->write(s + uWriteCount, uReadCount - uWriteCount);
			}
			uTotal += uWriteCount;
		}
		pFileOut->close();
		m_pInStream->close();
	}

private:
	PipedInStream* m_pInStream;
};

class TestOutThread : public ThreadPoolTask
{
public:
	TestOutThread(PipedOutStream* out)
		: ThreadPoolTask()
		, m_pOutStream(out)
	{

	}

	virtual void operator()() override
	{
// 		u2::String szStr = "abcdefghijklmnopqrstuv";
// 		size_t uCurCount = 0;
// 		while (uCurCount < szStr.size())
// 		{
// 			uCurCount += m_pOutStream->write((const u2byte*)szStr.c_str() + uCurCount, szStr.size());
// 		}
// 		
// 		int a = 0;

		FileInStream* pFileIn = U2_NEW FileInStream("aaa", "D://messagebox.zip", std::ios_base::in | std::ios_base::binary);
		const size_t BUF_SIZE = 1000;
		u2char s[BUF_SIZE] = {0};
		size_t uTotal = 0;
		while (!pFileIn->eof())
		{
			size_t uReadCount = pFileIn->read(s, BUF_SIZE);
			size_t uWriteCount = 0;
			while (uWriteCount < uReadCount)
			{
				uWriteCount += m_pOutStream->write(s + uWriteCount, uReadCount - uWriteCount);
			}
			uTotal += uWriteCount;
		}
		pFileIn->close();
		m_pOutStream->close();
	}

private:
	PipedOutStream* m_pOutStream;
};

bool AppDelegate::applicationDidFinishLaunching() {
	cocos2d::log("Hello world! CCLog! = %d", std::this_thread::get_id());
	CCLOG("Hello world! CCLOG");

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("u2", Rect(0, 0, winResolutionSize.width, winResolutionSize.height));
#else
        glview = GLViewImpl::create("u2");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
    //glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    cocos2d::Size frameSize = glview->getFrameSize();
	//director->setContentScaleFactor(2.0f);
	/*
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }
	*/

    register_all_packages();


	FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->getWritablePath());
	cocos2d::FileUtils::getInstance()->addSearchPath("res");
	cocos2d::FileUtils::getInstance()->addSearchPath("res/ui/plist");
	cocos2d::FileUtils::getInstance()->addSearchPath("res/ui/application");
	cocos2d::FileUtils::getInstance()->addSearchPath("res/ui/cg");

	m_pFrameListenerCollection = new CocosFrameListenerCollection;

// 	PredefinedFacade::getSingleton();
// 	CgFacade::getSingleton();
// 	ApplicationFacade::getSingleton().startup();

	Facade::createFacade<PredefinedFacade>(ON_Facade_Predefined);
	Facade::createFacade<CgFacade>(ON_Facade_Cg);
	ApplicationFacade* pAppFacade = Facade::createFacade<ApplicationFacade>(ON_Facade_Application);
	if (pAppFacade != nullptr)
	{
		pAppFacade->startup();
	}
	


	// Create log manager and default log file if there is no log manager yet
	if (u2::LogManager::getSingletonPtr() == nullptr)
	{
		m_pLogManager = U2_NEW u2::LogManager();
		m_pLogManager->createLog("u2.log", true, true);
	}

// #if U2_PLATFORM == U2_PLATFORM_ANDROID
// 	mAndroidLogger = U2_NEW AndroidLogListener();
// 	m_pLogManager->getDefaultLog()->addListener(mAndroidLogger);
// #endif

	{
		//------------------------------- Test Thread ----------------------------------------
		/*
		// Create a thread pool
		TaskGroup group(1);

		// Run a function in the thread pool
		for (int i = 0; i < 1; ++i)
		{
			group.run([i] {
				//cocos2d::log("Hello world! run = %d, %d", i, std::this_thread::get_id());
				//CCLOG("Hello world! run = %d, %d", i, std::this_thread::get_id());
				u2::LogManager::getSingleton().stream(u2::LML_NORMAL) 
					<< "Hello world! run = " << i << ", " << std::this_thread::get_id() 
					<< "\n";
				});
			//pool.run([i] { std::cout << "Hello world! = " << i << std::endl; });
		}

		// Wait for all queued functions to finish and the pool to become empty
		group.wait();
		u2::LogManager::getSingleton().stream(u2::LML_NORMAL)
			<< "Hello world! main = " << std::this_thread::get_id()
			<< "\n";
		cocos2d::log("Hello world! Thread over!");
		*/
	}
	

	{
		//------------------------------- Test ObjectCollection ----------------------------------------
		/*
		ObjectCollection<u2::Context> collect;
		collect.createObject(OT_Context, "aaa");
		collect.createObject(OT_Context, "ddd");
		collect.createObject(OT_Context, "eee");
		collect.createObject(OT_Context, "www");
		collect.createObject(OT_Context, "ccc");
		collect.createObject(OT_Context, "www");

		ObjectCollection<u2::Context>::ObjectMapIterator objIt = collect.retrieveAllObjectsByName("www");
		while (objIt.hasMoreElements())
		{
			u2::Context* pContext = dynamic_cast<u2::Context*>(objIt.getNext());
			CCLOG("=========Name, %s, %s", pContext->getName().c_str(), pContext->getGuid().c_str());
		}

		ObjectCollection<u2::Context>::ObjectMapIterator objIt2 = collect.retrieveAllObjectsByTN(OT_Context, "www");
		while (objIt2.hasMoreElements())
		{
			u2::Context* pContext = dynamic_cast<u2::Context*>(objIt2.getNext());
			CCLOG("=========TN, %s, %s", pContext->getName().c_str(), pContext->getGuid().c_str());
		}

		ObjectCollection<u2::Context>::ObjectMapIterator objIt3 = collect.retrieveAllObjectsByType(OT_Context);
		while (objIt3.hasMoreElements())
		{
			u2::Context* pContext = dynamic_cast<u2::Context*>(objIt3.getNext());
			CCLOG("=========Type, %s, %s", pContext->getName().c_str(), pContext->getGuid().c_str());
		}

		u2::Context* pContext = collect.retrieveObjectByGuid("class u2::Context6");
		CCLOG("=========Guid, %s, %s", pContext->getName().c_str(), pContext->getGuid().c_str());
		*/
	}
	
	{
		// Test Stream
// 		FilterOutQueue<DataFilterOutStream> out;
// 		out.push<FileOutStream>("aaa", "D://aaa.txt", std::ios_base::out);
// 		out.push<DataFilterOutStream>("bbb");
// 		out->writeInt32(100);
// 		out->close();
// 
// 		FilterInQueue<DataFilterInStream> in;
// 		in.push<FileInStream>("aaa", "D://aaa.txt", std::ios_base::in);
// 		in.push<DataFilterInStream>("bbb");
// 		u2int32 n = in->readInt32();

// 		OutStreamQueue<DataFilterOutStream> out;
// 		out.push<FileHandleOutStream>("aaa", "D://aaa.txt", "wb");
// 		out.push<DataFilterOutStream>("bbb");
// 		out->writeInt64(150);
// 		out->close();
// 
// 		InStreamQueue<DataFilterInStream> in;
// 		in.push<FileHandleInStream>("aaa", "D://aaa.txt", "rb");
// 		in.push<DataFilterInStream>("bbb");
// 		u2int64 n = in->readInt64();

		PipedInStream* pPipeIn = new PipedInStream;
		PipedOutStream* pPipeOut = new PipedOutStream(pPipeIn);
		//pPipeIn->connect(pPipeOut);

		TestInThread* pInTread = new TestInThread(pPipeIn);
		TestOutThread* pOutTread = new TestOutThread(pPipeOut);
		TaskGroup group(2);
		group.run(pInTread);
		group.run(pOutTread);
		group.wait();

		int bbb = 0;
	}

    
	
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

// to enable CCLOG()
#define COCOS2D_DEBUG 1

#include "cocos2d.h"
#include "FileOperation.h"
#include <stdio.h>

using namespace std;

void FileOperation::saveFile(char* buffer)
{
	CCLOG("In FIle SAvE");
	string path = getFilePath();
	FILE *fp = fopen(path.c_str(), "w");

	if (! fp)
	{
		CCLOG("can not create file %s", path.c_str());
		return;
	}

	fputs(buffer, fp);
	fclose(fp);

	CCLOG("In FIle ENDddddddddd SAvE");
}

void FileOperation::readFile(char* buffer)
{
	CCLOG("In FIle Readdddddddddddddddd");
	string path = getFilePath();
	FILE *fp = fopen(path.c_str(), "r");
	char buf[50] = {0};

	if (! fp)
	{
		CCLOG("can not open file %s", path.c_str());
		return;
	}

	fgets(buffer, 50, fp);
	CCLOG("read content %s", buf);

	fclose(fp);
	CCLOG("In FIle Readdddddddd end");
}

string FileOperation::getFilePath()
{
	string path("");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// In android, every programe has a director under /data/data.
	// The path is /data/data/ + start activity package name.
	// You can save application specific data here.
	path.append("/data/data/com.rgames.SOH/me.sav");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// You can save file in anywhere if you have the permision.
	path.append("E:/PI/SOH_client/SOH/proj.win32/Debug.win32/me.sav");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)
	path = cocos2d::CCApplication::sharedApplication().getAppDataPath();

#ifdef _TRANZDA_VM_
	// If runs on WoPhone simulator, you should insert "D:/Work7" at the
	// begin. We will fix the bug in no far future.
	path = "D:/Work7" + path;
	path.append("tmpfile");
#endif

#endif

	return path;
}

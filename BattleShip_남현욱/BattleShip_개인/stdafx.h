// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.


#include <list>
#include <vector>
#include <map>

#include <crtdbg.h>
#include <assert.h>

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK,__FILE__, __LINE__)
#pragma comment(lib, "BGNL_debug.lib")
#else
#pragma comment(lib, "BGNL.lib")
#endif //_DEBUG

#include "Network.h"
#include <Windows.h>
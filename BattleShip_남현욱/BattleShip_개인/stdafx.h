// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.


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
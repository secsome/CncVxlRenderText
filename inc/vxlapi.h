#pragma once

#include <vxlfile.h>
#include <vplfile.h>

#define EXPORT __declspec(dllexport)

extern "C" 
{
	extern thomas::vxlfile* CncImgCurrentVXL;
	extern thomas::vplfile* CncImgCurrentVPL;

	EXPORT void* WINAPI CncImgMalloc(unsigned int sz);
	EXPORT void WINAPI CncImgFree(void* ptr);

	EXPORT void WINAPI CncImgCreate();
	EXPORT void WINAPI CncImgRelease();

	EXPORT void WINAPI CncImgSetMaxFacing(unsigned int nFacingCount);
	EXPORT unsigned int WINAPI CncImgGetMaxFacing();

	EXPORT bool WINAPI CncImgLoadVPLFile(const unsigned char* pBuffer);
	EXPORT bool WINAPI CncImgLoadVXLFile(const unsigned char* pBuffer);
	EXPORT bool WINAPI CncImgLoadHVAFile(const unsigned char* pBuffer);

	EXPORT bool WINAPI CncImgPrepareVXLCache(unsigned int nFacing, int F, int L, int H);
	EXPORT int WINAPI CncImgGetImageWidth(unsigned int nFacing);
	EXPORT int WINAPI CncImgGetImageHeight(unsigned int nFacing);
	EXPORT void WINAPI CncImgGetImageFrame(unsigned int nFacing, int* width, int* height, int* x, int* y);
	EXPORT void WINAPI CncImgGetImageSize(unsigned int nFacing, int* width, int* height);
	EXPORT bool WINAPI CncImgGetImageData(unsigned int nFacing, unsigned char** ppBuffer);

	EXPORT void WINAPI CncImgClearCurrentVXL();
	EXPORT bool WINAPI CncImgIsVXLLoaded();
	EXPORT bool WINAPI CncImgIsVPLLoaded();
}
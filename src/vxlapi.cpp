#include <vxlapi.h>

extern "C"
{
	thomas::vxlfile* CncImgCurrentVXL = nullptr;
	thomas::vplfile* CncImgCurrentVPL = nullptr;

	void* WINAPI CncImgMalloc(unsigned int sz)
	{
		return malloc(sz);
	}

	void WINAPI CncImgFree(void* ptr)
	{
		free(ptr);
	}

	void WINAPI CncImgCreate()
	{
		CncImgCurrentVXL = new thomas::vxlfile;
		CncImgCurrentVPL = new thomas::vplfile;
	}

	void WINAPI CncImgRelease()
	{
		delete CncImgCurrentVXL; CncImgCurrentVXL = nullptr;
		delete CncImgCurrentVPL; CncImgCurrentVPL = nullptr;
	}

	void WINAPI CncImgSetMaxFacing(unsigned int nFacingCount)
	{
		thomas::vxlfile::direction_count = nFacingCount;
	}

	unsigned int WINAPI CncImgGetMaxFacing()
	{
		return thomas::vxlfile::direction_count;
	}

	bool WINAPI CncImgLoadVPLFile(const unsigned char* pBuffer)
	{
		return CncImgCurrentVPL->load(pBuffer);
	}

	bool WINAPI CncImgLoadVXLFile(const unsigned char* pBuffer)
	{
		return CncImgCurrentVXL->load(pBuffer);
	}

	bool WINAPI CncImgLoadHVAFile(const unsigned char* pBuffer)
	{
		return CncImgCurrentVXL->load_hva(pBuffer);
	}

	bool WINAPI CncImgPrepareVXLCache(unsigned int nFacing, int F, int L, int H)
	{
		return CncImgCurrentVXL->prepare_single_dir_cache(nFacing, *CncImgCurrentVPL, F, L, H);
	}

	int WINAPI CncImgGetImageWidth(unsigned int nFacing)
	{
		return CncImgCurrentVXL->frame_cache(nFacing).frame_bound.right - CncImgCurrentVXL->frame_cache(nFacing).frame_bound.left;
	}

	int WINAPI CncImgGetImageHeight(unsigned int nFacing)
	{
		return CncImgCurrentVXL->frame_cache(nFacing).frame_bound.bottom - CncImgCurrentVXL->frame_cache(nFacing).frame_bound.top;
	}

	void WINAPI CncImgGetImageFrame(unsigned int nFacing, int* width, int* height, int* x, int* y)
	{
		const auto& bound = CncImgCurrentVXL->frame_cache(nFacing).frame_bound;
		if (width)
			*width = bound.right - bound.left;
		if (height)
			*height = bound.bottom - bound.top;
		if (x)
			*x = bound.left;
		if (y)
			*y = bound.top;
	}

	void WINAPI CncImgGetImageSize(unsigned int nFacing, int* width, int* height)
	{
		CncImgGetImageFrame(nFacing, width, height, nullptr, nullptr);
	}

	bool WINAPI CncImgGetImageData(unsigned int nFacing, unsigned char** ppBuffer)
	{
		if (!ppBuffer)
			return false;

		int w, h;
		CncImgGetImageSize(nFacing, &w, &h);
		*ppBuffer = (unsigned char*)CncImgMalloc(w * h);
		memcpy(*ppBuffer, CncImgCurrentVXL->frame_cache(nFacing).cache.get(), w * h);

		return true;
	}

	void WINAPI CncImgClearCurrentVXL()
	{
		CncImgCurrentVXL->clear();
	}

	bool WINAPI CncImgIsVXLLoaded()
	{
		return CncImgCurrentVXL->is_loaded();
	}

	bool WINAPI CncImgIsVPLLoaded()
	{
		return CncImgCurrentVPL->is_loaded();
	}
}



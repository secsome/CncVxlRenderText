#include "fileformats.h"
#include "Utilities.h"

#ifdef WIN32

#ifndef DLL_PROCESS_DETACH
#define DLL_PROCESS_DETACH 0
#endif
#ifndef DLL_PROCESS_ATTACH
#define DLL_PROCESS_ATTACH 1
#endif

bool WINAPI DllMain(void* base, uint32_t reason, void* parameters)
{
	if (reason == DLL_PROCESS_ATTACH)
		thomas::logger::prepare_log();
	else if (reason == DLL_PROCESS_DETACH)
		thomas::logger::close_log();

	return true;
}

#endif

#pragma pack(push, 2)
typedef struct tagBITMAPFILEHEADER {
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
} BITMAPFILEHEADER;
#pragma pack(pop)

typedef struct tagBITMAPINFOHEADER {
	uint32_t biSize;
	int32_t  biWidth;
	int32_t  biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t  biXPelsPerMeter;
	int32_t  biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	byte rgbBlue;
	byte rgbGreen;
	byte rgbRed;
	byte rgbReserved;
} RGBQUAD;

#ifndef BI_RGB
#define BI_RGB 0L
#endif

bool save_cache_to_bitmap(const std::string& filename, thomas::vxlfile& vxl, thomas::palette& palette, const size_t index = 0)
{
	if (!vxl.is_loaded() || !palette.is_loaded())
		return false;

	std::ofstream file(filename + ".bmp", std::ios::out | std::ios::binary);
	if (!file)
		return false;

	thomas::vxlfile::cache_storage cache = vxl.frame_cache(index);
	thomas::vxlfile::cache_storage shadow = vxl.shadow_cache(index);

	BITMAPFILEHEADER file_header;
	BITMAPINFOHEADER info_header;
	RGBQUAD color_index[256];

	memset(&file_header, 0, sizeof(file_header)); 
	memset(&info_header, 0, sizeof(info_header));

	int32_t width = cache.frame_bound.right - cache.frame_bound.left;
	int32_t height = cache.frame_bound.bottom - cache.frame_bound.top;
	int32_t swidth = shadow.frame_bound.right - shadow.frame_bound.left;
	int32_t sheight = shadow.frame_bound.bottom - shadow.frame_bound.top;

	int64_t stride = ((width * 8 + 31) & ~31) >> 3;
	int64_t sstride = ((swidth * 8 + 31) & ~31) >> 3;

	file_header.bfSize = sizeof(file_header);
	file_header.bfType = 'MB';
	file_header.bfOffBits = sizeof(file_header) + sizeof(info_header) + sizeof(color_index);

	info_header.biSize = sizeof(info_header);
	info_header.biWidth = width;
	info_header.biHeight = -height;
	info_header.biPlanes = 1;
	info_header.biBitCount = 8;
	info_header.biCompression = BI_RGB;
	info_header.biSizeImage = static_cast<uint32_t>(stride * height);

	for (uint32_t i = 0; i <= 0xffu; i++)
		color_index[i] = { palette[i].b,palette[i].g,palette[i].r,0 };

	file.write(reinterpret_cast<char*>(&file_header), sizeof(file_header));
	file.write(reinterpret_cast<char*>(&info_header), sizeof(info_header));
	file.write(reinterpret_cast<char*>(color_index), sizeof(color_index));

	for (uint32_t l = 0; l < static_cast<uint32_t>(height); l++)
	{
		file.write(reinterpret_cast<char*>(&cache.cache[l * width]), width);
		file.write(reinterpret_cast<char*>(&cache.cache[l * width]), stride - width);
	}

	file.flush();
	file.close();

	info_header.biWidth = swidth;
	info_header.biHeight = -sheight;
	info_header.biSizeImage = static_cast<uint32_t>(sstride * sheight);

	file.open(filename + "_shadow.bmp", std::ios::out | std::ios::binary);
	file.write(reinterpret_cast<char*>(&file_header), sizeof(file_header));
	file.write(reinterpret_cast<char*>(&info_header), sizeof(info_header));
	file.write(reinterpret_cast<char*>(color_index), sizeof(color_index));

	for (uint32_t l = 0; l < static_cast<uint32_t>(sheight); l++)
	{
		file.write(reinterpret_cast<char*>(&shadow.cache[l * swidth]), swidth);
		file.write(reinterpret_cast<char*>(&shadow.cache[l * swidth]), sstride - swidth);
	}

	file.close();

	return true;
}

int main(int argc, const char** argv)
{
	using namespace thomas;

	if (argc != 6 && argc != 7)
	{
		printf("Usage: %s <VPLPath> <PalPath> <VxlPath> <HvaPath> <OutPath> [DirCount]\n", argv[0]);
		return 0;
	}
	
	const std::string vplname = argv[1];
	const std::string palname = argv[2];
	const std::string vxlname = argv[3];
	const std::string hvaname = argv[4];
	const char* outdir = argv[5];
	if (argc == 7)
		vxlfile::direction_count = atoi(argv[6]);

	vxlfile file;
	palette palette(palname);
	
	if (vplfile::load_global(vplname))
		std::cout << "Vpl is loaded.\n";
	else
	{
		std::cout << "Vpl is not loaded.\n";
		return 1;
	}

	if (file.load(vxlname))
		std::cout << "Vxl is loaded.\n";
	else
	{
		std::cout << "Vxl is not loaded.\n";
		return 1;
	}

	if (file.load_hva(hvaname))
		std::cout << "Hva is loaded.\n";
	else
	{
		std::cout << "Hva is not loaded.\n";
		return 1;
	}

	if (file.prepare_all_cache(vplfile::global, 0, 0, 0))
		std::cout << "All caches engaged.\n";
	else
	{
		std::cout << "Not all caches engaged.\n";
		return 1;
	}

	if (file.is_loaded())
		std::cout << "File is loaded.\n";
	else
	{
		std::cout << "File is not loaded.\n";
		return 1;
	}

	file.print_info();
	if (!std::filesystem::exists(outdir))
	{
		if (!std::filesystem::create_directory(outdir))
		{
			std::cout << "Directory is not created.\n";
			return 1;
		}
	}
	
	std::filesystem::current_path(outdir);
	std::cout << "Current path set to " << std::filesystem::current_path() << std::endl;

	for (size_t i = 0; i < vxlfile::direction_count; i++)
		save_cache_to_bitmap(std::string("out_") + std::to_string(i), file, palette, i);

	std::cout << vxlfile::direction_count << " bitmaps saved.\n";

	return 0;
}
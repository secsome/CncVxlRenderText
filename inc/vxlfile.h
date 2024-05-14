#pragma once

#include "definitions.h"

CLASSES_START

enum class normal_type :byte
{
	tiberian_sun = 2,
	red_alert = 4,
};

class hvafile
{
public:
	hvafile();
	~hvafile() = default;
	hvafile(const std::string& filename);
	hvafile(const byte* buffer);

	void clear();
	bool load(const std::string& filename);
	bool load(const byte* buffer);
	bool is_loaded();
	size_t frame_count();
	size_t section_count();
	game_matrix& matrix(size_t frame, size_t section);

private:
	char _signature[0x10];
	size_t _framecount;
	size_t _sectioncount;
	std::vector<game_matrix> _frame_matrices;
};

#pragma pack(1)
struct vxl_file_header
{
	char signature[0x10];
	uint32_t _reserved;
	uint32_t limb_count;
	uint32_t _limb_count;
	uint32_t body_size;
	uint8_t remap_start_index;
	uint8_t remap_end_index;
	color internal_palette[0x100];
};
#pragma pack()

struct voxel
{
	byte color = 0;
	byte normal = 0;
};

struct vxl_limb_header
{
	char name[16];
	int32_t limb_number;
	uint32_t _reserved1;
	uint32_t _reserved2;
};

struct vxl_limb_tailer
{
	uint32_t span_start_offset;
	uint32_t span_end_offset;
	uint32_t span_data_offset;
	float32_t scale;
	game_matrix matrix;
	vector3<float> min_bounds;
	vector3<float> max_bounds;
	uint8_t xsize;
	uint8_t ysize;
	uint8_t zsize;
	normal_type normal_type;
};

struct span_data
{
	uint8_t voxels_size = 0;
	std::vector<voxel> voxels;
};

struct vxl_limb
{
	std::vector<uint32_t> span_starts;
	std::vector<uint32_t> span_ends;
	std::vector<span_data> span_data_blocks;
};

struct vxl_vertex
{
	d3dvector position;
	voxel voxel;
};

template<typename cache_type>
struct vxl_cache
{
	cache_type cache;
	rect frame_bound;
};

class vxlfile
{
public:
	static constexpr size_t buffer_width = 256u;
	static constexpr size_t buffer_height = 256u;
	static size_t direction_count; // modify this constant to generate all facings
	static const d3dvector reversed_light;

	using cache_type = byte[buffer_height][buffer_width];
	using zbuffer_type = float32_t[buffer_height][buffer_width];
	using cache_ptr = byte[][buffer_width];
	using zbuffer_ptr = float32_t[][buffer_width];
	using cache_pointer = std::shared_ptr<cache_ptr>;
	using zbuffer_pointer = std::shared_ptr<zbuffer_ptr>;
	using cache_frame = std::shared_ptr<byte[]>;
	using cache_storage = vxl_cache<cache_frame>;
	using cache_map_type = std::vector<cache_storage>;
	using vertex_cache_type = std::vector<vxl_vertex>;
	using vertex_cache_vector = std::vector<vertex_cache_type>;

	vxlfile() = default;
	~vxlfile() = default;

	void clear();
	bool is_loaded();
	bool load(const std::string& filename);
	bool load(const byte* buffer);
	bool load_hva(const std::string& filename);
	bool load_hva(const byte* buffer);
	
	size_t limb_count();
	size_t frame_count();
	size_t section_count();
	voxel voxel_lh(size_t limb, uint32_t x, uint32_t y, uint32_t z);
	voxel voxel_rh(size_t limb, uint32_t x, uint32_t y, uint32_t z);

	bool prepare_single_dir_cache(const size_t diridx, class vplfile& vplfile, const int F, const int L, const int H);
	bool prepare_all_cache(class vplfile& vplfile, const int F, const int L, const int H);
	cache_storage frame_cache(const size_t direction);
	cache_storage shadow_cache(const size_t direction);

	//check function
	void print_info();

private:
	bool prepare_vertecies(const size_t limb);
	vertex_cache_type& vertecies(const size_t limb);

	vxl_file_header _fileheader;
	std::vector<vxl_limb> _body_data;
	std::vector<vxl_limb_header> _headers;
	std::vector<vxl_limb_tailer> _tailers;
	std::unique_ptr<hvafile> _associated_hva;

	vertex_cache_vector _prepared_vertex;
	cache_map_type _cache;
	cache_map_type _shadow_cache;
};

CLASSES_END
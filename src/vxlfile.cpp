#include "vxlfile.h"
#include "normal_table.h"
#include "vplfile.h"

CLASSES_START

const d3dvector vxlfile::reversed_light = { 0.0f,1.0f,0.0f };

hvafile::hvafile() :_signature(),
	_framecount(0),
	_sectioncount(0),
	_frame_matrices()
{}

hvafile::hvafile(const std::string& filename) :hvafile()
{
	load(filename);
}

hvafile::hvafile(const byte* buffer) : hvafile()
{
	load(buffer);
}

void hvafile::clear()
{
	_frame_matrices.clear();
	_framecount = _sectioncount = 0;
}

bool hvafile::load(const std::string& filename)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (!file)
		return false;

	std::vector<byte> file_buffer;
	size_t filesize = static_cast<size_t>(file.seekg(0, std::ios::end).tellg());

	file.seekg(0, std::ios::beg);
	file_buffer.resize(filesize);
	file.read(reinterpret_cast<char*>(file_buffer.data()), filesize);

	return load(file_buffer.data());
}

bool hvafile::load(const byte* buffer)
{
	if (!buffer)
		return false;

	clear();

	byte* bytes = const_cast<byte*>(buffer);
	size_t current_offset = 0;
	
	memcpy(_signature, bytes, sizeof(_signature));
	bytes += sizeof(_signature);
	memcpy(&_framecount, bytes, sizeof(_framecount));
	bytes += sizeof(_framecount);
	memcpy(&_sectioncount, bytes, sizeof(_sectioncount));
	bytes += sizeof(_sectioncount);

	_frame_matrices.resize(frame_count() * section_count());
	memcpy(_frame_matrices.data(), bytes + section_count() * sizeof(_signature), _frame_matrices.size() * sizeof(game_matrix));

	return true;
}

bool hvafile::is_loaded()
{
	return !_frame_matrices.empty();
}

size_t hvafile::frame_count()
{
	return _framecount;
}

size_t hvafile::section_count()
{
	return _sectioncount;
}

game_matrix& hvafile::matrix(size_t frame, size_t section)
{
	return _frame_matrices[frame * section_count() + section];
}

void vxlfile::clear()
{
	_body_data.clear();
	_headers.clear();
	_tailers.clear();
	_associated_hva.reset();

	_prepared_vertex.clear();
	_cache.clear();
	_shadow_cache.clear();
}

bool vxlfile::is_loaded()
{
	return !_body_data.empty() && _associated_hva;
}

bool vxlfile::load(const std::string& filename)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (!file)
		return false;

	std::vector<byte> filebuffer;
	size_t filesize = static_cast<size_t>(file.seekg(0, std::ios::end).tellg());
	file.seekg(0, std::ios::beg);

	filebuffer.resize(filesize);
	file.read(reinterpret_cast<char*>(filebuffer.data()), filesize);
	
	return load(filebuffer.data());

}

bool vxlfile::load(const byte* buffer)
{
	if (!buffer)
		return false;

	clear();

	byte* floating_cur = const_cast<byte*>(buffer);

	memcpy(&_fileheader, floating_cur, sizeof(_fileheader));
	for (color& color : _fileheader.internal_palette)
	{
		color.r <<= 2;
		color.g <<= 2;
		color.b <<= 2;
	}

	size_t limb_count = _fileheader.limb_count;
	_body_data.resize(limb_count);
	_headers.resize(limb_count);
	_tailers.resize(limb_count);

	floating_cur += sizeof(_fileheader);
	memcpy(_headers.data(), floating_cur, limb_count * sizeof(vxl_limb_header));

	floating_cur += limb_count * sizeof(vxl_limb_header) + _fileheader.body_size;
	memcpy(_tailers.data(), floating_cur, limb_count * sizeof(vxl_limb_tailer));

	floating_cur -= _fileheader.body_size;
	for (size_t i = 0; i < limb_count; i++)
	{
		vxl_limb_header& current_header = _headers[i];
		vxl_limb_tailer& current_tailer = _tailers[i];
		vxl_limb& current_body = _body_data[i];

		size_t span_count = current_tailer.xsize * current_tailer.ysize;
		current_body.span_data_blocks.resize(span_count);
		current_body.span_ends.resize(span_count);
		current_body.span_starts.resize(span_count);

		byte* span_data_blocks = floating_cur + current_tailer.span_data_offset;
		byte* span_start_data = floating_cur + current_tailer.span_start_offset;
		byte* span_end_data = floating_cur + current_tailer.span_end_offset;

		memcpy(current_body.span_starts.data(), span_start_data, span_count * sizeof(uint32_t));
		memcpy(current_body.span_ends.data(), span_end_data, span_count * sizeof(uint32_t));

		for (size_t n = 0; n < span_count; n++)
		{
			span_data& current_span = current_body.span_data_blocks[n];
			current_span.voxels_size = 0;
			current_span.voxels.resize(current_tailer.zsize);
			
			if (current_body.span_starts[n] == 0xffffffffu || current_body.span_ends[n] == 0xffffffffu)
				continue;

			byte* current_span_data = span_data_blocks + current_body.span_starts[n];
			byte* current_span_end = span_data_blocks + current_body.span_ends[n];

			size_t current_vox_idx = 0;
			byte skip_count, voxel_count, voxel_end;

			do
			{
				skip_count = *current_span_data++;
				voxel_count = *current_span_data++;
				current_vox_idx += skip_count;

				if (voxel_count)
				{
					memcpy(&current_span.voxels[current_vox_idx], current_span_data, voxel_count * sizeof(voxel));
					current_span_data += voxel_count * sizeof(voxel);
				}

				current_vox_idx += voxel_count;
				voxel_end = *current_span_data++;

				if (voxel_count != voxel_end)
				{
					// error report here
				}
			} 
			while (current_span_data <= current_span_end);

			for (size_t z = 0; z < current_tailer.zsize; z++)
			{
				if (current_span.voxels[z].color)
					current_span.voxels_size++;
			}
		}
	}

	return true;

}

bool vxlfile::load_hva(const std::string& filename)
{
	_associated_hva.reset();
	_associated_hva = std::make_unique<hvafile>(filename);
	return _associated_hva->is_loaded();
}

bool vxlfile::load_hva(const byte* buffer)
{
	_associated_hva.reset();
	_associated_hva = std::make_unique<hvafile>(buffer);
	return _associated_hva->is_loaded();
}

bool vxlfile::prepare_vertecies(const size_t limb)
{
	if (!is_loaded() || limb >= limb_count())
		return false;

	if (vertecies(limb).size())
		vertecies(limb).clear();

	vxl_vertex vertex;
	vxl_limb_tailer& first_tailer = _tailers[limb];

	for (uint32_t x = 0; x < first_tailer.xsize; x++)
	{
		for (uint32_t y = 0; y < first_tailer.ysize; y++)
		{
			for (uint32_t z = 0; z < first_tailer.zsize; z++)
			{
				voxel vox = voxel_rh(limb, x, y, z);
				
				if (vox.color)
				{
					vertex.position.x = static_cast<float>(x);
					vertex.position.y = static_cast<float>(y);
					vertex.position.z = static_cast<float>(z);
					vertex.voxel = vox;

					vertecies(limb).push_back(vertex);
				}
			}
		}
	}

	return true;
}

vxlfile::vertex_cache_type& vxlfile::vertecies(const size_t limb)
{
	return _prepared_vertex[limb];
}

bool vxlfile::prepare_single_dir_cache(const size_t diridx, vplfile& vplfile, const int F, const int L, const int H)
{
	if (!is_loaded() || diridx >= direction_count)
		return false;
	
	const rect buffer_view = { 0,0,buffer_width,buffer_height };

	cache_pointer cache(new byte[buffer_height][buffer_width]);
	cache_pointer shadow_cache(new byte[buffer_height][buffer_width]);
	zbuffer_pointer zbuffer(new float_t[buffer_height][buffer_width]);
	d3dmatrix off;
	d3dmatrix rotation;
	float_t rotation_angle = static_cast<float_t>(diridx * D3DX_PI * 2.0f / direction_count);

	D3DXMatrixTranslation(
		&off,
		static_cast<float>(F * 30.0 * D3DX_SQRT2 / 256.0),
		static_cast<float>(- L * 30.0 * D3DX_SQRT2 / 256.0),
		static_cast<float>(H * 30.0 * D3DX_SQRT2 / 256.0)
	);
	D3DXMatrixRotationZ(&rotation, rotation_angle);

	if (!cache || !shadow_cache || !zbuffer)
		return false;

	memset(cache.get(), 0, sizeof(cache_type));
	memset(shadow_cache.get(), 0, sizeof(cache_type));
	for (size_t i = 0; i < buffer_height; i++)
		for (size_t j = 0; j < buffer_width; j++)
			zbuffer[i][j] = std::numeric_limits<float_t>::max();
	
	int32_t xl, xh, yl, yh;
	int32_t sxl, sxh, syl, syh;

	xl = sxl = buffer_width - 1;
	yl = syl = buffer_height - 1;
	xh = yh = sxh = syh = 0;

	if (_prepared_vertex.empty())
	{
		_prepared_vertex.resize(limb_count());
		for (size_t i = 0; i < limb_count(); i++)
			prepare_vertecies(i);
	}

	for (size_t l = 0; l < limb_count(); l++)
	{
		vertex_cache_type vertex_cache = vertecies(l);
		vxl_limb_tailer& current_tailer = _tailers[l];

		if (vertex_cache.empty())
			continue;

		d3dvector scales;
		vector3<float_t> min_bound = current_tailer.min_bounds;
		vector3<float_t> max_bound = current_tailer.max_bounds;

		scales.x = (max_bound.x() - min_bound.x()) / current_tailer.xsize;
		scales.y = (max_bound.y() - min_bound.y()) / current_tailer.ysize;
		scales.z = (max_bound.z() - min_bound.z()) / current_tailer.zsize;

		d3dmatrix origin = current_tailer.matrix.d3d_matrix(current_tailer.scale);
		d3dmatrix transform = _associated_hva->matrix(0, l).integrate_matrix(scales, current_tailer.scale);
		d3dmatrix trans_center = math::translation_from(min_bound);
		d3dmatrix scale;
		D3DXMatrixScaling(&scale, scales.x, scales.y, scales.z);

		d3dmatrix result = trans_center * scale * transform * off * rotation;
		d3dvector* normal_table = normal::normal_table_directory[static_cast<uint8_t>(current_tailer.normal_type)];

		for (vxl_vertex& vertex : vertex_cache)
		{
			d3dvector normal_vec = normal_table[vertex.voxel.normal];// *normal_trans;
			d3dvector position = vertex.position * result;

			D3DXVec3TransformNormal(&normal_vec, &normal_vec, &result);

			d3dvector screen_pos = math::fructum_transformation(buffer_view, position);
			int32_t x = static_cast<int32_t>(screen_pos.x);
			int32_t y = static_cast<int32_t>(screen_pos.y);
			
			position.z = 0.0f;
			d3dvector shadow_pos = math::fructum_transformation(buffer_view, position);
			int32_t sx = static_cast<int32_t>(shadow_pos.x);
			int32_t sy = static_cast<int32_t>(shadow_pos.y);

			float_t light_angle = std::acos((D3DXVec3Dot(&vxlfile::reversed_light, &normal_vec)) / 
				D3DXVec3Length(&vxlfile::reversed_light) / D3DXVec3Length(&normal_vec));

			byte color = 0;
			if (light_angle >= D3DX_PI / 2.0f)
			{
				color = vplfile[0][vertex.voxel.color];
			}
			else
			{
				int32_t index = 31 - int32_t(light_angle / (D3DX_PI / 2.0f) * 32.0f);
				color = vplfile[index][vertex.voxel.color];
			}

			if (vertex.voxel.color && screen_pos.z < zbuffer[y][x] && screen_pos.z >= 0.0f)
			{
				zbuffer[y][x] = screen_pos.z;
				cache[y][x] = color;

				if (xl > x)xl = x;
				if (xh < x)xh = x;
				if (yl > y)yl = y;
				if (yh < y)yh = y;
			}

			shadow_cache[sy][sx] = 1;//shadow color index
			if (sxl > sx)sxl = sx;
			if (sxh < sx)sxh = sx;
			if (syl > sy)syl = sy;
			if (syh < sy)syh = sy;

		}
	}

	size_t width = xh - xl + 1, height = yh - yl + 1;
	size_t swidth = sxh - sxl + 1, sheight = syh - syl + 1;

	cache_frame clip_cache(new byte[width * height]);
	cache_frame clip_shadow(new byte[swidth * sheight]);

	if (!clip_cache || !clip_shadow)
		return false;

	memset(clip_cache.get(), 0, width * height);
	memset(clip_shadow.get(), 0, swidth * sheight);

	if (_cache.empty())
		_cache.resize(direction_count);

	if (_shadow_cache.empty())
		_shadow_cache.resize(direction_count);

	for (int32_t y = yl; y <= yh; y++)
		memcpy(&clip_cache[(y - yl) * width], &cache[y][xl], width);

	for (int32_t sy = syl; sy <= syh; sy++)
		memcpy(&clip_shadow[(sy - syl) * swidth], &shadow_cache[sy][sxl], swidth);

	_cache[diridx].cache = clip_cache;
	_cache[diridx].frame_bound = { xl,yl,xh + 1,yh + 1 };

	_shadow_cache[diridx].cache = clip_shadow;
	_shadow_cache[diridx].frame_bound = { sxl,syl,sxh + 1,syh + 1 };
	
	return true;
}

bool vxlfile::prepare_all_cache(vplfile& vplfile, const int F, const int L, const int H)
{
	bool result = true;
	for (size_t i = 0; i < direction_count; i++)
	{
		result &= prepare_single_dir_cache(i, vplfile, F, L, H);
	}

	return result;
}

vxlfile::cache_storage vxlfile::frame_cache(const size_t direction)
{
	cache_storage result;
	if (direction >= _cache.size())
		return result;
	return _cache[direction];
}

vxlfile::cache_storage vxlfile::shadow_cache(const size_t direction)
{
	cache_storage result;
	if (direction >= _shadow_cache.size())
		return result;
	return _shadow_cache[direction];
}

size_t vxlfile::limb_count()
{
	return is_loaded() ? _fileheader.limb_count : 0u;
}

size_t vxlfile::frame_count()
{
	return is_loaded() ? _associated_hva->frame_count() : 0u;
}

size_t vxlfile::section_count()
{
	return is_loaded() ? _associated_hva->section_count() : 0u;
}

voxel vxlfile::voxel_lh(size_t limb, uint32_t x, uint32_t y, uint32_t z)
{
	return voxel_rh(limb, y, x, z);
}

voxel vxlfile::voxel_rh(size_t limb, uint32_t x, uint32_t y, uint32_t z)
{
	voxel result;
	if (!is_loaded() || limb >= limb_count())
		return result;

	vxl_limb_tailer& tailer = _tailers[limb];
	vxl_limb& body = _body_data[limb];

	if (x >= tailer.xsize || y >= tailer.ysize || z >= tailer.zsize)
		return result;

	return body.span_data_blocks[y * tailer.xsize + x].voxels[z];
}

void vxlfile::print_info()
{
#if 0
	if (!is_loaded())
		std::cout << "Current vxl is not loaded.\n";

	std::cout << "Number of limbs : " << limb_count() << ".\n";
	for (size_t i = 0; i < limb_count(); i++)
	{
		vxl_limb_tailer& current_tailer = _tailers[i];
		vxl_limb& current_body = _body_data[i];

		std::cout << std::hex
			<< "Span start offset = " << current_tailer.span_start_offset << ".\n"
			<< "End offset = " << current_tailer.span_end_offset << ".\n"
			<< "Data offset = " << current_tailer.span_data_offset << ".\n"
			<< std::dec;

		std::cout << "Vxl dimensiton = " << (size_t)current_tailer.xsize << ", " << (size_t)current_tailer.ysize << ", " << (size_t)current_tailer.zsize << ".\n";
		for (size_t x = 0; x < current_tailer.xsize; x++)
		{
			for (size_t y = 0; y < current_tailer.ysize; y++)
			{
				span_data& span = current_body.span_data_blocks[y * current_tailer.xsize + x];
				std::cout << "Span coords = " << x << ", " << y << ", voxel count = " << (uint32_t)span.voxels_size << ".\n";

				for (auto& voxel : span.voxels)
				{
					std::cout << "(" << (int)voxel.color << ", " << (int)voxel.normal << "), ";
				}
				std::cout << std::endl;
			}
		}
		std::cout << "About to print info for next limb:\nPress any key to continue.\n";
		//std::cin.get();
	}
#endif
}

CLASSES_END

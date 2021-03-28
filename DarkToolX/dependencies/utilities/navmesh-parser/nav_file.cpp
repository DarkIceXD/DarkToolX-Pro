#include "nav_file.h"
#include <iostream>

namespace nav_mesh {
	nav_file::nav_file(std::string_view nav_mesh_file)
	{
		load(nav_mesh_file);
	}

	bool nav_file::load(std::string_view nav_mesh_file)
	{
		if (!m_pather)
			m_pather = std::make_unique< micropather::MicroPather >(this);

		m_pather->Reset();
		m_areas.clear();
		m_places.clear();

		m_buffer.load_from_file(nav_mesh_file);
		if (m_buffer.empty())
			return false;

		constexpr uint32_t magic = 0xFEEDFACE;
		if (m_buffer.read< std::uint32_t >() != magic)
			return false;

		constexpr uint32_t version = 16;
		if (m_buffer.read< std::uint32_t >() != version)
			return false;

		const auto sub_version = m_buffer.read< std::uint32_t >();
		const auto source_bsp_size = m_buffer.read< std::uint32_t >();
		const auto is_analyzed = m_buffer.read< std::uint8_t >();
		const auto place_count = m_buffer.read< std::uint16_t >();

		for (std::uint16_t i = 0; i < place_count; i++) {
			const auto place_name_length = m_buffer.read< std::uint16_t >();
			std::string place_name(place_name_length, 0);

			m_buffer.read(place_name.data(), place_name_length);
			m_places.push_back(place_name);
		}

		const auto has_unnamed_areas = m_buffer.read< std::uint8_t >() != 0;
		const auto area_count = m_buffer.read< std::uint32_t >();

		for (std::uint32_t i = 0; i < area_count; i++) {
			nav_area area(m_buffer);
			m_areas.push_back(area);
		}

		m_buffer.clear();
		return true;
	}

	std::vector< vec3_t > nav_file::find_path(const vec3_t& from, const vec3_t& to)
	{
		const auto start = get_area_by_position(from);
		const auto end = get_area_by_position(to);
		if (!start || !end)
			return {};
		float total_cost = 0.f;
		micropather::MPVector< void* > path_area_ids = { };

		std::vector< vec3_t > path = { };
		if (m_pather->Solve(reinterpret_cast<void*>(start->get_id()), reinterpret_cast<void*>(end->get_id()), &path_area_ids, &total_cost) != 0)
			return path;

		for (std::size_t i = 0; i < path_area_ids.size(); i++) {
			for (auto& area : m_areas) {
				if (area.get_id() == std::uint32_t(path_area_ids[i])) {
					path.push_back(area.get_center());
					break;
				}
			}
		}

		return path;
	}

	nav_area* nav_file::get_area_by_id(const std::uint32_t id)
	{
		for (auto& area : m_areas) {
			if (area.get_id() == id)
				return &area;
		}

		return nullptr;
	}

	nav_area* nav_file::get_area_by_position(const vec3_t& position)
	{
		for (auto& area : m_areas) {
			if (area.is_within(position))
				return &area;
		}

		return nullptr;
	}
}
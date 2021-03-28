#pragma once
#include "nav_area.h"
#include "micropather.h"

namespace nav_mesh {
	class nav_file : public micropather::Graph {
	public:
		nav_file() { }
		nav_file(std::string_view nav_mesh_file);

		bool load(std::string_view nav_mesh_file);

		std::vector< vec3_t > find_path(const vec3_t& from, const vec3_t& to);

		//MicroPather implementation
		virtual float LeastCostEstimate(void* start, void* end) {
			const auto start_area = get_area_by_id(std::uint32_t(start));
			const auto end_area = get_area_by_id(std::uint32_t(end));
			if (!start_area || !end_area)
				return FLT_MAX;

			const auto distance = start_area->get_center() - end_area->get_center();

			return distance.length();
		}

		virtual void AdjacentCost(void* state, micropather::MPVector< micropather::StateCost >* adjacent) {
			const auto area = get_area_by_id(std::uint32_t(state));
			if (!area)
				return;

			for (const auto& connection : area->get_connections()) {
				const auto connection_area = get_area_by_id(connection.id);
				if (!connection_area)
					continue;

				const auto distance = connection_area->get_center() - area->get_center();
				adjacent->push_back({ reinterpret_cast<void*>(connection_area->get_id()), distance.length() });
			}
		}

		virtual void PrintStateInfo(void* state) { }

	private:
		nav_area* get_area_by_id(const std::uint32_t id);
		nav_area* get_area_by_position(const vec3_t& position);
		std::unique_ptr< micropather::MicroPather > m_pather = nullptr;
		nav_buffer m_buffer = { };
		std::vector< nav_area > m_areas = { };
		std::vector< std::string > m_places = { };
	};
}
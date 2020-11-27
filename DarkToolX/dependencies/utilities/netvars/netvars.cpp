#include "netvars.hpp"
#include "../../interfaces/interfaces.hpp"
#include <unordered_map>

//antario if i remember correctly
namespace netvar_manager {
	using netvar_key_value_map = std::unordered_map< uint32_t, netvar_data >;
	using netvar_table_map = std::unordered_map< uint32_t, netvar_key_value_map >;
	void initialize_props(netvar_table_map& table_map);

	netvar_data* get_net_var_data(const uint32_t table, const uint32_t prop) {
		static netvar_table_map map = {};
		if (map.empty())
			initialize_props(map);

		if (map.find(table) == map.end())
			return nullptr;

		netvar_key_value_map& table_map = map.at(table);
		if (table_map.find(prop) == table_map.end())
			return nullptr;

		return &table_map.at(prop);
	}

	uintptr_t get_net_var(const uint32_t table, const uint32_t prop)
	{
		const auto* data = get_net_var_data(table, prop);
		if (!data)
			return 0;

		return data->offset;
	}

	recv_prop* get_prop(const uint32_t table, const uint32_t prop)
	{
		const auto* data = get_net_var_data(table, prop);
		if (!data)
			return nullptr;

		return data->prop;
	}

	void add_props_for_table(netvar_table_map& table_map, const uint32_t table_name_hash, const std::string& table_name, recv_table* table, const size_t child_offset = 0) {
		for (auto i = 0; i < table->props_count; ++i) {
			auto& prop = table->props[i];

			if (isdigit(prop.prop_name[0]))
				continue;

			if (prop.data_table && prop.elements_count > 0)
				add_props_for_table(table_map, table_name_hash, table_name, prop.data_table, prop.offset + child_offset);

			if (prop.prop_name[0] != 'm' /*&& name.substr( 0, 1 ) != "b"*/)
				continue;

			const auto offset = uintptr_t(prop.offset) + child_offset;
			table_map[table_name_hash][fnv::hash(prop.prop_name)] = { offset, &prop };
		}
	}

	void initialize_props(netvar_table_map& table_map) {
		for (auto client_class = interfaces::client->get_client_classes();
			client_class;
			client_class = client_class->next_ptr) {
			const auto table = reinterpret_cast<recv_table*>(client_class->recvtable_ptr);
			const auto table_name = table->table_name;
			const auto table_name_hash = fnv::hash(table_name);
			if (table == nullptr)
				continue;
			
			add_props_for_table(table_map, table_name_hash, table_name, table);
		}
	}
}
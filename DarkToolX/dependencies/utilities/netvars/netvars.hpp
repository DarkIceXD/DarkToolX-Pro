#pragma once
#include "../../../source-sdk/classes/recv_props.hpp"
#include "../../../source-sdk/classes/client_class.hpp"
#include "../../utilities/fnv.hpp"

namespace netvar_manager {
	struct netvar_data {
		uintptr_t offset;
		recv_prop* prop;
	};
	uintptr_t get_net_var(const uint32_t table, const uint32_t prop);
	recv_prop* get_prop(const uint32_t table, const uint32_t prop);
}

#define NETVAR_OFFSET( table, prop, off, func_name, type ) std::add_lvalue_reference_t<type> func_name() { \
    static uintptr_t offset = 0; \
    if( !offset ) { \
        offset = netvar_manager::get_net_var( fnv::hash( table ), fnv::hash( prop ) ) + off; \
    } \
    return *reinterpret_cast< std::add_pointer_t<type> >( uintptr_t( this ) + offset); \
}

#define NETVAR( table, prop, func_name, type ) NETVAR_OFFSET(table, prop, 0, func_name, type)

#define NETVAR_PTR( table, prop, func_name, type ) type* func_name() { \
    static uintptr_t offset = 0; \
    if( !offset ) { \
        offset = netvar_manager::get_net_var( fnv::hash( table ), fnv::hash( prop ) ); \
    } \
    return reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
}

#define NETPROP(table, prop, func_name) \
static recv_prop* func_name() \
{ \
	static recv_prop* prop_ptr = nullptr; \
	if(!prop_ptr) prop_ptr = netvar_manager::get_prop( fnv::hash( table ), fnv::hash( prop ) ); \
	return prop_ptr; \
}

#define OFFSET( type, var, offset ) type& var() { \
    return *( type* )( uintptr_t( this ) + offset ); \
}

#pragma once
#include <cstdint>

namespace utilities {
	template< typename t >
	__forceinline static t call_virtual( void *name, int offset ) {
		int *table = *( int ** ) name;
		int address = table[ offset ];
		return ( t ) ( address );
	}

	uint8_t* pattern_scan(const char* module_name, const char* signature) noexcept;
	void* get_export(const char* module_name, const char* export_name);
	
}

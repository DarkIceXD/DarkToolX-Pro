#include "nav_buffer.h"
#include <iterator>

namespace nav_mesh {
	nav_buffer::~nav_buffer()
	{
		clear();
	}

	void nav_buffer::load_from_file(const std::string_view nav_mesh_file)
	{
		clear();

		std::ifstream nav_file(nav_mesh_file.data(), std::istream::binary);

		if (!nav_file.is_open())
			return;

		nav_file.unsetf(std::istream::skipws);

		nav_file.seekg(0, std::istream::end);
		const auto file_size = nav_file.tellg();
		nav_file.seekg(0, std::istream::beg);

		m_nav_buffer.reserve(file_size);
		m_nav_buffer.insert(m_nav_buffer.begin(), std::istream_iterator< std::uint8_t >(nav_file), std::istream_iterator< std::uint8_t >());
	}

	void nav_buffer::skip(const std::size_t bytes_to_skip)
	{
		m_bytes_read += bytes_to_skip;
	}

	void nav_buffer::clear()
	{
		m_bytes_read = 0;
		m_nav_buffer.clear();
	}

	bool nav_buffer::empty()
	{
		return m_nav_buffer.empty();
	}
}
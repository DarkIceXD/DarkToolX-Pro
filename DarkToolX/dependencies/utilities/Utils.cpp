#include "Utils.h"
#include <Windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

uint64_t Util::hash(const char* str)
{
	uint64_t hash = 5381;
	for (; *str != '\0'; ++str)
		hash = ((hash << 5) + hash) + (*str);
	return hash;
}

std::string Util::download_file(const char* url)
{
	constexpr auto buf_size = 1 << 10;
	std::string str;
	const auto connection = InternetOpenA(nullptr, INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
	if (connection)
	{
		const auto address = InternetOpenUrlA(connection, url, nullptr, 0, INTERNET_FLAG_RELOAD, 0);
		if (address)
		{
			char data_received[buf_size];
			DWORD number_of_bytes_received = 0;
			do
			{
				if (InternetReadFile(address, data_received, buf_size, &number_of_bytes_received))
				{
					str.append(data_received, number_of_bytes_received);
				}
				else
				{
					break;
				}
			} while (number_of_bytes_received > 0);
			InternetCloseHandle(address);
		}
		InternetCloseHandle(connection);
	}
	return str;
}

void Util::next(int& current_index, const int size)
{
	current_index = current_index < size - 1 ? current_index + 1 : 0;
}

void Util::prev(int& current_index, const int size)
{
	current_index = current_index > 0 ? current_index - 1 : size - 1;
}

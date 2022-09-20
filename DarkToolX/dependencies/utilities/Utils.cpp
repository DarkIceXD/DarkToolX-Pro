#include "utils.h"
#include <Windows.h>
#include <winhttp.h>
#pragma comment(lib, "Winhttp.lib")

static wchar_t* str_to_wstr(const char* str)
{
	wchar_t* wstr;
	int n = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	wstr = new wchar_t[n];
	MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)wstr, n);
	return wstr;
}

std::vector<char> util::download(const char* server, const char* dir)
{
	std::vector<char> buffer;
	const auto hSession = WinHttpOpen(nullptr, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (hSession)
	{
		const auto w_server = str_to_wstr(server);
		const auto hConnect = WinHttpConnect(hSession, w_server, INTERNET_DEFAULT_HTTPS_PORT, 0);
		delete[] w_server;
		if (hConnect)
		{
			const auto w_dir = str_to_wstr(dir);
			const auto hRequest = WinHttpOpenRequest(hConnect, nullptr, w_dir, nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
			delete[] w_dir;
			if (hRequest)
			{
				auto bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
				if (bResults)
					bResults = WinHttpReceiveResponse(hRequest, nullptr);
				if (bResults)
				{
					while (true)
					{
						DWORD size = 0;
						if (!WinHttpQueryDataAvailable(hRequest, &size))
							break;

						if (!size)
							break;

						auto temp_buff = new char[size + 1];
						if (!temp_buff)
							break;

						DWORD downloaded = 0;
						if (WinHttpReadData(hRequest, (LPVOID)temp_buff, size, &downloaded))
							buffer.insert(buffer.end(), temp_buff, temp_buff + downloaded);
						delete[] temp_buff;
						if (!downloaded)
							break;
					}
				}
				WinHttpCloseHandle(hRequest);
			}
			WinHttpCloseHandle(hConnect);
		}
		WinHttpCloseHandle(hSession);
	}
	return buffer;
}

// #define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h" // https://github.com/yhirose/cpp-httplib

#include <iostream>
#include <tuple>
#include <cstring>
#include <vector>
#include <string>
#include <optional>
#include <time.h>

std::optional<std::tuple<std::string, std::string>> getYahooAuthInfo()
{
	httplib::Client yf_client("uk.finance.yahoo.com");
	yf_client.set_connection_timeout(5);
	yf_client.set_follow_location(true); // support redirect to avoid 301

	auto res = yf_client.Get("/quote/MSFT/history");

	if (res && res->status == 200) {
		auto header = res->headers.find("Set-Cookie");
		if (header != res->headers.end()) {
			auto pos = header->second.find(";");
			if (pos > 0) {
				auto cookie = header->second.substr(0, pos);
				auto keyword = "CrumbStore\":{\"crumb\":\"";
				auto keywordLen = strlen(keyword);
				auto body = res->body;
				auto start = body.find(keyword) + keywordLen;
				auto end = body.find("\"}", start);
				auto crumb = body.substr(start, end - start);
				return std::tuple{ cookie, crumb };
			}
		}
	}

	return {};
}

std::optional<std::string> fetchData(const std::string_view symbol) {
	auto authInfo = getYahooAuthInfo();
	if (authInfo) {
		auto [cookie, crumb] = authInfo.value();
		time_t now = time(nullptr);
		time_t then = now - 180 * 24 * 60 * 60;

		httplib::Client yf_client("query1.finance.yahoo.com");
		yf_client.set_connection_timeout(5);
		yf_client.set_follow_location(true);

		std::string path{ "/v7/finance/download/" };

		path.append(symbol).append("?")
			.append("period1=").append(std::to_string(then))
			.append("&period2=").append(std::to_string(now))
			.append("&interval=1d")
			.append("&events=history")
			.append("&crumb=" + crumb);

		httplib::Headers headers{
			{ "Cookie", cookie },
			{ "User-Agent", "Chrome/69.0.3497.100 Safari/537.36" }
		};

		auto res = yf_client.Get(path.c_str(), headers);

		if (res && res->status == 200) {
			return res->body;
		}
	}

	return {};
}

void startServer()
{
	httplib::Server srv;

	srv.Get("/hi",
		[](const httplib::Request& req, httplib::Response& res) {
			res.set_content("Hello World!", "text/plain");
		});

	srv.Get(R"(/numbers/(\d+))",
		[](const httplib::Request& req, httplib::Response& res) {
			auto numbers = req.matches[1];
			res.set_content(numbers, "text/plain");
		});

	srv.Get("/stop",
		[&](const httplib::Request& req, httplib::Response& res) {
			srv.stop();
		});

	srv.Get("/crumb",
		[](const httplib::Request& req, httplib::Response& res) {
			auto authInfo = getYahooAuthInfo();
			if (authInfo) {
				auto [cookie, crumb] = authInfo.value();
				res.set_content(cookie + "\n" + crumb, "text/plain");
			}
			else {
				res.set_content("Request failed.", "text/plain");
			}
		});

	srv.Get("/symbol",
		[](const httplib::Request& req, httplib::Response& res) {
			auto ohlc = fetchData("AAPL");
			if (ohlc) {
				res.set_content(ohlc.value(), "text/plain");
			}
			else {
				res.set_content("Request failed.", "text/plain");
			}
		});

	srv.listen("localhost", 1234);
}

#if defined(WIN32)
void printLastError()
{
	auto lastErrorCode = GetLastError();
	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, lastErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);
	LocalFree(messageBuffer);
	std::cout << "Last error (" << lastErrorCode << "): " << message << std::endl;
}
#endif

int main()
{
	auto str = fetchData("AAPL");
	if (str.has_value()) {
		std::cout << str.value() << std::endl;
	}
	//startServer();

#if defined(WIN32)
	printLastError();
#endif

	return 0;
}

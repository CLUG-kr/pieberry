#include "stdafx.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>                        // URI library
#include <cpprest/ws_client.h>                    // WebSocket client
#include <cpprest/containerstream.h>            // Async streams backed by STL containers
#include <cpprest/interopstream.h>              // Bridges for integrating Async streams with STL and WinRT streams
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory
#include <cpprest/producerconsumerstream.h>     // Async streams for producer consumer scenarios
#include <sstream>
#include <iostream>
#include <codecvt>

//using namespace utility;                    // Common utilities like string conversions
//using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

//using namespace web::http::experimental::listener;          // HTTP server
using namespace web::web_sockets::client;                    // WebSockets client
using namespace web::json;                                  // JSON library

const wchar_t* g_token = U("xoxb-16265808753-vUp4yDjNp8lFp6n5StLB1cau");
const std::wstring g_version(U("*[pieberry v0.1]*"));

pplx::task<void> async_rtm_start(value&);
void beginWebsocket(value&, websocket_client&);
void speak(value&, websocket_client&, std::wstring);
void endWebsocket(value&, websocket_client&);

int main(int argc, char* argv[])
{
	value slack = value::object();
	websocket_client wsclient;

	try
	{
		async_rtm_start(slack).wait();
		beginWebsocket(slack, wsclient);

		setlocale(LC_ALL, "");
		bool chat = false;

		speak(slack, wsclient, g_version + L" 시작합니다...");

		while (1) {
			std::wstring wstr;
			if (chat)
				std::wcout << U("*> ");
			else
				std::wcout << U("=>");

			std::getline(std::wcin, wstr);

			if (wstr == L"!exit" || wstr == L"exit" && !chat)
				break;
			if (chat) {
				if (wstr == L"!console") {
					chat = false;
					continue;
				}
				speak(slack, wsclient, wstr);
				continue;
			}
			if (wstr == L"chat") {
				chat = true;
				continue;
			}
			
		}

		speak(slack, wsclient, g_version + L" 종료합니다...");
		endWebsocket(slack, wsclient);
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}
	
	return 0;
}

pplx::task<void> async_rtm_start(value& sav) {
	http_client client(U("https://slack.com/api/"));

	// Build request URI and start the request.
	uri_builder builder(U("/rtm.start"));
	builder.append_query(U("token"), g_token);

	return client.request(methods::GET, builder.to_string()).then([](http_response response) -> pplx::task<value>
	{
		if (response.status_code() == status_codes::OK)
		{
			return response.extract_json();
		}

		// Handle error cases, for now return empty json value...
		return pplx::task_from_result(value());
	})
		.then([&sav](pplx::task<value> previousTask)
	{
		try
		{
			const value& v = previousTask.get();
			// Perform actions here to process the JSON value...
			sav = v;
		}
		catch (const http_exception& e)
		{
			// Print error.
			std::wostringstream ss;
			ss << e.what() << std::endl;
			std::wcout << ss.str();
		}
	});
}

void beginWebsocket(value& teamdata, websocket_client& wscl) {
	auto url = teamdata.at(U("url")).as_string();

	wscl.connect(url).then([&url]() {	/* We've finished connecting. */	}).wait();
}

void speak(value& teamdata, websocket_client& wscl, std::wstring text) {

	value tospeak;
	tospeak[L"type"] = value::string(U("message"));
	tospeak[L"channel"] = value::string(U("C0GDE81EZ"));
	tospeak[L"id"] = value::number(1);
	tospeak[L"text"] = value::string(text);

	websocket_outgoing_message msg;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string outstr(converter.to_bytes(tospeak.serialize()));

	msg.set_utf8_message(outstr);

	wscl.send(msg).then([]() {});
}

void endWebsocket(value& teamdata, websocket_client& wscl) {
	wscl.close().then([]() { /* Successfully closed the connection. */	});
}
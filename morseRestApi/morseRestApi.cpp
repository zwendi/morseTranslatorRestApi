// morseRestApi.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "morseTranslator.h"
#include "requestHandler.h"


using namespace std;
using namespace web::http::experimental::listener;

//use string instead wstring here enable us to use remove method from standard
const string stopFileName = ".\\stop.txt";
const wstring defaultPort = L"8080";

int _tmain(int argc, _TCHAR* argv[])
{
	//For this example, a simple console is used to to get the port number
	wcout << L"Please enter the port number you want the program listen on locally.";
	wstring address = L"http://localhost/";
	wstring port;
	wcin >> port;
	if (port.size() < 1)
		port = defaultPort;     // default port number

	http_listener listener(address+port);
	requestHandler handler;
	listener.support(methods::POST, [&](http_request req){
		handler.handle_post(req);
	});
	listener.support(methods::OPTIONS, [&](http_request req){
		//This is for the browser to send the options request if we want to build a local mock up
		handler.handle_options(req);
	});
	bool stop = false;

	while (!stop) {
		listener.open().wait();
		if (ifstream(stopFileName))
		{
			stop = true;
			remove(stopFileName.c_str());
		}
	}

	std::wcout << L"Bye Bye." << endl;
	listener.close();

	return 0;
}


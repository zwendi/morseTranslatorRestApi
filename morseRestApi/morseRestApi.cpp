// morseRestApi.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "morseTranslator.h"
#include "requestHandler.h"

using namespace std;
using namespace web::http::experimental::listener;


int _tmain(int argc, _TCHAR* argv[])
{
	
	http_listener listener(L"http://localhost/demo");
	requestHandler handler;
	listener.support(methods::POST, [&](http_request req){
		handler.handle_post(req);
	});
	listener.support(methods::OPTIONS, [&](http_request req){
		handler.handle_options(req);
	});

	while (true) {
		listener.open().wait();
	}

	std::cout << U("Bye Bye.") << endl;
	listener.close();

	/*
	wstring  test = L"SOS HELP ME";
	wstring errStr = L"";

	wstring morseCode = translator.encodeEnglish(test, errStr);
	wcout << morseCode<<endl;


	test = L"";
	errStr = L"";

	 morseCode = translator.encodeEnglish(test, errStr);

	wcout << morseCode;
	test = L"... --- ... ";
	morseCode = translator.decodeMorse(test, errStr);

	*/
	/*
	*  question: Do we need to consider white space between words?  If so how should we represent in morse code? Using 3 spaces?
	*/
	return 0;
}


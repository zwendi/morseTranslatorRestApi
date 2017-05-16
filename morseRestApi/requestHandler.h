#pragma once
//framework to help to handle http request.
#include <cpprest/http_listener.h> 
using namespace std;
using namespace web::http;


//uri for our rest api post request
const wstring tranEnglish = L"eng-to-morse";
const wstring tranMorse = L"morse-to-eng";


class requestHandler
{
public:
   requestHandler();
   virtual ~requestHandler();

   //allow future inheritence for different implementation or 
   virtual void handle_post(http_request& request);  

	//handle options request sent from browser;
   virtual void handle_options(http_request& request);


private:
	class privateData;
	privateData* pData;
};


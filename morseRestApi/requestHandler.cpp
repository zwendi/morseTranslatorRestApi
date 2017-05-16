#include "stdafx.h"
#include "requestHandler.h"
#include "morseTranslator.h"
#include <iterator>
#include <vector>

//define as constant here allow easier maintenance and changes
const wstring error = L"Error";
const wstring answer = L"Answer";
const int postUriSize = 2;
const int resPosition = 1;

using namespace web;
using namespace web::http;
using namespace std;

class requestHandler::privateData{

public:
	void handleJsonResponse(wstring errMsg, json::value& jsonObject, http_request& request);


	//following support method is from stack overflow.
	template<typename Out>
	void split(const std::wstring &s, wchar_t delim, Out result) {
		wstringstream ss;
		ss.str(s);
		wstring item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::vector<std::wstring> split(const wstring &s, wchar_t delim) {
		std::vector<std::wstring> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}


	//to process uri to a vector<string> so we can use in all types requests
	vector<wstring> processUri(uri& uri){
		auto resources = uri.resource().to_string();
		removeAdditionalCharacter(resources);
		transform(resources.begin(), resources.end(), resources.begin(), ::tolower);
		return split(resources, L'/');
	}

	/*rest c++ parse json with some additional characters during testing using postman
	* need to clean the string before use it.
	*/
	void removeAdditionalCharacter(wstring& str)
	{
		size_t s = str.size();
		wchar_t chars[] = L"\"";
		for (unsigned int i = 0; i < s; ++i)
		{
			str.erase(std::remove(str.begin(), str.end(), chars[i]), str.end());
		}
	}

};

requestHandler::requestHandler()
{
	pData = new privateData;
}


requestHandler::~requestHandler()
{
	if (pData)
	{
		delete pData;
		pData = NULL;
	}
}


void requestHandler::handle_post(http_request& request)
{
	auto uri = request.relative_uri();
	json::value resVal;
	if (uri.is_empty())
	{
		pData->handleJsonResponse(L"No uri received.", resVal, request);
		return;
	}

	auto resources = pData->processUri(uri);
	if (resources.size() != postUriSize)
	{
		pData->handleJsonResponse(L"Too many or too few resources are supply.", resVal, request);
		return;
	}
	
	wstring resource = resources[resPosition];
	auto body = request.extract_string();
	wstring inputString = body.get().c_str();
	auto& translator = morseTranslator::getMorseTranslator();
	wstring result = L"";
	wstring errMsg = L"";
	if (resource.compare(tranEnglish) == 0)
	{
		result = translator.encodeEnglish(inputString, errMsg);
	}
	else if (resource.compare(tranMorse) == 0)
	{
		result = translator.decodeMorse(inputString, errMsg);
	}
	else{
		errMsg = L"Unrecognized post request:"+resource;
	}
	
	resVal[answer] = json::value(result);
	pData->handleJsonResponse(errMsg, resVal, request);
	return;

	
}



void requestHandler::handle_options(http_request& request)
{
	http_response response(status_codes::OK);
	response.headers().add(U("Allow"), U("POST, OPTIONS"));
	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	response.headers().add(U("Access-Control-Allow-Methods"), U("POST, OPTIONS"));
	response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
	request.reply(response);
}


void requestHandler::privateData::handleJsonResponse(wstring errMsg, json::value& jsonObject, http_request& request)
{
	json::value resVal;
	http_response resp;
	resp.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	if (errMsg.size()>0)
	{
		resVal[error] = json::value(errMsg);
		resp.set_status_code(web::http::status_codes::BadRequest);
		resp.set_body(resVal);
	}
	else
	{
		resp.set_status_code(web::http::status_codes::OK);
		resp.set_body(jsonObject);
	}
	request.reply(resp);
}

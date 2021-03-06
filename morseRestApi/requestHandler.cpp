#include "stdafx.h"
#include "requestHandler.h"
#include "morseTranslator.h"
#include <vector>

//define as constant here allow easier maintenance and changes
const wstring error = L"Error";
const wstring answer = L"Answer";
const int postUriSize = 2;
const int resPosition = 1;

using namespace web;

class requestHandler::privateData{

public:
	void handleJsonResponse(wstring errMsg, json::value& jsonObject, http_request& request);
	//to process uri to a vector<string> so we can use in all types requests
	vector<wstring> processUri(uri& uri);
	
	//following support methods is from stack overflow.
	template<typename Out>
	void split(const std::wstring &s, wchar_t delim, Out result);
	
	std::vector<std::wstring> split(const wstring &s, wchar_t delim);
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
	//if uri contains more or less resource that postUriSize
	if (resources.size() != postUriSize)
	{
		pData->handleJsonResponse(L"Too many or too few resources are supplied.", resVal, request);
		return;
	}
	wstring resource = resources[resPosition];
	wstring inputString = request.extract_string().get().c_str();
	auto& translator = morseTranslator::getMorseTranslator();
	wstring result = L"";
	wstring errMsg = L"";

	if (resource.compare(tranEnglish) == 0)
	{
		result = translator.encodeEnglish(inputString);
	}
	else if (resource.compare(tranMorse) == 0)
	{
		result = translator.decodeMorse(inputString);
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
	http_response resp;
	resp.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	//some error happened, should sent back the error message
	if (errMsg.size()>0)
	{
		json::value resVal;
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


template<typename Out>
void requestHandler::privateData::split(const std::wstring &s, wchar_t delim, Out result) {
	wstringstream ss;
	ss.str(s);
	wstring item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::wstring> requestHandler::privateData::split(const wstring &s, wchar_t delim) {
	std::vector<std::wstring> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

vector<wstring> requestHandler::privateData::processUri(uri& uri){
	auto resources = uri.resource().to_string();
	transform(resources.begin(), resources.end(), resources.begin(), ::tolower);
	return split(resources, L'/');
}


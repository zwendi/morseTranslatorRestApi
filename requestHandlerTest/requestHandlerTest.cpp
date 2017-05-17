#include "stdafx.h"
#include "CppUnitTest.h"
#include "requestHandler.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace web;
using namespace web::http;

namespace requestHandlerTest
{		
	TEST_CLASS(requestHandlerTest)
	{
	public:
		wstring testBody = L"SOS";
		wstring expectedStr = L"";
		wstring jsonResponse = L"Answer";
		wstring errorJsonResponse = L"Error";
		int expectedCode;
		requestHandler handler;
		http_request mockRequest;
		web::uri uri;

		TEST_METHOD(handlePostEngToMorseRequestTest)
		{
			// TODO: Your test code here
			mockRequest.set_body(testBody);
			mockRequest.set_method(web::http::methods::POST);
			uri = L"/eng-to-morse";
			mockRequest.set_request_uri(uri);
			handler.handle_post(mockRequest);

			auto response =  mockRequest.get_response();
			auto statusCode  = (int)response.get().status_code();
			expectedCode = 200;
			Assert::AreEqual(expectedCode, statusCode, L"Status code is not matching.");
			json::value jsonAnswer = response.get().extract_json().get();

			wstring strAnswer = jsonAnswer[jsonResponse].as_string();
			expectedStr = L"... --- ...";
			Assert::AreEqual(expectedStr,strAnswer, L"Translated string is not matching expected string.");
		}

		TEST_METHOD(handlePostMorseToEngRequestTest)
		{
			// TODO: Your test code here
			testBody = L"... --- ...";
			mockRequest.set_body(testBody);
			mockRequest.set_method(web::http::methods::POST);
			uri = L"/morse-to-eng";
			mockRequest.set_request_uri(uri);
			handler.handle_post(mockRequest);

			auto response = mockRequest.get_response();
			auto statusCode = (int)response.get().status_code();
			expectedCode = 200;
			Assert::AreEqual(expectedCode, statusCode, L"Status code is not matching.");
			json::value jsonAnswer = response.get().extract_json().get();

			wstring strAnswer = jsonAnswer[jsonResponse].as_string();
			expectedStr = L"SOS";
			Assert::AreEqual(expectedStr, strAnswer, L"Translated string is not matching expected string.");
		}

		TEST_METHOD(handlePostNonRecognizeResource){
			mockRequest;
			testBody = L"... --- ...";
			mockRequest.set_body(testBody);
			mockRequest.set_method(web::http::methods::POST);
			uri = L"/non-existing-resource";
			mockRequest.set_request_uri(uri);
			handler.handle_post(mockRequest);

			auto response = mockRequest.get_response();
			auto statusCode = (int)response.get().status_code();
			expectedCode = 400;
			Assert::AreEqual(expectedCode, statusCode, L"Status code is not matching.");
			json::value jsonAnswer = response.get().extract_json().get();

			wstring strAnswer = jsonAnswer[errorJsonResponse].as_string();
			expectedStr = L"Unrecognized post request:non-existing-resource";
			Assert::AreEqual(expectedStr, strAnswer, L"Error message is not matching.");
		}


		TEST_METHOD(handlePostTooManyResource){
			mockRequest;
			testBody = L"body is not important";
			mockRequest.set_body(testBody);
			mockRequest.set_method(web::http::methods::POST);
			uri = L"/eng-to-morse/additional";
			mockRequest.set_request_uri(uri);
			handler.handle_post(mockRequest);

			auto response = mockRequest.get_response();
			auto statusCode = (int)response.get().status_code();
			expectedCode = 400;
			Assert::AreEqual(expectedCode, statusCode, L"Status code is not matching.");
			json::value jsonAnswer = response.get().extract_json().get();

			wstring strAnswer = jsonAnswer[errorJsonResponse].as_string();
			expectedStr = L"Too many or too few resources are supplied.";
			Assert::AreEqual(expectedStr, strAnswer, L"Error messages are not matching");
		}


	};
}
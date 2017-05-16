#pragma once
#include <string>
#include <unordered_map>
#include <memory>
using namespace std;

/*
* design chocie: why use singelton? Since the translator is used only for translating an input english string to morse code or decode 
* So across the entire application, we only need one such instance and that instance perform the same task.
*/
class morseTranslator
{
public:
	~morseTranslator();

	//effective modern c++, a delete method will provide better error message
	morseTranslator(morseTranslator const&) = delete;
	void operator=(morseTranslator const&) = delete;

	//taking a errStr to handle different translation error during encoding and decoding
	wstring encodeEnglish(wstring& englishStr, wstring& errStr);
	wstring decodeMorse(wstring& morseStr, wstring& errStr);
	
	//using singleton pattern to reduce memory resource.  
	static morseTranslator& getMorseTranslator();

private:
	//forbidden default constructor just taking one memory resource per program
	morseTranslator();
	
	//hiding implementation detail, allow switching our data structure without require a recompilation on client code.
	class privateData;
	privateData* pData;



};


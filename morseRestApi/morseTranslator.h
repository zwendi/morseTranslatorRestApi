#pragma once
#include <string>
using namespace std;

/*
* design chocie: why use singelton? Since the translator is used only for translating an input english string to morse code or decode 
* So across the entire application, we only need one such instance and that instance performs the same task.
*/
class morseTranslator
{
public:
	~morseTranslator();

	//according to effective modern c++, delete methods will provide better error message
	morseTranslator(morseTranslator const&) = delete;
	void operator=(morseTranslator const&) = delete;

    //skipping invalid characters, two methods required for the assignment
	wstring encodeEnglish(const wstring& englishStr);
	wstring decodeMorse(const wstring& morseStr);
	
	//using singleton pattern to reduce memory resource.  
	static morseTranslator& getMorseTranslator();

private:
	//forbidden default constructor just taking one memory resource per program
	morseTranslator();

	//hiding implementation detail, allow switching our data structure without require a recompilation on client code.
	class privateData;
	privateData* pData;



};


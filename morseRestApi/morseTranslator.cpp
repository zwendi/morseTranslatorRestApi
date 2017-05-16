#include "stdafx.h"
#include "morseTranslator.h"
#include <unordered_map>


class morseTranslator::privateData{
	
public: 
	privateData(){};

	~privateData(){};
	void initialized();

	void addPair(wchar_t character, wstring morse);

	/*
	 *Since morse code is well establish standard and likelihood for change is minimal, adding 
	 *an extra unordered_map is only taking extra O(number of characters for morse) space. By doing so 
	 *we will have a O(1) translation for morse or english translation.
	*/
	unordered_map<wchar_t, wstring> engMorseMap;
	unordered_map<wstring, wchar_t> morseEngMap;
};
morseTranslator::morseTranslator()
{
	pData = new privateData;
	pData->initialized();
}


morseTranslator& morseTranslator::getMorseTranslator()
{
	//static local variable is guarantee to be initialized and properly destroied, and it is thread safe in c++11 
	static morseTranslator translator;
	return  translator;
}


morseTranslator::~morseTranslator()
{
	if (pData){
		delete pData;
		pData = NULL;
	}
}


void morseTranslator::privateData::initialized()
{
	addPair(L'A', L".-");
	addPair(L'B', L"-...");
	addPair(L'C', L"-.-.");
	addPair(L'D', L"-..");
	addPair(L'E', L".");
	addPair(L'F', L"..-.");
	addPair(L'G', L"--.");
	addPair(L'H', L"....");
	addPair(L'I', L"..");
	addPair(L'J', L".---");  
	addPair(L'K', L"-.-");
	addPair(L'L', L".-..");
	addPair(L'M', L"--");
	addPair(L'N', L"-.");
	addPair(L'O', L"---");
	addPair(L'P', L".--.");
	addPair(L'Q', L"--.-");
	addPair(L'R', L".-.");
	addPair(L'S', L"...");
	addPair(L'T', L"-");
	addPair(L'U', L"..-");
	addPair(L'V', L"...-");
	addPair(L'W', L".--");
	addPair(L'X', L"-..-");
	addPair(L'Y', L"-.--");
	addPair(L'Z', L"--..");
	addPair(L'1', L".----");
	addPair(L'2', L"..---");
	addPair(L'3', L"...--");
	addPair(L'4', L"....-");
	addPair(L'5', L".....");
	addPair(L'6', L"-....");
	addPair(L'7', L"--...");
	addPair(L'8', L"---..");
	addPair(L'9', L"----.");
	addPair(L'0', L"-----");
	addPair(L' ', L"   ");  
}

/*
* Arguably this addPair function should require null check. However, since we are hard coding 
* the translation and this function is private(other user should only use the morseTranslator API)
* therefore i think it is safe to skip null check in this private function.
*/
void morseTranslator::privateData::addPair(wchar_t character, wstring morse)
{
	engMorseMap.insert(make_pair(character, morse));
	morseEngMap.insert(make_pair(morse, character));
}


wstring morseTranslator::encodeEnglish(wstring& englishStr, wstring& errStr)
{
	wstring result = L"";
	auto endItr = pData->engMorseMap.end();
	for (auto& character : englishStr)
	{
		auto itr = pData->engMorseMap.find(character);
		if (itr != endItr){
			result += itr->second +L" ";
		}
		else{
			errStr = L"The english string contains character that can't be translated.";
			return L"";
		}
	}
	//get rid of tailing space if there's more than 1 character
	auto strSize = result.size();
	if (strSize > 1)
		result = result.substr(0, strSize - 1);

	return result;
}

wstring morseTranslator::decodeMorse(wstring& morseStr, wstring& errStr)
{
	wstring result = L"";
	auto endItr = pData->morseEngMap.end();
	wstring tempString = L"";
	for (auto& morseCharacter : morseStr)
	{  
		if (morseCharacter != L' ')
		{
			tempString += morseCharacter;
			continue;
		}
		else{
			auto itr = pData->morseEngMap.find(tempString);
			if (itr != endItr){
				result += itr->second;
				tempString = L"";
			}
			else{
				errStr = L"The morse string contains code that can't be translated.";
				return L"";
			}
		} 
	}
	return result;
}
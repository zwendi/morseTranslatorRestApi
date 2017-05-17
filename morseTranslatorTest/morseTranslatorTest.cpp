#include "stdafx.h"
#include "CppUnitTest.h"
#include "morseTranslator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace morseTranslatorTest
{		
	TEST_CLASS(morseTranslatorTest)
	{
	public:
		//the translator and error msg will be used acrosses functions
		morseTranslator& translator = morseTranslator::getMorseTranslator();
		wstring expectedStr = L"";

		TEST_METHOD(encodeNullString)
		{
			// TODO: Your test code here
			wstring engString = L"";
	        auto resultStr = translator.encodeEnglish(engString);
			expectedStr = L""; // for easier to follow the test in the future

			Assert::AreEqual(expectedStr, resultStr, L"An empty english string should return an empty Morse string.");
		}

		/*
		*  This is a very important special case
		*/
		TEST_METHOD(encodeStringWithOnlySpaceCase)
		{
			// TODO: Your test code here
			wstring engString = L" ";  
			auto resultStr = translator.encodeEnglish(engString);
			expectedStr = L"  "; 
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
			engString = L"  ";
			resultStr = translator.encodeEnglish(engString);
			expectedStr = L"    "; 
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");

		}

		TEST_METHOD(encodeStringWithoutSpace)
		{
			// TODO: Your test code here
			wstring engString = L"SOS";
			auto resultStr = translator.encodeEnglish(engString);
			expectedStr = L"... --- ..."; 
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
		}

		TEST_METHOD(encodeStringWithUpperAndLowerCase)
		{
			// TODO: Your test code here
			wstring engString = L"Hello World";
			auto resultStr = translator.encodeEnglish(engString);
			expectedStr = L".... . .-.. .-.. ---  .-- --- .-. .-.. -.."; 
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
		}


		TEST_METHOD(encodeStringWithLeadingAndTailingSpaces)
		{
			// TODO: Your test code here
			wstring engString = L" S o S ";  
			auto resultStr = translator.encodeEnglish(engString);
			expectedStr = L"  ...  ---  ...  "; 
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
	        
			engString = L" This is an interesting excercises. ";
			expectedStr = L"  - .... .. ...  .. ...  .- -.  .. -. - . .-. . ... - .. -. --.  . -..- -.-. . .-. -.-. .. ... . ...  ";
			resultStr = translator.encodeEnglish(engString);
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");

		}

		
		TEST_METHOD(encodeStringWithUnrecognizedCharacter)
		{
			// TODO: Your test code here
			wstring engString = L">+>@  S";  
			auto resultStr = translator.encodeEnglish(engString);
			expectedStr = L"    ..."; // two space in front of S should result in four spaces in morse
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
		}


		TEST_METHOD(encodeAllAvailableCharacters)
		{
			wstring engString = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
			expectedStr = L".- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- .-. ... - ..- ...- .-- -..- -.-- --.. ----- .---- ..--- ...-- ....- ..... -.... --... ---.. ----. -----";
			auto resultStr = translator.encodeEnglish(engString);
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
			engString = L"abcdefghijklmnopqrstuvwxyz01234567890";
			resultStr = translator.encodeEnglish(engString);
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
		}


		TEST_METHOD(decodeMorseString)
		{
			wstring morseString = L"... --- ...";
			auto resultStr = translator.decodeMorse(morseString);
			expectedStr = L"SOS"; 
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
		}

		TEST_METHOD(decodeEmptyString)
		{
			wstring morseString = L"";
			auto resultStr = translator.decodeMorse(morseString);
			expectedStr = L"";
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
		}

		TEST_METHOD(decodeMorseStringWithSpace)
		{
			wstring morseString = L"....  .  .-.. .-.. ---  .-- --- .-. .-..  -..   ";
			auto resultStr = translator.decodeMorse(morseString);
			expectedStr = L"H E LLO WORL D "; 
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
		}

		TEST_METHOD(decodeMorseStringWithLeadingAndTailingSpace)
		{
			wstring morseString = L"  .... . .-.. .-.. ---  .-- --- .-. .-.. -..  ";
			auto resultStr = translator.decodeMorse(morseString);
			expectedStr = L" HELLO WORLD "; 
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
		}

		TEST_METHOD(decodeMorseStringWithOnlySpace)
		{
			wstring morseString = L"  ";
			auto resultStr = translator.decodeMorse(morseString);
			expectedStr = L" "; // for easier to follow the test in the future
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
			morseString = L"      ";
			expectedStr = L"   ";
			resultStr = translator.decodeMorse(morseString);
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
		}

		TEST_METHOD(decodeStringWithInvalidCharacters)
		{
			wstring morseString = L"... *x  ... --- ...";
			auto resultStr = translator.decodeMorse(morseString);
			expectedStr = L"S SOS"; // the two space in morse after x should result a " " in english
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");

			morseString = L"-------- .- -..... .";
			expectedStr = L"AE";  // we should ignoring invalid morse code
			resultStr = translator.decodeMorse(morseString);
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");

		}

		TEST_METHOD(decodeStringWithAllCharacters)
		{
			wstring morseString = L".- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- .-. ... - ..- ...- .-- -..- -.-- --.. ----- .---- ..--- ...-- ....- ..... -.... --... ---.. ----. -----";
			auto resultStr = translator.decodeMorse(morseString);
			expectedStr = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890"; 
			Assert::AreEqual(expectedStr, resultStr, L"The translation does not match expected output");
		}
	};
}
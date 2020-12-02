﻿#include "Font.h"
#include "SpriteManager.h"
using namespace std;
CFont::CFont()
{
	LoadSprite();
}

void CFont::LoadSprite()
{
	auto spriteManager = CSpriteManager::GetInstance();
	fonts.insert(make_pair('0', spriteManager->Get("spr-font-0")));
	fonts.insert(make_pair('1', spriteManager->Get("spr-font-1")));
	fonts.insert(make_pair('2', spriteManager->Get("spr-font-2")));
	fonts.insert(make_pair('3', spriteManager->Get("spr-font-3")));
	fonts.insert(make_pair('4', spriteManager->Get("spr-font-4")));
	fonts.insert(make_pair('5', spriteManager->Get("spr-font-5")));
	fonts.insert(make_pair('6', spriteManager->Get("spr-font-6")));
	fonts.insert(make_pair('7', spriteManager->Get("spr-font-7")));
	fonts.insert(make_pair('8', spriteManager->Get("spr-font-8")));
	fonts.insert(make_pair('9', spriteManager->Get("spr-font-9")));
	fonts.insert(make_pair('A', spriteManager->Get("spr-font-A")));
	fonts.insert(make_pair('B', spriteManager->Get("spr-font-B")));
	fonts.insert(make_pair('C', spriteManager->Get("spr-font-C")));
	fonts.insert(make_pair('D', spriteManager->Get("spr-font-D")));
	fonts.insert(make_pair('E', spriteManager->Get("spr-font-E")));
	fonts.insert(make_pair('F', spriteManager->Get("spr-font-F")));
	fonts.insert(make_pair('G', spriteManager->Get("spr-font-G")));
	fonts.insert(make_pair('H', spriteManager->Get("spr-font-H")));
	fonts.insert(make_pair('I', spriteManager->Get("spr-font-I")));
	fonts.insert(make_pair('J', spriteManager->Get("spr-font-J")));
	fonts.insert(make_pair('K', spriteManager->Get("spr-font-K")));
	fonts.insert(make_pair('L', spriteManager->Get("spr-font-L")));
	fonts.insert(make_pair('M', spriteManager->Get("spr-font-M")));
	fonts.insert(make_pair('N', spriteManager->Get("spr-font-N")));
	fonts.insert(make_pair('O', spriteManager->Get("spr-font-O")));
	fonts.insert(make_pair('P', spriteManager->Get("spr-font-P")));
	fonts.insert(make_pair('Q', spriteManager->Get("spr-font-Q")));
	fonts.insert(make_pair('R', spriteManager->Get("spr-font-R")));
	fonts.insert(make_pair('S', spriteManager->Get("spr-font-S")));
	fonts.insert(make_pair('T', spriteManager->Get("spr-font-T")));
	fonts.insert(make_pair('U', spriteManager->Get("spr-font-U")));
	fonts.insert(make_pair('V', spriteManager->Get("spr-font-V")));
	fonts.insert(make_pair('W', spriteManager->Get("spr-font-W")));
	fonts.insert(make_pair('X', spriteManager->Get("spr-font-X")));
	fonts.insert(make_pair('Y', spriteManager->Get("spr-font-Y")));
	fonts.insert(make_pair('Z', spriteManager->Get("spr-font-Z")));

}

void CFont::Render()
{
	// Test font
	/*vector<LPSprite> spriteStrings;
	spriteStrings = StringToSprites("Hello!");
	float x = 360, y = 360;
	if (spriteStrings.size() != 0)
	{
		for (auto s : spriteStrings)
		{
			s->Draw(D3DXVECTOR2(x, y), D3DXVECTOR2(1.0f, 1.0f), 0.0f);
			x += 50;
		}
	}*/
}

LPSprite CFont::CharToSprite(char character)
{
	// Kiểm tra coi có trong scope mình hỗ trợ hay k
	if (InScope(character) == false)
		return NULL;

	// Nếu là character ký tự A - Z hoặc a - z thì mình chuyển về uppercase hết r mới xử lý
	if (IsLetterLowerCase(character) == true)
	{
		int newUpperCase = (int)character - 32;
		character = (char)newUpperCase;
	}
	return fonts.at(character);
}

std::vector<LPSprite> CFont::StringToSprites(std::string str)
{
	vector<LPSprite> spriteStrings;
	for (char& c : str)
	{
		auto sprite = CharToSprite(c);
		if (sprite != NULL)
			spriteStrings.push_back(sprite);
	}
	return spriteStrings;
}

bool CFont::InScope(char character)
{
	// 0 - 9 tương ứng 48 - 57
	if ((int)character >= 48 && (int)character <= 57)
		return true;

	// A - Z tương ứng 65 - 90
	if ((int)character >= 64 && (int)character <= 90)
		return true;

	// a - z tương ứng 97 - 122
	if ((int)character >= 97 && (int)character <= 122)
		return true;

	return false;
}

bool CFont::IsLetterLowerCase(char character)
{
	// a - z tương ứng 97 - 122
	if ((int)character >= 97 && (int)character <= 122)
		return true;

	return false;
}

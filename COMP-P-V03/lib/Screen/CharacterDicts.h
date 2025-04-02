#ifndef __CharacterDicts_h__
#define __CharacterDicts_h__
#include <string.h>
#include <stdio.h>
#include "GeneralFunctions.h"
//Pseudo dictionaries
typedef struct CharacterBlockDictionary CharacterBlockDictionary;
typedef struct CBDictionaryElement CBDictionaryElement;
//Retrieve the byte array (p.e. font) related to the string input
unsigned char* Get(CharacterBlockDictionary*, char*, int);
#include "CharacterDicts.cpp"
#endif

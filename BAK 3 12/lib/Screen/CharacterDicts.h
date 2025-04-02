#ifndef __CharacterDicts_h__
#define __CharacterDicts_h__
#include <string.h>
#include <stdio.h>

#ifndef abs
#define abs(x) (x > 0 ? x : x*-1)
#endif
//Pseudo dictionaries
typedef struct CharacterBlockDictionary CharacterBlockDictionary;
typedef struct CBDictionaryElement CBDictionaryElement;
//Retrieve the byte array (p.e. font) related to the string input
unsigned char* Get(CharacterBlockDictionary*, char*, unsigned char);
#endif

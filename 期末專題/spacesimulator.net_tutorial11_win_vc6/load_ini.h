/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Author: Damiano Vitulli
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 *
 * Ini Parser library
 * 
 * File header
 *  
 */

#ifndef _LOAD_INI_H
#define _LOAD_INI_H

/*
 * CONSTANTS DECLARATION
 *  
 */

#define LOADINI_MAX_DICTIONARIES 20
#define LOADINI_MAX_ENTRIES 1000
#define LOADINI_MAX_CHARS 60



/*
 * TYPES DECLARATION
 *  
 */

typedef struct {
	char *section;
	char *key;
	char *value;
} loadini_field_type;

typedef struct {
	char filename[LOADINI_MAX_CHARS];
	int entries_qty;
	loadini_field_type fields[LOADINI_MAX_ENTRIES];
} loadini_type;



/*
 * VARIABLES DECLARATION
 *  
 */

extern int loadini_dictionaries_qty;
extern loadini_type loadini[LOADINI_MAX_DICTIONARIES];



/*
 * FUNCTIONS DECLARATION
 *  
 */

extern int LoadINI(char *p_filename);
extern int LoadINI_Save(char *p_filename);

extern int LoadINI_KeyParser(char *p_source_string,char *p_key, char *p_value);
extern void LoadINI_AddField(int p_dictionary_number, int p_field_number, char *p_section, char *p_key, char *p_value);
extern void LoadINI_StrDelChars (char *p_string, char *p_char_list);
extern void LoadINI_StrDelSpaces(char *p_string);
extern void LoadINI_StrDelComments(char *p_string);

extern char *LoadINI_GetString(char* p_filename, char *p_section, char *p_key, char *p_default);
extern long LoadINI_GetInt(char* p_filename, char *p_section, char *p_key, int p_default);
extern double LoadINI_GetFloat(char* p_filename, char *p_section, char *p_key, float p_default);
extern int LoadINI_SetString(char* p_filename, char *p_section, char *p_key, char *p_value);
extern int LoadINI_SetInt(char* p_filename, char *p_section, char *p_key, int p_value);
extern int LoadINI_SetFloat(char* p_filename, char *p_section, char *p_key, float p_value);

#endif
#ifndef __tHTML_Tools_h__
#define __tHTML_Tools_h__

void  HTML_SetInnerHTMLFromFile(char *id, char *filename);

void  HTML_SetInnerHTML(char *id, char *html);
char *HTML_GetInnerHTML(char *id);
void  HTML_SetStrAttribute(char *id, char *name, char *value);
char *HTML_GetStrAttribute(char *id, char *name);
void  HTML_SetNumAttribute(char *id, char *name, char *value);
int   HTML_GetNumAttribute(char *id, char *name);

#endif


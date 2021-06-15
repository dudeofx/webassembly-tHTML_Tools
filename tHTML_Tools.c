#include <emscripten.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>

//-----------------------------------------------------------------------
EM_JS(void, HTML_SetInnerHTML, (char *p1, char *p2), {
   var id = UTF8ToString(p1);
   var html = UTF8ToString(p2);

   document.getElementById(id).innerHTML = html;   
});
//-----------------------------------------------------------------------
EM_JS(char *, HTML_GetInnerHTML, (char *p1), { 
   var id = UTF8ToString(p1);

   var st = document.getElementById(id).innerHTML;
   var len = lengthBytesUTF8(st)+1;
   var buff = _malloc(len);
   stringToUTF8(st, buff, len);

   return buff; // be sure to free this memory
});
//-----------------------------------------------------------------------
EM_JS(void, HTML_SetStrAttribute, (char *p1, char *p2, char *p3), { 
   var id = UTF8ToString(p1);
   var name = UTF8ToString(p2);
   var value = UTF8ToString(p3);

   document.getElementById(id).setAttribute(name, value);
});
//-----------------------------------------------------------------------
EM_JS(char *, HTML_GetStrAttribute, (char *p1, char *p2), { 
   var id = UTF8ToString(p1);
   var name = UTF8ToString(p2);

   var st = document.getElementById(id).getAttribute(name);
   var len = lengthBytesUTF8(st)+1;
   var buff = _malloc(len);
   stringToUTF8(st, buff, len);

   return buff; // be sure to free this memory
});
//-----------------------------------------------------------------------
EM_JS(void, HTML_SetNumAttribute, (char *p1, char *p2, int value), { 
   var id = UTF8ToString(p1);
   var name = UTF8ToString(p2);

   document.getElementById(id).setAttribute(name, value);
});
//-----------------------------------------------------------------------
EM_JS(int, HTML_GetNumAttribute, (char *p1, char *p2), { 
   var id = UTF8ToString(p1);
   var name = UTF8ToString(p2);

   return document.getElementById(id).getAttribute(name);
});
//-----------------------------------------------------------------------
void  HTML_SetInnerHTMLFromFile(char *id, char *filename) {
   int fd;
   int size;
   char *buff;

   fd = open(filename, O_RDONLY);
   if (fd == -1) return;

   size = lseek(fd, 0, SEEK_END);
   buff = (char *) malloc(size+1);
   if (buff == NULL) return;

   lseek(fd, 0, SEEK_SET);
   read(fd, buff, size);
   close(fd);
   buff[size] = '\0';

   HTML_SetInnerHTML(id, buff);

}
//-----------------------------------------------------------------------


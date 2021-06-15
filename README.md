# webassembly-tHTML_Tools
Simple set of tools to manipulate the browser's DOM from emscripten C

In some other project I was trying to write a GUI library to make use of the browser's DOM. It dawned on me that HTML at its core is XML and XML data is stored either as attributes or as text in a body. Hence just a handfull of functions can do a long way. You can even use them to setup callbacks.

I included a simple calculator applet to show how to use them. 

### List of functions

      void  HTML_SetInnerHTMLFromFile(char *id, char *filename);
      void  HTML_SetInnerHTML(char *id, char *html);
      char *HTML_GetInnerHTML(char *id);
      void  HTML_SetStrAttribute(char *id, char *name, char *value);
      char *HTML_GetStrAttribute(char *id, char *name);
      void  HTML_SetNumAttribute(char *id, char *name, char *value);
      int   HTML_GetNumAttribute(char *id, char *name);

### Quick start to see the caculator applet

the batch file "build.sh" compiles the applet. Copy the following 4 files to your webserver and you are good to go.

      index.html
      calculator.data
      calculator.js
      calculator.wasm

### Quick overview of how the calculator works

On the assets folder there is an HTML snippet. The program loads the snippet from the file and inserts it inside the <div\> element with the id="test1". Doing that automatically deploys the GUI (Javascript DOM magic). Then the program attaches a callback to all the buttons. And there are a couple functions to read/write to the calculator's LCD. 

See the main() function of calculator.c to see how simple it is.
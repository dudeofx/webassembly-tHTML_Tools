#include <emscripten.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "tHTML_Tools.h"

char lcd[64];
int  lcd_size;
int  decimal;

double v_stack[2];
int    v_count;

int  opcode;
int  prev_opcode;
int  number_was_finalized;

int  lcd_error;

double v1;
double v2;

//-----------------------------------------------------------------------
void StartNewNumber() {
   number_was_finalized = 0;
   lcd[0] = '0';
   lcd_size = 1;
   lcd[lcd_size] = '\0';
   decimal = 0;
}
//-----------------------------------------------------------------------
void Clear(int digit) {
   lcd[0] = digit;
   lcd_size = 1;
   lcd[lcd_size] = '\0';
   decimal = 0;

   prev_opcode = 0;
   opcode = 0;
   number_was_finalized = 0;

   v_count = 0;
   lcd_error = 0;
}
//-----------------------------------------------------------------------
void WriteLCD(double num) {
   if (fabs(num) < 0.0000001) num = 0.0;
   if (fabs(num) > 9999999.0) {
      lcd_error = 1;
      sprintf(lcd, "error");
      lcd_size = strlen(lcd);
      return;
   }

   sprintf(lcd, "%.9f", num);
   lcd_size = strlen(lcd);
   while (lcd_size > 1) {
      if (lcd[lcd_size-1] != '0') break;
      lcd[lcd_size-1] = '\0';
      lcd_size--;
   }
   if (lcd[lcd_size-1] == '.') lcd[--lcd_size] = '\0';
}
//-----------------------------------------------------------------------
double ReadLCD() {
   return strtod(lcd, NULL);
}
//-----------------------------------------------------------------------
int UpdateLCD() {
   HTML_SetInnerHTML("lcd", lcd);
   return 1;
}
//-----------------------------------------------------------------------
double Solve(double v1, int opcode, double v2) {
   switch (opcode) {
      case '/': return v1 / v2; 
      case '*': return v1 * v2; 
      case '-': return v1 - v2; 
      case '+': return v1 + v2; 
   }
   return 0.0;
}
//-----------------------------------------------------------------------
EMSCRIPTEN_KEEPALIVE
int OpcodeHandler(int inp) {
   static int equ_flag = 0;

   if (inp == 'c') {
      equ_flag = 0;
      Clear('0');
      return UpdateLCD();
   }

   if (lcd_error > 0) return 1;

   if (equ_flag > 0) {
      equ_flag = 0;
      if (inp == '=') { // catch repeated '=' opcodes here
         v1 = v_stack[--v_count];
         v1 = Solve(v1, opcode, v2);
         v_stack[v_count++] = v1;
         WriteLCD(v1);
         equ_flag = 1;
         return UpdateLCD();
      }
   }

   if (inp == '=') {
      number_was_finalized = 1;
      v_stack[v_count++] = ReadLCD();
      if (v_count == 2) {
         v2 = v_stack[--v_count];
         v1 = v_stack[--v_count];
         v1 = Solve(v1, opcode, v2);
         v_stack[v_count++] = v1;
         WriteLCD(v1);
         equ_flag = 1;
         return UpdateLCD();
      }
   } else {
      if (number_was_finalized == 0) {
         number_was_finalized = 1;
         v_stack[v_count++] = ReadLCD();
         if (v_count == 2) {
            v2 = v_stack[--v_count];
            v1 = v_stack[--v_count];
            v1 = Solve(v1, opcode, v2);
            v_stack[v_count++] = v1;
            WriteLCD(v1);
         }
      }
      opcode = inp;
      return UpdateLCD();
   }

   return 1;
}
//-----------------------------------------------------------------------
EMSCRIPTEN_KEEPALIVE
int DigitHandler(int inp) {
   if (lcd_error > 0) return 1;

   if (inp == '.') {
      if (number_was_finalized > 0) StartNewNumber(); 
      if (decimal > 0) return 1;
      decimal = lcd_size;
      lcd[lcd_size++] = inp;
      lcd[lcd_size] = '\0';
      return UpdateLCD();
   } else {
      if (number_was_finalized > 0) StartNewNumber(); 
      if (lcd_size >= 9) return 1;
      if ((lcd_size == 1) && (lcd[0] == '0')) {
         lcd[0] = inp;
      } else {
         lcd[lcd_size++] = inp;
         lcd[lcd_size] = '\0';
      }
      return UpdateLCD();
   }

   return 1;
}
//-----------------------------------------------------------------------
void HandleAsOpcode(char *id, int inp) { // setup callback handler
   char script[128];

   sprintf(script, "Module.ccall('OpcodeHandler', null, ['number'], [%i]);", inp);
   HTML_SetStrAttribute(id, "onclick", script);
}
//-----------------------------------------------------------------------
void HandleAsDigit(char *id, int inp) { // setup callback handler
   char script[128];

   sprintf(script, "Module.ccall('DigitHandler', null, ['number'], [%i]);", inp);
   HTML_SetStrAttribute(id, "onclick", script);
}
//-----------------------------------------------------------------------
int main() {
   Clear('0');

   HTML_SetInnerHTMLFromFile("test1", "assets/calculator.html");

   HandleAsDigit("btn_digit_0", '0');
   HandleAsDigit("btn_digit_1", '1');
   HandleAsDigit("btn_digit_2", '2');
   HandleAsDigit("btn_digit_3", '3');
   HandleAsDigit("btn_digit_4", '4');
   HandleAsDigit("btn_digit_5", '5');
   HandleAsDigit("btn_digit_6", '6');
   HandleAsDigit("btn_digit_7", '7');
   HandleAsDigit("btn_digit_8", '8');
   HandleAsDigit("btn_digit_9", '9');
   HandleAsDigit("btn_dot", '.');

   HandleAsOpcode("btn_div", '/');
   HandleAsOpcode("btn_mul", '*');
   HandleAsOpcode("btn_sub", '-');
   HandleAsOpcode("btn_add", '+');
   HandleAsOpcode("btn_equ", '=');
   HandleAsOpcode("btn_clear", 'c');

   emscripten_exit_with_live_runtime();

   return 0;
}
//-----------------------------------------------------------------------


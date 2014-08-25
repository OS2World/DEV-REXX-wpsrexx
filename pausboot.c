/*------------------------------------------------------------------*/
/*           Copyright (c) 1992, Blue Bird Computing, Inc.          */
/*             This is free software (NOT public domain).           */
/*                                                                  */
/*  You can use, redistribute, or modify this software provided no  */
/*  fee is charged and this message is not removed.                 */
/*------------------------------------------------------------------*/
/*  Revision History:                                               */
/*  1.00 12Apr92  M.A. Stern      New                               */
/*  1.01 06May92  M.A. Stern      Changed time counter to tenths.   */
/*                                Allow cancel of countdown.        */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*  Pause the OS/2 2.0 boot up process and go to a command          */
/*  prompt if spacebar pressed within some number of seconds.       */
/*                                                                  */
/*  Usage (In config.sys):   CALL=PAUSBOOT.EXE <seconds>            */
/*                                                                  */
/*  <seconds> defaults to 5 if no value given.                      */
/*------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define INCL_VIO
#define INCL_DOSPROCESS

#include <os2.h>

#define PROMPT " /K PROMPT=Type EXIT to continue boot-up.$_$_$i[$p]"

int main( int argc, char **argv ) {

   int     i,fPause = 0;
   int     cSecs, cTenths, cDiff = 0;
   char    szMessage[80];
   char    szSysCmd[200];
   char    c;
   char    cCell[2];

   if ( argc > 1 ) cDiff = atoi( argv[1] );
   if ( cDiff == 0 ) cDiff = 5;

   cTenths = cDiff * 10;

   /* Clear screen */
   cCell[0] = 0x20;
   cCell[1] = 0x1f;
   VioScrollUp(0, 0, 0xFFFF, 0xFFFF, 0xFFFF, cCell, 0);
   VioSetCurPos( 0, 0, 0);

   strcpy( szMessage, "Press ESCAPE to cancel countdown "
                           "and continue boot." );
   VioWrtCharStr( szMessage, strlen( szMessage ), 9, 14, 0 );

   while ( cTenths && !fPause ) {

       cSecs = (cTenths+9) / 10;
       cTenths--;

       sprintf( szMessage, "Press SPACEBAR within %d seconds for "
                           "command prompt. ", cSecs );
                           /* Extra space above is deliberate! */

       VioWrtCharStr( szMessage, strlen( szMessage ), 12, 14, 0 );

       DosSleep( 100L );
       if ( kbhit() ) {
           c = getch();
           if ( c == 0 || c == 0xe0 ) c = getch();
           if ( c == 27 ) break;
           if ( c == 32 ) fPause = 1;
           }
       }

   /* Clear screen again */
   cCell[1] = 0x07;
   VioScrollUp(0, 0, 0xFFFF, 0xFFFF, 0xFFFF, cCell, 0);
   VioSetCurPos( 0, 0, 0);

   if ( fPause ) {
       strcpy( szSysCmd, getenv("COMSPEC") );
       strcat( szSysCmd, PROMPT );
       system( szSysCmd );
       }
   return 0;
   }

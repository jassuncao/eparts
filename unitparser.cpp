#include "unitparser.h"
#include "QVarLengthArray"
#include "QLocale"

UnitParser::UnitParser()
{
}


double UnitParser::parseUnit(const QString& num, bool * ok)
{
     QVarLengthArray<QChar, 256> charBuff;
     const QChar *uc = num.unicode();
     int len = num.length();
     int idx = 0;
     QChar prefix=0;
     QLocale locale;
     QChar decimalPoint = locale.decimalPoint();
     int decimalPointIdx = 0;
     while (idx < len && uc[idx].isSpace())
         ++idx;
     if (idx == len){
         if(ok!=0)
             *ok=false;
         return 0;
     }

     while(idx<len){
         const QChar &in = uc[idx];
         if(in.isDigit()){
             charBuff.append(in);
         }
         else if(in.isLetter() && prefix==0){
            prefix=in;
            if(decimalPointIdx==0)
                charBuff.append(decimalPoint);
         }
         else if(in==decimalPoint){
             decimalPointIdx=charBuff.count();
             charBuff.append(in);
         }
         else
            break;

         ++idx;
     }

     for (; idx < len; ++idx) {
        if (!uc[idx].isSpace()){
            if(ok!=0)
                *ok=false;
            return 0;
        }
     }

     double value = QString(charBuff.constData()).toDouble(ok);
     if(prefix!=0){
         switch(prefix.unicode()){
         case 'r':
         case 'R':
             break;
         case 'K':
         case 'k':
             value*=1E3;
             break;
         case 'M':
             value*=1E6;
             break;
         case 'G':
             value*=1E9;
             break;
         case 'T':
             value*=1E12;
             break;
         case 'P':
             value*=1E15;
             break;
         case 'E':
             value*=1E18;
             break;
         case 'Z':
             value*=1E21;
             break;
         case 'Y':
             value*=1E24;
             break;
         case 'm':
             break;
             value*=1E-3;
         case 0x03BC:
         case 'u':
             value*=1E-6;
             break;
         case 'n':
             value*=1E-9;
             break;
         case 'p':
             value*=1E-12;
             break;
         case 'f':
             value*=1E-5;
             break;
         case 'a':
             value*=1E-9;
             break;
         case 'z':
             value*=1E-12;
             break;
         case 'y':
             value*=1E-15;
             break;
         }
     }
     return value;
}

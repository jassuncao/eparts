#include "unitformatter.h"
#include "qtextstream.h"
#include "qdebug.h"

static const ushort BIG_PREFIXES[] = {0,'k','M','G','T','P','E','Z','Y'};
static const ushort SMALL_PREFIXES[] = {0,'m',0x03BC,'n','p','f','a','z','y'};

UnitFormatter::UnitFormatter()
{
}

QString UnitFormatter::format(PartParameter::ParameterType paramType, double value){
    switch(paramType){
        case PartParameter::Resistance:
            return formatResistance(value);
        case PartParameter::Capacitance:
            return formatCapacitance(value);
        case PartParameter::Inductance:
            return formatInductance(value);
        case PartParameter::Power:
            return formatPower(value);
        case PartParameter::Percentage:
            return formatPercentage(value);
        default:
            return QString::number(value);
    }
}

static void removeTrailingZeros(QString & str)
{
    QChar decimalPoint = '.';
    int idx = str.indexOf(decimalPoint);
    if(idx<0) return;
    int i=str.length()-1;
    while(i>0){
        QChar c = str.at(i);
        if(c=='0'){
            --i;
        }
        else if(c==decimalPoint){
            --i;
            break;
        }
        else{
            break;
        }
    }
    str.resize(i+1);
}

QString UnitFormatter::formatUnit(const double value, QChar unit)
{    


    if(value>0.999999999){
        double aux = value;
        int multiplier = 0;
        while(aux>1000){
            ++multiplier;
            aux=aux/1000;
        }
        ushort prefix = BIG_PREFIXES[multiplier];

        QString result;
        QTextStream out(&result);
        out.setRealNumberPrecision(2);
        out.setNumberFlags(out.numberFlags() & ~QTextStream::ForcePoint);
        out.setRealNumberNotation(QTextStream::FixedNotation);
        out<<aux;
        removeTrailingZeros(result);
        if(prefix){
            out<<QChar(prefix);
        }        
        out<<unit;
        return result;
    }
    else{
        double aux = value;
        int divider = 0;
        while(aux<0.999999999){
            ++divider;
            aux=aux*1000;
        }
        ushort prefix = SMALL_PREFIXES[divider];
        QString result;
        QTextStream out(&result);
        out.setRealNumberPrecision(2);        
        out.setRealNumberNotation(QTextStream::FixedNotation);
        out<<aux;
        removeTrailingZeros(result);
        if(prefix){
            out<<QChar(prefix);
        }
        out<<unit;
        return result;
    }
}

QString UnitFormatter::formatPercentage(double value){
    QString result;
    QTextStream out(&result);
    out.setRealNumberPrecision(4);
    out.setRealNumberNotation(QTextStream::FixedNotation);
    out<<value<<'%';
    return result;
}



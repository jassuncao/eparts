#include "unitformatter.h"
#include "qtextstream.h"
#include "qdebug.h"

static const ushort BIG_PREFIXES[] = {0,'k','M','G','T','P','E','Z','Y'};
static const ushort SMALL_PREFIXES[] = {0,'m',0x03BC,'n','p','f','a','z','y'};

UnitFormatter::UnitFormatter()
{
}

const QChar UnitFormatter::resistance(0x2126);
const QChar UnitFormatter::capacitance('F');
const QChar UnitFormatter::inductance('H');
const QChar UnitFormatter::power('W');
const QChar UnitFormatter::percentage('%');
const QLocale currentLocale;

QString UnitFormatter::formatParameter(PartParameter::ParameterType paramType, double value, bool includeUnit){
    switch(paramType){
        case PartParameter::Resistance:
            if(includeUnit)
                return format(value, resistance);
            else
                return format(value,0);
        case PartParameter::Capacitance:
            if(includeUnit)
                return format(value, capacitance);
            else
                return format(value,0);
        case PartParameter::Inductance:
            if(includeUnit)
                return format(value, inductance);
            else
                return format(value,0);
        case PartParameter::Power:
            if(includeUnit)
                return format(value, power);
            else
                return format(value,0);
        case PartParameter::Percentage:
        if(includeUnit)
            return formatPercentage(value,percentage);
        else
            return formatPercentage(value,0);
        default:
            return QString::number(value);
    }
}

QChar UnitFormatter::getUnitSymbol(PartParameter::ParameterType paramType)
{
    switch(paramType){
        case PartParameter::Resistance:
            return resistance;
        case PartParameter::Capacitance:
            return capacitance;
        case PartParameter::Inductance:
            return inductance;
        case PartParameter::Power:
            return power;
        case PartParameter::Percentage:
            return percentage;
        default:
            return 0;
    }
}

static void removeTrailingZeros(QString & str)
{
    QChar decimalPoint = currentLocale.decimalPoint();
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

QString UnitFormatter::format(double value)
{
    return format(value, 0);
}

QString UnitFormatter::format(const double value, QChar unit)
{
    QString result;
    ushort prefix;
    QTextStream out(&result);
    out.setLocale(currentLocale);
    if(value>0.999999999){
        double aux = value;
        int multiplier = 0;
        while(aux>1000){
            ++multiplier;
            aux=aux/1000;
        }
        prefix = BIG_PREFIXES[multiplier];
        out.setRealNumberPrecision(2);
        out.setNumberFlags(out.numberFlags() & ~QTextStream::ForcePoint);
        out.setRealNumberNotation(QTextStream::FixedNotation);
        out<<aux;        
    }
    else{
        double aux = value;
        int divider = 0;
        while(aux<0.999999999){
            ++divider;
            aux=aux*1000;
        }
        prefix = SMALL_PREFIXES[divider];
        out.setRealNumberPrecision(2);        
        out.setRealNumberNotation(QTextStream::FixedNotation);
        out<<aux;              
    }
    removeTrailingZeros(result);
    if(prefix){
        out<<QChar(prefix);
    }
    if(unit!=0){
        out<<unit;
    }
    return result;
}

QString UnitFormatter::formatPercentage(double value, QChar unit){
    QString result;
    QTextStream out(&result);
    out.setRealNumberPrecision(4);
    out.setRealNumberNotation(QTextStream::SmartNotation);
    out<<value;
    if(unit!=0){
        out<<unit;
    }
    return result;
}



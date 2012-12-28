#include "attributeformatter.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "partattribute.h"

namespace Models {

static const QLocale currentLocale;
static const ushort BIG_PREFIXES[] = {0,'k','M','G','T','P','E','Z','Y'};
static const ushort SMALL_PREFIXES[] = {0,'m',0x03BC,'n','p','f','a','z','y'};

static const QChar SYMBOL_RESISTANCE(0x2126);
static const QChar SYMBOL_CAPACITANCE('F');
static const QChar SYMBOL_INDUCTANCE('H');
static const QChar SYMBOL_POWER('W');
static const QChar SYMBOL_PERCENTAGE('%');

AttributeFormatter::AttributeFormatter()
{
}

TextAttributeFormatter::TextAttributeFormatter()
{
}

QVariant TextAttributeFormatter::format(QVariant value, int role) const
{
    return value;
}

GenericNumberAttributeFormatter::GenericNumberAttributeFormatter()
{
}

QVariant GenericNumberAttributeFormatter::format(QVariant value, int role) const
{
    return value;
}

UnitAttributeFormatter::UnitAttributeFormatter(const QString unitSymbol) :
    _unitSymbol(unitSymbol)
{
}

UnitAttributeFormatter::UnitAttributeFormatter(const QChar unitSymbol) :
    _unitSymbol(QString(unitSymbol))
{
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

QVariant UnitAttributeFormatter::format(QVariant v, int role) const
{
    QString result;
    ushort prefix;
    QTextStream out(&result);
    double val = v.toReal();
    out.setLocale(currentLocale);
    if(val>0.999999999){
        double aux = val;
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
        double aux = val;
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
    if(role!=Qt::EditRole){
        out<<_unitSymbol;
    }
    return QVariant(result);
}

PercentageAttributeFormatter::PercentageAttributeFormatter()
{
}

QVariant PercentageAttributeFormatter::format(QVariant v, int role) const
{
    QString result;
    QTextStream out(&result);
    out.setRealNumberPrecision(4);
    out.setRealNumberNotation(QTextStream::SmartNotation);
    out<<v.toReal();
    if(role!=Qt::EditRole){
        out<<SYMBOL_PERCENTAGE;
    }
    return QVariant(result);
}

AttributeFormatter *AttributeFormatterFactory::formatterFor(int attributeType)
{
    switch(attributeType)
    {
    case ATTRIBUTE_GENERIC_NUMBER:
        return new GenericNumberAttributeFormatter();
    case ATTRIBUTE_RESISTANCE:
        return new UnitAttributeFormatter(SYMBOL_RESISTANCE);
    case ATTRIBUTE_CAPACITANCE:
        return new UnitAttributeFormatter(SYMBOL_CAPACITANCE);
    case ATTRIBUTE_INDUCTANCE:
        return new UnitAttributeFormatter(SYMBOL_INDUCTANCE);
    case ATTRIBUTE_POWER:
        return new UnitAttributeFormatter(SYMBOL_POWER);
    case ATTRIBUTE_PERCENTAGE:
        return new PercentageAttributeFormatter();
    case ATTRIBUTE_TEXT:
    default:
        return new TextAttributeFormatter();
    }
}

}//namespace

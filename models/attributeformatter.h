#ifndef ATTRIBUTEFORMATTER_H
#define ATTRIBUTEFORMATTER_H

#include <QString>
#include <QVariant>

namespace Models {

class AttributeFormatter
{
public:
    virtual QVariant format(QVariant value, int role=Qt::DisplayRole) const = 0;
protected:
    AttributeFormatter();
};

class TextAttributeFormatter : public AttributeFormatter {
public:
    TextAttributeFormatter();
    virtual QVariant format(QVariant value, int role=Qt::DisplayRole) const;
};

class GenericNumberAttributeFormatter : public AttributeFormatter {
public:
    GenericNumberAttributeFormatter();
    virtual QVariant format(QVariant value, int role=Qt::DisplayRole) const;
};

class UnitAttributeFormatter : public AttributeFormatter {
public:
    UnitAttributeFormatter(const QString unitSymbol);
    UnitAttributeFormatter(const QChar unitSymbol);
    virtual QVariant format(QVariant v, int role=Qt::DisplayRole) const;
private:
    const QString _unitSymbol;
};

class PercentageAttributeFormatter : public AttributeFormatter {
public:
    PercentageAttributeFormatter();
    virtual QVariant format(QVariant v, int role=Qt::DisplayRole) const;
};

class AttributeFormatterFactory {
public:
    static AttributeFormatter * formatterFor(int attributeType);
};

}//namespace

#endif // ATTRIBUTEFORMATTER_H

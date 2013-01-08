#include "partattribute.h"

namespace Models {

/*
AbstractPartAttribute::AbstractPartAttribute(const int id, const QString & name, const QString & description, const QString &unit, const AttributeType type) :
    _id(id),
    _name(name),
    _description(description),
    _unit(unit),
    _type(type)
{
}
*/

AbstractPartAttribute::AbstractPartAttribute(const int id, const QString &name, const QString &description, QObject *parent) :
    QObject(parent),
    _id(id),
    _name(name),
    _description(description)
{
}

FloatAttribute::FloatAttribute(const int id, const QString &name, const QString &description, QObject *parent) :
    AbstractPartAttribute(id,name,description, parent)
{
}

void FloatAttribute::accept(PartAttributeVisitor &visitor)
{
    visitor.visit(this);
}

IntegerAttribute::IntegerAttribute(const int id, const QString &name, const QString &description, QObject *parent) :
    AbstractPartAttribute(id,name,description, parent)
{
}

void IntegerAttribute::accept(PartAttributeVisitor &visitor)
{
    visitor.visit(this);
}

UnitAttribute::UnitAttribute(const int id, const QString &name, const QString &description, const QString &unitName, const QString &unitSymbol, QObject *parent) :
    AbstractPartAttribute(id,name,description, parent),
    _unitName(unitName),
    _unitSymbol(unitSymbol)
{
}

void UnitAttribute::accept(PartAttributeVisitor &visitor)
{
    visitor.visit(this);
}

PercentageAttribute::PercentageAttribute(const int id, const QString &name, const QString &description, QObject *parent) :
    AbstractPartAttribute(id,name,description, parent)
{
}

void PercentageAttribute::accept(PartAttributeVisitor &visitor)
{
    visitor.visit(this);
}

TextAttribute::TextAttribute(const int id, const QString &name, const QString &description, QObject *parent) :
    AbstractPartAttribute(id,name,description, parent)
{
}

void TextAttribute::accept(PartAttributeVisitor &visitor)
{
    visitor.visit(this);
}

}//namespace


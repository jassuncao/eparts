#include "types.h"

using namespace EParts;

Category::Category(const int id, const QString & name, const QString & description)
    : _id(id), _name(name), _description(description)
{}

const QString & Category::name() const
{
    return _name;
}

const QString & Category::description() const
{
    return _description;
}

int Category::id () const
{
    return _id;
}

void Category::setId(const int id)
{
    _id = id;
}

void Category::setName(const QString &name)
{
    _name = name;
}

void Category::setDescription(const QString &description)
{
    _description = description;
}

// PartDefinition section starts

PartDefinition::PartDefinition(const int id, const QString & name, const QString & description, const int categoryId)
    : _id(id), _name(name), _description(description), _categoryId(categoryId)
{}


// Parameter section starts

Parameter::Parameter(const int id, const QString& name, const QString& description, const ParameterType type, const FormatType format, const int partDefinitionId, const bool isFixedValue, const int order)
    : _id(id), _name(name), _description(description), _type(type), _format(format), _partId(partDefinitionId), _isFixedvalue(isFixedValue), _order(order)
{}

Parameter::Parameter(const Parameter& other )
{
    *this = other;
}

Parameter& Parameter::operator=(const Parameter& other )
{
    if(this != &other){
        _id = other._id;
        _name = other._name;
        _description = other._description;
        _type = other._type;
        _format = other._format;
        _partId = other._partId;
        _isFixedvalue = other._isFixedvalue;
        _order = other._order;
    }
    return *this;
}

/*
PartValue::PartValue(const int id) : _id(id)
{
}


PartValue::PartValue(const PartValue& other)
{
    *this = other;
}


PartValue& PartValue::operator=(const PartValue& other )
{
    *this = other;
}


NumericValue::NumericValue(const int id, double value) : PartValue(id), _numericValue(value)
{
}

TextValue::TextValue(const int id, const QString& value) : PartValue(id), _textValue(value)
{
}


TextValue::TextValue(const TextValue& other)
{
   *this = other;
}

TextValue& TextValue::operator=(const TextValue& other )
{
    if(this!=&other) {
        _id = other._id;
        _textValue = other._textValue;
    }
    return *this;
}
*/

Part::Part(const int id, const int quantity, const int minimumQuantity, const QString & notes)
    : _id(id), _quantity(quantity), _minimumQuantity(minimumQuantity), _notes(notes)
{
}

#include "partattribute.h"

namespace Models {

PartAttribute::PartAttribute(const int id, const QString & name, const QString & description, const AttributeType type) :
    _id(id),
    _name(name),
    _description(description),
    _type(type)
{
}

}//namespace

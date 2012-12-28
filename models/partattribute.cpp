#include "partattribute.h"

namespace Models {

PartAttribute::PartAttribute(int id, const QString &name, AttributeType type, bool virtualAttr) :
    _id(id),
    _name(name),
    _type(type),
    _virtualAttr(virtualAttr)
{
}

}//namespace

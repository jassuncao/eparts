#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace EParts
{

    enum FilterOperator{
        FILTER_OP_EQUAL,
        FILTER_OP_NOT_EQUAL,
        FILTER_OP_LESS_EQUAL_THAN,
        FILTER_OP_GREATER_EQUAL_THAN,
        FILTER_OP_BETWEEN,
        FILTER_OP_CONTAINS,
        FILTER_OP_NOT_CONTAINS
    };

    enum AttributeType
    {
        ATTRIBUTE_GENERIC_FLOAT,
        ATTRIBUTE_GENERIC_INTEGER,
        ATTRIBUTE_UNIT,
        ATTRIBUTE_PERCENTAGE,
        ATTRIBUTE_TEXT=1<<16
    };
}

#endif // CONSTANTS_H

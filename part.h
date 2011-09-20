#ifndef PART_H
#define PART_H

#include <dquest.h>
#include "parttype.h"

class Part : public DQModel {
    DQ_MODEL
public:
    DQField<int> quantity;
    DQField<int> minimumQuantity;
    DQField<QString> notes;
    DQForeignKey<PartType> partType;
    DQField<qreal> numericValue;
    DQField<QString> textValue;
};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(Part,
                 "part", // the table name.
                 DQ_FIELD(quantity, DQNotNull),
                 DQ_FIELD(minimumQuantity, DQNotNull),
                 DQ_FIELD(notes),
                 DQ_FIELD(partType , DQNotNull),
                 DQ_FIELD(numericValue),
                 DQ_FIELD(textValue)
                 )

#endif // PART_H

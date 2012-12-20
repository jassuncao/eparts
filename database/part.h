#ifndef PART_H
#define PART_H

#include <dquest.h>
#include "parttype.h"

class Part : public DQModel {
    DQ_MODEL
public:
    DQField<int> quantity;
    DQField<int> minimumQuantity;
    DQForeignKey<PartType> partType;
};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(Part,
                 "part", // the table name.
                 DQ_FIELD(quantity, DQNotNull),
                 DQ_FIELD(minimumQuantity, DQNotNull),
                 DQ_FIELD(partType , DQNotNull)
                 )

#endif // PART_H

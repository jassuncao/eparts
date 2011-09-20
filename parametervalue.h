#ifndef PARAMETERVALUE_H
#define PARAMETERVALUE_H

#include <dquest.h>
#include "part.h"
#include "partparameter.h"

class ParameterValue : public DQModel {
    DQ_MODEL
public:
    DQField<qreal> numericValue;
    DQField<QString> textValue;
    DQForeignKey<Part> part;
    DQForeignKey<PartParameter> partParameter;
};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(ParameterValue,
                 "parameter_value", // the table name.
                 DQ_FIELD(numericValue),
                 DQ_FIELD(textValue),
                 DQ_FIELD(part , DQNotNull),
                 DQ_FIELD(partParameter , DQNotNull)
                 )

#endif // PARAMETERVALUE_H

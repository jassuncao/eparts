#ifndef PARTPARAMETER_H
#define PARTPARAMETER_H

#include <dquest.h>
#include "parttype.h"

class PartParameter : public DQModel {    
    DQ_MODEL
public:
    enum ParameterType
    {
        Text, LongText, GenericNumber, Resistance, Capacitance, Inductance, Power, Percentage
    };    

    DQField<QString> name;
    DQField<int> type;    
    DQField<QString> description;
    DQForeignKey<PartType> partType;
    DQField<bool> fixedValues;
    DQField<int> orderIndex;

    inline bool isText(){
        uint value = type.get().toUInt();
        return value == Text || value == LongText;
    }

    inline static bool isText(ParameterType paramType){
        return paramType == Text || paramType == LongText;
    }
};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(PartParameter,
                 "part_parameters", // the table name.
                 DQ_FIELD(name, DQNotNull),
                 DQ_FIELD(type, DQNotNull),
                 DQ_FIELD(description),
                 DQ_FIELD(partType , DQNotNull),
                 DQ_FIELD(fixedValues, DQNotNull),
                 DQ_FIELD(orderIndex, DQNotNull)
                 )


#endif // PARTPARAMETER_H

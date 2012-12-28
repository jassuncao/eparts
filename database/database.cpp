
#include "database.h"
#include <QtSql>
#include <QtGui>
#include <dquest.h>
#include "entities.h"
#include <models/partattribute.h>

//using namespace Database;

namespace Database {

Database::Database()
{   
}



bool Database::initTables(DQConnection &connection)
{
     QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
     db.setDatabaseName(":memory:");
     if (!db.open()) {
         QMessageBox::critical(0, qApp->tr("Cannot open database"),
             qApp->tr("Unable to establish a database connection.\n"
                      "This example needs SQLite support. Please read "
                      "the Qt SQL driver documentation for information how "
                      "to build it.\n\n"
                      "Click Cancel to exit."), QMessageBox::Cancel);
         return false;
     }
     connection.open(db); // Establish the connection to database. It will become the "default connection" shared by all DQModel

     connection.addModel<DQCategory>();
     connection.addModel<DQPart>();
     connection.addModel<DQAttribute>();
     connection.addModel<DQFloatValue>();
     connection.addModel<DQTextValue>();
     connection.addModel<DQCategoryActiveAttributes>();

     return connection.createTables();
}

void Database::generateMockData()
{
    DQCategory resistorsCat;
    resistorsCat.name="Resistors";
    resistorsCat.description="Fixed value, variable, adjustable and other kind of resistors";
    resistorsCat.save();

    DQCategory capacitorsCat;
    capacitorsCat.name="Capacitors";
    capacitorsCat.description="All kinds of capacitors";
    capacitorsCat.save();

    DQCategory fixedResistorsCat;
    fixedResistorsCat.name="Fixed Resistors";
    fixedResistorsCat.description="Fixed value resistors";
    fixedResistorsCat.parent = resistorsCat.id;
    fixedResistorsCat.save();

    DQCategory variableResistorsCat;
    variableResistorsCat.name="Variable Resistors";
    variableResistorsCat.description="Variable resistors";
    variableResistorsCat.parent = resistorsCat.id;
    variableResistorsCat.save();

    DQCategory fixedCapsCat;
    fixedCapsCat.name="Fixed Capacitors";
    fixedCapsCat.description="Fixed value capacitors";
    fixedCapsCat.parent = capacitorsCat.id;
    fixedCapsCat.save();

    //Some attributes

    DQAttribute resistanceAttr;
    resistanceAttr.type = Models::ATTRIBUTE_RESISTANCE;
    resistanceAttr.name = "Resistance";
    resistanceAttr.description = "Ohmic value";
    resistanceAttr.save();

    DQAttribute powerAttr;
    powerAttr.type = Models::ATTRIBUTE_POWER;
    powerAttr.name = "Power";
    powerAttr.description = "Maximum power dissipation value";
    powerAttr.save();

    DQAttribute toleranceAttr;
    toleranceAttr.type = Models::ATTRIBUTE_PERCENTAGE;
    toleranceAttr.name = "Tolerance";
    toleranceAttr.description = "Resistor tolerance";
    toleranceAttr.save();

    DQPart resistor1;
    resistor1.category = fixedResistorsCat;
    resistor1.quantity = 10;
    resistor1.minimumQuantity = 5;
    resistor1.partNumber = "R303";
    resistor1.description = "Some resistor";
    resistor1.save();

    DQFloatValue resistor1Resistance;
    resistor1Resistance.attribute = resistanceAttr.id;
    resistor1Resistance.part = resistor1.id;
    resistor1Resistance.value = 3300;
    resistor1Resistance.save();

    DQFloatValue resistor1Power;
    resistor1Power.attribute = powerAttr.id;
    resistor1Power.part = resistor1.id;
    resistor1Power.value = 0.25f;
    resistor1Power.save();



    for(int i=100;i<5000;++i)
    {
        DQPart resistor2;
        resistor2.category = fixedResistorsCat;
        resistor2.quantity = i/100;
        resistor2.minimumQuantity = 0;
        resistor2.partNumber = QString("R%0").arg(i);
        resistor2.description = "Some resistor";
        resistor2.save();

        DQFloatValue resistor2Resistance;
        resistor2Resistance.attribute = resistanceAttr.id;
        resistor2Resistance.part = resistor2.id;
        resistor2Resistance.value = i;
        resistor2Resistance.save();

        DQFloatValue resistor2Power;
        resistor2Power.attribute = powerAttr.id;
        resistor2Power.part = resistor2.id;
        resistor2Power.value = 0.25f;
        resistor2Power.save();
    }

    DQCategoryActiveAttributes catAttr;
    catAttr.attribute = resistanceAttr.id;
    catAttr.category = fixedResistorsCat;
    catAttr.idx = 0;
    catAttr.save();

    DQCategoryActiveAttributes catAttr1;
    catAttr1.attribute = powerAttr.id;
    catAttr1.category = fixedResistorsCat;
    catAttr1.idx = 1;
    catAttr1.save();
}

}//namespace

#ifdef IGNORE

     PartModel model;
     model.load(fixedResistors.id.get().toInt());
     QSqlQuery query(model.tableSchemaSql());
     query.exec();
     QSqlTableModel tableModel;
     tableModel.setTable(model.tableName());
     tableModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
     if(!tableModel.select())
         qDebug("Failed to select");


     double value=500;
     for(int i=0; i<1000; ++i){
         QSqlRecord record = tableModel.record();
         record.setValue("id",QVariant());
         record.setValue("quantity",2*i);
         record.setValue("minimum_quantity", 0);
         record.setValue("resistance", value);
         record.setValue("tolerance", 10);
         record.setValue("power", 0.25);
         record.setValue("notes", QVariant(QString()));
         if(!tableModel.insertRecord(tableModel.rowCount()-1, record))
             qDebug("Failed to insert record");

         /*

         Part part;
         part.partType = fixedResistors.id;
         part.quantity = 2*i;
         part.minimumQuantity = 0;
         part.save();

         ParameterValue resistanceValue;
         resistanceValue.numericValue = value;
         resistanceValue.part = part.id;
         resistanceValue.partParameter = resistance.id;
         resistanceValue.save();

         ParameterValue resistorPower;
         resistorPower.numericValue = 0.25;
         resistorPower.part = part.id;
         resistorPower.partParameter = power.id;
         resistorPower.save();

         ParameterValue resistorNote;
         resistorNote.textValue = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur rutrum laoreet tincidunt. Curabitur elit enim, laoreet sed interdum nec, sodales gravida turpis. Pellentesque consectetur pulvinar purus.";
         resistorNote.part = part.id;
         resistorNote.partParameter = resistorNotes.id;
         resistorNote.save();
           */
         value+=500;
     }

     if(!tableModel.submitAll()){
         qDebug("Failed to submit records");
        qDebug()<<"Error:"<<tableModel.lastError().text();
     }
     qDebug()<<"Row count is "<<tableModel.rowCount();

     model.load(logicICs.id.get().toInt());
     query.exec(model.tableSchemaSql());
     tableModel.setTable(model.tableName());
     tableModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
     if(!tableModel.select())
         qDebug("Failed to select");

     QString designation("LM%1");
    for(int i=0; i<1000; ++i){
        QSqlRecord record = tableModel.record();
        record.setValue("id",QVariant());
        record.setValue("quantity",i+1);
        record.setValue("minimum_quantity", 0);
        record.setValue("designation", designation.arg(i));
        record.setValue("notes", QVariant(QString()));
        if(!tableModel.insertRecord(tableModel.rowCount()-1, record))
            qDebug("Failed to insert record");
    }
    if(!tableModel.submitAll()){
        qDebug("Failed to submit records");
       qDebug()<<"Error:"<<tableModel.lastError().text();
    }


     /*
     double value=500;
     for(int i=0; i<1000; ++i){


     double value=500;
     for(int i=0; i<1000; ++i){
         Part part;
         part.partType = fixedResistors.id;
         part.quantity = 2*i;
         part.minimumQuantity = 0;
         part.save();         

         ParameterValue resistanceValue;
         resistanceValue.numericValue = value;
         resistanceValue.part = part.id;
         resistanceValue.partParameter = resistance.id;
         resistanceValue.save();

         ParameterValue resistorPower;
         resistorPower.numericValue = 0.25;
         resistorPower.part = part.id;
         resistorPower.partParameter = power.id;
         resistorPower.save();

         ParameterValue resistorNote;
         resistorNote.textValue = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur rutrum laoreet tincidunt. Curabitur elit enim, laoreet sed interdum nec, sodales gravida turpis. Pellentesque consectetur pulvinar purus.";
         resistorNote.part = part.id;
         resistorNote.partParameter = resistorNotes.id;
         resistorNote.save();

         value+=500;
     }
     */
     return true;
}



QList<Parameter> * Database::fetchPartTypeParameters(const int partTypeId){
    QSqlQuery query;
    query.prepare("SELECT id, name,type,format, description, fixed_values, order_idx FROM part_parameters WHERE fk_partDef = :partDef ORDER BY order_idx");
    query.bindValue(":partDef",partTypeId);
    query.exec();
    QList<Parameter>  * parameters = new QList<Parameter>();
    while(query.next()){
        int paramId = query.value(0).toInt();
        QString name = query.value(1).toString();
        Parameter::ParameterType type = (Parameter::ParameterType)query.value(2).toInt();
        Parameter::FormatType format = (Parameter::FormatType)query.value(3).toInt();
        QString description = query.value(4).toString();
        bool  fixedValues = query.value(5).toBool();
        int order = query.value(6).toInt();
        Parameter param(paramId, name, description, type, format, partTypeId, fixedValues, order);
        parameters->append(param);
    }
    return parameters;
}

QList<Part> * Database::fetchParts(const int partTypeId)
{
    QElapsedTimer timer;
    timer.start();
    QSqlQuery query;
    query.prepare("SELECT id, quantity, minimum_quantity, notes FROM part WHERE fk_partDef = :partType");
    query.bindValue(":partType",partTypeId);
    query.exec();
    QSqlQuery parametersQuery;
    parametersQuery.prepare("SELECT p.type, v.id, v.fk_parameter, v.textValue, v.numericValue FROM parameters_values AS v INNER JOIN part_parameters AS p ON p.id=v.fk_parameter WHERE v.fk_part = :part");
    QList<Part>  * parts = new QList<Part>();
    while(query.next()){
        int partId = query.value(0).toInt();
        int quantity = query.value(1).toInt();
        int minimumQuantity = query.value(2).toInt();
        QString notes = query.value(3).toString();
        Part part(partId, quantity, minimumQuantity, notes);        
        parts->append(part);
        /*
        parametersQuery.bindValue(0,partId);
        parametersQuery.exec();
        while(parametersQuery.next()){
            Parameter::ParameterType type = (Parameter::ParameterType)parametersQuery.value(0).toInt();
            int valueId = parametersQuery.value(1).toInt();
            int paramTypeId = parametersQuery.value(2).toInt();
            switch(type){
            case Parameter::Number: {
                double value = parametersQuery.value(4).toDouble();
                PartValue partValue(valueId, value);
                part.setParamValue(paramTypeId, partValue);
                }
                break;
            case Parameter::Text: {
                QString value = parametersQuery.value(3).toString();
                PartValue partValue(valueId, value);
                part.setParamValue(paramTypeId, partValue);
                }
                break;
            }
        }
        */
    }
    qDebug() << "Parts loading took" << timer.elapsed();
    timer.restart();
    QList<Part>::iterator partsIt;
    for (partsIt = parts->begin(); partsIt != parts->end(); ++partsIt){
        parametersQuery.bindValue(0,partsIt->id());
        parametersQuery.exec();
        while(parametersQuery.next()){
            Parameter::ParameterType type = (Parameter::ParameterType)parametersQuery.value(0).toInt();
            int valueId = parametersQuery.value(1).toInt();
            int paramTypeId = parametersQuery.value(2).toInt();
            switch(type){
            case Parameter::Number: {
                double value = parametersQuery.value(4).toDouble();
                PartValue partValue(valueId, value);
                partsIt->setParamValue(paramTypeId, partValue);
                }
                break;
            case Parameter::Text: {
                QString value = parametersQuery.value(3).toString();
                PartValue partValue(valueId, value);
                partsIt->setParamValue(paramTypeId, partValue);
                }
                break;
            }
        }
    }
    qDebug() << "Parts values loading took" << timer.elapsed();
    return parts;
}
#endif

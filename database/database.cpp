
#include "database.h"
#include <QtSql>
#include <QtGui>
#include <dquest.h>
#include "category.h"
#include "parttype.h"
#include "partparameter.h"
#include "part.h"
#include "parametervalue.h"
#include "partmodel.h"

using namespace EParts;

Database::Database()
{   
}

QString Database::makeTableName(const QString& partName)
{
    return partName.trimmed().replace(' ',"_").toLower();
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

     connection.addModel<Category>(); // Register a model to the connection
     connection.addModel<PartType>();
     connection.addModel<PartParameter>();
     connection.addModel<Part>();
     connection.addModel<ParameterValue>();

     connection.createTables(); // Create table for all added model

     DQList<Category> initialCategories;
     DQListWriter categoryWriter(&initialCategories);
     categoryWriter << "Resistors" << "Fixed value, variable, adjustable and other kind of resistors"
            << "Capacitors" << "All kinds of capacitors"
            << "ICs" << "Integrated Circuits";
     initialCategories.save();

     Category resistorsCat;
     resistorsCat.load(DQWhere("name") == "Resistors");

     Category capacitorsCat;
     capacitorsCat.load(DQWhere("name") == "Capacitors");

     Category icCat;
     icCat.load(DQWhere("name") == "ICs");

     PartType fixedResistors;
     fixedResistors.name = "Fixed Resistors";
     fixedResistors.partTableName = makeTableName("Fixed Resistors");
     fixedResistors.description = "Fixed value resistors";
     fixedResistors.category = resistorsCat;
     fixedResistors.save();

     PartType adjustableResistors;
     adjustableResistors.name = "Adjustable Resistors";
     adjustableResistors.partTableName = makeTableName("Adjustable Resistors");
     adjustableResistors.description = "Adjustable value resistors";
     adjustableResistors.category = resistorsCat;
     adjustableResistors.save();

     PartType variableResistors;
     variableResistors.name = "Variable Resistors";
     variableResistors.partTableName = makeTableName("Variable Resistors");
     variableResistors.description = "Variable value resistors";
     variableResistors.category = resistorsCat;
     variableResistors.save();

     PartType fixedCapacitors;
     fixedCapacitors.name = "Fixed Capacitors";
     fixedCapacitors.partTableName = makeTableName("Fixed Capacitors");
     fixedCapacitors.description = "Fixed value capacitors";
     fixedCapacitors.category = capacitorsCat;
     fixedCapacitors.save();

     PartType adjustableCapacitors;
     adjustableCapacitors.name = "Adjustable Capacitors";
     adjustableCapacitors.partTableName = makeTableName("Adjustable Capacitors");
     adjustableCapacitors.description = "Adjustable value capacitors";
     adjustableCapacitors.category = capacitorsCat;
     adjustableCapacitors.save();

     PartType logicICs;
     logicICs.name = "Digital";
     logicICs.partTableName = makeTableName("Digital");
     logicICs.description = "Digital ICs";
     logicICs.category = icCat;
     logicICs.save();

     PartType analogICs;
     analogICs.name = "Analog";
     analogICs.partTableName = makeTableName("Analog");
     analogICs.description = "Analog ICs";
     analogICs.category = icCat;
     analogICs.save();

     PartParameter resistance;
     resistance.name = "Resistance";
     resistance.columnName = makeTableName("Resistance");
     resistance.description = "Ohmic value";
     resistance.type = PartParameter::Resistance;
     resistance.partType = fixedResistors.id;
     resistance.fixedValues = false;
     resistance.orderIndex = 1;
     resistance.save();

     PartParameter power;
     power.name = "Power";
     power.columnName = makeTableName("Power");
     power.description = "Maximum power dissipation value";
     power.type = PartParameter::Power;
     power.partType = fixedResistors.id;
     power.fixedValues = false;
     power.orderIndex = 2;
     power.save();

     PartParameter tolerance;
     tolerance.name = "Tolerance";
     tolerance.columnName = makeTableName("Tolerance");
     tolerance.description = "Resistor tolerance";
     tolerance.type = PartParameter::Percentage;
     tolerance.partType = fixedResistors.id;
     tolerance.fixedValues = false;
     tolerance.orderIndex = 3;
     tolerance.save();

     PartParameter resistorNotes;
     resistorNotes.name = "Notes";
     resistorNotes.columnName = makeTableName("Notes");
     resistorNotes.description = "Resistor notes";
     resistorNotes.type = PartParameter::LongText;
     resistorNotes.partType = fixedResistors.id;
     resistorNotes.fixedValues = false;
     resistorNotes.orderIndex = 4;
     resistorNotes.save();

     PartParameter logicICDesignation;
     logicICDesignation.name="Designation";
     logicICDesignation.columnName = makeTableName("Designation");
     logicICDesignation.description = "Resistor notes";
     logicICDesignation.type = PartParameter::Text;
     logicICDesignation.partType = logicICs.id;
     logicICDesignation.fixedValues = false;
     logicICDesignation.orderIndex = 1;
     logicICDesignation.save();

     PartParameter logicICNotes;
     logicICNotes.name = "Notes";
     logicICNotes.columnName = makeTableName("Notes");
     logicICNotes.description = "Resistor notes";
     logicICNotes.type = PartParameter::LongText;
     logicICNotes.partType = logicICs.id;
     logicICNotes.fixedValues = false;
     logicICNotes.orderIndex = 2;
     logicICNotes.save();

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

#ifdef IGNORE

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
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QtGui>
#include "types.h"

static void createCategoryTable()
{
    QSqlQuery query;
    query.exec("create table category (id int primary key, "
               "name varchar(32) not null, description varchar(64))");    
}

static void createPartDefinitionTable()
{
    QSqlQuery query;
    query.exec("CREATE TABLE part_definition (id INTEGER PRIMARY KEY, "
               "name VARCHAR(32), description VARCHAR(64), fk_category INTEGER,"
               "FOREIGN KEY(fk_category) REFERENCES category(id))");    
}

static void createParametersTable()
{
    QSqlQuery query;
    query.exec("CREATE TABLE part_parameters( "
               "id INTEGER PRIMARY KEY, "
               "name VARCHAR(64) NOT NULL, "
               "type INTEGER NOT NULL,"
               "format INTEGER NOT NULL,"
               "description VARCHAR(128), "
               "fk_partDef INTEGER NOT NULL,"
               "fixed_values BOOLEAN NOT NULL, "
               "order_idx INTEGER NOT NULL, "
               "FOREIGN KEY(fk_partDef) REFERENCES part_definition(id))");
}

static void createPartsTable()
{
    QSqlQuery query;
    query.exec("CREATE TABLE part( "
               "id INTEGER PRIMARY KEY, "
               "fk_partDef INTEGER NOT NULL,"
               "quantity INTEGER NOT NULL,"
               "minimum_quantity INTEGER NOT NULL,"
               "notes VARCHAR(256), "
               "FOREIGN KEY(fk_partDef) REFERENCES part_definition(id))");

}

static void createParametersValuesTable()
{
    QSqlQuery query;
    query.exec("CREATE TABLE parameters_values( "
               "id INTEGER PRIMARY KEY, "
               "fk_part INTEGER NOT NULL,"
               "fk_parameter INTEGER NOT NULL,"
               "textValue VARCHAR(128) NULL, "
               "numericValue NUMERIC NULL, "
               "FOREIGN KEY(fk_part) REFERENCES part_definition(id),"
               "FOREIGN KEY(fk_parameter) REFERENCES part_parameters(id))");
}

/*
static void addParameter(const int id, const QString& name, const ParameterType type, const enum FormatType format, const QString& description, int partId, bool isFixedValue){
    QSqlQuery query;
    query.prepare("INSERT INTO part_parameters VALUES (:id, :name, :type, :description, :partId, :isFixedValue)");
    query.bindValue(":id",id);
    query.bindValue(":name",name);
    query.bindValue(":type",type);
    query.bindValue(":format",format);
    query.bindValue(":description",description);
    query.bindValue(":partId",partId);
    query.bindValue(":isFixedValue",isFixedValue);
    query.exec();
}

static void addPartDef(const int id, const QString & name, const QString& description, const int categoryId)
{
    QSqlQuery query;
    query.prepare("INSERT INTO part_definition VALUES (:id, :name, :description, :categoryId )");
    query.bindValue(":id",id);
    query.bindValue(":name",name);
    query.bindValue(":description",description);
    query.bindValue(":categoryId",categoryId);
    query.exec();
}

static void addCategory(const int id, const QString & name, const QString& description)
{
    QSqlQuery query;
    query.prepare("INSERT INTO category VALUES (:id, :name, :description)");
    query.bindValue(":id",id);
    query.bindValue(":name",name);
    query.bindValue(":description",description);
    query.exec();
}
*/

static void addCategory(const EParts::Category & category){
    QSqlQuery query;
    query.prepare("INSERT INTO category VALUES (:id, :name, :description)");
    query.bindValue(":id",category.id());
    query.bindValue(":name",category.name());
    query.bindValue(":description",category.description());
    query.exec();
}

static void addPartDefinition(const EParts::PartDefinition &partDefinition)
{
    QSqlQuery query;
    query.prepare("INSERT INTO part_definition VALUES (:id, :name, :description, :categoryId )");
    query.bindValue(":id",partDefinition.id());
    query.bindValue(":name",partDefinition.name());
    query.bindValue(":description",partDefinition.description());
    query.bindValue(":categoryId",partDefinition.categoryId());
    query.exec();
}

static void addParameter(const EParts::Parameter &parameter){
    QSqlQuery query;
    query.prepare("INSERT INTO part_parameters VALUES (:id, :name, :type, :format, :description, :partId, :isFixedValue, :orderIdx)");
    query.bindValue(":id",parameter.id());
    query.bindValue(":name",parameter.name());
    query.bindValue(":type",parameter.type());
    query.bindValue(":format",parameter.format());
    query.bindValue(":description",parameter.description());
    query.bindValue(":partId",parameter.partId());
    query.bindValue(":isFixedValue",parameter.isFixedValue());
    query.bindValue(":orderIdx",parameter.order());
    query.exec();
}

static void addPart(const int partId, const int partTypeId, const int quantity){
    QSqlQuery query;
    query.prepare("INSERT INTO part (id, fk_partDef, quantity, minimum_quantity)VALUES (:id,:partType,:quantity, 0)");
    query.bindValue(":id",partId);
    query.bindValue(":partType",partTypeId);
    query.bindValue(":quantity",quantity);
    query.exec();
}

static void addParameterValue(const int id, const int partId, const int paramId, const double value){
    QSqlQuery query;
    query.prepare("INSERT INTO parameters_values (id, fk_part, fk_parameter, numericValue) VALUES (:id,:partId,:paramId,:value)");
    query.bindValue(":id",id);
    query.bindValue(":partId",partId);
    query.bindValue(":paramId",paramId);
    query.bindValue(":value",value);
    query.exec();
}

static void addParameterValue(const int id, const int partId, const int paramId, const QString & value){
    QSqlQuery query;
    query.prepare("INSERT INTO parameters_values (id, fk_part, fk_parameter, textValue) VALUES (:id,:partId,:paramId,:value)");
    query.bindValue(":id",id);
    query.bindValue(":partId",partId);
    query.bindValue(":paramId",paramId);
    query.bindValue(":value",value);
    query.exec();
}


static void generateMockData(){
    const int RESISTORS_CAT_ID = 1;
    const int CAPACITORS_CAT_ID = 2;
    const int IC_CAT_ID = 3;

    const int FIXED_RESISTORS_ID = 1;
    const int ADJUSTABLE_RESISTORS_ID = 2;
    const int VARIABLE_RESISTORS_ID = 3;
    const int FIXED_CAPACITORS_ID = 4;
    const int ADJUSTABLE_CAPACITORS_ID = 5;
    const int LOGIC_IC_ID = 6;
    const int ANALOGIC_IC_ID = 7;

    EParts::Category resistorsCategory(RESISTORS_CAT_ID, "Resistors", "Fixed value, variable, adjustable and other kind of resistors");
    EParts::Category capacitorsCategory(CAPACITORS_CAT_ID, "Capacitors", "All kinds of capacitors");
    EParts::Category icCategory(IC_CAT_ID, "ICs", "Integrated Circuits");

    EParts::PartDefinition fixedResistors(FIXED_RESISTORS_ID, "Fixed Resistors", "Fixed value resistors", resistorsCategory.id());
    EParts::PartDefinition adjustableResistors(ADJUSTABLE_RESISTORS_ID, "Adjustable Resistors", "Adjustable value resistors", resistorsCategory.id());
    EParts::PartDefinition variableResistors(VARIABLE_RESISTORS_ID, "Variable Resistors", "Variable value resistors", resistorsCategory.id());

    EParts::PartDefinition fixedCapacitors(FIXED_CAPACITORS_ID, "Fixed Capacitors", "Fixed value capacitors", capacitorsCategory.id());
    EParts::PartDefinition adjustableCapacitors(ADJUSTABLE_CAPACITORS_ID, "Adjustable Capacitors", "Adjustable value capacitors", capacitorsCategory.id());

    EParts::PartDefinition logicICs(LOGIC_IC_ID, "Logic", "Logic ICs", icCategory.id());
    EParts::PartDefinition analogicICs(ANALOGIC_IC_ID, "Analogic", "Analogic ICs", icCategory.id());

    addCategory(resistorsCategory);
    addCategory(capacitorsCategory);
    addCategory(icCategory);

    addPartDefinition(fixedResistors);
    addPartDefinition(adjustableResistors);
    addPartDefinition(variableResistors);

    addPartDefinition(fixedCapacitors);
    addPartDefinition(adjustableCapacitors);

    addPartDefinition(logicICs);
    addPartDefinition(analogicICs);
    /*
    addCategory(RESISTORS_CAT_ID, "Resistors", "Fixed value, variable, adjustable and other kind of resistors");
    addCategory(CAPACITORS_CAT_ID, "Capacitors", "All kinds of capacitors");
    addCategory(IC_CAT_ID, "ICs", "Integrated Circuits");

    addPartDef(FIXED_RESISTORS_ID, "Fixed Resistors", "Fixed value resistors", RESISTORS_CAT_ID);
    addPartDef(ADJUSTABLE_RESISTORS_ID, "Adjustable Resistors", "Adjustable value resistors", RESISTORS_CAT_ID);
    addPartDef(VARIABLE_RESISTORS_ID, "Variable Resistors", "Variable value resistors", RESISTORS_CAT_ID);

    addPartDef(FIXED_CAPACITORS_ID, "Fixed Capacitors", "Fixed value capacitors", CAPACITORS_CAT_ID);
    addPartDef(ADJUSTABLE_CAPACITORS_ID, "Adjustable Capacitors", "Adjustable value capacitors", CAPACITORS_CAT_ID);

    addPartDef(LOGIC_IC_ID, "Logic", "Logic ICs", IC_CAT_ID);
    addPartDef(ANALOGIC_IC_ID, "Analogic", "Analogic ICs", IC_CAT_ID);
    */

    EParts::Parameter resistance(1, "Resistance", "Ohmic value", EParts::Parameter::Number, EParts::Parameter::Resistance, fixedResistors.id(), false, 1);
    EParts::Parameter power(2, "Power", "Maximum  power dissipation value", EParts::Parameter::Number, EParts::Parameter::Power, fixedResistors.id(), false, 2);
    addParameter(resistance);
    addParameter(power);

    int paramValueId = 0;
    for(int part_id=0; part_id<5000; ++part_id){
        addPart(part_id,FIXED_RESISTORS_ID, 2*part_id);
        addParameterValue(paramValueId++,part_id,resistance.id(),part_id*1000);
        addParameterValue(paramValueId++,part_id,power.id(),0.25);
    }
    /*
    const int part1_id = 4;
    const int part2_id = 5;
    addPart(part1_id,fixedResistors.id(),10);
    addPart(part2_id,fixedResistors.id(),20);

    int id=1;
    addParameterValue(id++,part1_id,resistance.id(),1000);
    addParameterValue(id++,part1_id,power.id(),0.25);

    addParameterValue(id++,part2_id,resistance.id(),120);
    addParameterValue(id++,part2_id,power.id(),0.25);
*/
}

static bool createConnection()
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
     createCategoryTable();
     createPartDefinitionTable();
     createParametersTable();
     createParametersValuesTable();
     createPartsTable();
     generateMockData();
     return true;
}



#endif // CONNECTION_H

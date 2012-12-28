
CREATE TABLE IF NOT EXISTS category (
    id INTEGER PRIMARY KEY,
    parent INTEGER,
    name VARCHAR,
    description VARCHAR
);

CREATE TABLE IF NOT EXISTS part (
    id INTEGER PRIMARY KEY,
    cat_id INTEGER,
    quantity INTEGER,
    minimum_quantity INTEGER,
    part_number VARCHAR,
    description VARCHAR,
    FOREIGN KEY(cat_id) REFERENCES category(id)
);

CREATE TABLE IF NOT EXISTS attribute (
    id INTEGER PRIMARY KEY,
    type INTEGER,
    name VARCHAR,
    description VARCHAR
);

CREATE TABLE IF NOT EXISTS float_value (
    part_id INTEGER,
    attr_id INTEGER,
    cat_id INTEGER,
    value DOUBLE,
    FOREIGN KEY(attr_id) REFERENCES attribute(id)
    FOREIGN KEY(part_id) REFERENCES part(id)
    FOREIGN KEY(cat_id) REFERENCES category(id)
);

CREATE TABLE IF NOT EXISTS text_value (
    part_id INTEGER,
    attr_id INTEGER,
    cat_id INTEGER,
    value VARCHAR,
    FOREIGN KEY(attr_id) REFERENCES attribute(id)
    FOREIGN KEY(part_id) REFERENCES part(id)
    FOREIGN KEY(cat_id) REFERENCES category(id)
);

CREATE TABLE IF NOT EXISTS category_active_attributes (
    cat_id INTEGER,
    attr_id INTEGER,
    idx INTEGER,
    FOREIGN KEY(cat_id) REFERENCES category(id),
    FOREIGN KEY(attr_id) REFERENCES attribute(id)
);

INSERT INTO category (id, name,description) VALUES (1, 'Fixed resistors', 'Fixed resistors');
INSERT INTO category (id, name,description) VALUES (2, 'Fixed capacitors', 'Fixed capacitors');

INSERT INTO attribute (id, type,name) VALUES (1,1,'Resistance');
INSERT INTO attribute (id,type,name) VALUES (2,2,'Capacitance');
INSERT INTO attribute (id,type,name) VALUES (3,4,'Power');

INSERT INTO part (id,cat_id,quantity) VALUES(1,1,10);
INSERT INTO float_value (part_id,attr_id,cat_id,value) VALUES(1,1,1,300);
INSERT INTO float_value (part_id,attr_id,cat_id,value) VALUES(1,3,1,0.25);

INSERT INTO part (id,cat_id,quantity) VALUES(2,2,5);

INSERT INTO part (id,cat_id,quantity) VALUES(3,1,10);
INSERT INTO float_value (part_id,attr_id,cat_id,value) VALUES(3,1,1,330);
INSERT INTO float_value (part_id,attr_id,cat_id,value) VALUES(3,3,1,0.25);

INSERT INTO category_active_attributes VALUES (1,1,1);
INSERT INTO category_active_attributes VALUES (1,3,1);

SELECT a.id, a.type, a.name FROM attribute a INNER JOIN category_active_attributes c WHERE c.cat_id=1 AND a.id=c.attr_id ORDER BY c.idx;

SELECT p.id, p.quantity, p.minimum_quantity,p.part_number,p.description FROM part p WHERE p.cat_id=1 ORDER BY p.part_number ASC;
SELECT p.id,p.quantity, v.value FROM part p INNER JOIN float_value v WHERE p.cat_id=1 AND v.part_id=p.id AND v.attr_id=1 ORDER BY v.value ASC;

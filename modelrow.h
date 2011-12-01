#ifndef MODELROW_H
#define MODELROW_H

template <class T>
class ModelRow {
public:
    enum RowState {New,Modified,Default};
    ModelRow(T item) ;
    T item;
    RowState state;
};

template <class T>
ModelRow<T>::ModelRow(T item) : item(item), state(Default) {}

#endif // MODELROW_H

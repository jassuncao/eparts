#ifndef CRITERIA_H
#define CRITERIA_H
#include <QVariant>

class Criterion
{
public:    
    virtual bool accepts(QVariant value) const = 0;
    inline int column() const {return m_column;}
protected:
    Criterion(int column);
    int m_column;
};

class NumericCriterion : public Criterion
{
protected:
    NumericCriterion(int column, double argument);
    const double m_argument;
};

class EqualNumericCriterion : public NumericCriterion
{
public:
    EqualNumericCriterion(int column, double argument);
    bool accepts(QVariant value) const;
};

class NotEqualNumericCriterion : public NumericCriterion
{
public:
    NotEqualNumericCriterion(int column, double argument);
    bool accepts(QVariant value) const;
};

class LessOrEqualCriterion : public NumericCriterion
{
public:
    LessOrEqualCriterion(int column, double argument);
    bool accepts(QVariant value) const;
};

class GreaterOrEqualCriterion : public NumericCriterion
{
public:
    GreaterOrEqualCriterion(int column, double argument);
    bool accepts(QVariant value) const;
};

class BetweenCriterion : public NumericCriterion
{
public:
    BetweenCriterion(int column, double low, double high);
    bool accepts(QVariant value) const;
private:
    double m_argument2;
};

class ContainsCriterion : public Criterion
{
public:
    ContainsCriterion(int column, const QString &argument);
    bool accepts(QVariant value) const;
protected:
    const QString m_argument;
};

class NotContainsCriterion : public ContainsCriterion
{
public:
    NotContainsCriterion(int column, const QString &argument);
    bool accepts(QVariant value) const;
};


#endif // CRITERIA_H

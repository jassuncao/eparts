#include "criteria.h"

Criterion::Criterion(int column) :
    m_column(column)
{
}


NumericCriterion::NumericCriterion(int column, double argument) :
    Criterion(column),
    m_argument(argument)
{
}


EqualNumericCriterion::EqualNumericCriterion(int column, double argument) :
    NumericCriterion(column, argument)
{
}

bool EqualNumericCriterion::accepts(QVariant value) const
{    
    return qFuzzyCompare(m_argument, value.toReal());
}


NotEqualNumericCriterion::NotEqualNumericCriterion(int column, double argument) :
    NumericCriterion(column, argument)
{
}

bool NotEqualNumericCriterion::accepts(QVariant value) const
{
    return !qFuzzyCompare(m_argument, value.toReal());
}


LessOrEqualCriterion::LessOrEqualCriterion(int column, double argument) :
    NumericCriterion(column, argument)
{
}

bool LessOrEqualCriterion::accepts(QVariant value) const
{
    return m_argument<=value.toReal();
}


GreaterOrEqualCriterion::GreaterOrEqualCriterion(int column, double argument) :
    NumericCriterion(column, argument)
{
}

bool GreaterOrEqualCriterion::accepts(QVariant value)  const
{
    return m_argument>=value.toReal();
}


BetweenCriterion::BetweenCriterion(int column, double low, double high) :
    NumericCriterion(column, low),
    m_argument2(high)
{
}

bool BetweenCriterion::accepts(QVariant value) const
{
    qreal v = value.toDouble();
    return v>m_argument && v<m_argument2;
}


ContainsCriterion::ContainsCriterion(int column, const QString &argument) :
    Criterion(column),
    m_argument(argument)
{
}

bool ContainsCriterion::accepts(QVariant value) const
{
    return value.toString().contains(m_argument,Qt::CaseInsensitive);
}


NotContainsCriterion::NotContainsCriterion(int column, const QString &argument) :
    ContainsCriterion(column, argument)
{
}

bool NotContainsCriterion::accepts(QVariant value) const
{
    return !ContainsCriterion::accepts(value);
}

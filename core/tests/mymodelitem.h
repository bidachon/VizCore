#ifndef MYMODELITEM_H
#define MYMODELITEM_H

#include "modelitem.h"

class MyPropertyList : public PropertyList
{
public:
    MyPropertyList(int i, bool b, QString str):PropertyList()
    {
        m_myInt = i;
        m_myBool = b;
        m_myQString = str;
    }

    MyPropertyList():PropertyList()
    {
        setDefault();
    }

    void setDefault(){
        m_myInt = 12;
        m_myBool = false;
        m_myQString = "teststr";
    }

    ~MyPropertyList()
    {
    }

    int myIntProperty() const {return m_myInt;}
    bool myBoolProperty() const {return m_myBool;}
    QString myStringProperty() const {return m_myQString;}

private:
    int m_myInt;
    bool m_myBool;
    QString m_myQString;

};

class MyModelItem : public ModelItem
{
    Q_OBJECT
public:
    MyModelItem()
    {

    }
    ~MyModelItem(){}

    void emitupdate()
    {
        this->events().broadcastUpdate();
    }

    QSharedPointer<PropertyList> defaultProperties() const
    {
        PropertyList *prop = new MyPropertyList();
        return QSharedPointer<PropertyList>(prop);
    }

signals:
    void testDoParentUpdated();
    void testDoParentPositionUpdated(const PositionUpdate &pu);
    void testDoParentVisibilityChanged(bool v);
    void testDoParentPropertyUpdated(QSharedPointer<PropertyList> prop);

protected slots:
    virtual void doParentUpdated() {emit testDoParentUpdated();}
    virtual void doParentPositionUpdated(const PositionUpdate &pu) {emit testDoParentPositionUpdated(pu);}
    virtual void doParentVisibilityChanged(bool v) {emit testDoParentVisibilityChanged(v);}
    virtual void doParentPropertyUpdated(QSharedPointer<PropertyList> prop) {emit testDoParentPropertyUpdated(prop);}

};


#endif // MYMODELITEM_H

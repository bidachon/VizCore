#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

#include "controlleritem.h"
#include "IViewEventObserver.h"

class MyController: public ControllerItem, public IViewEventObserver
{
    Q_OBJECT
public:
    MyController(){}

    virtual void doUpdated() {emit sigDoUpdated();}
    virtual void doPositionUpdated(const PositionUpdate &pu) {emit sigDoPositionUpdated(pu);}
    virtual void doVisibilityChanged(bool v) {emit sigDoVisibilityChanged(v);}
    virtual void doPropertyUpdated(QSharedPointer<PropertyList> prop) {emit sigDoPropertyUpdated(prop);}
    virtual void doSelectionChanged(bool s) {emit sigDoSelectionChanged(s);}
    virtual void doIsSelectableChanged(bool s) {emit sigDoIsSelectableChanged(s);}

    virtual void doModelUpdated() {emit sigDoModelUpdated();}
    virtual void doViewUpdated() {emit sigDoViewUpdated();}
    virtual void setEnabled(bool) {}
    virtual void cellPickingEvent(vtkSmartPointer<vtkProp3D>, vtkSmartPointer<vtkCellPicker>) {
        emit sigDoCellPickingEvent();
    }
    virtual void panEvent(vtkSmartPointer<vtkProp3D>, vtkSmartPointer<vtkCellPicker>) {}
    virtual void mouseMoveEvent(vtkSmartPointer<vtkProp3D>, vtkSmartPointer<vtkCellPicker>){}

    signals:
    void sigDoUpdated();
    void sigDoPositionUpdated(const PositionUpdate &pu);
    void sigDoVisibilityChanged(bool v);
    void sigDoPropertyUpdated(QSharedPointer<PropertyList> prop);
    void sigDoSelectionChanged(bool s);
    void sigDoIsSelectableChanged(bool s);
    void sigDoModelUpdated();
    void sigDoViewUpdated();
    void sigDoCellPickingEvent();

};

#endif // MYCONTROLLER_H

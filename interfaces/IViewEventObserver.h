#ifndef IVIEWEVENTOBSERVER_H
#define IVIEWEVENTOBSERVER_H

#include <vtkActor.h>
#include <vtkCellPicker.h>
#include <vtkSmartPointer.h>
#include <QUuid>

class IViewEventObserver
{
public:

    IViewEventObserver() {
        m_obsUid = QUuid::createUuid();
    }

    virtual ~IViewEventObserver(){}

    QUuid id() const {return m_obsUid;}

    virtual void cellPickingEvent(vtkSmartPointer<vtkProp3D>, vtkSmartPointer<vtkCellPicker>) = 0;
    virtual void panEvent(vtkSmartPointer<vtkProp3D>, vtkSmartPointer<vtkCellPicker>) = 0;
    virtual void mouseMoveEvent(vtkSmartPointer<vtkProp3D>, vtkSmartPointer<vtkCellPicker>) = 0;

private:
    QUuid m_obsUid;
};


#endif // IVIEWEVENTOBSERVER_H

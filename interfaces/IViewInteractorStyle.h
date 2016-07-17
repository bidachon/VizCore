#ifndef IVIEWINTERACTORSTYLE_H
#define IVIEWINTERACTORSTYLE_H
#include <QSharedPointer>
#include <vtkInteractorStyle.h>

#include <vtkCellPicker.h>
#include <vtkSmartPointer.h>

class QUuid;
class ViewItem;
class IViewEventObserver;

class IInteractorPropSubscriber
{
public:

    enum class EventType {
        PICK = 0,
        PAN,
        MOUSEMOVE
    };

    virtual ~IInteractorPropSubscriber() {}


    virtual void addEventObserver(vtkSmartPointer<vtkProp3D> actor, QSharedPointer<ViewItem> obs) = 0;
    virtual void addEventObserver(QSharedPointer<ViewItem> obs) = 0;
    virtual void addEventObserver(IViewEventObserver *obs, QUuid id) = 0;
    virtual void removeObserver(QSharedPointer<ViewItem> obs) = 0;
    virtual void removeEventObserver(IViewEventObserver *obs) = 0;
    virtual void broadcastEvent(vtkProp3D *actor, vtkSmartPointer<vtkCellPicker> picker, EventType e) = 0;

};

class IViewInteractorStyle
{
public:
    virtual ~IViewInteractorStyle(){}

    virtual vtkSmartPointer<vtkInteractorStyle> interactorStyle() const = 0;
    virtual void setInteractorPropSubscriber(QSharedPointer<IInteractorPropSubscriber>) = 0;
};

#endif // IVIEWINTERACTORSTYLE_H

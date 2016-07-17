#ifndef GENERICVIEWINTERACTORSTYLE_H
#define GENERICVIEWINTERACTORSTYLE_H

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCellPicker.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

//#include "IViewEventObserver.h"
#include "IViewInteractorStyle.h"

class PrivatePickingInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static PrivatePickingInteractorStyle* New();

    vtkTypeMacro(PrivatePickingInteractorStyle, vtkInteractorStyleTrackballCamera)
    PrivatePickingInteractorStyle();
    virtual ~PrivatePickingInteractorStyle();

    void setInteractorPropSubscriber(QSharedPointer<IInteractorPropSubscriber> subscriber)
    {
        m_subscriber = subscriber;
    }

    virtual void OnLeftButtonDown();
    virtual void OnLeftButtonUp();
    virtual void OnMouseMove();

private:
    QSharedPointer<IInteractorPropSubscriber> m_subscriber;
    bool m_mouseDown;
    vtkSmartPointer<vtkCellPicker>  m_cellPicker;
};

class PickingInteractorStyle: public IViewInteractorStyle
{

public:

    PickingInteractorStyle()
    {
        m_interactorSrtyle = vtkSmartPointer<PrivatePickingInteractorStyle>::New();
    }

    virtual ~PickingInteractorStyle(){}

    vtkSmartPointer<vtkInteractorStyle> interactorStyle() const
    {
        return m_interactorSrtyle;
    }


    void setInteractorPropSubscriber(QSharedPointer<IInteractorPropSubscriber> subscriber)
    {
        m_subscriber = subscriber;
        m_interactorSrtyle->setInteractorPropSubscriber(subscriber);
    }


private:
    QSharedPointer<IInteractorPropSubscriber> m_subscriber;
    vtkSmartPointer<PrivatePickingInteractorStyle> m_interactorSrtyle;


};

#endif // GENERICVIEWINTERACTORSTYLE_H

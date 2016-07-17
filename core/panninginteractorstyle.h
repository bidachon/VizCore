#ifndef VIEWITEMINTERACTORSTYLE_H
#define VIEWITEMINTERACTORSTYLE_H

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPropPicker.h>
#include <vtkCellPicker.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include "IViewInteractorStyle.h"
#include <vtkInteractorStyleImage.h>
#include <QMap>

class PrivatePanningInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:

    static PrivatePanningInteractorStyle* New();

    vtkTypeMacro(PrivatePanningInteractorStyle, vtkInteractorStyleTrackballCamera)
    PrivatePanningInteractorStyle();
    virtual ~PrivatePanningInteractorStyle();

    //desactivate everything but Panning
    virtual void Rotate() {}
    virtual void Spin(){}
    //virtual void Dolly(){}
    virtual void UniformScale(){}
    virtual void OnLeftButtonDown();
    virtual void OnLeftButtonUp();
    virtual void OnMouseMove();

    //virtual void Pan();

    virtual void setInteractorPropSubscriber(QSharedPointer<IInteractorPropSubscriber> subscriber)
    {
        m_subscriber = subscriber;
    }


private:

    QSharedPointer<IInteractorPropSubscriber> m_subscriber;
    vtkSmartPointer<vtkCellPicker>  m_cellPicker;
    bool m_mouseDown;


};

class PanningInteractorStyle: public IViewInteractorStyle
{

public:

    PanningInteractorStyle(){
        m_interactorStyle = vtkSmartPointer<PrivatePanningInteractorStyle>::New();
    }

    virtual ~PanningInteractorStyle() {}

    vtkSmartPointer<vtkInteractorStyle> interactorStyle() const
    {
        return m_interactorStyle;
    }


    void setInteractorPropSubscriber(QSharedPointer<IInteractorPropSubscriber> subscriber)
    {
        m_subscriber = subscriber;
        m_interactorStyle->setInteractorPropSubscriber(subscriber);
    }


private:
    QSharedPointer<IInteractorPropSubscriber> m_subscriber;
    vtkSmartPointer<PrivatePanningInteractorStyle> m_interactorStyle;


};

#endif // VIEWITEMINTERACTORSTYLE_H

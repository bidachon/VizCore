#include "panninginteractorstyle.h"
#include <vtkProp3D.h>
#include <QDebug>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

vtkStandardNewMacro(PrivatePanningInteractorStyle);

PrivatePanningInteractorStyle::PrivatePanningInteractorStyle():vtkInteractorStyleTrackballCamera()
{
    m_cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    this->AutoAdjustCameraClippingRangeOn();

}

PrivatePanningInteractorStyle::~PrivatePanningInteractorStyle()
{
}

void PrivatePanningInteractorStyle::OnLeftButtonDown()
{
    m_mouseDown = true;
    int* clickPos = this->GetInteractor()->GetEventPosition();
    // Pick from this location.

    m_cellPicker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
    vtkProp3D *actor = m_cellPicker->GetProp3D();

    m_subscriber->broadcastEvent(actor,m_cellPicker,IInteractorPropSubscriber::EventType::PICK);


    // Forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void PrivatePanningInteractorStyle::OnLeftButtonUp()
{
    m_mouseDown = false;
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void PrivatePanningInteractorStyle::OnMouseMove()
{
        int* mousePos = this->GetInteractor()->GetEventPosition();
        m_cellPicker->Pick(mousePos[0], mousePos[1], 0, this->GetDefaultRenderer());
        vtkProp3D *actor = m_cellPicker->GetProp3D();
        m_subscriber->broadcastEvent(actor,m_cellPicker,IInteractorPropSubscriber::EventType::MOUSEMOVE);
        vtkInteractorStyleTrackballCamera::OnMouseMove();
}

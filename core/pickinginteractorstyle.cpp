#include "pickinginteractorstyle.h"
#include <vtkObjectFactory.h>
#include <vtkProp3D.h>

vtkStandardNewMacro(PrivatePickingInteractorStyle);

PrivatePickingInteractorStyle::PrivatePickingInteractorStyle()
{
    m_cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    m_mouseDown = false;
}

PrivatePickingInteractorStyle::~PrivatePickingInteractorStyle()
{
}

void PrivatePickingInteractorStyle::OnLeftButtonDown()
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

void PrivatePickingInteractorStyle::OnLeftButtonUp()
{
    m_mouseDown = false;
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void PrivatePickingInteractorStyle::OnMouseMove()
{
    vtkInteractorStyleTrackballCamera::OnMouseMove();
}



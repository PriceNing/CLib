#include "EventQtSlotConnect.h"

#include "vtkGenericOpenGLRenderWindow.h"
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSphereSource.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <QMessageBox>
#include <vtkInteractorStyleTrackballCamera.h>

// Constructor
EventQtSlotConnect::EventQtSlotConnect()
{
    this->setupUi(this);
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    this->qvtkWidget->SetRenderWindow(renderWindow);

    vtkNew<vtkEventQtSlotConnect> slotConnector;
    this->Connections = slotConnector;

    // Sphere
    vtkNew<vtkSphereSource> sphereSource;
    sphereSource->Update();
    vtkNew<vtkPolyDataMapper> sphereMapper;
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

    vtkNew<vtkActor> sphereActor;
    sphereActor->SetMapper(sphereMapper);

    // VTK Renderer
    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(sphereActor);
    // sphereSource ==> vtkPolyDataMapper ==> vtkActor ==> vtkRenderer ==> renderWindow

    this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
      vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle( style );

    this->Connections->Connect(this->qvtkWidget->GetRenderWindow()->GetInteractor(),
    vtkCommand::LeftButtonPressEvent,
    this,
    SLOT(slot_clicked(vtkObject*, unsigned long, void*, void*)));

    this->Connections->Connect( qvtkWidget->GetRenderWindow()->GetInteractor(),
                               vtkCommand::KeyReleaseEvent,
                               this,
                               SLOT(slot_rightClicked()) );
}

void EventQtSlotConnect::slot_clicked(vtkObject*, unsigned long, void*, void*)
{
    std::cout << "Clicked." << std::endl;
}

void EventQtSlotConnect::slot_rightClicked()
{
    QMessageBox::information( this, "Key Released", "You release key.", QMessageBox::Ok );
}

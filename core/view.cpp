#include "view.h"

#include <vtkObjectFactory.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include "pickinginteractorstyle.h"
#include "commonMacros.h"

#include <QApplication>

View::View():
    QObject(),
    m_fps(15),
    m_timer(new QTimer(this)),
    m_qvtkwidget(new QVTKWidget(QApplication::mainWidget())),
    m_scene(NULL)
{
    m_renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_qvtkwidget->SetRenderWindow(m_renderWindow);
    int numLayers = static_cast<int>(ModelItem::RenderLayer::ANNOTATION);
    m_qvtkwidget->GetRenderWindow()->SetNumberOfLayers(numLayers+1);
    for ( int iter = numLayers; iter >=  0; iter-- )
    {
        ModelItem::RenderLayer layer = static_cast<ModelItem::RenderLayer>(iter);
        m_renderers[layer] = vtkSmartPointer<vtkRenderer>(vtkRenderer::New());
        m_renderers[layer]->SetLayer(static_cast<int>(layer));
        m_qvtkwidget->GetRenderWindow()->AddRenderer(m_renderers[layer]);
        /*if(iter == numLayers)
        {
            m_camera = m_renderers[layer]->GetActiveCamera();
        }
        else
        {
            m_renderers[layer]->SetActiveCamera(m_camera);
        }*/
    }
    connect(m_timer,SIGNAL(timeout()),this,SLOT(update()));
    m_timer->start(1000/m_fps);

    m_interatorStyle = QSharedPointer<PickingInteractorStyle>(new PickingInteractorStyle());
    m_interatorStyle->interactorStyle()->SetDefaultRenderer(m_renderers[ModelItem::RenderLayer::REFERENCE]);

    m_renderWindow->GetInteractor()->SetInteractorStyle(m_interatorStyle->interactorStyle().GetPointer());

    m_subscriber = QSharedPointer<InteractorPropSubscriber>(new InteractorPropSubscriber());
    m_interatorStyle->setInteractorPropSubscriber(m_subscriber);

    m_camera = vtkSmartPointer<vtkCamera>::New();

}

View::~View()
{
    removeScene();
    m_timer->stop();
}

void View::setUpdateRate(int fps)
{
    m_fps = fps;
    if (fps == 0)
    {
        m_timer->stop();
        return;
    }
    m_timer->setInterval(1000/m_fps);
    m_timer->start();
}

void View::setWorldTransform(vtkSmartPointer<vtkTransform> xfm)
{
    m_worldXfm->DeepCopy(xfm.GetPointer());
    QMapIterator<QUuid, QSharedPointer<ViewItem> > it(m_viewItems);//it points before the first element
    while (it.hasNext()) {
        it.next();
        it.value()->setWorldTransform(xfm);
    }
    return;

}

void View::addEventObserver(IViewEventObserver *customObs, QUuid uid)
{
    if (!customObs)
        return;

    m_subscriber->addEventObserver(customObs, uid);

}

void View::removeEventObserver(IViewEventObserver *customObs)
{
    if (!customObs)
        return;

    m_subscriber->removeEventObserver(customObs);

}

void View::addEventObserver(QUuid uid)
{
    if (this->m_viewItems.end() == this->m_viewItems.find(uid))
    {
        qDebug() << "Unknown UUid passed in";
        return;
    }
    m_subscriber->addEventObserver(this->m_viewItems[uid]);
}

void View::addEventObserver(vtkSmartPointer<vtkProp3D> actor, QUuid uid)
{
    if (this->m_viewItems.end() == this->m_viewItems.find(uid))
    {
        qDebug() << "Unknown UUid passed in addEventObserver";
        return;
    }
    QSharedPointer<ViewItem> item = this->m_viewItems[uid];
    m_subscriber->addEventObserver(actor, item);
}

void View::removeEventObserver(QUuid self)
{
    if (this->m_viewItems.end() == this->m_viewItems.find(self))
    {
        qDebug() << "Unknown UUid passed in";
        return;
    }
    QSharedPointer<ViewItem> item = this->m_viewItems[self];
    m_subscriber->removeObserver(item);

}


void View::setInteractorStyle(QSharedPointer<IViewInteractorStyle> style)
{
    m_interatorStyle = style;
    m_interatorStyle->setInteractorPropSubscriber(m_subscriber);
    m_interatorStyle->interactorStyle()->SetDefaultRenderer(m_renderers[ModelItem::RenderLayer::REFERENCE]);
    m_renderWindow->GetInteractor()->SetInteractorStyle(m_interatorStyle->interactorStyle().GetPointer());

}

void View::addFactory(QSharedPointer<IViewItemFactory> factory)
{
    m_factories.push_back(factory);
}

QVTKWidget * View::qvtkwidget() const
{
    return m_qvtkwidget;
}

void View::setScene(QSharedPointer<Scene> scene)
{

    removeScene();
    m_scene = scene;
    sharedconnect(m_scene,SIGNAL(modelItemAdded(QUuid) ),this,SLOT(createViewItem(QUuid )));
    sharedconnect(m_scene,SIGNAL(modelItemRemoved(QUuid) ),this,SLOT(deleteViewItem(QUuid )));

    QList < QSharedPointer<ModelItem> > models = m_scene->modelItems();
    QList < QSharedPointer<ModelItem> >::iterator it;
    for (it = models.begin(); it != models.end(); it++)
    {
        createViewItem((*it)->id());
    }

}


void View::update()
{
    m_qvtkwidget->update();
}


void View::removeScene()
{
    if (!m_scene)
        return;
    m_scene->disconnect(this);
    QList<QSharedPointer<ViewItem> > values = m_viewItems.values();
    for (int i = 0; i < values.size(); ++i)
    {
        QSharedPointer<ViewItem> value = values.at(i);
        value.clear();
    }
    m_viewItems.clear();

}

void View::takeScreenshot(const QString &filename)
{
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>(new windowToImageFilter);
      vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(m_renderWindow);
    windowToImageFilter->SetMagnification(1); //set the resolution of the output image (3 times the current resolution of vtk render window)
    windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
    windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
    windowToImageFilter->Update();
    vtkSmartPointer<vtkPNGWriter> writer =
      vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName(filename.toStdString().c_str());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();
}

void View::setCamera(vtkSmartPointer<vtkCamera> camera)
{
    m_camera =camera;
    int numLayers = static_cast<int>(ModelItem::RenderLayer::ANNOTATION);
    for ( int iter = numLayers; iter >=  0; iter-- )
    {
        ModelItem::RenderLayer layer = static_cast<ModelItem::RenderLayer>(iter);
        m_renderers[layer]->SetActiveCamera(m_camera);
    }

}


void View::createViewItem(QUuid id)
{
    QSharedPointer<ModelItem> model = m_scene->modelItem(id);
    if (!model)
    {
        qDebug() << "Model does not exist for given uuid";
        return;
    }

    QSharedPointer<ViewItem> viewitem;
    QList<QSharedPointer<IViewItemFactory> >::iterator it;
    for ( it = m_factories.begin(); it != m_factories.end(); it++)
    {
        viewitem = (*it)->createViewItem(model);
        if (viewitem)
            break;
    }

    if (!viewitem)
    {
        qDebug() << "No IViewItemFactory for given model";
        return;
    }

    if (m_viewItems.contains(id))
    {
        qDebug() << "ViewItem already exist for given uuid";
        m_viewItems[id].clear();
    }

    viewitem->setView(this);
    m_viewItems[id] = viewitem;
    viewitem->setRenderer(m_renderers[viewitem->layer()]);



    return;
}


void View::deleteViewItem(QUuid id)
{
    if (!m_viewItems.contains(id))
    {
        qDebug() << "ViewItem does not exists.";
        return;
    }
    this->removeEventObserver(id);
    m_viewItems.remove(id);
    return;

}



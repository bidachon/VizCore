
#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>

#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <QColor>
#include <vtkRegularPolygonSource.h>
#include <vtkProperty.h>



#include "IModelViewFactory.h"
#include "view.h"


class MarkerPropertyList : public PropertyList
{
public:
    MarkerPropertyList(){}
    ~MarkerPropertyList(){}

    void setSphereColor(QColor color){m_sphereColor = color;}
    void setPolygoneColor(QColor color){m_polygonColor = color;}

    QColor sphereColor() const {return m_sphereColor;}
    QColor polygoneColor() const {return m_polygonColor;}

    void setDefault() {}



private:
    QColor m_sphereColor;
    QColor m_polygonColor;

};

class MarkerModelItem: public ModelItem
{
public:
    MarkerModelItem():ModelItem(){
        m_poly = vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkRegularPolygonSource> polygonSource = vtkSmartPointer<vtkRegularPolygonSource>::New();

        polygonSource->SetNumberOfSides(50);
        polygonSource->SetRadius(20.0);
        polygonSource->SetCenter(0,0,0);

        polygonSource->Update();

        m_poly->DeepCopy(polygonSource->GetOutput());

        m_sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        m_sphereSource->SetRadius(10.0);

    }

    virtual ~MarkerModelItem(){}

    vtkSmartPointer<vtkPolyData> polyData() const {return m_poly;}
    vtkSmartPointer<vtkSphereSource> sphereSource() const {return m_sphereSource;}

    QSharedPointer<PropertyList> defaultProperties() const {return QSharedPointer<MarkerPropertyList>(new MarkerPropertyList()); }

private:
    vtkSmartPointer<vtkPolyData> m_poly;
      vtkSmartPointer<vtkSphereSource> m_sphereSource;

};

class MarkerViewItem: public ViewItem
{
    Q_OBJECT
public:
    MarkerViewItem(QSharedPointer<ModelItem> model):ViewItem(model){
        QSharedPointer<MarkerModelItem> markerModel = qSharedPointerDynamicCast<MarkerModelItem>(model);
        m_markerModel = markerModel;
        assert(m_markerModel);
    }


protected slots:
    virtual void doUpdated()
    {
        qDebug() << "doUpdated";
    }

    virtual void doPositionUpdated(const PositionUpdate &pu)
    {
        qDebug() << "doPositionUpdated";
        pu.transform()->Print(std::cerr);
        m_sphereActor->SetUserTransform(pu.transform().GetPointer());
    }

    virtual void doVisibilityChanged(bool v)
    {
        qDebug() << "doVisibilityChanged";
    }

    virtual void doPropertyUpdated(QSharedPointer<PropertyList> prop)
    {
        QSharedPointer<MarkerPropertyList> markerProp = prop.dynamicCast<MarkerPropertyList>();
        if (!markerProp)
            return;
        QColor color = markerProp->sphereColor();
        m_sphereActor->GetProperty()->SetColor(color.red(),color.green(),color.blue());
        color = markerProp->polygoneColor();
        m_markerActor->GetProperty()->SetColor(color.red(),color.green(),color.blue());

    }

protected:

    virtual void initActors()
    {
        ///set the polyData actor first
        m_markerMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        assert(m_markerModel);
        m_markerMapper->SetInput(m_markerModel->polyData());
        m_markerActor = vtkSmartPointer<vtkActor>::New();
        m_markerActor->SetMapper(m_markerMapper);
        m_ren->AddActor(m_markerActor);

        //set the Sphere actor
        m_sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        m_sphereMapper->SetInputConnection(m_markerModel->sphereSource()->GetOutputPort());
        m_sphereActor = vtkSmartPointer<vtkActor>::New();
        m_sphereActor->SetMapper(m_sphereMapper);
        m_ren->AddActor(m_sphereActor);
    }


private:


    QSharedPointer<MarkerModelItem> m_markerModel;
    vtkSmartPointer<vtkPolyDataMapper> m_markerMapper;
    vtkSmartPointer<vtkPolyDataMapper> m_sphereMapper;
    vtkSmartPointer<vtkActor> m_markerActor;
    vtkSmartPointer<vtkActor> m_sphereActor;



};






class MarkerFactory : public IViewItemFactory
{
public:
 virtual QSharedPointer<ViewItem> createViewItem(QSharedPointer<ModelItem> model) const
    {
        QSharedPointer<ViewItem> myViewItem;
        QSharedPointer<MarkerModelItem> myModel = qSharedPointerDynamicCast<MarkerModelItem>(model);
        if (myModel)
            myViewItem = QSharedPointer<MarkerViewItem>(new MarkerViewItem(myModel));

        return myViewItem;
    }
};


class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow():QMainWindow()
    {
        init();
    }

    ~MainWindow(){}

    void init()
    {

        _markerFactory = QSharedPointer<MarkerFactory>(new MarkerFactory());

        _view1.addFactory(_markerFactory);
        _view2.addFactory(_markerFactory);

        _scene = QSharedPointer<Scene>(new Scene());

        _modelItem = QSharedPointer<MarkerModelItem>(new MarkerModelItem());
        _modelItem->setRenderLayer(ModelItem::RenderLayer::REFERENCE);
        _scene->addItem(_modelItem);
        _view1.setScene(_scene);
        _view2.setScene(_scene);

        QPushButton *colorBtn = new QPushButton("Change color");
        QPushButton *moveBtn = new QPushButton("Translate(10,10,10)");
        _vbox = new QVBoxLayout;
        _vbox->addWidget(colorBtn);
        _vbox->addWidget(moveBtn);
        _vbox->addWidget(_view1.qvtkwidget());
        _vbox->addWidget(_view2.qvtkwidget());


        _centralWidget = new QWidget(this);
        _centralWidget->setLayout(_vbox);
        this->setCentralWidget(_centralWidget);
        this->updateGeometry();

        connect(colorBtn,SIGNAL(clicked()),this,SLOT(updateProperties()));
        connect(moveBtn,SIGNAL(clicked()),this,SLOT(updatePosition()));
    }

public slots:
    void updateProperties(){
        QSharedPointer<MarkerPropertyList> prop(new MarkerPropertyList());
        prop->setPolygoneColor(QColor(Qt::red));
        prop->setSphereColor(QColor(Qt::green));
        _modelItem->setProperties(prop);

    }

    void updatePosition(){
        vtkSmartPointer<vtkTransform> xform = _modelItem->transform();
        xform->Translate(10,10,10);
        //no signal emitted until setPosition is called.
        xform->Print(std::cerr);
        _modelItem->setTransform(xform);
    }

private:

    View _view1;
    View _view2;
    QVBoxLayout *_vbox;
    QWidget *_centralWidget;
    QSharedPointer<MarkerFactory> _markerFactory;
    QSharedPointer<Scene> _scene;
    QSharedPointer<MarkerModelItem> _modelItem;
};

int main(int argc, char **argv)
{

    QApplication app(argc,argv);
    MainWindow *mainWindow = new MainWindow();
    app.setMainWidget(mainWindow);
    mainWindow->setGeometry(100,100,500,500);
    mainWindow->show();
    app.exec();

}

#include "main.moc"

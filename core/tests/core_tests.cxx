
#include <iostream>
#include <QApplication>
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QVariant>

#include "scene.h"
#include "view.h"
#include "mymodelitem.h"
#include "myviewitem.h"
#include "mycontroller.h"





class TestCore: public QObject
{
    Q_OBJECT
public slots:
    void positionUpdated(const PositionUpdate& pu)
    {

        qDebug() << "Position Update";
        for (int i = 0 ; i < 4; i++)
            for (int j = 0 ; j < 4; j++)
                QVERIFY(pu.transform()->GetMatrix()->GetElement(i,j) == (i+1)*(j+1));

    }

private slots:

    void initTestCase()
    {
        qRegisterMetaType<PositionUpdate>();
        qRegisterMetaType<QSharedPointer<PropertyList> >();
    }

    /* QCOMPARE stops test execution after a failed test */

    void testEvents()
    {
        MyModelItem m;
        QSharedPointer<MyModelItem> parent(new MyModelItem());
        QSharedPointer<PropertyList> prop(new MyPropertyList(13,true,"youpitralala"));
        QSignalSpy spy1(&m.events(),SIGNAL(updated()));
        QSignalSpy spy2(&m.events(),SIGNAL(positionUpdated(PositionUpdate)));
        m.emitupdate();

        QCOMPARE(spy1.count(),1);
        QCOMPARE(spy2.count(),0);
        spy1.clear();
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
        for (int i = 0 ; i < 4; i++)
            for (int j = 0 ; j < 4; j++)
                matrix->SetElement(i,j,(i+1)*(j+1));
        transform->SetMatrix(matrix);
        connect(&m.events(),SIGNAL(positionUpdated(PositionUpdate)),this, SLOT(positionUpdated(PositionUpdate)));
        m.setTransform(transform);
        QCOMPARE(spy1.count(),0);
        QCOMPARE(spy2.count(),1);

        QSignalSpy spy3(&m,SIGNAL(testDoParentUpdated()));
        QSignalSpy spy4(&m,SIGNAL(testDoParentPositionUpdated(PositionUpdate)));
        QSignalSpy spy5(&m,SIGNAL(testDoParentVisibilityChanged(bool)));
        QSignalSpy spy6(&m,SIGNAL(testDoParentPropertyUpdated(QSharedPointer<PropertyList>)));



        //Verify all slots are connected whe nsetting a parent
        m.setParent(parent);
        parent->emitupdate();
        parent->setTransform(vtkSmartPointer<vtkTransform>::New());
        parent->setVisible(false);
        parent->setProperties(prop);
        QCOMPARE(spy3.count(),1);
        QCOMPARE(spy4.count(),1);
        QCOMPARE(spy5.count(),1);
        QCOMPARE(spy6.count(),1);

        //Verify all slos are deconnnected when the parent is removed
        QSharedPointer<ModelItem> emptyParent;
        m.setParent(emptyParent);
        parent->emitupdate();
        parent->setTransform(vtkSmartPointer<vtkTransform>::New());
        parent->setVisible(false);
        parent->setProperties(prop);
        QCOMPARE(spy3.count(),1);
        QCOMPARE(spy4.count(),1);
        QCOMPARE(spy5.count(),1);
        QCOMPARE(spy6.count(),1);
    }

    void testModelItem()
    {
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
        for (int i = 0 ; i < 4; i++)
            for (int j = 0 ; j < 4; j++)
                matrix->SetElement(i,j,(i+1)*(j+1));
        transform->SetMatrix(matrix);
        MyModelItem m;
        m.setTransform(transform);
        vtkSmartPointer<vtkTransform> newTranform = m.transform();
        for (int i = 0 ; i < 4; i++)
            for (int j = 0 ; j < 4; j++)
            {
                QString logMessage = QString::number(1 + transform->GetMatrix()->GetElement(i,j))
                                     + " not equal "
                                     + QString::number(1 + newTranform->GetMatrix()->GetElement(i,j))
                                     + " in call QVERIFY(1 + transform->GetMatrix()->GetElement(i,j) == "
                                       " 1 + newTranform->GetMatrix()->GetElement(i,j), logMessage);";

                QVERIFY2(qFuzzyCompare(1 + transform->GetMatrix()->GetElement(i,j), 1 + newTranform->GetMatrix()->GetElement(i,j)), logMessage);
            }

        QVERIFY2(newTranform->GetReferenceCount() == 2,"unexpected transform ref count for newTransform"); //one for inside model, one outside
        QVERIFY2(transform->GetReferenceCount() == 1,"unexpected transform ref count for transform");//only one since we're Deep copying the transform

       transform = vtkSmartPointer<vtkTransform>::New();

       for (int i = 0 ; i < 4; i++)
       {
            for (int j = 0 ; j < 4; j++)
            {
               QString logMessage = QString::number(1 + newTranform->GetMatrix()->GetElement(i,j))
                                    + " not equal "
                                    + QString::number(1 + (i+1)*(j+1))
                                    + " in call QVERIFY( 1 + transform->GetMatrix()->GetElement(i,j) == "
                                      "1 + (i+1)*(j+1), logMessage);";
               QVERIFY2(qFuzzyCompare(1 + newTranform->GetMatrix()->GetElement(i,j), 1 + (i+1)*(j+1)),logMessage);
            }
        }
    }

    void testViewItem()
    {
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
        for (int i = 0 ; i < 4; i++)
            for (int j = 0 ; j < 4; j++)
                matrix->SetElement(i,j,(i+1)*(j+1));
        transform->SetMatrix(matrix);
        QSharedPointer<MyModelItem> m(new MyModelItem());
        MyViewItem v(m);
        QSignalSpy spy1(&v,SIGNAL(testDoUpdated()));
        QSignalSpy spy2(&v,SIGNAL(testDoPositionUpdated(const PositionUpdate&)));
        QSignalSpy spy3(&v,SIGNAL(testDoVisibilityChanged(bool)));
        QSignalSpy spy4(&v,SIGNAL(testDoSelectableChanged(bool)));
        QSignalSpy spy5(&v,SIGNAL(testDoSelectedChanged(bool)));

        QCOMPARE(spy1.count(),0);
        QCOMPARE(spy2.count(),0);
        QCOMPARE(spy3.count(),0);
        QCOMPARE(spy4.count(),0);
        QCOMPARE(spy5.count(),0);

        m->setTransform(transform);
        QCOMPARE(spy1.count(),0);
        QCOMPARE(spy2.count(),1);
        QCOMPARE(spy3.count(),0);
        QCOMPARE(spy4.count(),0);
        QCOMPARE(spy5.count(),0);

        m->setVisible(true);
        QCOMPARE(spy1.count(),0);
        QCOMPARE(spy2.count(),1);
        QCOMPARE(spy3.count(),1);
        QCOMPARE(spy4.count(),0);
        QCOMPARE(spy5.count(),0);

        m->emitupdate();
        QCOMPARE(spy1.count(),1);
        QCOMPARE(spy2.count(),1);
        QCOMPARE(spy3.count(),1);
        QCOMPARE(spy4.count(),0);
        QCOMPARE(spy5.count(),0);

        m->setSelectable(true);
        QCOMPARE(m->isSelectable(),true);
        QCOMPARE(spy1.count(),1);
        QCOMPARE(spy2.count(),1);
        QCOMPARE(spy3.count(),1);
        QCOMPARE(spy4.count(),1);
        QCOMPARE(spy5.count(),0);
        QList<QVariant> arg = spy4.takeFirst();
        QVERIFY2(arg.at(0).toBool() == true, "testing selectable bool failed");

        m->setSelected(true);
        QCOMPARE(m->isSelected(),true);
        QCOMPARE(spy1.count(),1);
        QCOMPARE(spy2.count(),1);
        QCOMPARE(spy3.count(),1);
        QCOMPARE(spy4.count(),0);
        QCOMPARE(spy5.count(),1);
        arg = spy5.takeFirst();
        QVERIFY2(arg.at(0).toBool() == true, "testing isSelected bool failed");

        m->setSelectable(false);
        m->setSelected(false);
        QCOMPARE(m->isSelectable(),false);
        QCOMPARE(m->isSelected(),false);
        QCOMPARE(spy1.count(),1);
        QCOMPARE(spy2.count(),1);
        QCOMPARE(spy3.count(),1);
        QCOMPARE(spy4.count(),1);
        QCOMPARE(spy5.count(),1);


        QSignalSpy spy6(&v,SIGNAL(testDoPropertyChanged(int,bool,QString)));
        QSharedPointer<PropertyList> prop(new MyPropertyList(13,true,"cestgenial"));

        //test default properties values
        QSharedPointer<MyPropertyList> defaultProp = qSharedPointerDynamicCast<MyPropertyList>(m->properties());
        Q_ASSERT(defaultProp);
        QVERIFY2(defaultProp->myIntProperty() == 12, "12 should be the default value");
        QVERIFY2(defaultProp->myBoolProperty() == false, "false should be the default value");
        QVERIFY2(defaultProp->myStringProperty() == "teststr", "teststr should be the default value");

        m->setProperties(m->defaultProperties());
        QCOMPARE(spy6.count(),1);
        arg = spy6.takeFirst();
        QVERIFY2(arg.at(0).toInt() == 12, "Failed to retrieve int property value");
        QVERIFY2(arg.at(1).toBool() == false, "Failed to retrieve bool property value");
        QVERIFY2(arg.at(2).toString() == "teststr", "Failed to retrieve QString property value");

        m->setProperties(prop);
        QCOMPARE(spy6.count(),1);
        arg = spy6.takeFirst();
        QVERIFY2(arg.at(0).toInt() == 13, "Failed to retrieve int property value");
        QVERIFY2(arg.at(1).toBool() == true, "Failed to retrieve bool property value");
        QVERIFY2(arg.at(2).toString() == "cestgenial", "Failed to retrieve QString property value");



    }

    void testScene()
    {
        qRegisterMetaType<QUuid>("QUuid");
        Scene s;
        QSignalSpy spy1(&s,SIGNAL(modelItemAdded(QUuid)));
        QSignalSpy spy2(&s,SIGNAL(modelItemRemoved(QUuid)));
        QSharedPointer<MyModelItem> m1(new MyModelItem());

        //test the item is added properly and the signal emitted
        s.addItem(m1);
        QCOMPARE(spy1.count(),1);
        QCOMPARE(spy2.count(),0);

        QList<QVariant> arguments = spy1.takeFirst();
        QUuid uid = qvariant_cast<QUuid>(arguments.at(0));
        QVERIFY(uid == m1->id());
        QVERIFY(s.modelItems().size() == 1);

        //test that adding the same model does not add it and no signal is emitted.
        s.addItem(m1);
        QCOMPARE(spy1.count(),0);
        QCOMPARE(spy2.count(),0);
        QVERIFY(s.modelItems().size() == 1);

        //test adding another model, the signal is emitted
        QSharedPointer<MyModelItem> m2(new MyModelItem());
        s.addItem(m2);
        QCOMPARE(spy1.count(),1);
        QCOMPARE(spy2.count(),0);
        QVERIFY(s.modelItems().size() == 2);

        arguments = spy1.takeFirst();
        uid = qvariant_cast<QUuid>(arguments.at(0));
        QVERIFY(uid == m2->id());

        //test removing item, signal is emitted
        s.removeItem(m2->id());
        QCOMPARE(spy1.count(),0);
        QCOMPARE(spy2.count(),1);
        QVERIFY(s.modelItems().size() == 1);

        arguments = spy2.takeFirst();
        uid = qvariant_cast<QUuid>(arguments.at(0));
        QVERIFY(uid == m2->id());

        //test removing same item, no signal emitted
        s.removeItem(m2->id());
        QCOMPARE(spy1.count(),0);
        QCOMPARE(spy2.count(),0);
        QVERIFY(s.modelItems().size() == 1);

        //test removing another model, signal is emitted
        s.removeItem(m1->id());
        QCOMPARE(spy1.count(),0);
        QCOMPARE(spy2.count(),1);
        QVERIFY(s.modelItems().isEmpty());

        arguments = spy2.takeFirst();
        uid = qvariant_cast<QUuid>(arguments.at(0));
        QVERIFY(uid == m1->id());

    }

    void testView()
    {
        QSharedPointer<MyModelItem> m1(new MyModelItem());
        QSharedPointer<MyModelItem> m2(new MyModelItem());
        QSharedPointer<MyModelItem> m3(new MyModelItem());
        View v;
        QSharedPointer<Scene> s(new Scene());
        v.setScene(s);
        s->addItem(m1);

        QSharedPointer<IViewItemFactory> factory(new MyFactory());
        v.addFactory(factory);
        s->addItem(m2);
        s->addItem(m3);
        v.removeScene();

    }

    void testControllerItem()
    {
        QSharedPointer<View> view = QSharedPointer<View>(new View());
        QSharedPointer<Scene> scene = QSharedPointer<Scene>(new Scene());
        QSharedPointer<MyModelItem> model(new MyModelItem());
        QSharedPointer<IViewItemFactory> factory(new MyFactory());
        view->addFactory(factory);
        scene->addItem(model);
        view->setScene(scene);

        MyController *controller = new MyController();
        controller->setModel(model);
        QSignalSpy spy1(controller,SIGNAL(sigDoUpdated()));
        QSignalSpy spy2(controller,SIGNAL(sigDoPositionUpdated(const PositionUpdate&)));
        QSignalSpy spy3(controller,SIGNAL(sigDoVisibilityChanged(bool)));
        QSignalSpy spy4(controller,SIGNAL(sigDoSelectionChanged(bool)));
        QSignalSpy spy5(controller,SIGNAL(sigDoIsSelectableChanged(bool)));
        QSignalSpy spy6(controller,SIGNAL(sigDoCellPickingEvent()));

        view->addEventObserver(controller,model->id());

        model->setVisible(true);
        model->setSelectable(true);
        model->setSelected(true);
        model->emitupdate();
        vtkSmartPointer<vtkTransform> xfm = vtkSmartPointer<vtkTransform>::New();
        model->setTransform(xfm);


        QVERIFY(spy1.count() == 1);
        QVERIFY(spy2.count() == 1);
        QVERIFY(spy3.count() == 1);
        QVERIFY(spy4.count() == 1);
        QVERIFY(spy5.count() == 1);

        view->qvtkwidget()->show();

        QTimer::singleShot(3000,qApp,SLOT(quit()));
        model->setSelectable(true);
        QTest::mouseClick(view->qvtkwidget(),Qt::LeftButton,0,QPoint(50,50));
        model->setSelectable(false);
        QTest::mouseClick(view->qvtkwidget(),Qt::LeftButton,0,QPoint(50,50));
        QVERIFY(spy6.count() == 1);
        spy6.clear();
        view->removeEventObserver(controller);
        QTest::mouseClick(view->qvtkwidget(),Qt::LeftButton,0,QPoint(50,50));
        QVERIFY(spy6.count() == 0);
        qApp->exec();


        delete controller;
    }

    /* cleanup is automatically called after each test (private slots) */
    void cleanup()
    {
    }

};

QTEST_MAIN(TestCore)
#include "core_tests.moc"


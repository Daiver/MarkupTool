/********************************************************************************
** Form generated from reading UI file 'markupwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARKUPWINDOW_H
#define UI_MARKUPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "markupview.h"

QT_BEGIN_NAMESPACE

class Ui_MarkupWindow
{
public:
    QAction *loadFacesAction;
    QAction *actionNext;
    QAction *actionPast;
    QAction *actionClear;
    QAction *actionLoad_shape;
    QAction *actionSave_shape;
    QAction *actionDelete;
    QAction *actionUpdate_from_JSON;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QListWidget *listFiles;
    MarkupView *markupView;
    QDoubleSpinBox *spinLandmarkSize;
    QComboBox *comboBoxBodyPart;
    QLabel *ready;
    QRadioButton *editButton;
    QMenuBar *menuBar;
    QMenu *menuFiles;
    QMenu *menuControl;
    QMenu *menuLadnmarks;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MarkupWindow)
    {
        if (MarkupWindow->objectName().isEmpty())
            MarkupWindow->setObjectName(QStringLiteral("MarkupWindow"));
        MarkupWindow->resize(400, 300);
        MarkupWindow->setCursor(QCursor(Qt::ArrowCursor));
        loadFacesAction = new QAction(MarkupWindow);
        loadFacesAction->setObjectName(QStringLiteral("loadFacesAction"));
        actionNext = new QAction(MarkupWindow);
        actionNext->setObjectName(QStringLiteral("actionNext"));
        actionPast = new QAction(MarkupWindow);
        actionPast->setObjectName(QStringLiteral("actionPast"));
        actionClear = new QAction(MarkupWindow);
        actionClear->setObjectName(QStringLiteral("actionClear"));
        actionLoad_shape = new QAction(MarkupWindow);
        actionLoad_shape->setObjectName(QStringLiteral("actionLoad_shape"));
        actionSave_shape = new QAction(MarkupWindow);
        actionSave_shape->setObjectName(QStringLiteral("actionSave_shape"));
        actionDelete = new QAction(MarkupWindow);
        actionDelete->setObjectName(QStringLiteral("actionDelete"));
        actionDelete->setCheckable(true);
        actionUpdate_from_JSON = new QAction(MarkupWindow);
        actionUpdate_from_JSON->setObjectName(QStringLiteral("actionUpdate_from_JSON"));
        centralWidget = new QWidget(MarkupWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        listFiles = new QListWidget(centralWidget);
        listFiles->setObjectName(QStringLiteral("listFiles"));
        listFiles->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(listFiles, 4, 1, 1, 1);

        markupView = new MarkupView(centralWidget);
        markupView->setObjectName(QStringLiteral("markupView"));
        markupView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        markupView->setDragMode(QGraphicsView::NoDrag);
        markupView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        markupView->setResizeAnchor(QGraphicsView::NoAnchor);
        markupView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        gridLayout->addWidget(markupView, 0, 0, 5, 1);

        spinLandmarkSize = new QDoubleSpinBox(centralWidget);
        spinLandmarkSize->setObjectName(QStringLiteral("spinLandmarkSize"));
        spinLandmarkSize->setSingleStep(0.1);
        spinLandmarkSize->setValue(0.5);

        gridLayout->addWidget(spinLandmarkSize, 0, 1, 1, 1);

        comboBoxBodyPart = new QComboBox(centralWidget);
        comboBoxBodyPart->setObjectName(QStringLiteral("comboBoxBodyPart"));
        comboBoxBodyPart->setMinimumContentsLength(3);

        gridLayout->addWidget(comboBoxBodyPart, 1, 1, 1, 1);

        ready = new QLabel(centralWidget);
        ready->setObjectName(QStringLiteral("ready"));
        ready->setTextFormat(Qt::RichText);
        ready->setScaledContents(false);
        ready->setWordWrap(false);

        gridLayout->addWidget(ready, 3, 1, 1, 1);

        editButton = new QRadioButton(centralWidget);
        editButton->setObjectName(QStringLiteral("editButton"));
        editButton->setEnabled(true);
        editButton->setChecked(true);

        gridLayout->addWidget(editButton, 2, 1, 1, 1);

        MarkupWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MarkupWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 25));
        menuFiles = new QMenu(menuBar);
        menuFiles->setObjectName(QStringLiteral("menuFiles"));
        menuControl = new QMenu(menuBar);
        menuControl->setObjectName(QStringLiteral("menuControl"));
        menuLadnmarks = new QMenu(menuBar);
        menuLadnmarks->setObjectName(QStringLiteral("menuLadnmarks"));
        MarkupWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MarkupWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QPalette palette;
        QBrush brush(QColor(85, 87, 83, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        QBrush brush1(QColor(127, 131, 124, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        QBrush brush2(QColor(106, 109, 103, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        QBrush brush3(QColor(42, 43, 41, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush3);
        QBrush brush4(QColor(56, 58, 55, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush4);
        QBrush brush5(QColor(255, 255, 255, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush5);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush5);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush5);
        QBrush brush6(QColor(0, 0, 0, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Active, QPalette::HighlightedText, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Active, QPalette::NoRole, brush);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::NoRole, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        palette.setBrush(QPalette::Disabled, QPalette::NoRole, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
        mainToolBar->setPalette(palette);
        MarkupWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MarkupWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MarkupWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFiles->menuAction());
        menuBar->addAction(menuControl->menuAction());
        menuBar->addAction(menuLadnmarks->menuAction());
        menuFiles->addAction(loadFacesAction);
        menuFiles->addAction(actionLoad_shape);
        menuFiles->addAction(actionUpdate_from_JSON);
        menuFiles->addAction(actionSave_shape);
        menuFiles->addAction(actionClear);
        menuControl->addAction(actionNext);
        menuControl->addAction(actionPast);
        menuLadnmarks->addAction(actionDelete);

        retranslateUi(MarkupWindow);

        QMetaObject::connectSlotsByName(MarkupWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MarkupWindow)
    {
        MarkupWindow->setWindowTitle(QApplication::translate("MarkupWindow", "MarkupWindow", 0));
        loadFacesAction->setText(QApplication::translate("MarkupWindow", "Load faces", 0));
        actionNext->setText(QApplication::translate("MarkupWindow", "Next", 0));
        actionNext->setShortcut(QApplication::translate("MarkupWindow", "Right", 0));
        actionPast->setText(QApplication::translate("MarkupWindow", "Past", 0));
        actionPast->setShortcut(QApplication::translate("MarkupWindow", "Left", 0));
        actionClear->setText(QApplication::translate("MarkupWindow", "Clear", 0));
        actionLoad_shape->setText(QApplication::translate("MarkupWindow", "Load shape", 0));
        actionSave_shape->setText(QApplication::translate("MarkupWindow", "Save shape", 0));
        actionDelete->setText(QApplication::translate("MarkupWindow", "Delete", 0));
        actionDelete->setShortcut(QApplication::translate("MarkupWindow", "Del", 0));
        actionUpdate_from_JSON->setText(QApplication::translate("MarkupWindow", "Update from JSON", 0));
        actionUpdate_from_JSON->setShortcut(QApplication::translate("MarkupWindow", "Ctrl+L", 0));
        comboBoxBodyPart->clear();
        comboBoxBodyPart->insertItems(0, QStringList()
         << QApplication::translate("MarkupWindow", "Left eye", 0)
         << QApplication::translate("MarkupWindow", "Right eye", 0)
         << QApplication::translate("MarkupWindow", "mouth", 0)
        );
        ready->setText(QApplication::translate("MarkupWindow", "Ready: ", 0));
        editButton->setText(QApplication::translate("MarkupWindow", "Allow edit", 0));
        menuFiles->setTitle(QApplication::translate("MarkupWindow", "Files", 0));
        menuControl->setTitle(QApplication::translate("MarkupWindow", "Control", 0));
        menuLadnmarks->setTitle(QApplication::translate("MarkupWindow", "Ladnmarks", 0));
    } // retranslateUi

};

namespace Ui {
    class MarkupWindow: public Ui_MarkupWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARKUPWINDOW_H

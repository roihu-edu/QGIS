/***************************************************************************
    qgsrendererv2widget.h
    ---------------------
    begin                : November 2009
    copyright            : (C) 2009 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSRENDERERV2WIDGET_H
#define QGSRENDERERV2WIDGET_H

#include <QWidget>
#include <QMenu>
#include "qgssymbolv2.h"
#include "qgsdatadefined.h"

class QgsVectorLayer;
class QgsStyleV2;
class QgsFeatureRendererV2;
class QgsSymbolV2SelectorDialog;


/**
  Base class for renderer settings widgets

WORKFLOW:
- open renderer dialog with some RENDERER  (never null!)
- find out which widget to use
- instantiate it and set in stacked widget
- on any change of renderer type, create some default (dummy?) version and change the stacked widget
- when clicked ok/apply, get the renderer from active widget and clone it for the layer
*/
class GUI_EXPORT QgsRendererV2Widget : public QWidget
{
    Q_OBJECT
  public:
    QgsRendererV2Widget( QgsVectorLayer* layer, QgsStyleV2* style );

    virtual ~QgsRendererV2Widget() {}

    //! return pointer to the renderer (no transfer of ownership)
    virtual QgsFeatureRendererV2* renderer() = 0;

    //! show a dialog with renderer's symbol level settings
    void showSymbolLevelsDialog( QgsFeatureRendererV2* r );

  protected:
    QgsVectorLayer* mLayer;
    QgsStyleV2* mStyle;
    QMenu* contextMenu;
    QAction* mCopyAction;
    QAction* mPasteAction;

    /**Subclasses may provide the capability of changing multiple symbols at once by implementing the following two methods
      and by connecting the slot contextMenuViewCategories(const QPoint&)*/
    virtual QList<QgsSymbolV2*> selectedSymbols() { return QList<QgsSymbolV2*>(); }
    virtual void refreshSymbolView() {}

  protected slots:
    void  contextMenuViewCategories( const QPoint& p );
    /**Change color of selected symbols*/
    void changeSymbolColor();
    /**Change opacity of selected symbols*/
    void changeSymbolTransparency();
    /**Change units mm/map units of selected symbols*/
    void changeSymbolUnit();
    /**Change line widths of selected symbols*/
    void changeSymbolWidth();
    /**Change marker sizes of selected symbols*/
    void changeSymbolSize();
    /**Change marker angles of selected symbols*/
    void changeSymbolAngle();

    virtual void copy() {}
    virtual void paste() {}

};


////////////

#include <QObject>

class QMenu;
class QgsField;
class QgsFields;


/**
Utility class for providing GUI for data-defined rendering.
*/
class QgsRendererV2DataDefinedMenus : public QObject
{
    Q_OBJECT

  public:

    QgsRendererV2DataDefinedMenus( QMenu* menu, QgsVectorLayer* layer, QString rotationField, QString sizeScaleField, QgsSymbolV2::ScaleMethod scaleMethod );
    ~QgsRendererV2DataDefinedMenus();

    void populateMenu( QMenu* menu, QString fieldName, QActionGroup *actionGroup );
#if 0
    void updateMenu( QActionGroup* actionGroup, QString fieldName );
#endif
  public slots:

    void rotationFieldSelected( QAction *a );
    void sizeScaleFieldSelected( QAction *a );
    void scaleMethodSelected( QAction *a );

  signals:

    void rotationFieldChanged( QString fldName );
    void sizeScaleFieldChanged( QString fldName );
    void scaleMethodChanged( QgsSymbolV2::ScaleMethod scaleMethod );

  protected:
    QMenu* mRotationMenu;
    QMenu* mSizeScaleMenu;
    QActionGroup *mSizeMethodActionGroup;
    QActionGroup *mRotationAttributeActionGroup;
    QActionGroup *mSizeAttributeActionGroup;
    QgsVectorLayer* mLayer;
};

////////////

#include "ui_widget_set_dd_value.h"
#include "qgssizescalewidget.h"

/**
Utility classes for "en masse" size definition
*/
class GUI_EXPORT QgsDataDefinedValueDialog : public QDialog, public Ui::QgsDataDefinedValueDialog
{
    Q_OBJECT

  public:
    /** Constructor
     * @param symbolList must not be empty
     * @param layer must not be null
     */
    QgsDataDefinedValueDialog( const QList<QgsSymbolV2*>& symbolList, QgsVectorLayer * layer, const QString & label );
    virtual ~QgsDataDefinedValueDialog() {}

  public slots:
    void dataDefinedChanged();

  protected:
    QgsDataDefined symbolDataDefined() const;
    void init( const QString & description ); // needed in children ctor to call virtual

    virtual QgsDataDefined symbolDataDefined( const QgsSymbolV2 * ) const = 0;
    virtual double value( const QgsSymbolV2 * ) const = 0;
    virtual void setDataDefined( QgsSymbolV2* symbol, const QgsDataDefined& dd ) = 0;

    QList<QgsSymbolV2*> mSymbolList;
    QgsVectorLayer* mLayer;
};

class GUI_EXPORT QgsDataDefinedSizeDialog : public QgsDataDefinedValueDialog
{
    Q_OBJECT
  public:
    QgsDataDefinedSizeDialog( const QList<QgsSymbolV2*>& symbolList, QgsVectorLayer * layer )
        : QgsDataDefinedValueDialog( symbolList, layer, tr( "Size" ) )
    {
      init( tr( "Symbol size" ) );
      if ( symbolList.length() )
        mDDBtn->setAssistant( new QgsSizeScaleWidget( mLayer, static_cast<const QgsMarkerSymbolV2*>( symbolList[0] ) ) );
    }

  protected:
    QgsDataDefined symbolDataDefined( const QgsSymbolV2 * symbol ) const override;

    double value( const QgsSymbolV2 * symbol ) const override { return static_cast<const QgsMarkerSymbolV2*>( symbol )->size(); }

    void setDataDefined( QgsSymbolV2* symbol, const QgsDataDefined& dd ) override;
};

class GUI_EXPORT QgsDataDefinedRotationDialog : public QgsDataDefinedValueDialog
{
    Q_OBJECT
  public:
    QgsDataDefinedRotationDialog( const QList<QgsSymbolV2*>& symbolList, QgsVectorLayer * layer )
        : QgsDataDefinedValueDialog( symbolList, layer, tr( "Rotation" ) )
    {
      init( tr( "Symbol rotation" ) );
    }

  protected:
    QgsDataDefined symbolDataDefined( const QgsSymbolV2 * symbol ) const override;

    double value( const QgsSymbolV2 * symbol ) const override { return static_cast<const QgsMarkerSymbolV2*>( symbol )->angle(); }

    void setDataDefined( QgsSymbolV2* symbol, const QgsDataDefined& dd ) override;
};


class GUI_EXPORT QgsDataDefinedWidthDialog : public QgsDataDefinedValueDialog
{
    Q_OBJECT
  public:
    QgsDataDefinedWidthDialog( const QList<QgsSymbolV2*>& symbolList, QgsVectorLayer * layer )
        : QgsDataDefinedValueDialog( symbolList, layer, tr( "Width" ) )
    {
      init( tr( "Symbol width" ) );
    }

  protected:
    QgsDataDefined symbolDataDefined( const QgsSymbolV2 * symbol ) const override;

    double value( const QgsSymbolV2 * symbol ) const override { return static_cast<const QgsLineSymbolV2*>( symbol )->width(); }

    void setDataDefined( QgsSymbolV2* symbol, const QgsDataDefined& dd ) override;
};



#endif // QGSRENDERERV2WIDGET_H

/* This file is part of the KDE libraries
   Copyright (C) 2000 Max Judin <novaprint@mtu-net.ru>
   Copyright (C) 2000 Falk Brettschneider <falk@kdevelop.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

/*
   activities:
   -----------
   05/2001 -               : useful patches, bugfixes by Christoph Cullmann <crossfire@babylon2k.de>,
                             Joseph Wenninger <jowenn@bigfoot.com> and  Falk Brettschneider
   03/2001 - 05/2001       : maintained and enhanced by Falk Brettschneider <falk@kdevelop.org>
   03/2000                 : class documentation added by Falk Brettschneider <gigafalk@yahoo.com>
   10/1999 - 03/2000       : programmed by Max Judin <novaprint@mtu-net.ru>

   C++ classes in this file:
   -------------------------
   - KDockWidgetAbstractHeader     - minor helper class
   - KDockWidgetAbstractHeaderDrag - minor helper class
   - KDockWidgetHeaderDrag         - drag panel in a dockwidget title bar
   - KDockWidgetHeader             - dockwidget title bar containing the drag panel
   - KDockTabGroup                 - minor helper class
   - KDockWidget                   - IMPORTANT CLASS: the one and only dockwidget class
   - KDockManager                  - helper class
   - KDockMainWindow               - IMPORTANT CLASS: a special KMainWindow that can have dockwidgets
   - KDockArea                     - like KDockMainWindow but inherits just QWidget
   
   IMPORTANT Note: This file compiles also in Qt-only mode by using the NO_KDE2 precompiler definition!
*/

#define _JOWENN_EXPERIMENTAL_


#ifndef KDOCKWIDGET_H
#define KDOCKWIDGET_H

#define _KDOCKWIDGET_2_2_

#include <QPoint>
#include <QList>
#include <QFrame>
#include <QDomImplementation>

#ifndef NO_KDE2
#include <kmainwindow.h>
#include <netwm_def.h>
#undef  EXPORT_DOCKCLASS
#define EXPORT_DOCKCLASS
#else
#include <QMainWindow>
#include "exportdockclass.h"
#include <kmainwindow.h>
#endif

#include "kdocktabctl.h"

class KDockSplitter;
class KDockManager;
class KDockMoveManager;
class KDockWidget;
class KDockButton_Private;
class KDockWidgetPrivate;
class KDockArea;

//class QObjectList;
class QMenu;
class QVBoxLayout;
class QHBoxLayout;
class QPixmap;

#ifndef NO_KDE2
class KToolBar;
class KConfig;
#else
class QToolBar;
#endif

// KDE 3.0 TODO: this belongs in a namespace!! (Clash with X11/Intrinsic.h!)
typedef QList<QWidget*> WidgetList;

/**
 * An abstract base clase for all dockwidget headers (and member of the dockwidget class set).
 * See the class description of @ref KDockWidgetHeader!
 * More or less a minor helper class for the dockwidget class set.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class KDockWidgetAbstractHeader : public QFrame
{
  Q_OBJECT
public:

  /**
   * Constructs this.
   *
   * @param parent the parent widget (usually a dockwidget)
   * @param name   the object instance name
   */
  KDockWidgetAbstractHeader( KDockWidget* parent, const QString name );

  /**
   * Destructs this.
   */
  virtual ~KDockWidgetAbstractHeader(){};

  /** Provides things concerning to switching to toplevel mode. Must be overridden by an inheriting class. */
  virtual void setTopLevel( bool ){};

private:
  class KDockWidgetAbstractHeaderPrivate;
  KDockWidgetAbstractHeaderPrivate *d;
};

/**
 * An abstract class for all dockwidget drag-panels of a dockwidgets (and member of the dockwidget class set).
 * See the class description of @ref KDockWidgetHeaderDrag!
 * More or less a minor helper class for the dockwidget class set.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class KDockWidgetAbstractHeaderDrag : public QFrame
{
  Q_OBJECT
public:

  /**
   * Constructs this.
   *
   * @param parent the parent widget (usually a dockwidget header)
   * @param dock   the dockwidget where it belongs to
   * @param name   the object instance name
   */
  KDockWidgetAbstractHeaderDrag( KDockWidgetAbstractHeader* parent,
                                 KDockWidget* dock, const QString name);

  /**
   * Destructs this.
   */
  virtual ~KDockWidgetAbstractHeaderDrag(){};

  /**
   * @return the dockwidget where this belongs to
   */
  KDockWidget* dockWidget(){ return dw; }

private:
  /** the dockwidget where this belongs to */
  KDockWidget* dw;
  class KDockWidgetAbstractHeaderDragPrivate;
  KDockWidgetAbstractHeaderDragPrivate *d;
};

/**
 * This special widget is the panel one can grip with the mouses (and member of the dockwidget class set).
 * The widget for dragging, so to speak.
 * Usually it is located in the @ref KDockWidgetHeader.
 * More or less a minor helper class for the dockwidget class set.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class KDockWidgetHeaderDrag : public KDockWidgetAbstractHeaderDrag
{
  Q_OBJECT
public:

  /**
   * Constructs this.
   *
   * @param parent the parent widget (usually a dockwidget header)
   * @param dock   the dockwidget where it belongs to
   * @param name   the object instance name
   */
  KDockWidgetHeaderDrag( KDockWidgetAbstractHeader* parent, KDockWidget* dock,
                         const QString name=QString() );

  /**
   * Destructs this.
   */
  virtual ~KDockWidgetHeaderDrag(){};

protected:

  /**
   * Draws the drag panel (a double line)
   */
  virtual void paintEvent( QPaintEvent* );

private:
  class KDockWidgetHeaderDragPrivate;
  KDockWidgetHeaderDragPrivate *d;
};

/**
 * The header (additional bar) for a @ref KDockWidget s (and member of the dockwidget class set).
 * It have got the buttons located there. And it is for recording and reading the button states.
 * More or less a minor helper class for the dockwidget class set.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class KDockWidgetHeader : public KDockWidgetAbstractHeader
{
  Q_OBJECT
public:

  /**
   * Constructs this.
   *
   * @param parent the parent widget (usually a dockwidget)
   * @param name   the object instance name
   */
  KDockWidgetHeader( KDockWidget* parent, const char* name = 0L );

  /**
   * Destructs this.
   */
  virtual ~KDockWidgetHeader(){};

  /**
   * Hides the close button and stay button when switching to toplevel or vice versa shows them.
   *
   * @param t toplevel or not
   */
  virtual void setTopLevel( bool t);

  /**
   * Set the drag panel of this header.
   *
   * @param nd A pointer to the new drag panel
   */
  void setDragPanel( KDockWidgetHeaderDrag* nd );

  bool dragEnabled();
  void setDragEnabled(bool b);

#ifndef NO_KDE2
  /**
   * Saves the current button state to a KDE config container object.
   *
   * @param c the configuration safe
   */
  virtual void saveConfig( KConfig* c);

  /**
   * Loads the current button state from a KDE config container object.
   *
   * @param c the configuration safe
   */
  virtual void loadConfig( KConfig* );
#endif

protected slots:
  /**
   * Set dragging the dockwidget off when the stay button is pressed down and vice versa.
   */
  void slotStayClicked();

protected:

  /** A layout manager for placing the embedded buttons (close and stay) */
  QHBoxLayout* layout;

  /** a little button for closing (undocking and hiding) the dockwidget */
  KDockButton_Private* closeButton;

  /** a little button for enabling/disabling dragging the dockwidget with the mouse */
  KDockButton_Private* stayButton;

  /** a little button for dock back the dockwidget to it's previous dockwidget */
  KDockButton_Private* dockbackButton;

  /** the drag panel (double line) */
  KDockWidgetHeaderDrag* drag;

private:
  class KDockWidgetHeaderPrivate;
  KDockWidgetHeaderPrivate *d;
};

/**
 * It just hides the special implementation of a dockwidget tab groups (and is member of the dockwidget class set).
 * An abstraction what it is currently.
 * In general it is like @ref QTabWidget but is more useful for the dockwidget class set.
 * More or less a minor helper class for the dockwidget class set.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class EXPORT_DOCKCLASS KDockTabGroup : public KDockTabCtl
{
  Q_OBJECT
public:
  /**
   * Constructs this. It just calls the method of the base class.
   */
  KDockTabGroup( QWidget *parent = 0, const char *name = 0 )
  :KDockTabCtl( parent, name ){};

  /**
   * Destructs a KDockTabGroup.
   */
  virtual ~KDockTabGroup(){};

private:
  class KDockTabGroupPrivate;
  KDockTabGroupPrivate *d;
};

/**
 * Floatable widget that can be dragged around with the mouse and
 * encapsulate the actual widgets (and member of the dockwidget class
 * set).
 *
 * You just grip the double-lined panel, tear it off its parent
 * widget, drag it somewhere and let it loose. Depending on the
 * position where you leave it, the dockwidget becomes a toplevel
 * window on the desktop (floating mode) or docks to a new widget
 * (dock mode).  + Note: A @ref KDockWidget can only be docked to a
 * @ref KDockWidget.
 *
 * If you want to use this kind of widget, your main application
 * window has to be a @ref KDockMainWindow.  That is because it has
 * got several additional dock management features, for instance a
 * @ref KDockManager that has an overview over all dockwidgets and and
 * a dockmovemanager (internal class) that handles the dock process.
 *
 * Usually you create an @ref KDockWidget that covers the actual widget in this way:
 * <PRE>
 * ...
 * KDockMainWindow* mainWidget;
 * ...
 * KDockWidget* dock = 0L;
 * dock = mainWidget->createDockWidget( "Any window caption", nicePixmap, 0L, i18n("window caption")); // 0L==no parent
 * QWidget* actualWidget = new QWidget( dock);
 * dock->setWidget( actualWidget); // embed it
 * dock->setToolTipString(i18n("That's me")); // available when appearing as tab page
 * ...
 * </PRE>
 *
 * See @ref KDockMainWindow how a dockwidget is docked in.
 *
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class EXPORT_DOCKCLASS KDockWidget: public QWidget
{
  Q_OBJECT
friend class KDockManager;
friend class KDockSplitter;
friend class KDockMainWindow;
friend class KDockArea;

public:
  /**
   * Construct a dockwidget.
   *
   * Initially, docking to another and docking to this is allowed for
   * every @p DockPosition.  It is supposed to be no (tab) group. It will
   * taken under control of its dockmanager.
   *
   * @param dockManager The responsible manager (dock helper)
   * @param name        Object instance name
   * @param pixmap      An icon (for instance shown when docked centered)
   * @param parent      Parent widget
   * @param strCaption  Title of the dockwidget window (shown when toplevel)
   * @param strTabPageLabel The title of the tab page (shown when in tab page mode), if it is "", only the icon will be shown, if it is 0L, the label is set to strCaption
   */
  KDockWidget( KDockManager* dockManager, const QString name,
               const QPixmap &pixmap, QWidget* parent = 0L, const QString& strCaption = 0L,
               const QString& strTabPageLabel = " ",  Qt::WindowFlags f = Qt::Widget);

  /**
   * Destructs a dockwidget.
   */
  virtual ~KDockWidget();

  /**
   * The possible positions where a dockwidget can dock to another dockwidget
   */
  enum DockPosition
  {
    DockNone   = 0,
    DockTop    = 0x0001,
    DockLeft   = 0x0002,
    DockRight  = 0x0004,
    DockBottom = 0x0008,
    DockCenter = 0x0010,
    DockDesktop= 0x0020,

    DockCorner = DockTop | DockLeft | DockRight | DockBottom,
    DockFullSite = DockCorner | DockCenter,
    DockFullDocking = DockFullSite | DockDesktop
  };

  /**
   * This is a key method of this class! Use it to dock dockwidgets to
   * another dockwidget at the right position within its
   * @ref KDockMainWindow or a toplevel dockwidget.
   *
   *
   * If the target is null, it will become a toplevel dockwidget at position pos;
   * Note: Docking to another dockwidget means exactly:
   * A new parent dockwidget will be created, that replaces the target dockwidget and contains another single helper widget (tab widget or panner)
   * which contains both dockwidgets, this and the target dockwidget. So consider parent<->child relationships change completely during such actions.
   *
   * @param  target The dockwidget to dock to
   * @param  dockPos One of the DockPositions this is going to dock to
   * @param  spliPos The split relation (in percent, or percent*100 in high resolution) between both dockwidgets, target and this
   * @param  pos The dock position, mainly of interest for docking to the desktop (as toplevel dockwidget)
   * @param  check Only for internal use;
   * @param  tabIndex The position index of the tab widget (when in tab page mode), -1 (default) means append
   * @return result The group dockwidget that replaces the target dockwidget and will be grandparent of target and @p this.
   */
  KDockWidget* manualDock( KDockWidget* target, DockPosition dockPos, int spliPos = 50, QPoint pos = QPoint(0,0), bool check = false, int tabIndex = -1);

  /**
   * Specify where it is either possible or impossible for this to dock to another dockwidget.
   *
   * @param pos An OR'ed set of @p DockPositions
   */
  void setEnableDocking( int pos );

  /**
   * @return Where it is either possible or impossible for this to dock to another dockwidget (an OR'ed set of DockPositions).
   */
  int enableDocking(){ return eDocking; }

  /**
   * Specify where it is either possible or impossible for another dockwidget to dock to this.
   *
   * @param pos An OR'ed set of @p DockPositions
   */
  void setDockSite( int pos ){ sDocking = pos;}

  /**
   * @return There it is either possible or impossible for another dockwidget to dock to this (an OR'ed set of @p DockPositions).
   */
  int dockSite(){ return sDocking; }

  /**
   * Set the embedded widget.
   *
   * A QLayout takes care about proper resizing, automatically.
   *
   * @param w The pointer to the dockwidget's child widget.
   */
  void setWidget( QWidget* w);

  /**
   * Get the embedded widget.
   *
   * @return The pointer to the dockwidget's child widget, 0L if there's no such child.
   */
  QWidget* getWidget() { return widget; };

  /**
   * Set the header of this dockwidget.
   *
   * A @ref QLayout takes care about proper resizing, automatically.
   * The header contains the drag panel, the close button and the stay button.
   *
   * @param ah A base class pointer to the dockwidget header
   */
  void setHeader( KDockWidgetAbstractHeader* ah);

  /**
   * Normally it simply shows the dockwidget.
   *
   * But additionally, if it is docked to a tab widget (@p DockCenter), it is set as the active (visible) tab page.
   */
  void makeDockVisible();

  /**
   * @return If it may be possible to hide this.
   *
   * There are reasons that it's impossible:
   * @li It is a (tab) group.
   * @li It is already invisible ;-)
   * @li The parent of this is the @ref KDockMainWindow.
   * @li It isn't able to dock to another widget.
   */
  bool mayBeHide();

  /**
   * @return If it may be possible to show this.
   * There are reasons that it's impossible:
   * @li It is a (tab) group.
   * @li It is already visible ;-)
   * @li The parent of this is the @p KDockMainWindow.
   */
  bool mayBeShow();

  /**
   * @return The dockmanager that is responsible for this.
   */
  KDockManager* dockManager(){ return manager; }

  /**
   * Stores a string for a tooltip.
   *
   * That tooltip string has only a meaning when this dockwidget is shown as tab page.
   * In this case the tooltip is shown when one holds the mouse cursor on the tab page header.
   * Such tooltip will for instance be useful, if you use only icons there.
   * Note: Setting an empty string switches the tooltip off.
   *
   * @param ttStr A string for the tooltip on the tab.
   */
  void setToolTipString(const QString& ttStr) { toolTipStr = ttStr; };

  /**
   * @return The tooltip string being shown on the appropriate tab page header when in dock-centered mode.
   */
  const QString& toolTipString() { return toolTipStr; };

  /**
   * @return result @p true, if a dockback is possible, otherwise @p false.
   */
  bool isDockBackPossible();

  /**
   * Set a string that is used for the label of the tab page when in tab page mode
   * @param label The new tab page label.
   */
  void setTabPageLabel( const QString& label) { tabPageTitle = label; };

  /**
   * @return A string that is used for the label of the tab page when in tab page mode.
   */
  const QString& tabPageLabel() { return tabPageTitle; };

  /**
   * Catches and processes some @ref QWidget events that are interesting for dockwidgets.
   */
  virtual bool event( QEvent * );

  /**
   * Add dockwidget management actions to @ref QWidget::show.
   */
  virtual void show();
  /**
   * @return the parent widget of this if it inherits class KDockTabGroup
   */
  KDockTabGroup* parentDockTabGroup() { return parentTabGroup(); };

#ifndef NO_KDE2

    /**
   * Sets the type of the dock window
   *
   * @param windowType is type of dock window
   */
  void setDockWindowType (NET::WindowType windowType);

#endif

 /**
   * Sets the type of the dock window
   *
   * @param windowType is type of dock window
   */
  void setDockWindowTransient (QWidget *parent, bool transientEnabled);

public slots:
  /** Docks a dockwidget back to the dockwidget that was the neighbor widget before the current dock position. */
  void dockBack();

  /**
   * Toggles the visibility state of the dockwidget if it is able to be shown or to be hidden.
   *
   */
  void changeHideShowState();

  /**
   * Undocks this. It means it becomes a toplevel widget framed by the system window manager.
   * A small panel at the top of this undocked widget gives the possibility to drag it into
   * another dockwidget by mouse (docking).
   */
  void undock();

protected:

  /**
   * @return the parent widget of this if it inherits class KDockTabGroup
   * Warning: This will be removed in KDE-3.0. Use parentDockTabGroup instead.
   */
  KDockTabGroup* parentTabGroup();

  /**
   * Check some conditions and show or hide the dockwidget header (drag panel).
   * The header is hidden if:
   * @li the parent widget is the KDockMainWindow
   * @li this is a (tab) group dockwidget
   * @li it is not able to dock to another dockwidget
   */
  void updateHeader();
  
signals:
  /**
   * Emits that another dockwidget is docking to this.
   *
   * @param dw the dockwidget that is docking to this
   * @param dp the DockPosition where it wants to dock to
   */
  void docking( KDockWidget* dw, KDockWidget::DockPosition dp);

  /**
   * Signals that the dock default position is set.
   */
  void setDockDefaultPos();

  /**
   * Signals that the close button of the panel (@ref KDockWidgetHeader) has been clicked.
   */
  void headerCloseButtonClicked();

  /**
   * Signals that the dockback button of the panel (@ref KDockWidgetHeader) has been clicked.
   */
  void headerDockbackButtonClicked();

  /**
   * Signals that the widget processes a close event.
   */
  void iMBeingClosed();
  /**
   * Signals that the widget is undocked now.
   */
  void hasUndocked();

#ifdef Q_OS_MACOS
   /**
    * return local menubar for MacOS native menubar workaround
    */
  QMenuBar *menuBar() {return m_localMenuBar;};
#endif

protected slots:

  /** Does several things here when it has noticed that the former brother widget (closest neighbor) gets lost.
   * The former brother widget is needed for a possible dockback action, to speak with the Beatles:
   * "To get back to where you once belonged" ;-)
   */
  void loseFormerBrotherDockWidget();

protected:
  /** earlier closest neighbor widget, so it's possible to dock back to it. */
  KDockWidget* formerBrotherDockWidget;
  /** the current dock position. */
  DockPosition currentDockPos;
  /** the former dock position when it really was at another position before. */
  DockPosition formerDockPos;
  /** a string used as tooltip for the tab page header when in dock-centered mode. */
  QString toolTipStr;
  /** a string used as title of the tab page when in tab page mode */
  QString tabPageTitle;

private:
  /**
   * Sets the caption (window title) of the given tab widget.
   *
   * @param g the group (tab) widget
   */
  void setDockTabName( KDockTabGroup* g);

  /**
   * Reparent to s or set this to the KDockMainWindow's view if s is that dockmainwindow.
   * If s is O, simply move the widget.
   *
   * @param s the target widget to reparent to
   * @param p the point to move to (if it doesn't reparent)
   */
  void applyToWidget( QWidget* s, const QPoint& p  = QPoint(0,0) );

  /** A base class pointer to the header of this dockwidget */
  KDockWidgetAbstractHeader* header;

  /** the embedded widget */
  QWidget* widget;

  /** the layout manager that takes care about proper resizing and moving the embedded widget and the header */
  QVBoxLayout* layout;

  /** the responsible dockmanager */
  KDockManager* manager;

  /** an icon for the tab widget header */
  QPixmap* pix;

#ifdef Q_OS_MACOS
  /** menubar for MacOS native menubar workaround */
  QMenuBar *m_localMenuBar;
#endif

  /**
   * Information about the ability for docking to another dockwidget.
   */
  int eDocking;

  /**
   * Information which site of this dockwidget is free for docking of other dockwidgets.
   */
  int sDocking;

  /**
   * Previous side (left,right,top,bottom) where this dockwidget was before a dragging action, none if it wasn't dragged before.
   */
  KDockWidget::DockPosition prevSideDockPosBeforeDrag;

  // GROUP data
  QString firstName;
  QString lastName;
  Qt::Orientation splitterOrientation;
  bool isGroup;
  bool isTabGroup;

  KDockWidgetPrivate *d;
};

/**
 * The manager that knows all dockwidgets and handles the dock process (and member of the dockwidget class set).
 * More or less a helper class for the KDockWidget class set but of interest for some functionality
 * that can be called within a @ref KDockMainWindow or a @ref KDockWidget .
 *
 * An important feature is the ability to read or save the current state of all things concerning to
 * dockwidgets to @ref KConfig .
 *
 * The dockmanager is also often used when a certain dockwidget or a child of such dockwidget must be found.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class EXPORT_DOCKCLASS KDockManager: public QObject
{
  Q_OBJECT
friend class KDockWidget;
friend class KDockMainWindow;

public:
  /**
   * Constructs a dockmanager. Some initialization happen:
   * @li It installs an event filter for the main window,
   * @li a control list for dock objects
   * @li a control list for menu items concerning to menus provided by the dockmanager
   * @li Some state variables are set
   *
   * @param mainWindow the main window controlled by this
   * @param name the internal QOject name
   */
  KDockManager( QWidget* mainWindow, const QString name );

  /**
   * Destructs a dockmanager.
   */
  virtual ~KDockManager();

  /**
   * Saves the current dock window layout into a DOM tree below the given element.
   */
  void writeConfig(QDomElement &base);
  /**
   * Reads the current dock window layout from a DOM tree below the given element.
   */
  void readConfig(QDomElement &base);

  /**
   * Shows all encapsulated widgets of all controlled dockwidgets and shows all dockwidgets which are
   * parent of a dockwidget tab group.
   */
  void activate();

  /**
   * It's more or less a method that catches several events which are interesting for the dockmanager.
   * Mainly mouse events during the drag process of a dockwidgets are of interest here.
   *
   * @param _ the object that sends the event
   * @param _ the event
   * @return the return value of the method call of the base class method
   */
  virtual bool eventFilter( QObject *, QEvent * );

  /**
   * This method finds out what a widgets' dockwidget is. That means the dockmanager has a look at all
   * dockwidgets it knows and tells you when one of those dockwidgets covers the given widget.
   *
   * @param w any widget that is supposed to be encapsulated by one of the controlled dockwidgets
   * @return the dockwidget that encapsulates that widget, otherwise 0
   */
  KDockWidget* findWidgetParentDock( QWidget* w);

  /**
   * Works like makeDockVisible() but can be called for widgets that covered by a dockwidget.
   *
   * @param w the widget that is encapsulated by a dockwidget that turns to visible.
   */
  void makeWidgetDockVisible( QWidget* w ){ findWidgetParentDock(w)->makeDockVisible(); }

  /**
   * @return the popupmenu for showing/hiding dockwidgets
   */
  QMenu* dockHideShowMenu(){ return menu; }

  /**
   * @param dockName an internal QObject name
   * @return the dockwidget that has got that internal QObject name
   */
  KDockWidget* getDockWidgetFromName( const QString& dockName );

  /**
   * Enables opaque resizing. Opaque resizing is initially turned off.
   * Call this method before you create any dock widgets!   
   */
  void setSplitterOpaqueResize(bool b=true);

  /**
   * Returns TRUE if opaque resizing is enabled, FALSE otherwise.
   */
  bool splitterOpaqueResize() const;

  /**
   * Try to preserve the widget's size. Works like KeepSize resize mode
   * of QSplitter. Off by default.
   * Call this method before you create any dock widgets!
   */
  void setSplitterKeepSize(bool b=true);

  /**
   * Returns TRUE if the KeepSize is enabled, FALSE otherwise.
   */
  bool splitterKeepSize() const;

  /**
   * Operate the splitter with a higher resolution. Off by default.
   * Call this method before you create any dock widgets!
   * If high resolution is used all splitter position parameters
   * are percent*100 instead of percent.
   */
  void setSplitterHighResolution(bool b=true);

  /**
   * Returns TRUE if the splitter uses the high resolution, FALSE otherwise.
   */
  bool splitterHighResolution() const;

signals:

  /**
   * Signals changes of the docking state of a dockwidget. Usually the dock-toolbar will be updated then.
   */
  void change();

  /**
   * Signals a dockwidget is replaced with another one.
   */
  void replaceDock( KDockWidget* oldDock, KDockWidget* newDock );

  /**
   * Signals a dockwidget without parent (toplevel) is shown.
   */
  void setDockDefaultPos( KDockWidget* );

private slots:

  /**
   * Clears the popupmenu for showing/hiding dockwidgets and fills it with the current states of all controlled dockwidgets.
   */
  void slotMenuPopup();

  /**
   * This method assumes a menuitem of the popupmenu for showing/hiding dockwidgets is selected and toggles that state.
   *
   * @param action the popupmenu QAction of the selected menuitem
   */
  void slotMenuActivated( QAction * action);

  /* clears the old drawn drag rectangle (oldDragRect) from screen and
   * draws the new current drag rectangle (dragRect) depending on the current mouse position.
   * This highlights the dockwidget which is the currently chosen target during a dock action.
   */
  void drawDragRectangle();

private:

  /**
   * A data structure containing data about every dockwidget that is under control.
   */
  struct MenuDockData
  {
    MenuDockData( KDockWidget* _dock, bool _hide )
    {
      dock = _dock;
      hide = _hide;
    };
    ~MenuDockData(){};

    KDockWidget* dock;
    bool hide;
  };

  /**
   * Finds the KDockWidget at the position given as parameter
   *
   * @param pos global (desktop) position of the wanted dockwidget
   * @return the dockwidget at that position
   */
  KDockWidget* findDockWidgetAt( const QPoint& pos );

  /**
   * Finds the QWidget recursively at the position given as parameter
   *
   * @param w a variable where the method puts the QWidget at that position (instead of a return value)
   * @param p the parent widget where the recursive search should start from
   * @param pos global (desktop) position of the wanted dockwidget
   */
  void findChildDockWidget( QWidget*& w, const QWidget* p, const QPoint& pos );

  /**
   * Finds all dockwidgets which are child, grandchild and so on of p.
   *
   * @param p the parent widget where the recursive search starts from
   * @param l the widget list that contains the search result after the return of this method
   */
  void findChildDockWidget( const QWidget* p, WidgetList*& l);

  /**
   * Sets a dockwidget in drag mode.
   */
  void startDrag( KDockWidget* );

  /**
   * Moves a dockwidget that is in drag mode.
   *
   * @param d the dockwidget which is dragged
   * @param pos the new position of the dragged dockwidget
   */
  void dragMove( KDockWidget* d, QPoint pos );

  /**
   * Aborts the drag mode. Restores the cursor and hides the drag indicator.
   */
  void cancelDrop();

  /**
   * Finishes the drag mode. If the user let it drop on an other dockwidget, it will possibly be docked (if allowed),
   * if the user drops it outside of the application window it becomes toplevel.
   */
  void drop();

// class members

  /**
   * Usually the KDockMainWindow but not necessarily.
   */
  QWidget* main;

  /**
   * Dead. Scheduled for removing in 3.0. Do not use.
   */
  KDockMoveManager* mg;

  /**
   * The dockwidget that is being dragged at the moment
   */
  KDockWidget* currentDragWidget;

  /**
   * The target dockwidget where the currentDragWidget is dropped
   */
  KDockWidget* currentMoveWidget; // widget where mouse moving

  /**
   * It is of interest during the dock process. Then it contains all child dockwidgets.
   */
  WidgetList* childDockWidgetList;

  /**
   * The dockposition where the dockwidget would be docked to, if we dropped it here.
   */
  KDockWidget::DockPosition curPos;

  /**
   * A QList of all objects that are important for docking.
   * Some serve as group widgets of dockwidgets, others encapsulate normal widgets.
   */
  QObjectList* childDock;

  /**
   * Contains dockwidgets that are created automatically by the dockmanager. For internal use.
   */
  QObjectList* autoCreateDock;

  /**
   * For storing the width during the dragging of a dockwidget.
   */
  int storeW;

  /**
   *  For storing the height during the dragging of a dockwidget.
   */
  int storeH;

  /**
   * State variable if there is a drag process active.
   */
  bool draging;

  /**
   * State variable if there is an undock process active
   */
  bool undockProcess;

  /**
   * The dockmanager sets it to true if the user cancels the drag by moving the cursor
   * on a invalid drop place
   */
  bool dropCancel;

  /**
   * A popup menu that contains one menuitem for each dockwidget that shows the current visibility state and
   * to show or hide the appropriate dockwidget.
   */
  QMenu* menu;

  /**
   * An internal list containing data for the menuitems for the visibility popup menu.
   */
  //QList<MenuDockData*> *menuData;
    QMap<QAction*, MenuDockData*> menuData;

  class KDockManagerPrivate;
  KDockManagerPrivate *d;
};

/**
 * A special kind of @ref KMainWindow that is able to have dockwidget child widgets (and member of the dockwidget class set).
 *
 * The main widget should be a @ref KDockWidget where other @ref KDockWidget can be docked to
 * the left, right, top, bottom or to the middle.
 * Note: dock to the middle means to drop on a dockwidget and to unite them to a new widget, a tab control.
 * That tab widget is a @ref KDockTabCtl .
 *
 * Furthermore, the KDockMainWindow has got the @ref KDocManager and some data about the dock states.
 *
 * If you've got some dockwidgets, you can dock them to the dockmainwindow to initialize a start scene:
 * Here an example:
 * <PRE>
 * DockApplication::DockApplication( const char* name) : KDockMainWindow( name)
 * {
 *   ...
 *   KDockWidget* mainDock;
 *   mainDock = createDockWidget( "Falk's MainDockWidget", mainPixmap, 0L, "main_dock_widget");
 *   AnyContentsWidget* cw = new AnyContentsWidget( mainDock);
 *   mainDock->setWidget( cw);
 *   // allow others to dock to the 4 sides
 *   mainDock->setDockSite(KDockWidget::DockCorner);
 *   // forbit docking abilities of mainDock itself
 *   mainDock->setEnableDocking(KDockWidget::DockNone);
 *   setView( mainDock); // central widget in a KDE mainwindow
 *   setMainDockWidget( mainDock); // master dockwidget
 *   ...
 *   KDockWidget* dockLeft;
 *   dockLeft = createDockWidget( "Intially left one", anyOtherPixmap, 0L, i18n("The left dockwidget"));
 *   AnotherWidget* aw = new AnotherWidget( dockLeft);
 *   dockLeft->setWidget( aw);
 *   dockLeft->manualDock( mainDock,              // dock target
 *                         KDockWidget::DockLeft, // dock site
 *                         20 );                  // relation target/this (in percent)
 *   ...
 * </PRE>
 *
 * Docking is fully dynamical at runtime. That means you can always move dockwidgets via drag and drop.
 *
 * And last but not least you can use the popupmenu for showing or hiding any controlled dockwidget
 * of this class and insert it to your main menu bar or anywhere else.
 *
 * @author Max Judin (documentation: Falk Brettschneider).
 */
class EXPORT_DOCKCLASS KDockMainWindow : public KMainWindow
{
  Q_OBJECT

friend class KDockManager;

public:

  /**
   * Constructs a dockmainwindow. It calls its base class constructor and does additional things concerning
   * to the dock stuff:
   * @li information about the dock state of this' children gets initialized
   * @li a dockmanager is created...
   * @li ...and gets initialized
   * @li the main dockwidget is set to 0
   *
   * @param name object name
   */
  KDockMainWindow( QWidget* parent = 0L, const QString name=QString(), Qt::WindowFlags f = Qt::Window,
                   QList<Qt::WidgetAttribute> a = (QList<Qt::WidgetAttribute>() << Qt::WA_DeleteOnClose));

  /**
   * Destructs a dockmainwindow.
   */
  virtual ~KDockMainWindow();

  /**
   * Returns the dockmanager of this. (see @ref KDockManager)
   * @return pointer to the wanted dockmanager
   */
  KDockManager* manager(){ return dockManager; }

  /**
   * Sets a new main dockwidget.
   * Additionally, the toolbar is re-initialized.
   *
   * @param _ dockwidget that become the new main dockwidget
   */
  void setMainDockWidget( KDockWidget* );

  /**
   * Returns the main dockwidget.
   *
   * @return pointer to the main dockwidget
   */
  KDockWidget* getMainDockWidget(){ return mainDockWidget; }

  /**
   * This is one of the most important methods!
   * The KDockMainWindow creates a new dockwidget object here that usually should encapsulate the user's widget.
   * The new dockwidget is automatically taken under control by the dockmanager of the dockmainwindow.
   *
   * @param name   QObject name (default dockwidget caption)
   * @param pixmap window icon (for instance shown when docked as tabwidget entry)
   * @param parent parent widget for the new dockwidget
   * @param strCaption  window title (shown when toplevel)
   * @param strTabPageLabel title of the tab page (visible when in tab page mode), if it is "", only the icon will be shown; if it is 0L, the label is set to strCaption
   * @return    a pointer to the new created dockwidget
   */
  KDockWidget* createDockWidget( const QString& name, const QPixmap &pixmap, QWidget* parent = 0L, const QString& strCaption = 0L, const QString& strTabPageLabel = " ");

  /**
   * Saves the current dock window layout into a DOM tree below the given element.
   */
  void writeDockConfig(QDomElement &base);
  /**
   * Reads the current dock window layout from a DOM tree below the given element.
   */
  void readDockConfig(QDomElement &base);

#ifndef NO_KDE2
  /**
   * It writes the current dock state in the given section of KConfig.
   *
   * @param c     KDE class for saving configurations
   * @param group name of section to write to
   */
  void writeDockConfig( KConfig* c = 0L, QString group = QString() );

  /**
   * It reads the current dock state from the given section of KConfig.
   *
   * @param c     KDE class for saving configurations
   * @param group name of section to read from
   */
  void readDockConfig ( KConfig* c = 0L, QString group = QString() );
#endif

  /**
   * It runs through all dockwidgets which are under control of the dockmanager and calls show() for every
   * encapsulated widget and show() for the dockwidget itself if it is not in tab mode.
   * Additionally, if the main dockwidget is not a QDialog, it will be shown.
   */
  void activateDock(){ dockManager->activate(); }

  /**
   * Returns a popup menu that contains entries for all controlled dockwidgets making hiding and showing
   * them possible.
   *
   * @return the wanted popup menu
   */
  QMenu* dockHideShowMenu(){ return dockManager->dockHideShowMenu(); }

  /**
   * This method shows the given dockwidget.
   * The clue is that it also considers the dockwidget could be a tab page
   * and must set to be the activate one.
   *
   * @param dock the dockwidget that is to be shown
   */
  void makeDockVisible( KDockWidget* dock );

  /**
   * This method hides the given dockwidget.
   *
   * @param dock the dockwidget that is to be shown
   */
  void makeDockInvisible( KDockWidget* dock );

  /**
   * This is an overloaded member function, provided for convenience.
   * It differs from the above function only in what argument(s) it accepts.
   */
  void makeWidgetDockVisible( QWidget* widget );

  /**
   * This method calls the base class method.
   * If the given widget inherits KDockWidget, applyToWidget(this) is called.
   *
   * @param _ any widget that should become the main view
   */
  void setView( QWidget* );

signals:
  /**
  * Signals a certain dockwidget is undocked now.
  */
  void dockWidgetHasUndocked(KDockWidget*);

protected:

  /** A pointer to the main dockwidget (where one can manualDock() to */
  KDockWidget* mainDockWidget;

  /** A pointer to the manager for the dock process */
  KDockManager* dockManager;

protected slots:
  /**
  * Called whenever one of the dockwidgets of this has been undocked.
  */
  void slotDockWidgetUndocked();

private:
  class KDockMainWindowPrivate;
  KDockMainWindowPrivate *d;
};















#ifdef _JOWENN_EXPERIMENTAL_
/* Joseph Wenninger jowenn@kde.org Experimental (Just all KMainWindow references changed to QWidget, otherwise nearly exactly the 
same as KDockMainWindow*/

class EXPORT_DOCKCLASS KDockArea : public QWidget
{
  Q_OBJECT

friend class KDockManager;

public:


  KDockArea( QWidget* parent = 0L, const QString name = QString());

  virtual ~KDockArea();

  KDockManager* manager(){ return dockManager; }


  void setMainDockWidget( KDockWidget* );
  KDockWidget* getMainDockWidget(){ return mainDockWidget; }

  KDockWidget* createDockWidget( const QString& name, const QPixmap &pixmap, QWidget* parent = 0L, const QString& strCaption = 0L, const QString& strTabPageLabel = " ");

  void writeDockConfig(QDomElement &base);
  void readDockConfig(QDomElement &base);

#ifndef NO_KDE2
  void writeDockConfig( KConfig* c = 0L, QString group = QString() );
  void readDockConfig ( KConfig* c = 0L, QString group = QString() );
#endif



  void activateDock(){ dockManager->activate(); }
  QMenu* dockHideShowMenu(){ return dockManager->dockHideShowMenu(); }
  void makeDockVisible( KDockWidget* dock );
  void makeDockInvisible( KDockWidget* dock );
  void makeWidgetDockVisible( QWidget* widget );
  //void setView( QWidget* );

signals:
  /**
  * Signals a certain dockwidget is undocked now.
  */
  void dockWidgetHasUndocked(KDockWidget*);

protected:

  KDockWidget* mainDockWidget;
  KDockManager* dockManager;

protected slots:
  void slotDockWidgetUndocked();

protected:
  virtual void resizeEvent( QResizeEvent *);

private:
  class KDockMainWindowPrivate;
  KDockMainWindowPrivate *d;
};

#endif

#endif



#ifndef WWIDGETSTACK_H
#define WWIDGETSTACK_H

#include <QWidget>
#include <QObject>
#include <QSignalMapper>
#include <QStackedWidget>
#include <QEvent>

#include "controlobject.h"
#include "controlobjectthread.h"
#include "widget/wbasewidget.h"

class WidgetStackControlListener : public QObject {
    Q_OBJECT
  public:
    WidgetStackControlListener(QObject* pParent, ControlObject* pControl,
                               int index);
    virtual ~WidgetStackControlListener();

  signals:
    void switchToWidget();
    void hideWidget();

  public slots:
    void onCurrentWidgetChanged(int index);

  private slots:
    void slotValueChanged(double v);

  private:
    ControlObjectThread m_control;
    const int m_index;
};

class WWidgetStack : public QStackedWidget, public WBaseWidget {
    Q_OBJECT
  public:
    WWidgetStack(QWidget* pParent,
                 ControlObject* pNextControl,
                 ControlObject* pPrevControl,
                 ControlObject* pCurrentPageControl);
    virtual ~WWidgetStack();

    // We don't want to change pages until all the pages have been added,
    // so we override Init and hook up the connection there.
    virtual void Init();

    // QStackedWidget sizeHint and minimumSizeHint are the largest of all the
    // widgets in the stack. This is presumably to prevent UI resizes when the
    // stack changes. We explicitly want the UI to change when the stack changes
    // (potentially grow or shrink).
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void addWidgetWithControl(QWidget* pWidget, ControlObject* pControl);

  protected:
    bool event(QEvent* pEvent);

  private slots:
    void onNextControlChanged(double v);
    void onPrevControlChanged(double v);
    // Fired when the control object tells us to change pages.
    void onCurrentPageControlChanged(double v);
    // Fired when we change pages.
    void onCurrentPageChanged(int);
    void hideIndex(int index);

  private:
    QSignalMapper m_showMapper;
    QSignalMapper m_hideMapper;
    ControlObjectThread m_nextControl;
    ControlObjectThread m_prevControl;
    ControlObjectThread m_currentPageControl;
};

#endif /* WWIDGETSTACK_H */

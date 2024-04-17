#pragma once

#include <QObject>
#include <QMouseEvent>
#include <QKeyEvent>

class ModelEditingGraphicsView;

class ModelEditingState : public QObject {
    Q_OBJECT
    friend ModelEditingGraphicsView;

private:
    ModelEditingGraphicsView* view;

protected:
    /// called when raise() is called
    virtual void onRaise() = 0;
    /// called when enter(false) is called
    virtual void onDrop();
    /// called when this state is current state of view and changeState() is called
    virtual void onReplace() = 0;

    virtual void onViewMouseMoveEvent       (QMouseEvent *event);
    virtual void onViewMousePressEvent      (QMouseEvent *event);
    virtual void onViewMouseDoubleClickEvent(QMouseEvent *event);
    virtual void onViewMouseReleaseEvent    (QMouseEvent *event);
    virtual void onViewKeyPressEvent  (QKeyEvent *event);
    virtual void onViewKeyReleaseEvent(QKeyEvent *event);

public:
    explicit ModelEditingState(ModelEditingGraphicsView* _view);

    ModelEditingGraphicsView *getView() const;
    void setView(ModelEditingGraphicsView *newView);

public slots:
    void raise();

    /// true -> raise() -> onRaise(), false -> onDrop(), calls entering(bool)
    void enter(bool doRaise);

signals:
    void raised();
    void entering(bool didRaise);
    void dropped();
    // triggered when this state is current state of view and changeState() is called
    void replaced();
};

class EmptyEditingState : public ModelEditingState {
    Q_OBJECT

public:
    EmptyEditingState(ModelEditingGraphicsView* _view) : ModelEditingState(_view) {}

    void onRaise()   override {}
    void onReplace() override {}
};

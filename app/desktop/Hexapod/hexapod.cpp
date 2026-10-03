#include "hexapod.h"
#include "ui_hexapod.h"

Hexapod::Hexapod(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hexapod)
{
    ui->setupUi(this);
    ui->setupUi(this);

    setFocusPolicy(Qt::StrongFocus);

    ui->btn_w->setCheckable(true);
    ui->btn_s->setCheckable(true);
    ui->btn_a->setCheckable(true);
    ui->btn_d->setCheckable(true);
    ui->btn_b->setCheckable(true);

    // Чтобы сами кнопки не утаскивали фокус клавиатуры
    ui->btn_w->setFocusPolicy(Qt::NoFocus);
    ui->btn_s->setFocusPolicy(Qt::NoFocus);
    ui->btn_a->setFocusPolicy(Qt::NoFocus);
    ui->btn_d->setFocusPolicy(Qt::NoFocus);
    ui->btn_b->setFocusPolicy(Qt::NoFocus);

    view3d = new QQuickWidget(ui->preview);
    view3d->setResizeMode(QQuickWidget::SizeRootObjectToView);
    connect(view3d, &QQuickWidget::statusChanged, this, [this](QQuickWidget::Status status){
        qDebug() << "QML status:" << status;

        if(status == QQuickWidget::Error){
            for(const QQmlError &error : view3d->errors()){
                qDebug() << error.toString();
            }
        }
    });

    view3d->setSource(QUrl("qrc:/HexapodView.qml"));
    if(view3d->status() == QQuickWidget::Error){
        for(const QQmlError &error : view3d->errors()){
            qDebug() << error.toString();
        }
    }

    QVBoxLayout *layout = new QVBoxLayout(ui->preview);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(view3d);
}

Hexapod::~Hexapod()
{
    delete ui;
}

void Hexapod::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        return;

    switch(event->key()){
        case Qt::Key_B:
            bPressed = true;
            break;

        case Qt::Key_W:
            wPressed = true;
            break;

        case Qt::Key_S:
            sPressed = true;
            break;

        case Qt::Key_A:
            aPressed = true;
            break;

        case Qt::Key_D:
            dPressed = true;
            break;

        default:
            QWidget::keyPressEvent(event);
            return;
    }

    updateMoveState();
}


void Hexapod::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        return;

    switch(event->key()){

        case Qt::Key_B:
            bPressed = false;
            break;

        case Qt::Key_W:
            wPressed = false;
            break;

        case Qt::Key_S:
            sPressed = false;
            break;

        case Qt::Key_A:
            aPressed = false;
            break;

        case Qt::Key_D:
            dPressed = false;
            break;

        default:
            QWidget::keyReleaseEvent(event);
            return;
    }

    updateMoveState();
}


void Hexapod::updateMoveState(){
    uint8_t newMove = MOVE_NONE;

    /*
     * BASE имеет приоритет над WASD.
     */
    if(bPressed){
        newMove = MOVE_BASE;
    }
    else{

        if(wPressed) newMove |= MOVE_W;
        if(sPressed) newMove |= MOVE_S;
        if(aPressed) newMove |= MOVE_A;
        if(dPressed) newMove |= MOVE_D;
    }

    moveState = newMove;

    ui->btn_w->setChecked(wPressed);
    ui->btn_s->setChecked(sPressed);
    ui->btn_a->setChecked(aPressed);
    ui->btn_d->setChecked(dPressed);
    ui->btn_b->setChecked(bPressed);

    emit moveChanged(moveState);
}



void Hexapod::focusOutEvent(QFocusEvent *event)
{
    moveState = 0;

    updateMoveState();

    QWidget::focusOutEvent(event);
}


void Hexapod::updateHexapodModel(LegAngles_t new_angles[6]){
    QQuickItem *root = view3d->rootObject();

    if(!root){
        return;
    }

    for(uint8_t i = 0; i < 6u; i++){
        bool ok = QMetaObject::invokeMethod(
            root,
            "setLegAngles",
            Q_ARG(QVariant, (int)i + 1),
            Q_ARG(QVariant, (int)new_angles[i].coxa),
            Q_ARG(QVariant, (int)new_angles[i].femur),
            Q_ARG(QVariant, (int)new_angles[i].tibia)
            );

        qDebug() << "INVOKE"
                 << i + 1
                 << ok
                 << new_angles[i].coxa
                 << new_angles[i].femur
                 << new_angles[i].tibia;
    }
}

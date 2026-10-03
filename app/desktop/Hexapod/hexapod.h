#ifndef HEXAPOD_H
#define HEXAPOD_H

#include <QWidget>
#include <QKeyEvent>
#include <QQuickWidget>
#include <QQuickItem>
#include <QVBoxLayout>
#include <QQmlError>

typedef struct{
    int16_t coxa;
    int16_t femur;
    int16_t tibia;
} LegAngles_t;


namespace Ui {
class Hexapod;
}

class Hexapod : public QWidget
{
    Q_OBJECT

public:
    explicit Hexapod(QWidget *parent = nullptr);
    void updateHexapodModel(LegAngles_t new_angles[6]);
    ~Hexapod();
signals:
    void moveChanged(uint8_t move);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void updateMoveState();
private:
    uint8_t moveState = 0;
    LegAngles_t current_position[6];
    bool wPressed = false,
         aPressed = false,
         sPressed = false,
         dPressed = false,
         bPressed = false;
    QQuickWidget *view3d;
    static constexpr uint8_t MOVE_NONE = 0u << 0;
    static constexpr uint8_t MOVE_W    = 1u << 0;
    static constexpr uint8_t MOVE_S    = 1u << 1;
    static constexpr uint8_t MOVE_A    = 1u << 2;
    static constexpr uint8_t MOVE_D    = 1u << 3;
    static constexpr uint8_t MOVE_BASE = 1u << 4;
    Ui::Hexapod *ui;
};

#endif // HEXAPOD_H

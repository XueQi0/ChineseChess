#ifndef BOARD_H
#define BOARD_H
#include <QWidget>
#include <QPainter>
#include "stone.h"
#include <QMouseEvent>
#include <QLabel>
#include "step.h"

/*宏定义*/
#define G(i) (i)*mGap
#define LINE(x1,y1,x2,y2) drawLine(QPoint((x1)+mOffset.x(),(y1)+mOffset.y()),QPoint((x2)+mOffset.x(),(y2)+mOffset.y()))

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);

    //棋子状态
    enum CHESS{
        INBOARD = -1, OUTBOARD = -2, EMPTY = -3, UNSELECT = -4, DEAD = -5, UNSTART = -6
    };
    //方向
    enum DIRECTION{
      UP, DOWN, LEFT, RIGHT
    };

    /*功能*/
    void placeStone();//摆放棋子
    void drawChessBoard(QPainter &painter);
    void drawStone(QPainter &painter, int id);
    void getRowCol(QPoint pt, int &col, int &row);
    int getPointId(int col, int row);//得到坐标id
    bool isOut(int col, int row);//是否越界
    bool canMove(int selectId, int nCol, int nRow);
    bool canSHIMove(int selectId, int nCol, int nRow);
    bool canBINGMove(int selectId, int nCol, int nRow);
    bool canXIANGMove(int selectId, int nCol, int nRow);
    bool canMAMove(int selectId, int nCol, int nRow);
    bool canPAOMove(int selectId, int nCol, int nRow);
    bool canCHEMove(int selectId, int nCol, int nRow);
    bool canJIANGMove(int selectId, int nCol, int nRow);
    int distance(int x1, int y1, int x2, int y2);
    void swap(int &a, int &b);//交换
    void moveStone(int col, int row);
    bool isOver();
    void saveStep(int selectId, int col, int row, QVector<Step*>& steps);


    /*事件*/
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    /*成员*/
    int mGap;
    int mR;
    int mSelectId;
    int mNextId;
    int mPreviousId;

    bool mStartGame;
    bool mRedTurn;

    QPoint mOffset;

    Stone mS[32];

    QLabel *winner;

    QVector<Step*> mSteps;

private:

signals:

public slots:
};

#endif // BOARD_H

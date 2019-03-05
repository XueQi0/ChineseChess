#include "board.h"
#include <QPainter>

Board::Board(QWidget *parent) : QWidget(parent)
{
    //初始化参数
    mStartGame = true;
    mRedTurn = true;
    mOffset = QPoint(60, 60);
    mGap = 40;
    mR = G(0.5);
    mSelectId = UNSELECT;
    mNextId = UNSTART;
    mPreviousId = UNSTART;
    setGeometry(180, 120, 570, 490);
    setMaximumSize(570, 490);
    setMinimumSize(570, 490);
    winner = new QLabel(this);
    winner->setGeometry(440, 120, 200, 24);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::white);
    winner->setPalette(pe);
    winner->setFont(QFont("仿宋", mR, QFont::Bold));
    winner->setAlignment(Qt::AlignLeft);
    winner->setText("红方走");

    placeStone();
}

void Board::placeStone()//摆放棋子
{
    mS[0].initStone(0, 0, 0, true, Stone::CHE);
    mS[1].initStone(1, 0, 1, true, Stone::MA);
    mS[2].initStone(2, 0, 2, true, Stone::XIANG);
    mS[3].initStone(3, 0, 3, true, Stone::SHI);
    mS[4].initStone(4, 0, 4, true, Stone::JIANG);
    mS[5].initStone(5, 0, 5, true, Stone::SHI);
    mS[6].initStone(6, 0, 6, true, Stone::XIANG);
    mS[7].initStone(7, 0, 7, true, Stone::MA);
    mS[8].initStone(8, 0, 8, true, Stone::CHE);
    mS[9].initStone(1, 2, 9, true, Stone::PAO);
    mS[10].initStone(7, 2, 10, true, Stone::PAO);
    mS[11].initStone(0, 3, 11, true, Stone::BING);
    mS[12].initStone(2, 3, 12, true, Stone::BING);
    mS[13].initStone(4, 3, 13, true, Stone::BING);
    mS[14].initStone(6, 3, 14, true, Stone::BING);
    mS[15].initStone(8, 3, 15, true, Stone::BING);
    mS[16].initStone(0, 6, 16, false, Stone::BING);
    mS[17].initStone(2, 6, 17, false, Stone::BING);
    mS[18].initStone(4, 6, 18, false, Stone::BING);
    mS[19].initStone(6, 6, 19, false, Stone::BING);
    mS[20].initStone(8, 6, 20, false, Stone::BING);
    mS[21].initStone(1, 7, 21, false, Stone::PAO);
    mS[22].initStone(7, 7, 22, false, Stone::PAO);
    mS[23].initStone(0, 9, 23, false, Stone::CHE);
    mS[24].initStone(1, 9, 24, false, Stone::MA);
    mS[25].initStone(2, 9, 25, false, Stone::XIANG);
    mS[26].initStone(3, 9, 26, false, Stone::SHI);
    mS[27].initStone(4, 9, 27, false, Stone::JIANG);
    mS[28].initStone(5, 9, 28, false, Stone::SHI);
    mS[29].initStone(6, 9, 29, false, Stone::XIANG);
    mS[30].initStone(7, 9, 30, false, Stone::MA);
    mS[31].initStone(8, 9, 31, false, Stone::CHE);
}

void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap("bg.jpg"));
    drawChessBoard(painter);

    //绘制32个棋子
    for (int i = 0; i < 32; ++i)
    {
        drawStone(painter, i);
    }
}

void Board::drawChessBoard(QPainter &painter)
{
    for (int i = 0; i < 10; i++)//画10横线
    {
        painter.LINE(0, G(i), G(8), G(i));
    }
    for (int i = 0; i < 9; i++)//画10竖线
    {
        if (i == 0 || i == 8)
        {
            painter.LINE(G(i), G(0), G(i), G(9));
        }
        else
        {
            painter.LINE(G(i), G(0), G(i), G(4));
            painter.LINE(G(i), G(5), G(i), G(9));
        }
    }
    //画对角线
    painter.LINE(G(3), G(0), G(5), G(2));
    painter.LINE(G(3), G(2), G(5), G(0));
    painter.LINE(G(3), G(7), G(5), G(9));
    painter.LINE(G(3), G(9), G(5), G(7));

    QRect rect = QRect(G(0) + mOffset.x(), G(4) + mOffset.y(), mGap * 8, mGap);
    painter.setPen(QColor(255, 128, 0));
    painter.setFont(QFont("隶书", mR, QFont::Bold));
    painter.drawText(rect, "楚河        汉界", QTextOption(Qt::AlignCenter));
}

void Board::drawStone(QPainter &painter, int id)
{
    if (mS[id].mCol == DEAD)
        return;
    int x = mS[id].mCol, y = mS[id].mRow;
    QRect rect = QRect(G(x) - mR + mOffset.x(), G(y) - mR + mOffset.y(), mR * 2, mR * 2);
    if (id == mSelectId)
        painter.setBrush(QColor(Qt::gray));
    else
        painter.setBrush(QBrush(Qt::yellow));
    painter.setPen(Qt::black);
    painter.drawEllipse(QPoint(G(x) + mOffset.x(), G(y) + mOffset.y()), mR, mR);
    if (mS[id].mRed)
        painter.setPen(Qt::red);
    painter.setFont(QFont("仿宋", mR, QFont::Bold));
    painter.drawText(rect, mS[id].getText(), QTextOption(Qt::AlignCenter));
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    if (mStartGame == false)
        return;
    QPoint pt = ev->pos();
    int col, row;
    getRowCol(pt, col, row);
    if (col == INBOARD)
    {
        mNextId = INBOARD;
        return;
    }
    mNextId = getPointId(col, row);
    if (mNextId == OUTBOARD)//超出棋盘
        return;
    if (mSelectId == UNSELECT && mNextId >= 0 && mS[mNextId].mRed == mRedTurn)
    {
        mSelectId = mNextId;
        update();
    }
    else if (mSelectId >= 0)//已选择棋子
    {
        if (mSelectId == mNextId)
        {
            mSelectId = UNSELECT;
            update();
        }
        else if (canMove(mSelectId, col, row))
        {
            moveStone(col, row);
            mRedTurn = !mRedTurn;
            if (isOver())
                mStartGame = false;
            else if (mRedTurn == true)
                winner->setText("红方走");
            else if (mRedTurn == false)
                winner->setText("黑方走");
            update();
        }
    }
}

void Board::getRowCol(QPoint pt, int &col, int &row)//获取网格坐标
{
    pt.setX(pt.x() - mOffset.x());
    pt.setY(pt.y() - mOffset.y());
    int x = pt.x() / mGap;
    int y = pt.y() / mGap;
    if (pt.x() - G(x) > mR)
        x += 1;
    if (pt.y() - G(y) > mR)
        y += 1;
    if ((pt.x() - G(x)) * (pt.x() - G(x)) + (pt.y() - G(y)) * (pt.y() - G(y)) > mR * mR)
    {
        col = INBOARD;
        row = INBOARD;
    }
    else
    {
        col = x;
        row = y;
    }
}

int Board::getPointId(int col, int row)
{
    if (isOut(col, row))
        return OUTBOARD;
    int i;
    for (i = 0;i < 32;i++)
    {
        if (mS[i].mCol == col && mS[i].mRow == row)
        {
            break;
        }
    }
    if (i < 32)
        return i;
    else
        return EMPTY;
}

bool Board::isOut(int col, int row)
{
    if (col < 0 || col > 8 || row < 0 || row > 9)
        return true;
    else
        return false;
}

bool Board::canMove(int selectId, int nCol, int nRow)
{
    int nextId = getPointId(nCol, nRow);
    if (nextId >= 0 && mS[selectId].mRed == mS[nextId].mRed)//自己人，别开枪
        return false;
    switch (mS[selectId].mType)
    {
    case Stone::SHI:
        return canSHIMove(selectId, nCol, nRow);
    case Stone::BING:
        return canBINGMove(selectId, nCol, nRow);
    case Stone::XIANG:
        return canXIANGMove(selectId, nCol, nRow);
    case Stone::MA:
        return canMAMove(selectId, nCol, nRow);
    case Stone::PAO:
        return canPAOMove(selectId, nCol, nRow);
    case Stone::CHE:
        return canCHEMove(selectId, nCol, nRow);
    case Stone::JIANG:
        return canJIANGMove(selectId, nCol, nRow);
    }
    return false;
}

bool Board::canSHIMove(int selectId, int nCol, int nRow)
{
    int sCol = mS[selectId].mCol, sRow = mS[selectId].mRow;
    int dis = distance(sCol, sRow, nCol, nRow);
    bool sRed = mS[selectId].mRed;
    //步长判断
    if (dis != 2)
        return false;
    //越界判断
    if (sRed == true && (nCol < 3 || nCol > 5 || nRow < 0 || nRow > 2))
        return false;
    else if (sRed == false && (nCol < 3 || nCol > 5 || nRow < 7 || nRow > 9))
        return false;
    //斜对角判断
    if (sRed == true && (nCol ^ nRow) % 2 != 0)//红棋
        return true;
    else if (sRed == false && (nCol ^ nRow) % 2 == 0)//黑棋
        return true;
    return false;
}

bool Board::canBINGMove(int selectId, int nCol, int nRow)
{
    int sCol = mS[selectId].mCol, sRow = mS[selectId].mRow;
    int dis = distance(sCol, sRow, nCol, nRow);
    bool sRed = mS[selectId].mRed;
    //步长判断
    if (dis != 1)
        return false;
    //越界判断
    if (nCol < 0 || nCol > 8 || nRow < 0 || nRow > 9)
        return false;
    //方位判断
    if (sRed == true)//红棋
    {
        if (nCol == sCol && nRow > sRow)//横坐标不变
            return true;
        else if (nRow == sRow && nRow > 4)//纵坐标不变
            return true;
    }
    else if (sRed == false)//黑棋
    {
        if (nCol == sCol && nRow < sRow)//横坐标不变
            return true;
        else if (nRow == sRow && nRow < 5)//纵坐标不变
            return true;
    }
    return false;
}

bool Board::canXIANGMove(int selectId, int nCol, int nRow)
{
    int sCol = mS[selectId].mCol, sRow = mS[selectId].mRow;
    int dis = distance(sCol, sRow, nCol, nRow);
    bool sRed = mS[selectId].mRed;
    //步长判断
    if (dis != 8)
        return false;
    //越界判断
    if (sRed == true && (nCol < 0 || nCol > 8 || nRow < 0 || nRow > 4))
        return false;
    else if (sRed == false && (nCol < 0 || nCol > 8 || nRow < 5 || nRow > 9))
        return false;
    //阻塞判断
    if (getPointId((nCol + sCol) / 2, (nRow + sRow) / 2) >= 0)
        return false;
    return true;
}

bool Board::canMAMove(int selectId, int nCol, int nRow)
{
    int sCol = mS[selectId].mCol, sRow = mS[selectId].mRow;
    int dis = distance(sCol, sRow, nCol, nRow);
    //步长判断
    if (dis != 5)
        return false;
    //越界判断
    if (nCol < 0 || nCol > 8 || nRow < 0 || nRow > 9)
        return false;
    //阻塞判断
    if (getPointId((nCol + sCol) / 2, (nRow + sRow) / 2) >= 0)
        return false;
    return true;
}

bool Board::canPAOMove(int selectId, int nCol, int nRow)
{
    int sCol = mS[selectId].mCol, sRow = mS[selectId].mRow;
    int nextId = getPointId(nCol, nRow);
    int num = 0;
    //越界判断
    if ((nCol != sCol && nRow != sRow) || (nCol < 0 || nCol > 8 || nRow < 0 || nRow > 9))
        return false;
    //阻塞判断
    if (nCol == sCol)//同一列
    {
        if (nRow < sRow)
            swap(nRow, sRow);
        for (int i = sRow + 1; i < nRow; i++)
        {
            if (getPointId(nCol, i) >= 0)
                num++;
        }
    }
    else if (nRow == sRow)//同一行
    {
        if (nCol < sCol)
            swap(nCol, sCol);
        for (int i = sCol + 1; i < nCol; i++)
        {
            if (getPointId(i, nRow) >= 0)
                num++;
        }
    }
    if (nextId >= 0 && num == 1)
        return true;
    else if (nextId < 0 && num == 0)
        return true;
    return false;
}

bool Board::canCHEMove(int selectId, int nCol, int nRow)
{
    int sCol = mS[selectId].mCol, sRow = mS[selectId].mRow;
    //越界判断
    if ((nCol != sCol && nRow != sRow) || (nCol < 0 || nCol > 8 || nRow < 0 || nRow > 9))
        return false;
    //阻塞判断
    if (nCol == sCol)//同一列
    {
        if (nRow < sRow)
            swap(nRow, sRow);
        for (int i = sRow + 1;i < nRow;i++)
        {
            if (getPointId(nCol, i) >= 0)
                return false;
        }
    }
    else if (nRow == sRow)//同一行
    {
        if (nCol < sCol)
            swap(nCol, sCol);
        for (int i = sCol + 1;i < nCol;i++)
        {
            if (getPointId(i, nRow) >= 0)
                return false;
        }
    }
    return true;
}

bool Board::canJIANGMove(int selectId, int nCol, int nRow)
{
    int sCol = mS[selectId].mCol, sRow = mS[selectId].mRow;
    int dis = distance(sCol, sRow, nCol, nRow);
    bool red = mS[selectId].mRed;
    //步长判断
    if (dis != 1)
        return false;
    //越界判断
    if (red == true && (nCol < 3 || nCol > 5 || nRow < 0 || nRow > 2))
        return false;
    else if (red == false && (nCol < 3 || nCol > 5 || nRow < 7 || nRow > 9))
        return false;
    return true;
}

int Board::distance(int x1, int y1, int x2, int y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

void Board::swap(int &a, int &b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

void Board::moveStone(int col, int row)
{
    mS[mSelectId].mCol = col;
    mS[mSelectId].mRow = row;
    if (mNextId >= 0)
    {
        mS[mNextId].mCol = DEAD;
        mS[mNextId].mRow = DEAD;
    }
    mPreviousId = mSelectId;
    mSelectId = UNSELECT;
}

bool Board::isOver()
{
    if (mS[4].mCol == DEAD)
    {
        winner->setText("黑方胜");
        return true;
    }
    else if (mS[27].mCol == DEAD)
    {
        winner->setText("红方胜");
        return true;
    }
    if (mS[4].mCol == mS[27].mCol)
    {
        for (int i = mS[4].mRow + 1;i < mS[27].mRow;i++)
        {
            if (getPointId(mS[4].mCol, i) >= 0)
                return false;
        }
        if (mS[mPreviousId].mRed == true)
            winner->setText("黑方胜");
        else if (mS[mPreviousId].mRed == false)
            winner->setText("红方胜");
        return true;
    }
    return false;
}


void Board::saveStep(int selectId, int col, int row, QVector<Step*>& steps)
{
    int nextId = getPointId(col, row);
    Step* step = new Step;
    step->mSelectId = selectId;
    step->mNextId = nextId;
    step->msCol = mS[selectId].mCol;
    step->msRow = mS[selectId].mRow;
    step->mnCol = col;
    step->mnRow = row;
    steps.append(step);
}

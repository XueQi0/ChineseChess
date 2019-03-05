#include "singlegame.h"
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QDir>

SingleGame::SingleGame()
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    mLevel = 4;
    updateChessScore();
    mFakeRedTurn = false;
    winner->setText("红方走");
}

void SingleGame::mouseReleaseEvent(QMouseEvent *ev)
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
            if (isOver())
                mStartGame = false;
            update();

            sleep(100);
            computerMove();
            if (isOver())
                mStartGame = false;
            update();
        }
    }
}

void SingleGame::computerMove()
{
/*
    1.看看有哪些步骤可以走
    2.试着走一下
    3.评估走的结果
    4.取最好的结果
*/
    QVector<Step *> steps;
    mFakeRedTurn = false;
    getAllPossibleMove(mFakeRedTurn, steps);
    mFakeRedTurn = !mFakeRedTurn;
    int maxScore = -1000;
    Step *maxStep = NULL;
    while (steps.count())
    {
        Step *step = steps.last();
        fakeMove(step);
        int score = getMinScore(mLevel - 1, 0);
        unfakeMove(step);
        steps.removeLast();
        if (score > maxScore)
        {
            maxScore = score;
            if (maxStep)
                delete maxStep;
            maxStep = step;
        }
        else
        {
            delete step;
        }
    }
    if (maxStep)
    {
        fakeMove(maxStep);
    }
}

int SingleGame::getMinScore(int level, int curMaxScore)
{
    if (level == 0)
        return calcScore();
    QVector<Step *> steps;
    getAllPossibleMove(mFakeRedTurn, steps);
    mFakeRedTurn = !mFakeRedTurn;
    int minScore = 1000;
    while (steps.count())
    {
        Step *step = steps.back();
        steps.removeLast();
        fakeMove(step);
        int score = getMaxScore(level - 1, 0);
        unfakeMove(step);
        delete step;
        if (score <= curMaxScore)
        {
            while (steps.count())
            {
                Step *step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }
        if (score < minScore)
            minScore = score;
    }
    return minScore;
}

int SingleGame::getMaxScore(int level, int curMinScore)
{
    if (level == 0)
        return calcScore();
    QVector<Step *> steps;
    getAllPossibleMove(mFakeRedTurn, steps);
    mFakeRedTurn = !mFakeRedTurn;
    int maxScore = -1000;
    while (steps.count())
    {
        Step *step = steps.back();
        steps.removeLast();
        fakeMove(step);
        int score = getMinScore(level - 1, 1000);
        unfakeMove(step);
        delete step;
        if (score >= curMinScore)
        {
            while (steps.count())
            {
                Step *step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }
        if (score > maxScore)
            maxScore = score;
    }
    return maxScore;
}

void SingleGame::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void SingleGame::getAllPossibleMove(bool redTurn, QVector<Step *> &steps)
{
    int min = 16, max = 32;
    if (redTurn == true)
    {
        min = 0, max = 16;
    }
    int chess[7] = {Stone::JIANG, Stone::SHI, Stone::BING, Stone::XIANG, Stone::MA,
        Stone::PAO, Stone::CHE};
    for (int type = 0; type < 7; type++)
    {
        for (int i = min; i < max;i++)
        {
            if (mS[i].mType != chess[type])
                continue;
            for (int col = 0; col < 8; col++)
            {
                for (int row = 0; row <= 9; row++)
                {
                    if (canMove(i, col, row) && !isFailing(mFakeRedTurn))
                    {
                        saveStep(i, col, row, steps);
                    }
                }
            }
        }
    }
}

bool SingleGame::isFailing(bool redTurn)
{
    if (mS[4].mCol == mS[27].mCol)
    {
        int num = 0;
        for (int i = mS[4].mRow + 1; i < mS[27].mRow; i++)
        {
            if (getPointId(mS[4].mCol, i) >= 0)
            {
                num++;
                break;
            }
        }
        if (num == 0)
            return true;
    }
//    int min = 0, max = 16;
//    int nCol = mS[27].mCol, nRow = mS[27].mRow;
//    if (redTurn == true)
//    {
//        min = 16, max = 32;
//        nCol = mS[4].mCol, nRow = mS[4].mRow;
//    }
//    for (int i = min;i < max;i++)
//    {
//        if (canMove(i, nCol, nRow))
//            return true;
//    }
    return false;
}

void SingleGame::fakeMove(Step *step)
{
    mS[step->mSelectId].mCol = step->mnCol;
    mS[step->mSelectId].mRow = step->mnRow;
    if (step->mNextId >= 0)
    {
        mS[step->mNextId].mCol = DEAD;
        mS[step->mNextId].mRow = DEAD;
    }
}

void SingleGame::unfakeMove(Step *step)
{
    mS[step->mSelectId].mCol = step->msCol;
    mS[step->mSelectId].mRow = step->msRow;
    if (step->mNextId >= 0)
    {
        mS[step->mNextId].mCol = step->mnCol;
        mS[step->mNextId].mRow = step->mnRow;
    }
}

int SingleGame::calcScore()
{
    int redScore = 0, blackScore = 0;
//    updateChessScore();
    for (int i = 0; i < 32; i++)
    {
        if (mS[i].mRed == true && mS[i].mCol != DEAD)
        {
            redScore += chessScore[mS[i].mType];
        }
        else if (mS[i].mRed == false && mS[i].mCol != DEAD)
        {
            blackScore += chessScore[mS[i].mType];
        }
    }
    return blackScore - redScore;
}

void SingleGame::updateChessScore()
{
    //SHI, BING, XIANG, MA, PAO, CHE, JIANG
    chessScore[0] = 1;
    chessScore[1] = 3;
    chessScore[2] = 3;
    chessScore[3] = 4;
    chessScore[4] = 6;
    chessScore[5] = 8;
    chessScore[6] = 666;
}

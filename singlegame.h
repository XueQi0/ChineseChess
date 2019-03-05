#ifndef SINGLEGAME_H
#define SINGLEGAME_H
#include "board.h"

class SingleGame : public Board
{
public:
    SingleGame();

    void mouseReleaseEvent(QMouseEvent *);
    void sleep(unsigned int msec);
    void getAllPossibleMove(bool redTurn, QVector<Step *> &steps);
    bool isFailing(bool redTurn);
    void computerMove();
    int getMinScore(int level, int curMaxScore);
    int getMaxScore(int level, int curMinScore);
    void fakeMove(Step *step);
    void unfakeMove(Step *step);
    int calcScore();
    void updateChessScore();

    int mLevel;
    int chessScore[7];
    bool mFakeRedTurn;
};

#endif // SINGLEGAME_H

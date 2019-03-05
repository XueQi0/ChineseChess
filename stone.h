#ifndef STONE_H
#define STONE_H
#include <QString>

class Stone
{
public:
    Stone();
    ~Stone();

    enum TYPE{
        SHI, BING, XIANG, MA, PAO, CHE, JIANG
    };

    void initStone(int col, int row, int id, bool red, TYPE type);

    int mCol;//列
    int mRow;//行
    int mId;
    bool mRed;
    TYPE mType;

    QString getText();
};

#endif // STONE_H

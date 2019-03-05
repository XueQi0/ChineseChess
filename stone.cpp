#include "stone.h"

Stone::Stone()
{

}

Stone::~Stone()
{

}

void Stone::initStone(int col, int row, int id, bool red, TYPE type)
{
    mRow = row;
    mCol = col;
    mId = id;
    mRed = red;
    mType = type;
}

QString Stone::getText()
{
    switch (this->mType)
    {
    case SHI:
        return "士";
    case BING:
        return "兵";
    case XIANG:
        return "相";
    case MA:
        return "马";
    case PAO:
        return "炮";
    case CHE:
        return "车";
    case JIANG:
        return "将";
    }
}

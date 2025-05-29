#include "YellowBrick.h"
#include "BrokenBrick.h"
#include "QuestionBrick.h"
#include "SwitchBlock.h"
#include "PlayScene.h"

void CYellowBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_YELLOW_BRICK)->Render(x, y);
}

void CYellowBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - YELLOW_BRICK_BBOX_WIDTH / 2;
	t = y - YELLOW_BRICK_BBOX_HEIGHT / 2;
	r = l + YELLOW_BRICK_BBOX_WIDTH;
	b = t + YELLOW_BRICK_BBOX_HEIGHT;
}

void CYellowBrick::Break()
{
    this->isDeleted = true;

    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

    float pieceVX[] = { -BROKEN_BRICK_SPEED_X, BROKEN_BRICK_SPEED_X, -BROKEN_BRICK_SPEED_X, BROKEN_BRICK_SPEED_X };
    float pieceVY[] = { -BROKEN_BRICK_SPEED_Y, -BROKEN_BRICK_SPEED_Y, -BROKEN_BRICK_SPEED_Y / 2, -BROKEN_BRICK_SPEED_Y / 2 };

    for (int i = 0; i < 4; i++) {
        CBrokenBrick* piece = new CBrokenBrick(x, y, pieceVX[i], pieceVY[i]);
        scene->AddObject(piece);
    }

    if (type == 2) {
        CQuestionBrick* qb = new CQuestionBrick(x, y);
        qb->SetState(QUESTION_BRICK_STATE_DISABLED);
        scene->AddObject(qb);

        CSwitchBlock* PSwitch = new CSwitchBlock(x, y);
        scene->AddObject(PSwitch);
    }
}
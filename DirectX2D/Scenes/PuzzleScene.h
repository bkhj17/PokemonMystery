#pragma once
class PuzzleScene : public Scene
{
private:

    struct EmptyDir {
        bool up = true, down = true, right = true, left = true;
    };

    const UINT PAIR_NUM = 4;
    const float CARD_SCALE = 0.5f;
    const UINT SHUFFLE = 500;

    const float DELETE_DELAY_TIME = 0.5f;

public:
    PuzzleScene();
    ~PuzzleScene();

    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void Render() override;
    virtual void PostRender() override;

private:
    void CreateCards(UINT puzzleLevel);
    void LoadTextures();
    void ShuffleCard();
    void SwapCard(int n1, int n2);

    bool IsSameCard();
    bool IsSameLine(POINT coord1, POINT coord2);
    bool IsSameLineIncludeSelf(POINT coord1, POINT coord2);
    bool IsUTurn(POINT coord1, POINT coord2);

    EmptyDir GetEmptyDir(POINT coord);

    void CheckMatchCard();

    void DeleteMatchCard();

    void SelectCard(void* selectCard);
private:
    vector<wstring> cardTextures;
    vector<Card*> cards;
    vector<vector<Card*>> cardBoard;

    UINT width, height;

    Card* firstSelectCard = nullptr;
    Card* secondSelectCard = nullptr;

    float deleteTime = 0.0f;
    bool isDeleteDelay = false;
};


#include "Framework.h"
#include "PuzzleScene.h"

PuzzleScene::PuzzleScene()
{
	LoadTextures();
	CreateCards(15);
	ShuffleCard();

	Observer::Get()->AddParamEvent("ClickCard", bind(&PuzzleScene::SelectCard, this, placeholders::_1));
}

PuzzleScene::~PuzzleScene()
{
	for (Card* card : cards)
		delete card;
}

void PuzzleScene::Update()
{
	if (firstSelectCard && secondSelectCard) {
		deleteTime += DELTA;

		if (deleteTime >= DELETE_DELAY_TIME) {
			DeleteMatchCard();
		}
	}

	for (Card* card : cards)
		card->Update();

}

void PuzzleScene::Render()
{
	for (Card* card : cards) {
		if (card == firstSelectCard || card == secondSelectCard)
			continue;
		card->Render();
	}
	
	if (firstSelectCard != nullptr)
		firstSelectCard->Render();

	if (secondSelectCard != nullptr)
		secondSelectCard->Render();
}

void PuzzleScene::PostRender()
{
}

void PuzzleScene::CreateCards(UINT puzzleLevel)
{
	UINT totalCardNum = puzzleLevel * PAIR_NUM;

	//약수 중 둘 사이의 차가 가장 적은 경우
	{
		vector<UINT> divisors;
		for (UINT i = 1; i <= totalCardNum; i++) {
			if (totalCardNum % i == 0)
				divisors.push_back(i);
		}

		UINT halfDisvisorsSize = divisors.size() / 2;

		width = divisors[halfDisvisorsSize];
		height = divisors[(divisors.size() % 2 == 0) ? halfDisvisorsSize - 1 : halfDisvisorsSize];
	}

	Vector2 size = Texture::Add(cardTextures[0])->GetSize() * CARD_SCALE;
	Vector2 totalHalfSize = size * Vector2(width, height) * 0.5f;
	Vector2 startOffset = Vector2(CENTER_X, CENTER_Y) - totalHalfSize + size * 0.5f;

	cardBoard.resize(height);
	for (vector<Card*>& cards : cardBoard)
		cards.resize(width);
	
	for (UINT y = 0; y < height; y++) {
		for (UINT x = 0; x < width; x++) {
			Vector2 pos = startOffset + Vector2(x, y) * size;
			UINT index = (y * width + x) / PAIR_NUM;
			Card* card = new Card(cardTextures[index]);
			card->Pos() = pos;
			card->Scale() *= CARD_SCALE;
			card->SetCoord(x, y);

			cards.push_back(card);

			cardBoard[y][x] = card;
		}
	} 
}

void PuzzleScene::LoadTextures()
{
	WIN32_FIND_DATA findData;
	HANDLE handle = FindFirstFile(L"Textures/Cards/*png", &findData);
	
	bool result = true;
	wstring fileName;
	while (result) {
		fileName = L"Textures/Cards/";
		fileName += findData.cFileName;

		cardTextures.push_back(fileName);

		result = FindNextFile(handle, &findData);
	}
}

void PuzzleScene::ShuffleCard()
{
	for (int i = 0; i < SHUFFLE; i++) {
		int random1 = Random(0, cards.size());
		int random2 = Random(0, cards.size());

		SwapCard(random1, random2);
	}

}

void PuzzleScene::SwapCard(int n1, int n2)
{
	//위치 스왑
	{
		Vector2 tempPos = cards[n1]->Pos();
		cards[n1]->Pos() = cards[n2]->Pos();
		cards[n2]->Pos() = tempPos;
	}
	//좌표 스왑
	{
		POINT tempCoord = cards[n1]->GetCoord();
		cards[n1]->SetCoord(cards[n2]->GetCoord());
		cards[n2]->SetCoord(tempCoord);
	}
	//인덱스 스왑
	{
		POINT coord1 = cards[n1]->GetCoord();
		POINT coord2 = cards[n2]->GetCoord();

		Card* tempCard = cardBoard[coord1.y][coord1.x];
		cardBoard[coord1.y][coord1.x] = cardBoard[coord2.y][coord2.x];
		cardBoard[coord2.y][coord2.x] = tempCard;
	}

}

bool PuzzleScene::IsSameCard()
{
	if (firstSelectCard == secondSelectCard)
	{
		MessageBox(hWnd, L"같은 카드", L"Error", MB_OK);
		return false;
	}

	return firstSelectCard->GetKey() == secondSelectCard->GetKey();
}

bool PuzzleScene::IsSameLine(POINT coord1, POINT coord2)
{
	if (coord1.x != coord2.x && coord1.y != coord2.y)
	{
		return false;
	}
	if (coord1.x == coord2.x) {
		int minY = min(coord1.y, coord2.y);
		int maxY = max(coord1.y, coord2.y);

		for (int y = minY + 1; y < maxY; y++) {
			if (cardBoard[y][coord1.x]->Active()) {
				return false;
			}
		}
	}
	else {

		int minX = min(coord1.x, coord2.x);
		int maxX = max(coord1.x, coord2.x);

		for (int x = minX + 1; x < maxX; x++) {
			if (cardBoard[coord1.y][x]->Active()) {
				return false;
			}
		}
	}

	return true;
}

bool PuzzleScene::IsSameLineIncludeSelf(POINT coord1, POINT coord2)
{
	if (coord1.x != coord2.x && coord1.y != coord2.y)
	{
		return false;
	}
	if (coord1.x == coord2.x) {
		int minY = min(coord1.y, coord2.y);
		int maxY = max(coord1.y, coord2.y);

		for (int y = minY + 1; y <= maxY; y++) {
			if (cardBoard[y][coord1.x]->Active()) {
				return false;
			}
		}
	}
	else {

		int minX = min(coord1.x, coord2.x);
		int maxX = max(coord1.x, coord2.x);

		for (int x = minX + 1; x <= maxX; x++) {
			if (cardBoard[coord1.y][x]->Active()) {
				return false;
			}
		}
	}

}

bool PuzzleScene::IsUTurn(POINT coord1, POINT coord2)
{
	EmptyDir result1 = GetEmptyDir(coord1);
	EmptyDir result2 = GetEmptyDir(coord2);

	if (result1.up && result2.up)
		return true;
	if (result1.down && result2.down)
		return true;
	if (result1.left && result2.left)
		return true;
	if (result1.right && result2.right)
		return true;
	//Right
	for (int x = coord1.x + 1; x < width; x++) {
		POINT coord = { x , coord1.y };
		if (cardBoard[coord.y][coord.x]->Active())
			break;

		if (IsSameLine(coord, coord2)) {
			return true;
		}

		POINT tempCoord = { x, coord2.y };
		if (IsSameLineIncludeSelf(coord, tempCoord)) {
			if (IsSameLine(coord2, tempCoord))
				return true;
		}
	}
	//Left
	for (int x = coord1.x - 1; x >= 0; x--) {
		POINT coord = { x , coord1.y };
		if (cardBoard[coord.y][coord.x]->Active())
			break;

		if (IsSameLine(coord, coord2)) {
			return true;
		}

		POINT tempCoord = { x, coord2.y };
		if (IsSameLineIncludeSelf(coord, tempCoord)) {
			if (IsSameLine(coord2, tempCoord))
				return true;
		}
	}
	//Up
	for (int y = coord1.y + 1; y < height; y++) {
		POINT coord = { coord1.x , y };
		if (cardBoard[coord.y][coord.x]->Active())
			break;

		if (IsSameLine(coord, coord2)) {
			return true;
		}

		POINT tempCoord = { coord2.x, y };
		if (IsSameLineIncludeSelf(coord, tempCoord)) {
			if (IsSameLine(coord2, tempCoord))
				return true;
		}
	}
	//Dowm
	for (int y = coord1.y - 1; y >= 0; y--) {
		POINT coord = { coord1.x , y };
		if (cardBoard[coord.y][coord.x]->Active())
			break;

		if (IsSameLine(coord, coord2)) {
			return true;
		}

		POINT tempCoord = { coord2.x, y };
		if (IsSameLineIncludeSelf(coord, tempCoord)) {
			if (IsSameLine(coord2, tempCoord))
				return true;
		}
	}
	return false;
}

PuzzleScene::EmptyDir PuzzleScene::GetEmptyDir(POINT coord)
{
	EmptyDir result;

	for (int x = coord.x + 1; x < width; x++) {
		if (cardBoard[coord.y][x]->Active()) {
			result.right = false;
			break;
		}
	}

	for (int x = coord.x - 1; x >= 0; x--) {
		if (cardBoard[coord.y][x]->Active()) {
			result.left = false;
			break;
		}
	}

	for (int y = coord.y + 1; y < height; y++) {
		if (cardBoard[y][coord.x]->Active()) {
			result.up = false;
			break;
		}
	}

	for (int y = coord.y - 1; y >= 0; y--) {
		if (cardBoard[y][coord.x]->Active()) {
			result.down = false;
			break;
		}
	}

	return result;
}

void PuzzleScene::CheckMatchCard()
{
	if (!IsSameCard())
		return;

	if (IsSameLine(firstSelectCard->GetCoord(), secondSelectCard->GetCoord())) {
		firstSelectCard->SetActive(false);
		secondSelectCard->SetActive(false);
		return;
	}

	if (IsUTurn(firstSelectCard->GetCoord(), secondSelectCard->GetCoord())) {
		firstSelectCard->SetActive(false);
		secondSelectCard->SetActive(false);
		return;
	}
}

void PuzzleScene::DeleteMatchCard()
{
	isDeleteDelay = false;

	firstSelectCard->SetSelected(false);
	firstSelectCard = nullptr;

	secondSelectCard->SetSelected(false);
	secondSelectCard = nullptr;
}

void PuzzleScene::SelectCard(void* selectCard)
{
	Card* card = (Card*)selectCard;
	card->SetSelected(true);

	if (firstSelectCard == nullptr) {
		firstSelectCard = card;
		return;
	}
	else if (firstSelectCard == card) {
		firstSelectCard = nullptr;
	}
	else {
		secondSelectCard = card;
		CheckMatchCard();
		deleteTime = 0.0f;
		return;
	}
	card->SetSelected(false);
}

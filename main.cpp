#include <Novice.h>

const char kWindowTitle[] = "김준수";
const int kWindowWidth = 1920;
const int kWindowHeight = 1080;

// Windows 어플로의 엔트리 포인트 (메인 관수)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// 라이브러리의 초기화
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// 키입력 결과를 받는 박스
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// 플레이어 변수

	int playerPosX = 200; // 플레이어 위치
	int playerPosY = 400;
	int playerWidth = 100; // 플레이어 크기
	int playerHeight = 100;

	int playerFullHp = 1000; // 플레이어 기본 체력
	int playerHp = 1000; // 플레이어 현재 체력
	int playerRecovery = 1; // 플레이어 회복력
	int playerPower = 1; // 플레이어 데미지
	int playerCost = 30; // 공격시 체력 손실

	int playerAttackCoolTime = 10; // 공격 쿨타임

	bool isPlayerAttacking = false; // 플레이어 공격ing
	// 더미 변수

	int dummyPosX = 500; // 더미 위치
	int dummyPosY = 350;
	int dummyWidth = 50; // 더미 크기
	int dummyHeight = 150;

	int dummyFullHp = 10; // 더미 기본 체력
	int dummyHp = 10; // 더미 현재 체력

	bool isDummyAlive = true;
	int dummyReviveCoolTime = 100; // 재생성 시간

	//자원
	int gold = 0;

	// 윈도우의 X버튼이 눌릴때까지 루프
	while (Novice::ProcessMessage() == 0) {
		// 프레임
		Novice::BeginFrame();

		// 키 입력
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓갱신처리
		///

		// ↓플레이어

		if (keys[DIK_SPACE] && playerHp > playerCost) { // 버튼을 누르면
			isPlayerAttacking = true; // 플레이어 공격 개시
			if (isPlayerAttacking) {
				playerAttackCoolTime--;
				if (playerAttackCoolTime == 0) { // 쿨타임이 0일때만 데미지 적용
					playerHp -= playerCost;
					dummyHp -= playerPower;
				}
				if (playerAttackCoolTime < 0) { // 쿨타임이 0 밑으로 내려가면 초기화
					playerAttackCoolTime = 10;
				}
			}
		}
		if (!keys[DIK_SPACE] && isPlayerAttacking) { // 공격시가 아닐 경우
			isPlayerAttacking = false;
		}
		if (!isPlayerAttacking) { // 쿨타임 초기화
			playerAttackCoolTime = 10;
		}
		if (playerHp < playerFullHp && isPlayerAttacking == false) { // 공격시가 아닐 경우 HP 회복
			playerHp += playerRecovery;
		}
		// ↑플레이어

		// ↓더미

		if (isDummyAlive && dummyHp <= 0) { // 더미가 hp가 0이 되면
			isDummyAlive = false;
			gold = +1;
		}
		if (!isDummyAlive) { // 더미가 죽으면 
			dummyReviveCoolTime--;
		}
		if (!isDummyAlive && dummyReviveCoolTime <= 0) { // 더미 재생성
			isDummyAlive = true;
			dummyReviveCoolTime = 100;
			dummyHp = dummyFullHp;
		}

		// ↑더미

		///
		/// ↑갱신처리
		///

		///
		/// ↓그림처리
		///

		// 플레이어 렌더링
		Novice::DrawBox(playerPosX, playerPosY, playerWidth, playerHeight, 0, WHITE, kFillModeSolid);
		Novice::ScreenPrintf(playerPosX + playerWidth / 4, playerPosY - 50, "%d", playerHp);

		// 더미 렌더링
		if (isDummyAlive) {
			Novice::DrawBox(dummyPosX, dummyPosY, dummyWidth, dummyHeight, 0, RED, kFillModeSolid);
		}
		Novice::ScreenPrintf(dummyPosX + dummyWidth / 4, dummyPosY - 50, "%d", dummyHp);

		//데이터 확인용
		
		// 공격 중인지 아닌지
		if (isPlayerAttacking) {
			Novice::ScreenPrintf(10, 10, "ATTACK");
		}
		if (!isPlayerAttacking) {
			Novice::ScreenPrintf(10, 10, "NOT ATTACK");
		}
		//자원
		Novice::ScreenPrintf(10, 30, "GOLD : %d",gold);
		//부활타이머
		Novice::ScreenPrintf(10, 50, "DummyReviveCoolTime : %d", dummyReviveCoolTime);
		//


		Novice::SetWindowMode(kFullscreen);	//전체화면
		///
		/// ↑그림처리
		///

		// 프레임 종료
		Novice::EndFrame();

		// ESC키가 눌리면 루프 종료
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// 라이브러리의 종료
	Novice::Finalize();
	return 0;
}

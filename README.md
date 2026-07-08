# AWeek

> **Unreal Engine 5 / C++** 기반 서바이벌 게임 프로젝트
> 인벤토리 · 크래프팅 · 루팅 · 상호작용 시스템 및 UI 아키텍처 담당

팀 프로젝트로 진행 중이며, 이 문서는 제가 설계·구현한 시스템을 정리한 것입니다.
(플레이어 캐릭터 / 무기 시스템 등은 팀원 담당)

---

## 담당 시스템 개요

| 시스템 | 핵심 키워드 |
|---|---|
| [Interaction System](#1-interaction-system) | 인터페이스 다형성, 주기적 LineTrace, 델리게이트 UI 연동 |
| [Inventory System](#2-inventory-system) | 컴포넌트 상속 계층, 스택 분배, 결과 패턴, 슬롯 단위 갱신 |
| [Crafting System](#3-crafting-system) | CSV DataTable 캐싱, 수량 캐시 기반 판정, 레벨 해금 |
| [Loot System](#4-loot-system) | 확률 기반 랜덤 드롭, DataTable 루트 테이블, 정적 스폰 팩토리 |
| [Item System](#5-item-system) | UObject 아이템, 정적 팩토리, Outer 기반 메모리 관리 |
| [UI Architecture](#6-ui-architecture) | MVC + Facade/Mediator, Controller 레이어, Common UI, 드래그 앤 드롭 |

---

## 아키텍처

UI와 게임플레이 로직의 결합을 줄이기 위해 **MVC + Facade + Mediator** 구조를 적용했습니다.

```
┌─────────────────────────────────────────────────────────┐
│  UAWeekGameUIManager (GameInstance Subsystem)            │
│  - Facade / Mediator: 위젯 생명주기 & Controller 소유    │
│  ├── InventoryController  ── 슬롯 입력 처리, HeldItem    │
│  └── CraftingController   ── 제작 검증 & 실행 위임       │
└──────────────┬──────────────────────────────────────────┘
               │ (Controller가 UI ↔ Component 사이 번역)
┌──────────────┴──────────────────────────────────────────┐
│  Components (데이터 & 로직 소유)                          │
│  ├── PlayerInventoryComponent ─ OnSlotUpdated ─► Panel   │
│  ├── CraftingComponent ─ OnCraftingFinished ─► UI 갱신   │
│  └── (Interaction) ─ OnInteractionTargetChanged ─► HUD   │
└─────────────────────────────────────────────────────────┘
```

- **Widget**: 입력 이벤트를 Controller에 위임할 뿐 데이터를 직접 조작하지 않음
- **Controller**: UI 입력을 Component 조작으로 번역, UI 전용 상태(HeldItem) 소유
- **Component**: 인벤토리/크래프팅 데이터와 규칙을 소유, 델리게이트로 변경 통지

---

## 1. Interaction System

플레이어와 월드 오브젝트(상자, 픽업 아이템, 제작대 등)의 상호작용 흐름.

**인터페이스 기반 다형성** — [AWeekInteractionInterface.h](https://github.com/jch618/AWeek/blob/7bcbad48ec7dc65459340c0ff18f87afb3e42294/Source/AWeek/Interfaces/AWeekInteractionInterface.h#L1-L72)
- `BeginFocus` / `EndFocus` / `BeginInteract` / `EndInteract` / `Interact` 가상 함수 정의
- `AAWeekChest`, `AAWeekPickupItem`, `AAWeekCraftingTable` 등이 인터페이스를 구현해 오브젝트 종류 확장에 열려 있는 구조
- `FAWeekInteractableData` 구조체로 타입 / 이름 / 행동 텍스트 / 상호작용 지속시간 등 메타데이터 표현

**주기적 LineTrace 감지 (`PerformInteractionCheck`)**
- 매 프레임이 아닌 `InteractionCheckFrequency` 주기로 LineTrace를 수행해 비용 절감
- `TScriptInterface<IAWeekInteractionInterface>`로 현재 타겟을 타입 안전하게 보관
- 지속 상호작용(밸브 등)을 위해 `FTimerHandle` 기반 홀드 인터랙션 지원

**델리게이트 기반 UI 통지**
- `FOnInteractionTargetChanged` 멀티캐스트 델리게이트로 타겟 변경 시 HUD 위젯에 데이터 전달
- 게임플레이 코드가 UI 위젯을 직접 참조하지 않도록 분리

---

## 2. Inventory System

슬롯 배열 기반 인벤토리. 상속 계층으로 플레이어 전용 기능을 확장했습니다.

**컴포넌트 상속 계층** — [AWeekPlayerInventoryComponent.h](https://github.com/jch618/AWeek/blob/main/Source/AWeek/Components/AWeekPlayerInventoryComponent.h)
```
UAWeekInventoryComponent            ← 상자 등 범용 인벤토리
└── UAWeekPlayerInventoryComponent  ← HotBar / TrashCan 확장
```
- 기반 클래스: `TArray<FAWeekInventorySlotData>` 슬롯 관리, 무게/슬롯 용량, 스택 처리
- 파생 클래스: **HotBar와 TrashCan 슬롯을 별도 컨테이너 없이 동일한 배열의 인덱스 범위로 통합 관리** — 슬롯 이동/스왑 로직을 재사용하면서 특수 슬롯 규칙만 오버라이드(`AddNewItem`, `ClearItemSlot`)

**아이템 추가 결과 패턴 (`FAWeekItemAddResult`)** — [`HandleAddItem`](https://github.com/jch618/AWeek/blob/eb17e412b71b770538610c2e2dbba1651d5f4ed8/Source/AWeek/Components/AWeekInventoryComponent.cpp#L271-L313)
- `NoItemAdded` / `PartialAmountItemAdded` / `AllItemAdded` 3단계 결과 + 사용자 메시지
- 정적 팩토리(`AddedNone` / `AddedPartial` / `AddedAll`)로 결과 생성 표준화
- 호출자가 부분 추가(인벤토리 가득 참 등)를 명확히 처리 가능

**스택 분배 알고리즘** — [`HandleStackableItems`](https://github.com/jch618/AWeek/blob/eb17e412b71b770538610c2e2dbba1651d5f4ed8/Source/AWeek/Components/AWeekInventoryComponent.cpp#L185-L252)
1. `FindNextPartialStack`으로 기존 부분 스택 탐색
2. `CalculateNumberForFullStack`으로 채울 수 있는 만큼 기존 스택에 분배
3. 남은 수량은 빈 슬롯에 새 스택 생성
4. 각 단계에서 실제 추가된 수량을 반환해 결과 패턴과 연결

**무게 & 과부하 시스템**
- `UpdateInventoryTotalWeight(float Delta)` 단일 경로로 무게 변경을 집중
- 과부하 상태 전이 시에만 `FOnEncumberedStatusChanged` 브로드캐스트 → 캐릭터 이동속도 디버프 적용

**슬롯 단위 부분 갱신**
- 전체 패널 재구성 대신 `FOnSlotUpdated(const FAWeekInventorySlotData&)`로 **변경된 슬롯만** UI 갱신
- `FOnTrashCanItemDiscarded`, `FOnHotbarSelectionChanged` 등 목적별 델리게이트 분리

**인벤토리 간 이동**
- `TransferItem`: 대상 인벤토리의 `HandleAddItem` 결과에 따라 소스 슬롯 정리 (부분 이동 대응)
- Shift+클릭으로 플레이어 ↔ 상자 인벤토리 간 빠른 이동

---

## 3. Crafting System

CSV DataTable 레시피를 캐싱하고 인벤토리 수량 캐시로 제작 가능 여부를 판단합니다.

**CSV → 런타임 캐시 변환 파이프라인** — [`LoadAndCacheRecipes`](https://github.com/jch618/AWeek/blob/eb17e412b71b770538610c2e2dbba1651d5f4ed8/Source/AWeek/Components/AWeekCraftingComponent.cpp#L67-L106) · [AWeekItemCraftingRecipe.h](https://github.com/jch618/AWeek/blob/eb17e412b71b770538610c2e2dbba1651d5f4ed8/Source/AWeek/Data/AWeekItemCraftingRecipe.h#L1-L89)
```
FAWeekItemCraftingRecipeCSV (CSV Row, 재료는 문자열)
        │  FAWeekCraftingRecipeParser::ConvertRecipe
        ▼
FAWeekCachedCraftingRecipe (FAWeekItemEntry 배열로 변환 완료)
```
- CSV/Excel 편집 편의를 위해 **재료 목록을 단일 문자열 컬럼**으로 저장하고 정적 파서로 파싱
- BeginPlay 시 1회 변환·캐싱하여 런타임 중 DataTable 재조회 제거
- 중첩 `TArray` 구조 대신 평탄한 Row 구조를 택해 CSV 상호운용성 확보

**수량 캐시 기반 제작 판정**
- `TMap<FName, int32> InventoryItemCounts`로 아이템 ID별 보유 수량 캐시 유지
- 제작 완료(`OnCraftingFinished`) 시 캐시 갱신 → `CanCraft`가 O(재료 수)로 동작

**CraftingLevel 해금 시스템**
- 제작대 레벨에 따라 `RequiredCraftingLevel` 조건을 만족하는 레시피만 노출
- 레벨 변경 시 `OnCraftingLevelChanged` 브로드캐스트로 목록 UI 자동 갱신

**제작 결과 패턴 (`FCraftingResult` / `ECraftingFailureReason`)** — [AWeekCraftingController.h](https://github.com/jch618/AWeek/blob/main/Source/AWeek/UI/Controller/AWeekCraftingController.h)
- `Success` / `InvalidRecipe` / `InsufficientMaterials` / `InventoryFull` / `ComponentNotInitialized`
- 실패 사유를 구분해 UI에 구체적인 피드백 제공

---

## 4. Loot System

처치/파괴 등 이벤트에 따라 스폰되는 루트 상자와 확률 기반 아이템 생성.

**루트 테이블 데이터 구조 (`FAWeekLootItemData`)** — [AWeekLootItemData.h](https://github.com/jch618/AWeek/blob/main/Source/AWeek/Data/AWeekLootItemData.h)
- `FAWeekLootItemEntry`: 아이템 핸들 + 최소/최대 수량 + 드롭 확률(0–100%)
- `MinItemsToDrop` / `MaxItemsToDrop`으로 상자당 아이템 개수 범위 지정
- DataTable Row로 관리해 기획 데이터와 코드 분리

**확률 기반 생성** — [`GenerateRandomLoot`](https://github.com/jch618/AWeek/blob/eb17e412b71b770538610c2e2dbba1651d5f4ed8/Source/AWeek/World/AWeekLootChest.cpp#L107-L154)
- 엔트리별 `DropChance` 판정 → 통과 시 수량 랜덤 롤 → `FAWeekItemEntry` 생성
- 생성된 아이템은 상자의 `InventoryComponent`에 `HandleAddItem`으로 삽입 (인벤토리 시스템 재사용)

**정적 스폰 팩토리 (`SpawnLootChest`)** — [AWeekLootChest.cpp](https://github.com/jch618/AWeek/blob/main/Source/AWeek/World/AWeekLootChest.cpp)
- 스폰 위치 / 루트 테이블 핸들 / 수명(LifeSpan)을 받아 스폰과 초기화를 한 번에 처리
- `FTimerHandle` 기반 수명 만료 시 자동 파괴, 파괴 직전 열려 있는 상자 UI를 안전하게 닫음

---

## 5. Item System

**UObject 기반 아이템 (`UAWeekItemBase`)** — [AWeekItemBase.h](https://github.com/jch618/AWeek/blob/main/Source/AWeek/Items/AWeekItemBase.h) · [AWeekItemBase.cpp](https://github.com/jch618/AWeek/blob/main/Source/AWeek/Items/AWeekItemBase.cpp)
- `FAWeekItemData`를 private 멤버로 보유하고 접근자로 노출 — 필드 중복 없이 단일 출처 유지
- `FAWeekItemData`는 Text / Numeric / Asset / Consumable 데이터를 하위 구조체로 분리한 DataTable Row

**정적 팩토리 + Outer 기반 메모리 관리**
- `CreateFromData(ItemData, Quantity, Outer)` / `CreateFromRowHandle(RowHandle, Quantity, Outer)`
- 생성 주체(인벤토리 / 크래프팅 컴포넌트 등)를 `Outer`로 지정해 UObject GC 수명을 명확히 관리

**Copy / Pickup 플래그**
- `bIsCopy` / `bIsPickup` 플래그로 아이템 출처를 추적
- 인벤토리 간 이동·분할 시 필요할 때만 `CreateItemCopy` 수행, 여러 인벤토리가 동일 포인터를 공유하는 문제 방지

**타입별 파생 (`AWeekConsumableItem`, `AWeekWeaponItem`)**
- `UseItem(PlayerCharacter)` 가상 함수를 오버라이드해 아이템 타입별 사용 동작 분기
- `ConsumableItem`은 DataTable의 `FAWeekConsumableData`(체력/스태미나/허기 변화량)를 읽어 캐릭터 스탯에 적용 — 효과 수치가 코드가 아닌 데이터에 존재

---

## 6. UI Architecture

**GameInstance Subsystem 기반 UI Manager (`UAWeekGameUIManager`)** — [AWeekGameUIManager.h](https://github.com/jch618/AWeek/blob/main/Source/AWeek/UI/AWeekGameUIManager.h)
- 레벨 전환과 무관한 수명 보장, 위젯 클래스/인스턴스와 Controller를 중앙에서 소유
- 개별 위젯이 서로를 직접 참조하지 않고 UIManager를 경유 (Mediator)

**Controller 레이어** — [AWeekInventoryController.h](https://github.com/jch618/AWeek/blob/main/Source/AWeek/UI/Controller/AWeekInventoryController.h) · [AWeekInventoryController.cpp](https://github.com/jch618/AWeek/blob/main/Source/AWeek/UI/Controller/AWeekInventoryController.cpp)
- `UAWeekInventoryController`: 좌클릭(집기/놓기/스왑), 우클릭(1개씩), Shift+클릭(인벤토리 간 이동), Ctrl+클릭(버리기) — 4종 슬롯 입력을 한 곳에서 처리
- `UAWeekCraftingController`: `ValidateCrafting`으로 사전 검증 후 Component에 실행 위임
- UI 전용 상태인 **HeldItem(드래그 중인 아이템)을 Controller가 소유** — 인벤토리 데이터가 아닌 일시적 UI 상태이므로 Component가 아닌 Controller 책임으로 설계

**드래그 앤 드롭 (`UAWeekHeldItem` / `UAWeekHeldItemVisual`)** — [AWeekHeldItem.h](https://github.com/jch618/AWeek/blob/main/Source/AWeek/UI/Inventory/AWeekHeldItem.h)
- 데이터(아이템, 소스 인벤토리, 소스 슬롯)와 비주얼(커서 추적 위젯)을 분리
- 병합(`MergeItem`) 시 수용 가능한 수량만 합치고 나머지는 계속 들고 있는 부분 병합 지원
- 드롭 실패 시 `ReturnHeldItemToInventory`로 원위치 복원

**Inventory Hub — 패널 통합 위젯 (`UAWeekInventoryHubWidget`)** — [AWeekInventoryHubWidget.h](https://github.com/jch618/AWeek/blob/main/Source/AWeek/UI/Inventory/AWeekInventoryHubWidget.h) · [AWeekInventoryHubWidget.cpp](https://github.com/jch618/AWeek/blob/main/Source/AWeek/UI/Inventory/AWeekInventoryHubWidget.cpp)
- 플레이어 인벤토리 패널 + `UCommonActivatableWidgetSwitcher`(상자 / 크래프팅 패널) 조합
- `FAWeekPanelContext` 정적 팩토리(`ForChest` / `ForCrafting`)로 패널 전환에 필요한 데이터를 타입 안전하게 전달
- 델리게이트 체인: `CraftingItemSlot → ListPanel → HubWidget → DetailPanel`로 레시피 선택 전파 — 슬롯 통신에 `RecipeIndex`를 전달해 위젯 간 데이터 참조 결합 최소화

**Common UI (Lyra 스타일)**
- `UAWeekActivatableWidget`을 공통 베이스로 위젯 스택 기반 활성화/포커스 관리
- `BindWidget` / `BindWidgetOptional`로 Blueprint 위젯과 C++ 결합

---

## 프로젝트 구조

[Source/AWeek](https://github.com/jch618/AWeek/tree/main/Source/AWeek)

```
Source/AWeek/
├── Components/        # Inventory, PlayerInventory, Crafting, Loot 컴포넌트
├── Data/              # ItemData, CraftingRecipe, LootItemData, UIDataAsset
├── Items/             # ItemBase, ConsumableItem, WeaponItem
├── Interfaces/        # InteractionInterface
├── World/             # Chest, LootChest, PickupItem, CraftingTable
└── UI/
    ├── Controller/    # InventoryController, CraftingController
    ├── Inventory/     # HubWidget, Panel, ItemSlot, HotBar, HeldItem
    └── Crafting/      # ListPanel, DetailPanel, MainPanel, ItemSlot
```

## 기술 스택

- **Engine**: Unreal Engine 5 (C++)
- **UI**: Common UI (`UCommonActivatableWidgetSwitcher`, ActivatableWidget 스택)
- **Data**: DataTable (CSV import), DataAsset
- **패턴**: MVC, Facade, Mediator, Static Factory, Result Object, Observer(Delegate)

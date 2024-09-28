# MyHashMap 프로젝트 개요 및 고려사항
최신 언어들은 대부분 해시 테이블을 자체적으로 지원하지만, 고대 언어인 C는 해시 테이블을 자체적으로 지원하지 않는다. <br>
직접 HashMap을 구현한다!

## 제한사항
- 전역(global) 변수와 정적(static) 변수의 사용을 금지
  
## 핵심 설계 사항
### 해시 충돌 회피 
- 연결리스트
    - 성능을 위해서라면, 정적 배열로 미리 잡는 것도 하나의 방법으로 생각된다. 그럴경우, 해시충돌 횟수가 늘어남에따라 정책이 추가적으로 고려되어야 할 것이다.
    - 이는 범용적으로 제공하기에는 적합한 고려사항이 아니라고 생각된다.
    - 따라서, 연결리스트로 선택
- 연결리스트의 메모리 할당 시점 최소화
    - capacity, size 전략으로, 최소화
    - 증가, 감소 에 대해 모두 적용

## 지원 함수 및 설명
### `hashmap_t* init_hashmap_malloc(size_t length, unsigned int (*p_hash_func)(const char* key));`
- `init_hashmap_malloc()` 함수는 다음의 인자를 받는다.
    - hashmap->plist 배열의 길이: `size_t length`
    - 해시 함수의 함수 포인터: `unsigned int (*p_hash_func)(const char* key)`
- 이 함수는 동적으로 할당된 `hashmap_t` 구조체를 반환한다.
- `hashmap->plist`의 모든 요소는 `NULL`로 초기화돼야 한다.
  
### `int add_key(hashmap_t* hashmap, const char* key, const int value);`
- `add_key()` 함수는 다음의 인자를 받는다.
    - 해시 맵 포인터: `hashmap_t* hashmap`
    - 키(key)로 사용할 문자열: `const char* key`
    - key에 대응하는 값(value): `const int value`
- 지정된 키와 값을 `hashmap`에 추가합니다.
- 이 키/값 쌍을 `hashmap`에 성공적으로 추가했다면 TRUE(1)를, 아니라면 FALSE(0)를 반환합니다.

### `int get_value(const hashmap_t* hashmap, const char* key);`
- `get_value()` 함수는 다음의 인자를 받는다.
    - 해시 맵 포인터: `const hashmap_t* hashmap`
    - 키로 사용할 문자열: `const char* key`
- `key`에 대응하는 값을 반환한다.
- `hashmap`이 지정된 키에 대응하는 요소를 가지고 있지 않다면 -1을 반환한다.

### `int update_value(hashmap_t* hashmap, const char* key, int value);`
- `update_value()` 함수는 다음의 인자를 받는다.
    - 해시 맵 포인터: `hashmap_t* hashmap`
    - 키로 사용할 문자열: `const char* key`
    - `key`에 대응하는 값: `const int value`
- 지정된 키의 값을 업데이트한다.
- 지정된 키에 대응하는 값을 성공적으로 업데이트했다면 TRUE(1)를, 아니라면 FALSE(0)를 반환한다.
  
### `int remove_key(hashmap_t* hashmap, const char* key);`
- `remove_key()` 함수는 다음의 인자를 받는다.
    - 해쉬 맵 포인터: `hashmap_t* hashmap`
    - 제거할 요소의 키: `const char* key`
- `hashmap`에서 `ke`y에 대응하는 값을 찾아 제거한다.
- 이 키/값 쌍을 `hashmap`에서 성공적으로 제거했다면 TRUE(1)를 아니라면 FALSE(0)를 반환한다.

### `void destroy(hashmap_t* hashmap);`
- `destroy()` 함수는 다음의 인자를 받는다.
    - 해시 맵 포인터: `hashmap_t* hashmap`
- `hashmap`이 사용 중인 모든 동적 메모리 할당을 해제한다.

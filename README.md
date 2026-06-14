# Dining Philosophers Problem

## 1. 문제 개요

Dining Philosophers Problem은 운영체제에서 동기화(Synchronization)와 교착상태(Deadlock)를 설명하기 위해 사용되는 대표적인 문제이다.

5명의 철학자가 원형 테이블에 앉아 있으며 각 철학자 사이에는 포크가 하나씩 놓여 있다. 철학자는 식사를 하기 위해 자신의 왼쪽 포크와 오른쪽 포크를 모두 획득해야 한다.

만약 모든 철학자가 동시에 한 개의 포크만 획득한 상태에서 나머지 포크를 기다리게 되면 교착상태(Deadlock)가 발생할 수 있다.

본 과제에서는 다음 세 가지 방법을 사용하여 Dining Philosophers Problem을 해결하였다.

1. Request All Resources
2. Resource Ordering
3. Banker's Algorithm

---

## 2. 구현 환경

* OS : Ubuntu 20.04 LTS
* Compiler : GCC 9.4.0
* Thread Library : POSIX Threads (pthread)

---

## 3. Request All Resources

### 해결 방법

Request All Resources 방식은 철학자가 식사를 시작하기 전에 필요한 모든 자원을 한 번에 요청하는 방법이다.

본 구현에서는 semaphore 변수 once를 사용하여 포크를 획득하는 과정을 보호하였다.

철학자는 다음 순서로 동작한다.

1. once semaphore 획득
2. 왼쪽 포크 획득
3. 오른쪽 포크 획득
4. once semaphore 반환
5. 식사 수행
6. 포크 반환

이를 통해 Hold and Wait 조건을 제거하여 Deadlock을 방지할 수 있다.

### 특징

* Deadlock 발생하지 않음
* 구현이 비교적 단순함
* 포크 요청 과정이 직렬화되어 병렬성이 낮음

---

## 4. Resource Ordering

### 해결 방법

Resource Ordering 방식은 자원에 순서를 부여하여 Circular Wait 조건을 제거하는 방법이다.

본 구현에서는

* 철학자 0~3 : 왼쪽 포크 → 오른쪽 포크
* 철학자 4 : 오른쪽 포크 → 왼쪽 포크

순서로 포크를 획득하도록 구현하였다.

모든 철학자가 동일한 방향으로 자원을 요청하지 않기 때문에 원형 대기(Circular Wait)가 발생하지 않으며 Deadlock을 방지할 수 있다.

### 특징

* Deadlock 발생하지 않음
* 별도의 전역 semaphore가 필요하지 않음
* 병렬성이 높음
* 일반적으로 가장 좋은 성능을 보임

---

## 5. Banker's Algorithm

### 해결 방법

Banker's Algorithm은 Deadlock Prevention이 아닌 Deadlock Avoidance 기법이다.

자원을 할당하기 전에 현재 상태가 Safe State인지 검사한 후 안전한 경우에만 자원을 할당한다.

본 구현에서는

* available
* allocation
* max_need

배열을 사용하여 현재 자원 상태를 관리하였다.

철학자가 두 개의 포크를 요청하면 Safe State 여부를 검사하고, 안전한 경우에만 포크를 획득하도록 구현하였다.

이를 통해 Unsafe State로 진입하는 것을 방지하여 Deadlock을 예방할 수 있다.

### 특징

* Deadlock 발생하지 않음
* Safe State를 유지함
* 안전성 검사를 수행하므로 구현이 복잡함
* 추가 연산으로 인해 실행 시간이 증가할 수 있음

---

## 6. 성능 측정 방법

성능 측정을 위해 별도의 성능 측정용 소스 파일을 작성하였다.

성능 측정 시에는 출력문을 제거하여 I/O 오버헤드를 최소화하였다.

각 철학자는 동일한 횟수(100000회)의 식사를 수행하도록 설정하였으며, 프로그램 시작부터 모든 스레드 종료 시점까지의 총 실행 시간을 측정하였다.

실행 시간 측정에는 clock_gettime() 함수를 사용하였다.

또한 eat_count 변수는 여러 스레드가 동시에 접근하는 공유 변수이므로 Race Condition을 방지하기 위해 mutex를 사용하여 상호 배제를 적용하였다.

---

## 7. 성능 측정 결과

| Method                | Execution Time (sec) |
| --------------------- | -------------------- |
| Request All Resources | 측정 결과                |
| Resource Ordering     | 측정 결과                |
| Banker's Algorithm    | 측정 결과                |

---

## 8. 결과 분석

Request All Resources 방식은 Deadlock을 방지할 수 있었지만 포크 요청 과정에서 once semaphore를 사용하기 때문에 자원 요청이 직렬화되었다. 따라서 병렬성이 감소하여 실행 시간이 증가하는 경향을 보였다.

Resource Ordering 방식은 Circular Wait를 제거하면서도 여러 철학자가 동시에 포크를 획득할 수 있기 때문에 병렬성이 가장 높았다. 따라서 세 가지 방법 중 가장 좋은 성능을 보였다.

Banker's Algorithm 방식은 Safe State 검사를 수행하여 Deadlock을 회피하였다. 그러나 자원 요청 시마다 안전 상태를 검사해야 하므로 추가 연산이 발생하였다. 이로 인해 실행 시간이 증가하는 경향을 보였다.

---

## 9. 결론 및 느낀점

Dining Philosophers Problem을 통해 Deadlock이 발생하는 원인과 이를 해결하기 위한 다양한 방법을 학습할 수 있었다.

Request All Resources는 구현이 단순하지만 병렬성이 낮다는 단점이 있었으며, Resource Ordering은 간단한 방법으로 Deadlock을 방지하면서 높은 성능을 제공하였다.

Banker's Algorithm은 가장 복잡한 방법이었지만 Safe State를 유지함으로써 Deadlock을 예방할 수 있음을 확인할 수 있었다.

실험 결과 Deadlock을 해결하는 방법에 따라 시스템의 병렬성과 성능이 달라질 수 있음을 확인할 수 있었으며, 단순히 Deadlock을 방지하는 것뿐만 아니라 성능과 자원 활용 효율 또한 중요하다는 점을 알 수 있었다.

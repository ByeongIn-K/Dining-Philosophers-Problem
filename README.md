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

* Deadlock을 원천적으로 방지할 수 있음
* 필요한 모든 포크를 한 번에 확보한 후 식사를 시작함
* 부분적으로 자원을 점유한 상태에서 대기하는 상황이 발생하지 않음
* 구현이 비교적 단순함
* 철학자가 포크를 모두 확보할 때까지 대기해야 하므로 자원 활용률이 낮아질 수 있음
* 한 철학자가 여러 자원을 장시간 점유할 가능성이 있음
* 동시 식사가 제한되어 병행성이 감소할 수 있음
* 자원 확보 과정에서 대기 시간이 증가할 수 있음

### 장점

* Deadlock 발생 가능성이 없음
* 구현 및 검증이 비교적 쉬움
* 시스템 상태를 예측하기 쉬움

### 단점

* 자원 활용 효율이 낮음
* 동시 작업 수가 감소할 수 있음
* Throughput이 감소할 수 있음

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

* 모든 포크에 고정된 번호를 부여함
* 항상 번호가 작은 포크를 먼저 획득함
* Circular Wait 조건을 제거하여 Deadlock을 방지함
* 추가적인 중앙 관리자가 필요하지 않음
* 자원 사용 효율이 비교적 높음
* 여러 철학자가 동시에 식사할 가능성이 높음
* 구현이 비교적 간단하면서도 효율적임

### 장점

* Deadlock 발생 가능성이 없음
* 자원 활용률이 높음
* 높은 병행성 제공
* 구현이 비교적 간단함
* 추가적인 전역 제어가 거의 필요 없음

### 단점

* 특정 철학자가 반복적으로 대기할 가능성이 있음
* 공정성(Fairness)을 보장하지 못함
* Starvation 가능성이 존재함

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

* 자원 요청 시 안전 상태(Safe State)를 검사함
* 안전한 경우에만 자원을 할당함
* Unsafe State가 예상되면 자원 할당을 거부함
* Deadlock을 예방할 수 있음
* 시스템 전체 상태를 고려하여 의사결정을 수행함
* 가장 보수적인 방식의 자원 관리 방법임

### 장점

* Deadlock 회피 가능
* 안전 상태(Safe State)를 유지하면서 자원 할당 가능
* 이론적으로 매우 안정적인 방법
  
### 단점

* 구현이 가장 복잡함
* 안전 상태 검사 비용이 발생함
* 추가적인 계산 오버헤드 존재
* 자원 활용률이 낮아질 수 있음

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
| Request All Resources | 0.096219               |
| Resource Ordering     | 0.034355               |
| Banker's Algorithm    | 0.410697               |

---

## 8. 결과 분석

Request All Resources 방식은 Deadlock을 방지할 수 있었지만 포크 요청 과정에서 once semaphore를 사용하기 때문에 자원 요청이 직렬화되었다. 따라서 병렬성이 감소하여 실행 시간이 증가하는 경향을 보였다.

Resource Ordering 방식은 Circular Wait를 제거하면서도 여러 철학자가 동시에 포크를 획득할 수 있기 때문에 병렬성이 가장 높았다. 따라서 세 가지 방법 중 가장 좋은 성능을 보였다.

Banker's Algorithm 방식은 Safe State 검사를 수행하여 Deadlock을 회피하였다. 그러나 자원 요청 시마다 안전 상태를 검사해야 하므로 추가 연산이 발생하였다. 이로 인해 실행 시간이 증가하는 경향을 보였다.

---

## 9. 결론 및 느낀점

이를 통해 Dining Philosophers Problem에서 Deadlock 방지가 얼마나 중요한 문제인지 확인할 수 있었다.

Request All Resources 방식은 가장 직관적인 해결 방법으로 Deadlock을 완전히 방지할 수 있었지만, 모든 자원을 한 번에 확보해야 하므로 자원 활용률이 낮아질 수 있었다.

Resource Ordering 방식은 포크에 순서를 부여하여 Circular Wait 조건을 제거하는 방법으로 구현이 단순하면서도 높은 병행성을 제공하였다. 실험 결과 세 가지 방법 중 가장 효율적인 방식으로 동작하는 경우가 많았다.

Banker's Algorithm 방식은 시스템의 안전 상태를 지속적으로 검사함으로써 Deadlock을 예방할 수 있었지만, 추가적인 계산 과정으로 인해 구현 복잡도가 높고 실행 오버헤드가 발생하였다.

실험 결과를 통해 Deadlock을 해결하는 방법은 여러 가지가 존재하지만, 각 방법은 자원 활용률, 구현 복잡도, 공정성(Fairness), 실행 성능 측면에서 서로 다른 특성을 가진다는 점을 확인할 수 있었다.

또한 단순히 Deadlock을 제거하는 것만이 아니라 시스템 전체의 효율성과 공정성을 함께 고려해야 한다는 점을 배울 수 있었다.

이번 과제를 수행하면서 Deadlock Prevention, Deadlock Avoidance 기법에 대한 이해를 높일 수 있었으며, 운영체제에서 동기화 기법이 왜 중요한지 직접 확인할 수 있었다.

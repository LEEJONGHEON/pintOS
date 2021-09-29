# pintOS

## Problem 1 기차문제(Thread을 이용한 동기화문제)
Thread 를 생성하며 Mutex 방식으로 lock을 사용하여 critical section을 관리함
Mutex 조건문을 이용하여 thread lock과 unlock을 시킴
pthread_cond_signal(&cond) : signal을 통해 &cond 를 기다리는 pthread_cond_wait을  깨우는 역활을함. 
pthread_cond_wait(&cond) : cond 신호가 들어올동안 Thread 차단
pthread_cond_broadcast(&cond) : cond 신호를 기다리는 모든 Thread 를 깨움

ex)
Thread 1
lock() // critical section
if(조건문) pthread_cond_wait(&cond) // &cond가 들어올때까지 thread 중지
unlokc() // critical section

Thread 2
lock()
pthread_cond_signal(&cond) // &cond신호를 기다리는 Thread를 깨움
unlock()

### station_load_train
기차가 역을 도착하여 승객을 이 전부탑승할때까지 기다리는 Thread
남은좌석이없을 경우 출발하고 그전까지 Thread 대기상태


### station_wait_for_train
승객이 역에 도착해서 기차를 기다리는경우
기다리는 승객은 기차에 탑승함 탑승수 증가
만약 탑승객이 == 남아있는 사람수와 같아질경우
해당 Thread는 대기상태

### station_on_board
기차에 승객이 앉는 Thread
좌석수 감소, 탑승수 감소
만약 기차에 좌석이 0 이거나 탑승하는 승객이 0 일경우 새로운 기차를 호출함 cond_signal로  cond_leave 기차 출발신호를 보냄


## Problem 2 물만들기문제
H2 +  O -> Water 되는데
h2와 o가 만나서 water을 만들도록 Thread을 활용

### reaction_h
h 생성하는과정 h하나를생성
만약 h가 2개이상이면 o를 생성하여 물을 제작가능하므로 o에 signal을 보내 water 제작을준비함
h가 2개이하면 h가 더생산이필요하므로 o에게서 h제작 signal을 wait함

### reaction_o
o 를 생성하는과정 o를 하나생성
만약 h가 2개이상인경우 water 을 생산한뒤 h를 생산하는 signal을 두번보냄(다음번 물생산을위해)
h가 2개이하인경우 h로 h 필요하다는 signal을보냄




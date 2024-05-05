# 폭탄 찾기 게임

#### 사용 언어
C

###### 사용 라이브러리(헤더 파일)
stdio, stdlib, windows

---
## 작동 설명

#### 1. 초기 설정
정사각형 지도의 크기(4~26)를 입력한다.  
이후, 지도 내 폭탄의 갯수(전체 지역의 1/2 이하)를 입력한다.  

#### 2. 게임 시작
지도의 특정 지역의 좌표(Row, Column)를 입력 시, 해당 지역을 개방하며 정보를 알려 준다.  
미개방 지역의 입력값 뒤에 !를 입력 시, 해당 지역에 깃발을 설치하거나 철거할 수 있다.  
해당 지역에 폭탄이 있는 경우, 게임오버된다.  
해당 지역 주변에 폭탄이 있는 경우, 그 갯수를 알려준다.  
해당 지역 주변에 폭탄이 없는 경우, 폭탄이 그 주변에 있는 지역까지 개방 지역을 확장한다.  

---
## 기능
첫 번째 선택지와 그 주변에는 폭탄이 없다.  
이는 Flag 설정 시에도 동일하게 처리되니, 첫번째 선택에서는 Flag 설정을 하면 안된다.  
남은 찾을 폭탄의 갯수를 지도 아래에 표시한다.  

---
작성자 : YHC03  
최종 작성일 : 2024/5/5  
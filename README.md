# pieberry
pieberry bot (C++/Lua slackbot)

Project 파이베리 봇
개발자 Neostage


#Definition
1. slackbot이다
2. c++로 기반 프로그램을 작성한다 (http, socket, file i/o, lua머신)
3. bot ai는 lua로 작성한다

#Target spec
※ Lua 스크립팅만으로 자유롭게 봇을 만들 수 있는 프로그램
1. Windows/Ubuntu 지원
2. lua와 설정파일을 교체하면 다른 봇을 만들 수 있다
 -> 복수의 set을 이용해 복수의 bot을 운용할 수 있다
3. 콘솔 프로그램으로 실시간 모니터링/간섭 가능



#Version spec
##[v0.1]
http 연결, rtm.start 가능
websocket 연결, send 가능
콘솔 프로그램 상에서 자유롭게 메시지 보내기 가능

-------------done---------------
undone

##[v0.2]
파일 분리
 * action -> http 액션, send 액션 등 단일 기능의 단위 / 콘솔입력 및 메시지 응답 시 자주 사용될 것들
 * console -> 콘솔 인터페이스 처리
   . lua에서 정의한 것을 콘솔 명령어로 호출하는 것은 v0.3
 * response -> Slack 메시지 응답 처리
   . lua 처리는 v0.3
 * console_cmd -> 콘솔 명령어 처리
멀티스레드
 * 콘솔과 메시지 응답 스레드 분리
 

##[v0.3]
Lua 적용
 * Lua 로드 / 호출
Multibot configuration
 * 여러 bot을 가동할 수 있고 console을 이용해 중앙감독
 * 각각의 bot 설정(ai집합, 명령집합 등등)을 파일로 관리
클래스화
 * console
 * bot머신


##[v0.4]
web api 적용
 * action확장(rtm.start 이외의 것 지원)
콘솔 명령어set 개편
Lua 구조 정리


##[v0.5]
파이베리 구현

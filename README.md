# Auto Assignment Grading Program Using Shell Scripts - Release

DGIST CSE-201 Object Oriented Programming 수업 Assignment 채점에 사용되는 자동 채점 프로그램입니다.

bash를 이용하는 쉘 스크립트로 개발되었습니다. 리눅스 환경에서 동작합니다.

*Assignment 1* 릴리즈 버전입니다. 

자세한 코드 설명이 필요하다면 이 프로그램의 [샘플 버전 리포지토리](https://github.com/saychuwho/auto_grading)를 참조해주세요.

버그가 있다면 이슈 트래커로 남겨주시거나, saychuwho@dgist.ac.kr로 메일을 보내주시거나, 톡방, 디스코드에 알려주세요.

## Tables

- [Auto Assignment Grading Program Using Shell Scripts - Release](#auto-assignment-grading-program-using-shell-scripts---release)
  - [Tables](#tables)
  - [How to use](#how-to-use)
  - [Bug Reported](#bug-reported)

## How to use

다음 파일을 참고해주세요. [howtouse.md](./howtouse.md) / [howtouse.pdf](./howtouse.pdf)

## Bug Reported

- 2024.09.25
  - 채점 파일 제목에 공백이 있는 경우 오작동을 해 조치함.
  - 간혹 "zero width no-break space"가 소스코드에 들어가서 컴파일 오류를 발생하는 문제가 있었음. 
    - 컴파일 용 파일을 만들 때 이 문자를 소스 코드에서 없애도록 함
  - 학생 목록을 2024.09.25 기준으로 최신화함
  - `_report_print.sh`에 정규식이 잘못되어서 오작동 하는 사례가 있어서 해결함
  - 1번 문제 채점에 이제 `diff`에서 끝 부분 공백과 끝 부분 EOL을 무시하도록 하는 옵션을 넣음
- 2024.09.20
  - 소스코드 제출만 해도 제공되는 기본점수를 `result_score.csv`에 반영해야 한다.
    - `_result_score.py`를 수정해 반영함
  - EOL 때문에 실제로는 올바른 결과가 나오더라도 fail 처리되는 경우가 있다. 
    - 각 문제별 정답 case를 EOL이 있는 버전과 없는 버전 두 개로 만들어, 둘 중 하나만 통과하더라도 pass를 받을 수 있도록 `run.sh`의 4번 부분을 바꿈.
  - (단순 기능추가) 파일 형식을 지키지 않은 채로 제출하는 경우가 있었다.
    - 적어도 학번이 파일 이름에 있다면, zip 파일의 이름과 제출한 소스코드의 이름을 제출 파일 이름 형식에 맞도록 전처리 한 후 채점하도록 기능을 추가했다.
  - (단순 기능추가) report 마크다운 파일 안에 너무 많은 정보들이 담겨있어서 찾기 힘들었다. 
    - report 파일을 summary, submit_compile, output_diff 세 부분으로 나누어 파일 세개로 나누도록 기능을 추가했다. 
- 2024.09.19
  - 학생들이 폴더를 통째로 압축해서 압축 해제하면 폴더가 있고 그 안에 제출물이 있는 경우가 있음
    - 폴더의 이름이 제출 형식과 동일한 경우는 해결함
  - 헤더 파일을 포함 안 한 채로 제출한 경우가 있음
    - `grading_cases`에 각 문제 별 필요한 헤더를 `hw1_문제번호_header.cpp`와 같은 형식으로 추가하고, 헤더가 없는 경우에는 이 헤더를 추가해서 컴파일하도록 수정함
  - 제출한 학생의 리포트랑 제출 안한 학생의 리포트를 별도의 폴더로 풀어서 관리함
  - 3번 문제의 case 2에서 컴파일 오류 문제를 발견해서 해결함
  - `python`을 실행할 때 `python3`로 실행하도록 해 파이썬 실행 문제가 없도록 함
# Auto Assignment Grading Program Using Shell Scripts - Release

DGIST CSE-201 Object Oriented Programming 수업 Assignment 채점에 사용되는 자동 채점 프로그램입니다.

bash를 이용하는 쉘 스크립트로 개발되었습니다. 리눅스 환경에서 동작합니다.

*Assignment 3* 버전입니다.

자세한 코드 설명이 필요하다면 이 프로그램의 [샘플 버전 리포지토리](https://github.com/saychuwho/auto_grading)를 참조해주세요.

버그가 있다면 이슈 트래커로 남겨주시거나, saychuwho@dgist.ac.kr로 메일을 보내주시거나, 톡방, 디스코드에 알려주세요.

## Tables

- [Auto Assignment Grading Program Using Shell Scripts - Release](#auto-assignment-grading-program-using-shell-scripts---release)
  - [Tables](#tables)
  - [Releases](#releases)
  - [How to use](#how-to-use)
  - [Bug Reported \& Feature added](#bug-reported--feature-added)

## Releases

현재까지 릴리즈한 버전은 다음과 같습니다. 

- 2024-CSE201-Assignment-1 : 2024 DGIST OOP Assignment 1
- 2024-CSE202-Assignment-2 : 2024 DGIST OOP Assignment 2
- 2024-CSE202-Assignment-3 : 2024 DGIST OOP Assignment 3

## How to use

다음 파일을 참고해주세요. [howtouse.md](./howtouse.md) / [howtouse.pdf](./howtouse.pdf)

## Bug Reported & Feature added

- 2024.11.16
  - utf-8 encoding 관련 버그를 고쳤습니다.
- 2024.11.14
  - Major Update: 부분적으로 multiprocessing을 적용했습니다: `all` mode와 `regrade` mode에서 unzip, change file name 부분 이외의 부분
  - `log_delete` mode 구현: log file들을 지웁니다.
  - case 1이 test case이고, case 2가 기본 test case입니다.
  - 컴파일 규칙을 만드는 데 난해함이 있어서, 이번에는 학생이 제출한 코드에서 헤더 파일과 main 함수를 제거한 뒤 통째로 컴파일하도록 로직을 수정했습니다.
- 2024.10.25
  - Major Update: shell script에서 python script로 코드를 뜯어 고쳤습니다. 
  - Assignment 2 Release 버전을 출시했습니다. 
  - 이전 Assignment 1과 달라진 기능은 다음과 같습니다.
    - MOSS 결과를 실행할 때마다 뽑는 것이 아니라, 별도의 `MOSS` 옵션을 실행해야 뽑는 것으로 바꿈.
    - `regrade` 옵션에서 다시 채점할 학생 목록을 자동으로 만들도록 바꿈.
    - `run_output` 옵션을 만들어서 컴파일 된 학생의 결과물을 수동으로 실행할 수 있도록 만듬.
- 2024.10.02
  - (기능추가) `run_extra.sh`를 여러 학생들을 받아서 처리할 수 있도록 수정함. 기존 `run_extra.sh`는 `_run_extra_onlyone.sh`로 넘어감. `student_list_regrade.txt`에 다시 채점하고 싶은 학생의 학번을 넣으면 채점할 수 있음.
  - g++ 컴파일 옵션에 warning을 끔.
  - (기능추가) hw1 한정으로 재귀함수를 찾을 수 있는 logic을 도입함. 이 logic은 학생의 제출물에 함수 이름이 두번 이상 사용되었는지 여부로 재귀함수 사용 여부를 판단한다.
  - (기능추가) 학번, 문제를 입력하면 테스트케이스별로 컴파일 된 프로그램을 실행하는 `run_output.sh`를 추가함
- 2024.09.28 
  - `student_list.txt`를 정렬해서 나중에 폴더를 찾을 때 쉽게 찾을 수 있도록 만듬
  - 잘못 제출한 파일을 올바른 파일 이름 포맷으로 바꾸는 파이썬 스크립트를 새로 작성함. 올바른 파일을 찾는 기준은 제출한 소스코드 속에 제출 형식에 맞는 함수가 있는지 확인하는 방법을 이용함. `hw_pattern.json`에 패턴 내용이 담겨있음
  - macOS 파일 시스템에 존재하는 `MACOSX` 폴더 관련한 문제를 해결함
  - 학생이 짠 프로그램이 무한루프에 빠지는 경우가 있어서, 10초 이내에 반응이 없는 경우에 프로그램을 종료하도록 로직을 수정함
  - 진행상황 바 옆에 학번 또는 문제 케이스를 같이 출력하도록 해 진행 상황이 매끄럽게 보이도록 함.
- 2024.09.27
  - `grep`을 사용할 때 예외사항이 발생해 수정함.
  - (기능추가) 이제 일일이 학생의 리포트를 살필 필요 없이 컴파일 에러나 테스트 케이스 통과 실패 여부를 확인할 수 있는 파일을 채점 프로그램이 생성합니다. 자세한 내용은 `howtouse.pdf`의 6번 항목을 참고해주세요
  - (기능추가) 이제 학생들의 제출물 간 유사도를 측정할 수 있습니다. [MOSS](https://theory.stanford.edu/~aiken/moss/)를 이용하며, 유사도 측정 결과는 `result_moss.md`에 링크로 나옵니다. 지금 과제는 간단해서 이용하는 의미가 없을 수 있지만, 추후 복잡한 과제에서는 유용할 수 있습니다.
  - (기능추가) 새로운 기능 추가에 맞춰서 `setup.sh`, `howtouse.md`, `howtouse.pdf`를 업데이트 했습니다.
- 2024.09.26
  - 학생이 압축 파일 속에 폴더를 만든 후 소스코드를 넣은 경우를 처리하는 logic에서 이름 관련 오류가 발견되어 수정함.
  - 학생의 소스코드를 찾는 정규표현식에서 오로지 `.cpp` 파일만 찾을 수 있도록 logic을 수정함
  - (기능추가) 현재 학생 수에 비례해서, `./run.sh`의 각 과정에 progress bar를 추가함. 
  - (기능추가) `./setup.sh`를 추가해 초기 설정(실행권한 주기, `dos2unix`를 이용한 파일 형식 변환)을 하도록 만듬
  - (기능추가) 이제 학생이 제출한 코드에 `main()`이 포함되어 있으면, 이를 제거하고 채점을 진행합니다.
  - (기능추가) `./run_extra.sh`를 추가했습니다. 이 스크립트는 `./run.sh`를 실행한 이후, 특정 학생이 제출한 소스코드를 수정한 이후 해당 학생만 별도로 채점할 때 사용하면 됩니다.
  - (기능추가) 추가된 기능이 여럿 있어서 `howtouse.md`와 `howtouse.pdf`를 업데이트 했습니다.
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
- 2024.09.19
  - 학생들이 폴더를 통째로 압축해서 압축 해제하면 폴더가 있고 그 안에 제출물이 있는 경우가 있음
    - 폴더의 이름이 제출 형식과 동일한 경우는 해결함
  - 헤더 파일을 포함 안 한 채로 제출한 경우가 있음
    - `grading_cases`에 각 문제 별 필요한 헤더를 `hw1_문제번호_header.cpp`와 같은 형식으로 추가하고, 헤더가 없는 경우에는 이 헤더를 추가해서 컴파일하도록 수정함
  - 제출한 학생의 리포트랑 제출 안한 학생의 리포트를 별도의 폴더로 풀어서 관리함
  - `python`을 실행할 때 `python3`로 실행하도록 해 파이썬 실행 문제가 없도록 함


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
  - [Troubleshooting](#troubleshooting)
  - [Bug Reported](#bug-reported)

## How to use

1. 각 assignment에 맞는 release를 다운로드 받은 후, 압축해제를 합니다. 또는, 해당 과제의 branch로 전환한 다음 해당 branch를 clone해도 됩니다.

2. `git`, `zip`, `unzip` 그리고 `dos2unix`를 설치합니다.
```bash
sudo apt install git zip unzip dos2unix
```

3. 스크립트에 실행 권한을 다음과 같이 줍니다.
```bash
chmod 755 ./run.sh
chmod 755 ./reset.sh
chmod 755 ./_report_print.sh
```

4. 채점하고자 하는 학생들의 `.zip` 파일을 `./student_submission` 폴더에 넣습니다.
5. `./run.sh`을 실행합니다.
6. `./run.sh`을 다시 실행하려면, `./reset.sh`을 실행 한 뒤 다시 `./run.sh`를 실행하면 됩니다. 

## Troubleshooting

만약 /bin/bash^M을 찾을 수 없다는 오류 메시지가 나오거나, 초반부에 hw information을 말하는 부분에서 syntax error가 나온다면, 다음 파일들의 End Of Line을 CRLF에서 LF로 바꾸어야 제대로 동작합니다. 이를 위해서는 다음 명령어를 실행해줍니다.

```bash
dos2unix ./run.sh
dos2unix ./reset.sh
dos2unix ./student_list.txt
dos2unix ./_report_print.sh
dos2unix ./result_score.py
```

## Bug Reported

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
# Auto Assignment Grading Program Using Shell Scripts - Release

DGIST CSE-201 Object Oriented Programming 수업 Assignment 채점에 사용되는 자동 채점 프로그램입니다.

bash를 이용하는 쉘 스크립트로 개발되었습니다. 리눅스 환경에서 동작합니다.

릴리즈 버전입니다. 

자세한 코드 설명이 필요하다면 이 프로그램의 [샘플 버전 리포지토리](https://github.com/saychuwho/auto_grading)를 참조해주세요.

버그가 있다면 이슈 트래커로 남겨주시거나, saychuwho@dgist.ac.kr로 메일을 보내주시거나, 톡방, 디스코드에 알려주세요.

## Tables

- [Auto Assignment Grading Program Using Shell Scripts - Release](#auto-assignment-grading-program-using-shell-scripts---release)
  - [Tables](#tables)
  - [How to use](#how-to-use)

## How to use

1. 각 assignment에 맞는 release를 다운로드 받은 후, 압축해제를 합니다.

2. `git`, `zip`, and `unzip`을 설치합니다.
```bash
sudo apt install git zip unzip
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
